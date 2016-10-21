#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <string.h>

unsigned int combination(unsigned int n, const unsigned int& k);
unsigned int power(const unsigned int& exponent);

int inverseMatrix(unsigned char **matrix, unsigned char **inverse, const unsigned int& size);
void nonSingularMatrix(unsigned char **matrix, unsigned char **identity, const unsigned int& size);
void permutationMatrix(unsigned char **matrix, unsigned char **identity, const unsigned int& size);
void mtrixMultiplication(const unsigned int& pRow, const unsigned int& pCol, unsigned char **prev, const unsigned int& nRow, const unsigned int& nCol, unsigned char **next, unsigned char **result);
void errorAdd(const unsigned int& size, unsigned char *original, unsigned char *error, unsigned char *result);
void errorGenerator(const unsigned int& weight, const unsigned int& size, unsigned char *error);
void generatePublicKey(const unsigned int& k, const unsigned int& n, unsigned char **S, unsigned char **G, unsigned char **P, unsigned char **key, const unsigned int& t, unsigned char *z);
void generateMatrixMultiple(const int& n, const int& m, const int& lim, unsigned char **matrix, unsigned int& idx, int *row, int rowIdx, int& rowValue);
void generateMatrix(const int& r, const int& m, const int& n, unsigned char **matrix);

void msgEncrypt(const unsigned int& k, unsigned char **msg, unsigned char **publicKey, const unsigned int& n, unsigned char **error, unsigned char **prime, unsigned char **c);
void msgDecrypt(const unsigned int& n, unsigned char **c, unsigned char **PInverse, const unsigned int& k, unsigned char **SInverse, unsigned char **msg, unsigned char **GRM, const unsigned int& r, const unsigned int& m);

void rmRUpdate(unsigned int& start, unsigned int& end, unsigned char **recovery, unsigned char **G, unsigned char **codeword, const unsigned int& n);
unsigned char rmDecision(unsigned char *array, unsigned int arrayLength);
void rmIdxCalculator(unsigned int *array, const int& arrayLen, unsigned int& arrayPointer, unsigned int *result, unsigned int& resultIdx, unsigned int& resultValue);
void rmSCalculate(unsigned int *row, const int& rowLength, unsigned int *S);
void rmECalculate(unsigned int *row, const int& rowLength, const int& m, unsigned int *E);
void rmSCCalculate(unsigned int *row, const int& rowLength, unsigned int *SC);
unsigned char rmACalculate(unsigned int& SLength, unsigned int *S, const unsigned int& SCLength, unsigned int *SC, unsigned char **codeword);
void rmDecoding(const unsigned int& n, const unsigned int& m, const unsigned int& rowLength, unsigned unsigned int* row, unsigned int rowIdx, unsigned int& rowValue, unsigned char **codeword, unsigned char **recovery, unsigned int& recoveryIdx);
void rmDecoder(const unsigned int& r, const unsigned int& m, unsigned char **GRM, const unsigned int& n, unsigned char **codeword, unsigned char **recovery);


double* rcsvRight(int  r, int  m, double *orgMsg, int orgMsgLength, double *result, int resultLength);
double* rcsvLeft(int  r, int  m, double *orgMsg, int orgMsgLength, double *v, int vLength, double *result, int resultLength);
void rcsvDecisionZero(double *orgMsg, int orgMsgLength, double *result, int resultLength);
void rcsvDecisionEqual(double *orgMsg, int orgMsgLength, double *result, int resultLength);
void rcsvDecodingHard(int  r, int m, unsigned char *recvCodeword, int recvCodewordLength);

