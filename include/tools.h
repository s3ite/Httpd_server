#define POSIX_C_SOURCE 200809L

#ifndef TOOLS_H
#define TOOLS_H

#include <stddef.h>
#include <stdio.h>
#include "request.h"
#include <time.h>
struct returntype
{
    char *errormessage;
    size_t value;
};
void log_request(char *servname, struct request_info *request_info,
                 char *clientip, char *path);
void log_response(char *servname, char *statuscode,  char *clientip,struct request_info *request_info, char *path);

#endif /* TOOLS_H */
