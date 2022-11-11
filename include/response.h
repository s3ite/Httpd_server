#ifndef RESPONSE
#define RESPONSE

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "config-manager.h"
#include "errno.h"
#include "request.h"
#include "sys/sendfile.h"

char *status_line(struct request_info *request, struct servconfig *server);

#endif
