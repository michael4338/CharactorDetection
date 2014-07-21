// DialogBpnet.cpp : implementation file
//

#include "stdafx.h"
#include "CR.h"
#include "DialogBpnet.h"

#include "CRView.h"

//再次声明全局变量
extern GLOBAL_STRUCTURE Global;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBpnet dialog


CDialogBpnet::CDialogBpnet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBpnet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogBpnet)
	m_strHideNodeNum = _T("");
	m_strPlusRate = _T("");
	m_strSubRate = _T("");
	m_fError = 0.0;
	m_fMaxError = 0.0;
	m_fRelative = 0.0;
	m_fStep = 0.0;
	m_strScribe = _T("");
	//}}AFX_DATA_INIT
}


void CDialogBpnet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBpnet)
	DDX_CBString(pDX, IDC_COMBO_HIDE, m_strHideNodeNum);
	DDV_MaxChars(pDX, m_strHideNodeNum, 2);
	DDX_CBString(pDX, IDC_COMBO_PLUSRATE, m_strPlusRate);
	DDV_MaxChars(pDX, m_strPlusRate, 10);
	DDX_CBString(pDX, IDC_COMBO_SUBSRATE, m_strSubRate);
	DDV_MaxChars(pDX, m_strSubRate, 10);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_fError);
	DDV_MinMaxDouble(pDX, m_fError, 1.e-004, 2.e-003);
	DDX_Text(pDX, IDC_EDIT_MAXERROR, m_fMaxError);
	DDV_MinMaxDouble(pDX, m_fMaxError, 2.e-002, 5.e-002);
	DDX_Text(pDX, IDC_EDIT_RELATIVE, m_fRelative);
	DDV_MinMaxDouble(pDX, m_fRelative, 0.1, 0.9);
	DDX_Text(pDX, IDC_EDIT_STEP, m_fStep);
	DDV_MinMaxDouble(pDX, m_fStep, 1.e-002, 2.e-002);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_strScribe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBpnet, CDialog)
	//{{AFX_MSG_MAP(CDialogBpnet)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, OnButtonTrain)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBpnet message handlers

BOOL CDialogBpnet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
    //初始化
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(BST_CHECKED);
	OnRadio3();
	m_strHideNodeNum = "10";
    m_strPlusRate = "1.05";
	m_strSubRate = "0.75";
	m_fError = 0.0008;
	m_fMaxError = 0.04;
	m_fRelative = 0.5;
	m_fStep = 0.015;
    m_strScribe = "学习速率在网络的整个学习过程中可根据误差曲面上不同区域的曲率变化自适应调节，从而更大程度上加快网络的收敛。本系统推荐使用。";

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDialogBpnet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	   
	if(nCtlColor == CTLCOLOR_STATIC) 
	{ 
		long lStyle = GetWindowLong(pWnd->m_hWnd, GWL_STYLE); 
        if(lStyle & BS_GROUPBOX)
			pDC->SetTextColor(RGB(151,0,151)); 
	} 

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
	
void CDialogBpnet::DoTrain()
{
	OnButtonTrain();
}

void CDialogBpnet::OnButtonTrain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
		m_nMethodType = 1;
	else if(((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())
		m_nMethodType = 2;
	else
		m_nMethodType = 3;
	if(Global.m_pgCRView != NULL)
	{
		Global.m_pgCRView->BpTrain(m_nMethodType,m_fRelative,m_fError,atoi(m_strHideNodeNum),m_fStep,atof(m_strPlusRate),atof(m_strSubRate),m_fMaxError);			
	}
}

void CDialogBpnet::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_RELATIVE)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMBO_PLUSRATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SUBSRATE)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_MAXERROR)->EnableWindow(FALSE);
	m_strScribe = "基本神经网络算法，应用最为广泛的神经网络学习算法，结构简单，状态稳定，通用性强。缺点是学习收敛速度慢，容易陷入局部最小。";
    UpdateData(FALSE);
}

void CDialogBpnet::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_RELATIVE)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_PLUSRATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SUBSRATE)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_MAXERROR)->EnableWindow(FALSE);	
	m_strScribe = "针对BP网络的缺陷，引入动量因子，在维持算法稳定的前提下减少振荡，使用更高的学习速率，加速收敛。";
    UpdateData(FALSE);
}

void CDialogBpnet::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_RELATIVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_PLUSRATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_SUBSRATE)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_MAXERROR)->EnableWindow(TRUE);	
    m_strScribe = "学习速率在网络的整个学习过程中可根据误差曲面上不同区域的曲率变化自适应调节，从而更大程度上加快网络的收敛。本系统推荐使用。";
    UpdateData(FALSE);
}

BOOL CDialogBpnet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}
