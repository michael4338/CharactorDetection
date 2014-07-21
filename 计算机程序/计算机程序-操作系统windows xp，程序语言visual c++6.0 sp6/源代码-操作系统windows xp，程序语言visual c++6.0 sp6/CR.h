// CR.h : main header file for the CR application
//

#if !defined(AFX_CR_H__5CFE537A_113E_4FD1_BA2B_3259F72E6D0C__INCLUDED_)
#define AFX_CR_H__5CFE537A_113E_4FD1_BA2B_3259F72E6D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCRApp:
// See CR.cpp for the implementation of this class
//

class CCRApp : public CWinApp
{
public:
	CCRApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CCRApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CR_H__5CFE537A_113E_4FD1_BA2B_3259F72E6D0C__INCLUDED_)
