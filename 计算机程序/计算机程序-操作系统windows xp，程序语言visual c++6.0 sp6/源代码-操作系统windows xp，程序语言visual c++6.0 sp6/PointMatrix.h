#if !defined(AFX_PointMatrix_H__A603DC9A_C0D3_45EE_B3E5_FBFD7434FA03__INCLUDED_)
#define AFX_PointMatrix_H__A603DC9A_C0D3_45EE_B3E5_FBFD7434FA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointMatrix.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPointMatrix window

#define      MATRIX_ROW_COL                   8                   //默认为8*8的点阵
#define      MATRIX_POINT_COLOR               RGB(0,0,0)          //默认点的颜色
#define      MATRIX_POINT_SIZE                20                  //默认点的大小为20
#define      MATRIX_BACK_COLOR                RGB(211,211,211)    //默认背景颜色
#define      MATRIX_BORDER_WIDTH              3                   //边框的宽度
#define      DEVI_MATRIX_LEFT                 35                  //默认点阵偏离背景的左边距离
#define      DEVI_MATRIX_TOP                  35                  //默认点阵偏离背景的上边距离

//点阵类
class CPointMatrix : public CStatic
{
// Construction
public:
//	CPointMatrix();
    CPointMatrix(int rows = MATRIX_ROW_COL, int cols = MATRIX_ROW_COL, int PointSize = MATRIX_POINT_SIZE, COLORREF PointColor = MATRIX_POINT_COLOR, COLORREF BackColor =  MATRIX_BACK_COLOR);	
	CPointMatrix(int** PointMatrix, int rows = MATRIX_ROW_COL, int cols = MATRIX_ROW_COL, int PointSize = MATRIX_POINT_SIZE, COLORREF PointColor = MATRIX_POINT_COLOR, COLORREF BackColor =  MATRIX_BACK_COLOR);

// Attributes
protected:
	CRect     m_ClientRect;                            //本控件客户区
	CRect     m_MatrixRect;                            //点阵区域

    int**     m_PointMatrix;                           //二维数组表示点阵
	int       m_nRow;                                  //点阵的行数
	int       m_nCol;                                  //点阵的列数
	int       m_PointSize;                             //每个点的大小
	COLORREF  m_PointColor;                            //点的颜色
	COLORREF  m_BackColor;                             //背景颜色

	CRITICAL_SECTION cs_DrawPoint;                     //互斥体，用于控制点的同步操作
	
// Operations
public:
    void DrawPoint(CPoint Point, int value);            //画出方形的点
    void DrawMatrix();                                  //画出点阵
public:
	void SetPointMatrix(int row, int col, UINT value);  //根据行列值指定点阵元素
	void SetPointMatrix(int** matrix, int rows, int cols);//根据点阵和总行数总列数指定点阵元素
	void SetPointMatrix(int row, int col, BOOL bChange);//根据行列值保持或取反点阵元素
    BOOL SetPointMatrix(int row, int col);              //根据行列值设置鼠标移动事件中的点阵元素
	BOOL InitializeMatrix(int row, int col);           //根据行列值初始化点阵
	void InitializeMatrix(int** matrix, int rows, int cols);//根据点阵和行列值初始化点阵
    void InitializeMatrix(int* matrix, int rows, int cols);//根据点阵和行列值初始化点阵
	void SetPointSize(int size){m_PointSize = size;}  //设置点的大小
	void SetPointColor(COLORREF color){m_PointColor = color;}//设置点的颜色
	void SetBackColor(COLORREF color){m_BackColor = color;}//设置背景颜色
	int  GetMatrixRows(){return m_nRow;}//得到数组的行数
	int  GetMatrixCols(){return m_nCol;}//得到数组的列数
	int** GetPointMatrix(){return m_PointMatrix;}//得到点阵
	void ResetMatrix();//重设点阵
	void PrintMatrix();//打印二维点阵数组
    void RemoveNoiseForMatrix(int mValue);//根据指定阈值去除点阵中的离散噪声


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
