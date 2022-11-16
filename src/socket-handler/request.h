#ifndef REQUEST_H
#define REQUEST_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "config-manager.h"

struct request_info
{
    char *method;
    char *target;
    char *version;
};

struct request_info *parser_request(char *buffer, struct vhost *vhost);

#endif /* REQUEST_H */
