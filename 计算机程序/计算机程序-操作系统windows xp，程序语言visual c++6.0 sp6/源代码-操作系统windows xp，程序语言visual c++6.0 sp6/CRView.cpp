// CRView.cpp : implementation of the CCRView class
//

#include "stdafx.h"
#include "CR.h"
#include "func.h"

#include "CRDoc.h"
#include "CntrItem.h"
#include "CRView.h"

#include "mydiblib.h"
#include ".\crview.h"

#include "DialogOutput.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



GLOBAL_STRUCTURE Global;


//算法输出结果
double g_ArrayOut[8][3] ={ 
		0.1,0.1,0.1,
		0.1,0.1,0.9,
		0.1,0.9,0.1,
		0.1,0.9,0.9,
		0.9,0.1,0.1,
		0.9,0.1,0.9,
		0.9,0.9,0.1,
		0.9,0.9,0.9};
			
//最终输出正确的点阵		
int CR[9][8][8] ={
//A
{0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,
0,0,1,0,1,0,0,0,
0,0,1,0,1,0,0,0,
0,1,1,1,1,1,0,0,
0,1,0,0,0,1,0,0,
1,0,0,0,0,0,1,0,
1,0,0,0,0,0,1,0},
//B
{0,1,1,1,1,0,0,0,
0,1,0,0,0,1,0,0,
0,1,0,0,0,1,0,0,
0,1,0,0,0,1,0,0,
0,1,1,1,1,0,0,0,
0,1,0,0,0,1,0,0,
0,1,0,0,0,1,0,0,
0,1,1,1,1,0,0,0},
//C
{0,0,1,1,1,1,0,0,
0,1,0,0,0,0,1,0,
0,1,0,0,0,0,1,0,
0,1,0,0,0,0,0,0,
0,1,0,0,0,0,0,0,
0,1,0,0,0,0,1,0,
0,1,0,0,0,0,1,0,
0,0,1,1,1,1,0,0},
//U
{0,1,0,0,0,0,1,0,
0,1,0,0,0,0,1,0,
0,1,0,0,0,0,1,0,
0,1,0,0,0,0,1,0,
0,1,0,0,0,0,1,0,
0,1,0,0,0,0,1,0,
0,1,0,0,0,0,1,0,
0,0,1,1,1,1,0,0},
//N
{1,0,0,0,0,0,0,1,
1,1,0,0,0,0,0,1,
1,0,1,0,0,0,0,1,
1,0,0,1,0,0,0,1,
1,0,0,0,1,0,0,1,
1,0,0,0,0,1,0,1,
1,0,0,0,0,0,1,1,
1,0,0,0,0,0,0,1},
//I
{0,0,1,1,1,0,0,0,
0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,
0,0,1,1,1,0,0,0},
//X
{1,0,0,0,0,0,0,1,
0,1,0,0,0,0,1,0,
0,0,1,0,0,1,0,0,
0,0,0,1,1,0,0,0,
0,0,0,1,1,0,0,0,
0,0,1,0,0,1,0,0,
0,1,0,0,0,0,1,0,
1,0,0,0,0,0,0,1},
//Z
{1,1,1,1,1,1,1,1,
0,0,0,0,0,0,1,0,
0,0,0,0,0,1,0,0,
0,0,0,0,1,0,0,0,
0,0,0,1,0,0,0,0,
0,0,1,0,0,0,0,0,
0,1,0,0,0,0,0,0,
1,1,1,1,1,1,1,1},
//?
{0,0,1,1,1,0,0,0,
0,1,0,0,0,1,0,0,
0,1,0,0,0,0,1,0,
0,0,0,0,0,0,1,0,
0,0,0,1,1,1,0,0,
0,0,0,1,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,1,0,0,0,0}
};

/////////////////////////////////////////////////////////////////////////////
// CCRView

IMPLEMENT_DYNCREATE(CCRView, CFormView)

BEGIN_MESSAGE_MAP(CCRView, CFormView)
	//{{AFX_MSG_MAP(CCRView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_INPUT_MATRIX, OnInputMatrix)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_INPUT_HANDPAT, OnInputHandPat)
	ON_COMMAND(ID_INPUT_PICTURE, OnInputPicture)
	ON_COMMAND(ID_256ToGray, OnIMGPRC256ToGray)
	ON_COMMAND(ID_GrayToWhiteBlack, OnIMGPRCGrayToWhiteBlack)
	ON_COMMAND(ID_prcSharp, OnImgprcSharp)
	ON_COMMAND(ID_RemoveNoise, OnImgprcRemoveNoise)
	ON_COMMAND(ID_AdjustSlope, OnImgprcAdjustSlope)
	ON_COMMAND(ID_prcDivide, OnImgprcDivide)
	ON_COMMAND(ID_Standarize, OnImgprcStandarize)
	ON_COMMAND(ID_ShrinkAlign, OnImgprcShrinkAlign)
    ON_COMMAND(ID_TRAIN, OnBpnetTrain)
	ON_COMMAND(ID_RECGNIZE, OnBpnetRecognize)
	ON_COMMAND(ID_SHOWRESULT, OnShowresult)
	ON_COMMAND(ID_PREPRO_ALL, OnPreproAll)
	ON_COMMAND(ID_ADD_SAMPLE, OnAddSample)
	ON_COMMAND(ID_MATRIX_88, OnMatrix88)
	ON_COMMAND(ID_MATRIX_816, OnMatrix816)
	ON_COMMAND(ID_MATRIX_832, OnMatrix832)
	ON_COMMAND(ID_MATRIX_168, OnMatrix168)
	ON_COMMAND(ID_MATRIX_1616, OnMatrix1616)
	ON_COMMAND(ID_MATRIX_1632, OnMatrix1632)
	ON_COMMAND(ID_MATRIX_328, OnMatrix328)
	ON_COMMAND(ID_MATRIX_3216,  OnMatrix3216)
	ON_COMMAND(ID_MATRIX_3232, OnMatrix3232)
	ON_COMMAND(ID_HANDPAT_88,  OnHandPat88)
	ON_COMMAND(ID_HANDPAT_816, OnHandPat816)
	ON_COMMAND(ID_HANDPAT_832, OnHandPat832)
	ON_COMMAND(ID_HANDPAT_168, OnHandPat168)
	ON_COMMAND(ID_HANDPAT_1616, OnHandPat1616)
	ON_COMMAND(ID_HANDPAT_1632, OnHandPat1632)
	ON_COMMAND(ID_HANDPAT_328, OnHandPat328)
	ON_COMMAND(ID_HANDPAT_3216, OnHandPat3216)
	ON_COMMAND(ID_HANDPAT_3232, OnHandPat3232)
	ON_COMMAND(ID_PICTURE_OPEN, OnPictureOpen)
	ON_COMMAND(ID_CTRL_INPUT, OnCtrlInput)
	ON_COMMAND(ID_CTRL_BPNET, OnCtrlBpnet)
	ON_COMMAND(ID_CTRL_OUTPUT, OnCtrlOutput)
	ON_COMMAND(ID_SAMPLE_SAVE, OnSampleSave)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCRView construction/destruction

CCRView::CCRView()
	: CFormView(CCRView::IDD)
{
	//{{AFX_DATA_INIT(CCRView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSelection = NULL;
	// TODO: add construction code here
    m_pMatrix = NULL;
	m_pHandWritingPat = NULL;
	m_nWidth = 390, m_nHeight = 390;
	m_nHandWritingWidth = 390, m_nHandWritingHeight = 390;

	m_pStrokeCur = NULL;
	m_bLButtonDown = FALSE;
	m_nCurType = (ViewType)NONE;
	m_nSampleCurType = (ViewType)MATRIX;
	m_nSampleLastType = (ViewType)MATRIX;

	//m_hMatrixDIB = m_hHandPatDIB = m_hPictureDIB = NULL;
	m_data = NULL;
	m_datalen = 0;

	m_pDisplayWindow = NULL;

	for(int i=0;i<MULTI_MATRIX_SIZE;i++)
		m_pMultiMatrixs[i] = NULL;
	m_nMutliMatrixsNum = 0; 

	m_SampleList.LoadSampleFromFile(SAMPLE_FILE_NAME);

	//对全局变量赋值
	Global.m_pgCRView = this;
		
	fileloaded=false;
	gyhinfoinput=false;
	gyhfinished=false;
	m_hDIB=NULL;
}

CCRView::~CCRView()
{
	m_SampleList.WriteSampleToFile(SAMPLE_FILE_NAME);
	
	if(m_pMatrix != NULL)
	{
		delete m_pMatrix;
		m_pMatrix = NULL;
	}
	if(m_pHandWritingPat != NULL)
	{
		delete m_pHandWritingPat;
		m_pHandWritingPat = NULL;
	}

    freeDoubleArray(m_data,/*digicount*/m_datalen,m_nWidth*m_nHeight);

	for(int i=0;i<MULTI_MATRIX_SIZE;i++)
		if(m_pMultiMatrixs[i] != NULL)
		{
			delete m_pMultiMatrixs[i];
			m_pMultiMatrixs[i] = NULL;
		}
	
	if(!m_pDisplayWindow)
	{
		delete m_pDisplayWindow;
		m_pDisplayWindow = NULL;
	}
	if(!m_pStrokeCur)
	{
		delete m_pStrokeCur;
		m_pStrokeCur = NULL;
	}
}

