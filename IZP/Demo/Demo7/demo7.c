#include <stdio.h>
#include <stdlib.h>

double cf_phi(int n)
{
	double cf = 0.; // 0. tu sa pracuje s floatom = nepresnosti
	
	for (int k = 0; k < n; k++)
	{
		cf = 1.0/(1.0 + cf);
	}
	

	return 1.0 + cf;	

}

double cf_pi(int n)
{
	double cf = 0.;

	double a, b;
	
	for (int k = n; k > 0; k--)
	{
		a = 2.0;
		b = (2*k - 1);
		b *= b;
		cf = b / (a + cf);
	}
	cf = 1.0 + cf;

	return 4 / cf;

}


int main(int argc, char *argv[])
{	
	if (argc < 2)
		return 1;

	int n = atoi(argv[1]);

	double pi = cf_pi(n);

	printf("%.50lf\n", pi);
	printf("%.50f\n", 0.1);
	return 0;
}
