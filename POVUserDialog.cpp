// POVUserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "POVUserDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPOVUserDialog dialog


CPOVUserDialog::CPOVUserDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPOVUserDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPOVUserDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPOVUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPOVUserDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPOVUserDialog, CDialog)
	//{{AFX_MSG_MAP(CPOVUserDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOVUserDialog message handlers
