#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#define M 7
	
int matice[M][M] = 	
{
    {'m','o','C','\n','?','r','e',},
	{'p','a','h','W','\n','!','w',},
	{'u','t','o','l','l','d','s',},
	{'t',' ',',','H','e','l','n',},
	{'i','i',' ','w','o','r','a',},
	{'n','s',' ','t','h','e',' ',},
	{'g','.','.','.','4','2','\n',},
};

void zpracuj(int r, int c)
{
	putchar(matice[r][c]);
}

int jsem_v_matici(int r, int c)
{
	if(r < 0 || c < 0 || r >= M || c >= M)
		return 0;
	else
		return 1;
}

enum direction {V, S, Z, J};

struct delta{
	int dr;
	int dc;
};

int main()
{
	struct delta delta[4] = {
		{0,1}, //V
		{-1,0}, //S
		{0,-1}, //Z
		{1,0}, //J
	};

	int ddir[4] = {
		S, //V
		Z, //S
		J, //Z
		V, //J
	};

	int dsteps[4] = {1, 0, 1, 0};

	int r, c;
	r = c = M/2;

	int dir = V;

	int k = 1;

	while(1)
	{
	
		for (int i = 0; i < k; i++)
		{
			if(! jsem_v_matici(r, c))
				break;
			zpracuj(r, c);
			r = r + delta[dir].dr;
			c = c + delta[dir].dc;
		}
		if(! jsem_v_matici(r, c))
			break;
		
		dir = ddir[dir];
	
		k = k + dsteps[dir];
	}

	return 42;
}	
