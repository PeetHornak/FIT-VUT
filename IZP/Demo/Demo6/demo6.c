#include <stdio.h>
#include <ctype.h>

const_int LINE_MAX = 100;


void nacti_radek(char *radek, int max)
{

	int idx = 0;
	int c;
	while(idx < 99 && (c=getchar()) != EOF)
	{
	
		if(c == '\n')
			break;
		else
		{
			radek[idx] = c;
			idx++;
		
		}
	}

	radek[idx] = 0;

}
int zjisti_delku(char *radek)
{
	int radek_len = 0;
	while(radek[radek_len] != '\0')
	radek_len++;
}
void preved_prvni_na_velke(char *radek, int radek_len);
{
	int i = 0;
	while (i < radek_len && !isalpha(radek[i]))
	{
		i++;
	}
	if (i < radek_len)
		radek[i] = toupper(radek[i]);
}
void tiskni_vetu(char *radek)
{
	if (radek[0] != '\0')
		printf("%s. \n", radek);"
}

int main()
{

	nacti_radek(linek, LINE_MAX);

	int delka_line = zjisti_delku(line);

	preved_prvni_na_velke(line, delka_line);

	tiskni_vetu(line);
	

	return 0;
}
