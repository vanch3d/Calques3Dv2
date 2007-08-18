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
// ProjTask.cpp: implementation of the CTask class.
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
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CProjectionTask::CProjectionTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
	bTwice=FALSE;
	nProjID = 0;
	pl = NULL;
	pt1 = pt2 = pt3 = NULL;
	m_nStep = 0;
}

CProjectionTask::~CProjectionTask()
{
	m_pParent->SetFixedProjection(FALSE);
	m_pParent->GetVisualParam()->bFixed = FALSE;
}

CObjectId CProjectionTask::GetMask()
{
	if (!pl && !pt1)
		return TAllPointClass | TAllPlanClass;
	else if (pl)
		return TAllPlanClass;
	else
		return TAllPointClass;
}

unsigned CProjectionTask::GetHelpResID()
{
	unsigned nMask = 0;

	switch (m_nStep){
	case 1: nMask = CTX_SELECT_POINT2;break;
	case 2: nMask = CTX_SELECT_POINT3;break;
	case 3: nMask = CTX_PROJECTION_APPLY;break;
	default:
		nMask = CTX_PROJECTION_SELECT;break;
	}
	return nMask;
}

void CProjectionTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp;
	if (pl || (pt1 && pt2 && pt3)) return;
	int index = FindObject(thepos,GetMask(),FALSE);
	if (!index)  return;
 /*	 { if (pl || (pt1 && pt2 && pt3))
		 {
			if (pl) pl->IsSelected = false;
			if (pt1) pt1->IsSelected = false;
			if (pt2) pt2->IsSelected = false;
			if (pt3) pt3->IsSelected = false;
			pl = 0;
			pt1 = pt2 = pt3 = 0;
			Parent->Invalidate(0);
			twice = true;
		 }
		Geom3DApp->SetOnLineHelp(GetContextualHelp());
		Parent->VS->EnableWindow(true);
		Parent->HS->EnableWindow(true);
		return;
	 }  */

	temp = GetObject();
	if (!temp) return;
	temp->SetSelected(TRUE);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
	m_pParent->Invalidate(0);
	if (temp->MaskObject(TAllPlanClass))
	 {
		pl = (CPlan3D*)temp;
		m_nStep = 3;
		CreateObject3D();
	 }
	else
	 {	if (!pt1)
		{
			pt1 = (CPoint3D*)temp;
			m_nStep = 1;
		}	
		else if (!pt2)
		{
			if (temp != pt1)
			{
				pt2 = (CPoint3D*)temp;
				m_nStep = 2;
			}
			else
			{
				ShowErrorMsg(ERR_2SAMEPTS);
				return;
			}
		}
		else if (!pt3)
		{
			if ((temp != pt1) && (temp != pt1))
			{
				pt3  = (CPoint3D*)temp;
				m_nStep = 3;
				CreateObject3D();
			}
			else
			{
				ShowErrorMsg(ERR_2SAMEPTS);
				return;
			}
		}
	 }
	GetContextualHelp();

}

BOOL CProjectionTask::OnUpdateTasksOption(CCmdUI* pCmdUI)
{
	BOOL bDone = TRUE;
	BOOL bCheck = FALSE;
	BOOL bEnab = FALSE;

	switch (pCmdUI->m_nID){
	case ID_VISUALISATION_PROJECTION_FRONT :
//	case ID_VISUALISATION_PROJECTION_PROFIL:
		bEnab = (BOOL)(pt3 || pl);
		bCheck = (nProjID == (int)(pCmdUI->m_nID - ID_VISUALISATION_PROJECTION_FRONT));
		break;
	case ID_VISUALISATION_PROJECTION_MIRROR:
		bEnab = bTwice && (BOOL)(pt3 || pl);
		bCheck = (nProjID == (int)(pCmdUI->m_nID - ID_VISUALISATION_PROJECTION_FRONT));
		break;
	case ID_VISUALISATION_PROJECTION_KEEPIT:
		bEnab = TRUE;
		bCheck = m_pParent->GetVisualParam()->bKeepProj;
		break;
	default:
		bDone = FALSE;
		break;
	}
	pCmdUI->Enable(bEnab);
	pCmdUI->SetCheck(bCheck);
	return bDone;
}


