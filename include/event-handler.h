#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H


struct global
{
	char *logfile;
	bool log;
	char *pidfile;
}


struct vhost
{
	char *server_name;
	char *rootdir;
	size_t port;
	size_t ip;
}


/*
	global : variable global de notre fichier de config
	vhosts : tableau de vhost
*/
struct config
{
	struct global global;
	struct vhost *vhosts
}


#endif /* EVENT_HANDLER */