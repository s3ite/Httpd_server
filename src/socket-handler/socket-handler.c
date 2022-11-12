#define _POSIX_C_SOURCE 200112L
#include "socket-handler.h"

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

int socket_handler(char *ip, char *port, struct servconfig *server)
{
    printf("\n+++++++ Waiting for new connection ++++++++\n\n");

    int listening_sock = create_and_bind(ip, port);

    if (listening_sock == -1)
        return 2;

    if (listen(listening_sock, 30) == -1)
        return 2;

    while (1)
    {
        //  signal catch
        // signal(SIGINT, (void (*)(int))stop_server);

        int client_socket = accept(listening_sock, NULL, NULL);
        if (client_socket == -1)
            continue;
        ssize_t nb_read;
        char buff[512];
        size_t total_read = 0;

        while ((nb_read =
                    recv(client_socket, buff + total_read, 512 - total_read, 0))
               > 0)
        {
            size_t index = get_index_from_buff(total_read, total_read + nb_read,
                                               buff, '%');
            if (index != total_read + nb_read)
            {
                total_read = index;
                break;
            }
            total_read += nb_read;
        }

        buff[total_read - 1] = '\0';

        if (nb_read == -1)
        {
            close(client_socket);
            continue;
        }

        struct request_info *request_info = parser_request(buff);

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
            parser_response(request_info, server);

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

        if (strcmp(request_info->method, "GET") == 0
            && strcmp(response_info->statuscode, "200") == 0)
        {
            int fd = open(response_info->path, O_RDONLY);
            if (fd == -1)
                return 2;

            sendfile(client_socket, fd, 0, 100000);

            close(fd);
        }

        close(client_socket);
    }
    return 0;
}

// GET src/main.c HTTP/1.1 %