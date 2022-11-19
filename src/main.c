#include "main.h"

struct servconfig *server = NULL;

int main(int argc, char const *argv[])
{
    const char *path = argv[argc - 1];
    const char *command = argv[1];
    bool launchdaemon = false;
    struct returntype returntype;

    struct stat st;
    int correctfile = stat(path, &st);
    if (correctfile != 0)
        return 2;

    returntype = parser(path, &server);
    if (returntype.value != 0)
        return returntype.value;

    if (strcasecmp(argv[1], "--dry-run") == 0)
    {
        if (returntype.value == 0)
            return 0;
        else
            return 2;
    }

    if (returntype.value != 0)
        return 2;

    if (strcasecmp(argv[1], "-a") == 0)
    {
        launchdaemon = true;
        command = argv[2];

        if (strcasecmp(command, "start") != 0
            && strcasecmp(command, "stop") != 0
            && strcasecmp(command, "reload") != 0
            && strcasecmp(command, "restart") != 0)
            return 2;

        if (strcasecmp(command, "restart") == 0)
        {
            stop_server(server);
            command = "start";
        }
    }

    if (launchdaemon == true)
        returntype = daemon_control(&server, command);
    else
        returntype.value = start_server(server);

    return returntype.value == 1 ? returntype.value : 0;
}
