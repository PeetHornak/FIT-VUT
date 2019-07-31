#include <stdio.h>
#include <string.h>

int main()
{
	//
	int terminate = 0;
	char slovo[11];
	
	while (terminate == 0)
	{
		scanf("%10s" , slovo);
		if (strcmp(slovo,"exit") == 0)
		{
			terminate = 1;
		}
		else
		{
			
			int i = 0;
			while(i < strlen(slovo))
			{
				printf("%c\n", slovo[i]);
				i++;
			}
		}
	}
	return 0;
}
