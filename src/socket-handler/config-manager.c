#include "config-manager.h"

#include "tools.h"

#define SIZE 100
struct servconfig *init_config(void)
{
    struct servconfig *config = malloc(sizeof(struct servconfig));
    config->vhosts = malloc(sizeof(struct vhost));

    config->global.logfile = malloc(SIZE);
    config->global.log = true;
    config->global.pidfile = malloc(SIZE);

    config->vhosts->servername = malloc(SIZE);
    config->vhosts->rootdir = malloc(SIZE);
    config->vhosts->defaultfile = malloc(SIZE);
    config->vhosts->ip = malloc(SIZE);
    config->vhosts->port = malloc(SIZE);

    return config;
}

void print_config_parameter(struct servconfig *server)
{
    printf("\n\tGLOBAL\n");
    printf("logfile : %s\n", server->global.logfile);
    printf("log : %s\n", server->global.log ? "true" : "false");

    if (server->global.pidfile)
        printf("pidfile : %s\n", server->global.pidfile);

    printf("\n\tVHOSTS\n");
    printf("servername : %s\n", server->vhosts->servername);
    printf("rootdir : %s\n", server->vhosts->rootdir);

    if (server->vhosts->defaultfile)
        printf("defaultfile : %s\n", server->vhosts->defaultfile);

    printf("ip : %s\n", server->vhosts->ip);
    printf("port : %s\n", server->vhosts->port);
}
