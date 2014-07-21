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
	  double min_ex;      //ѵ��ʱ��������������
	  double momentum;    //BP����Ķ�������
	  double old_momentum;//��¼��һ���޸ĵĶ�������
	  double eta;         //BP�����ѵ������
	  int    max_count;   //����������
	  double eta_scale;   //LWBP�㷨����������ķ�Χ�ٷֱ�(1%-5%֮��)
	  double eta_add;     //LWBP�㷨��ѧϰ��������������
	  double eta_sub;     //LWBP�㷨��ѧϰ�������ٵı���
/* ǰ������ */
void  bpForward(double *l1, double *l2, double **conn, int n1, int n2);
//void  bpnn_feedforward(BPNNBase *net);
void  bpErrorOutput(double *delta, double *target, double *output, int nj);
void  bpErrorHidden(double* delta_h, int nh, double *delta_o, int no, double **who, double *hidden);
void  bpWeightsAdjust(double *delta, int ndelta, double *ly, int nly, double** w, double **oldw, double eta, double momentum);
void  writeWeights(double **w,int n1,int n2,char*name);
bool  readWeights(double **w,int n1,int n2,char *name);
void  writeNum(int n1,int n2,int n3,char*name);

};