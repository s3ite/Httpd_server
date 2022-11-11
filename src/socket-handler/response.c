#include "response.h"

// connect to socket and send the response to the client
char *status_line(struct request_info *request, struct servconfig *server)
{
    char *statusline = malloc(100);
    *statusline = '\0';
    char *statuscode = "";

    time_t timestamp = time(NULL);
    struct tm *pTime = localtime(&timestamp);
    char date[100];
    strftime(date, 100, "Date : %a, %d %b %G %T GMT\n", pTime);
    // printf("data : %s", date);

    printf("%s", request->version);
    if (!request)
    {
        statuscode = "400 Bad Request";
        strcat(statusline, statuscode);
        return statusline;
    }

    if (strcmp(request->method, "GET") != 0
        && strcmp(request->method, "HEAD") != 0)
        statuscode = "405 Method not allowed\n";

    else if (strcmp(request->version, "HTTP/1.1") != 0)
        statuscode = "505 HTTP Version Not Supported\n";

    else
    {
        char *path = malloc(100);
        *path = '\0';
        strcat(path, server->vhosts->rootdir);
        strcat(path, request->target);

        printf("%s", path);
        // test de d'accessibilite de droit d'ouverture de fichier
        int correctfile = access(path, R_OK);
        if (correctfile == 0)
            statuscode = "200 OK\n";
        else if (correctfile == -1 && errno == ENOENT)
            statuscode = "404 Not Found\n";
        else
            statuscode = "403 Forbidden\n";
    }

    strcat(statusline, "HTTP/1.1 ");
    strcat(statusline, statuscode);
    strcat(statusline, date);

    return statusline;
}
