#include "event-handler.h"
extern struct servconfig *server;

void reload_signal_handler(int sig)
{
    sig++;
    parser(NULL, &server);
    print_config_parameter(server);
}
