// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CR.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_INPUT_CTRLPAT, OnCtrlPat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
 
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);

		
	//添加的代码
    CImageList  m_ilToolBar;
    CBitmap bm;

    bm.LoadBitmap(IDB_BITMAP_TOOLBAR);
    m_ilToolBar.Create(16,16,ILC_COLOR24,0,4);
    m_ilToolBar.Add(&bm, RGB(0, 0, 0));
    //为工具栏加载位图
    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ilToolBar);
    m_ilToolBar.Detach();
    bm.DeleteObject();
	
//    bm.LoadBitmap(IDB_BITMAP_TOOLBAR_COLD);
//    m_ilToolBar.Create(16,16,ILC_COLOR24,0,3);
//    m_ilToolBar.Add(&bm, RGB(0, 0, 0));
//    //为工具栏加载位图
//    m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ilToolBar);
//    m_ilToolBar.Detach();
//    bm.DeleteObject();

	
	CImageList img;
	CString str;

	if (!m_wndReBar.Create(this))
	{
		AfxMessageBox("Error in create rebar");
		return -1;      // fail to create
	}
	if (!m_wndMyToolBar.CreateEx(this))
	{
		AfxMessageBox("Error in create toolbar");
		return -1;      // fail to create
	}

// 	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//  EnableDocking(CBRS_ALIGN_ANY);
//  DockControlBar(&m_wndToolBar);
//  	 
//  m_wndMyToolBar.EnableDocking(CBRS_ALIGN_ANY);
//  EnableDocking(CBRS_ALIGN_ANY);
//  DockControlBar(&m_wndMyToolBar);

	//设置工具栏属性
	m_wndMyToolBar.GetToolBarCtrl().SetButtonWidth(20, 180);
	m_wndMyToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);


	img.Create(IDB_BITMAP2, 22, 0, RGB(255, 0, 255));
	m_wndMyToolBar.GetToolBarCtrl().SetHotImageList(&img);
	img.Detach();

	img.Create(IDB_BITMAP1, 22, 0, RGB(255, 0, 255));
	m_wndMyToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();

	m_wndMyToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_wndMyToolBar.SetButtons(NULL, 10);

	//设置每个工具栏按钮
	m_wndMyToolBar.SetButtonInfo(0, ID_INPUT_MATRIX, TBSTYLE_BUTTON, 0);
	str.LoadString(ID_INPUT_MATRIX);
	m_wndMyToolBar.SetButtonText(0, str);

	m_wndMyToolBar.SetButtonInfo(1, ID_INPUT_HANDPAT, TBSTYLE_BUTTON, 1);
	str.LoadString(ID_INPUT_HANDPAT);
	m_wndMyToolBar.SetButtonText(1, str);

	m_wndMyToolBar.SetButtonInfo(2, ID_INPUT_PICTURE, TBSTYLE_BUTTON, 2);
	str.LoadString(ID_INPUT_PICTURE);
	m_wndMyToolBar.SetButtonText(2, str);

	m_wndMyToolBar.SetButtonInfo(3, ID_INPUT_CTRLPAT, TBSTYLE_BUTTON, 3);
	str.LoadString(ID_INPUT_CTRLPAT);
	m_wndMyToolBar.SetButtonText(3, str);

	m_wndMyToolBar.SetButtonInfo(4, ID_SHOWRESULT, TBSTYLE_BUTTON, 4);
	str.LoadString(ID_SHOWRESULT);
	m_wndMyToolBar.SetButtonText(4, str);

	m_wndMyToolBar.SetButtonInfo(5, ID_CTRL_INPUT, TBSTYLE_BUTTON, 5);
	str.LoadString(ID_CTRL_INPUT);
	m_wndMyToolBar.SetButtonText(5, str);

	m_wndMyToolBar.SetButtonInfo(6, ID_CTRL_BPNET, TBSTYLE_BUTTON, 6);
	str.LoadString(ID_CTRL_BPNET);
	m_wndMyToolBar.SetButtonText(6, str);

	m_wndMyToolBar.SetButtonInfo(7, ID_CTRL_OUTPUT, TBSTYLE_BUTTON, 7);
	str.LoadString(ID_CTRL_OUTPUT);
	m_wndMyToolBar.SetButtonText(7, str);

	m_wndMyToolBar.SetButtonInfo(8, ID_TRAIN, TBSTYLE_BUTTON, 8);
	str.LoadString(ID_TRAIN);
	m_wndMyToolBar.SetButtonText(8, str);

	m_wndMyToolBar.SetButtonInfo(9, ID_RECGNIZE, TBSTYLE_BUTTON, 9);
	str.LoadString(ID_RECGNIZE);
	m_wndMyToolBar.SetButtonText(9, str);
	
	//设置工具栏的尺寸及属性
	CRect rectToolBar;
	m_wndToolBar.GetItemRect(0, &rectToolBar);

	CRect rectMyToolBar;
	m_wndMyToolBar.GetItemRect(0, &rectMyToolBar);
	m_wndMyToolBar.SetSizes(rectMyToolBar.Size(), CSize(30,20));
	m_wndMyToolBar.SetBarStyle(m_wndMyToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	//在Rebar中添加工具条
	m_wndReBar.AddBar(&m_wndToolBar);
	m_wndReBar.AddBar(&m_wndMyToolBar, NULL, NULL, RBBS_FIXEDBMP | RBBS_BREAK);

	// 设定Rebar中的每个尺寸
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;
	rbbi.cxMinChild = 0;
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cxIdeal = 200;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;
	rbbi.cxMinChild = rectMyToolBar.Width();
	rbbi.cyMinChild = rectMyToolBar.Height();
	rbbi.cx = rbbi.cxIdeal = rectMyToolBar.Width() * 4;
 	m_wndReBar.GetReBarCtrl().SetBandInfo(1, &rbbi);

	//创建底部控制栏
	CreateBottomCtrlBar();

	m_wndToolBar.ShowWindow(SW_HIDE);

	return 0;
}


