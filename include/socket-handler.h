#ifndef SOCKET_HANDLER
#define SOCKET_HANDLER

/*
**	logfile (mandatory)
**	log (mandatory)
**	pidfile (mandatory)
*/
struct global
{
	char *logfile;
	bool log;
	char *pidfile;
}

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
	size_t port;
	size_t ip;

}


/*
**	global : variable global de notre fichier de config
**	vhosts : tableau de vhost
*/
struct servconfig
{
	struct global global;
	struct vhost *vhosts
}

#endif /* SOCKET_HANDLER_H */