// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Cercle3D.h"

#include "..\Calques3DDoc.h"
#include "..\ViewUniv.h"
//#include "..\MainFrm.h"

#include "Task.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCenterTask::CCenterTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
	pl = 0;
}

CCenterTask::~CCenterTask()
{
	if (pl)	pl->SetSelected(FALSE);
	if (pl)
		m_pParent->Invalidate(0);
	pl = NULL;
	GetContextualHelp();
}

unsigned CCenterTask::GetHelpResID()
{
	return CTX_SELECT_CIRCLE;
}



DWORD CCenterTask::GetMask()
{
	return TAllCercleClass;
}

void CCenterTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp;
	int index = FindObject(thepos,GetMask(),FALSE);
	if (!index) return;

	temp = GetObject();
	if (!temp) return;
	temp->SetSelected(TRUE);//bIsSelected = 1;
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
	m_pParent->Invalidate(0);
//	if (temp->MaskObject(GetMask()))
//	 {
	pl = (CCercle3D*)temp;
	CreateObject3D();
//	 }
//	Geom3DApp->SetOnLineHelp(GetContextualHelp());
}

void CCenterTask::OnMouseMove(UINT, CPoint thepos)
{
	FindObject(thepos,GetMask(),TRUE);
}

void CCenterTask::CreateObject3D()
{
	CObject3D *temp=0;
	if (!pl) return;

	temp = new CPointCenter3D(pl);
	temp->CalculConceptuel();
	PrepareAddedObject(temp);

	if (pl) pl->SetSelected(FALSE);//bIsSelected = 0;
	pl = 0;
	m_pParent->Invalidate(0);
}
