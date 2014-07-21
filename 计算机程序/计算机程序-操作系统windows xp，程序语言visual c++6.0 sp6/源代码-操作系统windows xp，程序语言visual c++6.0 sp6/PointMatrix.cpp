// PointMatrix.cpp : implementation file
//

#include "stdafx.h"
#include "PointMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPointMatrix
//CPointMatrix::CPointMatrix()
//{
//	m_PointMatrix = NULL;
//}
	
CPointMatrix::CPointMatrix(int** PointMatrix, int rows, int cols, int PointSize, COLORREF PointColor, COLORREF BackColor)
{
	m_PointMatrix = NULL;
	InitializeCriticalSection(&cs_DrawPoint);
	InitializeMatrix(PointMatrix, rows, cols);
	SetPointSize(PointSize);
	SetPointColor(PointColor);
	SetBackColor(BackColor);
}

CPointMatrix::CPointMatrix(int rows, int cols, int PointSize, COLORREF PointColor, COLORREF BackColor)
{
	m_PointMatrix = NULL;
	InitializeCriticalSection(&cs_DrawPoint);
	InitializeMatrix(rows, cols);
	SetPointSize(PointSize);
	SetPointColor(PointColor);
	SetBackColor(BackColor);
}

CPointMatrix::~CPointMatrix()
{
	if(m_PointMatrix != NULL)
	{
		for(int i=0;i<m_nRow;i++)
		{
			if(m_PointMatrix[i] != NULL)
				/*delete*/free(m_PointMatrix[i]);
		}
	    /*delete */free(m_PointMatrix);
	}
}

void CPointMatrix::InitializeMatrix(int** matrix, int rows, int cols)
{
	int i,j;

	//申请空间
	if(!InitializeMatrix(rows, cols))
		return;

	//赋值
	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
			m_PointMatrix[i][j] = matrix[i][j];
    return;	
}

void CPointMatrix::InitializeMatrix(int* matrix, int rows, int cols)
{
	int i,j;

	//申请空间
	if(!InitializeMatrix(rows, cols))
		return;

	//赋值
	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
			m_PointMatrix[i][j] = *(matrix+i*cols+j);
    return;	
}

BOOL CPointMatrix::InitializeMatrix(int row, int col)
{
	int i,j;
	
	//如果数组中已经有值就将其删除
	EnterCriticalSection(&cs_DrawPoint);
	if(m_PointMatrix != NULL)
	{
		for(i=0;i<m_nRow;i++)
		{
			if(m_PointMatrix[i] != NULL)
				/*delete */free(m_PointMatrix[i]);
		}
	    /*delete */free(m_PointMatrix);
	}
	
	//申请二维空间 
	m_nRow = row;
	m_nCol = col;
	m_PointMatrix = (int **) malloc ((unsigned) (row * sizeof (int *)));
	if (m_PointMatrix == NULL) 
	{
		AfxMessageBox("内存不足，无法生成点阵");
		return FALSE;
	}
	//申请一维空间
	for (i = 0; i < row; i++)
	{
		m_PointMatrix[i] = (int *) malloc ((unsigned) (col * sizeof (int)));
		if(m_PointMatrix[i] == NULL)
		{
			AfxMessageBox("内存不足，无法生成点阵");
			return FALSE;
		}
	}
	
	//赋值,初始化为0
	for(i = 0; i < row; i++)
		for(j = 0; j < col; j++)
			m_PointMatrix[i][j] = 0;

	LeaveCriticalSection(&cs_DrawPoint);

	return TRUE;
}

void CPointMatrix::SetPointMatrix(int** matrix, int rows, int cols)
{
	int i,j;
	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
			SetPointMatrix(i, j, matrix[i][j]);
}

void CPointMatrix::SetPointMatrix(int row, int col, UINT value)
{
	if(row < 0 || col < 0 || row >= m_nRow || col >= m_nCol)
		return;
	if(m_PointMatrix)
		m_PointMatrix[row][col] = value;
}


void CPointMatrix::SetPointMatrix(int row, int col, BOOL bChange)
{
	if(row < 0 || col < 0 || row >= m_nRow || col >= m_nCol)
		return;
	if(m_PointMatrix)
		if(bChange)
			m_PointMatrix[row][col] = !m_PointMatrix[row][col];
}