BOOL CProjectionTask::OnDoTasksOption(UINT nID)
{
	BOOL bDone = TRUE;
	switch (nID){
	case ID_VISUALISATION_PROJECTION_FRONT :
	case ID_VISUALISATION_PROJECTION_PROFIL:
	case ID_VISUALISATION_PROJECTION_MIRROR:
		nProjID = (int)(nID - ID_VISUALISATION_PROJECTION_FRONT);
		CreateObject3D();
		break;
		break;
	case ID_VISUALISATION_PROJECTION_KEEPIT:
		{
			BOOL bKP = m_pParent->GetVisualParam()->bKeepProj;
			m_pParent->SetFixedProjection(!bKP);
			m_pParent->GetVisualParam()->bFixed = FALSE;
		}
		break;
	default:
		bDone = FALSE;
		break;
	}
	return bDone;
}


void CProjectionTask::OnMouseR(UINT, CPoint point)
{
	theApp.ShowPopupMenu (IDR_POPUP_PROJECTION, point, m_pParent);
}

void CProjectionTask::OnMouseMove(UINT, CPoint thepos)
{
	FindObject(thepos,GetMask(),TRUE);
	m_ptOld = thepos;
//	m_pParent->Invalidate(FALSE);
}

void CProjectionTask::OnMouseLUp(UINT, CPoint)
{
}

void CProjectionTask::CreateObject3D()
{
	if (!pl && (!pt1 && !pt2 && !pt3)) return;

	//bTwice = TRUE;
	if (pl)	pl->SetSelected(FALSE);
	if (pt1)	pt1->SetSelected(FALSE);
	if (pt2)	pt2->SetSelected(FALSE);
	if (pt3)	pt3->SetSelected(FALSE);
//	OnMouseR(0,TPoint(0,0));
//	GetContextualHelp();

	BOOL bRet= FALSE;
	if (nProjID == 0)
	{
		bRet = ProjectFrontal();
	}
	else if (nProjID == 2)
	{
		bRet = ProjectMiroir();
	}
	if (bRet)
	{
		CenterProject();
		InvalidateParent(TRUE);
		///m_pParent->Invalidate();
		//m_pParent->UpdateWindow();
	}
}

void CProjectionTask::SelectProjection(int id)
{
}

BOOL CProjectionTask::ProjectFrontal()
{
	CVisualParam *mV = m_pParent->GetVisualParam();
	CVector4 ff;
	if (pl)	ff = pl->VecNorm;
	else
	 {
		CVector4 p1p2 = pt2->Concept_pt - pt1->Concept_pt;
		CVector4 p1p3 = pt3->Concept_pt - pt1->Concept_pt;
		ff = (p1p2 % p1p3);
	 }
	CVector4 oeil= mV->GetEyePos();
	CVector4 origin(0,0,0,1);
	CVector4 VisuNorm= oeil - ((mV->bParProj) ? origin : origin);
	if (ff.NullVector() || VisuNorm.NullVector())
		return FALSE;

	CVector4 v = VisuNorm * (1/VisuNorm.Norme());
	CVector4 vp(VisuNorm.x,VisuNorm.y,0.);
	CVector4 up(ff.x,ff.y,0.);
	CVector4 u = ff * (1/ff.Norme());

	FCoord kvp = vp.Norme();
	FCoord kup = up.Norme();
	if (!vp.NullVector())
		vp = vp * (1/kvp);
	if (!up.NullVector())
		up = up * (1/kup);

	CVector4 UvV = (u % v);
	if (!UvV.NullVector())
		UvV = UvV * (1/UvV.Norme());
	CVector4 UpvVp = (up %vp);
	if (!UpvVp.NullVector())
		UpvVp = UpvVp * (1/UpvVp.Norme());

	FCoord phi1,thet,phi2;
	if (vp.NullVector() && up.NullVector())
	{
		phi1 = thet = phi2 = 1.0;
	}
	else if (vp.NullVector())
	{	
		phi1 = (v * CVector4(1,0,0));
		thet = CVector4(1,0,0) * up;
		phi2 = up * u;
	}
	else if (up.NullVector())
	{
		phi1 = v * vp;
		thet = vp * CVector4(1,0,0);
		phi2 = 0;//DotProduct(up,u);
	}
	else
	{
		phi1 = v * vp;
		thet = vp * up;
		phi2 = up * u;
	}

	///rotation selon phi
	FCoord dd;
	if ((dd = fabsl(phi1)) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			phi1 = (phi1 < 0.00) ? -1.00 : 1.00;
			//return 0;
		}
	if ((dd = fabsl(phi2)) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			//return 0;
         phi2 = (phi2 < 0.00) ? -1.00 : 1.00;
		}
	FCoord Aphi1 = acosl(phi1);
	Aphi1 = (Aphi1*180.0)/M_PI;
	FCoord Aphi2 = acosl(phi2);
	Aphi2 = (Aphi2*180.0)/M_PI;
	if (v.z <0) Aphi1 *= -1.;
	if (u.z <0) Aphi2 *= -1.;
	FCoord RotPas = Aphi2-Aphi1;
	mV->AddProjParam(RotPas,CVisualParam::ID_PHI);

	///rotation selon theta
	if ((dd = fabsl(thet)) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			thet = (thet < 0.00) ? -1.00 : 1.00;
			//return 0;
		}
	RotPas = acosl(thet);
	if (UpvVp.z > 0) RotPas *=-1;
	RotPas = (RotPas*180.0)/M_PI;
	mV->AddProjParam(RotPas,CVisualParam::ID_THETA);

	bTwice = TRUE;
	return TRUE;
}

