#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void multconst(double *dest, double *src, int n, int k)
{
	for(int i = 0; i < n; i++)
	{
		dest[i] = src[i] * k;
		printf("dest[%d]: %lf\n", i, dest[i]);
	}
}

int main(int argc, char *argv[])
{
	(void)argc;
	int n = atoi(argv[1]);
    double *src;
    double *dest;
    src = (double *) malloc(n*sizeof(double));
    dest = (double *) malloc(n*sizeof(double));

    for(int i = 0; i < n; i++)
	{
		src[i] = i;
	}

	multconst(dest, src, n, 10);

    return 42;
}

