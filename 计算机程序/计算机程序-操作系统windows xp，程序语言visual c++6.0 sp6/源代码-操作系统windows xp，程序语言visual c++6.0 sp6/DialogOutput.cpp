// DialogOutput.cpp : implementation file
//

#include "stdafx.h"
#include "CR.h"
#include "DialogOutput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "CRView.h"

//再次声明全局变量
extern GLOBAL_STRUCTURE Global;

extern int CR[10][8][8];


/////////////////////////////////////////////////////////////////////////////
// CDialogOutput dialog


CDialogOutput::CDialogOutput(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOutput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogOutput)
	m_strSampleValue = _T("");
	//}}AFX_DATA_INIT
	m_pMatrix = NULL;                   
	m_nWidth = 120;                        //点阵宽度
	m_nHeight = 120;                       //点阵高度
	m_nCount = 0;
    memset(m_pErrResult,0,15000*sizeof(double));
	m_nErrResult = 0;
	m_unTime = 0;
}


void CDialogOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOutput)
	DDX_Control(pDX, IDC_LIST_RESULT, m_ListResult);
	DDX_Text(pDX, IDC_EDIT_SAMPLE, m_strSampleValue);
	DDV_MaxChars(pDX, m_strSampleValue, 1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOutput, CDialog)
	//{{AFX_MSG_MAP(CDialogOutput)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_CHECK_SAMPLE, OnCheckSample)
	ON_BN_CLICKED(IDC_BUTTON_SAMPLE, OnButtonSample)
	ON_BN_CLICKED(IDC_BUTTON_SAMPLE_SUB, OnButtonSampleSub)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOutput message handlers



HBRUSH CDialogOutput::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDialogOutput::SetShowResult(ResetDisplayMessageStruct* display)
{
	if(display->resdata && display->num == 1)
	{
		if(display->resdata[0] < 0 || display->resdata[0] > 7)display->resdata[0] = 8;
	    ShowCorrectMatrix(display->resdata[0]);
	}
}
void CDialogOutput::ShowCorrectMatrix(int n)
{
	GetDlgItem(IDC_STATIC_MATRIX)->GetWindowRect(&m_MatrixRect);
	ScreenToClient(&m_MatrixRect);
	m_MatrixRect.left += 20;
	m_MatrixRect.top += 15;
	m_MatrixRect.right = m_MatrixRect.left + m_nWidth;
	m_MatrixRect.bottom = m_MatrixRect.top + m_nHeight;
		
	m_pMatrix = new CPointMatrix(8,8,13);
	m_pMatrix->Create("this",WS_SIZEBOX|SS_ETCHEDFRAME|SS_SUNKEN|SS_WHITEFRAME/*|WS_DLGFRAME*//*|SS_NOTIFY*/,m_MatrixRect,this,IDC_STATIC_BOTTOM_MATRIX/*NULL*/);
    m_pMatrix->UpdateWindow();
	m_pMatrix->ShowWindow(SW_SHOW);

	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			m_pMatrix->SetPointMatrix(i,j,CR[n][i][j]);
	m_pMatrix->Invalidate(TRUE);

}
	