void CCRView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCRView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CCRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CCRView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// TODO: remove this code when final selection model code is written
	m_pSelection = NULL;    // initialize selection

	//Active documents should always be activated
	COleDocument* pDoc = (COleDocument*) GetDocument();
	if (pDoc != NULL)
	{
		// activate the first one
		POSITION posItem = pDoc->GetStartPosition();
		if (posItem != NULL)
		{
			CDocItem* pItem = pDoc->GetNextItem(posItem);

			// only if it's an Active document
			COleDocObjectItem *pDocObjectItem =
				DYNAMIC_DOWNCAST(COleDocObjectItem, pItem);

			if (pDocObjectItem != NULL)
			{
				pDocObjectItem->DoVerb(OLEIVERB_SHOW, this);
			}
		}
	}
	//调用预处理窗口但不显示		
	OnMatrix88();
	OnBpnetTrain();
	OnInputMatrix();
	ResultShow(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CCRView printing

BOOL CCRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if (!CView::DoPreparePrinting(pInfo))
		return FALSE;
	
	if (!COleDocObjectItem::OnPreparePrinting(this, pInfo))
		return FALSE;

	return TRUE;
}

void CCRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCRView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add customized printing code here
	if(pInfo->m_bDocObject)
		COleDocObjectItem::OnPrint(this, pInfo, TRUE);
	else
		CView::OnPrint(pDC, pInfo);
}

void CCRView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CFormView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CCRView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CCRCntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced.

	// TODO: implement this function that tests for a selected OLE client item

	return pDocItem == m_pSelection;
}

void CCRView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CCRCntrItem object.
	COleInsertDialog dlg;
	if (dlg.DoModal(COleInsertDialog::DocObjectsOnly) != IDOK)
		return;

	BeginWaitCursor();

	CCRCntrItem* pItem = NULL;
	TRY
	{
		// Create new item connected to this document.
		CCRDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CCRCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// Initialize the item from the dialog data.
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // any exception will do
		ASSERT_VALID(pItem);
		
		pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);

		// As an arbitrary user interface design, this sets the selection
		//  to the last item inserted.

		// TODO: reimplement selection as appropriate for your application

		m_pSelection = pItem;   // set selection to last inserted item
		pDoc->UpdateAllViews(NULL);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
void CCRView::OnCancelEditCntr()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CCRView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CFormView::OnSetFocus(pOldWnd);
}

void CCRView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CCRView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CCRView diagnostics

#ifdef _DEBUG
void CCRView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCRView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCRDoc* CCRView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCRDoc)));
	return (CCRDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCRView message handlers


//删除视图上的其他项，survive表示幸存者
void CCRView::DeleteItems(ViewType survive)
{
	if(survive == NONE)
	{
		if(m_pMatrix){delete m_pMatrix; m_pMatrix = NULL;}
		if(m_pHandWritingPat){delete m_pHandWritingPat; m_pHandWritingPat = NULL;}
		//if(m_pPicture){delete m_pPicture; m_pPicture = NULL;}
	}
	else if(survive == MATRIX)
	{
		if(m_pHandWritingPat){delete m_pHandWritingPat; m_pHandWritingPat = NULL;}
		//if(m_pPicture){delete m_pPicture; m_pPicture = NULL;}
	}
	else if(survive == HANDWRITINGPAT)
	{
		if(m_pMatrix){delete m_pMatrix; m_pMatrix = NULL;}
		//if(m_pPicture){delete m_pPicture; m_pPicture = NULL;}
	}
	else if(survive == PICTURE)
	{
		if(m_pMatrix){delete m_pMatrix; m_pMatrix = NULL;}
		if(m_pHandWritingPat){delete m_pHandWritingPat; m_pHandWritingPat = NULL;}
	}
	else if(survive == MULTIMATRIX)
	{
		if(m_pMatrix){delete m_pMatrix; m_pMatrix = NULL;}
		if(m_pHandWritingPat){delete m_pHandWritingPat; m_pHandWritingPat = NULL;}
	}
}


/*点阵相关代码//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////*/


void  CCRView::ShowMatrix(int nRow, int nCol)
{
	CRect rect;
	GetClientRect(&rect);
	m_MatrixRect.left = rect.left + DEVI_LEFT_MATRIX_HANDWRITING;
    m_MatrixRect.right = m_MatrixRect.left + m_nWidth;
	m_MatrixRect.right += DEVI_LEFT_MATRIX_HANDWRITING;
	m_MatrixRect.top = rect.top + DEVI_TOP_MATRIX_HANDWRITING;
	m_MatrixRect.bottom = m_MatrixRect.top + m_nHeight;
	m_MatrixRect.bottom += DEVI_TOP_MATRIX_HANDWRITING;

	m_nCurType = (ViewType)MATRIX;

	m_pMatrix = new CPointMatrix(nRow,nCol,36);
	m_pMatrix->Create("this",WS_SIZEBOX|SS_ETCHEDFRAME|SS_SUNKEN|SS_WHITEFRAME/*|WS_DLGFRAME*//*|SS_NOTIFY*/,m_MatrixRect,this,IDC_STATIC_LEFT_MATRIX/*NULL*/);
    m_pMatrix->UpdateWindow();
	m_pMatrix->ShowWindow(SW_SHOW);

	Invalidate();
}

void CCRView::SetMatrixWidth(int nWidth)
{
	m_nWidth = nWidth;

	CRect rect;
	GetClientRect(&rect);
	m_MatrixRect.left = rect.left + DEVI_LEFT_MATRIX_HANDWRITING;
    m_MatrixRect.right = m_MatrixRect.left + m_nWidth;
	m_MatrixRect.right += DEVI_LEFT_MATRIX_HANDWRITING;
	m_MatrixRect.top = rect.top + DEVI_TOP_MATRIX_HANDWRITING;
	m_MatrixRect.bottom = m_MatrixRect.top + m_nHeight;
	m_MatrixRect.bottom += DEVI_TOP_MATRIX_HANDWRITING;

	if(m_pMatrix)
	{
		m_pMatrix->MoveWindow(m_MatrixRect, TRUE);
		m_pMatrix->Invalidate(TRUE);
	}
}

void CCRView::SetMatrixHeight(int nHeight)
{
	m_nHeight = nHeight;

	CRect rect;
	GetClientRect(&rect);
	m_MatrixRect.left = rect.left + DEVI_LEFT_MATRIX_HANDWRITING;
    m_MatrixRect.right = m_MatrixRect.left + m_nWidth;
	m_MatrixRect.right += DEVI_LEFT_MATRIX_HANDWRITING;
	m_MatrixRect.top = rect.top + DEVI_TOP_MATRIX_HANDWRITING;
	m_MatrixRect.bottom = m_MatrixRect.top + m_nHeight;
	m_MatrixRect.bottom += DEVI_TOP_MATRIX_HANDWRITING;

	if(m_pMatrix)
	{
		m_pMatrix->MoveWindow(m_MatrixRect, TRUE);
		m_pMatrix->Invalidate(TRUE);
	}

}

UINT  CCRView::GetCurWinID(CPoint point)
{
	//得到滚动条的偏移
//	int minscroll, maxscroll;
//	GetScrollRange(SB_HORZ, &minscroll, &maxscroll);
//	int h=GetScrollPos(SB_HORZ) - minscroll;
//	GetScrollRange(SB_VERT, &minscroll, &maxscroll);
//	int v=GetScrollPos(SB_VERT) - minscroll;

	//得到窗口和客户区的位置关系
	ScreenToClient(&point);
	CRect winrect, clientrect;
	GetWindowRect(&winrect);
	GetClientRect(&clientrect);

	//计算点的实际位置
	point.y -= clientrect.top-winrect.top;
//	point.y += v;
	point.x -= clientrect.left-winrect.left;
//	point.x += h;

	CWnd* pWnd = ChildWindowFromPoint(point,CWP_SKIPINVISIBLE);
	if(pWnd == NULL)
	{
		return 0;
	}

	//获得该窗口的ID
	return pWnd->GetDlgCtrlID();
}

void CCRView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default			
	switch( m_nCurType )
	{
	//如果视图当前显示为空
	case (ViewType)NONE:
		break;

    //如果视图当前显示为矩阵
	case (ViewType)MATRIX:
		{
			if(m_pMatrix == NULL) break;
			//获得该窗口的ID
			UINT nCtrlID = GetCurWinID(point);
			switch(nCtrlID)
			{
			case IDC_STATIC_LEFT_MATRIX:
				{			
					//显示十字光标
		            SetCursor(LoadCursor(NULL,IDC_CROSS));
                    //::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PEN));
		            if(m_bLButtonDown)
					{
						m_pMatrix->SendMessage(WM_USER_SETMOVEPOINT,(WPARAM)(point.x),(LPARAM)(point.y));
					}
		            break;
				}
			default:break;
			}
			break;
		}

	//如果视图当前显示为手写板
	case (ViewType)HANDWRITINGPAT:
		{
			if(m_pHandWritingPat == NULL)break;
			if(m_bLButtonDown)
			{
				//ScreenToClient(&point);
				if(GetCapture() != this)
						return;
				if(m_HandWritingRect.PtInRect(point))
				{
					CClientDC dc(this);
	                //CPen* pOldPen = CreatePen(&dc);
                    CPen strokepen;

				    if(m_pHandWritingPat->GetPenWidth() > 0)
					{
						::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PEN));
				        if(!strokepen.CreatePen(PS_SOLID,m_pHandWritingPat->GetPenWidth(),m_pHandWritingPat->GetPointColor()))
					       return;
					}
			    	else
					{
						::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ERAZOR));
					    if(!strokepen.CreatePen(PS_SOLID,m_pHandWritingPat->GetPenWidth(),m_pHandWritingPat->GetBackColor()))
					       return;
					}
				    //if(GetCapture() != this)
					    //return;
                    CPen* pOldPen = dc.SelectObject(&strokepen);
				
			     	m_pStrokeCur->m_PointArray.Add(point);

				    dc.MoveTo(m_ptPrev);
				    dc.LineTo(point);
				    dc.SelectObject(pOldPen);
				    m_ptPrev = point;

					//if(GetCapture() != this)
						//return;
				}
				else
					SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
			if(m_HandWritingRect.PtInRect(point))
			{
				if(m_pHandWritingPat->GetPenWidth() > 0)
					::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PEN));
				else
					::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ERAZOR));
			}
			break;
		}

	//如果视图当前显示为图片
	case (ViewType)PICTURE:
		{
			break;
		}

	//否则
	default:break;
	}


	CFormView::OnMouseMove(nFlags, point);
}

