// PrefHistory.cpp : implementation file
//

#include "stdafx.h"
#include "..\calques3d.h"
#include "PrefHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefHistory property page

IMPLEMENT_DYNCREATE(CPrefHistory, CPropertyPage)

CPrefHistory::CPrefHistory() : CPropertyPage(CPrefHistory::IDD)
{
	//{{AFX_DATA_INIT(CPrefHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPrefHistory::~CPrefHistory()
{
}

void CPrefHistory::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefHistory)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefHistory, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefHistory)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefHistory message handlers
