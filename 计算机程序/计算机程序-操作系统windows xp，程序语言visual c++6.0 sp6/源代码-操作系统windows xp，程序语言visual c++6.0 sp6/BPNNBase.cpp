
#include "stdafx.h"

#include "BPNNBase.h"
#include "func.h"


BPNNBase::BPNNBase()
{
}


BPNNBase::BPNNBase(int input,int hidden,int output)
{
	input_n=input;                    /*Ĭ����������Ԫ����Ϊ64*/
	hidden_n=hidden;                  /*Ĭ�����������Ԫ����Ϊ9*/
	output_n=output;                  /*Ĭ����������Ԫ����Ϊ2*/   
	/*
	CString str;
	str.Format ("�����%d��",output_n);
		
	::MessageBox(NULL,str,"���������",NULL);
	*/

	//���������,���ز㼰��������Ԫ��������
	createBpnnData();
   
   	//���������,���ز㼰�����������������
	createBpnnDeltaData();
	
	//���������,���ز㼰������ǰ�򴫲��ͺ��򴫲���Ȩֵ��������
	createBpnnWeights();
	
    //���и���Ȩֵ����ĳ�ʼ��
	initBpnnWeights();
	//AfxMessageBox("��ʼ�����������");
}

BPNNBase::~BPNNBase()
{ 
   int n1, n2, i;

	n1 = input_n;
	n2 = hidden_n;
    
	//�ͷ������,���ز㼰�����������ռ���ڴ�ռ�
	free((char *) input_units);
	free((char *) hidden_units);
	free((char *) output_units);

	//�ͷ������,���ز㼰��������������ռ���ڴ�ռ�
	free((char *) hidden_delta);
	free((char *) output_delta);
	free((char *) target);

	//�ͷ������,���ز㼰�����Ȩֵ������ռ���ڴ�ռ�
	for (i = 0; i <= n1; i++) {
		free((char *) input_weights[i]);
		free((char *) input_prev_weights[i]);
	}
	free((char *) input_weights);
	free((char *) input_prev_weights);

	for (i = 0; i <= n2; i++) {
		free((char *) hidden_weights[i]);
		free((char *) hidden_prev_weights[i]);
	}
	free((char *) hidden_weights);
	free((char *) hidden_prev_weights);

}

void BPNNBase::setBpnnData(int input,int hidden,int output)
{
	input_n=input;                    /*Ĭ����������Ԫ����Ϊ64*/
	hidden_n=hidden;                  /*Ĭ�����������Ԫ����Ϊ9*/
	output_n=output;                  /*Ĭ����������Ԫ����Ϊ2*/   
}

void BPNNBase::createBpnnData()
{
	//���������,���ز㼰��������Ԫ��������
    input_units = createDoubleArray1(input_n + 1);
	hidden_units = createDoubleArray1(hidden_n + 1);
	output_units = createDoubleArray1(output_n + 1);
}

void BPNNBase::createBpnnDeltaData()
{
	//���������,���ز㼰�����������������
	hidden_delta = createDoubleArray1(hidden_n + 1);
	output_delta = createDoubleArray1(output_n + 1);
	target = createDoubleArray1(output_n + 1);
}

void BPNNBase::createBpnnWeights()
{
	//���������,���ز㼰������ǰ�򴫲��ͺ��򴫲���Ȩֵ��������
	
	input_weights = createDoubleArray2(input_n + 1, hidden_n + 1);
	hidden_weights = createDoubleArray2(hidden_n + 1, output_n + 1);
	input_prev_weights = createDoubleArray2(input_n + 1, hidden_n + 1);
	hidden_prev_weights = createDoubleArray2(hidden_n + 1, output_n + 1);

   
}

void BPNNBase::initBpnnWeights()
{
	 //���и���Ȩֵ����ĳ�ʼ��
	//zeroWeights(newnet->input_weights, input_n, hidden_n);
	time_t t;
	initSeed((unsigned) time(&t));

	rndWeights(input_weights, input_n, hidden_n);
	rndWeights(hidden_weights, hidden_n, output_n);
	zeroWeights(input_prev_weights, input_n, hidden_n);
	zeroWeights(hidden_prev_weights, hidden_n, output_n);
}

void BPNNBase::initBPNN(int input,int hidden,int output)
{
 	input_n=input;                    /*Ĭ����������Ԫ����Ϊ64*/
	hidden_n=hidden;                  /*Ĭ�����������Ԫ����Ϊ9*/
	output_n=output;                  /*Ĭ����������Ԫ����Ϊ2*/  
	
	//���������,���ز㼰��������Ԫ��������
	createBpnnData();
   
   	//���������,���ز㼰�����������������
	createBpnnDeltaData();
	
	//���������,���ز㼰������ǰ�򴫲��ͺ��򴫲���Ȩֵ��������
	createBpnnWeights();
	
    //���и���Ȩֵ����ĳ�ʼ��
	initBpnnWeights();
	//AfxMessageBox("��ʼ�����������");

}

// �����ʼ��Ȩֵ 
void BPNNBase::rndWeights(double **w, int m, int n)
{
	int i, j;
	
	for (i = 0; i <= m; i++) {
		for (j = 0; j <= n; j++) {
			w[i][j] = createRndLimit();
		}
	}
}

// 0ֵ��ʼ��Ȩֵ 
void BPNNBase::zeroWeights(double **w, int m, int n)
{
	int i, j;

	for (i = 0; i <= m; i++) {
		for (j = 0; j <= n; j++) {
			w[i][j] = 0.0;
		}
	}
}

void BPNNBase::initSeed(int seed)
{
	srand(seed);
}

int BPNNBase::getInputNum()
{
	return input_n;
}

int BPNNBase::getHiddenNum()
{
	return hidden_n;
}

int BPNNBase::getOutputNum()
{
	return output_n;
}
/*
void BPNNBase::test()
{
	for(int i=0;i<input_n;i++)
		cout<<input_weights[i]<<",";
}
*/