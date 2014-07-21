// PointMatrixWindow.cpp : implementation file
//

#include "stdafx.h"
#include "CR.h"
#include "PointMatrixWindow.h"
#include ".\pointmatrixwindow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern int CR[10][8][8];

/////////////////////////////////////////////////////////////////////////////
// CPointMatrixWindow dialog


CPointMatrixWindow::CPointMatrixWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CPointMatrixWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointMatrixWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMatrix = NULL;
	m_nWidth = 205;
	m_nHeight = 195;
}


void CPointMatrixWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointMatrixWindow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPointMatrixWindow, CDialog)
	//{{AFX_MSG_MAP(CPointMatrixWindow)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_RESET_DISPLAY, OnResetDisplay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointMatrixWindow message handlers

BOOL CPointMatrixWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetupRegion();

	SetupAnimateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPointMatrixWindow::SetupRegion()
{
	CDC* pDC;
	CDC	 memDC;
	CBitmap	cBitmap;
	CBitmap* pOldMemBmp = NULL;
	COLORREF col;
	CRect cRect;
	int	x, y;
	CRgn wndRgn, rgnTemp;

	pDC = GetDC();
	GetWindowRect(&cRect);
	
	//��ģ��λͼѡ���豸������
	cBitmap.LoadBitmap(IDB_BITMAP_MASK);
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);

	//����ģ����״�Ĳ���������
	wndRgn.CreateRectRgn(0, 0, cRect.Width()+2,cRect.Height());
	for(x=0; x<=cRect.Width(); x++)
	{
		for(y=0; y<=cRect.Height(); y++)
		{
			//��λͼ�к�ɫСԲ����Ӧ������ٵ�
			col = memDC.GetPixel(x, y);
			if(col == 0)
			{
				rgnTemp.CreateRectRgn(x, y, x+1, y+1);
				wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
				rgnTemp.DeleteObject();	
			}
		}
	}
	if (pOldMemBmp) memDC.SelectObject(pOldMemBmp);

	//�������������������壬Ҳ���Ǵ���������Ĵ���
	SetWindowRgn((HRGN)wndRgn, TRUE);
	SetForegroundWindow();
	ReleaseDC(pDC);


	//�����Ի������е���ͼ��
    ShowMatrix();

	//�������Ƶ����Ͻ�
	CRect rc;
	GetClientRect(&rc);
	
	rc.top = 150;
	rc.left = 650;
	rc.right = rc.left + 364;
	rc.bottom = rc.top + 504;
 	MoveWindow(&rc,TRUE);
}


void  CPointMatrixWindow::SetupAnimateWindow()
{
	//��ô���Ԥ��Ĵ�С
	CRect dlgRect;
	GetWindowRect(dlgRect);
	CRect desktopRect;
	//�����ڿ�ʼ��С��Ϊ0
	GetDesktopWindow()->GetWindowRect(desktopRect);
	MoveWindow(desktopRect.Width(),
			   (desktopRect.Height() - dlgRect.Height()) / 2,
			   0,
			   0 );
	//��ʼ���仯��С
	m_nWindowWidth=dlgRect.Width();
	m_nWindowHeight=dlgRect.Height();
	m_nDx=2;
	m_nDy=4;
	m_nDx1=2;
	m_nDy1=4;
	m_unAminateID = 128;
	m_nAminateTime = 2000;
	m_unWindowID = 151;
	m_nWindowTime = 1;
	m_unSparkID = 23;
	m_nSparkTime = 1000;

	//�趨��ʱ��1	
	SetTimer(m_unWindowID,m_nWindowTime,NULL);
}


void  CPointMatrixWindow::ShowMatrix(void)
{
	CRect rect;
	GetClientRect(&rect);
	m_pMatrixRect.left = rect.left + DEVI_LEFT_MATRIX;
    m_pMatrixRect.right = m_pMatrixRect.left + m_nWidth;
	m_pMatrixRect.right += DEVI_LEFT_MATRIX;
	m_pMatrixRect.top = rect.top + DEVI_TOP_MATRIX;
	m_pMatrixRect.bottom = m_pMatrixRect.top + m_nHeight;
	m_pMatrixRect.bottom += DEVI_TOP_MATRIX;

	m_pMatrix = new CPointMatrix();
	m_pMatrix->Create("this",SS_WHITEFRAME|SS_NOTIFY,m_pMatrixRect,this,IDC_STATIC_RIGHT_MATRIX/*NULL*/);
    m_pMatrix->Invalidate();
	m_pMatrix->ShowWindow(SW_SHOW);
}

BOOL CPointMatrixWindow::OnEraseBkgnd(CDC* pDC) 
{	
	CRect	rect;
	GetWindowRect(&rect);

	CDC memDC;
	CBitmap			cBitmap;
	CBitmap*		pOldMemBmp = NULL;

	//������λͼ�ӵ��豸������
	cBitmap.LoadBitmap(IDB_BITMAP_BACK);
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);

	//������λͼ��ʾ����
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	if (pOldMemBmp) memDC.SelectObject( pOldMemBmp );
	
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}


UINT CPointMatrixWindow::OnNcHitTest(CPoint point) 
{
	UINT nHitTest = CDialog::OnNcHitTest(point); 
    return (nHitTest == HTCLIENT)? HTCAPTION:nHitTest;
//  return CDialog::OnNcHitTest(point);
}

