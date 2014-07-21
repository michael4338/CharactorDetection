/***************************************************
BPNNBase���BP��������Ҫ�õ��Ļ����������з�װ
*****************************************************/

#include <stdio.h>
#include <math.h> 
#include <time.h>
#include <stdlib.h>
#include <iostream.h>

#define RAND_SEED 32767     //�������,���ڲ���0-1�����˫������  
#define ABS(x)          (((x) > 0.0) ? (x) : (-(x)))   //ȡ����ֵ

//����0��1��˫���������
static double createRnd()
{
	return ((double) rand() / (double) RAND_SEED);
}

//����-1.0��1.0֮���˫���������
static double createRndLimit()
{
	return ((createRnd() * 2.0) - 1.0);
}

static double genSquash(double x)
{
	return (1.0 / (1.0 + exp(-x)));
}
// ����1ά˫����ʵ������

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

//����2ά˫����ʵ������

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