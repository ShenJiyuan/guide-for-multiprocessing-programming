//Here we go.

#include "stdafx.h"
#include "stdlib.h"
#include "malloc.h"
#include "omp.h"

double **matrixA, **matrixB, **matrixC;
int matrixDimension;
int Thread_num;

double **matrixCreate(int dimension, char flag = 'y')//生成随机矩阵,flag为产生随机矩阵/零矩阵标志
{
	int i, j;
	double **matrix = (double **)malloc(dimension*sizeof(double*));
	if (matrix == NULL)
	{
		printf("malloc fail!\n");
		system("pause");
		exit(-1);
	}
	srand((unsigned)omp_get_wtime());
	for (i = 0; i<dimension; i++)
	{
		matrix[i] = (double *)malloc(dimension*sizeof(double));
		if (matrix == NULL)
		{
			printf("malloc fail!\n");
			system("pause");
			exit(-1);
		}
		if (flag == 'n')
		{
			for (j = 0; j<dimension; j++)
				matrix[i][j] = 0.00;
		}
		else
		{
			for (j = 0; j<dimension; j++)
				matrix[i][j] = (double)(rand() % 100);
		}
	}
	return matrix;
}

double MultSerialized()
{
	int i, j, k;
	double time_Start, time_Finish, time_Total;

	time_Start = omp_get_wtime();
	matrixC = matrixCreate(matrixDimension, 'n');
	for (i = 0; i<matrixDimension; i++)
	for (j = 0; j<matrixDimension; j++)
	for (k = 0; k<matrixDimension; k++)
		matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
	time_Finish = omp_get_wtime();
	time_Total = (time_Finish - time_Start);
	printf("The total time of serialized multiply programme=%.4lf\n\n", time_Total);

	return time_Total;
}

double AddSerialized(){
	int i, j;
	double time_Start, time_Finish, time_Total;
	time_Start = omp_get_wtime();
	matrixC = matrixCreate(matrixDimension, 'n');
	for (i = 0; i<matrixDimension; i++)
	for (j = 0; j<matrixDimension; j++)
		matrixC[i][j] = matrixA[i][j] + matrixB[i][j];
	time_Finish = omp_get_wtime();
	time_Total = (time_Finish - time_Start);
	printf("The total time of serialized add programme=%.4lf\n", time_Total);

	return time_Total;
}

double SubSerialized(){
	int i, j;
	double time_Start, time_Finish, time_Total;
	time_Start = omp_get_wtime();
	matrixC = matrixCreate(matrixDimension, 'n');
	for (i = 0; i<matrixDimension; i++)
	for (j = 0; j<matrixDimension; j++)
		matrixC[i][j] = matrixA[i][j] - matrixB[i][j];
	time_Finish = omp_get_wtime();
	time_Total = (time_Finish - time_Start);
	printf("The total time of serialized subtract programme=%.4lf\n", time_Total);

	return time_Total;
}

double MultParallel()
{
	int i, j, k, index;
	double time_Start, time_Finish, time_Total;
	int N = matrixDimension*matrixDimension;

	time_Start = omp_get_wtime();
#pragma omp parallel num_threads(Thread_num)
	{
#pragma omp for private(i,j,k)  
		for (index = 0; index<N; index++)
		{
			i = index / matrixDimension;
			j = index%matrixDimension;
			matrixC[i][j] = 0;
			for (k = 0; k<matrixDimension; k++)
				matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
		}
	}
	time_Finish = omp_get_wtime();

	time_Total = (time_Finish - time_Start);
	printf("The total time of paralleled multiply programme=%.4lf\n", time_Total);

	return time_Total;
}

double AddParallel(){
	int i, j, index;
	double time_Start, time_Finish, time_Total;
	int N = matrixDimension*matrixDimension;

	time_Start = omp_get_wtime();
#pragma omp parallel num_threads(Thread_num)
	{
#pragma omp for private(i,j)  
		for (index = 0; index<N; index++)
		{
			i = index / matrixDimension;
			j = index%matrixDimension;
			matrixC[i][j] = matrixA[i][j] + matrixB[i][j];
		}
	}
	time_Finish = omp_get_wtime();

	time_Total = (time_Finish - time_Start);
	printf("The total time of paralleled add programme=%.4lf\n", time_Total);

	return time_Total;

}

double SubParallel(){
	int i, j, index;
	double time_Start, time_Finish, time_Total;
	int N = matrixDimension*matrixDimension;

	time_Start = omp_get_wtime();
#pragma omp parallel num_threads(Thread_num)
	{
#pragma omp for private(i,j)  
		for (index = 0; index<N; index++)
		{
			i = index / matrixDimension;
			j = index%matrixDimension;
			matrixC[i][j] = matrixA[i][j] - matrixB[i][j];
		}
	}
	time_Finish = omp_get_wtime();

	time_Total = (time_Finish - time_Start);
	printf("The total time of paralleled subtract programme=%.4lf\n", time_Total);

	return time_Total;

}

void matrixFree(double **matrix, int dimension)
{
	int i;
	for (i = 0; i<dimension; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

int main()
{
	int i;
	double Ser_time_Add, Par_time_Add, Ser_time_Sub, Par_time_Sub,
		Ser_time_Mult, Par_time_Mult, SpeedUp_Add, SpeedUp_Sub, SpeedUp_Mult;
	int arr[] = { 2, 4, 8, 16, 32, 64, 100, 200 };
input:
	printf("Please input the size of matrix:");
	scanf_s("%d", &matrixDimension);
	if (matrixDimension<1)
	{
		printf("MatrixDimension cannot less than 1!\n");
		goto input;
	}
	matrixA = matrixCreate(matrixDimension);
	matrixB = matrixCreate(matrixDimension);

	Ser_time_Add = AddSerialized();
	Ser_time_Sub = SubSerialized();
	Ser_time_Mult = MultSerialized();

	for (i = 0; i<8; i++)
	{
		Thread_num = arr[i];
		printf("The number of threads is %d \n", Thread_num);
		Par_time_Add = AddParallel();
		SpeedUp_Add = Ser_time_Add / Par_time_Add;
		printf("The SpeedUp in add is %4lf\n", SpeedUp_Add);

		Par_time_Sub = SubParallel();
		SpeedUp_Sub = Ser_time_Sub / Par_time_Sub;
		printf("The SpeedUp in subtract is %4lf\n", SpeedUp_Sub);

		Par_time_Mult = MultParallel();
		SpeedUp_Mult = Ser_time_Mult / Par_time_Mult;
		printf("The SpeedUp in multiply is %4lf\n\n", SpeedUp_Mult);

	}
	matrixFree(matrixA, matrixDimension);
	matrixFree(matrixB, matrixDimension);
	matrixFree(matrixC, matrixDimension);

	system("pause");
	return 0;
}
