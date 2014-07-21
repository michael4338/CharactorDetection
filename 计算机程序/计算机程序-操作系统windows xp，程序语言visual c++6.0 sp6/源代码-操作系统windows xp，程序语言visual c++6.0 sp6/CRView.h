// CRView.h : interface of the CCRView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRVIEW_H__AD6ED2BD_9EB8_4D8D_BB50_082A4D0E3B08__INCLUDED_)
#define AFX_CRVIEW_H__AD6ED2BD_9EB8_4D8D_BB50_082A4D0E3B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRDoc.h"
#include "PointMatrix.h"
#include "DIBAPI.h"
#include "PointMatrixWindow.h"
#include "BPNNBase.h"
#include "BPNNRec.h"


class CCRCntrItem;
class CCRView;

enum  ViewType{NONE,MATRIX,HANDWRITINGPAT,PICTURE,MULTIMATRIX}; //视图窗口中显示的对象类型


//定义全局变量
typedef struct _GLOBAL_STRUCTURE
{
	CCRView* m_pgCRView;     //当前视图对象指针
	char m_szOutput[20];     //最终输出结果字符
	_GLOBAL_STRUCTURE(){m_pgCRView = NULL;memset(m_szOutput,0,20);strcpy(m_szOutput,"ABCUNIXZ");}
}GLOBAL_STRUCTURE, *PGLOBALSTRUCTURE;




/*样本类及其相关变量///////////////////////////////////////////////////////////////*/

#define                    SAMPLE_FILE_NAME                 "sample.dat"

class SAMPLELIST; 


class SAMPLE
{
	friend class SAMPLELIST;                    //声明友元类
	
public:

	//基本类成员函数
	SAMPLE(){link = NULL;}
	SAMPLE(int width, int height, int type, int samvalue, double** data)
	{
		m_Width = width;
		m_Height = height;
		m_Type = type;
		m_Samvalue = samvalue;
        if(width > 32 || height >32)
			return;
		else
		{
			for(int i=0;i<height;i++)
				for(int j=0;j<width;j++)
					m_Samdata[i][j] = data[0][i*width+j];
		}
	}
	SAMPLE* NextSample() {return link;}    //当前节点的下一个节点

	//在当前节点后插入一个新节点
	void InsertAfterSelf(SAMPLE* sample)
	{
		sample->link = link;
	    link = sample;
	}
          
	//删除当前节点的下一个节点
	SAMPLE* RemoveAfterSelf()
	{
		SAMPLE* temp = link;
    	if(link == NULL) return NULL;
	    link = temp->link;
    	return temp;
	}
	
//private:
public:
	
	SAMPLE *link;

	int m_Width;         //样本宽度
	int m_Height;        //样本高度
	int m_Type;          //样本类型，分别可为图片，手写板，点阵
	int m_Samvalue;      //样本值
	double m_Samdata[32][32];//样本的二维数组
};

//CONNECTION链表类
class SAMPLELIST
{
public:

	//基本类成员函数
	SAMPLELIST(){Last = First = new SAMPLE();}
	~SAMPLELIST(){MakeEmpty();}
	//将链表置为空表

	SAMPLE* GetFirstNode(){return First;}

