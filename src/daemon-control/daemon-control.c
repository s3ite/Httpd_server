#include "daemon-control.h"

void start_server(struct servconfig *server)
{
    socket_handler(server->vhosts->ip, server->vhosts->port, server);
}

struct returntype stop_server(struct servconfig **server)
{
    struct returntype returntype;
    (void)server;
    returntype.message = "server launch perfectly";
    returntype.value = 0;

    return returntype;
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
