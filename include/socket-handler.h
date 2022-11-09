#ifndef SOCKET_HANDLER
#define SOCKET_HANDLER

#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>


#include "main.h"

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
	int port;
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



#endif /* SOCKET_HANDLER_H */