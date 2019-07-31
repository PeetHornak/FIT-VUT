/*************************************************/
/***************** File: proj2.c *****************/
/**************** Date: 2017/11/5 ****************/
/************* Author: Peter Hornak **************/
/******************* xhorna14 ********************/
/*********** Project: Iteracni vypocty ***********/
/*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *help=
{
	"			Welcome to help page\n"
	"Arguments: --help / --tan A N M / [-c H] -m A [B]\n"
    "	--help: Prints this page\n"
	"	--tan: Compares approximations of tangens between functions\n"
    "		A - Angle in radians 0 < A <= 1.4\n"
    "		N, M - Range of iterations to be printed 0 < N <= M <= 13\n"
    "	-c: Sets height of measuring tool\n"
    "		H - Height 0 < H <= 100\n"
    "	-m: Calculates height and distance of measured object\n"
    "		A - Calculates distance 0 < A <= 1.4\n"
    "		B - Calcuales height 0 < A <= 1.4\n"
    "			Made by Peter Hornak\n"
};

double taylor_tan(double x, unsigned int n)
{
    static long long numer[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
    static long long denom[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};

    double sum = 0.0;
    double pow = x*x;

    for(unsigned int i = 0; i < n; i++)
    {
        sum += x * numer[i] /denom[i];
        x *= pow;
    }
    return sum;
}

double cfrac_tan(double x, unsigned int n )
{
    double numer, denom;
    double cf = 0;

    for(;n > 0; n--)
    {
        numer = 1.0;
        denom = (2 * n - 1) / x;
        cf = numer / (denom - cf);
    }
    return cf;
}

/* Funkcia ktora analyzuje vstup na zaklade --tan a porovnava aproximacie tangensu*/
int parse_tan(int argc,char *argv[])
{
	char *enable;
	double buffer[3] = {0.0};
	if(argc != 5)
		return EXIT_FAILURE;
	for(int i = 2; i < argc; i++) // Pretypovanie na double a kontrola ci je spravny vstup
	{
		buffer[i-2] = strtod(argv[i],&enable);
		if(strcmp(enable,"\0"))
			return EXIT_FAILURE;
	}

	if(buffer[1] > buffer[2])
		return EXIT_FAILURE;
	if(buffer[0] <= 0 || buffer[0] > 1.4)
		return EXIT_FAILURE;
	if(buffer[1] <= 0 || buffer[1] >= 14)
		return EXIT_FAILURE;
	if(buffer[2] <= 0 || buffer[2] >= 14)
		return EXIT_FAILURE;

	for(int i = buffer[1]; i <= buffer[2]; i++)
	{
		double TE = fabs(tan(buffer[0]) - taylor_tan(buffer[0],i));
		double CE = fabs(tan(buffer[0]) - cfrac_tan(buffer[0],i));
		printf("%d %e %e %e %e %e\n", i, tan(buffer[0]), taylor_tan(buffer[0],i), TE, cfrac_tan(buffer[0],i), CE);
	}
	return EXIT_SUCCESS;
}

/* Jednoducha funkcia ktora analyzuje input v pripade -m a na zaklade toho pocita vzdialenosta vysku */
int parse_m(int argc,char *argv[], double c)
{
	char *enable;
	double buffer[2] = {0.0};
	double distance,height;

	for(int i = 2; i < argc; i++) // Pretypovanie na double a konrola vstupu
	{
		buffer[i-2] = strtod(argv[i], &enable);
		if(strcmp(enable,"\0"))
			return EXIT_FAILURE;
	}
/* 8 magicka konstanta, v najhorsom priprade --tan 1.4 je presnost na 10 desatin. v 8. iteracii */
	distance = c / cfrac_tan(buffer[0], 8); // vypocet vzdialenosti
	height = cfrac_tan(buffer[1], 8) * distance + c; // vypocet vysky 

	if(argc == 3)
	{
		if(0 < buffer[0] && buffer[0] <=1.4)
		{
			printf("%.10e\n", distance);
			return EXIT_SUCCESS;
		}
		else return EXIT_FAILURE;
	}
	else if(argc == 4)
	{
		if(0 < buffer[0]  && buffer[0] <=1.4 && 0 < buffer[1]  && buffer[1] <=1.4)
		{
			printf("%.10e\n", distance);
			printf("%.10e\n", height);
			return EXIT_SUCCESS;
		}
		else 
			return EXIT_FAILURE;
	}
	else 
		return EXIT_FAILURE;
}

/* Funkcia na kontrolu inputu v pripade -c */
int parse_c(int argc, char *argv[])
{
	double c;
	char *enable;
	
	if(strcmp(argv[3],"-m"))
		return EXIT_FAILURE;
	if(argc == 5 || argc == 6)
	{
		c = strtod(argv[2], &enable);
		if(c <= 0 || c > 100)
			return EXIT_FAILURE;
		if(strcmp(enable,"\0"))
			return EXIT_FAILURE;
		else	
			return parse_m(argc - 2, argv + 2, c); //ak je spravny vstup odignoruje prve dva arg = funguje ako ./proj2 -m A B
	}
	else return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
	double c = 1.5;
	if(argc == 1)
	{
		fprintf(stderr,"ERROR: Wrong arguments, use '--help'\n");
		return EXIT_FAILURE;
	}
	if(!strcmp(argv[1],"--help") && argc == 2)
		printf(help);
	else if(!strcmp(argv[1],"-m"))
	{
		if(parse_m(argc, argv, c))
			{
				fprintf(stderr,"ERROR: Wrong arguments, use '--help'\n");
				return EXIT_FAILURE;
			}
	}
	else if (!strcmp(argv[1],"-c"))
	{
		if(parse_c(argc, argv))
			{
				fprintf(stderr,"ERROR: Wrong arguments, use '--help'\n");
				return EXIT_FAILURE;
			}
	}
	else if(!strcmp(argv[1],"--tan"))
	{
		if(parse_tan(argc, argv))
			{
				fprintf(stderr,"ERROR: Wrong arguments, use '--help'\n");
				return EXIT_FAILURE;
			}
	}
	else
	{
		fprintf(stderr,"ERROR: Wrong arguments, use '--help'\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
