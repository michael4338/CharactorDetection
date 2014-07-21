
#include "stdafx.h"

#include "BPNNBase.h"
#include "BPNNRec.h"
#include "func.h"

BPNNRec::BPNNRec(BPNNBase *net,double ex,double mot,double step,int loop_count)
{
	bp=net;
	min_ex=ex;
	momentum=mot;
	eta=step;
	max_count=loop_count;

	/*
	CString str;
	str.Format ("循环次数：%d,误差：%f,相关系数：%f,步长：%f",max_count,min_ex,momentum,eta);
	::MessageBox(NULL,str,"输入参数：",NULL);
*/
}
BPNNRec::BPNNRec(BPNNBase *net)
{
    bp=net;
	min_ex=0.0;
	momentum=0.0;
	eta=0.0;
	max_count=0;
}

BPNNRec::BPNNRec(BPNNBase *net,double ex,double mot,double step,int loop_count,double LW_etaadd,double LW_etasub,double LW_etascale)
{
	bp=net;
	min_ex=ex;
	momentum=mot;
	eta=step;
	max_count=loop_count;
	eta_add=LW_etaadd;
	eta_sub=LW_etasub;
	eta_scale=LW_etascale;

	/*
	CString str;
	str.Format ("循环次数：%d,误差：%f,相关系数：%f,步长：%f",max_count,min_ex,momentum,eta);
	::MessageBox(NULL,str,"输入参数：",NULL);
*/
}

BPNNRec::~BPNNRec()
{
	delete bp;
}

void BPNNRec::setLWParam(double LW_etaadd,double LW_etasub,double LW_etascale)
{
	 eta_add=LW_etaadd;
	 eta_sub=LW_etasub;
	 eta_scale=LW_etascale;
}


BPNNBase* BPNNRec::getBP()
{
	return bp;
}

void BPNNRec::bpForward(double *l1, double *l2, double **conn, int n1, int n2)
{
	double sum;
	int j, k;

	/*** 设置阈值 ***/
	l1[0] = 1.0;

	/*** 对于第二层的每个神经元 ***/
	for (j = 1; j <= n2; j++) {

		/*** 计算输入的加权总和 ***/
		sum = 0.0;
		for (k = 0; k <= n1; k++) {
			sum += conn[k][j] * l1[k];
		}
		l2[j] = genSquash(sum);
	}
}

/* 输出误差 */
void BPNNRec::bpErrorOutput(double *delta, double *target, double *output, int nj)
{
	int j;
	double o, t, errsum;

	errsum = 0.0;
	for (j = 1; j <= nj; j++) {
		o = output[j];
		t = target[j];
		delta[j] = o * (1.0 - o) * (t - o);
		errsum += ABS(delta[j]);
	}
}


/* 隐含层误差 */
void BPNNRec::bpErrorHidden(double* delta_h, int nh, double *delta_o, int no, double **who, double *hidden)
{
	int j, k;
	double h, sum, errsum;

	errsum = 0.0;
	for (j = 1; j <= nh; j++) {
		h = hidden[j];
		sum = 0.0;
		for (k = 1; k <= no; k++) {
			sum += delta_o[k] * who[j][k];
		}
		delta_h[j] = h * (1.0 - h) * sum;
		errsum += ABS(delta_h[j]);
	}
}

/* 调整权值 */
void BPNNRec::bpWeightsAdjust(double *delta, int ndelta, double *ly, int nly, double** w, double **oldw, double eta, double momentum)
{
	double new_dw;
	int k, j;

	ly[0] = 1.0;
	for (j = 1; j <= ndelta; j++) {
		for (k = 0; k <= nly; k++) {
			new_dw = (eta * delta[j] * ly[k]) + (momentum * oldw[k][j]);
			w[k][j] += new_dw;
			oldw[k][j] = new_dw;
		}
	}
}


/*******保存权值**********/
void BPNNRec::writeWeights(double **w,int n1,int n2,char*name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	fp=fopen(name,"wb+");
	buffer=(double*)malloc((n1+1)*(n2+1)*sizeof(double));
	for(i=0;i<=n1;i++)
	{
		for(j=0;j<=n2;j++)
			buffer[i*(n2+1)+j]=w[i][j];
	}
	fwrite((char*)buffer,sizeof(double),(n1+1)*(n2+1),fp);
	fclose(fp);
	free(buffer);
}



