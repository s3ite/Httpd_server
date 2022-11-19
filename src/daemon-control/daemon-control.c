#define _XOPEN_SOURCE
#include "daemon-control.h"

static size_t get_pid(struct servconfig *server)
{
    size_t pid = 0;
    FILE *file = fopen(server->global.pidfile, "r");
    if (file)
    {
        char buffer[15] = { '\0' };
        if (fgets(buffer, 7, file))
            pid = strtol(buffer, NULL, 10);
        fclose(file);
    }

    return pid;
}
size_t start_server(struct servconfig *server)
{
    size_t pid = get_pid(server);
    if (pid > 0)
        return 1;

    FILE *pidfile = fopen(server->global.pidfile, "a");
    if (pidfile)
    {
        fprintf(pidfile, "%d\n", getpid());
        fclose(pidfile);

        socket_handler(server->vhosts->ip, server->vhosts->port, server);
    }

    return 0;
}

void free_server(struct servconfig *server)
{
    free(server->global.logfile);
    free(server->global.pidfile);
    free(server->global.path);

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
    int pid = get_pid(server);

    if (pid > 0)
        kill(pid, SIGKILL);

    if (server->global.logfile)
        remove(server->global.logfile);
    if (server->global.pidfile)
        remove(server->global.pidfile);
}
static struct returntype reload_server(struct servconfig **server)
{
    struct returntype returntype;
    FILE *file = fopen((*server)->global.pidfile, "r");
    if (file)
    {
        char buffer[15];
        while ((fgets(buffer, 7, file)) != NULL)
        {
            buffer[6] = '\0';
            kill(strtol(buffer, NULL, 10), SIGCONT);
        }
        returntype.value = 0;
        fclose(file);
    }
    return returntype;
}
static void restart_server(struct servconfig **server)
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
        size_t pid = get_pid(*server);
        if (pid > 0)
        {
            returntype.value = 1;
            return returntype;
        }

        pid_t childpid = fork();

        if (childpid == 0)
            start_server(*server);

        return returntype;
    }

    else if (strcasecmp(command, "stop") == 0)
    {
        stop_server(*server);
        free_server(*server);
    }

    else if (strcasecmp(command, "reload") == 0)
    {
        reload_server(server);
        free_server(*server);
    }
    else if (strcasecmp(command, "restart") == 0)
        restart_server(server);

    return returntype; // will never be reached
}
