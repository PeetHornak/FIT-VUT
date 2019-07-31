#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//pcpad, winscp

int mystrcmp(char* str1, char* str2)
{
	if (strlen(str1) != strlen(str2))
		return -1;
	else
	{
		int i = 0;
		while ( i < strlen(str1) )
		{
			if (str1[i] != str2[i]) 
				return -1;
			i++;
		}
		return 0; 
	}
}

int main(int argc, char* argv[])
{

	double result = 0;

	if (( argc-1) % 2 != 1)
	{
		printf("Wrong number of parameters \n");
		return 0;
	}

	int i = 1;
	result = atof(argv[i]);
	
	while( (argc-1) > i) 
	{
		if (strlen(argv[i+1]) == 1)
		{
			switch (argv[i+1][0])
			{
				case '+':
					result = result + atof(argv[i+2]);
					break;
				case '-':
					result = result - atof(argv[i+2]);
					break;
				case '/':
					result = result / atof(argv[i+2]);
					break;
				default:
					printf("Unsupportedd operation");
			}
		}
		
	
		else
		{
			if(mystrcmp(argv[2],"mod") == 0)
			{
				result = (int)result % atoi(argv[i+2]);
				printf("result = %f\n", result);
			}
			else
			{
				if(mystrcmp(argv[2],"pow") == 0)
				{
					result = pow(result,atof(argv[i+2]));
					printf("result = %f\n", result);
				}
				else
				{
					printf("Unsupported operation\n");
				}
			}
		}
	i = i + 2;
	}

	return 0;
}
