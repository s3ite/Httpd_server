#include "request.h"

#define MAX_SIZE 100
// return the path to the file location asked by the user
struct request_info *parser_request(char *buffer, struct vhost *vhost)
{
    // strtok(NULL, "/n");

    struct request_info *request_info = malloc(sizeof(struct request_info));

    request_info->method = malloc(MAX_SIZE);
    request_info->target = malloc(MAX_SIZE);
    request_info->version = malloc(MAX_SIZE);

    char *method = strtok(buffer, " \n");
    char *target = strtok(NULL, " \n");
    char *version = strtok(NULL, " \n");

    if (!version)
    {
        version = target;
        target = vhost->defaultfile;
    }

    request_info->method = method ? strcpy(request_info->method, method) : NULL;

    request_info->target = target ? strcpy(request_info->target, target) : NULL;

    request_info->version =
        version ? strcpy(request_info->version, version) : NULL;

    if (!request_info->version || !request_info->method
        || !request_info->target)
        return NULL;

    return request_info;
}
