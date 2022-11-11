#ifndef CONFIG_MANAGER
#define CONFIG_MANAGER

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

/*
**	logfile :
**	log :
**	pidfile : (mandatory)
*/
struct global
{
    char *logfile;
    bool log;
    char *pidfile;
};

/*
**	servername : (mandatory)
**	rootdir : (mandatory)
**	port : (mandatory)
**	ip : (mandatory)
**	defaultfile :
*/
struct vhost
{
    char *servername;
    char *rootdir;
    char *defaultfile;
    char *ip;
    char *port;
};

/*
**	global : variable global de notre fichier de config
**	vhosts : tableau de vhost
*/
struct servconfig
{
    struct global global;
    struct vhost *vhosts;
};

struct servconfig *init_config(void);

void print_config_parameter(struct servconfig *server);

#endif /* CONFIG_MANAGER */