// View3DRender.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "View3DRender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView3DRender

IMPLEMENT_DYNCREATE(CView3DRender, CView)

CView3DRender::CView3DRender()
{
}

CView3DRender::~CView3DRender()
{
}


BEGIN_MESSAGE_MAP(CView3DRender, CView)
	//{{AFX_MSG_MAP(CView3DRender)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView3DRender drawing

void CView3DRender::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CView3DRender diagnostics

#ifdef _DEBUG
void CView3DRender::AssertValid() const
{
	CView::AssertValid();
}

void CView3DRender::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView3DRender message handlers
