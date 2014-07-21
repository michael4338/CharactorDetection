#if !defined(AFX_DialogInput_H__78B08C55_AB00_42D3_A292_F6E4DB3B8B45__INCLUDED_)
#define AFX_DialogInput_H__78B08C55_AB00_42D3_A292_F6E4DB3B8B45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInput dialog

enum SetInputPara{
	MATRIX_SET_POINTSIZE,MATRIX_SET_POINTCOLOR,MATRIX_SET_BACKCOLOR,              //点阵参数
	HANDPAT_SET_PENWIDTH,HANDPAT_SET_BACKCOLOR,HANDPAT_SET_COLOR                  //手写板参数
};

class CDialogInput : public CDialog
{
// Construction
public:
	CDialogInput(CWnd* pParent = NULL);   // standard constructor

public:
	COLORREF m_PreColor;
	BOOL     m_bErazor;

public:
	void     DoPreprocess();

// Dialog Data
	//{{AFX_DATA(CDialogInput)
	enum { IDD = IDD_DIALOG_CTRL };
		// NOTE: the ClassWizard will add data members here
	CComboBox	m_ComboPointSize;
	CComboBox	m_ComboRows;
	CComboBox	m_ComboCols;
	CSliderCtrl m_SliderWidth;
	CSliderCtrl m_SliderHeight;
	CSliderCtrl m_SliderWidthPat;
	CSliderCtrl m_SliderHeightPat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogInput)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogInput)
		// NOTE: the ClassWizard will add member functions here
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonColor();
	afx_msg void OnButtonBakColor();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonColorPat();
	afx_msg void OnButtonBakColorPat();
	afx_msg void OnButtonResetPat();
	afx_msg void OnSelChangePointSize();
	afx_msg void OnSelChangeRows();
	afx_msg void OnSelChangeCols();
	afx_msg void OnReleasedcaptureSliderWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderHeight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderWidthPat(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderHeightPat(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnButtonPrePro();
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DialogInput_H__78B08C55_AB00_42D3_A292_F6E4DB3B8B45__INCLUDED_)
