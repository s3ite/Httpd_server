#include "socket-handler.h"

#include "tools.h"

struct servconfig *init_config(void)
{
    struct servconfig *config = malloc(sizeof(struct servconfig));
    config->vhosts = malloc(sizeof(struct vhost));

    config->global.logfile = NULL;
    config->global.log = false;
    config->global.pidfile = NULL;

    ;

    config->vhosts->servername = NULL;
    config->vhosts->rootdir = NULL;
    config->vhosts->defaultfile = NULL;
    config->vhosts->ip = NULL;
    config->vhosts->port = -1;

    return config;
}