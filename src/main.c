#include "main.h"


int main(int argc, char const *argv[])
{
	// appel du parseur avec les arguments d'executable
	if ((struct returntype returntype = parse_argument(argv)).value == 2)
	{
		perror(returntype.errormessage);
		return returntype.value;
	}


	
	return 0;
}