#ifndef PARSER_H
#    define PARSER_H

#    include <stdbool.h>
#    include <stdio.h>
#    include <stdlib.h>
#    include <string.h>

#    include "config-manager.h"
#    include "tools.h"

struct returntype parser(char const *path, struct servconfig **serveur);

#endif /* PARSER_H */
