#ifndef DAEMON_CONTROL_H
#define DAEMON_CONTROL_H

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "socket-handler.h"
#include "tools.h"

void stop_server(struct servconfig *server);
struct returntype daemon_control(struct servconfig **server, const char *path,
                                 const char *command);
#endif /* DAEMON_CONTROL_H */
