#if !defined(AFX_DIALOGOUTPUT_H__B7886007_4BCF_45DB_9D73_F368565CD8CF__INCLUDED_)
#define AFX_DIALOGOUTPUT_H__B7886007_4BCF_45DB_9D73_F368565CD8CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogOutput.h : header file
//
#include "PointMatrixWindow.h"
#include "PointMatrix.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogOutput dialog

class CDialogOutput : public CDialog
{
// Construction
public:
	CDialogOutput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogOutput)
	enum { IDD = IDD_DIALOG_CTRL_OUTPUT };
	CListCtrl	m_ListResult;
	CString	m_strSampleValue;
	//}}AFX_DATA

public:
	void ShowCorrectMatrix(int n);         //输出正确的点阵
	void DrawAnalysisChart(double* dataarray, int arraysize, UINT time);//画误差分析表
	void DrawArrow(CPoint start, CPoint end, int penwidth, COLORREF pencolor);//画箭头
    void AddResultToList(int Res, CString sample, int type, int width, int height, int sampleID);
    void CalculateResult();//计算识别率和平均计算时间
	void SetErrResult(double* pErr, int nErr, UINT unTime){m_nErrResult = nErr; memcpy(m_pErrResult,pErr,nErr*sizeof(double)); m_unTime = unTime;}//设置误差参数
    void SetShowResult(ResetDisplayMessageStruct* display);//设置显示结果
	void DoAddSample(double** data);//添加样本
    void ResetSampleIdInList(int len);//重新设置列表中的样本序号

protected:
	CPointMatrix* m_pMatrix;                    //点阵视图指针
    CRect      m_MatrixRect;                    //点阵位置
	int        m_nWidth;                        //点阵宽度
	int        m_nHeight;                       //点阵高度

	int        m_nErrResult;                    //误差迭代次数
	double     m_pErrResult[15000];             //误差结果
	UINT       m_unTime;                        //计算时间

	CFont      m_fnt;                           //列表控件字体
	int        m_nCount;                        //记录数量


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogOutput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogOutput)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnCheckSample();
	afx_msg void OnButtonSample();
	afx_msg void OnButtonSampleSub();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGOUTPUT_H__B7886007_4BCF_45DB_9D73_F368565CD8CF__INCLUDED_)
