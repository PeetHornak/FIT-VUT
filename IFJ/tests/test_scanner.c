#include "../src/header/scanner.h"


int main()
{
    char *buffer= NULL;
    int i,c=0;

	myQueueInit();	
	
	while(1){
		i=getNextToken(&buffer);

		printf("TYPE : %d\n",i);
		if(buffer)
			printf("BUFFER %s\n",buffer );
	}
/*
    ungetToken(i, buffer);
    i= getNextToken(&buffer);

    printf("TYPE : %d\n",i);
    if(buffer){
        printf("BUFFER %s\n",buffer );
	}

*/
	myQueueFree();
    printf("%d\n",c );

	return 0;

}
