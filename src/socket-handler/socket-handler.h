#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H
#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../daemon-control/daemon-control.h"
#include "../event-handler/event-handler.h"
#include "../tools.h"
#include "config-manager.h"
#include "response.h"

int socket_handler(char *ip, char *port, struct servconfig *server);

#endif /* SOCKET_HANDLER_H */