void CMainFrame::CreateBottomCtrlBar()
{
	if(m_wndBottomRebar)
	{
		m_wndBottomRebar.ShowAgain();
		return;
	}
    if (!m_wndBottomRebar.Create(_T("控制栏"),this,CSize(600,190),TRUE,IDD_BOTTOM_COOLBAR)) 
	{
		AfxMessageBox("无法创建底部工具栏");
		return ;
	}
	//创建标签控件
    m_TabCtrl.Create(TCS_DOWN|WS_CHILD|WS_VISIBLE,CRect(0,0,100,100),&m_wndBottomRebar,IDD_BOTTOM_TABCTRL);		

	//在标签控件中添加底部控制对话框	
    m_CtrlInDlg.Create(IDD_DIALOG_CTRL_INPUT, &m_TabCtrl);
	m_CtrlBpDlg.Create(IDD_DIALOG_CTRL_BPNET, &m_TabCtrl);
	m_CtrlOutDlg.Create(IDD_DIALOG_CTRL_OUTPUT, &m_TabCtrl);
	m_TabCtrl.AddPage(&m_CtrlInDlg,"输入控制",IDI_ICON_INPUT);
	m_TabCtrl.AddPage(&m_CtrlBpDlg,"算法控制",IDI_ICON_BPNET);
	m_TabCtrl.AddPage(&m_CtrlOutDlg,"输出控制",IDI_ICON_OUTPUT);
    m_CtrlInDlg.ShowWindow(SW_SHOW);
    m_CtrlInDlg.ShowWindow(SW_SHOW);
    m_CtrlInDlg.ShowWindow(SW_SHOW);
	m_TabCtrl.UpdateWindow();
	m_TabCtrl.SetActivePage(0);
	
	//设置控制栏的属性
	m_wndBottomRebar.SetBarStyle(m_wndBottomRebar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC); 
    m_wndBottomRebar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndBottomRebar, AFX_IDW_DOCKBAR_BOTTOM);///停靠在下边

}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszName ="基于BP网络的字符识别";
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

//使窗口起始便最大化
void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow = SW_SHOWMAXIMIZED;
	CFrameWnd::ActivateFrame(nCmdShow);
}



void CMainFrame::OnCtrlPat() 
{
	// TODO: Add your command handler code here
	CreateBottomCtrlBar();
}


