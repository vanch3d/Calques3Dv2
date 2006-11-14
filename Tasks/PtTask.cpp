// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Plan3D.h"

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
///Construction/Destruction
///
///
/// \verbatim
///                                          ______
///        |               /|               |      |
///        |  *           / |               |   /  |
///        |  |          | ---*             |  /   |
///        |__|___       |  |______         |_*____|
///       /   |  /       | /               /      
///      /______/        |/               /       
///
/// \endverbatim
//////////////////////////////////////////////////////////////////////

CPoint3DTask::CPoint3DTask(CView *AParent,UINT nID) : CTask(AParent,nID)
{
	m_pObj = NULL;
	// changement du repère
	oldProj = m_pParent->GetVisualParam()->ProjParam;
	nOldVisu = m_pParent->GetVisualParam()->nVisuKind;
//	TVisualParam *myVisu = TYPESAFE_DOWNCAST(Parent->GetViewParam(),TVisualParam);
//	TVisuClois *myVisuC = TYPESAFE_DOWNCAST(myVisu,TVisuClois);
//	myVisuC->CalcRgn = 1;
	SProjection SPr = {25.0,45.0,oldProj.rho,oldProj.dis};
	if ((oldProj.phi > 0.) && (oldProj.phi < 90.))
		SPr.phi = oldProj.phi;
	if ((oldProj.theta > 0.) && (oldProj.theta < 90.))
		SPr.theta = oldProj.theta;
	m_pParent->GetVisualParam()->SetProjParam(SPr);
	m_pParent->SetVisualParam(CVisualParam::VisuClois);
	m_bIsRepereFixed = TRUE;
	CVisualParam *myVisuParam = m_pParent->GetVisualParam();
	myVisuParam->bFeedPlane = TRUE;

	//Parent->RCB->EnableWindow(false);
	//Parent->VS->EnableWindow(false);
	//Parent->HS->EnableWindow(false);
	//TPref::MoveType = 0;
	m_pParent->SetFixedProjection(TRUE);
	InvalidateParent();
	//m_pParent->Invalidate();
	GetContextualHelp();
}

CPoint3DTask::~CPoint3DTask()
{
	if (m_pObj)	delete m_pObj;
	m_pObj = 0;
	//Parent->RCB->EnableWindow(true);
	//Parent->VS->EnableWindow(true);
	//Parent->HS->EnableWindow(true);
	m_pParent->SetFixedProjection(FALSE);
	m_pParent->GetVisualParam()->SetProjParam(oldProj);
	m_pParent->SetVisualParam(nOldVisu);
	m_bIsRepereFixed = TRUE;
	CVisualParam *myVisuParam = m_pParent->GetVisualParam();
	myVisuParam->bFeedPlane = FALSE;
	//TPref::MoveType = 0;

	InvalidateParent();
	//if (m_pParent && m_pParent->GetSafeHwnd()) m_pParent->Invalidate();
}

unsigned CPoint3DTask::GetHelpResID()
{
	return ((m_nStep) ? CTX_POINT_DRAG : CTX_POINT_INIT);
}


void CPoint3DTask::OnMouseL(UINT modkey, CPoint thepos)
{
	if (m_pObj)
	{
		CreateObject3D();
		//	ReleaseCapture();
		return;
	}

	CPoint theP(thepos) ;
	CRect theR(theP.x-4,theP.y-4,theP.x+4,theP.y+4);

	CVisualParam *myVisuParam = m_pParent->GetVisualParam();
	CPoint3D a(CVector4(0,0,0,0));
	CPoint3D b(CVector4(0,100,0,0));
	CPoint3D c(CVector4(0,0,100,0));
	CPoint3D d(CVector4(100,0,0,0));


	BOOL bDone = FALSE;
	CVector4 newPt1(0,0,0,0);
	switch (TPref::TUniv.nMoveType)
	 { case TPref::MV_HORIZ:
		 {
			CPlan3D pl(&a,&b,&d);
			pl.CalculConceptuel();
			CPointSurP3D pton(&pl);
			pton.MoveObject(myVisuParam,0,theP,newPt1);
			bDone = pton.CalculConceptuel();
			newPt1 = pton.Concept_pt;
		 }
		 break;
		case TPref::MV_LEFT:
		 {
			CPlan3D pl(&a,&d,&c);
			pl.CalculConceptuel();
			CPointSurP3D pton(&pl);
			pton.MoveObject(myVisuParam,0,theP,newPt1);
			bDone = pton.CalculConceptuel();
			newPt1 = pton.Concept_pt;
		 }
		 break;
		case TPref::MV_RIGHT:
		 {
			CPlan3D pl(&a,&b,&c);
			pl.CalculConceptuel();
			CPointSurP3D pton(&pl);
			pton.MoveObject(myVisuParam,0,theP,newPt1);
			bDone = pton.CalculConceptuel();
			newPt1 = pton.Concept_pt;
		 }
		 break;
	 }
//	newPt = myVisuParam->GetProjectedPoint(theP);
	if (!bDone)
	{
		m_pObj = new CPoint3D(newPt1);
		m_pObj->CalculConceptuel();
		m_pObj->CalculVisuel(myVisuParam);
		m_nStep++;
		GetContextualHelp();
	}
}

