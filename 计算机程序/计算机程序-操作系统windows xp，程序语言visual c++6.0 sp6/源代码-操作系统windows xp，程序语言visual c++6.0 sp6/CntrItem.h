// CntrItem.h : interface of the CCRCntrItem class
//

#if !defined(AFX_CNTRITEM_H__0F5653A7_6A1B_42F6_8538_3A01B65E5C69__INCLUDED_)
#define AFX_CNTRITEM_H__0F5653A7_6A1B_42F6_8538_3A01B65E5C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCRDoc;
class CCRView;

class CCRCntrItem : public COleDocObjectItem
{
	DECLARE_SERIAL(CCRCntrItem)

// Constructors
public:
	CCRCntrItem(CCRDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CCRDoc* GetDocument()
		{ return (CCRDoc*)COleDocObjectItem::GetDocument(); }
	CCRView* GetActiveView()
		{ return (CCRView*)COleDocObjectItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL CanActivate();
	//}}AFX_VIRTUAL

// Implementation
public:
	~CCRCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__0F5653A7_6A1B_42F6_8538_3A01B65E5C69__INCLUDED_)
