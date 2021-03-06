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
// Intersect.cpp: implementation of the CTask class.
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
#include "..\Objects\Sphere3D.h"
#include "..\Objects\CompositeObj3D.h"
#include "..\Objects\Cylinder3D.h"
#include "..\Objects\Cercle3D.h"
#include "..\Objects\Cone3D.h"
#include "..\Objects\ConicSection3D.h"

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

CInter3DTask::CInter3DTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
	dr1 = dr2 = NULL;
	p = pl2 = NULL;
	p1 = p2 = p3 = NULL;
	sp = NULL;
	sp2 = NULL;
	circle = NULL;
	cyl = NULL;
	cone = NULL;
	m_nStep = 0;
	GetContextualHelp();
}

CInter3DTask::~CInter3DTask()
{
	if (dr1)	dr1->SetSelected(FALSE);
	if (dr2)	dr2->SetSelected(FALSE);
	if (p)	p->SetSelected(FALSE);
	if (pl2)	pl2->SetSelected(FALSE);
	if (sp)	sp->SetSelected(FALSE);
	if (sp2)	sp2->SetSelected(FALSE);
	if (cyl)	cyl->SetSelected(FALSE);
	if (circle)	circle->SetSelected(FALSE);
	if (cone)	cone->SetSelected(FALSE);
	if (dr1 || dr2 || pl2 || p || sp || cyl || sp2 || circle || cone)
		m_pParent->Invalidate(0);
}

unsigned CInter3DTask::GetTaskResID() const
{ 
	if (m_nTaskID==ID_CONSTRUCTION_CROSSPRODUCT)
		return CTX_CROSSPRODUCT; 
	else
		return CTX_INTERSECTION; 
}

unsigned CInter3DTask::GetHelpResID()
{
  UINT mask;
  switch (m_nTaskID)
	{	case ID_CONSTRUCTION_INTERSECTION_LINELINE:
  		case ID_CONSTRUCTION_CROSSPRODUCT:
				mask = (m_nStep) ? CTX_SELECT_DROITE2 : CTX_SELECT_DROITE;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINECIRCLE:
				mask = (m_nStep) ? CTX_SELECT_CIRCLE : CTX_SELECT_DROITE;
				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECIRCLE:
				mask = (m_nStep) ? CTX_SELECT_CIRCLE : CTX_SELECT_PLAN1;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINEPLANE:
				//mask = (dr1) ? TAllPlanClass : TAllDroiteClass;
				mask = (m_nStep) ? CTX_SELECT_PLAN1 : CTX_SELECT_DROITE;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINESPHERE:
				//mask = (dr1) ? TSphere3DClass : TAllDroiteClass;
				mask = (m_nStep) ? CTX_SELECT_SPHERE : CTX_SELECT_DROITE;
				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANEPLANE:
				mask = (m_nStep) ? CTX_SELECT_PLAN2 : CTX_SELECT_PLAN1;
				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECYLINDER:
				mask = (m_nStep) ? CTX_SELECT_CYL : CTX_SELECT_PLAN1;
				break;
		case ID_CONSTRUCTION_INTERSECTION_SPHERESPHERE:
				mask = (m_nStep) ? CTX_SELECT_SPHERE : CTX_SELECT_SPHERE;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINECONE:
				mask = (m_nStep) ? CTX_SELECT_CONE : CTX_SELECT_DROITE;
				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECONE:
				mask = (m_nStep) ? CTX_SELECT_CONE : CTX_SELECT_PLAN1;
				break;
	}
	return mask;
}


