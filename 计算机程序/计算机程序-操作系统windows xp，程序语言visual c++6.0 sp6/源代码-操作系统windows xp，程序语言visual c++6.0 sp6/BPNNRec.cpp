
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
	str.Format ("ѭ��������%d,��%f,���ϵ����%f,������%f",max_count,min_ex,momentum,eta);
	::MessageBox(NULL,str,"���������",NULL);
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
	str.Format ("ѭ��������%d,��%f,���ϵ����%f,������%f",max_count,min_ex,momentum,eta);
	::MessageBox(NULL,str,"���������",NULL);
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

	/*** ������ֵ ***/
	l1[0] = 1.0;

	/*** ���ڵڶ����ÿ����Ԫ ***/
	for (j = 1; j <= n2; j++) {

		/*** ��������ļ�Ȩ�ܺ� ***/
		sum = 0.0;
		for (k = 0; k <= n1; k++) {
			sum += conn[k][j] * l1[k];
		}
		l2[j] = genSquash(sum);
	}
}

/* ������ */
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


/* ��������� */
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

/* ����Ȩֵ */
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


/*******����Ȩֵ**********/
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



/************��ȡȨֵ*************/
bool  BPNNRec::readWeights(double **w,int n1,int n2,char *name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"�޷���ȡȨֵ��Ϣ",NULL,MB_ICONSTOP);
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


/***********������������*************/
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
/**********�����뱣�浽��������******************/
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

/*****������������Ŀ******/
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
/*******����ѵ�����е�������Ŀ******/
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

/********��ȡ��������Ŀ*********/

bool BPNNRec::readNum(char *name)
{
	int *buffer;
	FILE *fp;
	buffer=(int *)malloc(3*sizeof(int));
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"������",NULL,MB_ICONSTOP);
		return (false);
	}
	fread((char*)buffer,sizeof(int),3,fp);
    bp->initBPNN(buffer[0],buffer[1],buffer[2]);
	fclose(fp);
	free(buffer);
	return(true);
}

//��ȡ�������������ĸ���
int BPNNRec::readSampleNum(char *name)
{
	int *buffer;
	FILE *fp;
	buffer=(int *)malloc(sizeof(int));
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"��ȡ��������",NULL,MB_ICONSTOP);
		return (false);
	}
	fread((char*)buffer,sizeof(int),1,fp);
	fclose(fp);
	int result=buffer[0];
	free(buffer);
	return(result);
}

