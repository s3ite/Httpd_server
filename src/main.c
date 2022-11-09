#include "main.h"

struct servconfig *config = NULL;

int main(int argc, char const *argv[]) {
	(void) argc;
	
	// /!\ to modify
	const char *path = argv[1];

  	// appel du parseur avec les arguments d'executable
	struct returntype returntype = parser(path, &config);
  if (returntype.value == 2) {
    perror(returntype.errormessage);
    return returntype.value;
  }

  return 0;
}