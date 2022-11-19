#define _POSIX_C_SOURCE 200112L
#include "socket-handler.h"

// GET src/main.c HTTP/1.1
// GEt HTTP/1.1
// HEAD src/main.c HTTP/1.1
// Head HTTP/1.1
static volatile bool runserver = true;
#define BUFFER_SIZE 32000

static int create_and_bind(char *ip, char *port)
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

static size_t get_index_from_buff(size_t begin, size_t end, char *buff,
                                  char delim)
{
    for (size_t i = begin; i < end; i++)
    {
        if (buff[i] == delim)
            return i;
    }

    return end;
}

static void signal_handler_term(int sig)
{
    // useless operation == (void) sig
    sig++;
    runserver = false;
}

static char *recv_data(int socket, char *buffer)
{
    size_t total_read = 0;
    ssize_t nb_read = 0;
    while ((nb_read =
                recv(socket, buffer + total_read, BUFFER_SIZE - total_read, 0))
           > 0)
    {
        size_t index =
            get_index_from_buff(total_read, total_read + nb_read, buffer, '\n');

        if (index != total_read + nb_read)
        {
            total_read = index;
            break;
        }
        total_read += nb_read;
    }

    return buffer;

    if (nb_read == -1)
    {
        close(socket);
        return NULL;
    }
}

int socket_handler(char *ip, char *port, struct servconfig *server)
{
    // signal handler
    signal(SIGTERM, signal_handler_term);

    // printf("\n+++++++ Waiting for new connection ++++++++\n\n");

    int listening_sock = create_and_bind(ip, port);

    if (listening_sock == -1)
        return 2;

    if (listen(listening_sock, 30) == -1)
        return 2;

    while (runserver)
    {
        int client_socket = accept(listening_sock, NULL, NULL);
        if (client_socket == -1)
            continue;

        char *bufferstatusline = malloc(BUFFER_SIZE);
        bufferstatusline = recv_data(client_socket, bufferstatusline);

        // char *bufferheader = malloc(BUFFER_SIZE);
        // bufferheader = recv_data(client_socket, bufferheader);

        // printf("%s\n%s", bufferstatusline, bufferheader);

        struct request_info *request_info =
            parser_request(bufferstatusline, server->vhosts);

        if (server->global.log)
        {
            // log de la request
            log_request(server->vhosts->servername, request_info, ip,
                        server->global.logfile);

            // log de la reponse
            log_response(server->vhosts->servername, "400", ip, request_info,
                         server->global.logfile);
        }

        struct response_info *response_info =
            parser_response(request_info, server->vhosts);

        size_t lenbuff = strlen(response_info->statusline);
        size_t nb_sent;
        size_t total_sent = 0;
        while ((nb_sent =
                    send(client_socket, response_info->statusline + total_sent,
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

            sendfile(client_socket, fd, 0, 100000);

            close(fd);
        }

        close(client_socket);
    }
    free_server(server);
    return 0;
}