void main()
{
	unsigned int r = 5; //4; 5
	unsigned int m = 12; //10; 12

	unsigned int n=0; //code length
	unsigned int k=0; //message length	

	srand((unsigned)time(NULL));

	n = power(m);
	for (int i = 0; i <= r; i++)
	{
		k += combination(m, i);
	}
	
	printf("n = %d, k = %d\n\n", n, k);

	unsigned char** publicKeyG = (unsigned char**)malloc(sizeof(unsigned char*) * k);
	unsigned int publicKeyt = 38; //31

	unsigned char** G = (unsigned char**)malloc(sizeof(unsigned char*) * k);
	unsigned char** S = (unsigned char**)malloc(sizeof(unsigned char*) * k);
	unsigned char** SInverse = (unsigned char**)malloc(sizeof(unsigned char*) * k);

	for (int i = 0; i < k; i++)
	{	
		publicKeyG[i] = (unsigned char*)malloc(sizeof(unsigned char) * n);
		G[i] = (unsigned char*)malloc(sizeof(unsigned char) * n);

		for(int j=0; j<n; j++)
		{
			G[i][j] = 1;
		}

		S[i] = (unsigned char*)malloc(sizeof(unsigned char) * k);
		SInverse[i] = (unsigned char*)malloc(sizeof(unsigned char) * k);
	}	

	//printf("S ");
	while(1)
	{
		nonSingularMatrix(S, SInverse, k); //generate S

		if (inverseMatrix(S, SInverse, k) != 0) //calcurate S inverse
			break;
	}

	printf("S\n");
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			printf("%d ", S[i][j]);
		}
		printf("\n");
	}
	printf("\n"); printf("\n");


	generateMatrix(r, m, n, G); //generate G
	

	/*printf("G\n");
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%d ", G[i][j]);
		}
		printf("\n");
	}
	printf("\n"); printf("\n");*/

	unsigned char** P = (unsigned char**)malloc(sizeof(unsigned char*) * n);
	unsigned char** PInverse = (unsigned char**)malloc(sizeof(unsigned char*) * n);
	for (int i = 0; i < n; i++)
	{		
		P[i] = (unsigned char*)malloc(sizeof(unsigned char) * n);
		PInverse[i] = (unsigned char*)malloc(sizeof(unsigned char) * n);
	}

	//printf("P ");
	permutationMatrix(P, PInverse, n); //generate P
	inverseMatrix(P, PInverse, n); //calcurate P inverse	
	/*printf("P\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%d ", P[i][j]);
		}
		printf("\n");
	}
	printf("\n"); printf("\n");*/


	unsigned char** z = (unsigned char**)malloc(sizeof(unsigned char*) * 1);
	z[0] = (unsigned char*)malloc(sizeof(unsigned char) * n);
	for (int i = 0; i < n; i++)
	{
		z[0][i] = 0;
	}
	
	generatePublicKey(k, n, S, G, P, publicKeyG, publicKeyt, z[0]); //generate public key G' and error pattern z
	/*printf("public key G'\n");
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%d ", publicKeyG[i][j]);
		}
		printf("\n");
	}
	printf("\n"); printf("\n");
*/
	clock_t before = clock();
	double spendTime;

for (int msgLoop = 0; msgLoop < 1; msgLoop++)
{
	unsigned char** msg = (unsigned char**)malloc(sizeof(unsigned char*) * 1);
	msg[0] = (unsigned char*)malloc(sizeof(unsigned char) * k);

	for (int i = 0; i < k; i++)
	{
		msg[0][i] = rand() % 2;
	}

	/*printf("msg\n");
	for (int i = 0; i < k; i++)
	{
		printf("%d ", msg[0][i]);
	}
	printf("\n"); printf("\n");*/


	/*printf("z\n");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", z[0][i]);
	}
	printf("\n"); printf("\n");*/

	unsigned char** cPrm = (unsigned char**)malloc(sizeof(unsigned char*) * 1);
	cPrm[0] = (unsigned char*)malloc(sizeof(unsigned char) * n);

	unsigned char** c = (unsigned char**)malloc(sizeof(unsigned char*) * 1);
	c[0] = (unsigned char*)malloc(sizeof(unsigned char) * n);

	for (int tLoop = publicKeyt; tLoop <= publicKeyt; tLoop++)
	{
		msgEncrypt(k, msg, publicKeyG, n, z, cPrm, c); //encrypt meassage


		unsigned char** decipher = (unsigned char**)malloc(sizeof(unsigned char*) * 1);
		decipher[0] = (unsigned char*)malloc(sizeof(unsigned char) * k);
		msgDecrypt(n, c, PInverse, k, SInverse, decipher, G, r, m); //decrypt recived meassage

		/*printf("decipher\n");
		for (int i = 0; i < k; i++)
		{
			printf("%d ", decipher[0][i]);
		}
		printf("\n"); printf("\n");*/


		for (int i = 0; i < k; i++)
		{
			if (msg[0][i] != decipher[0][i])
			{
				printf("error msg[%d][%d] : %d\n", msgLoop, tLoop, i);
				break;
			}
		}
		//printf("\n"); printf("\n");


		free(decipher[0]);
		free(decipher);
	}

	free(cPrm[0]);
	free(cPrm);
	free(c[0]);
	free(c);
	
	free(msg[0]);
	free(msg);

}
spendTime = (double)(clock() - before) / CLOCKS_PER_SEC;

