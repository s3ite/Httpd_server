#ifndef RESPONSE_H
#define RESPONSE_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "config-manager.h"
#include "request.h"
#include "sys/sendfile.h"

struct response_info
{
    char *statuscode;
    char *path;
    char *statusline;
};

struct response_info *parser_response(struct request_info *request,
                                      struct vhost *vhost);

#endif /* RESPONSE_H */