void CCRView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//指示鼠标左键已按下
	m_bLButtonDown = TRUE;

	switch( m_nCurType )
	{
	//如果视图当前显示为空
	case (ViewType)NONE:
		break;

    //如果视图当前显示为矩阵
	case (ViewType)MATRIX:
		{
			if(m_pMatrix == NULL)break;
			//获得该窗口的ID
			UINT nCtrlID = GetCurWinID(point);
			switch(nCtrlID)
			{
			case IDC_STATIC_LEFT_MATRIX:
				{
					//向点阵传递当前点坐标，并刷新点阵
		            SetCursor(LoadCursor(NULL,IDC_CROSS));
		            m_pMatrix->SendMessage(WM_USER_SETPOINT,(WPARAM)(point.x),(LPARAM)(point.y));
		            m_pMatrix->SendMessage(WM_PAINT,0,0);
		            break;
				}
			default:break;
			}
			break;
		}

	//如果视图当前显示为手写板
	case (ViewType)HANDWRITINGPAT:
		{
			if(m_pHandWritingPat == NULL)break;
			if(m_HandWritingRect.PtInRect(point))
			{
				m_pStrokeCur = new CStroke();
				if(m_pHandWritingPat->GetPenWidth() > 0)
				{
					m_pStrokeCur->m_bErazor = FALSE;
					::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PEN));
				}
				else
				{
					m_pStrokeCur->m_bErazor = TRUE;
					::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ERAZOR));
				}
				m_pHandWritingPat->m_StrokeList.AddTail(m_pStrokeCur);
				m_pStrokeCur->m_PointArray.Add(point);
			    SetCapture();
                m_ptPrev = point;
			}
			break;
		}

	//如果视图当前显示为图片
	case (ViewType)PICTURE:
		{
			break;
		}

	//否则
	default:break;
	}


	CFormView::OnLButtonDown(nFlags, point);
}

void CCRView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = FALSE;

	switch( m_nCurType )
	{
	//如果视图当前显示为空
	case (ViewType)NONE:
		break;

    //如果视图当前显示为矩阵
	case (ViewType)MATRIX:
		{
			if(m_pMatrix == NULL)break;
			//获得该窗口的ID
			UINT nCtrlID = GetCurWinID(point);
			switch(nCtrlID)
			{
			case IDC_STATIC_LEFT_MATRIX:
				{
					SetCursor(LoadCursor(NULL,IDC_CROSS));
		            break;
				}
			default:break;
			}
			break;
		}

	//如果视图当前显示为手写板
	case (ViewType)HANDWRITINGPAT:
		{
			if(m_pHandWritingPat == NULL)break;
			if(GetCapture() != this)
					return;
			if(m_HandWritingRect.PtInRect(point))
			{
				CClientDC dc(this);
	            //CPen* pOldPen = CreatePen(&dc);
                CPen strokepen;

				if(m_pHandWritingPat->GetPenWidth() > 0)
				{
					::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PEN));
				    if(!strokepen.CreatePen(PS_SOLID,m_pHandWritingPat->GetPenWidth(),m_pHandWritingPat->GetPointColor()))
					    return;
				}
				else
				{
					::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ERAZOR));
					if(!strokepen.CreatePen(PS_SOLID,m_pHandWritingPat->GetPenWidth(),m_pHandWritingPat->GetBackColor()))
					    return;
				}
				//if(GetCapture() != this)
					//return;
                CPen* pOldPen = dc.SelectObject(&strokepen);

				dc.MoveTo(m_ptPrev);
				dc.LineTo(point);
				dc.SelectObject(pOldPen);
				m_pStrokeCur->m_PointArray.Add(point);

				//Invalidate();
				//ReleaseCapture();
			}
			ReleaseCapture();
			break;
		}

	//如果视图当前显示为图片
	case (ViewType)PICTURE:
		{
			break;
		}

	//否则
	default:break;
	}


	CFormView::OnLButtonUp(nFlags, point);
}

/*点阵相关代码结束//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////*/







/*手写板相关代码//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////*/

/*辅助函数/////////////////////////////////////////////////*/
CPen* CCRView::CreatePen(CDC* pDC)
{
	CPen penstroke;
	if(!penstroke.CreatePen(PS_SOLID,6/*m_pHandWritingPat->GetPenWidth()*/,m_pHandWritingPat->GetPointColor()))
		return NULL;
	return pDC->SelectObject(&penstroke);
}

void  CCRView::CreateBckGround(CDC* pDC)
{
	CBrush brush; 
    brush.CreateSolidBrush(m_pHandWritingPat->GetBackColor()); 
	pDC->FillRect(m_HandWritingRect,&brush); 
	//画边框
	CreateHandPatBorder(pDC);
}

void  CCRView::CreateHandPatBorder(CDC *pDC)
{
	int left = m_HandWritingRect.left;	int right = m_HandWritingRect.right;
	int top = m_HandWritingRect.top;	int bottom = m_HandWritingRect.bottom;

	//填充外边框
	BrushRect(pDC,CRect(left - HANDPAT_BORDER_WIDTH * 2, top - HANDPAT_BORDER_WIDTH * 2,
		right + HANDPAT_BORDER_WIDTH, top - HANDPAT_BORDER_WIDTH), 
		HANDPAT_BORDER_COLOR_OUT_LEFT);
    //画上内边框
	BrushRect(pDC,CRect(left - HANDPAT_BORDER_WIDTH, top - HANDPAT_BORDER_WIDTH,
		right, top),
		HANDPAT_BORDER_COLOR_IN_LEFT);
 	//画左外边框
	BrushRect(pDC,CRect(left - HANDPAT_BORDER_WIDTH * 2, top - HANDPAT_BORDER_WIDTH,
		left - HANDPAT_BORDER_WIDTH, bottom + HANDPAT_BORDER_WIDTH), 
		HANDPAT_BORDER_COLOR_OUT_LEFT);
	//画左内边框
	BrushRect(pDC,CRect(left - HANDPAT_BORDER_WIDTH, top - HANDPAT_BORDER_WIDTH,
		left, bottom), 
		HANDPAT_BORDER_COLOR_IN_LEFT);
	//画下外边框
	BrushRect(pDC,CRect(left - HANDPAT_BORDER_WIDTH * 2, bottom + HANDPAT_BORDER_WIDTH,
		right + HANDPAT_BORDER_WIDTH * 2, bottom + HANDPAT_BORDER_WIDTH * 2), 
		HANDPAT_BORDER_COLOR_OUT_RIGHT);
	//画下内边框
	BrushRect(pDC,CRect(left - HANDPAT_BORDER_WIDTH, bottom,
		right + HANDPAT_BORDER_WIDTH, bottom + HANDPAT_BORDER_WIDTH),
		HANDPAT_BORDER_COLOR_IN_RIGHT);
	//画右外边框
	BrushRect(pDC,CRect(right + HANDPAT_BORDER_WIDTH, top - HANDPAT_BORDER_WIDTH * 2,
		right + HANDPAT_BORDER_WIDTH * 2, bottom + HANDPAT_BORDER_WIDTH), 
		HANDPAT_BORDER_COLOR_OUT_RIGHT);
	//画右内边框
	BrushRect(pDC,CRect(right, top - HANDPAT_BORDER_WIDTH,
		right + HANDPAT_BORDER_WIDTH, bottom), 
		HANDPAT_BORDER_COLOR_IN_RIGHT);
}

