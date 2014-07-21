/***************************************************
BPNNBase类对BP神经网络需要用到的基本操作进行封装
*****************************************************/

#include <stdio.h>
#include <math.h> 
#include <time.h>
#include <stdlib.h>
#include <iostream.h>

#define RAND_SEED 32767     //随机种子,用于产生0-1的随机双精度数  
#define ABS(x)          (((x) > 0.0) ? (x) : (-(x)))   //取绝对值

//返回0－1的双精度随机数
static double createRnd()
{
	return ((double) rand() / (double) RAND_SEED);
}

//返回-1.0到1.0之间的双精度随机数
static double createRndLimit()
{
	return ((createRnd() * 2.0) - 1.0);
}

static double genSquash(double x)
{
	return (1.0 / (1.0 + exp(-x)));
}
// 申请1维双精度实数数组

static double *createDoubleArray1(int n)
{
	double *new1;
	
	new1 = (double *) malloc ((unsigned) (n * sizeof (double)));
	if (new1 == NULL) {
		printf("createDoubleArray1: Couldn't allocate array of doubles\n");
		return (NULL);
	}
	return (new1);
}

//申请2维双精度实数数组

static double **createDoubleArray2(int m, int n)
{
	int i;
	double **new1;
	
	new1 = (double **) malloc ((unsigned) (m * sizeof (double *)));
	if (new1 == NULL) {
		//	printf("createDoubleArray2: Couldn't allocate array of dbl ptrs\n");
		return (NULL);
	}
	
	for (i = 0; i < m; i++) {
		new1[i] = createDoubleArray1(n);
	}
	
	return (new1);
}

static void freeDoubleArray(double**& data, int m, int n)
{
	if(data != NULL)
	{
		for(int i=0;i<m;i++)
		{
			if(data[i] != NULL)
			{
				free(data[i]);
				data[i] = NULL;
			}
		}
	    free(data);
		data = NULL;
	}
}

static int FindCharPosInString(char* str, char ch)
{
	int pos = 0;
	while(str[pos] != ch && str[pos] != '\0')pos++;
	if(str[pos] == ch) return pos;
	return -1;
}