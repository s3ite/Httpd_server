#include "main.h"

struct servconfig *config = NULL;

int main(int argc, char const *argv[])
{
    const char *path = NULL;

    // appel du parseur avec les arguments d'executable
    struct returntype returntype;

    // Dry run
    if (strcmp(argv[1], "--dry-run") == 0)
    {
        if (argc != 3)
            errx(2, "invalid arguments : --dry-run");

        path = argv[2];
        returntype = parser(path, &config);

        if (returntype.value == 0)
            printf("%s", returntype.message);
        else
            errx(returntype.value, "%s", returntype.message);
        return returntype.value;
    }

    if (strcmp(argv[1], "-a") == 0)
    {
        const char *command = argv[2];
        if (argc != 4)
            errx(2, "invalid arguments : -a");

        path = argv[3];

        if (strcmp(command, "start") != 0 && strcmp(command, "stop") != 0
            && strcmp(command, "reload") != 0
            && strcmp(command, "restart") != 0)
            errx(2, "invalid arguments : -a");
        else
            daemon_control(&config, path, command);
    }

    else
        errx(2, "invalid arguments : %s", argv[1]);

    return 0;
}