printf("걸린시간은 %5.2f 입니다.\n", spendTime);

	free(z[0]);
	free(z);
	for (int i = 0; i < n; i++)
	{
		free(PInverse[i]);
		free(P[i]);	
	}
	free(PInverse);
	free(P);

	for (int i = 0; i < k; i++)
	{
		free(publicKeyG[i]);
		free(SInverse[i]);
		free(S[i]);
		free(G[i]);
	}
	free(publicKeyG);
	free(SInverse);
	free(S);
	free(G);
}

unsigned int combination(unsigned int n, const unsigned int& k)
{
	if (k > n) return 0;

	unsigned int result = 1;
	for (unsigned int i = 1; i <= k; i++)
	{
		result *= n--;
		result /= i;
	}

	return result;
}

unsigned int power(const unsigned int& exponent)
{
	return (1 << exponent);
}

int inverseMatrix(unsigned char** matrix, unsigned char** inverse, const unsigned int& size)
{
	unsigned char** clone = (unsigned char**)malloc(sizeof(unsigned char*) * size);

	for (int i = 0; i < size; i++)
	{
		clone[i] = (unsigned char*)malloc(sizeof(unsigned char) * size);
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
				unsigned char* swap = clone[i];
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


	/*printf("inverse matrix\n");
	for (int i = 0; i < size; i++)
	{
	for (int j = 0; j < size; j++)
	{
	printf("%d ", inverse[i][j]);
	}
	printf("\n");
	}
	printf("\n"); printf("\n");

	printf("verify inverse matrix\n");
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
	printf("\n"); printf("\n");*/


	for (int i = 0; i < size; i++)
	{
		free(clone[i]);
	}
	free(clone);

	return 1;
}

void nonSingularMatrix(unsigned char** matrix, unsigned char** identity, const unsigned int& size)
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
		}
	}
}

void permutationMatrix(unsigned char** matrix, unsigned char** identity, const unsigned int& size)
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

		int idx = rand() % (i + 1);
		if (i != idx)
		{
			unsigned char* swap = matrix[i];
			matrix[i] = matrix[idx];
			matrix[idx] = swap;
		}
	}
}

void mtrixMultiplication(const unsigned int& pRow, const unsigned int& pCol, unsigned char** prev, const unsigned int& nRow, const unsigned int& nCol, unsigned char** next, unsigned char** result)
{
	for (int i = 0; i < pRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			unsigned char sum = 0;
			for (int z = 0; z < pCol; z++)
			{
				sum ^= (prev[i][z] * next[z][j]);
			}
			result[i][j] = sum;
			//printf("%d ", result[i][j]);
		}
		//printf("\n");
	}
}

void errorAdd(const unsigned int& size, unsigned char* original, unsigned char* error, unsigned char* result)
{
	for (int i = 0; i < size; i++)
	{
		result[i] = original[i];

		if (error[i] == 1)
		{
			if (original[i] == 0)
			{
				result[i] = 1;
			}
			else
			{
				result[i] = 0;
			}
			//result[i] = (~original[i])%2;
		}
	}

}

