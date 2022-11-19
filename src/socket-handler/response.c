#include "response.h"

int get_length_file(char *path)
{
    struct stat st;
    stat(path, &st);
    return st.st_size;
}

char *code_request(struct request_info *request,
                   struct response_info **response_info, char *path)
{
    char *statuscode;
    struct stat sb;
    if (strcasecmp(request->method, "GET") != 0
        && strcasecmp(request->method, "HEAD") != 0)
    {
        (*response_info)->statuscode = "405";
        statuscode = "405 Method Not Allowed\n";
    }

    else if (strcasecmp(request->version, "HTTP/1.1") != 0)
    {
        (*response_info)->statuscode = "505";
        statuscode = "505 HTTP Version Not Supported\n";
    }

    else
    {
        (*response_info)->path = path;

        // test de d'accessibilite de droit d'ouverture de fichier
        int correctfile = stat(path, &sb);
        if (correctfile == 0)
        {
            (*response_info)->statuscode = "200";
            statuscode = "200 OK\n";
        }
        else if (correctfile == -1 && errno == ENOENT)
        {
            (*response_info)->statuscode = "404";
            statuscode = "404 Not Found\n";
        }
        else
        {
            (*response_info)->statuscode = "403";
            statuscode = "403 Forbidden\n";
        }
    }
    return statuscode;
}

// connect to socket and send the response to the client
struct response_info *parser_response(struct request_info *request,
                                      struct vhost *vhost)
{
    struct response_info *response_info = malloc(sizeof(struct response_info));

    response_info->statusline = malloc(100);

    response_info->statusline[0] = '\0';
    char *statuscode = "";

    time_t timestamp = time(NULL);
    struct tm *pTime = localtime(&timestamp);
    char date[100];
    strftime(date, 100, "Date : %a, %d %b %G %T GMT\n", pTime);

    if (!request)
    {
        response_info->statuscode = "400";
        statuscode = "400 Bad Request";
        strcat(response_info->statusline, statuscode);
        return response_info;
    }

    char *path = malloc(100);
    *path = '\0';
    strcat(path, vhost->rootdir);
    strcat(path, request->target);

    statuscode = code_request(request, &response_info, path);

    int contentlength = get_length_file(path);
    sprintf(response_info->statusline, "%s %s%sContent-Length: %d\n%s\n\n",
            "HTTP/1.1", statuscode, date, contentlength, "Connection: close");

    return response_info;
}
