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
// ViewCalque.cpp: implementation of the CViewCalque class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Calques3D.h"
#include "Calques3DDoc.h"

#include "ViewUniv.h"
#include "ViewCalque.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewCalque

IMPLEMENT_DYNCREATE(CViewCalque, CViewUniv)

CViewCalque::CViewCalque()
{
}

CViewCalque::~CViewCalque()
{
}


BEGIN_MESSAGE_MAP(CViewCalque, CViewUniv)
	//{{AFX_MSG_MAP(CViewCalque)
	//}}AFX_MSG_MAP
	//ON_COMMAND_RANGE(ID_OBJECT_POINT,ID_OBJECT_DELETE, OnStartTask)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OBJECT_POINT,ID_OBJECT_POLYGON, OnUpdateTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CONSTRUCTION_POINTON,ID_CONSTRUCTION_DIVSEGMENT, OnUpdateTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EXPLORATION_LOCUS,ID_EXPLORATION_LABEL, OnUpdateTasks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EXPLORATION_DISCOVERY,ID_EXPLORATION_DISCOVERY, OnUpdateTasks)
	//ON_COMMAND_RANGE(ID_CONSTRUCTION_POINTON_CYLINDER,ID_CONSTRUCTION_POINTON_CYLINDER, OnStartTask)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCalque drawing

void CViewCalque::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	CViewUniv::OnDraw(pDC);
}


/////////////////////////////////////////////////////////////////////////////
// CViewCalque diagnostics

#ifdef _DEBUG
void CViewCalque::AssertValid() const
{
	CViewUniv::AssertValid();
}

void CViewCalque::Dump(CDumpContext& dc) const
{
	CViewUniv::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewCalque message handlers

void CViewCalque::OnInitialUpdate() 
{
	CViewUniv::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
//	GetVisualParam()->nCalqueNum = 1;	
}

//////////////////////////////////////////////////////////////////////
/// Override CViewUniv::OnUpdateTasks by disabling all construction tasks.
///
/// \param pCmdUI	A pointer to the command handler
//////////////////////////////////////////////////////////////////////
void CViewCalque::OnUpdateTasks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

BOOL CViewCalque::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return CViewUniv::OnPreparePrinting(pInfo);
}