HCURSOR CPointMatrixWindow::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPointMatrixWindow::OnCancel() 
{
	// TODO: Add extra cleanup here
//	CDialog::OnCancel();
	DestroyWindow();
}


void CPointMatrixWindow::OnOK() 
{
	// TODO: Add extra validation here
//	CDialog::OnOK();
	DestroyWindow();
}

void CPointMatrixWindow::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pMatrix!=NULL)
	{
		delete m_pMatrix;
		m_pMatrix = NULL;
	}
	delete this;
}


	


void CPointMatrixWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}


BOOL CPointMatrixWindow::PreTranslateMessage(MSG* pMsg) 
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




void CPointMatrixWindow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
		
	//��ô�ʱ���ڵ�ʵ�ʴ�С
	CRect dlgRect;
	GetWindowRect(dlgRect);
	//�������Ĵ�С
	CRect desktopRect;
	GetDesktopWindow()->GetWindowRect(desktopRect);
	//����Ǵ��ڵ������̣��������󴰿�
	if(nIDEvent == m_unWindowID)
	{
		MoveWindow(
				  -m_nDx+desktopRect.Width()-dlgRect.Width() - 50,
				  (-m_nDy+desktopRect.Height() - dlgRect.Height() - 50)/2, 
				  +m_nDx+dlgRect.Width(),
				  +m_nDy+dlgRect.Height() );
		
		//��Ҫ��������Ԥ��Ŀ��
		if(dlgRect.Width() >=m_nWindowWidth) 
			m_nDx=0; 
		//��Ҫ��������Ԥ��ĸ߶�
		if(dlgRect.Height() >=m_nWindowHeight)
			m_nDy=0;
		//ֹͣ�仯���رն�ʱ��1
		if((dlgRect.Width() >=m_nWindowWidth) && (dlgRect.Height() >=m_nWindowHeight))
		    KillTimer(m_unWindowID);
	}
	
	//ֹͣ�仯���رն�ʱ��1
	if((dlgRect.Width() >=m_nWindowWidth) && (dlgRect.Height() >=m_nWindowHeight))
		KillTimer(m_unWindowID); 
	
	//����Ǵ��ڹرչ��̣�������С����
	if(nIDEvent == m_unWindowID+1)
	{
		MoveWindow((+m_nDx+desktopRect.Width() - dlgRect.Width()) / 2,
			(+m_nDy+desktopRect.Height() - dlgRect.Height()) / 2,
			-m_nDx1+dlgRect.Width(),
			-m_nDy1+dlgRect.Height() );
		
		//����ȵ�������ȾͲ��ڱ仯
		if(dlgRect.Width() <= 0) 
			m_nDx1=0;
		//���߶ȵ������߶ȾͲ��ڱ仯
		if(dlgRect.Height() <= 0 )
			m_nDy1=0; 
		//ֹͣ�仯���رն�ʱ��2�����ҹرմ���
		if((dlgRect.Width() <= 0 ) && (dlgRect.Height() <=0))
		{
			KillTimer(m_unWindowID+1);
			//CDialog::OnOK();
		}
		
	}

	if(nIDEvent == m_unSparkID)
	{
		static int a=128;
		static int b=0;
		if(b>10){KillTimer(m_unSparkID);b=0;}
		m_pMatrix->SetBackColor(RGB((a+=151)%255,(a+=151)%255,(a+=151)%255));
		m_pMatrix->SetPointColor(RGB((a-=23)%255,(a-=23)%255,(a-=23)%255));
		m_pMatrix->Invalidate();
		b++;
	}

	CDialog::OnTimer(nIDEvent);
}



LRESULT CPointMatrixWindow::OnResetDisplay(WPARAM wParam,LPARAM lParam)
{
	ResetDisplayMessageStruct* display = (ResetDisplayMessageStruct*)lParam;
	if(display)
	{
		m_pMatrix->InitializeMatrix(display->height, display->width);
		int min = (m_pMatrixRect.Height()-10)/display->height > (m_pMatrixRect.Width()-10)/display->width ?  (m_pMatrixRect.Width()-10)/display->width : (m_pMatrixRect.Height()-10)/display->height;
		m_pMatrix->SetPointSize(min);
        
		if(display->resdata && display->num == 1)
		{
			if(display->resdata[0] < 0 || display->resdata[0] > 7)display->resdata[0] = 8;
			for(int j=0;j<display->height;j++)
				for(int k=0;k<display->width;k++)
					m_pMatrix->SetPointMatrix(j,k,CR[display->resdata[0]][j][k]);

		}
		else if(display->data)
		{
			for(int i=0;i<display->num;i++)
			{
				for(int j=0;j<display->height;j++)
					for(int k=0;k<display->width;k++)
						m_pMatrix->SetPointMatrix(j,k,(UINT)display->data[i][(display->height-1-j)*display->width+k]);
			}

		}
		m_pMatrix->Invalidate(TRUE);
		//������˸Ч��
		SetTimer(m_unSparkID,m_nSparkTime,NULL);

		char* str = "Ԥ��������ʾ";
	    char* str2 = "ʶ�������ʾ";
		char* str3 = "������������ʾ";
		if(display->nPreprocess == 1)
			GetDC()->TextOut(128,8,str,strlen(str));
		else if(display->nPreprocess == 2)
			GetDC()->TextOut(128,8,str2,strlen(str2));
		else if(display->nPreprocess == 3)
			GetDC()->TextOut(128,8,str3,strlen(str3));

	}
	return 0;
}
