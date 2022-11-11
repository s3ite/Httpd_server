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
    freeaddrinfo(list);

    if (cursor == NULL)
        return -1;

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
        return 3;

    while (1)
    {
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
            log_request("localhost", request_info, ip, server->global.logfile);

            // log de la reponse
            log_response("localhost", "400", ip, request_info,
                         server->global.logfile);
        }        char *bufferresponse = status_line(request_info, server);

        size_t lenbuff = strlen(bufferresponse);
        size_t nb_sent;
        size_t total_sent = 0;
        while ((nb_sent = send(client_socket, bufferresponse + total_sent,
                               lenbuff - total_sent, MSG_NOSIGNAL))
               > 0)
        {
            total_sent += nb_sent;
        }
        close(client_socket);
    }
    return 0;
}