/***********��ȡѵ��������ֵ**************/
bool  BPNNRec::readInput(double **w,int n1,int n2,char *name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"�޷���ȡȨֵ��Ϣ",NULL,MB_ICONSTOP);
		return (false);
	}
	buffer=(double*)malloc(n1*n2*sizeof(double));
	fread((char*)buffer,sizeof(double),n1*n2,fp);
	
	CString str;
	/*
	for(i=0;i<20;i++)
	{
	str.Format("%f",buffer[i]);
	::MessageBox(NULL,str,"��ȡ���",NULL);
	}
	*/
	
	
	for(i=0;i<n1;i++)
	{
		for(j=0;j<n2;j++)
			w[i][j]=buffer[i*n2+j];
		/*
        str.Format("%f",w[i][j]);
		::MessageBox(NULL,str,"��ȡ���",NULL);
	    */
	  }
	
	fclose(fp);
	free(buffer);
	return(true);
}
/****************************************************
* �������� BpTrain()
* 
* ������
*   double **data_in    -ָ��������������������ָ��    
*	double **data_out   -ָ��������������ָ��
int n_in            -�������ĸ��� 
*   int n_hidden        -BP�������������Ŀ
*   double min_ex       -ѵ��ʱ��������������
*   double momentum     -BP��������ϵ��
*   double eta          -BP�����ѵ������
*   int num             -���������ĸ���
*
* �������ܣ�
*     ����������������������������������BP���羡��ѵ��
*     ѵ��������Ȩֵ���沢��ѵ���Ľ����ʾ����
********************************************************/
void BPNNRec::BpTrain(double ** data_in, double** data_out,int num,double *&errRecord,int *errNum)
{
	

  
	int i,k,l;
	int n_in=bp->input_n;
	int n_hidden=bp->hidden_n;
	//���������Ŀ
	int  n_out=bp->output_n;   
	//ָ����������ݵ�ָ��
	double* input_unites=bp->input_units; 
	//ָ���������ݵ�ָ��
	double* hidden_unites=bp->hidden_units;
	//ָ����������ݵ�ָ��
	double* output_unites=bp->output_units; 
	//ָ������������ݵ�ָ��
	double* hidden_deltas=bp->hidden_delta;
	//ָ���������������ָ��
	double* output_deltas=bp->output_delta;  
	//ָ������Ŀ�������ָ��
	double* target=bp->target;    
	//ָ�������������֮��Ȩֵ��ָ��
	double** input_weights=bp->input_weights;
	//ָ�������������֮���Ȩֵ��ָ��
	double** hidden_weights=bp->hidden_weights;
	//ָ����һ�������������֮��Ȩֵ��ָ��
	double** input_prev_weights=bp->input_prev_weights ;
	//ָ����һ�������������֮���Ȩֵ��ָ��
	double** hidden_prev_weights=bp->hidden_prev_weights;
	//ÿ��ѭ����ľ���������ֵ 
	double ex;
  
	errRecord=createDoubleArray1(max_count);
	for(int a=0;a<max_count;a++)
		errRecord[a]=0.0;

	CString str;
/*	str.Format ("���룺%d�������أ�%d��,�����%d��",n_in,n_hidden,n_out);
		
	::MessageBox(NULL,str,"���������",NULL);
*/	
	//Ϊ�������ݽṹ�����ڴ�ռ�
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

	
	
	//�Ը���Ȩֵ���г�ʼ����ʼ��
//	bpnn_randomize_weights( input_weights,n_in,n_hidden);
//	bpnn_randomize_weights( hidden_weights,n_hidden,n_out);
//	bpnn_zero_weights(input_prev_weights, n_in,n_hidden );
//	bpnn_zero_weights(hidden_prev_weights,n_hidden,n_out );
/*   if (input_weights[0][0]!=0&&hidden_weights[0][0]!=0)
	AfxMessageBox("Ȩֵ�Ѿ�����ʼ����ϣ�");
	else
    AfxMessageBox("Ȩֵ����û�б���ʼ����");

	AfxMessageBox("��ʼ����BP����ѵ��!");
*/
    //��ʼ����BP����ѵ��
	//�����趨���ĵ�������Ϊ15000��
	for(l=0;l<max_count;l++)  
	{ 
		//�Ծ����������
		ex=0;
		//���������������ɨ��
		for(k=0;k<num;k++)  
		{ 
			//����ȡ�������������������͵��������
			for(i=1;i<=n_in;i++)
				input_unites[i] = data_in[k][i-1];
			
			//��Ԥ��������������͵�BP��������������Ԫ
			for(i=1;i<=n_out;i++)
				target[i]=data_out[k][i-1];
			
			//ǰ���伤��
			
			//������������㴫������ 
			bpForward(input_unites,hidden_unites,
				input_weights, n_in,n_hidden);
			//�������������������
			bpForward(hidden_unites, output_unites,
				hidden_weights,n_hidden,n_out);
			
			//������
			
			//���������������������Ƚϼ��������ÿ������ϵ����
			bpErrorOutput(output_deltas,target,output_unites,n_out);
			//������������ϵ�����������ÿ���ڵ��ϵ����
			bpErrorHidden(hidden_deltas,n_hidden, output_deltas, n_out,hidden_weights, hidden_unites);
			
			//Ȩֵ����
			//���������ÿ���ڵ��ϵ���������������������֮���Ȩֵ    
			bpWeightsAdjust(output_deltas,n_out, hidden_unites,n_hidden,
				hidden_weights, hidden_prev_weights, eta, momentum); 
			//��������ÿ���ڵ��ϵ���������������������֮���Ȩֵ    	
			bpWeightsAdjust(hidden_deltas, n_hidden, input_unites, n_in,
				input_weights, input_prev_weights, eta, momentum);  
			
			//���ͳ��		
			for(i=1;i<=n_out;i++)
				ex+=(output_unites[i]-data_out[k][i-1])*(output_unites[i]-data_out[k][i-1]);
		}
		//����������
		ex=ex/double(num*n_out);

        errRecord[l]=ex;
		//�����������Ѿ��㹻��С������ѭ����ѵ�����  
		if(ex<min_ex)break;
	}
	
	//��ر���
	*errNum=l;
	//���������������֮���Ȩֵ
	writeWeights(input_weights,n_in,n_hidden,"in_hi.dat");
	//���������������֮���Ȩֵ
	writeWeights(hidden_weights,n_hidden,n_out,"hi_out.dat");
	
	//���������ĸ���
	writeNum(n_in,n_hidden,n_out,"num");
	
	//��ʾѵ�����
	
/*	
	if(ex<=min_ex)
	{
		str.Format ("����%d�Σ�\nƽ�����%.4f",l,ex);
		
		::MessageBox(NULL,str,"ѵ�����",NULL);
	}
	
	if(ex>min_ex)
	{
		
		str.Format("����%d�Σ�ƽ�����%.4f\n���Ѿ��������Ŭ���˻��Ǵﲻ������Ҫ��\n�������������ѵ���ɣ�",l,ex);
		::MessageBox(NULL,str,"ѵ�����",NULL);
	}
*/	
	//�ͷ��ڴ�ռ�
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

//LWBP�㷨��ѵ������
void BPNNRec::LWBpTrain(double ** data_in, double** data_out,int num,double *&errRecord,int *errNum)
{
	

    double sub_result;
	int i,j,k,l,LWErrNum=0;
	int n_in=bp->input_n;
	int n_hidden=bp->hidden_n;
	//���������Ŀ
	int  n_out=bp->output_n;   
	//ָ����������ݵ�ָ��
	double* input_unites=bp->input_units; 
	//ָ���������ݵ�ָ��
	double* hidden_unites=bp->hidden_units;
	//ָ����������ݵ�ָ��
	double* output_unites=bp->output_units; 
	//ָ������������ݵ�ָ��
	double* hidden_deltas=bp->hidden_delta;
	//ָ���������������ָ��
	double* output_deltas=bp->output_delta;  
	//ָ������Ŀ�������ָ��
	double* target=bp->target;    
	//ָ�������������֮��Ȩֵ��ָ��
	double** input_weights=bp->input_weights;
	double** old_input_weights;
	//ָ�������������֮���Ȩֵ��ָ��
	double** hidden_weights=bp->hidden_weights;
	double** old_hidden_weights;
	//ָ����һ�������������֮��Ȩֵ��ָ��
	double** input_prev_weights=bp->input_prev_weights ;
	//ָ����һ�������������֮���Ȩֵ��ָ��
	double** hidden_prev_weights=bp->hidden_prev_weights;
	//ÿ��ѭ����ľ���������ֵ 
	double ex,old_ex;


	CString str;
/*	str.Format ("���룺%d�������أ�%d��,�����%d��",n_in,n_hidden,n_out);
		
	::MessageBox(NULL,str,"���������",NULL);
*/
	//Ϊ����¼�������ռ�
	errRecord=createDoubleArray1(max_count);
	for(int a=0;a<max_count;a++)
		errRecord[a]=0.0;
	
	//Ϊ�������ݽṹ�����ڴ�ռ�
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

	
	
	//�Ը���Ȩֵ���г�ʼ����ʼ��
//	bpnn_randomize_weights( input_weights,n_in,n_hidden);
//	bpnn_randomize_weights( hidden_weights,n_hidden,n_out);
//	bpnn_zero_weights(input_prev_weights, n_in,n_hidden );
//	bpnn_zero_weights(hidden_prev_weights,n_hidden,n_out );
/*  if (input_weights[0][0]!=0&&hidden_weights[0][0]!=0)
	AfxMessageBox("Ȩֵ�Ѿ�����ʼ����ϣ�");
	else
    AfxMessageBox("Ȩֵ����û�б���ʼ����");

	AfxMessageBox("��ʼ����BP����ѵ��!");
*/
  //��ʼ����BP����ѵ��
	//�����趨���ĵ�������Ϊ15000��
	for(l=0;l<max_count;l++)  
	{ 
		//�Ծ����������
		ex=0;
		//���汾�β���֮ǰ�ĸ���Ȩֵ
		for(i=1;i<=n_in;i++)
			for(j=1;j<=n_hidden;j++)
				old_input_weights[i][j]=input_weights[i][j];
		for(i=1;i<n_hidden;i++)
			for(j=1;j<=n_out;j++)
				old_hidden_weights[i][j]=hidden_weights[i][j];


		//���������������ɨ��
		for(k=0;k<num;k++)  
		{ 
			//����ȡ�������������������͵��������
			for(i=1;i<=n_in;i++)
				input_unites[i] = data_in[k][i-1];
			
			//��Ԥ��������������͵�BP��������������Ԫ
			for(i=1;i<=n_out;i++)
				target[i]=data_out[k][i-1];
			
			//ǰ���伤��
			
			//������������㴫������ 
			bpForward(input_unites,hidden_unites,
				input_weights, n_in,n_hidden);
			//�������������������
			bpForward(hidden_unites, output_unites,
				hidden_weights,n_hidden,n_out);
			
			//������
			
			//���������������������Ƚϼ��������ÿ������ϵ����
			bpErrorOutput(output_deltas,target,output_unites,n_out);
			//������������ϵ�����������ÿ���ڵ��ϵ����
			bpErrorHidden(hidden_deltas,n_hidden, output_deltas, n_out,hidden_weights, hidden_unites);
			
		
			//Ȩֵ����
			//���������ÿ���ڵ��ϵ���������������������֮���Ȩֵ    
			bpWeightsAdjust(output_deltas,n_out, hidden_unites,n_hidden,
				hidden_weights, hidden_prev_weights, eta, momentum); 
			//��������ÿ���ڵ��ϵ���������������������֮���Ȩֵ    	
			bpWeightsAdjust(hidden_deltas, n_hidden, input_unites, n_in,
				input_weights, input_prev_weights, eta, momentum);  
			
			//���ͳ��		
			for(i=1;i<=n_out;i++)
				ex+=(output_unites[i]-data_out[k][i-1])*(output_unites[i]-data_out[k][i-1]);
		    
		}
		//����������
		ex=ex/double(num*n_out);
		if (LWErrNum==0)
				old_ex=ex;
		else
		//�����������Ѿ��㹻��С������ѭ����ѵ�����
		{
		sub_result=ex-old_ex;
		if(sub_result>=0)
		{
			if(sub_result/old_ex>eta_scale)
			{
				//ȡ����һ�ε�Ȩֵ�Ķ�
                for(k=0;k<num;k++)  
				{
					for(i=1;i<=n_in;i++)
						for(j=1;j<=n_hidden;j++)
							input_weights[i][j]=old_input_weights[i][j];
					for(i=1;i<=n_hidden;i++)
						for(j=1;j<=n_out;j++)
							hidden_weights[j][j]=old_hidden_weights[i][j];
				}
                //����ѧϰ����
				eta=eta*eta_sub;
				//��������������Ϊ0
				old_momentum=momentum;
				momentum=0;
				ex=old_ex;//������Ҫ��,�ص��ϴα��ֵ����ֵ
				
			}
			else
			{
				if ((momentum==0)&&(old_momentum!=0))
					momentum=old_momentum;
				old_ex=ex;  //���汾�ε����
			}
		}
		else //����������ϴ����Ĳ�ֵС��0,�����Ȩֵ����,ѧϰ�������Դ���1������,
			//����������ӹ�ȥ����Ϊ0,��ָ���ԭ����ֵ
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
	
    
	//����������ݵĸ���
	*errNum=l;
	
	//���������������֮���Ȩֵ
	writeWeights(input_weights,n_in,n_hidden,"in_hi.dat");
	//���������������֮���Ȩֵ
	writeWeights(hidden_weights,n_hidden,n_out,"hi_out.dat");
	
	//���������ĸ���
	writeNum(n_in,n_hidden,n_out,"num");
	
	//��ʾѵ�����
/*	static BOOL bShow = FALSE;
	
    if(bShow)
    {
		if(ex<=min_ex)
		{
			str.Format ("����%d�Σ�\nƽ�����%.4f",l,ex);
			::MessageBox(NULL,str,"ѵ�����",NULL);
		}
		if(ex>min_ex)
		{
			str.Format("����%d�Σ�ƽ�����%.4f\n���Ѿ��������Ŭ���˻��Ǵﲻ������Ҫ��\n�������������ѵ���ɣ�",l,ex);
		    ::MessageBox(NULL,str,"ѵ�����",NULL);
		}
    }
    bShow = TRUE;
*/	

	free(old_input_weights);
	free(old_hidden_weights);
	//�ͷ��ڴ�ռ�
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
	//ѭ������
	int i,k;
	// ָ��ʶ������ָ�� 
	//Ϊ���ʶ��Ľ������洢�ռ�
	recgonize_result=(int*)malloc(num*sizeof(int));
	
	//ָ����������ݵ�ָ��
	double* input_unites; 
	//ָ���������ݵ�ָ��
	double* hidden_unites;
	//ָ����������ݵ�ָ��
	double* output_unites; 
	//ָ�������������֮��Ȩֵ��ָ��
	double** input_weights;
	//ָ�������������֮���Ȩֵ��ָ��
	double** hidden_weights;
	//Ϊ�������ݽṹ�����ڴ�ռ�
	input_unites= bp->input_units;
	hidden_unites= bp->hidden_units;
	output_unites=bp->output_units;
	input_weights= bp->input_weights;
	hidden_weights = bp->hidden_weights;	
	
	//��ȡȨֵ
	if(readWeights(input_weights,bp->input_n,bp->hidden_n,"in_hi.dat")==false)
		return;
	if(readWeights(hidden_weights,bp->hidden_n,bp->output_n,"hi_out.dat")==false)
		return;
	
	
	//�������ɨ��
	for(k=0;k<num;k++)
	{ 
		//����ȡ�������������������͵��������
		for(i=1;i<=n_in;i++)
			input_unites[i]=data_in[k][i-1];
		
		//ǰ�����뼤��
        bpForward(input_unites,hidden_unites,
			input_weights, n_in,n_hidden);
        bpForward(hidden_unites, output_unites,
			hidden_weights,n_hidden,n_out);
		
		//��������������ʶ��
		int result=0 ;
		//����ÿһλ�����
		int recnum=0;
		for(i=1;i<=n_out;i++)
		{
			//�������0.5��Ϊ1
			if((output_unites[i]<0.75)&&(output_unites[i]>0.25))
				recnum++;
			if(output_unites[i]>0.5)
				
				result+=(int)pow(2,double(3-i));
		}
		
		//����ж��Ľ��С�ڵ���7����Ϊ����
		//if(result<=7)
			recgonize_result[k]=result;
		//����ж��Ľ������7����Ϊ�����������λΪһ������ֵ-1
		if(recnum>=2)
			recgonize_result[k]=-1;
		
	}	
}


