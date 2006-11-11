// ViewDepend.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"

#include "Calques3DDoc.h"

#include "ViewDepend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDepend

IMPLEMENT_DYNCREATE(CViewDepend, CListView)

CViewDepend::CViewDepend()
{
}

CViewDepend::~CViewDepend()
{
}


BEGIN_MESSAGE_MAP(CViewDepend, CListView)
	//{{AFX_MSG_MAP(CViewDepend)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDepend drawing

void CViewDepend::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CViewDepend diagnostics

#ifdef _DEBUG
void CViewDepend::AssertValid() const
{
	CListView::AssertValid();
}

void CViewDepend::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCalques3DDoc* CViewDepend::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalques3DDoc)));
	return (CCalques3DDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDepend message handlers

void CViewDepend::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl& mListCtrl = GetListCtrl();
	switch (lHint)
	{
		case WM_UPDATEOBJ_DEP:	// Update Depend List
		{
			mListCtrl.DeleteAllItems();

			CObject3D *pObj=NULL;
			pObj = DYNAMIC_DOWNCAST(CObject3D,pHint);
			if (!pObj) break;

			CxObject3DSet myList;
			pObj->GetDependList(&myList);

			int nb = myList.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D* pObj = myList.GetAt(i);
				if (!pObj) continue;
				int idx = mListCtrl.InsertItem(i,pObj->GetObjectDef());
				mListCtrl.SetItemData(idx,(DWORD)pObj);
			}
		}
		break;
	}

	
}

void CViewDepend::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	CListCtrl& mListCtrl = GetListCtrl();
	DWORD dwStyle = mListCtrl.GetStyle();

	dwStyle &= ~LVS_ICON;
	dwStyle |= LVS_LIST;//LVS_SMALLICON;
	::SetWindowLong(mListCtrl.m_hWnd,GWL_STYLE,dwStyle);
}