	void MakeEmpty()
	{
		SAMPLE *temp;
	    while(First->link != NULL)
		{
			temp = First->link;
		    First->link = temp->link;
		    delete temp;
		}	
	    Last = First;
	}
	//计算链表的长度
	int Length() const
	{
		SAMPLE *temp = First->link;
	    int count = 0;
	    while(temp != NULL)
		{
			temp = temp->link;
		    count++;
		}
		return count;
	}
	//根据位置寻找指定节点
	SAMPLE* FindByPos(int pos)
	{
		SAMPLE *temp = First->link;
	    while(temp != NULL && !(pos--))
			temp = temp->link;
	    return temp;
	}
	//将新节点接到链表上
	void AppendList(SAMPLE *add)
	{	
		Last->link = add;
	    add->link = NULL;
	    Last = add;
	}
	//将指定节点删除
	void RemoveNode(SAMPLE *del)
	{
		SAMPLE *temp = First->link;
	    SAMPLE *pretemp = First;
	    while(temp != NULL && temp != del)
		{
			pretemp = temp;
	    	temp = temp->link;
		}
	    if(temp != NULL)
		{
			if(temp == Last)
				Last = pretemp;
		    pretemp->RemoveAfterSelf(); 
		}
	}
	//经指定位置节点删除
	void RemoveNodeAt(int nIndex)
	{
		SAMPLE *temp = First->link;
	    SAMPLE *pretemp = First;
		int i = 0;
	    while(temp != NULL && i < nIndex)
		{
			pretemp = temp;
	    	temp = temp->link;
			i++;
		}
	    if(temp != NULL && i == nIndex)
		{
			if(temp == Last)
				Last = pretemp;
		    pretemp->RemoveAfterSelf(); 
		}
	}
	//从文件中得到链表
	void LoadSampleFromFile(char* filename)
	{
		FILE* fp;
		fp = fopen(filename, "rb");
		if(fp == NULL)return;
		while(!feof(fp))
		{
			SAMPLE* sample = new SAMPLE();
			if(fread(sample,sizeof(SAMPLE),1,fp) != 1)
			{
				if(feof(fp)){fclose(fp);return;}
			}
			AppendList(sample);
		}
		fclose(fp);
	}
	//把链表写入文件
	void WriteSampleToFile(char* filename)
	{
		//因为View的析构函数要执行两次，所以阻止第二次执行
		static BOOL bTwice = FALSE;
		if(bTwice) return;

		FILE* fp;
		SAMPLE *sample = First->link;			
		fp = fopen(filename, "wb");
		
		if(sample == NULL)
		{
			fclose(fp);
		    bTwice = TRUE;
			return;
		}	
		while(sample != NULL)
		{
			if(fwrite(sample,sizeof(SAMPLE),1,fp) != 1)
			{
				fclose(fp);
				bTwice = TRUE;
				return;
			}
			sample = sample->link;
		}
		fclose(fp);
		bTwice = TRUE;
	}
 	//把链表保存入文件
	void SaveSampleToFile(char* filename)
	{
		FILE* fp;
		SAMPLE *sample = First->link;			
		fp = fopen(filename, "wb");
		
		if(sample == NULL)
		{
			fclose(fp);
			return;
		}	
		while(sample != NULL)
		{
			if(fwrite(sample,sizeof(SAMPLE),1,fp) != 1)
			{
				fclose(fp);
				return;
			}
			sample = sample->link;
		}
		fclose(fp);
	}
	//从链表中得到二维数组指针
	double** GetDataFromSampleList(int type, int width, int height, int& dbarraysize, int *&arrayvalue)
	{
		int num = 0;
		double** dbarray = NULL;
			
		SAMPLE* sample = First;
		while(sample != NULL)
		{
			if(sample->m_Type == type  &&  sample->m_Width == width  &&  sample->m_Height == height)
				num++;
			sample = sample->link;
		}
		if(num == 0)
		{
			dbarraysize = 0;
			return NULL;
		}
		sample = First;
		int i = 0;
		dbarray =  (double **) malloc ((unsigned) (num * sizeof (double *)));
			
		arrayvalue = (int*)malloc((unsigned)((num)*sizeof(int)));
		if(arrayvalue == NULL)
		{
			AfxMessageBox("内存不足");
			return NULL;
		}
        memset(arrayvalue,0,sizeof(int)*(num));
		while(sample != NULL && i < num)
		{
			if(sample->m_Type == type  &&  sample->m_Width == width  &&  sample->m_Height == height)
			{
				arrayvalue[i] = sample->m_Samvalue;
				dbarray[i] = (double *) malloc ((unsigned) (width * height * sizeof (double)));
				for(int j=0;j<height;j++)
					for(int k=0;k<width;k++)
						dbarray[i][j*width+k] = sample->m_Samdata[j][k];
				i++;
			}
				
			sample = sample->link;
		}
		dbarraysize = num;
		return dbarray;
	}

private:
	SAMPLE *First, *Last;
};

/*样本类及其相关变量结束/////////////////////////////////////////////////////////*/



/*手写板类及其相关变量///////////////////////////////////////////////////////////*/

#define      HANDPAT_POINT_COLOR               RGB(255,0,0)       //默认点的颜色
#define      HANDPAT_PEN_WIDTH                 3                  //默认笔宽度
#define      HANDPAT_BACK_COLOR                RGB(211,211,211)   //默认背景颜色

#define      HANDPAT_BORDER_WIDTH              2                  //手写板边框宽度
#define      HANDPAT_BORDER_COLOR_OUT_LEFT     RGB(255,255,255)   //手写板外左边框颜色
#define      HANDPAT_BORDER_COLOR_IN_LEFT      RGB(192,192,192)   //手写板内左边框颜色
#define      HANDPAT_BORDER_COLOR_OUT_RIGHT    RGB(109,109,109)   //手写板外右边框颜色
#define      HANDPAT_BORDER_COLOR_IN_RIGHT     RGB(147,147,147)   //手写板内右边框颜色

#define      MULTI_MATRIX_SIZE                 50                 //点阵数组的大小

