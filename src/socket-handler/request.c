#include "request.h"

#define MAX_SIZE 100
// return the path to the file location asked by the user
struct request_info *parser_request(char *buffer)
{
    // strtok(NULL, "/n");

    struct request_info *request_info = malloc(sizeof(struct request_info));

    request_info->method = malloc(MAX_SIZE);
    request_info->target = malloc(MAX_SIZE);
    request_info->version = malloc(MAX_SIZE);

    char *tmp = strtok(buffer, " ");
    request_info->method = tmp ? strcpy(request_info->method, tmp) : NULL;

    tmp = strtok(NULL, " ");
    request_info->target = tmp ? strcpy(request_info->target, tmp) : NULL;

    tmp = strtok(NULL, " \n");
    request_info->version = tmp ? strcpy(request_info->version, tmp) : NULL;

    if (!request_info->version || !request_info->method
        || !request_info->target)
        return NULL;
    return request_info;
}
