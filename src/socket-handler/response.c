#include "response.h"

// connect to socket and send the response to the client
struct response_info *parser_response(struct request_info *request,
                                      struct servconfig *server)
{
    struct response_info *response_info = malloc(sizeof(struct response_info));

    response_info->statusline = malloc(100);

    response_info->statusline[0] = '\0';
    char *statuscode = "";

    time_t timestamp = time(NULL);
    struct tm *pTime = localtime(&timestamp);
    char date[100];
    strftime(date, 100, "Date : %a, %d %b %G %T GMT\n\n", pTime);
    // printf("data : %s", date);

    printf("%s", request->version);
    if (!request)
    {
        response_info->statuscode = "400";
        statuscode = "400 Bad Request";
        strcat(response_info->statusline, statuscode);
        return response_info;
    }

    if (strcmp(request->method, "GET") != 0
        && strcmp(request->method, "HEAD") != 0)
    {
        response_info->statuscode = "405";
        statuscode = "405 Method not allowed\n";
    }

    else if (strcmp(request->version, "HTTP/1.1") != 0)
    {
        response_info->statuscode = "505";
        statuscode = "505 HTTP Version Not Supported\n";
    }

    else
    {
        char *path = malloc(100);
        *path = '\0';
        strcat(path, server->vhosts->rootdir);
        strcat(path, request->target);
        response_info->path = path;

        printf("%s", path);
        // test de d'accessibilite de droit d'ouverture de fichier
        int correctfile = access(path, R_OK);
        if (correctfile == 0)
        {
            response_info->statuscode = "200";
            statuscode = "200 OK\n";
        }
        else if (correctfile == -1 && errno == ENOENT)
        {
            response_info->statuscode = "404";
            statuscode = "404 Not Found\n";
        }
        else
        {
            response_info->statuscode = "403";
            statuscode = "403 Forbidden\n";
        }
    }

    strcat(response_info->statusline, "HTTP/1.1 ");
    strcat(response_info->statusline, statuscode);
    strcat(response_info->statusline, date);

    return response_info;
}
