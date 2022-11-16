#define _POSIX_C_SOURCE 200112L
#include "socket-handler.h"
// GET src/main.c HTTP/1.1

static volatile bool runserver = true;
#define MAX_EVENT 10000
int create_and_bind(char *ip, char *port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *list;
    int error = getaddrinfo(ip, port, &hints, &list);

    if (error == -1)
        return -1;

    struct addrinfo *cursor;
    int sockfd;

    for (cursor = list; cursor != NULL; cursor = cursor->ai_next)
    {
        sockfd =
            socket(cursor->ai_family, cursor->ai_socktype, cursor->ai_protocol);
        if (sockfd == -1)
            continue;
        if (bind(sockfd, cursor->ai_addr, cursor->ai_addrlen) != -1)
            break;
        close(sockfd);
    }

    if (cursor == NULL)
        return -1;
    freeaddrinfo(list);

    return sockfd;
}

size_t get_index_from_buff(size_t begin, size_t end, char *buff, char delim)
{
    for (size_t i = begin; i < end; i++)
    {
        if (buff[i] == delim)
            return i;
    }

    return end;
}

void signal_handler_term(int sig)
{
    // useless operation == (void) sig
    sig++;
    runserver = false;
}

void setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;

    fcntl(fd, F_SETFL, new_option);
}

int socket_handler(char *ip, char *port, struct servconfig *server)
{
    // signal handler
    signal(SIGTERM, signal_handler_term);

    printf("\n+++++++ Waiting for new connection ++++++++\n\n");

    int listening_sock = create_and_bind(ip, port);

    if (listening_sock == -1)
        return 2;

    setnonblocking(listening_sock);

    if (listen(listening_sock, 30) == -1)
        return 2;

    //	creation of epoll_instance
    struct epoll_event event;
    struct epoll_event events[MAX_EVENT];

    int fd_server = 0;

    int epoll_fd = epoll_create1(0);

    if (epoll_fd == -1)
        return 2;

    event.events = EPOLLIN | EPOLLOUT | EPOLLET;
    event.data.fd = fd_server;

    // add server to interrest list
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_server, &event) == -1)
        return 2;

    while (runserver)
    {
        // gestion de la liste d'interet
        int clientready = epoll_wait(epoll_fd, events, MAX_EVENT, -1);
        if (clientready == -1)
            return 2;

        for (int i = 0; i < clientready; i++)
        {
            if (events[i].data.fd == listening_sock)
            {
                int client_socket = accept(listening_sock, NULL, NULL);
                if (client_socket == -1)
                    continue;

                // Ajout du client dans la liste d'interet
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event)
                    == -1)
                    return 2;
            }
            else
            {
                ssize_t nb_read;
                size_t nb_sent;
                size_t total_sent = 0;
                size_t total_read = 0;
                char buff[1024];

                while (true)
                {
                    nb_read =
                        recv(events[i].data.fd, buff + total_read, 1024, 0);
                    if (nb_read == -1)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        else
                        {
                            close(events[i].data.fd);
                            continue;
                        }
                    }
                }

                struct request_info *request_info =
                    parser_request(buff, server->vhosts);

                event.events = EPOLLOUT;

                if (server->global.log)
                {
                    // log de la request
                    log_request(server->vhosts->servername, request_info, ip,
                                server->global.logfile);

                    // log de la reponse
                    log_response(server->vhosts->servername, "400", ip,
                                 request_info, server->global.logfile);
                }

                struct response_info *response_info =
                    parser_response(request_info, server->vhosts);

                size_t lenbuff = strlen(response_info->statusline);

                while ((nb_sent = send(events[i].data.fd,
                                       response_info->statusline + total_sent,
                                       lenbuff - total_sent, MSG_NOSIGNAL))
                       > 0)
                {
                    total_sent += nb_sent;
                }

                if (strcasecmp(request_info->method, "GET") == 0
                    && strcasecmp(response_info->statuscode, "200") == 0)
                {
                    int fd = open(response_info->path, O_RDONLY);
                    if (fd == -1)
                        return 2;

                    sendfile(events[i].data.fd, fd, 0, 100000);
                    close(fd);
                }
                close(events[i].data.fd);
            }
        }
    }
    free_server(server);

    close(epoll_fd);
    return 0;
}