class CStroke: public CObject
{
public:
	CStroke(){m_bErazor = FALSE;}
	virtual ~CStroke(){}

public:
    CArray<CPoint,CPoint> m_PointArray;	
	BOOL                  m_bErazor;
};

class CHandWritingPat
{
public:
	CHandWritingPat(int penwidth = HANDPAT_PEN_WIDTH, COLORREF PointColor = HANDPAT_POINT_COLOR, COLORREF BackColor =  HANDPAT_BACK_COLOR)
	{m_nPenWidth = penwidth;m_PointColor = PointColor;m_BackColor = BackColor;};

	~CHandWritingPat()
	{m_StrokeList.RemoveAll();}
protected:
	CRect     m_ClientRect;                            //本控件客户区
	CRect     m_HandPatRect;                           //手写板区域

	int       m_nPenWidth;                             //笔宽度
	COLORREF  m_PointColor;                            //点的颜色
	COLORREF  m_BackColor;                             //背景颜色

public:
	CArray<CPoint,CPoint> m_PointArray;                //点的数组
	CTypedPtrList<CObList,CStroke*>m_StrokeList;       //点的数组的链表

public:
	
	void SetPenWidth(int width){m_nPenWidth = width;}  //设置点的大小
	void SetPointColor(COLORREF color){m_PointColor = color;}//设置点的颜色
	void SetBackColor(COLORREF color){m_BackColor = color;}//设置背景颜色
	COLORREF GetBackColor(){return m_BackColor;}       //得到背景颜色 
	COLORREF GetPointColor(){return m_PointColor;}     //得到点的颜色
	int GetPenWidth(){return m_nPenWidth;}             //得到画笔宽度
	void ResetPointArray(){m_StrokeList.RemoveAll();}  //清除画面上的点
	

};
/*////////////////////////////////////////////////////////////////////////////////*/
  


class CCRView : public CFormView
{
public:
	CCRView();
	
protected: // create from serialization only
	DECLARE_DYNCREATE(CCRView)

public:
	//{{AFX_DATA(CCRView)
	enum{ IDD = IDD_CR_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CCRDoc* GetDocument();
	// m_pSelection holds the selection to the current CCRCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CCRCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
	CCRCntrItem* m_pSelection;

protected:
	CPointMatrix*  m_pMatrix;           //视图类中的点阵
	CRect       m_MatrixRect;           //点阵容器的位置
	int         m_nWidth;               //点阵的宽度
	int         m_nHeight;              //点阵的高度
	BOOL        m_bLButtonDown;         //判断鼠标左键是否已按下
//	HDIB        m_hMatrixDIB;           //点阵要转化而成的DIB句柄
	
    CHandWritingPat* m_pHandWritingPat; //视图类中的手写板
	CRect       m_HandWritingRect;      //手写板容器的位置
	int         m_nHandWritingWidth;    //手写板的宽度
	int         m_nHandWritingHeight;   //手写板的高度
	CPoint      m_ptPrev;               //上一个点
	CStroke*    m_pStrokeCur;           //当前线
//	HDIB        m_hHandPatDIB;          //手写板要转化而成的DIB句柄

	HDIB        m_hPictureDIB;          //图片要转化而成的DIB

    CPointMatrix* m_pMultiMatrixs[MULTI_MATRIX_SIZE];//一个点阵的指针数组
	int         m_nMutliMatrixsNum;     //点阵指针数组元素个数

    ViewType    m_nCurType;             //当前视图中显示的对象类型

	ViewType    m_nSampleCurType;       //当前样本类型
	ViewType    m_nSampleLastType;      //记录上次对象类型，用于训练和识别的同步

	double**    m_data;                 //预处理后输出的数组，用于算法和显示的输入
	int         m_datalen;              //数组的长度
	int         m_nDataWidth;
	int         m_nDataHeight;          //预处理后的点阵宽度和高度

	CPointMatrixWindow* m_pDisplayWindow;//右边显示窗口