void errorGenerator(const unsigned int& weight, const unsigned int& size, unsigned char* error)
{
	srand((unsigned)time(NULL));

	int num = 0;

	while (1)
	{
		unsigned int idx = rand() % size;

		if (error[idx] == 0)
		{
			error[idx] = 1;
			num++;
		}

		if (num >= weight)
		{
			break;
		}
	}
}

void generatePublicKey(const unsigned int& k, const unsigned int& n, unsigned char** S, unsigned char** G, unsigned char** P, unsigned char** key, const unsigned int& t, unsigned char* z)
{
	unsigned char** pub = (unsigned char**)malloc(sizeof(unsigned char*) * k);

	for (int i = 0; i < k; i++)
	{
		pub[i] = (unsigned char*)malloc(sizeof(unsigned char) * n);
	}

	mtrixMultiplication(k, k, S, k, n, G, pub);
	mtrixMultiplication(k, n, pub, n, n, P, key);

	errorGenerator(t, n, z);

	for (int i = 0; i < k; i++)
	{
		free(pub[i]);
	}
	free(pub);
}

//void generateMatrixMultiple(const int& n, const int& m, const int& lim, unsigned char** matrix, unsigned int& idx, int* row, int rowIdx, int& rowValue)
//{
//	if (lim <= rowIdx)
//	{
//		int i = 0;
//		//printf("%d : ", idx);
//		while (i != rowIdx)
//		{
//			//printf("%d ", row[i]);
//			for (int j = 0; j < n; j++)
//			{
//				matrix[idx][j] *= matrix[row[i]][j];
//			}
//			i++;
//		}
//		idx++;
//		//printf("\n");
//		return;
//	}
//
//	for (int i = rowValue + 1; i <= m; i++)
//	{
//		row[rowIdx] = i;
//		generateMatrixMultiple(n, m, lim, matrix, idx, row, rowIdx + 1, i);
//	}
//
//}

void generateMatrixMultiple(const int& n, const int& m, const int& lim, unsigned char** matrix, unsigned int& idx, int* row, int rowIdx, int& rowValue)
{
	if (lim <= rowIdx)
	{
		int i = 0;
		//printf("%d : ", idx);
		while (i != rowIdx)
		{
			//printf("%d ", row[i]);
			for (int j = 0; j < n; j++)
			{
				matrix[idx][j] *= matrix[row[i]][j];
			}
			i++;
		}
		idx++;
		//printf("\n");
		return;
	}

	for (int i = rowValue - 1; i >= 1; i--)
	{
		row[rowIdx] = i;
		generateMatrixMultiple(n, m, lim, matrix, idx, row, rowIdx + 1, i);
	}

}

void generateMatrix(const int& r, const int& m, const int& n, unsigned char** matrix)
{
	/*for (int i = 0; i < n; i++)
	{
	matrix[0][i] = 1;
	}*/

	for (int i = 1; i <= m; i++)
	{
		static unsigned char temp = 0;
		int z = 0;
		for (int j = 0; j < n / power(i - 1); j++)
		{
			for (int k = 0; k < power(i - 1); k++)
			{
				matrix[i][z] = temp;
				z++;
			}
			temp = temp == 0 ? 1 : 0;
		}
	}

	/*for (int i = 2; i <= r; i++)
	{
	int* multiplRows = (int*)malloc(sizeof(int) * r);
	for (int j = 1; j <= m; j++)
	{
	int rowIdx = 0;
	static unsigned int idx = m + 1;
	multiplRows[rowIdx] = j;
	generateMatrixMultiple(n, m, i, matrix, idx, multiplRows, rowIdx + 1, j);
	}
	free(multiplRows);
	}*/

	for (int i = r; i > 1; i--)
	{
		int* multiplRows = (int*)malloc(sizeof(int) * r);
		for (int j = m; j >= 1; j--)
		{
			int rowIdx = 0;
			static unsigned int idx = m + 1;
			multiplRows[rowIdx] = j;
			generateMatrixMultiple(n, m, i, matrix, idx, multiplRows, rowIdx + 1, j);
		}
		free(multiplRows);
	}

}

