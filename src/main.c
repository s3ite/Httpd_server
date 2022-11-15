#include "main.h"

struct servconfig *server = NULL;

void signal_handler_int(int sig)
{
    sig++;
    stop_server(server);
    free_server(server);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, signal_handler_int);

    const char *path = argv[argc - 1];
    const char *command;
    bool launchdaemon = false;

    // appel du parseur avec les arguments d'executable
    struct returntype returntype;

    // Dry run
    if (strcasecmp(argv[1], "--dry-run") == 0)
    {
        path = argv[argc - 1];
        returntype = parser(path, &server);

        if (returntype.value == 0)
        {
            print_config_parameter(server);
            printf("%s\n", returntype.message);
        }
        else
            errx(returntype.value, "%s", returntype.message);
        return returntype.value;
    }

    if (strcasecmp(argv[1], "-a") == 0)
    {
        launchdaemon = true;
        command = argv[2];
    }

    else
        command = argv[1];

    if (strcasecmp(command, "start") != 0 && strcasecmp(command, "stop") != 0
        && strcasecmp(command, "reload") != 0
        && strcasecmp(command, "restart") != 0)
        errx(2, "invalid argument : command");
    else
    {
        returntype = parser(path, &server);

        if (returntype.value != 0)
            return returntype.value;

        if (strcasecmp(command, "restart") == 0)
        {
            stop_server(server);
            returntype = parser(path, &server);
            if (returntype.value != 0)
                return returntype.value;
            command = "start";
        }
        if (launchdaemon)
            daemon_control(&server, command);

        else
            start_server(server);
    }

    return 0;
}
