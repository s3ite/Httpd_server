#include "main.h"

struct servconfig *config = NULL;

int main(int argc, char const *argv[])
{
    (void)argc;

    char *PORT = "8080";
    char *IP = "127.0.0.1";
    if (!config)
        config = init_config();

    // /!\ to modify
    const char *path = argv[1];

    // appel du parseur avec les arguments d'executable
    struct returntype returntype = parser(path, &config);
    if (returntype.value == 2)
    {
        perror(returntype.errormessage);
        return returntype.value;
    }

    print_config_parameter(config);

    // char buffer[100] = "GET include/main.o HTTP/1.1 \r\n";

    // struct request_info *request_info = parser_request(buffer);

    // printf("\n\nMethod: %s\nTarget : %s\nVersion: %s\n",
    // request_info->method,request_info->target, request_info->version);

    // printf("\n\n%s", status_line(request_info, config));

    socket_handler(IP, PORT, config);
    return 0;
}
