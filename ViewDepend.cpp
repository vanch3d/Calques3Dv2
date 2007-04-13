//////////////////////////////////////////////////////////////////////
// Calques 3D - a 3D Dynamic Geometry Learning Environment
// Copyright (c) 1997-2007 Nicolas Van Labeke
//////////////////////////////////////////////////////////////////////
// This file is part of Calques 3D.
// 
// Calques 3D is free software; you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by 
// the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.
// 
// Calques 3D is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License 
// along with Calques 3D; if not, write to The Free Software Foundation, Inc., 
// 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA 
//////////////////////////////////////////////////////////////////////
// ViewDepend.cpp: implementation of the CViewDepend class.
//
//////////////////////////////////////////////////////////////////////

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
