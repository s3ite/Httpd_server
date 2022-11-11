#include "parser.h"

struct servconfig **get_global_tag_value(char *line, struct servconfig **server,
                                         struct returntype *returntype)
{
    char *tmpchar = strtok_r(line, " =", &line);
    char *var = strtok_r(NULL, "= \n", &line);

    if (strcmp(tmpchar, "log_file") == 0)
        (*server)->global.logfile =
            var ? strcpy((*server)->global.logfile, var) : NULL;

    else if (strcmp(tmpchar, "pid_file") == 0)
        (*server)->global.pidfile =
            var ? strcpy((*server)->global.pidfile, var) : NULL;

    else if (strcmp(tmpchar, "log") == 0)
        (*server)->global.log = strcmp(var, "true") == 0 ? true : false;

    else
    {
        returntype->value = 2;
        returntype->errormessage =
            "erreur lors du parsing du fichier de configuration";
    }

    return server;
}

struct servconfig **get_vhost_tag_value(char *line, struct servconfig **server,
                                        struct returntype *returntype)
{
    char *tmpchar = strtok_r(line, " =", &line);
    char *var = strtok_r(NULL, "= \n", &line);

    if (strcmp(tmpchar, "server_name") == 0)
        (*server)->vhosts->servername =
            var ? strcpy((*server)->vhosts->servername, var) : NULL;

    else if (strcmp(tmpchar, "port") == 0)
        (*server)->vhosts->port =
            var ? strcpy((*server)->vhosts->servername, var) : NULL;

    else if (strcmp(tmpchar, "root_dir") == 0)
        (*server)->vhosts->rootdir =
            var ? strcpy((*server)->vhosts->rootdir, var) : NULL;

    else if (strcmp(tmpchar, "default_file") == 0)
        (*server)->vhosts->defaultfile =
            var ? strcpy((*server)->vhosts->defaultfile, var) : "index.html";

    else if (strcmp(tmpchar, "ip") == 0)
        (*server)->vhosts->rootdir =
            var ? strcpy((*server)->vhosts->ip, var) : NULL;
    else
    {
        returntype->value = 2;
        returntype->errormessage =
            "erreur lors du parsing du fichier de configuration";
    }

    return server;
}

struct returntype checking(struct servconfig *config)
{
    struct returntype returntype;

    if (!config->global.pidfile)
        returntype.errormessage =
            "erreur lors du parsing du fichier de configuration. pidfile value";

    else if (!config->vhosts->servername)
        returntype.errormessage = "erreur lors du parsing du fichier de "
                                  "configuration. servername value";

    else if (!config->vhosts->rootdir)
        returntype.errormessage =
            "erreur lors du parsing du fichier de configuration. rootdir value";

    else if (!config->vhosts->port)
        returntype.errormessage =
            "erreur lors du parsing du fichier de configuration. port value";

    else if (!config->vhosts->ip)
        returntype.errormessage =
            "erreur lors du parsing du fichier de configuration. ip value";

    else
    {
        returntype.value = 0;
        return returntype;
    }

    returntype.value = 2;
    return returntype;
}

struct returntype parser(char const *path, struct servconfig **server)
{
    struct returntype returntype = { .errormessage = NULL, .value = 0 };

    FILE *stream = NULL;
    char *line = NULL;
    char *tmpchar = NULL;

    size_t len = 0;
    ssize_t nread = 0;

    // impossiblite d'ouverture du fichier
    stream = fopen(path, "r");
    if (stream == NULL)
    {
        returntype.value = 2;
        returntype.errormessage =
            "erreur d'ouverture du fichier de configuration";
    }

    // parsing du tag global
    nread = getline(&line, &len, stream);
    tmpchar = strtok(line, "[ ]");

    if (strcmp(tmpchar, "global") != 0)
    {
        returntype.value = 2;
        returntype.errormessage =
            "erreur lors du parsing du fichier de configuration : global";
        return returntype;
    }

    while ((nread = getline(&line, &len, stream)) != 1 && returntype.value == 0)
        server = get_global_tag_value(line, server, &returntype);

    // saut de la ligne de separation
    nread = getline(&line, &len, stream);
    tmpchar = strtok(line, "[ ]");

    // parsing du tag vhosts
    if (strcmp(tmpchar, "vhosts") != 0)
    {
        returntype.value = 2;
        returntype.errormessage =
            "erreur lors du parsing du fichier de configuration : vhosts";

        return returntype;
    }

    while ((nread = getline(&line, &len, stream)) != -1
           && returntype.value == 0)
        server = get_vhost_tag_value(line, server, &returntype);

    // mandatory checking
    returntype = checking(*server);
    if (returntype.value == 2)
        return returntype;

    returntype.errormessage = "Parsing done well";

    free(line);
    fclose(stream);

    return returntype;
}