void msgEncrypt(const unsigned int& k, unsigned char** msg, unsigned char** publicKey, const unsigned int& n, unsigned char** error, unsigned char** prime, unsigned char** c)
{
	mtrixMultiplication(1, k, msg, k, n, publicKey, prime);

	/*for (int i = 0; i < n; i++)
	{
	c[0][i] = prime[0][i];
	}*/

	/*printf("c prime\n");
	for (int i = 0; i < n; i++)
	{
	printf("%d ", prime[0][i]);
	}
	printf("\n"); printf("\n");*/

	errorAdd(n, prime[0], error[0], c[0]);
	/*printf("c\n");
	for (int i = 0; i < n; i++)
	{
	printf("%d ", c[0][i]);
	}
	printf("\n"); printf("\n");*/

}


//void SCalculate(int* row, const int& lim, unsigned int* S)
//{
//	S[0] = 0;
//
//	for (int i = lim - 1; i >= 0; i--)
//	{
//		S[i] += S[i - 1] + power(row[i] - 1);
//	}
//}
//
//void RMDecoding(const int& n, const int& m, const int& lim, int* row, int rowIdx, int& rowValue, unsigned int& idx)
//{
//	if (lim <= rowIdx)
//	{
//		int i = 0;
//		//printf("%d : ", idx);
//
//		unsigned int* S = (unsigned int*)malloc(sizeof(unsigned int) * power(m - lim));
//		SCalculate(row, lim, S);
//
//		while (i != rowIdx)
//		{
//			//printf("%d ", row[i]);
//
//			i++;
//		}
//		idx++;
//		//printf("\n");
//		return;
//	}
//
//	for (int i = rowValue - 1; i >= 1; i--)
//	{
//		row[rowIdx] = i;
//		rmDecoding(n, m, lim, row, rowIdx + 1, i, idx);
//	}
//}

//void RMDecoder(unsigned char** GRM, const unsigned int& k, const unsigned int& n, unsigned char** c, unsigned char** msg, const unsigned int& r, const unsigned int& m)
//{
//	for (int i = r; i >= 2; i--)
//	{
//		int* multiplRows = (int*)malloc(sizeof(int) * r);
//		for (int j = m; j >= 1; j--)
//		{
//			int rowIdx = 0;
//			static unsigned int idx = m + 1;
//			multiplRows[rowIdx] = j;
//			RMDecoding(n, m, i, multiplRows, rowIdx + 1, j, idx);
//		}
//		free(multiplRows);
//	}
//}




void rmRUpdate(unsigned int& start, unsigned int& end, unsigned char **recovery, unsigned char **G, unsigned char **codeword, const unsigned int& n)
{
	for (int i = 0; i < n; i++)
	{
		unsigned char sum = 0;
		for (int j = start; j < end; j++)
		{
			sum += (recovery[0][j] * G[j][i]);
		}

		codeword[0][i] = (codeword[0][i] + sum) % 2;
	}
}

unsigned char rmDecision(unsigned char *array, unsigned int arrayLength)
{
	short zeroCount = 0;
	short oneCount = 0;

	for (int i = 0; i < arrayLength; i++)
	{
		if (0 == array[i])
		{
			zeroCount++;
		}
		else
		{
			oneCount++;
		}
	}

	if (zeroCount > oneCount)
		return 0;
	else if (zeroCount < oneCount)
		return 1;
	else
		return rand() % 2;
}


