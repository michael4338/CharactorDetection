// DialogInput.cpp : implementation file
//

#include "stdafx.h"
#include "CR.h"
#include "DialogInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "CRView.h"

//再次声明全局变量
extern GLOBAL_STRUCTURE Global;

/////////////////////////////////////////////////////////////////////////////
// CDialogInput dialog


CDialogInput::CDialogInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_COMBO_POINTSIZE, m_ComboPointSize);
	DDX_Control(pDX, IDC_COMBO_ROWS, m_ComboRows);
	DDX_Control(pDX, IDC_COMBO_COLS, m_ComboCols);
	DDX_Control(pDX, IDC_SLIDER_WIDTH, m_SliderWidth);
	DDX_Control(pDX, IDC_SLIDER_HEIGHT, m_SliderHeight);
	DDX_Control(pDX, IDC_SLIDER_WIDTH_PAT, m_SliderWidthPat);
	DDX_Control(pDX, IDC_SLIDER_HEIGHT_PAT, m_SliderHeightPat);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogInput, CDialog)
	//{{AFX_MSG_MAP(CDialogInput)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_CTLCOLOR()
		ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	    ON_BN_CLICKED(IDC_BUTTON_BAKCOLOR, OnButtonBakColor)
		ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
		ON_BN_CLICKED(IDC_BUTTON_COLOR_PAT, OnButtonColorPat)
	    ON_BN_CLICKED(IDC_BUTTON_BAKCOLOR_PAT, OnButtonBakColorPat)
		ON_BN_CLICKED(IDC_BUTTON_RESET_PAT, OnButtonResetPat)
		ON_CBN_SELCHANGE(IDC_COMBO_POINTSIZE, OnSelChangePointSize)
        ON_CBN_SELCHANGE(IDC_COMBO_ROWS, OnSelChangeRows)
        ON_CBN_SELCHANGE(IDC_COMBO_COLS, OnSelChangeCols)
    	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_WIDTH, OnReleasedcaptureSliderWidth)
    	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HEIGHT, OnReleasedcaptureSliderHeight)
    	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_WIDTH_PAT, OnReleasedcaptureSliderWidthPat)
    	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HEIGHT_PAT, OnReleasedcaptureSliderHeightPat)
	    ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	    ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	    ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	    ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
		ON_BN_CLICKED(IDC_BUTTON_PREPRO, OnButtonPrePro)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDialogInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    
	m_bErazor = FALSE;

	//设置按钮属性
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_BITMAP_COLOR);
	HBITMAP hBitmap = (HBITMAP)Bitmap.Detach();
	CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_BAKCOLOR);
	pButton->SetBitmap(hBitmap);
	pButton = (CButton*)GetDlgItem(IDC_BUTTON_COLOR);
    pButton->SetBitmap(hBitmap);
	pButton = (CButton*)GetDlgItem(IDC_BUTTON_COLOR_PAT);
	pButton->SetBitmap(hBitmap);
	pButton = (CButton*)GetDlgItem(IDC_BUTTON_BAKCOLOR_PAT);
	pButton->SetBitmap(hBitmap);

	//设置滑块属性
	m_SliderWidth.SetRangeMin(20);
	m_SliderWidth.SetRangeMax(450);
	m_SliderWidth.SetLineSize(10);
	m_SliderWidth.SetPos(390);
	
	m_SliderHeight.SetRangeMin(20);
	m_SliderHeight.SetRangeMax(450);
	m_SliderHeight.SetLineSize(10);
	m_SliderHeight.SetPos(390);

	m_SliderWidthPat.SetRangeMin(20);
	m_SliderWidthPat.SetRangeMax(450);
	m_SliderWidthPat.SetLineSize(10);
	m_SliderWidthPat.SetPos(390);
	
	m_SliderHeightPat.SetRangeMin(20);
	m_SliderHeightPat.SetRangeMax(450);
	m_SliderHeightPat.SetLineSize(10);
	m_SliderHeightPat.SetPos(390);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CDialogInput::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

/////////////////////////////////////////////////////////////////////////////
// CDialogInput message handlers


