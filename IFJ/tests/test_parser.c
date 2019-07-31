#include <stdio.h>
#include "../src/header/parser.h"

int main()
{

	printf("------------SPUSTANIE TESTU------------");
	printf("---------------------------------------");


	int return_value;
	
	//	Test na bezchybny program
	printf("Test na spravny kod");
	return_value = parse();
	fprintf(stdin, "variable = 120\n second = variable + 20\n%d", EOF);

	switch(return_value){
		case 0:
			printf("Ziadna chyba");
			break;
		case 2:
			printf("Chyba v syntaktickej analyzy");
			break;
		default:
			 printf("Test bol neuspesny");
	}

	//	test na chybu v lexikalnej analyze
	printf("Test na nespravne priradenie");
	return_value = parse();
	fprintf(stdin, "variable = 120\n second = variable + 20\n 20 = 1\n%d", EOF);

	switch(return_value){
		case 0:
			printf("Ziadna chyba");
			break;
		case 2:
			printf("Chyba v syntaktickej analyzy");
			break;
	}

	printf("Test na nespravnu definiciu funkciu");
	return_value = parse();
	fprintf(stdin, "variable = 120\n second = variable + 20\ndef foo(first second)\nvariable = second +31\n%d", EOF);

	switch(return_value){
		case 0:
			printf("Ziadna chyba");
			break;
		case 2:
			printf("Chyba v syntaktickej analyzy");
			break;
	}

	return 0;
}