void CCRView::DrawLine(CDC* pDC, CPoint start, CPoint end, int width, COLORREF color)
{
	CPen pen;
	if(!pen.CreatePen(PS_ENDCAP_FLAT|PS_SOLID,width,color))
		return;
	pDC->MoveTo(start);	
	pDC->LineTo(end);
}

void CCRView::BrushRect(CDC* pDC, CRect rect, COLORREF color)
{
	//填充颜色
	CBrush brush; 
    brush.CreateSolidBrush(color); 
	pDC->FillRect(rect,&brush); 
}

void CCRView::SetHandPatWidth(int nWidth)
{
	m_nHandWritingWidth = nWidth;
    
	CRect rect;
	GetClientRect(&rect);
	m_HandWritingRect.left = rect.left + DEVI_LEFT_MATRIX_HANDWRITING;
    m_HandWritingRect.right = m_HandWritingRect.left + m_nHandWritingWidth;
	m_HandWritingRect.right += DEVI_LEFT_MATRIX_HANDWRITING;
	m_HandWritingRect.top = rect.top + DEVI_TOP_MATRIX_HANDWRITING;
	m_HandWritingRect.bottom = m_HandWritingRect.top + m_nHandWritingHeight;
	m_HandWritingRect.bottom += DEVI_TOP_MATRIX_HANDWRITING;
	
	Invalidate(TRUE);

}

void CCRView::SetHandPatHeight(int nHeight)
{
	m_nHandWritingHeight = nHeight;
    
	CRect rect;
	GetClientRect(&rect);
	m_HandWritingRect.left = rect.left + DEVI_LEFT_MATRIX_HANDWRITING;
    m_HandWritingRect.right = m_HandWritingRect.left + m_nHandWritingWidth;
	m_HandWritingRect.right += DEVI_LEFT_MATRIX_HANDWRITING;
	m_HandWritingRect.top = rect.top + DEVI_TOP_MATRIX_HANDWRITING;
	m_HandWritingRect.bottom = m_HandWritingRect.top + m_nHandWritingHeight;
	m_HandWritingRect.bottom += DEVI_TOP_MATRIX_HANDWRITING;
	
	Invalidate(TRUE);
}

/*////////////////////////////////////////////////////////////*/


void CCRView::ShowHandWritingPat()
{
	CRect rect;
	GetClientRect(&rect);
	m_HandWritingRect.left = rect.left + DEVI_LEFT_MATRIX_HANDWRITING;
    m_HandWritingRect.right = m_HandWritingRect.left + m_nHandWritingWidth;
	m_HandWritingRect.right += DEVI_LEFT_MATRIX_HANDWRITING;
	m_HandWritingRect.top = rect.top + DEVI_TOP_MATRIX_HANDWRITING;
	m_HandWritingRect.bottom = m_HandWritingRect.top + m_nHandWritingHeight;
	m_HandWritingRect.bottom += DEVI_TOP_MATRIX_HANDWRITING;

	m_pHandWritingPat = new CHandWritingPat();
	m_nCurType = (ViewType)HANDWRITINGPAT;
    Invalidate();
}

void CCRView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_nCurType == (ViewType)HANDWRITINGPAT )
	{		
		if(m_pHandWritingPat == NULL)return;
		CreateBckGround(pDC);

		//循环显示数组中的点
		POSITION pos = m_pHandWritingPat->m_StrokeList.GetHeadPosition();
		while(pos != NULL)
		{
			CPen strokepen;
			CStroke* pStroke = m_pHandWritingPat->m_StrokeList.GetNext(pos);
			if(pStroke->m_bErazor == FALSE)
			{
				if(!strokepen.CreatePen(PS_SOLID,m_pHandWritingPat->GetPenWidth(),m_pHandWritingPat->GetPointColor()))
			       return;
			}
			else
			{		
				if(!strokepen.CreatePen(PS_SOLID,m_pHandWritingPat->GetPenWidth(),m_pHandWritingPat->GetBackColor()))
			       return;
			}
            CPen* pOldPen = pDC->SelectObject(&strokepen);

			if(pStroke->m_PointArray.GetSize() > 0)
			{
				pDC->MoveTo(pStroke->m_PointArray[0]);
			}
			for(int i=1;i<pStroke->m_PointArray.GetSize();i++)
			{
				pDC->LineTo(pStroke->m_PointArray[i]);
			}
				
			pDC->SelectObject(pOldPen);

		}

	}
	else if(m_nCurType == (ViewType)PICTURE)
	{
		if(m_hDIB!=NULL) 
			DisplayDIB(pDC,m_hDIB);
//		if(m_hHandPatDIB!=NULL) 
//			DisplayDIB(pDC,m_hHandPatDIB);
//		if(m_hMatrixDIB!=NULL) 
//			DisplayDIB(pDC,m_hMatrixDIB);
	}

}



/*手写板相关代码结束//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////*/








/*图片相关代码//////////////////////////////////////////////////////////////////////////*/

//从文件中打开图片
void CCRView::ShowPicture()
{
	//创建一个打开文件对话框，并返回完整的文件路径
	static char BASED_CODE szFilter[] = "256色位图文件(*.bmp)|";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL);
    if(dlg.DoModal() == IDOK)
	   strPathName = dlg.GetPathName();
	else return;
 
	m_hDIB = CreateHDIBByPicture(strPathName);
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIB);

	m_nCurType = PICTURE;
	
	return;
}

//取消一切更改，重新加载位图文件
void CCRView::FileReLoadBmp() 
{
	//判断位图文件是否已加载。如果尚未加载，则弹出文件打开对话框
	if(fileloaded==false)
	{
		ShowPicture();
		if(fileloaded==false)
			return;
	}
   	CFile file;
	file.Open (strPathName,CFile::modeReadWrite);
	m_hDIB=::ReadDIBFile (file);
	file.Close ();
	//在屏幕上显示位图
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIB);		
}
/*图片相关代码结束///////////////////////////////////////////////////////////////////////*/




//预处理入口
void CCRView::OnPreProcess(int DesireWidth, int DesireHeight)
{
	switch(m_nCurType)
	{
	case MATRIX:
		{
			//根据二维数组创建DIB句柄
			if(m_pMatrix == NULL)
				return;
			int Height = m_pMatrix->GetMatrixRows();
			int Width = m_pMatrix->GetMatrixCols();
			m_pMatrix->RemoveNoiseForMatrix(2);
			//为了使预处理的分割字符步骤能画上边框，使高度和宽度都加64
			int add = 64;
			Width = (Width+3)/4*4;
			Height += add; Width += add;
			BYTE* pointarray = (BYTE*)malloc(Width*Height);
			memset(pointarray, 0, Width*Height);
			for(int i = add/2; i < Height-add/2; i++)
				for(int j = add/2; j < Width-add/2; j++)
					pointarray[(i)*Width+j/*(i-add/2)*Width+j-add/2*/] = m_pMatrix->GetPointMatrix()[i-add/2][j-add/2];

//			for(int i = 0; i < Height; i++)
//				for(int j = 0; j < Width; j++)
//					pointarray[(i)*Width+j] = m_pMatrix->GetPointMatrix()[i][j];

			m_hDIB = CreateHDIBByDbArray(pointarray, Width, Height);
			
			free(pointarray);
			break;
		}
	case HANDWRITINGPAT:
		{
			//根据屏幕位置创建DIB句柄
			if(m_pHandWritingPat == NULL)
				return;
			m_hDIB = CreateHDIBByScreen(&m_HandWritingRect);
			break;
		}
	case PICTURE:
		{
			//根据文件创建DIB句柄
			FileReLoadBmp();
			//m_hDIB= CreateHDIBByPicture(strPathName);
			break;
		}
	default:break;
	}

	if(m_hDIB == NULL)
	{
		MessageBox("创建DIB错误", "系统提示" , MB_ICONINFORMATION | MB_OK);
		return;
	}
//	Invalidate(TRUE);	
	if(IsEmpty(m_hDIB))
	{
		MessageBox("您输入的字符为空，请修正","系统提示",MB_ICONINFORMATION | MB_OK);
		return;
	}
	
	//以得到的句柄为参数调用预处理，返回的是预处理后的二维数组
	//将此数组保存为成员变量，以后可以传递到显示模块和处理模块
    if((m_data = PreProcess(DesireWidth, DesireHeight, m_hDIB)) != NULL)
	{
		m_nDataWidth = DesireWidth;
	    m_nDataHeight = DesireHeight;
	}
	else
	{
		MessageBox("特征提取错误", "系统提示" , MB_ICONINFORMATION | MB_OK);
		return;
	}

	//送预处理结果到显示
	if(m_pDisplayWindow)
	{
		ResetDisplayMessageStruct display;
		display.data = m_data;
	    display.height = m_nDataHeight;
	    display.width = m_nDataWidth;
		display.num = digicount;
		display.nPreprocess = 1;
		display.resdata = NULL;
		if(digicount == 1)
		{
			ResultShow(TRUE);
			m_pDisplayWindow->SendMessage(WM_USER_RESET_DISPLAY,0,(LPARAM)&display);
		}
		else
		{
			ResultShow(FALSE);
			ShowMultiMatrixs(&display);
		}
	}
}

