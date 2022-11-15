#ifndef DAEMON_CONTROL_H
#define DAEMON_CONTROL_H

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "../parser/parser.h"
#include "../socket-handler/socket-handler.h"
#include "../tools.h"

void start_server(struct servconfig *server);
void stop_server(struct servconfig *server);
void free_server(struct servconfig *server);
struct returntype daemon_control(struct servconfig **server,
                                 const char *command);
#endif /* DAEMON_CONTROL_H */
