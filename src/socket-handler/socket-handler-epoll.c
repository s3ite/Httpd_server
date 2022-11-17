#include <stdio.h>

#include "socket-handler.h"
// GET src/main.c HTTP/1.1

static volatile bool runserver = true;
#define MAX_EVENT 10000
#define MAX_READ 100000
#define BUFFER_SIZE 1024

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

void set_non_blocking(int fd)
{
    int oldoption = fcntl(fd, F_GETFL);
    int newoption = oldoption | O_NONBLOCK;

    fcntl(fd, F_SETFL, newoption);
}

int socket_handler(char *ip, char *port, struct servconfig *server)
{
    // signal handler
    signal(SIGTERM, signal_handler_term);

    printf("\n+++++++ Waiting for new connection ++++++++\n\n");

    int listeningsock = create_and_bind(ip, port);

    if (listeningsock == -1)
        return 2;

    set_non_blocking(listeningsock);

    if (listen(listeningsock, 30) == -1)
        return 2;

    //	creation of epoll_instance

    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    struct epoll_event events[MAX_EVENT];

    int epollfd = epoll_create1(0);

    if (epollfd == -1)
        return 2;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = listeningsock;

    // add server to interrest list
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listeningsock, &event) == -1)
        return 2;

    while (runserver)
    {
        printf("server ready\n");
        struct epoll_event tmpevent;
        //  gestion de la liste d'interet
        int nfds = epoll_wait(epollfd, events, MAX_EVENT, -1);
        if (nfds == -1)
            return 2;

        printf("nfds = %d\n", nfds);

        for (int i = 0; i < nfds; i++)
        {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
                || (!(events[i].events & EPOLLIN)))
            {
                printf("ERROR\n");
                // error case
                close(events[i].data.fd);
                continue;
            }
            else if (events[i].data.fd == listeningsock)
            {
                for (;;)
                {
                    struct sockaddr in_addr;
                    socklen_t in_addr_len = sizeof(in_addr);
                    int clientsocket =
                        accept(listeningsock, &in_addr, &in_addr_len);

                    printf("listeningsock = %d\n", clientsocket);
                    if (clientsocket == -1)
                        break;

                    else
                    {
                        printf("accepted new connection on fd %d\n",
                               clientsocket);
                        set_non_blocking(clientsocket);

                        event.data.fd = clientsocket;
                        event.events = EPOLLIN | EPOLLET;

                        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clientsocket,
                                      &tmpevent)
                            == -1)
                            return 2;
                    }
                }
                printf("%s\n", "next step");
            }
            else
            {
                ssize_t nbread;
                size_t nbsent;
                size_t totalsent = 0;
                char buff[BUFFER_SIZE];
                printf("%s\n", "receiving request");

                while (true)
                {
                    nbread = read(events[i].data.fd, buff, BUFFER_SIZE);
                    printf("nbread = %ld\n", nbread);
                    if (nbread == -1)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        else
                        {
                            close(events[i].data.fd);
                            break;
                        }
                    }
                    else if (nbread == 0)
                        break;
                }

                printf("%s\n", "data received");
                if (nbread == -1)
                {
                    close(events[i].data.fd);
                    continue;
                }
                struct request_info *request_info =
                    parser_request(buff, server->vhosts);

                event.events = EPOLLOUT;

                // ===========================LOG===============================
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

                while ((nbsent = send(events[i].data.fd,
                                      response_info->statusline + totalsent,
                                      lenbuff - totalsent, MSG_NOSIGNAL))
                       > 0)
                {
                    totalsent += nbsent;
                }

                if (strcasecmp(request_info->method, "GET") == 0
                    && strcasecmp(response_info->statuscode, "200") == 0)
                {
                    int fd = open(response_info->path, O_RDONLY);
                    if (fd == -1)
                        return 2;

                    sendfile(events[i].data.fd, fd, 0, MAX_READ);
                    close(fd);
                }
                close(events[i].data.fd);
            }
        }
    }
    free_server(server);

    close(epollfd);
    return 0;
}