//根据二维数组创建DIB句柄
HDIB CCRView::CreateHDIBByDbArray(BYTE* pointarray, int Width, int Height)
{
	unsigned char* lpSrc;
	int i_src,j_src;
	BYTE* lpDIB;
	BYTE* lpNewDIBBits;
	BYTE* lpDst;
	LONG lLineBytes=(Width+3)/4*4;
	LONG lLineBytesnew =(Width+3)/4*4;
	HDIB hDIB=NULL;
		
	hDIB=::NewDIB (lLineBytes/*Width*/,Height,8);
    lpDIB=(BYTE*) ::GlobalLock((HGLOBAL)hDIB);	
	lpNewDIBBits = (BYTE*)::FindDIBBits((char*)lpDIB);
	lpDst=(BYTE*)lpNewDIBBits;
	memset(lpDst,(BYTE)255,lLineBytesnew * Height);		
	
	for(i_src=0;i_src<Height;i_src++)
		for(j_src=0;j_src<Width;j_src++)
		{
			lpSrc=(BYTE *)pointarray + lLineBytes *  (Height -1 - i_src) + j_src;
			lpDst=(BYTE *)lpNewDIBBits + lLineBytesnew * i_src + j_src;
			*lpDst=255-*lpSrc*255;
		}
		
	::GlobalUnlock (hDIB);
	return hDIB;
}

//根据屏幕位置创建DIB句柄
HDIB CCRView::CreateHDIBByScreen(CRect* ScreenRect)
{
	HDC hScrDC;           // 屏幕和内存设备描述表 
    int nX, nY, nX2, nY2; // 选定区域坐标 
    int nWidth, nHeight;  // 位图宽度和高度 
    int xScrn, yScrn;     // 屏幕分辨率 

	// 确保选定区域不为空矩形 
	if (IsRectEmpty(ScreenRect)) 
		return NULL; 
    //为屏幕创建设备描述表 
    hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 
    // 获得选定区域坐标 
    nX = ScreenRect->left; 
    nY = ScreenRect->top; 
    nX2 = ScreenRect->right; 
    nY2 = ScreenRect->bottom; 
    // 获得屏幕分辨率 
    xScrn = GetDeviceCaps(hScrDC, HORZRES); 
    yScrn = GetDeviceCaps(hScrDC, VERTRES); 
    //确保选定区域是可见的 
    if (nX < 0)nX = 0; 
	if (nY < 0)nY = 0; 
	if (nX2 > xScrn)nX2 = xScrn;
	if (nY2 > yScrn)nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY; 

	CDC* pDC = GetDC();

	//逐行逐列扫描屏幕，得到实际的字符宽度和高度
	//先从上到下扫一遍，得到上下底
	int i,j;
	int left=nX2,top=nY,right=nX,bottom=nY;
	int bfirstop=FALSE,bfirstbotom=TRUE,bfirstleft=FALSE,bfirstright=TRUE;
	for(i = nY ; i < nY2; i++)
	{
		for(j = nX; j < nX2; j++)
		{
			if(pDC->GetPixel(j,i) == m_pHandWritingPat->GetPointColor())
			{
				if(bfirstop==FALSE)
				{
					top = i;
					bfirstop = TRUE;
				}
				if(bfirstleft==FALSE)
				{
					if(left>j)left=j;
				}
				bfirstbotom = FALSE;
				bfirstright = FALSE;
			}
			else
			{
				if(bfirstbotom==FALSE)
				{
					if(bottom<i)bottom = i;
					bfirstbotom = TRUE;
				}
				if(bfirstright==FALSE)
				{
					if(right<j)right=j;
					bfirstright = TRUE;
				}
			}
		}
	}
	int add = 8;
	nX = left-add>0?left-add:left;//这是为了字符分割时不至于画不上边框
	nY = top-add>0?top-add:top;
	nX2 = right+add/*<ScreenRect->right?right+8:right*/;
	nY2 = bottom+add/*<ScreenRect->bottom?bottom+8:bottom*/;
//	nX = left-4; nY = top-4; nX2 = right+4; nY2 = bottom+4; 
	nWidth = (nX2-nX)/4*4; nHeight = nY2-nY;
	
	//逐行逐列扫描屏幕
	BYTE* screenarray = (BYTE*)malloc(nWidth*nHeight);
	for(i = nY ; i < nY2; i++)
		for(j = nX; j < nX+nWidth; j++)
		{
			if(pDC->GetPixel(j,i) == m_pHandWritingPat->GetPointColor())
				screenarray[(i-nY)*nWidth+j-nX] = 1;
			else
				screenarray[(i-nY)*nWidth+j-nX] = 0;
		}

	HDIB hDIB = CreateHDIBByDbArray(screenarray, nWidth, nHeight);
    free(screenarray);

	return hDIB;
}

//根据图像文件创建DIB句柄
HDIB CCRView::CreateHDIBByPicture(CString strPathFileName)
{
	CFile file;
	file.Open (strPathFileName,CFile::modeRead);
	HDIB hDIB = ::ReadDIBFile (file);
	file.Close ();
	BYTE* lpDIB=(BYTE*)::GlobalLock ((HGLOBAL)hDIB);
    // 获取DIB中颜色表中的颜色数目
    WORD wNumColors;	
	wNumColors = ::DIBNumColors((char*)lpDIB);	
	// 判断是否是256色位图
	if (wNumColors != 256)
	{
		MessageBox("非256色位图", "系统提示" , MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)hDIB);
	}
	fileloaded=true;
	gyhfinished=false;
	return hDIB;
}




double** CCRView::PreProcess(int Width, int Height, HDIB hDIB) 
{
	//规定归一化为width*height的点阵
	w_sample = Width;
	h_sample = Height;
    m_hDIB = hDIB;

    //打开文件
	//FileReLoadBmp();
	//判断用户是否已输入归一化高度和宽度信息
	gyhinfoinput=true;


	//将256色图转换为灰度图
	if(!OnIMGPRC256ToGray())return NULL;
	//将灰度图二值化
	if(!OnIMGPRCGrayToWhiteBlack())return NULL;
	//梯度锐化
	//if(!OnImgprcSharp())return NULL;
	//去除离散杂点噪声
	if(!OnImgprcRemoveNoise())return NULL;
	//调整数字字符的整体倾斜
	if(!OnImgprcAdjustSlope())return NULL;
	//分割并画框标识
	if(!OnImgprcDivide())return NULL;
	//将分割后的数字字符宽、高标准化，以便于下一步与BP网络的输入兼容
	if(!OnImgprcStandarize())return NULL;
	//紧缩重排数字字符
	if(!OnImgprcShrinkAlign())return NULL;
	
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIB);	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		::MessageBox(NULL,"只能处理灰度图像",NULL,MB_ICONSTOP);
		return NULL;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 	
	//归一化的宽度
	LONG lSwidth = w_sample;	
	//归一化的高度
	LONG LSheight = h_sample;

	m_datalen = digicount;
	return code(lpDIBBits, digicount,  lLineByte, lSwidth, LSheight);
}


//图像预处理第1步：将256色图像转化为灰度图像
BOOL CCRView::OnIMGPRC256ToGray() 
{	
	if(m_hDIB == NULL)
	{
		MessageBox("请打开图片后再操作，只显示图片预处理效果","系统提示",MB_OK);
		return FALSE;
	}
	if(IsEmpty(m_hDIB))
	{
		AfxMessageBox("预处理第一步出错，请返回");
		return FALSE;
	}
	if(m_nCurType != PICTURE)return TRUE;
	Convert256toGray(m_hDIB);	
	//在屏幕上显示位图
	if(m_nCurType == PICTURE)
	{
    	CDC* pDC=GetDC();
    	DisplayDIB(pDC,m_hDIB);	
	}

	return TRUE;
}

//图像预处理第2步：将灰度图二值化
BOOL CCRView::OnIMGPRCGrayToWhiteBlack()
{
	if(m_hDIB == NULL)
	{
		MessageBox("请打开图片后再操作，只显示图片预处理效果","系统提示",MB_OK);
		return FALSE;
	}
	ConvertGrayToWhiteBlack(m_hDIB);
	if(m_nCurType != PICTURE)return TRUE;
	if(IsEmpty(m_hDIB))
	{
		AfxMessageBox("预处理第二步出错，请返回");
		return FALSE;
	}
	//在屏幕上显示位图
	if(m_nCurType == PICTURE)
	{
    	CDC* pDC=GetDC();
    	DisplayDIB(pDC,m_hDIB);	
	}

	return TRUE;
}