/************读取权值*************/
bool  BPNNRec::readWeights(double **w,int n1,int n2,char *name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"无法读取权值信息",NULL,MB_ICONSTOP);
		return (false);
	}
	buffer=(double*)malloc((n1+1)*(n2+1)*sizeof(double));
	fread((char*)buffer,sizeof(double),(n1+1)*(n2+1),fp);
	
	for(i=0;i<=n1;i++)
	{
		for(j=0;j<=n2;j++)
			w[i][j]=buffer[i*(n2+1)+j];
	}
	fclose(fp);
	free(buffer);
	return(true);
}


/***********保存输入样本*************/
void BPNNRec::writeInput(double **w,int n1,int n2,char*name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	fp=fopen(name,"wb+");
	buffer=(double*)malloc(n1*n2*sizeof(double));
	for(i=0;i<n1;i++)
	{
		for(j=0;j<n2;j++)
			buffer[i*n2+j]=w[i][j];
	}
	fwrite((char*)buffer,sizeof(double),n1*n2,fp);
	fclose(fp);
	free(buffer);
}
/**********将输入保存到样本库中******************/
 void  BPNNRec::addToSample(double **w,int n1,int n2,char *name)
  {
	 int i,j;
	double *buffer;
	FILE *fp;
	fp=fopen(name,"ab");
	buffer=(double*)malloc(n1*n2*sizeof(double));
	for(i=0;i<n1;i++)
	{
		for(j=0;j<n2;j++)
			buffer[i*n2+j]=w[i][j];
	}
	fwrite((char*)buffer,sizeof(double),n1*n2,fp);
	fclose(fp);
	free(buffer);

  }

/*****保存各层结点的数目******/
void BPNNRec::writeNum(int n1,int n2,int n3,char*name)
{
	FILE *fp;
	fp=fopen(name,"wb+");
	int *buffer;
	buffer=(int*)malloc(3*sizeof(int));
	buffer[0]=n1;
	buffer[1]=n2;
	buffer[2]=n3;
	fwrite((char*)buffer,sizeof(int),3,fp);
	fclose(fp);
	free(buffer);
}
/*******保存训练集中的样本数目******/
void BPNNRec::writeSampleNum(int n1,char*name)
{
	FILE *fp;
	fp=fopen(name,"wb+");
	int *buffer;
	buffer=(int*)malloc(sizeof(int));
	*buffer=n1;
	fwrite((char*)buffer,sizeof(int),1,fp);
	fclose(fp);
	free(buffer);
}

/********读取各层结点数目*********/

bool BPNNRec::readNum(char *name)
{
	int *buffer;
	FILE *fp;
	buffer=(int *)malloc(3*sizeof(int));
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"结点参数",NULL,MB_ICONSTOP);
		return (false);
	}
	fread((char*)buffer,sizeof(int),3,fp);
    bp->initBPNN(buffer[0],buffer[1],buffer[2]);
	fclose(fp);
	free(buffer);
	return(true);
}

//读取样本集中样本的个数
int BPNNRec::readSampleNum(char *name)
{
	int *buffer;
	FILE *fp;
	buffer=(int *)malloc(sizeof(int));
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"读取样本个数",NULL,MB_ICONSTOP);
		return (false);
	}
	fread((char*)buffer,sizeof(int),1,fp);
	fclose(fp);
	int result=buffer[0];
	free(buffer);
	return(result);
}

