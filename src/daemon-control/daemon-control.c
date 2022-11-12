#define _XOPEN_SOURCE
#include "daemon-control.h"

void start_server(struct servconfig *server)
{
    socket_handler(server->vhosts->ip, server->vhosts->port, server);
}

void stop_server(struct servconfig *server)
{
    // Recuperation et arret du serveur
    FILE *file = fopen(server->global.pidfile, "r");
    char *buffer = malloc(7);
    while ((fgets(buffer, 7, file)) != NULL)
    {
        buffer[6] = '\0';
        kill(strtol(buffer, NULL, 10), SIGKILL);
    }

    if (server->global.logfile)
        remove(server->global.logfile);
    if (server->global.pidfile)
        remove(server->global.pidfile);

    free(server->global.logfile);
    free(server->global.pidfile);

    free(server->vhosts->servername);
    free(server->vhosts->rootdir);
    free(server->vhosts->defaultfile);
    free(server->vhosts->ip);
    free(server->vhosts->port);

    free(server->vhosts);
    free(server);

    exit(0);
}
struct returntype reload_server(struct servconfig **server)
{
    struct returntype returntype;
    (void)server;
    returntype.message = "server launch perfectly";
    returntype.value = 0;

    return returntype;
}
struct returntype restart_server(struct servconfig **server)
{
    struct returntype returntype;

    (void)server;

    returntype.message = "server launch perfectly";
    returntype.value = 0;

    return returntype;
}

struct returntype daemon_control(struct servconfig **server, const char *path,
                                 const char *command)
{
    struct returntype returntype;
    returntype.message = "server launch perfectly";
    returntype.value = 0;

    if (strcmp(command, "start") == 0)
    {
        returntype = parser(path, server);
        if (returntype.value != 0)
            return returntype;

        pid_t childpid = fork();

        if (childpid == 0)
        {
            start_server(*server);
        }
        else
        {
            FILE *pidfile = fopen((*server)->global.pidfile, "a");
            fprintf(pidfile, "%d\n", childpid);
        }

        return returntype;
    }

    else if (strcmp(command, "stop") == 0)
    {
        // recuperation du pid dans le pidfile , arret du serveur, suppression
        // du pidfile
    }

    else if (strcmp(command, "reload") == 0)
    {}
    else if (strcmp(command, "restart") == 0)
    {}

    return returntype; // will never be reached
}
