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
// DrParTask.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Droite3D.h"
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

CParallele3DTask::CParallele3DTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
	pt = 0;
	dr = 0;
	pl = 0;
	//IsShift = 0;
}
	
CParallele3DTask::~CParallele3DTask()
{
	if (pt)	pt->SetSelected(FALSE);
	if (dr)	dr->SetSelected(FALSE);
	if (pl)	pl->SetSelected(FALSE);
	if (dr || pt || pl)	m_pParent->Invalidate(0);
}

unsigned CParallele3DTask::GetHelpResID()
{
	unsigned mask;
	if (!dr)	mask=	CTX_SELECT_DROITE;
	else		mask = CTX_SELECT_POINT1;
	return mask;
}


DWORD CParallele3DTask::GetMask()
{
	DWORD mask;
	if (!dr)	mask=	TAllDroiteClass;
	else		mask = TAllPointClass;
	return mask;
}
	
void CParallele3DTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp;

	int index = FindObject(thepos,GetMask(),FALSE);
	if (!index) return;
	temp = GetObject();
	if (!temp) return;

	if (temp->MaskObject(TAllPlanClass))
		pl = (CPlan3D *) temp;
	else
	if (temp->MaskObject(TAllDroiteClass))
		dr = (CDroite3D *) temp;
	else
		pt = (CPoint3D *)temp;

	temp->SetSelected(TRUE);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
//	if (GetMask() == TAllPointClass)
//			temp->CalculVisuel(Parent->GetViewParam());
//	string toto(GetContextualHelp());
//	OnLineHelp->SetText(toto.c_str());
	GetContextualHelp();
	m_pParent->Invalidate(0);

	if (((pt) && (dr)) || ((pt) && (pl)))
	 {
		//IsShift = (flag & MK_SHIFT);
		CreateObject3D();
	 }
	if (dr || pl)
	 {
		m_ptOld = thepos;
		m_nStep++;
		GetContextualHelp();
	  }
}

void CParallele3DTask::OnMouseMove(UINT, CPoint thepoint)
{
	FindObject(thepoint,GetMask(),TRUE);
	m_ptOld = thepoint;
	if (dr || pl)
		m_pParent->Invalidate(FALSE);
}

void CParallele3DTask::CreateObject3D()
{
	if (!dr|| !pt) return;
	CObject3D *temp = new CDroitePar3D(pt,dr);
	((CDroitePar3D*)temp)->CalculConceptuel();
	PrepareAddedObject(temp);

	if (dr) dr->SetSelected(FALSE);
	if (pt) pt->SetSelected(FALSE);
	pt = 0;
	dr = 0;
	m_nStep=0;
	GetContextualHelp();
	m_pParent->Invalidate(0);

}

