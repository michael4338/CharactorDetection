#if !defined(AFX_POINTMATRIXWINDOW_H__945F7B4A_397D_4FB2_8518_CD69B9080793__INCLUDED_)
#define AFX_POINTMATRIXWINDOW_H__945F7B4A_397D_4FB2_8518_CD69B9080793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointMatrixWindow.h : header file
//

#include "PointMatrix.h"

#define   DEVI_LEFT_MATRIX        52          //定义点阵偏离本窗口的左边位置
#define   DEVI_TOP_MATRIX         24          //定义点阵偏离本窗口的上边位置

/////////////////////////////////////////////////////////////////////////////
// CPointMatrixWindow dialog

struct ResetDisplayMessageStruct
{
	double** data;
	int *resdata;
	int num;
	int width;
	int height;
	int nPreprocess;
};//用于传递消息的三元组


class CPointMatrixWindow : public CDialog
{
// Construction
public:
	CPointMatrixWindow(CWnd* pParent = NULL);   
    
	void  SetupRegion();                        //起始窗口绘图操作
	void  SetupAnimateWindow();                 //起始窗口动画效果

	void  ShowMatrix(void);                     //画点阵
	HICON m_hIcon;
     
	CPointMatrix* m_pMatrix;                    //点阵视图指针
    CRect      m_pMatrixRect;                   //点阵位置
	int        m_nWidth;                        //点阵宽度
	int        m_nHeight;                       //点阵高度

    int	m_nWindowWidth;                         //窗口宽度和高度
	int m_nWindowHeight;
	int m_nDx;                                  //渐入渐出变化
	int m_nDy;
	int m_nDx1;
	int m_nDy1;
	
	UINT m_unAminateID;                          //窗口渐入渐出时间标志和时间大小
	int  m_nAminateTime;                   
	UINT m_unWindowID;                           //窗口动作时间标志和时间大小
    int  m_nWindowTime;                           
	UINT m_unSparkID;                            //灯箱闪烁效果标志和时间大小
	int  m_nSparkTime;

// Dialog Data
	//{{AFX_DATA(CPointMatrixWindow)
	enum { IDD = IDD_DIALOG_POINTMATRIX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPointMatrixWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPointMatrixWindow)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnNcHitTest(CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnResetDisplay(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POINTMATRIXWINDOW_H__945F7B4A_397D_4FB2_8518_CD69B9080793__INCLUDED_)
