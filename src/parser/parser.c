#include "parser.h"

void get_global_tag_value(char *line, struct servconfig *config,
                          struct returntype *returntype)
{
    char *tmpchar = NULL;
    tmpchar = strtok(line, " =");

    if (strcmp(tmpchar, "log_file"))
        config->global.logfile = strtok(line, " ");

    else if (strcmp(tmpchar, "pid_file"))
        config->global.pidfile = strtok(line, " ");

    else if (strcmp(tmpchar, "log"))
        config->global.log = strcmp(strtok(line, " "), "true") ? true : false;

    else
    {
        returntype->value = 2;
        returntype->errormessage =
            "erreur lors du parsing du fichier de configuration";
    }
}

void get_vhost_tag_value(char *line, struct servconfig *config,
                         struct returntype *returntype)
{
    char *tmpchar = NULL;
    tmpchar = strtok(line, " =");

    if (strcmp(tmpchar, "server_name"))
        config->vhosts->servername = strtok(line, " ");

    else if (strcmp(tmpchar, "port"))
        config->vhosts->port = atoi(strtok(line, " ")); // to size_t

    else if (strcmp(tmpchar, "root_dir"))
        config->vhosts->rootdir = strtok(line, " ");

    else if (strcmp(tmpchar, "default_file"))
        config->vhosts->defaultfile = strtok(line, " ");

    else if (strcmp(tmpchar, "ip"))
        config->vhosts->rootdir = strtok(line, " ");
    else
    {
        returntype->value = 2;
        returntype->errormessage =
            "erreur lors du parsing du fichier de configuration";
    }
}

struct returntype parser(char const *path, struct servconfig **serveur)
{
    struct servconfig *config = NULL;
    struct returntype returntype;

    if (!*serveur)
        *serveur = init_config();

    config = *serveur;

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
            "erreur lors du parsing du fichier de configuration";
    }

    while ((nread = getline(&line, &len, stream)) != '\n')
        get_global_tag_value(line, config, &returntype);

    // parsing du tag vhosts
    if (strcmp(tmpchar, "vhosts") != 0)
    {
        returntype.value = 2;
        returntype.errormessage =
            "erreur lors du parsing du fichier de configuration";
    }

    while ((nread = getline(&line, &len, stream)) != -1)
        get_vhost_tag_value(line, config, &returntype);

    // mandatory checking
    if (!config->global.pidfile || !config->vhosts->servername
        || !config->vhosts->rootdir || config->vhosts->port == -1
        || !config->vhosts->ip)
    {
        returntype.value = 2;
        returntype.errormessage = "erreur lors du parsing du fichier de "
                                  "configuration. missing mandatory value";
    }

    free(line);
    fclose(stream);

    return returntype;
}