//图像预处理第3步：梯度锐化
BOOL CCRView::OnImgprcSharp() 
{
	if(m_hDIB == NULL)
	{
		MessageBox("请打开图片后再操作，只显示图片预处理效果","系统提示",MB_OK);
		return FALSE;
	}
	if(m_nCurType != PICTURE)return TRUE;
	GradientSharp(m_hDIB);
	if(IsEmpty(m_hDIB))
	{
		AfxMessageBox("预处理第三步出错，请返回");
		return FALSE;
	}
	//在屏幕上显示位图
	if(m_nCurType == PICTURE)
	{
    	CDC* pDC=GetDC();
    	DisplayDIB(pDC,m_hDIB);	
	}

	return TRUE;
}

//图像预处理第4步：去离散杂点噪声
BOOL CCRView::OnImgprcRemoveNoise() 
{
	if(m_hDIB == NULL)
	{
		MessageBox("请打开图片后再操作，只显示图片预处理效果","系统提示",MB_OK);
		return FALSE;
	}
	if(m_nCurType == MATRIX)return TRUE;
	RemoveScatterNoise(m_hDIB);
	if(IsEmpty(m_hDIB))
	{
		AfxMessageBox("预处理第四步出错，请返回");
		return FALSE;
	}
	//在屏幕上显示位图
	if(m_nCurType == PICTURE)
	{
    	CDC* pDC=GetDC();
    	DisplayDIB(pDC,m_hDIB);	
	}

	return TRUE;
}

//图像预处理第5步：倾斜度调整
BOOL CCRView::OnImgprcAdjustSlope() 
{
	if(m_hDIB == NULL)
	{
		MessageBox("请打开图片后再操作，只显示图片预处理效果","系统提示",MB_OK);
		return FALSE;
	}
	if(m_nCurType != PICTURE)return TRUE;
    SlopeAdjust(m_hDIB);
	if(IsEmpty(m_hDIB))
	{
		AfxMessageBox("预处理第五步出错，请返回");
		return FALSE;
	}
	//在屏幕上显示位图
	if(m_nCurType == PICTURE)
	{
    	CDC* pDC=GetDC();
    	DisplayDIB(pDC,m_hDIB);	
	}

	return TRUE;
}

//图像预处理第6步：分割，并在分割出来的字符外面画框以标识
BOOL CCRView::OnImgprcDivide() 
{
	if(m_hDIB == NULL)
	{
		MessageBox("请打开图片后再操作，只显示图片预处理效果","系统提示",MB_OK);
		return FALSE;
	}
	if(m_nCurType == PICTURE)
		m_charRect=CharSegment(m_hDIB, TRUE);
	else
		m_charRect=CharSegment(m_hDIB, FALSE);
	
	if(IsEmpty(m_hDIB))
	{
		AfxMessageBox("预处理第六步出错，请返回");
		return FALSE;
	}

	//在屏幕上显示位图
	if(m_nCurType == PICTURE)
	{
    	CDC* pDC=GetDC();
    	DisplayDIB(pDC,m_hDIB);	
    	DrawFrame(pDC,m_hDIB,m_charRect,2,RGB(20,60,200));
	}
	return TRUE;
}

//图像预处理第7步：标准归一化
//将分割出来的各个不同宽、高的数字字符宽、高统一
BOOL CCRView::OnImgprcStandarize() 
{
	if(m_hDIB == NULL)
	{
		MessageBox("请打开图片后再操作，只显示图片预处理效果","系统提示",MB_OK);
		return FALSE;
	}
	if(m_nCurType == PICTURE)
		StdDIBbyRect(m_hDIB,w_sample,h_sample,TRUE);
	else
		StdDIBbyRect(m_hDIB,w_sample,h_sample,FALSE);
	if(IsEmpty(m_hDIB))
	{
		AfxMessageBox("预处理第七步出错，请返回");
		return FALSE;
	}

	//在屏幕上显示位图
	if(m_nCurType == PICTURE)
	{
    	CDC* pDC=GetDC();
    	DisplayDIB(pDC,m_hDIB);	
    	DrawFrame(pDC,m_hDIB,m_charRect,2,RGB(21,255,25));
	}
	gyhfinished=true;
	return TRUE;
}

//图像预处理第8步：紧缩重排已经分割完毕的数字字符，并形成新的位图句柄
BOOL CCRView::OnImgprcShrinkAlign() 
{
	if(m_hDIB == NULL)
	{
		MessageBox("请打开图片后再操作，只显示图片预处理效果","系统提示",MB_OK);
		return FALSE;
	}
	m_hDIB=AutoAlign(m_hDIB);
	if(IsEmpty(m_hDIB))
	{
		AfxMessageBox("预处理第八步出错，请返回");
		return FALSE;
	}
	
	//在屏幕上显示位图
	if(m_nCurType == PICTURE)
	{
    	CDC* pDC=GetDC();
    	DisplayDIB(pDC,m_hDIB);	
	    DrawFrame(pDC,m_hDIB,m_charRect,1,RGB(252,115,27));
	}

	return TRUE;
}

	
//将当前DIB中的内容输出到点阵中去
void CCRView::ShowPicInMatrix(HDIB hDIB)
{
	LPSTR lpDIB=(LPSTR) ::GlobalLock((HGLOBAL)hDIB);
	LPSTR    lpDIBBits;	
	lpDIBBits = ::FindDIBBits(lpDIB);
	unsigned char*	lpSrc;
	LONG	i;
	LONG	j;
	LONG	lLineBytes;
	LONG    lWidth;
	LONG	lHeight;
	//获取图像的长度和宽度
	lWidth=::DIBWidth ((char*)lpDIB);
	lHeight=::DIBHeight ((char*)lpDIB);
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	LONG counts=0;
	for (i=0;i<lHeight;i++)
	{   
		for (j=0;j<lWidth;j++)
		{
			//指向第i行第j个象素的指针	
		    lpSrc=(unsigned char*)lpDIBBits + lLineBytes *  i + j;
		    if (*lpSrc == 0)
			{
				m_pMatrix->SetPointMatrix(lHeight-1-i,j,1);
				m_pMatrix->Invalidate(TRUE);
			}

		}
	}
	return;
}

/*图片相关代码结束///////////////////////////////////////////////////////////////////////*/






/*显示多个图片代码////////////////////////////////////////////////////////////////////////////*/
void CCRView::ShowMultiMatrixs(ResetDisplayMessageStruct* display)
{
	//判断参数是否合理
	if(display->num > MULTI_MATRIX_SIZE)
	{
		MessageBox("系统无法支持显示如此多的点阵，但并不影响功能，请继续训练并见谅","系统提示",MB_OK);
		return;
	}

	HideMultiMatrixs();
	DeleteItems(MULTIMATRIX);
//	m_nCurType = MULTIMATRIX;

	//得到画图区域
	CRect rect;
	CRect MatrixRects[MULTI_MATRIX_SIZE];
	
	GetClientRect(&rect);
	int LeftDist = 20, RightDist = 20, TopDist = 20, BotomDist = 20;
    int MatrixWidth = 80, MatrixHeight = 80;

	rect.left += LeftDist; rect.right -= RightDist; rect.top += TopDist; rect.bottom -= BotomDist;
  
	int SegmentWidth = 20;//横向的间隔
	int SegmentHeight = 20;//纵向的间隔

	//指定每个点阵的大小和位置
	for(int i=0;i<MULTI_MATRIX_SIZE;i++)
	{
		MatrixRects[i].left = rect.left + i%10*(SegmentWidth+MatrixWidth);
		MatrixRects[i].right = MatrixRects[i].left + MatrixWidth;
		MatrixRects[i].top = i/10*(SegmentHeight+MatrixHeight) + rect.top;
		MatrixRects[i].bottom = MatrixRects[i].top + MatrixHeight;
		
	}
	static UINT CurMatrixID = 7777;

	//创建或显示点阵
	int pointsize = display->width>display->height?(MatrixWidth-6)/display->width:(MatrixHeight-6)/display->height;

	for(i=0;i<display->num;i++)
	{
		//如果当前的指针为空，则新建一个点阵对象
		if(m_pMultiMatrixs[i] == NULL)
		{
	        m_pMultiMatrixs[i] = new CPointMatrix(display->height,display->width,pointsize,RGB(128,0,128),RGB(211,211,211));
	        m_pMultiMatrixs[i]->Create("this",WS_SIZEBOX|SS_ETCHEDFRAME|SS_SUNKEN|SS_WHITEFRAME,MatrixRects[i],this,CurMatrixID++);
		}
		//显示
		//for(int j=0;j<display->height;j++)
			//for(int k=0;k<display->width;k++)
				//m_pMultiMatrixs[i]->SetPointMatrix(j,k,(UINT)display->data[i][(display->height-1-j)*display->width+k]);
        else
		{
			m_pMultiMatrixs[i]->InitializeMatrix(/*(int*)(display->data[i]),*/display->height,display->width);
		    m_pMultiMatrixs[i]->SetPointSize(pointsize);
		}
				
		if(m_pMultiMatrixs[i] != NULL)
		{
			m_pMultiMatrixs[i]->UpdateWindow();
	        m_pMultiMatrixs[i]->ShowWindow(SW_SHOW);
		}

		//显示
		if(display->data)
		{
			for(int j=0;j<display->height;j++)
				for(int k=0;k<display->width;k++)
					m_pMultiMatrixs[i]->SetPointMatrix(j,k,(UINT)display->data[i][(display->height-1-j)*display->width+k]);
		}
		else if(display->resdata)
		{
			if(display->resdata[i] < 0 || display->resdata[i] > 7)display->resdata[i] = 8;
			for(int j=0;j<8;j++)
				for(int k=0;k<8;k++)
					m_pMultiMatrixs[i]->SetPointMatrix(j,k,CR[display->resdata[i]][j][k]);
		}


	}
	Invalidate(TRUE);
}