void rmIdxCalculator(unsigned int *array, const int& arrayLen, unsigned int& arrayPointer, unsigned int *result, unsigned int& resultIdx, unsigned int& resultValue)
{
	if (arrayLen == arrayPointer)
	{
		arrayPointer--;

		result[resultIdx] = resultValue;
		resultIdx++;

		return;
	}

	for (int i = 0; i < 2; i++)
	{
		unsigned short temp = power(array[arrayPointer]) * i;
		resultValue += temp;

		arrayPointer++;
		rmIdxCalculator(array, arrayLen, arrayPointer, result, resultIdx, resultValue);

		resultValue -= temp;
	}
	arrayPointer--;
}

void rmSCalculate(unsigned int *row, const int& rowLength, unsigned int *S)
{
	unsigned int *rowCopy = (unsigned int *)malloc(sizeof(unsigned int) * rowLength);

	for (int i = 0; i < rowLength; i++)
	{
		rowCopy[i] = row[i] - 1;
	}

	unsigned int SValue = 0;
	unsigned int SIdx = 0;

	unsigned int rowPointer = 0;

	rmIdxCalculator(rowCopy, rowLength, rowPointer, S, SIdx, SValue);

	free(rowCopy);

}

void rmECalculate(unsigned int *row, const int& rowLength, const int& m, unsigned int *E)
{

	int k = 0;
	for (int i = 0; i < m; i++)
	{
		int temp = 0;
		for (int j = 0; j < rowLength; j++)
		{
			if (i == (row[j] - 1))
			{
				temp = 1;
				break;
			}
		}

		if (temp == 0)
		{
			E[k] = i;
			k++;
		}
	}
}


void rmSCCalculate(unsigned int *row, const int& rowLength, unsigned int *SC)
{
	unsigned int SCValue = 0;
	unsigned int SCIdx = 0;

	unsigned int rowPointer = 0;

	rmIdxCalculator(row, rowLength, rowPointer, SC, SCIdx, SCValue);
}

unsigned char rmACalculate(unsigned int& SLength, unsigned int *S, const unsigned int& SCLength, unsigned int *SC, unsigned char **codeword)
{
	unsigned char *A = (unsigned char *)malloc(sizeof(unsigned char) * SCLength);

	unsigned char val;
	for (int i = 0; i < SCLength; i++)
	{
		unsigned char sum = 0;

		for (int j = 0; j < SLength; j++)
		{
			sum += codeword[0][S[j] + SC[i]];
		}
		A[i] = sum % 2;
	}

	val = rmDecision(A, SCLength);

	free(A);

	return val;
}



void rmDecoding(const unsigned int& n, const unsigned int& m, const unsigned int& rowLength, unsigned unsigned int* row, unsigned int rowIdx, unsigned int& rowValue, unsigned char **codeword, unsigned char **recovery, unsigned int& recoveryIdx)
{
	if (rowLength <= rowIdx)
	{
		/*int i = 0;
		printf("%d : ", recoveryIdx);
		while (i != rowIdx)
		{
		printf("%d ", row[i]);
		i++;
		}
		printf("\n");*/



		/**/
		unsigned int SLength = power(rowLength);
		unsigned int *S = (unsigned int *)malloc(sizeof(unsigned int) * SLength);
		rmSCalculate(row, rowLength, S);

		/*for (int i = 0; i < SLength; i++)
		{
		printf("%d ", S[i]);
		}
		printf("\n");*/

		unsigned int ELength = m - (rowLength);
		unsigned int *E = (unsigned int *)malloc(sizeof(unsigned int) * ELength);
		rmECalculate(row, rowLength, m, E);

		unsigned int SCLength = power(ELength);
		unsigned int *SC = (unsigned int *)malloc(sizeof(unsigned int) * SCLength);
		rmSCCalculate(E, ELength, SC);

		/*for (int i = 0; i < SCLength; i++)
		{
		printf("%d ", SC[i]);
		}
		printf("\n");*/

		recovery[0][recoveryIdx] = rmACalculate(SLength, S, SCLength, SC, codeword);
		//printf("%d ", recovery[0][recoveryIdx]);

		free(SC);
		free(E);
		free(S);
		/**/

		recoveryIdx++;
		//printf("\n");
		return;
	}

	for (unsigned int i = rowValue - 1; i >= 1; i--)
	{
		row[rowIdx] = i;
		rmDecoding(n, m, rowLength, row, rowIdx + 1, i, codeword, recovery, recoveryIdx);
	}

}


