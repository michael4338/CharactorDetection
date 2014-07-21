#if !defined(AFX_POINTMATRIXWINDOW_H__945F7B4A_397D_4FB2_8518_CD69B9080793__INCLUDED_)
#define AFX_POINTMATRIXWINDOW_H__945F7B4A_397D_4FB2_8518_CD69B9080793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointMatrixWindow.h : header file
//

#include "PointMatrix.h"

#define   DEVI_LEFT_MATRIX        52          //�������ƫ�뱾���ڵ����λ��
#define   DEVI_TOP_MATRIX         24          //�������ƫ�뱾���ڵ��ϱ�λ��

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
};//���ڴ�����Ϣ����Ԫ��


class CPointMatrixWindow : public CDialog
{
// Construction
public:
	CPointMatrixWindow(CWnd* pParent = NULL);   
    
	void  SetupRegion();                        //��ʼ���ڻ�ͼ����
	void  SetupAnimateWindow();                 //��ʼ���ڶ���Ч��

	void  ShowMatrix(void);                     //������
	HICON m_hIcon;
     
	CPointMatrix* m_pMatrix;                    //������ͼָ��
    CRect      m_pMatrixRect;                   //����λ��
	int        m_nWidth;                        //������
	int        m_nHeight;                       //����߶�

    int	m_nWindowWidth;                         //���ڿ�Ⱥ͸߶�
	int m_nWindowHeight;
	int m_nDx;                                  //���뽥���仯
	int m_nDy;
	int m_nDx1;
	int m_nDy1;
	
	UINT m_unAminateID;                          //���ڽ��뽥��ʱ���־��ʱ���С
	int  m_nAminateTime;                   
	UINT m_unWindowID;                           //���ڶ���ʱ���־��ʱ���С
    int  m_nWindowTime;                           
	UINT m_unSparkID;                            //������˸Ч����־��ʱ���С
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
