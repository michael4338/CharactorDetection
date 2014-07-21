class BPNNRec
{
public:
	  BPNNRec(BPNNBase *net,double ex,double mot,double step,int loop_count);
	  BPNNRec(BPNNBase *net,double ex,double mot,double step,int loop_count,double LW_etaadd,double LW_etasub,double LW_etascale);
	  BPNNRec(BPNNBase *net);
	  ~BPNNRec();
void  BpTrain(double ** data_in, double** data_out,int num,double *&errRecord,int *errNum);
void  LWBpTrain(double ** data_in, double** data_out,int num,double *&errRecord,int *errNum);
void  BpRecognize(double **data_in, int num , int *&recgonize_result);
bool  readNum(char *name);
void  setLWParam(double LW_etaadd,double LW_etasub,double LW_etascale);
void  writeInput(double **w,int n1,int n2,char*name);
bool  readInput(double **w,int n1,int n2,char *name);
int   readSampleNum(char *name);
void  writeSampleNum(int n1,char*name);
void  addToSample(double **w,int n1,int n2,char *name);
BPNNBase* getBP();

//friend class BPNNBase;
private:
	  BPNNBase *bp;      
	  double min_ex;      //训练时允许的最大均方误差
	  double momentum;    //BP网络的动量因子
	  double old_momentum;//记录上一次修改的动量因子
	  double eta;         //BP网络的训练步长
	  int    max_count;   //最大迭代次数
	  double eta_scale;   //LWBP算法中误差增长的范围百分比(1%-5%之间)
	  double eta_add;     //LWBP算法中学习步长的增长比例
	  double eta_sub;     //LWBP算法中学习步长减少的比例
/* 前向运算 */
void  bpForward(double *l1, double *l2, double **conn, int n1, int n2);
//void  bpnn_feedforward(BPNNBase *net);
void  bpErrorOutput(double *delta, double *target, double *output, int nj);
void  bpErrorHidden(double* delta_h, int nh, double *delta_o, int no, double **who, double *hidden);
void  bpWeightsAdjust(double *delta, int ndelta, double *ly, int nly, double** w, double **oldw, double eta, double momentum);
void  writeWeights(double **w,int n1,int n2,char*name);
bool  readWeights(double **w,int n1,int n2,char *name);
void  writeNum(int n1,int n2,int n3,char*name);

};