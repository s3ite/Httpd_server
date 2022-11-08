
#include "main.h"

int main(int argc, char const *argv[])
{
	/*
	 * 						appel du parseur avec les arguments du main
	 * const *array : 
	 * 		tableau des differents arguements recu lors de l'excution du programme
	 * retour : 
	 * 		struct returntype(value, error)
	 */ 
	if ((struct returntype returntype = parse_argument(argv)).value == -1)
	{
		printf("%s\n", returntype.errormessage);
		return returntype.value;
	}


	
	return 0;
}