void CPoint3DTask::OnMouseR(UINT modkey, CPoint thepos)
{
	theApp.ShowPopupMenu (IDR_POPUP_MOVE, thepos, m_pParent);
}


void CPoint3DTask::OnMouseMove(UINT modkey, CPoint thepos)
{
	FindObject(thepos,0,TRUE);
	if (m_pObj)		// there is a selected point
	{
		CVisualParam *myVisuParam = m_pParent->GetVisualParam();
		CVector4 TempCpt = m_pObj->Concept_pt;

		// calcul new objects position and draw all objects
		if (m_pObj->MoveObject(myVisuParam,modkey,thepos,TempCpt))
		 {
			m_pObj->Concept_pt = TempCpt;
			m_pObj->CalculConceptuel();
			m_pObj->CalculVisuel(myVisuParam);

			m_pParent->Invalidate(FALSE);
		 }
	 }

}

void CPoint3DTask::OnMouseLUp(UINT, CPoint)
{
}

void CPoint3DTask::CreateObject3D()
{
	PrepareAddedObject(m_pObj);
	m_pObj = 0;
	m_nStep = 0;
	GetContextualHelp();
	m_pParent->Invalidate(FALSE);
}

void CPoint3DTask::DrawFeedBack(CDC *pDC)
{
	if (!m_pObj) return;
	CVisualParam *myVisuParam = m_pParent->GetVisualParam();

	m_pObj->Draw(pDC,myVisuParam);
	myVisuParam->DrawFeedBack(pDC,m_pObj);
}

BOOL CPoint3DTask::OnUpdateTasksOption(CCmdUI* pCmdUI)
{
	BOOL bDone = TRUE;
	BOOL bCheck = FALSE;
	BOOL bEnab = FALSE;

	switch (pCmdUI->m_nID){
	case ID_EXPLORATION_MOVE_MAGNETISM :
		bEnab = TRUE;
		bCheck = TPref::TUniv.bMagnet;
		break;
	case ID_EXPLORATION_MOVE_HORIZONTAL  :
	case ID_EXPLORATION_MOVE_LEFT :
	case ID_EXPLORATION_MOVE_RIGTH:
		bEnab = TRUE;
		bCheck = (TPref::TUniv.nMoveType == (int)(pCmdUI->m_nID - ID_EXPLORATION_MOVE_HORIZONTAL));
		break;
	default:
		bDone = FALSE;
		break;
	}

	if (m_nStep) bEnab = FALSE;
	pCmdUI->Enable(bEnab);
	pCmdUI->SetCheck(bCheck);
	return bDone;
}



BOOL CPoint3DTask::OnDoTasksOption(UINT nID)
{
	BOOL bDone = TRUE;
	switch (nID){
	case ID_EXPLORATION_MOVE_MAGNETISM :
		TPref::TUniv.bMagnet = !TPref::TUniv.bMagnet;
		break;
	case ID_EXPLORATION_MOVE_HORIZONTAL  :
	case ID_EXPLORATION_MOVE_LEFT :
	case ID_EXPLORATION_MOVE_RIGTH:
		{
			TPref::TUniv.nMoveType = (nID - ID_EXPLORATION_MOVE_HORIZONTAL);
			InvalidateParent(TRUE);
			//m_pParent->Invalidate();
			//m_pParent->UpdateWindow();
		}
		break;
	default:
		bDone = FALSE;
		break;
	}
	GetContextualHelp();
	return bDone;
}