void CPointMatrix::PrintMatrix()
{
	FILE* file = fopen("C:\\matrix.txt","ab+");
	char* total = new char[1024];
	memset(total,0,1024);
	strcpy(total,"本次测验结果: \n \n");
	char temp[3] = {0};
	for(int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			memset(temp,0,3);
			sprintf(temp,"%d",m_PointMatrix[i][j]);
			strcat(total,temp);
			strcat(total,"  ");
		}
	    strcat(total," \n");
	}
	fwrite(total,strlen(total),sizeof(char),file);
	fclose(file);
	delete total;
}

void CPointMatrix::ResetMatrix()
{
	for(int i=0;i<m_nRow;i++)
		for(int j=0;j<m_nCol;j++)
			SetPointMatrix(i,j,(UINT)0);
	Invalidate(TRUE);
}
	
void CPointMatrix::RemoveNoiseForMatrix(int mValue)
{
	for(int i=0;i<m_nRow;i++)
		for(int j=0;j<m_nCol;j++)
		{
			if(m_PointMatrix[i][j] == 1)
			{
				BOOL bRemove = TRUE;
				for(int t=1;t<=mValue;t++)
				{
					if(i+t < m_nRow && m_PointMatrix[i+t][j] == 1
				   ||  i-t >= 0  &&  m_PointMatrix[i-t][j] == 1
				   ||  j+t < m_nCol && m_PointMatrix[i][j+t] == 1
				   ||  j-t >= 0  &&  m_PointMatrix[i][j-t] == 1
				   ||  i+t < m_nRow && j+t < m_nCol && m_PointMatrix[i+t][j+t] == 1
				   ||  i-t >= 0 && j-t >= 0 && m_PointMatrix[i-t][j-t] == 1
				   ||  i+t < m_nRow && j-t >= 0 && m_PointMatrix[i+t][j-t] == 1
				   ||  i-t >= 0 && j+t < m_nCol && m_PointMatrix[i-t][j+t] == 1)
					{
						bRemove = FALSE;
						break;
					}
				}
				if(bRemove)
					m_PointMatrix[i][j] = 0;
			}
		}
}


/*////////////////////////////////////////////////////////
//函数功能：根据行列坐标设置点阵
//参数：
//    row 为点的横坐标
//    col 为点的纵坐标
//返回值：如果已经被设置为1则返回FALSE，否则返回TRUE
*//////////////////////////////////////////////////////////
BOOL CPointMatrix::SetPointMatrix(int row, int col)
{
	if(row < 0 || col < 0 || row >= m_nRow || col >= m_nCol)
		return FALSE;
	if(m_PointMatrix)
	{
		if(m_PointMatrix[row][col] == 1)
			return FALSE;
		else 
			m_PointMatrix[row][col] = 1;
	}
	else return FALSE;
	return TRUE;		
}


BEGIN_MESSAGE_MAP(CPointMatrix, CStatic)
	//{{AFX_MSG_MAP(CPointMatrix)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER_SETPOINT, OnSetPointMessage)
	ON_MESSAGE(WM_USER_SETMOVEPOINT, OnSetMovePointMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointMatrix message handlers


void CPointMatrix::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	GetClientRect(&m_ClientRect);
//	m_ClientRect.left += DEVI_MATRIX_LEFT;
//	m_ClientRect.right += DEVI_MATRIX_LEFT;
//	m_ClientRect.top += DEVI_MATRIX_TOP;
//	m_ClientRect.bottom += DEVI_MATRIX_TOP;
	if(m_PointSize * m_nCol > m_ClientRect.Width()  ||  m_PointSize * m_nRow > m_ClientRect.Height())
	{
		MessageBox("设置错误，请减少点的大小或增加点阵窗口的大小","设置错误",MB_OK);
		return;
	}
	else
	{
		m_MatrixRect.left = m_ClientRect.left + (m_ClientRect.Width() - m_PointSize * m_nCol)/2;
		m_MatrixRect.top = m_ClientRect.top + (m_ClientRect.Height() - m_PointSize * m_nRow)/2;
	    m_MatrixRect.right = m_MatrixRect.left + m_PointSize * m_nCol;
		m_MatrixRect.bottom = m_MatrixRect.top + m_PointSize * m_nRow;
	}
	EnterCriticalSection(&cs_DrawPoint);
	DrawMatrix();
	LeaveCriticalSection(&cs_DrawPoint);
}