void rmDecoder(const unsigned int& r, const unsigned int& m, unsigned char **GRM, const unsigned int& n, unsigned char **codeword, unsigned char **recovery)
{
	unsigned int* multiplRows = (unsigned int*)malloc(sizeof(unsigned int) * r);

	for (unsigned int i = r; i >= 1; i--)
	{
		static unsigned int recoveryIdx = m + 1;
		static unsigned int upIdx = recoveryIdx;

		for (unsigned int j = m; j >= 1; j--)
		{
			int rowIdx = 0;
			if (i == 1)
			{
				recoveryIdx = j;
			}

			multiplRows[rowIdx] = j;
			rmDecoding(n, m, i, multiplRows, rowIdx + 1, j, codeword, recovery, recoveryIdx);
		}


		if (i == 1)
		{
			upIdx = 1;
			recoveryIdx = m + 1;
		}
		rmRUpdate(upIdx, recoveryIdx, recovery, GRM, codeword, n);
		upIdx = recoveryIdx;
	}

	/*unsigned short *one = (unsigned short *)malloc(sizeof(unsigned short) * n);

	for (short j = 0; j < n; j++)
	{
		one[j] = codeword[0][j];
	}

	recovery[0][0] = rmDecision(one, n);

	free(one);*/
	recovery[0][0] = rmDecision(codeword[0], n);


	free(multiplRows);
}

void msgDecrypt(const unsigned int& n, unsigned char** c, unsigned char** PInverse, const unsigned int& k, unsigned char** SInverse, unsigned char** msg, unsigned char** GRM, const unsigned int& r, const unsigned int& m)
{
	unsigned char** cPrime = (unsigned char**)malloc(sizeof(unsigned char*) * 1);
	cPrime[0] = (unsigned char*)malloc(sizeof(unsigned char) * n);

	mtrixMultiplication(1, n, c, n, n, PInverse, cPrime);
	/*printf("c prime\n");
	for (int i = 0; i < n; i++)
	{
	printf("%d ", cPrime[0][i]);
	}
	printf("\n"); printf("\n");*/

	/* reed muller decoding */
	unsigned char** mPrime = (unsigned char**)malloc(sizeof(unsigned char*) * 1);
	mPrime[0] = (unsigned char*)malloc(sizeof(unsigned char) * k);

	//rcsvDecodingHard(r, m, cPrime[0], n);
	rmDecoder(r, m, GRM, n, cPrime, mPrime);

	/*printf("m prime\n");
	for (int i = 0; i < k; i++)
	{
	printf("%d ", mPrime[0][i]);
	}
	printf("\n"); printf("\n");*/

	mtrixMultiplication(1, k, mPrime, k, k, SInverse, msg);

	/*printf("msg\n");
	for (int i = 0; i < k; i++)
	{
	printf("%d ", msg[0][i]);
	}
	printf("\n"); printf("\n");*/





	//mtrixMultiplication(1, n, prime, k, k, SInverse, m);


	free(cPrime[0]);
	free(cPrime);

	free(mPrime[0]);
	free(mPrime);
}


void rcsvDecisionZero(double *orgMsg, int orgMsgLength, double *result, int resultLength)
{
	double sum = 0;
	for (int i = 0; i<resultLength; i++)
		sum += orgMsg[i];

	double everage = sum / orgMsgLength;
	for (int i = 0; i<resultLength; i++)
	{
		if (everage > 0)
			result[i] = 1;
		else if (everage < 0)
			result[i] = -1;
		else
			result[i] = (rand() % 2) == 0 ? 1 : -1;
	}
}

