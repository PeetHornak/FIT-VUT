#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int sum(int size,int *p)
{
	int sucet = 0;
	for(int i = 0; i < size; i++)
	{
		sucet = sucet + p[i];
	}
	return sucet;
}

void sumof_array(int *p1, int *p2, int *p3, int size)
{
	for(int i = 0; i < size; i++)
	{
		p3[i] = p2[i] + p1[i];
	}

}

void my_printf(char *arr,int size)
{
	for(int i = 1; i <= size; i++)
	{
		putchar(arr[size - i]);
	}
		putchar('\n');
}
int oct2int (char *str,int size)
{
	int dec = 0;
	for(int i = 0; i < size; i++)
	{
		dec = dec + str[i]
		if i != size - 1;
			dec = dec * 8;
	}

}

int main()
{
	int sucet;
	int size = sizeof(arr1)/sizeof(arr1[0]);
	int arr1 [5] = {1,2,3,4,5};
	int arr2 [5] = {1,2,3,4,5};
	int arr3 [5];
	char string[] = {"345"};
	sucet = sum(size, arr1);
	sumof_array(arr1, arr2, arr3, size);
	
	printf("The values of arr3 are: ");
	for(int i = 0; i < size; i++)
	{
		printf("%d ", arr3[i]);
	}
		putchar('\n');
	
	printf("The sum of arr1 is: %d \n", sucet);
	
	my_printf(string, strlen(string));
	
	int dec = oct2int(string,strlen(string));
	printf("%s in octal is equal to %d in decimal \n", string, dec); 
	

	return 0;

}
