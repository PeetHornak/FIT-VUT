#include <stdio.h>
#include <stdlib.h>

void zvys(char *c)
{
	(*c) = (*c) + 1;
}

int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		fprintf(stderr, "chyba, zadej argumenty!\n");
		return 1;
	}

	printf("%s\n", argv[1]);
	puts(argv[1]) // to iste co printf

	//./demo6 Hello world
	// argv[0] argv[1] [argv[2]

	printf("znak prvniho argumentu a druheho znaku je: '%c' \n",
			argv[1][1]);

	zvys(&argv[1][1]);

	printf("znak prvniho argumentu a druheho znaku je: '%c' \n",
			argv[1][1]);

	printf("%s\n", argv[1]);
	

	return 0;
}
