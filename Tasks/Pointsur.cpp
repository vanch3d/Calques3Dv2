// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Droite3D.h"
#include "..\Objects\Cercle3D.h"
#include "..\Objects\Plan3D.h"
#include "..\Objects\Sphere3D.h"
#include "..\Objects\Cylinder3D.h"

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

CPointSur3DTask::CPointSur3DTask(CView *AParent,UINT taskID):CTask(AParent,taskID)
{

	pCurr = NULL;
	dr1 = NULL;
	cr1 = NULL;
	pl1 = NULL;
	sp1 = NULL;
	cyl = NULL;
	GetContextualHelp();
}


CPointSur3DTask::~CPointSur3DTask()
{
	if (dr1)	dr1->SetSelected(FALSE);
	if (cr1)	cr1->SetSelected(FALSE);
	if (pl1)	pl1->SetSelected(FALSE);
	if (sp1)	sp1->SetSelected(FALSE);
	if (cyl)	cyl->SetSelected(FALSE);
	if (dr1 || cr1 || pl1 || sp1 || cyl)	
		m_pParent->Invalidate(0);
	if (pCurr) delete pCurr;
	pCurr = NULL;
	dr1 = NULL;
	cr1 = NULL;
	pl1 = NULL;
	sp1 = NULL;
	cyl = NULL;
}

unsigned CPointSur3DTask::GetTaskResID() const 
{ 
	switch (m_nTaskID)
	{	case ID_CONSTRUCTION_POINTON_LINE:
			return CTX_POINTSUR_DROITE;
		case ID_CONSTRUCTION_POINTON_CYLINDER:
			return CTX_POINTSUR_CERCLE;
		case ID_CONSTRUCTION_POINTON_CIRCLE:
			return CTX_POINTSUR_CERCLE;
		case ID_CONSTRUCTION_POINTON_PLANE:
			return CTX_POINTSUR_PLAN;
		case ID_CONSTRUCTION_POINTON_SPHERE:
		default:
			return CTX_POINTSUR_SPHERE;
	}
}

unsigned CPointSur3DTask::GetHelpResID()
{
	UINT mask;
	if (m_nStep == 0)
	{
		switch (m_nTaskID)
		{	case ID_CONSTRUCTION_POINTON_LINE:
				mask = CTX_SELECT_DROITE;
				break;
			case ID_CONSTRUCTION_POINTON_CYLINDER:
			case ID_CONSTRUCTION_POINTON_CIRCLE:
				//mask = (dr1) ? TAllPlanClass : TAllDroiteClass;
				mask = CTX_SELECT_CIRCLE;
				break;
			case ID_CONSTRUCTION_POINTON_PLANE:
				//mask = (dr1) ? TSphere3DClass : TAllDroiteClass;
				mask = CTX_SELECT_PLAN1;
				break;
			case ID_CONSTRUCTION_POINTON_SPHERE:
				mask = CTX_SELECT_SPHERE;
				break;
		}
	}
	else
		mask = CTX_POINTSUR_MOVE;
	return mask;
}


unsigned long CPointSur3DTask::GetMask()
{
	unsigned long mask;
	switch (m_nTaskID)
	{	case ID_CONSTRUCTION_POINTON_LINE		:
			mask = TAllDroiteClass;
			break;
		case ID_CONSTRUCTION_POINTON_CYLINDER:
			mask = TCylindre3DClass;
			break;
		case ID_CONSTRUCTION_POINTON_CIRCLE		:
			mask = TCercle3DClass | TArcCercle3DClass | TCercleInterSS3D;//| TEllipse3DClass;
			////mask = TCylindre3DClass;
			break;
		case ID_CONSTRUCTION_POINTON_PLANE		:
			mask = TAllPlanClass;
			break;
		case ID_CONSTRUCTION_POINTON_SPHERE		:
			mask = TSphere3DClass;
			break;
	}
	return mask;
}

/*CVector4 PrjPtFromSrceen(CVisualParam* myVisuParam,CVector4& res)
{
	FCoord
		st = myVisuParam->ST,
		sp = myVisuParam->SP,
		ct = myVisuParam->CT,
		cp = myVisuParam->CP,
		RCx = myVisuParam->ptRepCoord.x,
		RCy = myVisuParam->ptRepCoord.y,
		x0 = res.x-RCx,
		y0 = RCy-res.y,
		z0 = myVisuParam->ProjParam.dis,
		Rh = myVisuParam->ProjParam.rho,
		z = -sp*z0 + sp*Rh + y0*cp,
		y = -sp*y0*st + x0*ct -cp*z0*st + cp*Rh*st,
		x = -ct*sp*y0 - ct*cp*z0 + ct*cp*Rh - x0*st;
	CVector4 ecran(x,y,z);
	return ecran;
}*/

