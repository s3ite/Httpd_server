#ifndef SOCKET_HANDLER
#define SOCKET_HANDLER

#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "config-manager.h"
#include "daemon-control.h"
#include "response.h"
#include "tools.h"

int socket_handler(char *ip, char *port, struct servconfig *server);

#endif /* SOCKET_HANDLER_H */
