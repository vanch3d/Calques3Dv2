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
// CubeTask.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Cercle3D.h"
#include "..\Objects\CompositeObj3D.h"

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

CCube3DTask::CCube3DTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
	ptA = ptB = ptS = NULL;
	cr = NULL;
}

CCube3DTask::~CCube3DTask()
{
	if (cr)	delete cr;
	if (ptS) delete ptS;
	cr = NULL;
	ptS = NULL;
	if (ptA)	ptA->SetSelected(FALSE);
	if (ptB)	ptB->SetSelected(FALSE);
	if (ptA || ptB)	m_pParent->Invalidate(FALSE);

}

CObjectId CCube3DTask::GetMask()
{
	return TAllPointClass;
}

unsigned CCube3DTask::GetHelpResID()
{
	if (m_nStep==0)
		return CTX_SELECT_POINT1;
	else if (m_nStep==1)
		return CTX_SELECT_POINT2;
	else if (m_nStep==2)
		return CTX_CUBE_ONCIRCLE;
	else
		return CTX_CUBE_ORIENTATE;
}

void CCube3DTask::OnMouseL(UINT , CPoint thepos)
{
	CObject3D *temp=0;

	int index = FindObject(thepos,GetMask(),FALSE);
	CVisualParam *mv = m_pParent->GetVisualParam();

	switch (m_nStep)
	 {
		case 0 :	//désignation du premier point
		{
			if (!index) 
			{
				GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,NULL);
				return;
			}
			temp = GetObject();
			if (!temp) break;
			ptA = (CPoint3D*) temp;
			m_ptOld = ptA->Visual_pt;
			m_nStep++;
			break;
		}
		case 1 :	//désignation du deuxieme point
		{
			if (!index) return;
			temp = GetObject();
			if (!temp) break;
			if (temp == ptA)
			 {	
				ShowErrorMsg(ERR_2SAMEPTS);
				return;
			 }
			ptB = (CPoint3D*) temp;

			CVector4 U = (ptB->Concept_pt - ptA->Concept_pt);
			FCoord r = U.Norme();
			if (FCZero(r)) return;
			if (m_nTaskID == ID_OBJECT_CUBE)
			 {
				cr = new CCercle3D(ptB,U,r);
				cr->pObjectShape.clrObject = RGB(0,0,0);
				cr->pObjectShape.nShapeId = 0;
				cr->CalculConceptuel();
				//mask = TCercle3DClass;
			 }
/*			else
			 {
				cr = new TPlan3D(0,ptB,U);
				cr->ObjectShape->ObjectColor = TColor(0,0,0);
				cr->ObjectShape->AShape = 0;
				cr->CalculConceptuel();
				ptS = new TPointSurP3D(0,(TPlan3D*)cr);
				//((TPointSurP3D *)Ps)->alpha = a;
				//((TPointSurP3D *)Ps)->beta = b;
				Vector4 TempCpt;
				if (ptS->MovePoint(Parent->GetViewParam(),0,thepos,TempCpt))
				 {	ptS->Concept_pt = TempCpt;
					ptS->CalculVisuel(Parent->GetViewParam());
				  }
				Step++;
			 }*/
			m_nStep++;
			m_pParent->Invalidate(FALSE);
			m_ptOld = ptB->Visual_pt;
			//oldP.x-=(int) ((Parent->Scroller) ? Parent->Scroller->XPos : 0);
			//oldP.y-=(int) ((Parent->Scroller) ? Parent->Scroller->YPos : 0);
		 }
		 break;
		case 2 :	//désignation d'un point du cercle (pour le cube)
		  {
			if (!cr) return;
			 {
				//mask = TAllPointClass;
				ptS = new CPointSurC3D(cr);
				CVector4 TempCpt;
				if (ptS->MoveObject(mv,0,thepos,TempCpt))
				 {	
					ptS->Concept_pt = TempCpt;
					ptS->CalculVisuel(mv);
					//ptS->Draw(*DragDC);
					m_nStep++;
					m_pParent->Invalidate(0);
				 }
				else
				 {
					delete ptS;
					ptS = NULL;
				 }
			 }
			}
			break;
		case 3 :	// fin de la création du cube
			CreateObject3D();
			break;
		default :
			break;
	 }
	if (temp)
	 {	
		temp->SetSelected(TRUE);
		GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
		m_pParent->Invalidate(FALSE);
	}
		//Parent->Invalidate(0);
	GetContextualHelp();
}

