#define POSIX_C_SOURCE 200809L

#include "tools.h"

void log_request(char *servname, struct request_info *request_info,
                 char *clientip, char *path)
{
    char *statusline = malloc(100);
    *statusline = '\0';

    time_t timestamp = time(NULL);
    struct tm *pTime = localtime(&timestamp);
    char date[100];
    strftime(date, 100, "Date : %a, %d %b %G %T GMT", pTime);

    FILE *file;

    if (path)
        file = fopen(path, "a");
    else
        file = stdout;

    fprintf(file, "%s [%s] received %s on %s from %s\n", date, servname,
            request_info->method, request_info->target, clientip);

    if (path)
        fclose(file);
}

void log_response(char *servname, char *statuscode, char *clientip,
                  struct request_info *request_info, char *path)
{
    char *statusline = malloc(100);
    *statusline = '\0';

    time_t timestamp = time(NULL);
    struct tm *pTime = localtime(&timestamp);
    char date[100];
    strftime(date, 100, "Date : %a, %d %b %G %T GMT", pTime);

    FILE *file;

    if (path)
        file = fopen(path, "a");
    else
        file = stdout;

    fprintf(file, "%s [%s] responding with %s to %s for %s on %s\n", date,
            servname, statuscode, clientip, request_info->method,
            request_info->target);

    if (path)
        fclose(file);
}