void CCRView::HideMultiMatrixs()
{
	for(int i=0;i<MULTI_MATRIX_SIZE;i++)
	{
		if(m_pMultiMatrixs[i] != NULL)
		{
			m_pMultiMatrixs[i]->ResetMatrix();
			m_pMultiMatrixs[i]->ShowWindow(SW_HIDE);
		}
		else
			return;
	}
	Invalidate(TRUE);
}
/*显示多个图片代码结束*/







/*其它命令操作代码//////////////////////////////////////////////////////////////////////////////////////*/

void CCRView::ResultShow(BOOL bShow)
{
	if(bShow)
	{
		if(m_pDisplayWindow){m_pDisplayWindow->ShowWindow(SW_SHOW);return;}
		m_pDisplayWindow = new CPointMatrixWindow;
        m_pDisplayWindow->Create(IDD_DIALOG_POINTMATRIX,NULL);
        m_pDisplayWindow->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pDisplayWindow)
		{
		//	m_pDisplayWindow->CloseWindow();
		//  delete m_pDisplayWindow;
		//  m_pDisplayWindow = NULL;
			m_pDisplayWindow->ShowWindow(SW_HIDE);
		}
	}
}



void CCRView::OnShowresult() 
{
	// TODO: Add your command handler code here
	static bool bShow = FALSE;
	ResultShow(!bShow);
	bShow = !bShow;
}



void CCRView::OnInputMatrix() 
{
	// TODO: Add your command handler code here
	
	//如果点阵已经存在就直接退出
	if(m_pMatrix != NULL)
	{
		m_pMatrix->ShowWindow(SW_SHOW);
		m_pMatrix->ResetMatrix();
		return;
	}

	//显示点阵
	DeleteItems(MATRIX);
	m_nCurType = MATRIX;
	m_nSampleCurType = NONE;
  	HideMultiMatrixs();
	ShowMatrix(8,8);
	OnCtrlInput();
}

void CCRView::OnInputHandPat() 
{
	// TODO: Add your command handler code here
	//如果手写板已经存在就直接退出
	if(m_pHandWritingPat != NULL)
	{
//		m_pHandWritingPat->ShowWindow(SW_SHOW);
		m_pHandWritingPat->ResetPointArray();
		Invalidate(TRUE);
		return;
	}
	DeleteItems(HANDWRITINGPAT);
	m_nCurType = HANDWRITINGPAT;
	m_nSampleCurType = NONE;
	HideMultiMatrixs();
	ShowHandWritingPat();
	OnCtrlInput();
}


void CCRView::OnInputPicture()
{
	// TODO: 在此添加命令处理程序代码
	DeleteItems(PICTURE);
	m_nCurType = PICTURE;
	m_nSampleCurType = NONE;
	//显示图片
	OnCtrlInput();
	HideMultiMatrixs();
	ShowPicture();
	Invalidate();
}


void CCRView::OnPreproAll() 
{
	// TODO: Add your command handler code here
	CFrameWnd *pFrameWnd=(CFrameWnd*)AfxGetMainWnd();
	CDialogInput* pIn=(CDialogInput*)((CMainFrame*)(pFrameWnd->GetActiveFrame()))->GetDialogIn();
	pIn->DoPreprocess();
}

void CCRView::OnBpnetTrain() 
{	
	OnCtrlBpnet();
	CFrameWnd *pFrameWnd=(CFrameWnd*)AfxGetMainWnd();
	CDialogBpnet* pBp=(CDialogBpnet*)((CMainFrame*)(pFrameWnd->GetActiveFrame()))->GetDialogBp();
    pBp->DoTrain();
}


void CCRView::OnAddSample() 
{
	// TODO: Add your command handler code here
	MessageBox("请确保已经运行过预处理操作，然后在输出控制板的样本管理栏中填入样本信息","系统提示",MB_OK);
    OnCtrlOutput();
}

void CCRView::OnCtrlInput() 
{
	// TODO: Add your command handler code here
	CFrameWnd *pFrameWnd=(CFrameWnd*)AfxGetMainWnd();
	CMainFrame* pMainFrm = (CMainFrame*)(pFrameWnd->GetActiveFrame());
	pMainFrm->SetActiveCtrlPat(0);

}
void CCRView::OnCtrlBpnet() 
{
	// TODO: Add your command handler code here
	CFrameWnd *pFrameWnd=(CFrameWnd*)AfxGetMainWnd();
	CMainFrame* pMainFrm = (CMainFrame*)(pFrameWnd->GetActiveFrame());
	pMainFrm->SetActiveCtrlPat(1);	
}

void CCRView::OnCtrlOutput() 
{
	// TODO: Add your command handler code here
	CFrameWnd *pFrameWnd=(CFrameWnd*)AfxGetMainWnd();
	CMainFrame* pMainFrm = (CMainFrame*)(pFrameWnd->GetActiveFrame());
	pMainFrm->SetActiveCtrlPat(2);
}

/*其它命令操作代码结束//////////////////////////////////////////////////////////////////////////////////*/















/*神经网络算法////////////////////////////////////////////////////////////////////*/

void CCRView::BpTrain(int combp, double momentum, double min_ex, int n_hidden, double eta, double addrate, double subrate, double maxerror)
{
	BYTE *lpDIB, *lpDIBBits;
	int numColors = 0;
	LONG lWidth = 0; 
	LONG lHeight = 0;
	LONG lLineByte = 0; 
	LONG lSwidth = 0;
	LONG LSheight = 0;

	double **data_in = NULL;
	int n_in;
	int dbarraysize;
	int *arrayvalue = NULL;

	if(m_nCurType == PICTURE)
	{
		OnPreProcess(w_sample,h_sample);

	    //判断是否经过了归一划的处理
	    if(gyhfinished==false)
		{
			::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
	     	return;
		}
		
	    //获得指向DIB的指针
	    lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIB);
	    lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	    numColors=(int) ::DIBNumColors((char *)lpDIB);
        if (numColors!=256) 
		{
			::GlobalUnlock((HGLOBAL) m_hDIB);
		    ::MessageBox(NULL,"只能处理灰度图像",NULL,MB_ICONSTOP);
		    return;
		}

		lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	    lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	    lLineByte = (lWidth+3)/4*4; 
	    lSwidth = w_sample;
	    LSheight = h_sample;

		data_in = code ( lpDIBBits, digicount,  lLineByte, lSwidth, LSheight);
	    n_in = LSheight*lSwidth;

		m_nSampleLastType = PICTURE;
	}
    else if(m_nSampleCurType == MATRIX || m_nSampleCurType == HANDWRITINGPAT)
	{
		data_in = m_SampleList.GetDataFromSampleList(m_nSampleCurType,w_sample,h_sample,dbarraysize,arrayvalue);
	    if(data_in == NULL)
		{
			MessageBox("当前训练样本为空，请及时补充","系统提示",MB_OK);
		    if(arrayvalue)
			{
				delete arrayvalue;
		        arrayvalue = NULL;
			}
			return;
		}
		n_in = w_sample * h_sample;
		m_nSampleLastType = m_nSampleCurType;
		digicount = dbarraysize;
	}
	else
	{
		MessageBox("请选择一种输入类型进行训练","系统提示",MB_OK);
		return;
	}
		
	double **data_out;
	data_out = createDoubleArray2(digicount,3);

	if(m_nCurType == PICTURE)
	{
		for(int i=0;i<digicount;i++)
		{
			for(int j=0;j<3;j++)
				data_out[i][j]=g_ArrayOut[i%8][j];	
		}
	}
    else if(m_nSampleCurType == MATRIX || m_nSampleCurType == HANDWRITINGPAT)
	{
		if(!arrayvalue)
		{
			AfxMessageBox("无法得到当前样本对应的输出值，无法完成训练");
			return;
		}
	    for(int i=0;i<digicount;i++)
		{
			int curnum = FindCharPosInString(Global.m_szOutput,arrayvalue[i]);
			if(curnum == -1)
			{
				AfxMessageBox("样本库中有错误的样本值，无法完成训练");
				return;
			}
			for(int j=0;j<3;j++)
				data_out[i][j]=g_ArrayOut[curnum][j];
		}
		if(arrayvalue)
		{
			delete arrayvalue;
		    arrayvalue = NULL;
		}
	}

	//建立BPPNBase类的对象
	int n_out=3;
	BPNNBase *bpnet=new BPNNBase(n_in,n_hidden,n_out);

	//建立BPPNRec类的对象
	int max_count=15000;

	BPNNRec *bprec = NULL;
	double *err = NULL;
	int errNum = 0;
	DWORD dwStart = GetTickCount();
	if(combp == 1)
	{
		bprec=new BPNNRec(bpnet,min_ex,0,eta,max_count);
		bprec->BpTrain(data_in,data_out,digicount,err,&errNum);
		delete bprec;
	}
	else if(combp == 2)
	{
		bprec=new BPNNRec(bpnet,min_ex,momentum,eta,max_count);
		bprec->BpTrain(data_in,data_out,digicount,err,&errNum);
		delete bprec;
	}
	else
	{
		bprec=new BPNNRec(bpnet,min_ex,momentum,eta,max_count,addrate,subrate,maxerror);		
		bprec->LWBpTrain(data_in,data_out,digicount,err,&errNum);
		delete bprec;
	}
    dwStart = GetTickCount() - dwStart;

	//把误差分析结果送输出模块
	CFrameWnd *pFrameWnd=(CFrameWnd*)AfxGetMainWnd();
	CDialogOutput* pOut=(CDialogOutput*)((CMainFrame*)(pFrameWnd->GetActiveFrame()))->GetDialogOut();
	pOut->SetErrResult(err,errNum,dwStart);
	pOut->Invalidate();

	if(err)free(err);
    freeDoubleArray(m_data,/*digicount*/m_datalen,m_nWidth*m_nHeight);

	OnCtrlOutput();
	::GlobalUnlock(m_hDIB);
}


