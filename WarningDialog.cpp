// WarningDialog.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "WarningDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarningDialog dialog


CWarningDialog::CWarningDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarningDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWarningDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarningDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWarningDialog, CDialog)
	//{{AFX_MSG_MAP(CWarningDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarningDialog message handlers
