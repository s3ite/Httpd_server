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
    if (file)
    {
        char buffer[15];
        while ((fgets(buffer, 7, file)) != NULL)
        {
            buffer[6] = '\0';
            kill(strtol(buffer, NULL, 10), SIGKILL);
        }

        if (server->global.logfile)
            remove(server->global.logfile);
        if (server->global.pidfile)
            remove(server->global.pidfile);
    }

    free(server->global.logfile);
    free(server->global.pidfile);

    free(server->vhosts->servername);
    free(server->vhosts->rootdir);
    free(server->vhosts->defaultfile);
    free(server->vhosts->ip);
    free(server->vhosts->port);

    free(server->vhosts);
    free(server);
}
struct returntype reload_server(struct servconfig **server)
{
    struct returntype returntype;
    (void)server;
    returntype.message = "server launch perfectly";
    returntype.value = 0;

    return returntype;
}
void restart_server(struct servconfig **server)
{
    stop_server(*server);
    start_server(*server);
}

struct returntype daemon_control(struct servconfig **server, const char *path,
                                 const char *command)
{
    struct returntype returntype;
    returntype.message = "server launch perfectly";
    returntype.value = 0;

    returntype = parser(path, server);
    if (returntype.value != 0)
        return returntype;

    if (strcasecmp(command, "start") == 0)
    {
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

    else if (strcasecmp(command, "stop") == 0)
    {
        // Recuperation et arret du serveur
        FILE *file = fopen((*server)->global.pidfile, "r");
        if (file)
        {
            char buffer[7];
            while ((fgets(buffer, 7, file)) != NULL)
            {
                buffer[6] = '\0';
                kill(strtol(buffer, NULL, 10), SIGTERM);
            }
        }
        // raise(SIGTERM);
        stop_server(*server);
    }

    else if (strcasecmp(command, "reload") == 0)
    {}
    else if (strcasecmp(command, "restart") == 0)
    {
        restart_server(server);
    }

    return returntype; // will never be reached
}
