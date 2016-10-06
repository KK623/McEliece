#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
unsigned int power(unsigned int base, unsigned int exponent)
{
	unsigned int result = 1;

	for (int i = 1; i <= exponent; i++)
		result *= base;
	
	return result;
}

int inverseMatrix(unsigned int** matrix, unsigned int** inverse, unsigned int size)
{
	unsigned int** clone = (unsigned int**)malloc(sizeof(unsigned int*) * size);

	for (int i = 0; i < size; i++)
	{
		clone[i] = (unsigned int*)malloc(sizeof(unsigned int) * size);
		for (int j = 0; j < size; j++)
		{
			clone[i][j] = matrix[i][j];
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (clone[i][i] == 0 && clone[j][i] != 0)
			{
				unsigned int* swap = clone[i];
				clone[i] = clone[j];
				clone[j] = swap;

				swap = inverse[i];
				inverse[i] = inverse[j];
				inverse[j] = swap;
			}
			else if (clone[j][i] != 0)
			{
				for (int k = 0; k < size; k++)
				{
					clone[j][k] = clone[j][k] ^ clone[i][k];
					inverse[j][k] = inverse[j][k] ^ inverse[i][k];
				}
			}
		}
		
		if (clone[i][i] == 0)
		{
			return 0;
		}

		for (int j = 0; j < i; j++)
		{
			if (clone[j][i] != 0)
			{
				for (int k = 0; k<size; k++)
				{
					clone[j][k] = clone[j][k] ^ clone[i][k];
					inverse[j][k] = inverse[j][k] ^ inverse[i][k];
				}
			}
		}
	}


	printf("inverse matrix\n");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", inverse[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("multiplication : A * X\n");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int sum; sum = 0;
			for (int k = 0; k < size; k++)
			{
				sum ^= matrix[i][k] * inverse[k][j];
			}
			printf("%d ", (sum));
		
		}
		printf("\n");
	}
	printf("\n");

	free(clone);

	return 1;
}

void nonSingularMatrix(unsigned int** matrix, unsigned int** identity, unsigned int size)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = rand() % 2;

			identity[i][j] = 0;
			if (i == j)
			{
				identity[i][j] = 1;
			}
			//printf("%d ", matrix[i][j]);
		}
		//printf("\n");
	}
}

void permutationMatrix(unsigned int** matrix, unsigned int** identity, unsigned int size)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = 0;
			identity[i][j] = 0;
			if (i == j)
			{
				matrix[i][j] = 1;
				identity[i][j] = 1;
			}
		}

		int idx = rand() % (i+1);
		if (i != idx)
		{
			unsigned int* swap = matrix[i];
			matrix[i] = matrix[idx];
			matrix[idx] = swap;
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

void main()
{
	unsigned int r=2;
	unsigned int m=4;
	
	unsigned int n=16;	//code length
	unsigned int k=11;	//message length

	srand((unsigned)time(NULL));
	
	unsigned int** G = (unsigned int**)malloc(sizeof(unsigned int*) * k);
	unsigned int** S = (unsigned int**)malloc(sizeof(unsigned int*) * k);
	unsigned int** SInverse = (unsigned int**)malloc(sizeof(unsigned int*) * k);

	for (int i = 0; i < k; i++)
	{	
		G[i] = (unsigned int*)malloc(sizeof(unsigned int) * n);
		S[i] = (unsigned int*)malloc(sizeof(unsigned int) * k);
		SInverse[i] = (unsigned int*)malloc(sizeof(unsigned int) * k);
	}

	while(1)
	{
		nonSingularMatrix(S, SInverse, k);

		if (inverseMatrix(S, SInverse, k) != 0)
			break;

		printf("SingularMatrix\n");
	}

	printf("nonSingularMatrix\n");
	printf("\n"); printf("\n");

	
	unsigned int** P = (unsigned int**)malloc(sizeof(unsigned int*) * n);
	unsigned int** PInverse = (unsigned int**)malloc(sizeof(unsigned int*) * n);
	for (int i = 0; i < n; i++)
	{		
		P[i] = (unsigned int*)malloc(sizeof(unsigned int) * n);
		PInverse[i] = (unsigned int*)malloc(sizeof(unsigned int) * n);
	}
	printf("permutationMatrix\n");
	permutationMatrix(P, PInverse, n);
	inverseMatrix(P, PInverse, n);
	
	for (int i = 0; i < n; i++)
	{
		G[0][i] = 1;
	}

	for (int i = 1; i <= m; i++)
	{
		static int k = 0;
		int size = power(2, i-1);		
		for (int j = 0; j < size; j++)
		{
			G[i][k+j] = 0;	
		}
		k++;
		for (int j = 0 + 1; j < size; j++)
		{
			G[i][(k*)+j] = 1;	
		}
		k++;
	}


	unsigned int* msg = (unsigned int*)malloc(sizeof(unsigned int) * k);
	for (int i = 0; i < k; i++)
	{
		msg[i] = rand() % 2;
	}

	unsigned int* error = (unsigned int*)malloc(sizeof(unsigned int) * n);
	for (int i = 0; i < n; i++)
	{
		error[i] = 0;
	}


	const int maxErr = 2;
	while (1)
	{
		static int i = 0;
		unsigned int idx = rand() % n;

		if (error[idx] == 0)
		{
			error[idx] = 1;
			i++;
		}

		if (i > maxErr)
		{
			break;
		}
	}



	for (int i = 0; i < n; i++)
	{
		free(P[i]);	
	}
	free(P);
	
	for (int i = 0; i < k; i++)
	{
		free(SInverse[i]);
		free(S[i]);
		free(G[i]);
	}
	free(SInverse);
	free(S);
	free(G);
}