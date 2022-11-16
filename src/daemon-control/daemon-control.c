#define _XOPEN_SOURCE
#include "daemon-control.h"

void start_server(struct servconfig *server)
{
    // for (struct vhost *index = server->vhosts; index; index = index->next)

    socket_handler(server->vhosts->ip, server->vhosts->port, server);
}

void free_server(struct servconfig *server)
{
    free(server->global.logfile);
    free(server->global.pidfile);

    for (struct vhost *index = server->vhosts; index;)
    {
        free(index->servername);
        free(index->rootdir);
        free(index->defaultfile);
        free(index->ip);
        free(index->port);

        struct vhost *tmp = index;
        index = index->next;
        free(tmp);
    }

    free(server);
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

            // need to be change to SIGTERM
            kill(strtol(buffer, NULL, 10), SIGKILL);
        }

        if (server->global.logfile)
            remove(server->global.logfile);
        if (server->global.pidfile)
            remove(server->global.pidfile);
        fclose(file);
    }
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

struct returntype daemon_control(struct servconfig **server,
                                 const char *command)
{
    struct returntype returntype;
    returntype.message = "server launch perfectly";
    returntype.value = 0;

    if (strcasecmp(command, "start") == 0)
    {
        pid_t childpid = fork();

        if (childpid == 0)
            start_server(*server);

        else
        {
            FILE *pidfile = fopen((*server)->global.pidfile, "a");
            fprintf(pidfile, "%d\n", childpid);
        }

        return returntype;
    }

    else if (strcasecmp(command, "stop") == 0)
    {
        stop_server(*server);
        free_server(*server);
    }

    else if (strcasecmp(command, "reload") == 0)
    {
        FILE *file = fopen((*server)->global.pidfile, "r");
        if (file)
        {
            char buffer[7];
            while ((fgets(buffer, 7, file)) != NULL)
            {
                buffer[6] = '\0';
                kill(strtol(buffer, NULL, 10), SIGSTOP);
            }
        }
        fclose(file);
    }
    else if (strcasecmp(command, "restart") == 0)
    {
        restart_server(server);
        free_server(*server);
    }

    return returntype; // will never be reached
}
