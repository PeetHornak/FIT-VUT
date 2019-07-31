#include <stdio.h>

void swap(int *p_a, int *p_b)
{
	*(p_b) = *(p_a) + *(p_b);	// 5 == 2 + 3
	*(p_a) = *(p_b) - *(p_a);	// 3 == 5 - 2
	*(p_b) = *(p_b) - *(p_a);	// 2 == 5 - 3 
}

int main()
{	
	int a = 2;
	int b = 3;
	int *p_a = &a;
	int *p_b = &b;
	swap(&p_a,&p_b);
	printf("%d \t %d \n", a, b);
	return 0;
}
