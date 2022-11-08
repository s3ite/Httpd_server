#include "parser.h"

struct returntype parse_argument(char const *arguments)
{


}

struct returntype parse_file(char *path)
{
   FILE *stream;
   char *line = NULL;
   size_t len = 0;
   ssize_t nread;

   stream = fopen(argv[1], "r");
   if (stream == NULL) {
       ("fopen");
       exit(EXIT_FAILURE);
   }

   while ((nread = getline(&line, &len, stream)) != -1) {

   }

   free(line);
   fclose(stream);
   exit(EXIT_SUCCESS);
}