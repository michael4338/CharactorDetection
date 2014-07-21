//class BPNNRec;

class BPNNBase{
public:
	BPNNBase();
	BPNNBase(int input,int hidden,int output);
	~BPNNBase();
void rndWeights(double **w, int m, int n);    /*�����ʼ��Ȩֵ*/
void zeroWeights(double **w, int m, int n);         /*0ֵ��ʼ��Ȩֵ*/
void setBpnnData(int input,int hidden,int output);
void createBpnnData();
void createBpnnDeltaData();
void createBpnnWeights();
void initBpnnWeights();
void initBPNN(int input,int hidden,int output);
void initSeed(int seed);
int  getInputNum();
int  getHiddenNum();
int  getOutputNum();
friend class BPNNRec;
private:
	int input_n;                  /* ��������Ԫ���� */
	int hidden_n;                 /* ���������Ԫ���� */
	int output_n;                 /* ��������Ԫ���� */

	double *input_units;          /* ��������Ԫ */
	double *hidden_units;         /* ���ز����Ԫ */
	double *output_units;         /* ���������Ԫ */

	double *hidden_delta;         /* ���ز����� */
	double *output_delta;         /* ��������� */

	double *target;               /* Ŀ������ */

	double **input_weights;       /* ����㵽���ز������Ȩ */
	double **hidden_weights;      /* ���ز㵽�����������Ȩ */

                                /*** ���������ڵ���ʱʹ�� ***/
	double **input_prev_weights;  /* ǰ������㵼���ز�Ȩֵ�ĸı� */
	double **hidden_prev_weights; /* ǰ�����ز㵼�����Ȩֵ�ĸı� */
} ;