BOOL CPointMatrix::OnEraseBkgnd(CDC* pDC) 
{	
	CRect rect; 
	GetClientRect(&rect);
//	ScreenToClient(&rect);
    CBrush brush; 
	brush.CreateSolidBrush(m_BackColor); 
//	pDC->GetClipBox(rect); 
	pDC->FillRect(rect,&brush); 

	return TRUE;
//	return CStatic::OnEraseBkgnd(pDC);
}



/*////////////////////////////////////////////////////////
//函数功能：画点
//参数：
//    Point 为点的坐标
//    value 为点的值
//返回值：空
*//////////////////////////////////////////////////////////
void CPointMatrix::DrawPoint(CPoint Point, int value)
{
   CClientDC dc(this);
   CPen pen;
   pen.CreatePen (PS_NULL,1,RGB(0,0,0));
   dc.SelectObject (&pen);
   if(value == 0)
   {
	   CBrush brush(RGB(255,255,255));
	   dc.SelectObject (&brush);
       dc.Rectangle (Point.x-m_PointSize/2,Point.y -m_PointSize/2,Point.x +m_PointSize/2,Point.y +m_PointSize/2);
   }
   else if(value == 1)
   {
	   CBrush brush(m_PointColor);
       dc.SelectObject (&brush);
	   dc.Rectangle (Point.x-m_PointSize/2,Point.y -m_PointSize/2,Point.x +m_PointSize/2,Point.y +m_PointSize/2);
   }
}


/*////////////////////////////////////////////////////////
//函数功能：画出点阵
//参数：
//返回值：空
*//////////////////////////////////////////////////////////
void CPointMatrix::DrawMatrix()
{
	CPoint Point, StartPoint;
	
	//在客户区域空间开始画点
	StartPoint.x = m_MatrixRect.left+ m_PointSize/2;  
	StartPoint.y = m_MatrixRect.top+ m_PointSize/2;
	
	int i,j;
	for(i=0;i<m_nRow;i++)
	{  
		for(j=0;j<m_nCol;j++)
	   {
		   Point.x =StartPoint.x +j*m_PointSize;
		   Point.y =StartPoint.y +i*m_PointSize;
		   DrawPoint(Point, m_PointMatrix[i][j]);
	   }
	}
}

/*////////////////////////////////////////////////////////
//函数功能：响应视图消息，设置点阵点击变化
//参数：
//    wParam 为点的横坐标
//    lParam 为点的纵坐标
//返回值：0
*//////////////////////////////////////////////////////////
LRESULT CPointMatrix::OnSetPointMessage(WPARAM wParam, LPARAM lParam)
{
	//算出点击处距离点阵左上角的相对距离
	int x = (int)wParam - DEVI_LEFT_MATRIX_HANDWRITING - (m_MatrixRect.left - m_ClientRect.left) - MATRIX_BORDER_WIDTH;
	int y = (int)lParam - DEVI_TOP_MATRIX_HANDWRITING - (m_MatrixRect.top - m_ClientRect.top) - MATRIX_BORDER_WIDTH;

	if(x < 0 || y < 0 || x > m_nCol * m_PointSize || y > m_nRow * m_PointSize)
		return 0;
	else
	{
		SetPointMatrix(y/m_PointSize,x/m_PointSize,TRUE);
	}
	return 0;
}

/*////////////////////////////////////////////////////////
//函数功能：响应视图消息，设置点阵根据鼠标拖动变化
//参数：
//    wParam 为点的横坐标
//    lParam 为点的纵坐标
//返回值：0
*//////////////////////////////////////////////////////////
LRESULT CPointMatrix::OnSetMovePointMessage(WPARAM wParam, LPARAM lParam)
{
	//算出点击处距离点阵左上角的相对距离
	int x = (int)wParam - DEVI_LEFT_MATRIX_HANDWRITING - (m_MatrixRect.left - m_ClientRect.left) - MATRIX_BORDER_WIDTH;
	int y = (int)lParam - DEVI_TOP_MATRIX_HANDWRITING - (m_MatrixRect.top - m_ClientRect.top) - MATRIX_BORDER_WIDTH;

	if(x < 0 || y < 0 || x > m_nCol * m_PointSize || y > m_nRow * m_PointSize)
		return 0;
	else
	{
		if(SetPointMatrix(y/m_PointSize,x/m_PointSize))
			SendMessage(WM_PAINT,0,0);
	}
	return 0;
}