void CParallele3DTask::DrawFeedBack(CDC *pDC)
{
	if (!dr) return;
	CVector4 T = dr->GetDirVector();
	CVector4 T2 = m_pParent->GetVisualParam()->ProjectPoint(T);
	//Vector4 T4 = {0,0,0,1};
	//Vector4  T3 = ((PTWindow3D) Parent)->myVisuParam->ProjectPoint(T4);

	//T2.x -= T3.x;
	//T2.y -= T3.y;
	T2.x -= m_pParent->GetVisualParam()->ptRepCoord.x;
	T2.y -= m_pParent->GetVisualParam()->ptRepCoord.y;// - T2.y;
	long double norm = 50 / sqrt(T2.x*T2.x + T2.y*T2.y);
	T2.x = T2.x * norm;
	T2.y = T2.y * norm;

				//T.x+=(int) Parent->Scroller->XPos;
			//T.y+=(int) Parent->Scroller->YPos;
	CPoint t1,t2;
	t1 = m_ptOld + (CPoint)T2;
	t2 = m_ptOld - (CPoint)T2;
//	t1.x = m_ptOld.x + T2.x;
//	t1.y = m_ptOld.y + T2.y;
//	t2.x = m_ptOld.x - T2.x;
//	t2.y = m_ptOld.y - T2.y;
	pDC->MoveTo(t1);
	pDC->LineTo(t2);
/*	t1.x = thepos.x + T2.x;
	t1.y = thepos.y + T2.y;
	t2.x = thepos.x - T2.x;
	t2.y = thepos.y - T2.y;
	DragDC.MoveTo(t1.x,t1.y);
	oldP.x = thepos.x;
	oldP.y = thepos.y;
	DragDC.LineTo(t2.x,t2.y);  */
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPerpendic3DTask::CPerpendic3DTask(CView *AParent,UINT taskID):
		CParallele3DTask(AParent,taskID)
{
}

unsigned CPerpendic3DTask::GetTaskResID() const 
{ 
	if (m_nTaskID== ID_CONSTRUCTION_NORMALLINE)
		return CTX_DROITE_NORMAL; 
	if (m_nTaskID== ID_CONSTRUCTION_PERPENDICULARPLANE)
		return CTX_PLAN_PERP; 
	//if (m_nTaskID== ID_CONSTRUCTION_PERPENDICULARLINE)
	return CTX_DROITE_PERP; 
}

		
unsigned CPerpendic3DTask::GetHelpResID()
{
	DWORD mask;
	if ((!dr) && (!pl))
		mask=	(m_nTaskID== ID_CONSTRUCTION_NORMALLINE) ? CTX_SELECT_PLAN1 : CTX_SELECT_DROITE;
	else
		mask = CTX_SELECT_POINT1;
	return mask;
}


		
DWORD CPerpendic3DTask::GetMask()
{
	DWORD mask;
	if ((!dr) && (!pl))
		mask=	(m_nTaskID== ID_CONSTRUCTION_NORMALLINE) ? TAllPlanClass : TAllDroiteClass;
	else
		mask = TAllPointClass;
	return mask;
}

void CPerpendic3DTask::CreateObject3D()
{
	CObject3D *temp=0;
	int h=0;
	if (pl)	temp = new CDroitePerp3D(pt,pl);
	else
//	if (dr)	temp = new TDroitePerp3D(Parent,pt,dr);
	if (dr)
	 {
		if (m_nTaskID == ID_CONSTRUCTION_PERPENDICULARPLANE)
			temp = new CPlanPerp3D(pt,dr);
		else
			temp = new CDroitePerp3D(pt,dr);
	 }
	else
		temp = 0;

	if (!temp) return;

	h=temp->CalculConceptuel();
	if (h)
	 {
		((CPlan3D*)temp)->HandleObjectError(h,TRUE);
		delete temp;
	 }
	else
	 {
/*		bool test = ((calque3dDoc&)Parent->GetDocument()).VerifyUnicity(temp);
		if (!test)
		 {
			((calque3dDoc&)Parent->GetDocument()).AddObject(temp);
			AdjustMenu();
			if (TPref::GiveAName)	temp->SetProperties();
		 }
		else
		 {
			temp->HandleObjectError(ERR_THEREISDR,TRUE);
			delete temp;
		 }*/
		BOOL test = PrepareAddedObject(temp);

	 }
	if (dr) dr->SetSelected(FALSE);
	if (pl) pl->SetSelected(FALSE);
	if (pt) pt->SetSelected(FALSE);
	pt= 0;
	pl = 0;
	dr=0;
	GetContextualHelp();
	m_pParent->Invalidate(0);
}

void CPerpendic3DTask::DrawFeedBack(CDC *pDC)
{
	if (!dr && !pl) return;

	CPoint t1,t2;
	if (dr)
	 {
		//// plan perpendiculaire
		if (m_nTaskID == ID_CONSTRUCTION_PERPENDICULARPLANE)
		 {
			//Vector4 newPt = Parent->GetViewParam()->GetProjectedPoint(theP);
			CPoint3D tempoPt(dr->GetBasePoint());
			tempoPt.CalculConceptuel();
			CPlanPerp3D tempoPl(&tempoPt,dr);
			tempoPl.CalculConceptuel();
			tempoPl.CalculVisuel(m_pParent->GetVisualParam());
			//tempoPl->ObjectShape->AShape=1;
			//tempoPl->Draw(DragDC,Parent->GetViewParam(),1);
			//HandleDC.SelectObject(curPen);
			CPoint center((tempoPl.tp1.x+tempoPl.tp3.x)/2,
								(tempoPl.tp1.y+tempoPl.tp3.y)/2);
			CSize delta = center-m_ptOld;
			pDC->MoveTo(tempoPl.tp1-delta);
			pDC->LineTo(tempoPl.tp2-delta);
			pDC->LineTo(tempoPl.tp3-delta);
			pDC->LineTo(tempoPl.tp4-delta);
			pDC->LineTo(tempoPl.tp1-delta);
		 }
		else //// droite normale
		 {
			CVector4 T = dr->GetDirVector();
			CVector4 T2 = m_pParent->GetVisualParam()->ProjectPoint(T);
			CVector4 T3 = T2;
			T2.x -= m_pParent->GetVisualParam()->ptRepCoord.x;
			T2.y -= m_pParent->GetVisualParam()->ptRepCoord.y;// - T2.y;
			long double norm = 50 / sqrt(T2.x*T2.x + T2.y*T2.y);
			T2.x = T2.x * norm;
			T2.y = T2.y * norm;

			T3 = T2;

			T2.x = -T3.y;
			T2.y = T3.x;

			//T.x+=(int) Parent->Scroller->XPos;
		//T.y+=(int) Parent->Scroller->YPos;
			t1 = m_ptOld + (CPoint)T2;
			t2 = m_ptOld - (CPoint)T2;
			//t1.x = m_ptOld.x + T2.x;
			//t1.y = m_ptOld.y + T2.y;
			//t2.x = m_ptOld.x - T2.x;
			//t2.y = m_ptOld.y - T2.y;
			pDC->MoveTo(t1);
			pDC->LineTo(t2);
		}
	 }
	else 			//// droite perpendiculaire
	if (pl)
	 {
		CVector4 T = pl->VecNorm;
		CVector4 T2 = m_pParent->GetVisualParam()->ProjectPoint(T);

		T2.x -= m_pParent->GetVisualParam()->ptRepCoord.x;
		T2.y -= m_pParent->GetVisualParam()->ptRepCoord.y;// - T2.y;
		long double norm = 50 / sqrt(T2.x*T2.x + T2.y*T2.y);
		T2.x = T2.x * norm;
		T2.y = T2.y * norm;

			//T.x+=(int) Parent->Scroller->XPos;
		//T.y+=(int) Parent->Scroller->YPos;
			t1 = m_ptOld + (CPoint)T2;
			t2 = m_ptOld - (CPoint)T2;
		//t1.x = m_ptOld.x + T2.x;
		//t1.y = m_ptOld.y + T2.y;
	//	t2.x = m_ptOld.x - T2.x;
	//	t2.y = m_ptOld.y - T2.y;
		pDC->MoveTo(t1);
		pDC->LineTo(t2);
	 }

}

