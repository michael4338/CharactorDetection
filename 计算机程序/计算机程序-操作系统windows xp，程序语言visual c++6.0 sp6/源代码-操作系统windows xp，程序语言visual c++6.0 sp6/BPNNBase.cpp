
#include "stdafx.h"

#include "BPNNBase.h"
#include "func.h"


BPNNBase::BPNNBase()
{
}


BPNNBase::BPNNBase(int input,int hidden,int output)
{
	input_n=input;                    /*默认输入层的神经元个数为64*/
	hidden_n=hidden;                  /*默认隐含层的神经元个数为9*/
	output_n=output;                  /*默认输出层的神经元个数为2*/   
	/*
	CString str;
	str.Format ("输出：%d个",output_n);
		
	::MessageBox(NULL,str,"输出参数：",NULL);
	*/

	//创建输入层,隐藏层及输出层的神经元数据数组
	createBpnnData();
   
   	//创建输入层,隐藏层及输出层的误差数据数组
	createBpnnDeltaData();
	
	//创建输入层,隐藏层及输出层的前向传播和后向传播的权值数据数组
	createBpnnWeights();
	
    //进行各层权值数组的初始化
	initBpnnWeights();
	//AfxMessageBox("初始化各层结束！");
}

BPNNBase::~BPNNBase()
{ 
   int n1, n2, i;

	n1 = input_n;
	n2 = hidden_n;
    
	//释放输入层,隐藏层及输出层数据所占的内存空间
	free((char *) input_units);
	free((char *) hidden_units);
	free((char *) output_units);

	//释放输入层,隐藏层及输出层误差数据所占的内存空间
	free((char *) hidden_delta);
	free((char *) output_delta);
	free((char *) target);

	//释放输入层,隐藏层及输出层权值数据所占的内存空间
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
	input_n=input;                    /*默认输入层的神经元个数为64*/
	hidden_n=hidden;                  /*默认隐含层的神经元个数为9*/
	output_n=output;                  /*默认输出层的神经元个数为2*/   
}

void BPNNBase::createBpnnData()
{
	//创建输入层,隐藏层及输出层的神经元数据数组
    input_units = createDoubleArray1(input_n + 1);
	hidden_units = createDoubleArray1(hidden_n + 1);
	output_units = createDoubleArray1(output_n + 1);
}

void BPNNBase::createBpnnDeltaData()
{
	//创建输入层,隐藏层及输出层的误差数据数组
	hidden_delta = createDoubleArray1(hidden_n + 1);
	output_delta = createDoubleArray1(output_n + 1);
	target = createDoubleArray1(output_n + 1);
}

void BPNNBase::createBpnnWeights()
{
	//创建输入层,隐藏层及输出层的前向传播和后向传播的权值数据数组
	
	input_weights = createDoubleArray2(input_n + 1, hidden_n + 1);
	hidden_weights = createDoubleArray2(hidden_n + 1, output_n + 1);
	input_prev_weights = createDoubleArray2(input_n + 1, hidden_n + 1);
	hidden_prev_weights = createDoubleArray2(hidden_n + 1, output_n + 1);

   
}

void BPNNBase::initBpnnWeights()
{
	 //进行各层权值数组的初始化
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
 	input_n=input;                    /*默认输入层的神经元个数为64*/
	hidden_n=hidden;                  /*默认隐含层的神经元个数为9*/
	output_n=output;                  /*默认输出层的神经元个数为2*/  
	
	//创建输入层,隐藏层及输出层的神经元数据数组
	createBpnnData();
   
   	//创建输入层,隐藏层及输出层的误差数据数组
	createBpnnDeltaData();
	
	//创建输入层,隐藏层及输出层的前向传播和后向传播的权值数据数组
	createBpnnWeights();
	
    //进行各层权值数组的初始化
	initBpnnWeights();
	//AfxMessageBox("初始化各层结束！");

}

// 随机初始化权值 
void BPNNBase::rndWeights(double **w, int m, int n)
{
	int i, j;
	
	for (i = 0; i <= m; i++) {
		for (j = 0; j <= n; j++) {
			w[i][j] = createRndLimit();
		}
	}
}

// 0值初始化权值 
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