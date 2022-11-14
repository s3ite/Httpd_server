#include "main.h"

struct servconfig *server = NULL;

int main(int argc, char const *argv[])
{
    const char *path = NULL;

    // appel du parseur avec les arguments d'executable
    struct returntype returntype;

    // Dry run
    if (strcasecmp(argv[1], "--dry-run") == 0)
    {
        if (argc != 3)
            errx(2, "invalid arguments : --dry-run");

        path = argv[2];
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
        const char *command = argv[2];
        if (argc != 4)
            errx(2, "invalid arguments : -a");

        path = argv[3];

        if (strcasecmp(command, "start") != 0
            && strcasecmp(command, "stop") != 0
            && strcasecmp(command, "reload") != 0
            && strcasecmp(command, "restart") != 0)
            errx(2, "invalid arguments : -a");
        else
            daemon_control(&server, path, command);
    }

    else
        errx(2, "invalid arguments : %s", argv[1]);

    return 0;
}
