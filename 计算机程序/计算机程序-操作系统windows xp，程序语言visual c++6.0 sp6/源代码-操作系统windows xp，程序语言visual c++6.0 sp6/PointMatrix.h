#if !defined(AFX_PointMatrix_H__A603DC9A_C0D3_45EE_B3E5_FBFD7434FA03__INCLUDED_)
#define AFX_PointMatrix_H__A603DC9A_C0D3_45EE_B3E5_FBFD7434FA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointMatrix.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPointMatrix window

#define      MATRIX_ROW_COL                   8                   //Ĭ��Ϊ8*8�ĵ���
#define      MATRIX_POINT_COLOR               RGB(0,0,0)          //Ĭ�ϵ����ɫ
#define      MATRIX_POINT_SIZE                20                  //Ĭ�ϵ�Ĵ�СΪ20
#define      MATRIX_BACK_COLOR                RGB(211,211,211)    //Ĭ�ϱ�����ɫ
#define      MATRIX_BORDER_WIDTH              3                   //�߿�Ŀ��
#define      DEVI_MATRIX_LEFT                 35                  //Ĭ�ϵ���ƫ�뱳������߾���
#define      DEVI_MATRIX_TOP                  35                  //Ĭ�ϵ���ƫ�뱳�����ϱ߾���

//������
class CPointMatrix : public CStatic
{
// Construction
public:
//	CPointMatrix();
    CPointMatrix(int rows = MATRIX_ROW_COL, int cols = MATRIX_ROW_COL, int PointSize = MATRIX_POINT_SIZE, COLORREF PointColor = MATRIX_POINT_COLOR, COLORREF BackColor =  MATRIX_BACK_COLOR);	
	CPointMatrix(int** PointMatrix, int rows = MATRIX_ROW_COL, int cols = MATRIX_ROW_COL, int PointSize = MATRIX_POINT_SIZE, COLORREF PointColor = MATRIX_POINT_COLOR, COLORREF BackColor =  MATRIX_BACK_COLOR);

// Attributes
protected:
	CRect     m_ClientRect;                            //���ؼ��ͻ���
	CRect     m_MatrixRect;                            //��������

    int**     m_PointMatrix;                           //��ά�����ʾ����
	int       m_nRow;                                  //���������
	int       m_nCol;                                  //���������
	int       m_PointSize;                             //ÿ����Ĵ�С
	COLORREF  m_PointColor;                            //�����ɫ
	COLORREF  m_BackColor;                             //������ɫ

	CRITICAL_SECTION cs_DrawPoint;                     //�����壬���ڿ��Ƶ��ͬ������
	
// Operations
public:
    void DrawPoint(CPoint Point, int value);            //�������εĵ�
    void DrawMatrix();                                  //��������
public:
	void SetPointMatrix(int row, int col, UINT value);  //��������ֵָ������Ԫ��
	void SetPointMatrix(int** matrix, int rows, int cols);//���ݵ����������������ָ������Ԫ��
	void SetPointMatrix(int row, int col, BOOL bChange);//��������ֵ���ֻ�ȡ������Ԫ��
    BOOL SetPointMatrix(int row, int col);              //��������ֵ��������ƶ��¼��еĵ���Ԫ��
	BOOL InitializeMatrix(int row, int col);           //��������ֵ��ʼ������
	void InitializeMatrix(int** matrix, int rows, int cols);//���ݵ��������ֵ��ʼ������
    void InitializeMatrix(int* matrix, int rows, int cols);//���ݵ��������ֵ��ʼ������
	void SetPointSize(int size){m_PointSize = size;}  //���õ�Ĵ�С
	void SetPointColor(COLORREF color){m_PointColor = color;}//���õ����ɫ
	void SetBackColor(COLORREF color){m_BackColor = color;}//���ñ�����ɫ
	int  GetMatrixRows(){return m_nRow;}//�õ����������
	int  GetMatrixCols(){return m_nCol;}//�õ����������
	int** GetPointMatrix(){return m_PointMatrix;}//�õ�����
	void ResetMatrix();//�������
	void PrintMatrix();//��ӡ��ά��������
    void RemoveNoiseForMatrix(int mValue);//����ָ����ֵȥ�������е���ɢ����


// Override
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPointMatrix)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPointMatrix();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPointMatrix)
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSetPointMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMovePointMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PointMatrix_H__A603DC9A_C0D3_45EE_B3E5_FBFD7434FA03__INCLUDED_)