void CPointSur3DTask::CreateObject3D()
{
	if (pCurr)
	{
		BOOL test = PrepareAddedObject(pCurr);
		if (test && pCurr)
			pCurr->CalculVisuel(m_pParent->GetVisualParam());
		if (pl1)	pl1->UpdateContour();

		m_pParent->Invalidate(0);

		if (dr1) dr1->SetSelected(FALSE);
		if (cr1) cr1->SetSelected(FALSE);
		if (pl1) pl1->SetSelected(FALSE);
		if (sp1) sp1->SetSelected(FALSE);
		if (cyl) cyl->SetSelected(FALSE);

		dr1 = NULL;
		cr1 = NULL;
		pl1 = NULL;
		sp1 = NULL;
		cyl = NULL;
		pCurr = 0;
		m_nStep = 0;
		GetContextualHelp();
		return;
	}

	CVisualParam *myV = m_pParent->GetVisualParam();
	//CVector4 F = myV->ProjectPointFromScreen(ptClic);
	//CVector4 R = myV->ProjectPoint(F);
	CPoint3D * temp=0;
	if (dr1)
	{
		temp = new CPointSurD3D(dr1);
		CVector4 TempCpt;
		if (((CPointSurD3D*)temp)->MoveObject(myV,0,(CPoint)ptClic,TempCpt))
			((CPointSurD3D*)temp)->Concept_pt = TempCpt;
		else
		 {
			dr1 = NULL;
			delete temp;
			temp = NULL;
		 }
	 }
	else if (cr1)
	{
		temp = new CPointSurC3D(cr1);
		CVector4 TempCpt;
		if (((CPointSurC3D*)temp)->MoveObject(myV,0,(CPoint)ptClic,TempCpt))
			((CPointSurC3D*)temp)->Concept_pt = TempCpt;
		else
		{
			cr1 = NULL;
			delete temp;
			temp = NULL;
		}
	}
	else if (pl1)
	{
		temp = new CPointSurP3D(pl1);
		CVector4 TempCpt;
		if (((CPointSurP3D*)temp)->MoveObject(myV,0,(CPoint)ptClic,TempCpt))
			((CPointSurP3D*)temp)->Concept_pt = TempCpt;
		else
		{
			pl1 = NULL;
			delete temp;
			temp = NULL;
		}
	}
	else if (cyl)
	{
		temp = new CPointSurCyl3D(cyl);
		CVector4 TempCpt;
		if (((CPointSurP3D*)temp)->MoveObject(myV,0,(CPoint)ptClic,TempCpt))
			((CPointSurP3D*)temp)->Concept_pt = TempCpt;
		else
		{
			pl1 = NULL;
			delete temp;
			temp = NULL;
		}
	}
	else
	{
		temp = new CPointSurS3D(sp1);
		CVector4 TempCpt;
		if (((CPointSurS3D*)temp)->MoveObject(myV,0,(CPoint)ptClic,TempCpt))
			((CPointSurS3D*)temp)->Concept_pt = TempCpt;
		else
		{
			sp1 = NULL;
			delete temp;
			temp = NULL;
		}
	}
	pCurr = temp;
	m_nStep++;
	if (!pCurr)
	{
		m_nStep = 0;
	}
	GetContextualHelp();
	m_pParent->Invalidate(FALSE);

}


void CPointSur3DTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp=NULL;

	if (pCurr)
	 {
		CreateObject3D();
		//	ReleaseCapture();
		return;
	 }

	int index = FindObject(thepos,GetMask(),FALSE);
	temp = GetObject();
	if (!temp) return;

	switch (m_nTaskID)
	{	case ID_CONSTRUCTION_POINTON_LINE:
			dr1 = (CDroite3D*)temp;
			break;
		case ID_CONSTRUCTION_POINTON_CYLINDER:
			cyl = (CCylinder3D*)temp;
			break;
		case ID_CONSTRUCTION_POINTON_CIRCLE:
			cr1 = (CCercle3D*)temp;
			//cyl = (CCylinder3D*)temp;
			break;
		case ID_CONSTRUCTION_POINTON_PLANE:
			pl1 = (CPlan3D*)temp;
			break;
		case ID_CONSTRUCTION_POINTON_SPHERE:	
			sp1 = (CSphere3D *)temp;
			break;
		default:
			return;
	}

	ptClic = CVector4(thepos.x,thepos.y,0,1);
	temp->SetSelected(TRUE);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
	CreateObject3D();
	if (pCurr)
	{
		//	m_pParent->SetCapture();
		CVisualParam *myVisuParam = m_pParent->GetVisualParam();

		pCurr->CalculConceptuel();
		pCurr->CalculVisuel(myVisuParam);
	}

}

void CPointSur3DTask::OnMouseR(UINT, CPoint)
{
}

void CPointSur3DTask::DrawFeedBack(CDC *pDC)
{
	if (!pCurr) return;
	CVisualParam *myVisuParam = m_pParent->GetVisualParam();
	pCurr->Draw(pDC,myVisuParam);
	// draw feedback
	pCurr->DrawRetro(pDC,myVisuParam);
}


void CPointSur3DTask::OnMouseMove(UINT modkey, CPoint thepos)
{	
	if (!pCurr)
	{
		FindObject(thepos,GetMask(),TRUE);
		return;
	}
	
	CVisualParam *myVisuParam = m_pParent->GetVisualParam();

	CVector4 TempCpt = pCurr->Concept_pt;
	CPoint theP = thepos;
	//theP.x+=(int) ((Parent->Scroller) ? Parent->Scroller->XPos : 0);
	//theP.y+=(int) ((Parent->Scroller) ? Parent->Scroller->YPos : 0);

	CPoint MouseClic = theP;
	
	BOOL bOk = pCurr->MoveObject(myVisuParam,modkey,MouseClic,TempCpt);
	if (bOk)
	{
		pCurr->Concept_pt = TempCpt;
		pCurr->CalculConceptuel();
		pCurr->CalculVisuel(myVisuParam);
		m_pParent->Invalidate(FALSE);
	}
}