void rcsvDecisionEqual(double *orgMsg, int orgMsgLength, double *result, int resultLength)
{
	for (int i = 0; i<resultLength; i++)
	{
		if (orgMsg[i] > 0)
			result[i] = 1;
		else if (orgMsg[i] < 0)
			result[i] = -1;
		else
			result[i] = (rand() % 2) == 0 ? 1 : -1;
	}
}
double *rcsvLeft(int r, int m, double *orgMsg, int orgMsgLength, double *v, int vLength, double *result, int resultLength)
{
	double *yu;
	int yuLength = resultLength;

	yu = (double *)malloc(sizeof(double) * yuLength);

	for (int i = 0; i<yuLength; i++)
	{
		yu[i] = (orgMsg[i] + (orgMsg[i + yuLength] * v[i])) / 2;
	}

	result = (double *)malloc(sizeof(double) * resultLength);
	if (r == 0)
	{
		rcsvDecisionZero(yu, yuLength, result, resultLength);
	}
	else if (r == m)
	{
		rcsvDecisionEqual(yu, yuLength, result, resultLength);
	}
	else
	{
		double *v = NULL;
		int vLength = yuLength / 2;

		double *u = NULL;
		int uLength = yuLength / 2;

		v = rcsvRight(r - 1, m - 1, yu, yuLength, v, vLength);
		u = rcsvLeft(r, m - 1, yu, yuLength, v, vLength, u, uLength);

		for (int i = 0; i<uLength; i++)
		{
			result[i + uLength] = u[i] * v[i];
			result[i] = u[i];
		}

		free(v);
		free(u);
	}

	free(yu);

	return result;
}

double *rcsvRight(int r, int m, double *orgMsg, int orgMsgLength, double *result, int resultLength)
{
	double *yv;
	int yvLength = resultLength;

	yv = (double *)malloc(sizeof(double) * yvLength);
	for (int i = 0; i<yvLength; i++)
	{
		yv[i] = orgMsg[i] * orgMsg[i + yvLength];
	}

	result = (double *)malloc(sizeof(double) * resultLength);
	if (r == 0)
	{
		rcsvDecisionZero(yv, yvLength, result, resultLength);
	}
	else if (r == m)
	{
		rcsvDecisionEqual(yv, yvLength, result, resultLength);
	}
	else
	{
		double *v = NULL;
		int vLength = yvLength / 2;

		double *u = NULL;
		int uLength = yvLength / 2;

		v = rcsvRight(r - 1, m - 1, yv, yvLength, v, vLength);
		u = rcsvLeft(r, m - 1, yv, yvLength, v, vLength, u, uLength);

		for (int i = 0; i<uLength; i++)
		{
			result[i + (uLength)] = u[i] * v[i];
			result[i] = u[i];
		}

		free(v);
		free(u);
	}

	free(yv);

	return result;
}




void rcsvDecodingHard(int r, int m, unsigned char *recvCodeword, int recvCodewordLength)
{
	double *codeword;
	int codewordLength = power(m);
	codeword = (double *)malloc(sizeof(double) * codewordLength);

	double *result;
	result = (double *)malloc(sizeof(double) * power(m));

	for (int i = 0; i<recvCodewordLength; i++)
	{
		if (recvCodeword[i] == 1)
			codeword[i] = -1;
		else
			codeword[i] = 1;
	}

	double *v = NULL;
	int vLength = codewordLength / 2;
	double *u = NULL;
	int uLength = codewordLength / 2;

	v = rcsvRight(r - 1, m - 1, codeword, codewordLength, v, vLength);
	u = rcsvLeft(r, m - 1, codeword, codewordLength, v, vLength, u, uLength);

	for (int i = 0; i<uLength; i++)
	{
		result[i + uLength] = u[i] * v[i];
		result[i] = u[i];
	}

	for (int i = 0; i<recvCodewordLength; i++)
	{
		if (result[i] == -1)
		{
			recvCodeword[i] = 1;
		}
		else
		{
			recvCodeword[i] = 0;
		}
	}

	free(v);
	free(u);

	free(result);
	free(codeword);
}