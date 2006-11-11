// ViewCalque.cpp : implementation file
//

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