/***********读取训练样本的值**************/
bool  BPNNRec::readInput(double **w,int n1,int n2,char *name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"无法读取权值信息",NULL,MB_ICONSTOP);
		return (false);
	}
	buffer=(double*)malloc(n1*n2*sizeof(double));
	fread((char*)buffer,sizeof(double),n1*n2,fp);
	
	CString str;
	/*
	for(i=0;i<20;i++)
	{
	str.Format("%f",buffer[i]);
	::MessageBox(NULL,str,"读取结果",NULL);
	}
	*/
	
	
	for(i=0;i<n1;i++)
	{
		for(j=0;j<n2;j++)
			w[i][j]=buffer[i*n2+j];
		/*
        str.Format("%f",w[i][j]);
		::MessageBox(NULL,str,"读取结果",NULL);
	    */
	  }
	
	fclose(fp);
	free(buffer);
	return(true);
}
/****************************************************
* 函数名称 BpTrain()
* 
* 参数：
*   double **data_in    -指向输入的特征向量数组的指针    
*	double **data_out   -指向理想输出数组的指针
int n_in            -输入层结点的个数 
*   int n_hidden        -BP网络隐层结点的数目
*   double min_ex       -训练时允许的最大均方误差
*   double momentum     -BP网络的相关系数
*   double eta          -BP网络的训练步长
*   int num             -输入样本的个数
*
* 函数功能：
*     根据输入的特征向量和期望的理想输出对BP网络尽行训练
*     训练结束后将权值保存并将训练的结果显示出来
********************************************************/
void BPNNRec::BpTrain(double ** data_in, double** data_out,int num,double *&errRecord,int *errNum)
{
	

  
	int i,k,l;
	int n_in=bp->input_n;
	int n_hidden=bp->hidden_n;
	//输出层结点数目
	int  n_out=bp->output_n;   
	//指向输入层数据的指针
	double* input_unites=bp->input_units; 
	//指向隐层数据的指针
	double* hidden_unites=bp->hidden_units;
	//指向输出层数据的指针
	double* output_unites=bp->output_units; 
	//指向隐层误差数据的指针
	double* hidden_deltas=bp->hidden_delta;
	//指向输出层误差数剧的指针
	double* output_deltas=bp->output_delta;  
	//指向理想目标输出的指针
	double* target=bp->target;    
	//指向输入层于隐层之间权值的指针
	double** input_weights=bp->input_weights;
	//指向隐层与输出层之间的权值的指针
	double** hidden_weights=bp->hidden_weights;
	//指向上一此输入层于隐层之间权值的指针
	double** input_prev_weights=bp->input_prev_weights ;
	//指向上一此隐层与输出层之间的权值的指针
	double** hidden_prev_weights=bp->hidden_prev_weights;
	//每次循环后的均方误差误差值 
	double ex;
  
	errRecord=createDoubleArray1(max_count);
	for(int a=0;a<max_count;a++)
		errRecord[a]=0.0;

	CString str;
/*	str.Format ("输入：%d个，隐藏：%d个,输出：%d个",n_in,n_hidden,n_out);
		
	::MessageBox(NULL,str,"输入参数：",NULL);
*/	
	//为各个数据结构申请内存空间
//	input_unites= createDoubleArray1(n_in + 1);
//	hidden_unites=createDoubleArray1(n_hidden + 1);
//	output_unites=createDoubleArray1(n_out + 1);
//	hidden_deltas = createDoubleArray1(n_hidden + 1);
//	output_deltas = createDoubleArray1(n_out + 1);
//	target = createDoubleArray1(n_out + 1);
//	input_weights=createDoubleArray2(n_in + 1, n_hidden + 1);
//	input_prev_weights = createDoubleArray2(n_in + 1, n_hidden + 1);
//	hidden_prev_weights = createDoubleArray2(n_hidden + 1, n_out + 1);
//	hidden_weights = createDoubleArray2(n_hidden + 1, n_out + 1);

	
	
	//对各种权值进行初始化初始化
//	bpnn_randomize_weights( input_weights,n_in,n_hidden);
//	bpnn_randomize_weights( hidden_weights,n_hidden,n_out);
//	bpnn_zero_weights(input_prev_weights, n_in,n_hidden );
//	bpnn_zero_weights(hidden_prev_weights,n_hidden,n_out );
/*   if (input_weights[0][0]!=0&&hidden_weights[0][0]!=0)
	AfxMessageBox("权值已经被初始化完毕！");
	else
    AfxMessageBox("权值根本没有被初始化！");

	AfxMessageBox("开始进行BP网络训练!");
*/
    //开始进行BP网络训练
	//这里设定最大的迭代次数为15000次
	for(l=0;l<max_count;l++)  
	{ 
		//对均方误差置零
		ex=0;
		//对样本进行逐个的扫描
		for(k=0;k<num;k++)  
		{ 
			//将提取的样本的特征向量输送到输入层上
			for(i=1;i<=n_in;i++)
				input_unites[i] = data_in[k][i-1];
			
			//将预定的理想输出输送到BP网络的理想输出单元
			for(i=1;i<=n_out;i++)
				target[i]=data_out[k][i-1];
			
			//前向传输激活
			
			//将数据由输入层传到隐层 
			bpForward(input_unites,hidden_unites,
				input_weights, n_in,n_hidden);
			//将隐层的输出传到输出层
			bpForward(hidden_unites, output_unites,
				hidden_weights,n_hidden,n_out);
			
			//误差计算
			
			//将输出层的输出与理想输出比较计算输出层每个结点上的误差
			bpErrorOutput(output_deltas,target,output_unites,n_out);
			//根据输出层结点上的误差计算隐层每个节点上的误差
			bpErrorHidden(hidden_deltas,n_hidden, output_deltas, n_out,hidden_weights, hidden_unites);
			
			//权值调整
			//根据输出层每个节点上的误差来调整隐层与输出层之间的权值    
			bpWeightsAdjust(output_deltas,n_out, hidden_unites,n_hidden,
				hidden_weights, hidden_prev_weights, eta, momentum); 
			//根据隐层每个节点上的误差来调整隐层与输入层之间的权值    	
			bpWeightsAdjust(hidden_deltas, n_hidden, input_unites, n_in,
				input_weights, input_prev_weights, eta, momentum);  
			
			//误差统计		
			for(i=1;i<=n_out;i++)
				ex+=(output_unites[i]-data_out[k][i-1])*(output_unites[i]-data_out[k][i-1]);
		}
		//计算均方误差
		ex=ex/double(num*n_out);

        errRecord[l]=ex;
		//如果均方误差已经足够的小，跳出循环，训练完毕  
		if(ex<min_ex)break;
	}
	
	//相关保存
	*errNum=l;
	//保存输入层与隐层之间的权值
	writeWeights(input_weights,n_in,n_hidden,"in_hi.dat");
	//保存隐层与输出层之间的权值
	writeWeights(hidden_weights,n_hidden,n_out,"hi_out.dat");
	
	//保存各层结点的个数
	writeNum(n_in,n_hidden,n_out,"num");
	
	//显示训练结果
	
/*	
	if(ex<=min_ex)
	{
		str.Format ("迭代%d次，\n平均误差%.4f",l,ex);
		
		::MessageBox(NULL,str,"训练结果",NULL);
	}
	
	if(ex>min_ex)
	{
		
		str.Format("迭代%d次，平均误差%.4f\n我已经尽了最大努力了还是达不到您的要求\n请调整参数重新训练吧！",l,ex);
		::MessageBox(NULL,str,"训练结果",NULL);
	}
*/	
	//释放内存空间
	/*
	free(input_unites);
	free(hidden_unites);
	free(output_unites);
	free(hidden_deltas);
	free(output_deltas);
	free(target);
	free(input_weights);
	free(hidden_weights);
	free(input_prev_weights);
	free(hidden_prev_weights);
	*/
	
}