CObjectId CInter3DTask::GetMask()
{
  CObjectId mask;
  switch (m_nTaskID)
	{	case ID_CONSTRUCTION_INTERSECTION_LINELINE:
  		case ID_CONSTRUCTION_CROSSPRODUCT:
				mask = TAllDroiteClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINEPLANE:
				mask = (dr1) ? TAllPlanClass : TAllDroiteClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINESPHERE:
				mask = (dr1) ? TSphere3DClass : TAllDroiteClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINECIRCLE:
				mask = (dr1) ? TAllCercleClass : TAllDroiteClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECIRCLE:
				mask = (p) ? TAllCercleClass : TAllPlanClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANEPLANE:
				mask = TAllPlanClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECYLINDER:
				mask = (p) ? TCylindre3DClass : TAllPlanClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_SPHERESPHERE:
				mask = (p) ? TSphere3DClass : TSphere3DClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_SPHEREPLANE:
				mask = (sp) ? TAllPlanClass : TSphere3DClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINECONE:
				mask = (dr1) ? TCone3DClass : TAllDroiteClass;
				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECONE:
				mask = (p) ? TCone3DClass : TAllPlanClass;
				break;
	}
	return mask;
}
	
void CInter3DTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp = NULL;

	int index = FindObject(thepos,GetMask(),FALSE);
	temp = GetObject();
	if (!temp) return;

	switch (m_nTaskID)
	{	case ID_CONSTRUCTION_INTERSECTION_LINELINE:
  		case ID_CONSTRUCTION_CROSSPRODUCT:
			if (!dr1)
				dr1 = (CDroite3D*) temp;
			else
			 { if (temp != dr1)
					dr2 = (CDroite3D*) temp;
				else
				 {
					ShowErrorMsg(ERR_2SAMEDRS);
					return;
				 }
			 }
			break;
		case ID_CONSTRUCTION_INTERSECTION_LINEPLANE:
			if (GetMask() == TAllDroiteClass)
				dr1 = (CDroite3D*) temp;
			else
				p = (CPlan3D *) temp;
				break;
		case ID_CONSTRUCTION_INTERSECTION_LINESPHERE:
			if (GetMask() == TAllDroiteClass)
				dr1 = (CDroite3D*) temp;
			else
				sp = (CSphere3D *) temp;

				break;
		case ID_CONSTRUCTION_INTERSECTION_LINECONE:
			if (GetMask() == TAllDroiteClass)
				dr1 = (CDroite3D*) temp;
			else
				cone = (CCone3D *) temp;

				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECONE:
			if (GetMask() == TAllPlanClass)
				p = (CPlan3D*) temp;
			else
				cone = (CCone3D *) temp;

				break;
		case ID_CONSTRUCTION_INTERSECTION_LINECIRCLE:
			if (GetMask() == TAllDroiteClass)
				dr1 = (CDroite3D*) temp;
			else
				circle = (CCercle3D *) temp;

				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECIRCLE:
			if (GetMask() == TAllPlanClass)
				p = (CPlan3D*) temp;
			else
				circle = (CCercle3D *) temp;

				break;
		case ID_CONSTRUCTION_INTERSECTION_PLANECYLINDER:
			if (GetMask() == TAllPlanClass)
				p = (CPlan3D*) temp;
			else
				cyl = (CCylinder3D*) temp;
			break;
		case ID_CONSTRUCTION_INTERSECTION_PLANEPLANE:
			if (!p)
				p = (CPlan3D *) temp;
			else
			 { if (temp != p)
					pl2 = (CPlan3D*) temp;
				else
				 {
					ShowErrorMsg(ERR_2SAMEPLS);
					return;
				 }
			 }
			break;
		case ID_CONSTRUCTION_INTERSECTION_SPHERESPHERE:
			if (!sp)
				sp = (CSphere3D*) temp;
			else
			 { if (temp != sp)
					sp2 = (CSphere3D*) temp;
				else
				 {
					ShowErrorMsg(ERR_2SAMEPLS);
					return;
				 }
			 }
			break;
				break;
		case ID_CONSTRUCTION_INTERSECTION_SPHEREPLANE:
			if (GetMask() == TAllPlanClass)
				p = (CPlan3D*) temp;
			else
				sp = (CSphere3D *) temp;

				break;
		default:
			return;
	}
	temp->SetSelected(TRUE);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
	m_pParent->Invalidate(0);

	m_nStep++;
	GetContextualHelp();

	if (((dr1) && (dr2 || p || sp || circle || cone)) ||
		 (p && (pl2|| circle || cone)) || (p && cyl) || (sp && (sp2||p)))
	{
		CreateObject3D();
		return;
	}
}

