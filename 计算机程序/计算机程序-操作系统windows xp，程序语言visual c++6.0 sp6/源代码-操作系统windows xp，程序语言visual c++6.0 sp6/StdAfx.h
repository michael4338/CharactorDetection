// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B9D17E24_E449_4781_8109_698BA3B5888B__INCLUDED_)
#define AFX_STDAFX_H__B9D17E24_E449_4781_8109_698BA3B5888B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdocob.h>
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "sizecbar.h" 
#include "scbarg.h" 


#define      IDC_STATIC_RIGHT_MATRIX        2222         //�ұߵĵ���ID
#define      IDC_STATIC_LEFT_MATRIX         3333         //��ߵĵ���ID
#define      IDC_STATIC_BOTTOM_MATRIX       6666         //�±ߵĵ���ID

#define      WM_USER_SETPOINT               WM_USER+128  //�Զ�����Ϣ������ͼ����󴫵ݵ�����         
#define      WM_USER_SETMOVEPOINT           WM_USER+151  //�Զ�����Ϣ������ͼ����󴫵��ƶ�������
#define      WM_USER_RESET_DISPLAY          WM_USER+23    //�Զ�����Ϣ������ͼ���ұ���ʾ���󴫵�Ԥ�����Ľ��

//#define    WM_USER_SETVIEW_MATRIX         WM_USER+152  //�Զ�����Ϣ���ɿ���������ͼ���ݵ������ò���
//#define    WM_USER_SETVIEW_HANDPAT        WM_USER+153  //�Զ�����Ϣ���ɿ���������ͼ������д�����ò���
//#define    WM_USER_SETVIEW_PICTURE        WM_USER+154  //�Զ�����Ϣ���ɿ���������ͼ����ͼƬ���ò���


#define      DEVI_LEFT_MATRIX_HANDWRITING   10           //������д��͵���ƫ��ͻ��������λ��
#define      DEVI_TOP_MATRIX_HANDWRITING    10           //������д��͵���ƫ��ͻ������ϱ�λ��


  


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B9D17E24_E449_4781_8109_698BA3B5888B__INCLUDED_)
