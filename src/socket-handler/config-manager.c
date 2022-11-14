#include "config-manager.h"

#define SIZE 100
struct servconfig *init_config(void)
{
    struct servconfig *server = malloc(sizeof(struct servconfig));
    server->vhosts = malloc(sizeof(struct vhost));

    server->global.logfile = malloc(SIZE);
    server->global.log = true;
    server->global.pidfile = malloc(SIZE);

    server->vhosts->servername = malloc(SIZE);
    server->vhosts->rootdir = malloc(SIZE);
    server->vhosts->defaultfile = malloc(SIZE);
    server->vhosts->ip = malloc(SIZE);
    server->vhosts->port = malloc(SIZE);
    server->vhosts->next = NULL;

    return server;
}

void print_config_parameter(struct servconfig *server)
{
    printf("\n\tGLOBAL\n");
    printf("logfile : %s\n", server->global.logfile);
    printf("log : %s\n", server->global.log ? "true" : "false");

    if (server->global.pidfile)
        printf("pidfile : %s\n", server->global.pidfile);

    for (struct vhost *index = server->vhosts; index; index = index->next)
    {
        printf("\n\tVHOSTS\n");
        printf("servername : %s\n", server->vhosts->servername);
        printf("rootdir : %s\n", server->vhosts->rootdir);

        if (server->vhosts->defaultfile)
            printf("defaultfile : %s\n", server->vhosts->defaultfile);

        printf("ip : %s\n", server->vhosts->ip);
        printf("port : %s\n", server->vhosts->port);
    }
}
