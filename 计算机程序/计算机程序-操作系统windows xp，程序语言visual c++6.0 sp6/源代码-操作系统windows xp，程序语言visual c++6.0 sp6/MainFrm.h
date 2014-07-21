// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__7BBA98CA_6FCC_4626_97DB_20D63B87A688__INCLUDED_)
#define AFX_MAINFRM_H__7BBA98CA_6FCC_4626_97DB_20D63B87A688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointMatrixWindow.h"
#include "DialogInput.h"
#include "DialogBpnet.h"
#include "DialogOutput.h"
#include "CRView.h"
#include "CoolTabCtrl.h"

#define  IDD_BOTTOM_COOLBAR     4444                //底部控制Rebar的ID
#define  IDD_BOTTOM_TABCTRL     5555                //底部控制TAB的ID

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void CreateBottomCtrlBar();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

protected:
	CToolBar    m_wndMyToolBar;            //自定义工具栏
	CReBar      m_wndReBar;                //自定义ReBar存放自定义工具栏
    CCoolBar    m_wndBottomRebar;          //自定义Rebar存放底部控制对话框
	CDialogInput m_CtrlInDlg;              //底部输入控制对话框
	CDialogBpnet m_CtrlBpDlg;              //底部算法控制对话框
	CDialogOutput m_CtrlOutDlg;            //底部输出控制对话框
	CCRView     m_wndView;                 //视图对象
	CCoolTabCtrl m_TabCtrl;                //标签控件

public:
	CDialogOutput* GetDialogOut(){return &m_CtrlOutDlg;}//得到输出控制对话框的指针
    CDialogInput* GetDialogIn(){return &m_CtrlInDlg;}//得到输入控制对话框的指针
    CDialogBpnet* GetDialogBp(){return &m_CtrlBpDlg;}//得到算法控制对话框的指针
	//设置当前控制板属性页
	void  SetActiveCtrlPat(int n)
	{
		if(n>=0 && n<=2)
			m_TabCtrl.SetActivePage(n);
	}

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCtrlPat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__7BBA98CA_6FCC_4626_97DB_20D63B87A688__INCLUDED_)
