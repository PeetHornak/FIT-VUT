#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

bool jemale(int c)
{
	return (c >= 'a' && c <= 'Z');
}


int navelke(int znak)
{
	int velke;
	if (jemale(znak))
		return  znak - 'a' + 'A';
	else
		return  znak;
	// return jemale (znak) ? znak - 'a' + 'A' : znak; //TO ISTE CO HORE
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv
;
	int c;
	int velke;
	
	while( (c = getchar() ) != EOF)
	{
		velke = toupper(c);
		putchar(velke);
	}
	return 0;
}

