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
	void ShowCorrectMatrix(int n);         //�����ȷ�ĵ���
	void DrawAnalysisChart(double* dataarray, int arraysize, UINT time);//����������
	void DrawArrow(CPoint start, CPoint end, int penwidth, COLORREF pencolor);//����ͷ
    void AddResultToList(int Res, CString sample, int type, int width, int height, int sampleID);
    void CalculateResult();//����ʶ���ʺ�ƽ������ʱ��
	void SetErrResult(double* pErr, int nErr, UINT unTime){m_nErrResult = nErr; memcpy(m_pErrResult,pErr,nErr*sizeof(double)); m_unTime = unTime;}//����������
    void SetShowResult(ResetDisplayMessageStruct* display);//������ʾ���
	void DoAddSample(double** data);//�������
    void ResetSampleIdInList(int len);//���������б��е��������

protected:
	CPointMatrix* m_pMatrix;                    //������ͼָ��
    CRect      m_MatrixRect;                    //����λ��
	int        m_nWidth;                        //������
	int        m_nHeight;                       //����߶�

	int        m_nErrResult;                    //����������
	double     m_pErrResult[15000];             //�����
	UINT       m_unTime;                        //����ʱ��

	CFont      m_fnt;                           //�б�ؼ�����
	int        m_nCount;                        //��¼����


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
