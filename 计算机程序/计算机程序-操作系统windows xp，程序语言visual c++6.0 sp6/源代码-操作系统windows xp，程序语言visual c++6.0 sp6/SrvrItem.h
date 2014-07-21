// SrvrItem.h : interface of the CCRSrvrItem class
//

#if !defined(AFX_SRVRITEM_H__0A7A0FE6_7077_454C_8C60_C34DD598E5DA__INCLUDED_)
#define AFX_SRVRITEM_H__0A7A0FE6_7077_454C_8C60_C34DD598E5DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCRSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CCRSrvrItem)

// Constructors
public:
	CCRSrvrItem(CCRDoc* pContainerDoc);

// Attributes
	CCRDoc* GetDocument() const
		{ return (CCRDoc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CCRSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVRITEM_H__0A7A0FE6_7077_454C_8C60_C34DD598E5DA__INCLUDED_)