void CCube3DTask::OnMouseMove(UINT, CPoint thepos)
{
	FindObject(thepos,GetMask(),TRUE);
	CPoint TheOld = m_ptOld;
	switch (m_nStep)
	 {
		case 3 :
		  {
			CVector4 TempCpt;
			if (((CPointSurC3D*)ptS)->MoveObject(m_pParent->GetVisualParam(),
										0,thepos,TempCpt))
			 {	((CPointSurC3D*)ptS)->Concept_pt = TempCpt;
				((CPointSurC3D*)ptS)->CalculVisuel(m_pParent->GetVisualParam());
			 }
		  }
		  break;
		case 2 :	//désignation du premier point
		  /*	if (cr)
			if (cr->IsInActiveArea(thepos))
				Geom3DApp->SetToolTip("Ce cercle");
			else
				Geom3DApp->SetToolTip(0);*/
			break;
		case 1 :	//désignation du deuxieme point
		default:
			break;
	 }
	m_ptOld = thepos;
	if (m_nStep)
		m_pParent->Invalidate(FALSE);
}

void CCube3DTask::OnMouseLUp(UINT, CPoint)
{
}

void CCube3DTask::CreateObject3D()
{
	if (!ptS) return;

	
	CObject3D *temp = 0;
	if (m_nTaskID == ID_OBJECT_CUBE)
		temp = new CCube3D(ptA,ptB,((CPointSurC3D*)ptS)->lambda);
/*	else
		temp = new CCylindre3D(ptA,ptB,
				((CPointSurP3D *)ptS)->alpha,
				((CPointSurP3D *)ptS)->beta);*/

	UINT a = temp->CalculConceptuel();
	if (a)
	 {	
		temp->HandleObjectError(a,TRUE);
		delete temp;
	 }
	else
		PrepareAddedObject(temp);

	if (cr)	delete cr;
	if (ptS) delete ptS;
	cr = NULL;
	ptS = NULL;
	ptA->SetSelected(FALSE);
	ptB->SetSelected(FALSE);
	ptA=ptB=NULL;
	m_nStep=0;
	m_pParent->Invalidate(FALSE);
	GetContextualHelp();
}

void CCube3DTask::drawFirstSeg(CDC* pDC,CPoint thepos)
{
	pDC->MoveTo(ptA->Visual_pt);
	pDC->LineTo(thepos);
}

void CCube3DTask::drawCircle(CDC* pDC,CPoint thepos)
{
	CVisualParam *myVisuParam = m_pParent->GetVisualParam();
	drawFirstSeg(pDC,ptB->Visual_pt);
	pDC->MoveTo(ptB->Visual_pt);
	pDC->LineTo(thepos);
	cr->CalculVisuel(myVisuParam);
	cr->Draw(pDC,myVisuParam);
}

void CCube3DTask::drawFirstSide(CDC* pDC,CPoint )
{
	CVisualParam *myVisuParam = m_pParent->GetVisualParam();
	drawCircle(pDC,ptS->Visual_pt);
	ptS->Draw(pDC,myVisuParam);
	CPoint a(ptA->Visual_pt);
	CPoint b(ptB->Visual_pt);
	CPoint s(ptS->Visual_pt);
	CPoint j = a + (s-b);
	pDC->MoveTo(a);
	pDC->LineTo(j);
	pDC->MoveTo(j);
	pDC->LineTo(s);

}

void CCube3DTask::DrawFeedBack(CDC* pDC)
{
	CPoint thepos = m_ptOld;
	switch (m_nStep)
	 {	case 1 :	//premier point désigné
			drawFirstSeg(pDC,thepos);
			break;
		case 2 :	//deuxieme point désigné
			drawCircle(pDC,thepos);
			break;
		case 3 : //point sur cercle construit
			drawFirstSide(pDC,thepos);
			break;
		default:
			break;
	 }

}
