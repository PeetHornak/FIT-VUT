#include <stdio.h>
#include <stdlib.h>

int in_interval(int x, int a, int b)
{
	if (x > a && x < b)
		return 1;
	else 
		return 0;
}

int main()
{
	double x,a,b;
	scanf("%lf %lf %lf", &x, &a, &b);
	
	if (in_interval(x,a,b) == 1)
		printf("Yes\n");
	else
		printf("No\n");
	
	return 0;
}

