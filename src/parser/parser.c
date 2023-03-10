#include "parser.h"

#define SIZE 100

static struct servconfig **get_global_tag_value(char *line,
                                                struct servconfig **server,
                                                struct returntype *returntype)
{
    char *tmpchar = strtok_r(line, " =", &line);
    char *var = strtok_r(NULL, "= \n", &line);

    if (strcasecmp(tmpchar, "log_file") == 0)
    {
        //(*server)->global.logfile = malloc(SIZE);
        (*server)->global.logfile =
            var ? strcpy((*server)->global.logfile, var) : NULL;
    }

    else if (strcasecmp(tmpchar, "pid_file") == 0)
        (*server)->global.pidfile =
            var ? strcpy((*server)->global.pidfile, var) : NULL;

    else if (strcasecmp(tmpchar, "log") == 0)
        (*server)->global.log = strcasecmp(var, "true") == 0 ? true : false;

    else
    {
        returntype->value = 2;
        returntype->message =
            "erreur lors du parsing du fichier de configuration";
    }

    return server;
}

static void get_vhost_tag_value(char *line, struct vhost **vhost,
                                struct returntype *returntype)
{
    char *tmpchar = strtok_r(line, " =", &line);
    char *var = strtok_r(NULL, "= \n", &line);

    if (strcasecmp(tmpchar, "server_name") == 0)
        (*vhost)->servername = var ? strcpy((*vhost)->servername, var) : NULL;

    else if (strcasecmp(tmpchar, "port") == 0)
        (*vhost)->port = var ? strcpy((*vhost)->port, var) : NULL;

    else if (strcasecmp(tmpchar, "root_dir") == 0)
        (*vhost)->rootdir = var ? strcpy((*vhost)->rootdir, var) : NULL;

    else if (strcasecmp(tmpchar, "default_file") == 0)
    {
        (*vhost)->defaultfile =
            var ? strcpy((*vhost)->defaultfile, var) : "index.html";
    }

    else if (strcasecmp(tmpchar, "ip") == 0)
        (*vhost)->ip = var ? strcpy((*vhost)->ip, var) : NULL;
    else
    {
        returntype->value = 2;
        returntype->message =
            "erreur lors du parsing du fichier de configuration";
    }

    // return vhost;
}

static struct returntype checking(struct servconfig *server)
{
    struct returntype returntype;
    returntype.value = 2;
    if (!server->global.pidfile)
    {
        returntype.message =
            "erreur lors du parsing du fichier de configuration. pidfile value";
    }

    for (struct vhost *index = server->vhosts; index; index = index->next)
    {
        if (!server->vhosts->servername)
        {
            returntype.message = "erreur lors du parsing du fichier de "
                                 "configuration. servername value";
            return returntype;
        }

        else if (!server->vhosts->rootdir)
        {
            returntype.message = "erreur lors du parsing du fichier de "
                                 "configuration. rootdir value";
            return returntype;
        }

        else if (!server->vhosts->port)
        {
            returntype.message = "erreur lors du parsing du fichier de "
                                 "configuration. port value";
            return returntype;
        }

        else if (!server->vhosts->ip)
        {
            returntype.message =
                "erreur lors du parsing du fichier de configuration. ip value";
            return returntype;
        }
    }

    returntype.value = 0;
    return returntype;
}

static struct vhost **set_index(struct vhost **index)
{
    *index = malloc(sizeof(struct vhost));
    (*index)->servername = malloc(SIZE);
    (*index)->rootdir = malloc(SIZE);
    (*index)->defaultfile = malloc(SIZE);
    ;
    (*index)->ip = malloc(SIZE);
    (*index)->port = malloc(SIZE);
    (*index)->next = NULL;

    return index;
}

struct returntype parser(char const *path, struct servconfig **server)
{
    struct returntype returntype = { .message = NULL, .value = 0 };
    if (!*server)
        *server = init_config();

    if (path)
        // strcpy((*server)->global.path, path);

        if (!path)
            path = (*server)->global.path;

    FILE *stream = NULL;
    char *line = NULL;
    char *tmpchar = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    stream = fopen(path, "r");
    if (stream == NULL)
        returntype.value = 2;

    nread = getline(&line, &len, stream);
    tmpchar = strtok(line, "[ ]");

    if (strcasecmp(tmpchar, "global") != 0)
    {
        returntype.value = 2;
        return returntype;
    }

    while ((nread = getline(&line, &len, stream)) != 1 && returntype.value == 0)
        server = get_global_tag_value(line, server, &returntype);

    struct vhost **index = NULL;
    index = &((*server)->vhosts);

    nread = getline(&line, &len, stream);
    while (line)
    {
        if (!(*index))
            index = set_index(index);

        tmpchar = strtok(line, "[ ]");
        if (strcasecmp(tmpchar, "vhosts") != 0)
        {
            returntype.value = 2;
            return returntype;
        }

        while ((nread = getline(&line, &len, stream)) > 1
               && returntype.value == 0)
            get_vhost_tag_value(line, index, &returntype);

        nread = getline(&line, &len, stream);
        if (nread == -1)
            break;
        index = &((*index)->next);
    }

    returntype = checking(*server);
    if (returntype.value == 2)
        return returntype;

    free(line);
    fclose(stream);
    return returntype;
}