void CDialogInput::OnButtonColor() 
{
	// TODO: Add your control notification handler code here
	COLORREF MyColor = RGB(0,0,0);
	CColorDialog Dlg(0,CC_FULLOPEN,this);
	if(IDOK == Dlg.DoModal())
	{
		MyColor = Dlg.GetColor();
		if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetMatrixPointer() != NULL)
		{
			Global.m_pgCRView->GetMatrixPointer()->SetPointColor(MyColor);
            Global.m_pgCRView->GetMatrixPointer()->InvalidateRect(NULL,TRUE);
		}
	}
}

void CDialogInput::OnButtonBakColor() 
{
	// TODO: Add your control notification handler code here
	COLORREF MyColor = RGB(0,0,0);
	CColorDialog Dlg(0,CC_FULLOPEN,this);
	if(IDOK == Dlg.DoModal())
	{
		MyColor = Dlg.GetColor();
		if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetMatrixPointer() != NULL)
		{
			Global.m_pgCRView->GetMatrixPointer()->SetBackColor(MyColor);
            Global.m_pgCRView->GetMatrixPointer()->InvalidateRect(NULL,TRUE);
		}
	}
}

void CDialogInput::OnButtonReset() 
{
	if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetMatrixPointer() != NULL)
	{
		CPointMatrix* pMatrix = Global.m_pgCRView->GetMatrixPointer();
		//pMatrix->PrintMatrix();
		pMatrix->ResetMatrix();
	}
}

void CDialogInput::OnSelChangePointSize()
{
	if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetMatrixPointer() != NULL)
	{
		UpdateData(TRUE);
		CString sText;
		m_ComboPointSize.GetLBText(m_ComboPointSize.GetCurSel(), sText);

		if(atoi(sText) >= 8 && atoi(sText) <= 40)
		{
			Global.m_pgCRView->GetMatrixPointer()->SetPointSize(atoi(sText));
            Global.m_pgCRView->GetMatrixPointer()->Invalidate();
		}
		
	}
}

void CDialogInput::OnSelChangeRows()
{
	if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetMatrixPointer() != NULL)
	{
		UpdateData(TRUE);
		CString sText;
		m_ComboRows.GetLBText(m_ComboRows.GetCurSel(), sText);

		if(atoi(sText) >= 8 && atoi(sText) <= 32)
		{
			CPointMatrix* pMatrix = Global.m_pgCRView->GetMatrixPointer();
			pMatrix->InitializeMatrix(atoi(sText), pMatrix->GetMatrixCols());
			pMatrix->Invalidate();
		}
		
	}
}

void CDialogInput::OnSelChangeCols()
{
	if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetMatrixPointer() != NULL)
	{
		UpdateData(TRUE);
		CString sText;
		m_ComboCols.GetLBText(m_ComboCols.GetCurSel(), sText);

		if(atoi(sText) >= 8 && atoi(sText) <= 32)
		{
			CPointMatrix* pMatrix = Global.m_pgCRView->GetMatrixPointer();
			pMatrix->InitializeMatrix(pMatrix->GetMatrixRows(), atoi(sText));
			pMatrix->Invalidate();
		}
	}
}

void CDialogInput::OnReleasedcaptureSliderWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
	if(Global.m_pgCRView != NULL)
	{
		Global.m_pgCRView->SetMatrixWidth(m_SliderWidth.GetPos());
	}
}

void CDialogInput::OnReleasedcaptureSliderHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
	if(Global.m_pgCRView != NULL)
	{
		Global.m_pgCRView->SetMatrixHeight(m_SliderHeight.GetPos());
	}
}



//这是关于手写板的控制
void CDialogInput::OnButtonColorPat() 
{
	// TODO: Add your control notification handler code here
	COLORREF MyColor = RGB(0,0,0);
	CColorDialog Dlg(0,CC_FULLOPEN,this);
	if(IDOK == Dlg.DoModal())
	{
		MyColor = Dlg.GetColor();
		if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetHandPatPointer() != NULL)
		{
			Global.m_pgCRView->GetHandPatPointer()->SetPointColor(MyColor);
            Global.m_pgCRView->InvalidateRect(NULL,TRUE);
		}
	}
}

