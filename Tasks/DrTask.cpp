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
// DrTask.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Droite3D.h"
#include "..\Objects\Sphere3D.h"

#include "..\Objects\Locus3D.h"


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

CDroite3DTask::CDroite3DTask(CView *AParent,UINT nID) : CTask(AParent,nID)
{
	ptA = 0;
	ptB = 0;
	GetContextualHelp();

}

CDroite3DTask::~CDroite3DTask()
{
	if (ptA)	ptA->SetSelected(FALSE);
	if (ptB)	ptB->SetSelected(FALSE);
	if (ptA || ptB)	
		InvalidateParent();
		//m_pParent->Invalidate();
}

void CDroite3DTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp=0;

	int index = FindObject(thepos,GetMask(),FALSE);
	if (!index) 
	{
		if (!ptA)
			GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,NULL);
		return;
	}
	temp = GetObject();

	if (!ptA)
		ptA = (CPoint3D*)temp;
	else if (temp != ptA)	
		ptB = (CPoint3D*)temp;
	else
	{ 
		ShowErrorMsg(ERR_2SAMEPTS);
		return;
	}

	if (temp)
	 {
		temp->SetSelected(TRUE);
		GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
		temp->CalculVisuel(m_pParent->GetVisualParam());
		m_pParent->Invalidate(0);
	 }
	if ((ptA) && (ptB))
	 {
		CreateObject3D();
	 }
	if (ptA)
	 {
		m_ptOld = ptA->Visual_pt;
		m_nStep++;
		GetContextualHelp();
	}
}

void CDroite3DTask::OnMouseR(UINT modkey, CPoint thepos)
{
	DoContextMenu(modkey,thepos);
}
	
void CDroite3DTask::OnMouseMove(UINT, CPoint thepoint)
{
	FindObject(thepoint,GetMask(),TRUE);
	m_ptOld = thepoint;
	if (ptA)
		m_pParent->Invalidate(FALSE);
}

DWORD CDroite3DTask::GetMask()
{
	return TAllPointClass;
}

unsigned CDroite3DTask::GetHelpResID()
{
	return ((m_nStep) ? CTX_SELECT_POINT2 : CTX_SELECT_POINT1);
}

void CDroite3DTask::CreateObject3D()
{
	CDroite3D* temp = new CDroite3D(ptA,ptB);
	temp->CalculConceptuel();
	PrepareAddedObject(temp);

	if (ptA) ptA->SetSelected(FALSE);
	if (ptB) ptB->SetSelected(FALSE);
	InvalidateParent();
	//m_pParent->Invalidate();
	ptA=ptB=NULL;
	m_nStep = 0;
}

void CDroite3DTask::DrawFeedBack(CDC *pDC)
{
	if (!ptA) return;

	CPoint thePt = ptA->Visual_pt;
	CPoint endPt(m_ptOld.x,m_ptOld.y);
	CSize dl = endPt - thePt;
	dl.cx =dl.cx/4;
	dl.cy =dl.cy/4;
	thePt-=dl;
	endPt+=dl;

	pDC->MoveTo(thePt);
	pDC->LineTo(endPt);

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSegment3DTask::CSegment3DTask(CView *AParent,UINT nID) : CDroite3DTask(AParent,nID)
{
}

unsigned CSegment3DTask::GetTaskResID() const
{ 
	if (m_nTaskID == ID_OBJECT_RAY)
		return CTX_RAY; 
	else
		return CTX_SEGMENT;
}


void CSegment3DTask::DrawFeedBack(CDC* pDC)
{
	if (!ptA) return;

	CPoint thePt = ptA->Visual_pt;
	CPoint endPt(m_ptOld.x,m_ptOld.y);
	CSize dl = endPt - thePt;
	dl.cx =dl.cx/2;
	dl.cy =dl.cy/2;
	if (m_nTaskID == ID_OBJECT_RAY)
		endPt+=dl;

	pDC->MoveTo(thePt);
	pDC->LineTo(endPt);
}

void CSegment3DTask::CreateObject3D()
{
	CObject3D* temp=NULL;
	if (m_nTaskID == ID_OBJECT_RAY)
		temp = new CDemiDroite3D(ptA,ptB);
	else
		temp = new CSegment3D(ptA,ptB);

	temp->CalculConceptuel();
	PrepareAddedObject(temp);

	if (ptA) ptA->SetSelected(FALSE);
	if (ptB) ptB->SetSelected(FALSE);
	InvalidateParent();
	//m_pParent->Invalidate();
	ptA=ptB=NULL;
	m_nStep = 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSphere3DTask::CSphere3DTask(CView *AParent,UINT taskID): CDroite3DTask(AParent,taskID)
{
}

void CSphere3DTask::CreateObject3D()
{
	CSphere3D* temp = NULL;
	if (ptB->isA() == TSegment3DClass)
		temp = new CSphere3D(ptA,(CSegment3D*)ptB);
	else 
		temp = new CSphere3D(ptA,ptB);

	temp->CalculConceptuel();
	PrepareAddedObject(temp);

	if (ptA) ptA->SetSelected(FALSE);
	if (ptB) ptB->SetSelected(FALSE);
	InvalidateParent();
	//m_pParent->Invalidate();
	ptA=ptB=NULL;
	m_nStep = 0;
}

DWORD CSphere3DTask::GetMask()
{
	return (m_nStep) ? TAllPointClass|TAllSegmentClass : TAllPointClass;
}

unsigned CSphere3DTask::GetHelpResID()
{
	return ((m_nStep) ? CTX_SELECT_POINTSEG : CTX_SELECT_POINT1);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CLocus3DTask::CLocus3DTask(CView *AParent,UINT nID) : CDroite3DTask(AParent,nID)
{
}

DWORD CLocus3DTask::GetMask()
{
	if (!ptA) 
		return TPointSurD3DClass | TPointSurC3DClass | TPointSurP3DClass | 
				TPointSurS3DClass;
	else 
		return TAllPointClass;
}

unsigned CLocus3DTask::GetHelpResID()
{
	if (!ptA) 
		return CTX_LOCUS_BASE;
	else
		return CTX_LOCUS_LOCUS;
}


void CLocus3DTask::DrawFeedBack(CDC* pDC)
{
	if (!ptA) return;

	CPoint thePt = ptA->Visual_pt;
	CPoint endPt(m_ptOld.x,m_ptOld.y);
	CSize dl = endPt - thePt;
	dl.cx =dl.cx/2;
	dl.cy =dl.cy/2;
	endPt+=dl;
	thePt+=dl;

	pDC->MoveTo(thePt);
	pDC->LineTo(endPt);
}

void CLocus3DTask::CreateObject3D()
{
	CLocus3D* temp = new CLocus3D(ptA,ptB);

	UINT res = temp->CalculConceptuel();
	if (res)
	{
		temp->HandleObjectError(res,TRUE);
		delete temp;
	}
	else
		PrepareAddedObject(temp);

	if (ptA) ptA->SetSelected(FALSE);
	if (ptB) ptB->SetSelected(FALSE);
	m_pParent->Invalidate();
	ptA=ptB=NULL;
	m_nStep = 0;
}

