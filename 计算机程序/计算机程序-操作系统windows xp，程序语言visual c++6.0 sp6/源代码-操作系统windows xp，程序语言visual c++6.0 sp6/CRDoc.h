// CRDoc.h : interface of the CCRDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRDOC_H__89C2E5EB_ED79_4D90_8E06_E506E31E798A__INCLUDED_)
#define AFX_CRDOC_H__89C2E5EB_ED79_4D90_8E06_E506E31E798A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCRSrvrItem;

class CCRDoc : public COleServerDoc
{
protected: // create from serialization only
	CCRDoc();
	DECLARE_DYNCREATE(CCRDoc)

// Attributes
public:
	CCRSrvrItem* GetEmbeddedItem()
		{ return (CCRSrvrItem*)COleServerDoc::GetEmbeddedItem(); }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRDoc)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

// Generated message map functions
protected:
	//{{AFX_MSG(CCRDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRDOC_H__89C2E5EB_ED79_4D90_8E06_E506E31E798A__INCLUDED_)
