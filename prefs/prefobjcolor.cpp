// prefobjcolor.cpp : implementation file
//

#include "stdafx.h"
#include "..\calques3d.h"
#include "prefobjcolor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefObjColor property page

IMPLEMENT_DYNCREATE(CPrefObjColor, CPropertyPage)

CPrefObjColor::CPrefObjColor() : CPropertyPage(CPrefObjColor::IDD)
{
	//{{AFX_DATA_INIT(CPrefObjColor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPrefObjColor::~CPrefObjColor()
{
}

void CPrefObjColor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefObjColor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefObjColor, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefObjColor)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefObjColor message handlers
