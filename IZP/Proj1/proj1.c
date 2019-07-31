/*************************************************/
/***************** File: proj1.c *****************/
/**************** Date: 2017/11/5 ****************/
/************* Author: Peter Hornak **************/
/******************* xhorna14 ********************/
/*********** Project: Prace s textem *************/
/*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN	101
#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

/* Ak char mozme vypisat ako enable prida ho do pola ako velke pismeno */
 void add_to_array(char c, char *enable)
 {
	unsigned int i = 0;
	while (i < strlen(enable))
	{
		if(enable[i] == c)
		{
			break;
		}
		else if(enable[i] == c + 'a' - 'A')	
		{
			enable[i] = c;
			break;
		}
		i++;
	}
 }

/* Vypise velke pismena z pola */
 void print_enable(char *enable)
 {
	int executed = 0;
	printf("Enable: ");
	for(unsigned int i = 0; i < strlen(enable); i++)
	{
		if(isupper(enable[i]))
		{
			putchar(enable[i]);
			executed = 1;
		}
	}
	if(executed == 0)
		putchar(' '); // tlaci medzeru pri dvojslovnych nazvoch
	putchar('\n');
 }

/* Zisti ci je string alphabetic ak ano uprav jeho pismena na velke */
int string_isnot_alpha(char *prefix,int size)
{
	for(int i = 0; i < size; i++)
	{
		if (isalpha(prefix[i]) || prefix[i] == ' ')
		{
			prefix[i] = toupper(prefix[i]);
		}
		else if(prefix[i] != 1) // prvy riadok v txt prazdny == 1 Start of Header
			return 1; 
	}
	return 0;
}

/* Ak je string prefix vypise ho ako FOUND, osetrenie pripadu napr. Brno a Brnovice */
int string_isnot_prefix(int enable_pos, int size,int *str_equal,char *str)
{
		if(enable_pos == size)
		{	
			if(*str_equal == 0)
			{
				printf("Found: %s\n", str);
				*str_equal = 1;
			}
			return 0;
		}
		else return 1;
}

/* Tlacenie vystupu */
void my_print(int str_equal,int enable_count,char *enable,char *found)
{
	if(str_equal == 0)
	{
		if(enable_count > 1)
		{		
			print_enable(enable);	
		}
	
		else if(enable_count == 1)
		{
			printf("Found: %s\n", found);
		}
		else
		{
			printf("Not found\n");	
		}
	}
	else if (enable_count >= 1)
		{
			print_enable(enable);
		}
}

/* Kontrola argumentu */
int safety_check(int argc,char *prefix,int *enable_pos,char *argv)
{	
	if(argc == 1)
	{
		char *pom = "";
		strcpy(prefix,pom);
		*enable_pos = 0;
	}
	else if (argc > 2) 
	{
		fprintf(stderr,COLOR_RED "ERROR: Too many arguments\n" COLOR_RESET );
		return 1;
	}
	else if(strlen(argv) <= 100) 
	{
		strcpy(prefix,argv);
		*enable_pos = strlen(prefix);
	}
	else 
	{
		fprintf(stderr,COLOR_RED"ERROR: Argument is greater than 100 characters\n"COLOR_RESET);
		return 1;
	}

	if(string_isnot_alpha(prefix, strlen(prefix)))
	{
		fprintf(stderr,COLOR_RED"ERROR: Argument is not alphabetic\n"COLOR_RESET);
		return 1;
	}
	else return 0;
}

int main(int argc,char *argv[])
{
	char str[MAX_LEN]; 
	char enable[] = "abcdefghijklmnopqrstuvwxyz";
	char prefix[MAX_LEN];
	char found[MAX_LEN];
	char c;
	int enable_pos;
	int enable_count = 0;
	int str_equal = 0;
	int i = 0;

	if(safety_check(argc, prefix, &enable_pos, argv[1]))
		return EXIT_FAILURE;

	while(scanf("%100[^\n]s", str) != EOF) // scanf az po koniec riadku || 100 znakov
	{
		while ((c = getchar()) != '\n'); // if input > 100 nacitam po koniec riadku

		if(string_isnot_alpha(str, strlen(str)))
		{
			fprintf(stderr,COLOR_RED"IGNORED: %s (is not alphabetic)\n"COLOR_RESET, str);
			continue;
		}
		else if(strcmp(str, found) == 0) // edge case 2x to iste mesto v databaze 
		{
			continue;
		}
		else if(strncmp(prefix, str, enable_pos) == 0)
		{	
			if(string_isnot_prefix(enable_pos,strlen(str),&str_equal,str))
			{
				add_to_array(str[enable_pos], enable);
				enable_count = enable_count + 1;
				strcpy(found, str);
			}
		}
		i++;
	}

	if(i == 1 && str[0] == 1)
	{
		fprintf(stderr,COLOR_RED"ERROR: Textfile is empty\n"COLOR_RESET);
		return EXIT_FAILURE;
	}
	my_print(str_equal, enable_count, enable, found);
	return 0;
}
