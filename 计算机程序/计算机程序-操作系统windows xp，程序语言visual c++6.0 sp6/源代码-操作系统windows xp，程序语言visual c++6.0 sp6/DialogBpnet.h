#if !defined(AFX_DIALOGBPNET_H__085A484C_431E_45D8_9BA8_4488E52319EA__INCLUDED_)
#define AFX_DIALOGBPNET_H__085A484C_431E_45D8_9BA8_4488E52319EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogBpnet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogBpnet dialog

class CDialogBpnet : public CDialog
{
// Construction
public:
	CDialogBpnet(CWnd* pParent = NULL);   // standard constructor

public:
	void DoTrain();

// Dialog Data
	//{{AFX_DATA(CDialogBpnet)
	enum { IDD = IDD_DIALOG_CTRL_BPNET };
	CString	m_strHideNodeNum;
	CString	m_strPlusRate;
	CString	m_strSubRate;
	double	m_fError;
	double	m_fMaxError;
	double	m_fRelative;
	double	m_fStep;
	CString	m_strScribe;
	//}}AFX_DATA

public:
	int m_nMethodType;                   //用户选择的算法

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBpnet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogBpnet)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonTrain();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBPNET_H__085A484C_431E_45D8_9BA8_4488E52319EA__INCLUDED_)
