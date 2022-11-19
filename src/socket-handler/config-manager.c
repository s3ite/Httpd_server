#include "config-manager.h"

#define SIZE 100
struct servconfig *init_config()
{
    struct servconfig *server = malloc(sizeof(struct servconfig));
    server->vhosts = malloc(sizeof(struct vhost));

    server->global.logfile = malloc(SIZE);
    server->global.log = true;
    server->global.pidfile = malloc(SIZE);
    server->global.path = malloc(SIZE);

    server->vhosts->servername = malloc(SIZE);
    server->vhosts->rootdir = malloc(SIZE);
    server->vhosts->defaultfile = malloc(SIZE);
    server->vhosts->ip = malloc(SIZE);
    server->vhosts->port = malloc(SIZE);
    server->vhosts->next = NULL;

    return server;
}
