#ifndef TOOLS_H
#define TOOLS_H

#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "socket-handler/request.h"
struct returntype
{
    char *message;
    size_t value;
};
void log_request(char *servname, struct request_info *request_info,
                 char *clientip, char *path);
void log_response(char *servname, char *statuscode, char *clientip,
                  struct request_info *request_info, char *path);

#endif /* TOOLS_H */
