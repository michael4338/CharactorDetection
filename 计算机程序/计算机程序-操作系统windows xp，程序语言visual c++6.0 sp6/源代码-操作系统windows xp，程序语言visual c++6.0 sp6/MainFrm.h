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

#define  IDD_BOTTOM_COOLBAR     4444                //�ײ�����Rebar��ID
#define  IDD_BOTTOM_TABCTRL     5555                //�ײ�����TAB��ID

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
	CToolBar    m_wndMyToolBar;            //�Զ��幤����
	CReBar      m_wndReBar;                //�Զ���ReBar����Զ��幤����
    CCoolBar    m_wndBottomRebar;          //�Զ���Rebar��ŵײ����ƶԻ���
	CDialogInput m_CtrlInDlg;              //�ײ�������ƶԻ���
	CDialogBpnet m_CtrlBpDlg;              //�ײ��㷨���ƶԻ���
	CDialogOutput m_CtrlOutDlg;            //�ײ�������ƶԻ���
	CCRView     m_wndView;                 //��ͼ����
	CCoolTabCtrl m_TabCtrl;                //��ǩ�ؼ�

public:
	CDialogOutput* GetDialogOut(){return &m_CtrlOutDlg;}//�õ�������ƶԻ����ָ��
    CDialogInput* GetDialogIn(){return &m_CtrlInDlg;}//�õ�������ƶԻ����ָ��
    CDialogBpnet* GetDialogBp(){return &m_CtrlBpDlg;}//�õ��㷨���ƶԻ����ָ��
	//���õ�ǰ���ư�����ҳ
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