void CCRView::OnBpnetRecognize() 
{
	// TODO: Add your command handler code here
	OnPreProcess(w_sample,h_sample);
	
	//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	}

	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIB);	
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		::MessageBox(NULL,"只能处理256色图像",NULL,MB_ICONSTOP);
		return;
	}
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	LONG lLineByte = (lWidth+3)/4*4; 
	LONG lSwidth = w_sample;
	LONG LSheight = h_sample;

	//建立BPPNBase类的对象	
	BPNNBase *bpnet=new BPNNBase();
	BPNNRec *bprec=new BPNNRec(bpnet);
	if(bprec->readNum("num")==false)
		return;

	if(bprec->getBP()->getInputNum()!=lSwidth * LSheight)
	{
		::MessageBox(NULL,"归一划尺寸与上一次训练时不一致",NULL,MB_ICONSTOP);
		return;
	}
	if(m_nCurType != m_nSampleLastType)
	{
		::MessageBox(NULL,"输入类型与上一次训练时不一致",NULL,MB_ICONSTOP);
		return;
	}
	
	double **data_in;
	data_in = code ( lpDIBBits, digicount,  lLineByte, lSwidth, LSheight);

	int *recognize_result= NULL;

	bprec->BpRecognize(data_in,digicount,recognize_result);

	//把结果送输出模块
	ResetDisplayMessageStruct display;
	display.nPreprocess = 2;
	display.num = digicount;
	display.width = 8;
	display.height = 8;
	display.resdata = recognize_result;
	display.data = NULL;
	if(digicount > 1)
	{
		ResultShow(FALSE);
		ShowMultiMatrixs(&display);
	}
	else if(digicount == 1)
	{
		CFrameWnd *pFrameWnd=(CFrameWnd*)AfxGetMainWnd();
	    CDialogOutput* pOut=(CDialogOutput*)((CMainFrame*)(pFrameWnd->GetActiveFrame()))->GetDialogOut();
	    pOut->SetShowResult(&display);
	    pOut->Invalidate();
		if(m_pDisplayWindow)
		{
			ResultShow(TRUE);
			m_pDisplayWindow->SendMessage(WM_USER_RESET_DISPLAY,0,(LPARAM)&display);
		}
	}
 	//delete recognize_result;
	OnCtrlOutput();
	::GlobalUnlock(m_hDIB);	
	delete bprec;
}

/*神经网络算法结束/////////////////////////////////////////////////////////////////*/




/*样本添加相关代码//////////////////////////////////////////////////////////////////*/
void  CCRView::GetSampleInfo(double**& data, int& width, int& height, int& num, int& type)
{
	//预处理
	OnPreproAll();

	//得到相关值
	if(m_data != NULL && digicount == 1)
	{
		data = m_data;
		width = m_nDataWidth;
		height = m_nDataHeight;
		type = m_nCurType;
		num = digicount;
	}
	else
		data = NULL;
}

void CCRView::ShowSample()
{
	double** data_in;
	int dbarraysize;
	int* arrayvalue = NULL;
	data_in = m_SampleList.GetDataFromSampleList(m_nSampleCurType,w_sample,h_sample,dbarraysize,arrayvalue);
    if(data_in == NULL)
	{
		MessageBox("当前训练样本为空，请及时补充","系统提示",MB_OK);
		return;
	}

	ResultShow(FALSE);
    ResetDisplayMessageStruct display;
    display.nPreprocess = 3;
    display.num = dbarraysize;
    display.width = w_sample;
    display.height = h_sample;
    display.data = data_in;
	ShowMultiMatrixs(&display);
	if(arrayvalue)
	{
		delete arrayvalue;
		arrayvalue = NULL;
	}
}
void CCRView::OnMatrix88() 
{
	// TODO: Add your command handler code here
	w_sample = 8;
	h_sample = 8;
	m_nSampleCurType = MATRIX;
	ShowSample();
}
void CCRView::OnMatrix816() 
{
	// TODO: Add your command handler code here
	w_sample = 8;
	h_sample = 16;
	m_nSampleCurType = MATRIX;
	ShowSample();
}
void CCRView::OnMatrix832() 
{
	// TODO: Add your command handler code here
	w_sample = 8;
	h_sample = 32;
	m_nSampleCurType = MATRIX;
	ShowSample();
}
void CCRView::OnMatrix168() 
{
	// TODO: Add your command handler code here
	w_sample = 16;
	h_sample = 8;
	m_nSampleCurType = MATRIX;
	ShowSample();
}void CCRView::OnMatrix1616() 
{
	// TODO: Add your command handler code here
	w_sample = 16;
	h_sample = 16;
	m_nSampleCurType = MATRIX;
	ShowSample();
}void CCRView::OnMatrix1632() 
{
	// TODO: Add your command handler code here
	w_sample = 16;
	h_sample = 32;
	m_nSampleCurType = MATRIX;
	ShowSample();
}void CCRView::OnMatrix328() 
{
	// TODO: Add your command handler code here
	w_sample = 32;
	h_sample = 8;
	m_nSampleCurType = MATRIX;
	ShowSample();
}
void CCRView::OnMatrix3216() 
{
	// TODO: Add your command handler code here
	w_sample = 32;
	h_sample = 16;
	m_nSampleCurType = MATRIX;
	ShowSample();
}
void CCRView::OnMatrix3232() 
{
	// TODO: Add your command handler code here
	w_sample = 32;
	h_sample = 32;
	m_nSampleCurType = MATRIX;
	ShowSample();
}
void CCRView::OnHandPat88() 
{
	// TODO: Add your command handler code here
	w_sample = 8;
	h_sample = 8;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnHandPat816() 
{
	// TODO: Add your command handler code here
	w_sample = 8;
	h_sample = 16;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnHandPat832() 
{
	// TODO: Add your command handler code here
	w_sample = 8;
	h_sample = 32;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnHandPat168() 
{
	// TODO: Add your command handler code here
	w_sample = 16;
	h_sample = 8;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnHandPat1616() 
{
	// TODO: Add your command handler code here
	w_sample = 16;
	h_sample = 16;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnHandPat1632() 
{
	// TODO: Add your command handler code here
	w_sample = 16;
	h_sample = 32;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnHandPat328() 
{
	// TODO: Add your command handler code here
	w_sample = 32;
	h_sample = 8;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnHandPat3216() 
{
	// TODO: Add your command handler code here
	w_sample = 32;
	h_sample = 16;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnHandPat3232() 
{
	// TODO: Add your command handler code here
	w_sample = 32;
	h_sample = 32;
	m_nSampleCurType = HANDWRITINGPAT;
	ShowSample();
}
void CCRView::OnPictureOpen() 
{
	// TODO: Add your command handler code here
	w_sample = 8;
	h_sample = 16;
	m_nSampleCurType = PICTURE;
	ShowPicture();
}

void CCRView::OnSampleSave() 
{
	// TODO: Add your command handler code here
	m_SampleList.SaveSampleToFile(SAMPLE_FILE_NAME);
	MessageBox("样本保存成功","系统提示",MB_OK);
}

/*样本添加结束/////////////////////////////////////////////////////////////////////*/