void CDialogOutput::DrawAnalysisChart(double* dataarray, int arraysize, UINT time)
{
	if(dataarray == NULL || arraysize == 0)return;
	CRect ChartRect;
	GetDlgItem(IDC_STATIC_CHART)->GetWindowRect(&ChartRect);
	ScreenToClient(&ChartRect);

	ChartRect.left += 20;
	ChartRect.top += 15;
	ChartRect.right = ChartRect.left + m_nWidth * 3;
	ChartRect.bottom = ChartRect.top + m_nHeight;

	//画箭头
	DrawArrow(CPoint(ChartRect.left,ChartRect.bottom),CPoint(ChartRect.right,ChartRect.bottom),3,RGB(128,0,128));
	DrawArrow(CPoint(ChartRect.left,ChartRect.bottom),CPoint(ChartRect.left,ChartRect.top),3,RGB(128,0,128));
 
	//画坐标点和线段
	
	ChartRect.left += 3;
	ChartRect.right -= 5;
	double segment = (double)(ChartRect.Width()-15)/arraysize;
		
	CDC* pDC = GetDC();
	CPen strokepen;
	if(!strokepen.CreatePen(PS_SOLID,1,RGB(200,0,200)))
		return;
    CPen* pOldPen = pDC->SelectObject(&strokepen);

	int MAXHEIGHT = ChartRect.Height();
	int Temp = 0;

	double Min = 100.0, Max = 0.0;
	for(int i=0;i<arraysize;i++)
	{
		Temp = (int)((i+1)*segment);
		if(Min > dataarray[i]) Min = dataarray[i];
		if(Max < dataarray[i]) Max = dataarray[i];
		pDC->MoveTo(ChartRect.left+Temp,ChartRect.bottom);
		pDC->LineTo(ChartRect.left+Temp,(int)(dataarray[i]*3000)>MAXHEIGHT?ChartRect.top:ChartRect.bottom-(int)(dataarray[i]*3000));
	}

	pDC->SelectObject(pOldPen);

	//输出时间结果
	char text[256] = {0};
	sprintf(text, "%d", time);
	strcat(text,"毫秒");
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(text);

	memset(text,0,256);
	sprintf(text,"%f",Max);
	GetDlgItem(IDC_STATIC_MAXERR)->SetWindowText(text);

	memset(text,0,256);
	sprintf(text,"%f",Min);
	GetDlgItem(IDC_STATIC_MINERR)->SetWindowText(text);
}
void CDialogOutput::DrawArrow(CPoint start, CPoint end, int penwidth, COLORREF pencolor)
{
	//只做水平和垂直两个方向上的箭头
	//首先画线
	CDC* pDC = GetDC();
	CPen strokepen;
	if(!strokepen.CreatePen(PS_SOLID,penwidth,pencolor))
		return;
    CPen* pOldPen = pDC->SelectObject(&strokepen);
    pDC->MoveTo(start);
	pDC->LineTo(end);
	//然后画箭头
	HBRUSH newbrush,oldbrush;
	HDC hDC = pDC->GetSafeHdc();
    newbrush=::CreateSolidBrush(pencolor);
    oldbrush=(HBRUSH)::SelectObject(pDC->GetSafeHdc(),newbrush);
	if(start.y == end.y)//水平画箭头
	{
        ::BeginPath(hDC);
	    ::LineTo(hDC,end.x-15,end.y+5);
	    ::LineTo(hDC,end.x-15,end.y-5);
	    ::LineTo(hDC,end.x,end.y);
		::EndPath(hDC);
	    ::FillPath(hDC);
	}
    else if(start.x == end.x)//垂直画箭头
	{
        ::BeginPath(hDC);
		::LineTo(hDC,end.x+5,end.y+15);
	    ::LineTo(hDC,end.x-5,end.y+15);
	    ::LineTo(hDC,end.x,end.y);
		::EndPath(hDC);
	    ::FillPath(hDC);
	}

	pDC->SelectObject(pOldPen);
    ::SelectObject(hDC,oldbrush);
    ::DeleteObject(newbrush);

}

void CDialogOutput::AddResultToList(int Res, CString sample, int type, int width, int height, int sampleID)
{
	int nIndex=0;
	char str[50] = {0};

	nIndex=m_ListResult.InsertItem(m_nCount,(CString)str);
	sprintf(str, "%d", nIndex);
    m_ListResult.SetItemText(nIndex,0,(CString)str);

	if(Res == 1)
		m_ListResult.SetItemText(nIndex,1,"正确");
	else if(Res == 2)
		m_ListResult.SetItemText(nIndex,1,"错误");

	if(sample != "NOTHING")
		m_ListResult.SetItemText(nIndex,2,sample);

	if(type != -1)
	{
		if(type == 1)
			m_ListResult.SetItemText(nIndex,3,"点阵");
		else if(type == 2)
			m_ListResult.SetItemText(nIndex,3,"手写板");
	}	

	if(width != -1)
	{
		memset(str,0,50);
	    sprintf(str,"%d",width);
	    m_ListResult.SetItemText(nIndex,4,(CString)str);
	}

	if(height != -1)
	{
		memset(str,0,50);
	    sprintf(str,"%d",height);
	    m_ListResult.SetItemText(nIndex,5,(CString)str);
	}
		
	if(sampleID != -1)
	{
		memset(str,0,50);
	    sprintf(str,"%d",sampleID);
	    m_ListResult.SetItemText(nIndex,6,(CString)str);
	}
	m_nCount++;
}

void CDialogOutput::CalculateResult()
{
	CString strRes, strTime, strSampleNum;
	int nCorrectNum = 0, nWrongNum = 0;
	UINT nAverTime = 0; 
	int nSampleNum = 0;
	for(int i=0;i<m_nCount;i++)
	{
		strRes = m_ListResult.GetItemText(i,1);
		if(strRes == "正确")nCorrectNum++;
		else if(strRes == "错误")nWrongNum++;
		
//		strTime = m_ListResult.GetItemText(i,3);
//		nAverTime = (nAverTime*i + atoi(strTime))/(i+1);
		strSampleNum = m_ListResult.GetItemText(i,6);
		if(strSampleNum.GetLength()!=0)nSampleNum++;
	}

	char res[256] = {0};
	if(nCorrectNum != 0 || nWrongNum != 0)
	{
		sprintf(res,"%.3f%%",(float)nCorrectNum/(nCorrectNum+nWrongNum)*100);
	    GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(res);
	}
	memset(res,0,256);
//	sprintf(res,"%d",nAverTime);
//	strcat(res,"毫秒");
	sprintf(res,"%d",nSampleNum);
	GetDlgItem(IDC_STATIC_AVERTIME)->SetWindowText(res);

	return;
}



