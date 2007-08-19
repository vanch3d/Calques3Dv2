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
// MilieuTask.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Droite3D.h"
#include "..\Objects\CompositeObj3D.h"
//#include "..\Objects\Plan3D.h"

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

CMilieu3DTask::CMilieu3DTask(CView *AParent,UINT taskID):CDroite3DTask(AParent,taskID)
{
	seg = NULL;
}
	
CMilieu3DTask::~CMilieu3DTask()
{
	if (ptA)	ptA->SetSelected(FALSE);
	if (ptB)	ptB->SetSelected(FALSE);
	if (seg)	seg->SetSelected(FALSE);
	if (ptA || ptB || seg)	
		InvalidateParent();
		//m_pParent->Invalidate();
}

void CMilieu3DTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp;

//	DWORD mask;
	int index=0;
	index = FindObject(thepos,GetMask(),FALSE);
//	if (!ptA && !index) //mask = TAllPointClass;
//		index = FindObject(thepos,TSegment3DClass,FALSE);
	if (!index) return;

	temp = GetObject();
	if (!temp) return;
	if (temp->isA() == TSegment3DClass)
	 {
		seg = (CSegment3D*)temp;
		CreateObject3D();
		return;
	 }
	if (!ptA)
		ptA = (CPoint3D*) temp;
	else
		if (temp != ptA)
			ptB = (CPoint3D*) temp;
		else
		 {
			ShowErrorMsg(ERR_2SAMEPTS);
			return;
		 }
	temp->SetSelected(TRUE);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);

	if ((ptA) && (ptB))
	{
		CreateObject3D();
	}
	if (ptA)
	{
		m_ptOld = ptA->Visual_pt;
		m_nStep++;
	}
	m_pParent->Invalidate(0);
	GetContextualHelp();
}

void CMilieu3DTask::OnMouseMove(UINT, CPoint thepos)
{
	int index=0;
	index = FindObject(thepos,GetMask(),TRUE);
/*	if (!ptA && !index)
	{
		index = FindObject(thepos,TSegment3DClass,TRUE);
	}*/
	m_ptOld = thepos;
	if (ptA)
		m_pParent->Invalidate(FALSE);

}

unsigned CMilieu3DTask::GetHelpResID()
{
	if (m_nTaskID==ID_CONSTRUCTION_DIVSEGMENT)
		return CTX_SELECT_DROITE;
	else
		return ((m_nStep) ? CTX_SELECT_POINT2 : CTX_SELECT_POINTSEG);
}

CObjectId CMilieu3DTask::GetMask()
{
	if (m_nTaskID==ID_CONSTRUCTION_DIVSEGMENT)
		return TAllSegmentClass;
	else
		return ((m_nStep) ? TAllPointClass : TAllPointClass|TAllSegmentClass);

}




void CMilieu3DTask::CreateObject3D()
{
	//CPointMilieu3D* temp;
	CObject3D* temp;

	if ((ptA) && (ptB))
		temp = new CPointMilieu3D(ptA,ptB);
	else
	if (seg)
	{
		if (m_nTaskID==ID_CONSTRUCTION_DIVSEGMENT)
			temp = new CDivSegment3D(seg);
		else
			temp = new CPointMilieu3D(seg);
		
	}
	else
		temp = 0;

	if (temp)
	 {
		temp->CalculConceptuel();
		BOOL test = PrepareAddedObject(temp);
		if (test && temp)
			temp->CalculVisuel(m_pParent->GetVisualParam());
		if (ptA)	ptA->SetSelected(FALSE);
		if (ptB)	ptB->SetSelected(FALSE);
		if (seg)	seg->SetSelected(FALSE);
		ptA=ptB=NULL;
		seg = NULL;
		m_nStep = 0;
		m_pParent->Invalidate(0);
	}
	GetContextualHelp();
}


void CMilieu3DTask::DrawFeedBack(CDC *pDC)
{
	if (!ptA) return;

	CPoint thePt = ptA->Visual_pt;
	thePt.x += m_ptOld.x;
	thePt.y += m_ptOld.y;
	thePt.x /=2;
	thePt.y /=2;

	CPen curPen(PS_SOLID,1,TPref::TUniv.clrFeedback);
    CPen *oldP = pDC->SelectObject(&curPen);
	pDC->Ellipse(thePt.x-3,thePt.y-3,thePt.x+3,thePt.y+3);
	pDC->SelectObject(oldP);

}
