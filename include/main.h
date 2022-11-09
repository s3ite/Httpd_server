#define _GNU_SOURCE

#ifndef MAIN
#define MAIN

#include "daemon-control.h"
#include "event-handler.h"
#include "parser.h"
#include "socket-handler.h"
#include "tools.h"

#include <errno.h>

#include <stdio.h>





// ===========================Parser===============================
struct servconfig *init_config(void);


//=============================Socket==============================
struct returntype parser(char const *path, struct servconfig **serveur);


#endif /* MAIN_H */
