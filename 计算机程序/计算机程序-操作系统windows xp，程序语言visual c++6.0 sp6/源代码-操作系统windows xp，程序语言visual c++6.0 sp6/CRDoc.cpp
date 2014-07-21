// CRDoc.cpp : implementation of the CCRDoc class
//

#include "stdafx.h"
#include "CR.h"

#include "CRDoc.h"
#include "CntrItem.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCRDoc

IMPLEMENT_DYNCREATE(CCRDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CCRDoc, COleServerDoc)
	//{{AFX_MSG_MAP(CCRDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleServerDoc::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleServerDoc::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleServerDoc::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, COleServerDoc::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleServerDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleServerDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, COleServerDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCRDoc construction/destruction

CCRDoc::CCRDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

}

CCRDoc::~CCRDoc()
{
}

BOOL CCRDoc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCRDoc server implementation

COleServerItem* CCRDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CCRSrvrItem* pItem = new CCRSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CCRDoc Active Document server implementation

CDocObjectServer *CCRDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}



/////////////////////////////////////////////////////////////////////////////
// CCRDoc serialization

void CCRDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class COleServerDoc enables serialization
	//  of the container document's COleClientItem objects.
	COleServerDoc::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CCRDoc diagnostics

#ifdef _DEBUG
void CCRDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CCRDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCRDoc commands