void CDialogInput::OnButtonBakColorPat() 
{
	// TODO: Add your control notification handler code here
	COLORREF MyColor = RGB(0,0,0);
	CColorDialog Dlg(0,CC_FULLOPEN,this);
	if(IDOK == Dlg.DoModal())
	{
		MyColor = Dlg.GetColor();
		if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetHandPatPointer() != NULL)
		{
			Global.m_pgCRView->GetHandPatPointer()->SetBackColor(MyColor);
            Global.m_pgCRView->InvalidateRect(NULL,TRUE);
		}
	}
}

void CDialogInput::OnButtonResetPat() 
{
	if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetHandPatPointer() != NULL)
	{
		Global.m_pgCRView->GetHandPatPointer()->ResetPointArray();
		Global.m_pgCRView->Invalidate(TRUE);
	}
}


void CDialogInput::OnReleasedcaptureSliderWidthPat(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
	if(Global.m_pgCRView != NULL)
	{
		Global.m_pgCRView->SetHandPatWidth(m_SliderWidthPat.GetPos());
	}
}

void CDialogInput::OnReleasedcaptureSliderHeightPat(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
	if(Global.m_pgCRView != NULL)
	{
		Global.m_pgCRView->SetHandPatHeight(m_SliderHeightPat.GetPos());
	}
}

void CDialogInput::OnRadio1() 
{
	// TODO: Add your control notification handler code here
    if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetHandPatPointer() != NULL)
		{
			//if(m_bErazor)Global.m_pgCRView->GetHandPatPointer()->SetPointColor(m_PreColor);
			Global.m_pgCRView->GetHandPatPointer()->SetPenWidth(3);
			Global.m_pgCRView->Invalidate(TRUE);
		}
	}
}
void CDialogInput::OnRadio2() 
{
	// TODO: Add your control notification handler code here
    if(((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetHandPatPointer() != NULL)
		{
			//if(m_bErazor)Global.m_pgCRView->GetHandPatPointer()->SetPointColor(m_PreColor);
			Global.m_pgCRView->GetHandPatPointer()->SetPenWidth(6);
			Global.m_pgCRView->Invalidate(TRUE);
		}
	}
}
void CDialogInput::OnRadio3() 
{
	// TODO: Add your control notification handler code here
    if(((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())
	{
		if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetHandPatPointer() != NULL)
		{
			//if(m_bErazor)Global.m_pgCRView->GetHandPatPointer()->SetPointColor(m_PreColor);
			Global.m_pgCRView->GetHandPatPointer()->SetPenWidth(9);
			Global.m_pgCRView->Invalidate(TRUE);
		}
	}
}
void CDialogInput::OnRadio4() 
{
	// TODO: Add your control notification handler code here
    if(((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())
	{
		if(Global.m_pgCRView != NULL && Global.m_pgCRView->GetHandPatPointer() != NULL)
		{
			int penwidth = Global.m_pgCRView->GetHandPatPointer()->GetPenWidth();
			Global.m_pgCRView->GetHandPatPointer()->SetPenWidth(-penwidth);
				
			//将背景颜色与前景颜色设为一致
			//m_PreColor = Global.m_pgCRView->GetHandPatPointer()->GetPointColor();
			//COLORREF backcolor = Global.m_pgCRView->GetHandPatPointer()->GetBackColor();
			//Global.m_pgCRView->GetHandPatPointer()->SetPointColor(backcolor);
			//Global.m_pgCRView->Invalidate(TRUE);
			m_bErazor = TRUE;
		}
	}
}





//这是预处理部分的控制
void CDialogInput::OnButtonPrePro() 
{
	// TODO: Add your control notification handler code here
	CString sWidth, sHeight;
	GetDlgItem(IDC_COMBO_WIDTH)->GetWindowText(sWidth);
	GetDlgItem(IDC_COMBO_HEIGHT)->GetWindowText(sHeight);
	
	if(sWidth.GetLength() == 0 || sHeight.GetLength() == 0)
	{
		MessageBox("请输入正确的预处理后的高度和宽度信息","系统提示",MB_OK);
		return;
	}

	if(Global.m_pgCRView != NULL)
	{
	    Global.m_pgCRView->ResultShow(TRUE);
		Global.m_pgCRView->OnPreProcess(atoi(sWidth), atoi(sHeight));
	}
}

void  CDialogInput::DoPreprocess()
{
	OnButtonPrePro();
}


BOOL CDialogInput::PreTranslateMessage(MSG* pMsg) 
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