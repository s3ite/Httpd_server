#define _GNU_SOURCE
#ifndef PARSER
#    define PARSER

#    include <stdbool.h>
#    include <stdio.h>
#    include <stdlib.h>
#    include <string.h>

#    include "config-manager.h"
#    include "tools.h"

struct returntype parser(char const *path, struct servconfig **serveur);

#endif /* PARSER */