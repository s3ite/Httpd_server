#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "config-manager.h"
#include "errno.h"
#include "request.h"
#include "sys/sendfile.h"

struct response_info
{
    char *statuscode;
    char *path;
    char *statusline;
};

struct response_info *parser_response(struct request_info *request,
                                      struct servconfig *server);

#endif /* RESPONSE_H */