//LWBP算法的训练函数
void BPNNRec::LWBpTrain(double ** data_in, double** data_out,int num,double *&errRecord,int *errNum)
{
	

    double sub_result;
	int i,j,k,l,LWErrNum=0;
	int n_in=bp->input_n;
	int n_hidden=bp->hidden_n;
	//输出层结点数目
	int  n_out=bp->output_n;   
	//指向输入层数据的指针
	double* input_unites=bp->input_units; 
	//指向隐层数据的指针
	double* hidden_unites=bp->hidden_units;
	//指向输出层数据的指针
	double* output_unites=bp->output_units; 
	//指向隐层误差数据的指针
	double* hidden_deltas=bp->hidden_delta;
	//指向输出层误差数剧的指针
	double* output_deltas=bp->output_delta;  
	//指向理想目标输出的指针
	double* target=bp->target;    
	//指向输入层于隐层之间权值的指针
	double** input_weights=bp->input_weights;
	double** old_input_weights;
	//指向隐层与输出层之间的权值的指针
	double** hidden_weights=bp->hidden_weights;
	double** old_hidden_weights;
	//指向上一此输入层于隐层之间权值的指针
	double** input_prev_weights=bp->input_prev_weights ;
	//指向上一此隐层与输出层之间的权值的指针
	double** hidden_prev_weights=bp->hidden_prev_weights;
	//每次循环后的均方误差误差值 
	double ex,old_ex;


	CString str;
/*	str.Format ("输入：%d个，隐藏：%d个,输出：%d个",n_in,n_hidden,n_out);
		
	::MessageBox(NULL,str,"输入参数：",NULL);
*/
	//为误差记录数组分配空间
	errRecord=createDoubleArray1(max_count);
	for(int a=0;a<max_count;a++)
		errRecord[a]=0.0;
	
	//为各个数据结构申请内存空间
//	input_unites= createDoubleArray1(n_in + 1);
//	hidden_unites=createDoubleArray1(n_hidden + 1);
//	output_unites=createDoubleArray1(n_out + 1);
//	hidden_deltas = createDoubleArray1(n_hidden + 1);
//	output_deltas = createDoubleArray1(n_out + 1);
//	target = createDoubleArray1(n_out + 1);
    old_input_weights=createDoubleArray2(n_in + 1, n_hidden + 1);
//  input_prev_weights = createDoubleArray2(n_in + 1, n_hidden + 1);
//	hidden_prev_weights = createDoubleArray2(n_hidden + 1, n_out + 1);
	old_hidden_weights = createDoubleArray2(n_hidden + 1, n_out + 1);

	
	
	//对各种权值进行初始化初始化
//	bpnn_randomize_weights( input_weights,n_in,n_hidden);
//	bpnn_randomize_weights( hidden_weights,n_hidden,n_out);
//	bpnn_zero_weights(input_prev_weights, n_in,n_hidden );
//	bpnn_zero_weights(hidden_prev_weights,n_hidden,n_out );
/*  if (input_weights[0][0]!=0&&hidden_weights[0][0]!=0)
	AfxMessageBox("权值已经被初始化完毕！");
	else
    AfxMessageBox("权值根本没有被初始化！");

	AfxMessageBox("开始进行BP网络训练!");
*/
  //开始进行BP网络训练
	//这里设定最大的迭代次数为15000次
	for(l=0;l<max_count;l++)  
	{ 
		//对均方误差置零
		ex=0;
		//保存本次操作之前的各层权值
		for(i=1;i<=n_in;i++)
			for(j=1;j<=n_hidden;j++)
				old_input_weights[i][j]=input_weights[i][j];
		for(i=1;i<n_hidden;i++)
			for(j=1;j<=n_out;j++)
				old_hidden_weights[i][j]=hidden_weights[i][j];


		//对样本进行逐个的扫描
		for(k=0;k<num;k++)  
		{ 
			//将提取的样本的特征向量输送到输入层上
			for(i=1;i<=n_in;i++)
				input_unites[i] = data_in[k][i-1];
			
			//将预定的理想输出输送到BP网络的理想输出单元
			for(i=1;i<=n_out;i++)
				target[i]=data_out[k][i-1];
			
			//前向传输激活
			
			//将数据由输入层传到隐层 
			bpForward(input_unites,hidden_unites,
				input_weights, n_in,n_hidden);
			//将隐层的输出传到输出层
			bpForward(hidden_unites, output_unites,
				hidden_weights,n_hidden,n_out);
			
			//误差计算
			
			//将输出层的输出与理想输出比较计算输出层每个结点上的误差
			bpErrorOutput(output_deltas,target,output_unites,n_out);
			//根据输出层结点上的误差计算隐层每个节点上的误差
			bpErrorHidden(hidden_deltas,n_hidden, output_deltas, n_out,hidden_weights, hidden_unites);
			
		
			//权值调整
			//根据输出层每个节点上的误差来调整隐层与输出层之间的权值    
			bpWeightsAdjust(output_deltas,n_out, hidden_unites,n_hidden,
				hidden_weights, hidden_prev_weights, eta, momentum); 
			//根据隐层每个节点上的误差来调整隐层与输入层之间的权值    	
			bpWeightsAdjust(hidden_deltas, n_hidden, input_unites, n_in,
				input_weights, input_prev_weights, eta, momentum);  
			
			//误差统计		
			for(i=1;i<=n_out;i++)
				ex+=(output_unites[i]-data_out[k][i-1])*(output_unites[i]-data_out[k][i-1]);
		    
		}
		//计算均方误差
		ex=ex/double(num*n_out);
		if (LWErrNum==0)
				old_ex=ex;
		else
		//如果均方误差已经足够的小，跳出循环，训练完毕
		{
		sub_result=ex-old_ex;
		if(sub_result>=0)
		{
			if(sub_result/old_ex>eta_scale)
			{
				//取消上一次的权值改动
                for(k=0;k<num;k++)  
				{
					for(i=1;i<=n_in;i++)
						for(j=1;j<=n_hidden;j++)
							input_weights[i][j]=old_input_weights[i][j];
					for(i=1;i<=n_hidden;i++)
						for(j=1;j<=n_out;j++)
							hidden_weights[j][j]=old_hidden_weights[i][j];
				}
                //减少学习步长
				eta=eta*eta_sub;
				//将动量因子设置为0
				old_momentum=momentum;
				momentum=0;
				ex=old_ex;//误差不符合要求,回到上次保持的误差值
				
			}
			else
			{
				if ((momentum==0)&&(old_momentum!=0))
					momentum=old_momentum;
				old_ex=ex;  //保存本次的误差
			}
		}
		else //如果本次与上次误差的差值小于0,则接受权值更新,学习步长乘以大于1的因子,
			//如果动量因子过去设置为0,则恢复到原来的值
		{ 
			eta=eta*eta_add;
			if((momentum==0)&&(old_momentum>0))
				momentum=old_momentum;
			old_ex=ex;
		}
		}
				
		errRecord[l]=ex;
        LWErrNum++;
		if(ex<min_ex)break;
	}
	
    
	//设置误差数据的个数
	*errNum=l;
	
	//保存输入层与隐层之间的权值
	writeWeights(input_weights,n_in,n_hidden,"in_hi.dat");
	//保存隐层与输出层之间的权值
	writeWeights(hidden_weights,n_hidden,n_out,"hi_out.dat");
	
	//保存各层结点的个数
	writeNum(n_in,n_hidden,n_out,"num");
	
	//显示训练结果
/*	static BOOL bShow = FALSE;
	
    if(bShow)
    {
		if(ex<=min_ex)
		{
			str.Format ("迭代%d次，\n平均误差%.4f",l,ex);
			::MessageBox(NULL,str,"训练结果",NULL);
		}
		if(ex>min_ex)
		{
			str.Format("迭代%d次，平均误差%.4f\n我已经尽了最大努力了还是达不到您的要求\n请调整参数重新训练吧！",l,ex);
		    ::MessageBox(NULL,str,"训练结果",NULL);
		}
    }
    bShow = TRUE;
*/	

	free(old_input_weights);
	free(old_hidden_weights);
	//释放内存空间
	/*
	free(input_unites);
	free(hidden_unites);
	free(output_unites);
	free(hidden_deltas);
	free(output_deltas);
	free(target);
	free(input_weights);
	free(hidden_weights);
	free(input_prev_weights);
	free(hidden_prev_weights);
	*/
	
}