	SAMPLELIST  m_SampleList;            //样本链表对象

// Operations
public:
    UINT  GetCurWinID(CPoint point);                     //根据当前点的位置得到窗口ID
    void  ShowMatrix(int nRow, int nCol);                //显示左边的点阵
	int   GetMatrixWidth(){return m_nWidth;}             //得到点阵的宽度
	int   GetMatrixHeight(){return m_nHeight;}           //得到点阵的高度
	void  SetMatrixWidth(int nWidth);                    //设置点阵的宽度
	void  SetMatrixHeight(int nHeight);                  //设置点阵的高度
	void  ShowHandWritingPat(void);                      //显示手写板
	void  DeleteItems(ViewType survive);                 //删除视图上的元素
	void  SetHandPatWidth(int nWidth);                   //设置手写板的宽度
	void  SetHandPatHeight(int nHeight);                 //设置手写板的高度
  
public:
	CPen* CreatePen(CDC* pDC);                           //创建画笔
	void  CreateBckGround(CDC* pDC);                     //创建背景
	void  CreateHandPatBorder(CDC *pDC);                 //创建手写板的边框
    void  DrawLine(CDC* pDC, CPoint start, CPoint end, int width, COLORREF color);//画线
    void  BrushRect(CDC* pDC, CRect rect, COLORREF color);//填充区域
	
public:
	void OnPreProcess(int DesireWidth, int DesireHeight); //预处理操作入口
	void ShowPicture();                                   //打开图片
    void FileReLoadBmp();                                 //重新打开图片文件
	double** PreProcess(int Width,int Height,HDIB hDIB);  //对图像文件做一次性归一化处理
	HDIB CreateHDIBByDbArray(BYTE* pointarray, int Width, int Height);//根据二维数组创建DIB句柄
    HDIB CreateHDIBByScreen(CRect* ScreenRect);           //根据屏幕位置创建DIB句柄
    HDIB CreateHDIBByPicture(CString strPathFileName);    //根据图像文件创建DIB句柄

	void ShowPicInMatrix(HDIB hDIB);                      //将当前DIB中的内容输出到点阵中去
	void ResultShow(BOOL);                                //显示或隐藏预处理结果

public:
	void ShowMultiMatrixs(ResetDisplayMessageStruct* display);//显示多个点阵
	void HideMultiMatrixs();                              //隐藏多个点阵

public:
	CHandWritingPat* GetHandPatPointer(){return m_pHandWritingPat;}
	CPointMatrix* GetMatrixPointer(){return m_pMatrix;}
    SAMPLELIST* GetSampleList(){return &m_SampleList;}

public:
	void BpTrain(int combp, double momentum, double min_ex, int n_hidden, double eta, double addrate, double subrate, double maxerror);//训练
    void GetSampleInfo(double**& data, int& width, int& height, int &num, int& type);
    void ShowSample();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCRView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnCancelEditSrvr();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnInputMatrix();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnInputHandPat();
	afx_msg void OnInputPicture();
	afx_msg BOOL OnIMGPRC256ToGray();          //图像预处理第1步：将256色图像转化为灰度图像
	afx_msg BOOL OnIMGPRCGrayToWhiteBlack();   //图像预处理第2步：将灰度图二值化
	afx_msg BOOL OnImgprcSharp();              //图像预处理第3步：梯度锐化	
    afx_msg BOOL OnImgprcRemoveNoise();        //图像预处理第4步：去离散杂点噪声
    afx_msg BOOL OnImgprcAdjustSlope();        //图像预处理第5步：倾斜度调整
    afx_msg BOOL OnImgprcDivide();             //图像预处理第6步：分割，并在分割出来的字符外面画框以标识
	afx_msg BOOL OnImgprcStandarize();         //图像预处理第7步：标准归一化将分割出来的各个不同宽、高的数字字符宽、高统一
    afx_msg BOOL OnImgprcShrinkAlign();        //图像预处理第8步：紧缩重排已经分割完毕的数字字符，并形成新的位图句柄
    afx_msg void OnBpnetTrain();               //训练神经网络
	afx_msg void OnBpnetRecognize();           //神经网络识别
	afx_msg void OnShowresult();
	afx_msg void OnPreproAll();
	afx_msg void OnAddSample();
	afx_msg void OnMatrix88();
	afx_msg void OnMatrix816();
	afx_msg void OnMatrix832();
	afx_msg void OnMatrix168();
	afx_msg void OnMatrix1616();
	afx_msg void OnMatrix1632();
	afx_msg void OnMatrix328();
	afx_msg void OnMatrix3216();
	afx_msg void OnMatrix3232();
	afx_msg void OnHandPat88();
	afx_msg void OnHandPat816();
	afx_msg void OnHandPat832();
	afx_msg void OnHandPat168();
	afx_msg void OnHandPat1616();
	afx_msg void OnHandPat1632();
	afx_msg void OnHandPat328();
	afx_msg void OnHandPat3216();
	afx_msg void OnHandPat3232();
	afx_msg void OnPictureOpen();
	afx_msg void OnCtrlInput();
	afx_msg void OnCtrlBpnet();
	afx_msg void OnCtrlOutput();
	afx_msg void OnSampleSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // debug version in CRView.cpp
inline CCRDoc* CCRView::GetDocument()
   { return (CCRDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRVIEW_H__AD6ED2BD_9EB8_4D8D_BB50_082A4D0E3B08__INCLUDED_)