BOOL CDialogOutput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//显示结果点阵
	//ShowCorrectMatrix('N');

	m_fnt.CreatePointFont(100,"宋体");
	SetFont(&m_fnt);
	m_ListResult.SetFont(&m_fnt);
	m_ListResult.SetFont(&m_fnt);

	DWORD dwStyle=GetWindowLong(m_ListResult.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_ListResult.GetSafeHwnd(),GWL_STYLE,dwStyle);
 
	//初始化视图列表
    m_ListResult.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	m_ListResult.InsertColumn(1,"结果",LVCFMT_CENTER,40);
	m_ListResult.InsertColumn(2,"样本值",LVCFMT_CENTER,45);
	m_ListResult.InsertColumn(3,"样本类型",LVCFMT_CENTER,45);
	m_ListResult.InsertColumn(4,"样本宽",LVCFMT_CENTER,40);
	m_ListResult.InsertColumn(5,"样本高",LVCFMT_CENTER,40);
	m_ListResult.InsertColumn(6,"样本序号",LVCFMT_CENTER,40);

	m_ListResult.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_ListResult.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
      LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	GetDlgItem(IDC_EDIT_SAMPLE)->EnableWindow(FALSE);

	if(Global.m_pgCRView != NULL)
	{
		char temp[10] = {0};
		int i = 0;
		SAMPLE* sample = Global.m_pgCRView->GetSampleList()->GetFirstNode()->link;
		while(sample!=NULL)
		{
			memset(temp,0,10);
			sprintf(temp,"%c",(char)sample->m_Samvalue);
			AddResultToList(3,(CString)temp,sample->m_Type,sample->m_Width,sample->m_Height,i++);
			sample = sample->link;
		}
	}
	CalculateResult();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogOutput::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
    if(m_pErrResult)	
		DrawAnalysisChart(m_pErrResult,m_nErrResult,m_unTime);
}


void CDialogOutput::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	AddResultToList(1,"NOTHING",-1,-1,-1,-1);
	CalculateResult();
}

void CDialogOutput::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	AddResultToList(2,"NOTHING",-1,-1,-1,-1);
	CalculateResult();
}

void CDialogOutput::OnCheckSample() 
{
	// TODO: Add your control notification handler code here
	if(((CButton*)(GetDlgItem(IDC_CHECK_SAMPLE)))->GetCheck() == BST_CHECKED)
		GetDlgItem(IDC_EDIT_SAMPLE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT_SAMPLE)->EnableWindow(FALSE);
}

void CDialogOutput::OnButtonSample() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	double** data = NULL;
	int width=0, height=0, num=0, type=0;

	if(Global.m_pgCRView != NULL)
	{ 
		Global.m_pgCRView->GetSampleInfo(data,width,height,num,type);
		if(data == NULL || type < 1 || type >2)
		{
			MessageBox("添加样本出错，请确保执行了预处理操作。只能添加单个的点阵或手写板样本","系统提示",MB_OK);
			return;
		}
	}

	char* samplestr = m_strSampleValue.GetBuffer(m_strSampleValue.GetLength());
	if(strstr(Global.m_szOutput,samplestr) == NULL)
	{
		MessageBox("对不起，您要添加的样本超出规定范围，请重新输入","系统提示",MB_OK);
		return;
	}

	//添加到控件中和链表中
	int SampleId = 0;
	if(Global.m_pgCRView != NULL)
	{
		SampleId = Global.m_pgCRView->GetSampleList()->Length();
		char str[2] = {0};
		str[0] = m_strSampleValue.GetBuffer(m_strSampleValue.GetLength())[0];
		int value = (int)str[0];
		Global.m_pgCRView->GetSampleList()->AppendList(new SAMPLE(width,height,type,value,data));
	}
	AddResultToList(3,samplestr,type,width,height,SampleId);
	CalculateResult();
}

void CDialogOutput::ResetSampleIdInList(int len)
{
	CString str;
	int j = 0;
	char jstr[10] = {0};
	for (int i=0;j < len && i < m_ListResult.GetItemCount();i++)
	{
		str = m_ListResult.GetItemText(i, 6);
		if (str.GetLength() != 0)
		{
			memset(jstr,0,10);
			sprintf(jstr,"%d",j);
			m_ListResult.SetItemText(i, 6, jstr);
			j++;
		}
	}
}

void CDialogOutput::OnButtonSampleSub() 
{
	// TODO: Add your control notification handler code here

	CString str;
    POSITION pos = m_ListResult.GetFirstSelectedItemPosition();
    if (pos == NULL)return;
    else
	{
		while (pos)
		{
			int nItem = m_ListResult.GetNextSelectedItem(pos);
			str = m_ListResult.GetItemText(nItem, 6);
			if(str.GetLength()==0)continue;
			Global.m_pgCRView->GetSampleList()->RemoveNodeAt(atoi(str));
			m_ListResult.DeleteItem(nItem);
			ResetSampleIdInList(Global.m_pgCRView->GetSampleList()->Length());
        }
	}
	CalculateResult();
}

BOOL CDialogOutput::PreTranslateMessage(MSG* pMsg) 
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
