#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct matrix
{
	int c;
	int r;
	double *m;

}Tmatrix;

Tmatrix m_alloc1(int row, int col)
{
	double *m = (double *) malloc(sizeof(double)*row*col);	
	Tmatrix matrix;
	matrix.c = col;
	matrix.r = row;
	matrix.m = m;
	for(int i = 0; i < row*col; i++)
	{
		m[i] = i;
	}
	return matrix;
}

Tmatrix *m_alloc2(int row, int col)
{
	double *m = (double *) malloc(sizeof(double)*row*col);
	Tmatrix *matrix = (Tmatrix *) malloc(sizeof(Tmatrix));
	matrix->c = col;
	matrix->r = row;
	matrix->m = m;

	for(int i = 0; i < row*col; i++)
		matrix->m[i] = i;

	return matrix;
}

void my_print(Tmatrix *matrix)
{
	int print = 0;
	for(int i = 0; i < matrix->r; i++)
	{
		for(int j = 0; j < matrix->c; j++)
		{
			printf("%lf ", matrix->m[print]);
			print++;
		}
		printf("\n");
	}
	printf("\n");
}

double *mi(Tmatrix *matrix, int r, int c)
{
	return &matrix->m[matrix->];
}

int main()
{
	int rows = 5;
	int cols = 6;
	Tmatrix matrix1;
	matrix1 = m_alloc1(rows,cols);
	my_print(&matrix1);
	Tmatrix *matrix2;
	matrix2 = m_alloc2(rows,cols);
	my_print(matrix2);
	double *number = mi(&matrix1, 2, 3);
	printf("matrix[2,3] is equal to: %lf\n", *number);
	return 42;
}
