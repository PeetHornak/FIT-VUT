#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>


double my_function(double y, int i)
{
	y *= (y*y) / (i*(i+1));
	return y;
}

int main(int argc, char *argv[])
{	
	(void)argc;
	double y_o = atof(argv[1]);
	double sum = y_o;
	double y = y_o;
	double eps = atof(argv[2]);
	int i = 2;
	int j = 1;
	do
	{
		y = my_function(y,i);
		i = i + 2;
		if(j % 2 == 0)
			sum = sum + y;
		else
			sum = sum - y;
		j++;

	}while(fabs(y) > eps);

	printf("%.20f\n",sum);
	return 0;
}
