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

enum  ViewType{NONE,MATRIX,HANDWRITINGPAT,PICTURE,MULTIMATRIX}; //��ͼ��������ʾ�Ķ�������


//����ȫ�ֱ���
typedef struct _GLOBAL_STRUCTURE
{
	CCRView* m_pgCRView;     //��ǰ��ͼ����ָ��
	char m_szOutput[20];     //�����������ַ�
	_GLOBAL_STRUCTURE(){m_pgCRView = NULL;memset(m_szOutput,0,20);strcpy(m_szOutput,"ABCUNIXZ");}
}GLOBAL_STRUCTURE, *PGLOBALSTRUCTURE;




/*�����༰����ر���///////////////////////////////////////////////////////////////*/

#define                    SAMPLE_FILE_NAME                 "sample.dat"

class SAMPLELIST; 


class SAMPLE
{
	friend class SAMPLELIST;                    //������Ԫ��
	
public:

	//�������Ա����
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
	SAMPLE* NextSample() {return link;}    //��ǰ�ڵ����һ���ڵ�

	//�ڵ�ǰ�ڵ�����һ���½ڵ�
	void InsertAfterSelf(SAMPLE* sample)
	{
		sample->link = link;
	    link = sample;
	}
          
	//ɾ����ǰ�ڵ����һ���ڵ�
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

	int m_Width;         //�������
	int m_Height;        //�����߶�
	int m_Type;          //�������ͣ��ֱ��ΪͼƬ����д�壬����
	int m_Samvalue;      //����ֵ
	double m_Samdata[32][32];//�����Ķ�ά����
};

//CONNECTION������
class SAMPLELIST
{
public:

	//�������Ա����
	SAMPLELIST(){Last = First = new SAMPLE();}
	~SAMPLELIST(){MakeEmpty();}
	//��������Ϊ�ձ�

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
	//��������ĳ���
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
	//����λ��Ѱ��ָ���ڵ�
	SAMPLE* FindByPos(int pos)
	{
		SAMPLE *temp = First->link;
	    while(temp != NULL && !(pos--))
			temp = temp->link;
	    return temp;
	}
	//���½ڵ�ӵ�������
	void AppendList(SAMPLE *add)
	{	
		Last->link = add;
	    add->link = NULL;
	    Last = add;
	}
	//��ָ���ڵ�ɾ��
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
	//��ָ��λ�ýڵ�ɾ��
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
	//���ļ��еõ�����
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
	//������д���ļ�
	void WriteSampleToFile(char* filename)
	{
		//��ΪView����������Ҫִ�����Σ�������ֹ�ڶ���ִ��
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
 	//�����������ļ�
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
	//�������еõ���ά����ָ��
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
			AfxMessageBox("�ڴ治��");
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

/*�����༰����ر�������/////////////////////////////////////////////////////////*/



/*��д���༰����ر���///////////////////////////////////////////////////////////*/

#define      HANDPAT_POINT_COLOR               RGB(255,0,0)       //Ĭ�ϵ����ɫ
#define      HANDPAT_PEN_WIDTH                 3                  //Ĭ�ϱʿ��
#define      HANDPAT_BACK_COLOR                RGB(211,211,211)   //Ĭ�ϱ�����ɫ

#define      HANDPAT_BORDER_WIDTH              2                  //��д��߿���
#define      HANDPAT_BORDER_COLOR_OUT_LEFT     RGB(255,255,255)   //��д������߿���ɫ
#define      HANDPAT_BORDER_COLOR_IN_LEFT      RGB(192,192,192)   //��д������߿���ɫ
#define      HANDPAT_BORDER_COLOR_OUT_RIGHT    RGB(109,109,109)   //��д�����ұ߿���ɫ
#define      HANDPAT_BORDER_COLOR_IN_RIGHT     RGB(147,147,147)   //��д�����ұ߿���ɫ

#define      MULTI_MATRIX_SIZE                 50                 //��������Ĵ�С

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
	CRect     m_ClientRect;                            //���ؼ��ͻ���
	CRect     m_HandPatRect;                           //��д������

	int       m_nPenWidth;                             //�ʿ��
	COLORREF  m_PointColor;                            //�����ɫ
	COLORREF  m_BackColor;                             //������ɫ

public:
	CArray<CPoint,CPoint> m_PointArray;                //�������
	CTypedPtrList<CObList,CStroke*>m_StrokeList;       //������������

public:
	
	void SetPenWidth(int width){m_nPenWidth = width;}  //���õ�Ĵ�С
	void SetPointColor(COLORREF color){m_PointColor = color;}//���õ����ɫ
	void SetBackColor(COLORREF color){m_BackColor = color;}//���ñ�����ɫ
	COLORREF GetBackColor(){return m_BackColor;}       //�õ�������ɫ 
	COLORREF GetPointColor(){return m_PointColor;}     //�õ������ɫ
	int GetPenWidth(){return m_nPenWidth;}             //�õ����ʿ��
	void ResetPointArray(){m_StrokeList.RemoveAll();}  //��������ϵĵ�
	

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
	CPointMatrix*  m_pMatrix;           //��ͼ���еĵ���
	CRect       m_MatrixRect;           //����������λ��
	int         m_nWidth;               //����Ŀ��
	int         m_nHeight;              //����ĸ߶�
	BOOL        m_bLButtonDown;         //�ж��������Ƿ��Ѱ���
//	HDIB        m_hMatrixDIB;           //����Ҫת�����ɵ�DIB���
	
    CHandWritingPat* m_pHandWritingPat; //��ͼ���е���д��
	CRect       m_HandWritingRect;      //��д��������λ��
	int         m_nHandWritingWidth;    //��д��Ŀ��
	int         m_nHandWritingHeight;   //��д��ĸ߶�
	CPoint      m_ptPrev;               //��һ����
	CStroke*    m_pStrokeCur;           //��ǰ��
//	HDIB        m_hHandPatDIB;          //��д��Ҫת�����ɵ�DIB���

	HDIB        m_hPictureDIB;          //ͼƬҪת�����ɵ�DIB

    CPointMatrix* m_pMultiMatrixs[MULTI_MATRIX_SIZE];//һ�������ָ������
	int         m_nMutliMatrixsNum;     //����ָ������Ԫ�ظ���

    ViewType    m_nCurType;             //��ǰ��ͼ����ʾ�Ķ�������

	ViewType    m_nSampleCurType;       //��ǰ��������
	ViewType    m_nSampleLastType;      //��¼�ϴζ������ͣ�����ѵ����ʶ���ͬ��

	double**    m_data;                 //Ԥ�������������飬�����㷨����ʾ������
	int         m_datalen;              //����ĳ���
	int         m_nDataWidth;
	int         m_nDataHeight;          //Ԥ�����ĵ����Ⱥ͸߶�

	CPointMatrixWindow* m_pDisplayWindow;//�ұ���ʾ����

	SAMPLELIST  m_SampleList;            //�����������

// Operations
public:
    UINT  GetCurWinID(CPoint point);                     //���ݵ�ǰ���λ�õõ�����ID
    void  ShowMatrix(int nRow, int nCol);                //��ʾ��ߵĵ���
	int   GetMatrixWidth(){return m_nWidth;}             //�õ�����Ŀ��
	int   GetMatrixHeight(){return m_nHeight;}           //�õ�����ĸ߶�
	void  SetMatrixWidth(int nWidth);                    //���õ���Ŀ��
	void  SetMatrixHeight(int nHeight);                  //���õ���ĸ߶�
	void  ShowHandWritingPat(void);                      //��ʾ��д��
	void  DeleteItems(ViewType survive);                 //ɾ����ͼ�ϵ�Ԫ��
	void  SetHandPatWidth(int nWidth);                   //������д��Ŀ��
	void  SetHandPatHeight(int nHeight);                 //������д��ĸ߶�
  
public:
	CPen* CreatePen(CDC* pDC);                           //��������
	void  CreateBckGround(CDC* pDC);                     //��������
	void  CreateHandPatBorder(CDC *pDC);                 //������д��ı߿�
    void  DrawLine(CDC* pDC, CPoint start, CPoint end, int width, COLORREF color);//����
    void  BrushRect(CDC* pDC, CRect rect, COLORREF color);//�������
	
public:
	void OnPreProcess(int DesireWidth, int DesireHeight); //Ԥ����������
	void ShowPicture();                                   //��ͼƬ
    void FileReLoadBmp();                                 //���´�ͼƬ�ļ�
	double** PreProcess(int Width,int Height,HDIB hDIB);  //��ͼ���ļ���һ���Թ�һ������
	HDIB CreateHDIBByDbArray(BYTE* pointarray, int Width, int Height);//���ݶ�ά���鴴��DIB���
    HDIB CreateHDIBByScreen(CRect* ScreenRect);           //������Ļλ�ô���DIB���
    HDIB CreateHDIBByPicture(CString strPathFileName);    //����ͼ���ļ�����DIB���

	void ShowPicInMatrix(HDIB hDIB);                      //����ǰDIB�е����������������ȥ
	void ResultShow(BOOL);                                //��ʾ������Ԥ������

public:
	void ShowMultiMatrixs(ResetDisplayMessageStruct* display);//��ʾ�������
	void HideMultiMatrixs();                              //���ض������

public:
	CHandWritingPat* GetHandPatPointer(){return m_pHandWritingPat;}
	CPointMatrix* GetMatrixPointer(){return m_pMatrix;}
    SAMPLELIST* GetSampleList(){return &m_SampleList;}

public:
	void BpTrain(int combp, double momentum, double min_ex, int n_hidden, double eta, double addrate, double subrate, double maxerror);//ѵ��
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
	afx_msg BOOL OnIMGPRC256ToGray();          //ͼ��Ԥ�����1������256ɫͼ��ת��Ϊ�Ҷ�ͼ��
	afx_msg BOOL OnIMGPRCGrayToWhiteBlack();   //ͼ��Ԥ�����2�������Ҷ�ͼ��ֵ��
	afx_msg BOOL OnImgprcSharp();              //ͼ��Ԥ�����3�����ݶ���	
    afx_msg BOOL OnImgprcRemoveNoise();        //ͼ��Ԥ�����4����ȥ��ɢ�ӵ�����
    afx_msg BOOL OnImgprcAdjustSlope();        //ͼ��Ԥ�����5������б�ȵ���
    afx_msg BOOL OnImgprcDivide();             //ͼ��Ԥ�����6�����ָ���ڷָ�������ַ����滭���Ա�ʶ
	afx_msg BOOL OnImgprcStandarize();         //ͼ��Ԥ�����7������׼��һ�����ָ�����ĸ�����ͬ���ߵ������ַ�����ͳһ
    afx_msg BOOL OnImgprcShrinkAlign();        //ͼ��Ԥ�����8�������������Ѿ��ָ���ϵ������ַ������γ��µ�λͼ���
    afx_msg void OnBpnetTrain();               //ѵ��������
	afx_msg void OnBpnetRecognize();           //������ʶ��
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