void BPNNRec::BpRecognize(double **data_in, int num,int *&recgonize_result)
{

	int n_in=bp->input_n;
	int n_hidden=bp->hidden_n;
	int n_out=bp->output_n;
	//循环变量
	int i,k;
	// 指向识别结果的指针 
	//为存放识别的结果申请存储空间
	recgonize_result=(int*)malloc(num*sizeof(int));
	
	//指向输入层数据的指针
	double* input_unites; 
	//指向隐层数据的指针
	double* hidden_unites;
	//指向输出层数据的指针
	double* output_unites; 
	//指向输入层于隐层之间权值的指针
	double** input_weights;
	//指向隐层与输出层之间的权值的指针
	double** hidden_weights;
	//为各个数据结构申请内存空间
	input_unites= bp->input_units;
	hidden_unites= bp->hidden_units;
	output_unites=bp->output_units;
	input_weights= bp->input_weights;
	hidden_weights = bp->hidden_weights;	
	
	//读取权值
	if(readWeights(input_weights,bp->input_n,bp->hidden_n,"in_hi.dat")==false)
		return;
	if(readWeights(hidden_weights,bp->hidden_n,bp->output_n,"hi_out.dat")==false)
		return;
	
	
	//逐个样本扫描
	for(k=0;k<num;k++)
	{ 
		//将提取的样本的特征向量输送到输入层上
		for(i=1;i<=n_in;i++)
			input_unites[i]=data_in[k][i-1];
		
		//前向输入激活
        bpForward(input_unites,hidden_unites,
			input_weights, n_in,n_hidden);
        bpForward(hidden_unites, output_unites,
			hidden_weights,n_hidden,n_out);
		
		//根据输出结果进行识别
		int result=0 ;
		//考察每一位的输出
		int recnum=0;
		for(i=1;i<=n_out;i++)
		{
			//如果大于0.5判为1
			if((output_unites[i]<0.75)&&(output_unites[i]>0.25))
				recnum++;
			if(output_unites[i]>0.5)
				
				result+=(int)pow(2,double(3-i));
		}
		
		//如果判定的结果小于等于7，认为合理
		//if(result<=7)
			recgonize_result[k]=result;
		//如果判定的结果大于7，认为不合理将结果定位为一个特殊值-1
		if(recnum>=2)
			recgonize_result[k]=-1;
		
	}	
}


