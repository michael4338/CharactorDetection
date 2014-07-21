//class BPNNRec;

class BPNNBase{
public:
	BPNNBase();
	BPNNBase(int input,int hidden,int output);
	~BPNNBase();
void rndWeights(double **w, int m, int n);    /*随机初始化权值*/
void zeroWeights(double **w, int m, int n);         /*0值初始化权值*/
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
	int input_n;                  /* 输入层的神经元个数 */
	int hidden_n;                 /* 隐含层的神经元个数 */
	int output_n;                 /* 输出层的神经元个数 */

	double *input_units;          /* 输入层的神经元 */
	double *hidden_units;         /* 隐藏层的神经元 */
	double *output_units;         /* 输出曾的神经元 */

	double *hidden_delta;         /* 隐藏层的误差 */
	double *output_delta;         /* 输出层的误差 */

	double *target;               /* 目标向量 */

	double **input_weights;       /* 输入层到隐藏层的连接权 */
	double **hidden_weights;      /* 隐藏层到输出曾的连接权 */

                                /*** 下面两个在迭代时使用 ***/
	double **input_prev_weights;  /* 前次输入层导隐藏层权值的改变 */
	double **hidden_prev_weights; /* 前次隐藏层导输出层权值的改变 */
} ;