BOOL CProjectionTask::ProjectProfil()
{
	bTwice = TRUE;
	return TRUE;
}

BOOL CProjectionTask::ProjectMiroir()
{
	CVisualParam *mV = m_pParent->GetVisualParam();

	SProjection proj = mV->GetProjParam();
	///rotation selon phi
	mV->AddProjParam(180.,CVisualParam::ID_THETA);
		///rotation selon theta
	mV->AddProjParam(-2*proj.phi,CVisualParam::ID_PHI);
	return TRUE;
}


BOOL CProjectionTask::CenterProject()
{
	CVisualParam *mV = m_pParent->GetVisualParam();

	//Parent->RCB->EnableWindow(false);
	BOOL ft = TRUE;
	if (ft)
	{
		//Geom3DApp->SetToolTip(0);
		int b = (int)mV->ProjParam.theta;
		b = 180 -b + 45;
		b = (b < 0) ? b + 360 : ((b > 360) ? b-360 : b);
		//int mb = Parent->HS->GetPosition();
		//if (mb != b)
		// {	Parent->HS->SetPosition(b);
		//	Parent->HS->Invalidate(TRUE);
		// }
		b = (int)mV->ProjParam.phi;
		b = 180 +b - 20;
		b = (b < 0) ? b + 360 : ((b > 360) ? b-360 : b);
		//mb = Parent->VS->GetPosition();
		//if (mb != b)
		 //{	Parent->VS->SetPosition(b);
			//Parent->VS->Invalidate(TRUE);
		 //}
	 }


	//bTwice = FALSE;
	///translation du centre du repère
	CVector4 p1p3;
	if (pl)
	 {
		p1p3 = pl->p3 - pl->p1;
		p1p3 = pl->p1 + p1p3 * .5;
	 }
	else
	 {	p1p3.x = (pt1->Concept_pt.x + pt2->Concept_pt.x + pt3->Concept_pt.x) / 3;
		p1p3.y = (pt1->Concept_pt.y + pt2->Concept_pt.y + pt3->Concept_pt.y) / 3;
		p1p3.z = (pt1->Concept_pt.z + pt2->Concept_pt.z + pt3->Concept_pt.z) / 3;
	 }
	CPoint center = mV->ProjectPoint(p1p3);
	CRect mrect;
	m_pParent->GetClientRect(&mrect);
	CSize dis = center-CPoint(mrect.Width()/2,mrect.Height()/2);
	mV->ptRepCoord.x -= dis.cx;
	mV->ptRepCoord.y -= dis.cy;
	//Parent->Invalidate(0);
	return true;
}

void CProjectionTask::OnMouseLDC(UINT, CPoint pt)
{
	if (pl)	pl->SetSelected(FALSE);
	if (pt1)	pt1->SetSelected(FALSE);
	if (pt2)	pt2->SetSelected(FALSE);
	if (pt3)	pt3->SetSelected(FALSE);
	pt1 = pt2 = pt3 = NULL;
	pl = NULL;
	m_nStep = 0;
	InvalidateParent();
	//m_pParent->Invalidate();
	GetContextualHelp();
}