void CInter3DTask::OnMouseR(UINT, CPoint)
{
}

void CInter3DTask::OnMouseMove(UINT, CPoint pt)
{
	int nb = FindObject(pt,GetMask());
}

void CInter3DTask::CreateObject3D()
{
	CObject3D* temp;
	int res;
	if ((dr1) && (dr2))
	 {
  		if (m_nTaskID==ID_CONSTRUCTION_CROSSPRODUCT)
			temp = new CDroitePerpDD3D(dr1,dr2);
		else
			temp = new CPointInterDD3D(dr1,dr2);
		//res = ((CPointInterDD3D*)temp)->CalculConceptuel();
		res = temp->CalculConceptuel();
	 }
	else if ((dr1) && (p))
	 {
		temp = new CPointInterDP3D(dr1,p);
		res = ((CPointInterDP3D*)temp)->CalculConceptuel();
	 }
	else if ((dr1) && (sp))
	 {
		temp = new CInterSphDr3D(sp,dr1);
		res = ((CInterSphDr3D*)temp)->CalculConceptuel();
	 }
	else if ((dr1) && (cone))
	 {
		temp = new CInterConeDr3D(cone,dr1);
		res = ((CInterConeDr3D*)temp)->CalculConceptuel();
	 }
	else if ((dr1) && (circle))
	 {
		temp = new CInterCircDr3D(circle,dr1);
		res = ((CInterCircDr3D*)temp)->CalculConceptuel();
	 }
	else if ((p) && (circle))
	 {
		temp = new CInterCircPlane3D(circle,p);
		res = ((CInterCircDr3D*)temp)->CalculConceptuel();
	 }
	else if ((p) && (pl2))
	 {
		temp = new CDroiteInterPP3D(p,pl2);
		res = ((CDroiteInterPP3D*)temp)->CalculConceptuel();
	 }
	else if ((p) && (cyl))
	 {
		temp = new CEllipse3D(cyl,p);
		res = ((CEllipse3D*)temp)->CalculConceptuel();
	 }
	else if ((p) && (cone))
	 {
		temp = new CConicSection3D(cone,p);
		res = ((CConicSection3D*)temp)->CalculConceptuel();
	 }
	else if ((sp) && (sp2))
	 {
		temp = new CCercleInterSS3D(sp,sp2);
		res = ((CCercleInterSS3D*)temp)->CalculConceptuel();
	 }
	else if ((sp) && (p))
	 {
		temp = new CCercleInterPS3D(p,sp);
		res = ((CCercleInterPS3D*)temp)->CalculConceptuel();
	 }
	else return;

	if (res)
	 {
		temp->HandleObjectError(res,TRUE);
		delete temp;
	 }
	else
	 {
/*		bool test = ((calque3dDoc&)Parent->GetDocument()).VerifyUnicity(temp);
		if (test)
		 {
			temp->HandleObjectError(ERR_THEREISPT,TRUE);
			delete temp;
		 }
		else
		 {
			((calque3dDoc&)Parent->GetDocument()).AddObject(temp);
			AdjustMenu();
			if (TPref::GiveAName)	temp->SetProperties();
		 }*/
		PrepareAddedObject(temp);
	 }
	if (dr1)	dr1->SetSelected(FALSE);
	if (dr2) dr2->SetSelected(FALSE);
	if (pl2)	pl2->SetSelected(FALSE);
	if (p)	p->SetSelected(FALSE);
	if (sp)	sp->SetSelected(FALSE);
	if (sp2)	sp2->SetSelected(FALSE);
	if (cyl)	cyl->SetSelected(FALSE);
	if (circle)	circle->SetSelected(FALSE);
	if (cone)	cone->SetSelected(FALSE);
	InvalidateParent();
	//m_pParent->Invalidate();
	dr1 = dr2 = NULL;
	p = pl2 = NULL;
	p1 = p2 = p3 = NULL;
	sp = NULL;
	sp2 = NULL;
	cyl = NULL;
	circle = NULL;
	cone = NULL;
	m_nStep = 0;
	GetContextualHelp();
}

