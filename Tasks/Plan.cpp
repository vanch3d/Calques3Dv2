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
// Plan.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Plan3D.h"
#include "..\Objects\Cercle3D.h"
#include "..\Objects\Cylinder3D.h"
#include "..\Objects\Cone3D.h"

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

CPlan3DTask::CPlan3DTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
	ptA = ptB = ptC = NULL;
	GetContextualHelp();
}

CPlan3DTask::~CPlan3DTask()
{
	if (ptA)	ptA->SetSelected(FALSE);
	if (ptB)	ptB->SetSelected(FALSE);
	if (ptC)	ptC->SetSelected(FALSE);
	if (ptA || ptB || ptC)	
		if (m_pParent && m_pParent->GetSafeHwnd()) m_pParent->Invalidate(0);
}

unsigned CPlan3DTask::GetHelpResID()
{
	if (m_nStep==0)
		return CTX_SELECT_POINT1;
	else if (m_nStep==1)
		return CTX_SELECT_POINT2;
	else 
		return CTX_SELECT_POINT3;
}


CObjectId CPlan3DTask::GetMask()
{
	return TAllPointClass;
}

void CPlan3DTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp;

	int index = FindObject(thepos,GetMask(),FALSE);
	if (!index)
	{
		if (!ptA)
			GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,NULL);
		return;
	}

	temp = GetObject();
	if (!temp) return;

	BOOL bPTnew = TRUE;
	temp->SetSelected(TRUE);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
	m_pParent->Invalidate(0);

	if (!ptA)
	{
		m_ptOld = thepos;
	}
	if ((!ptA) && (!ptB) && (!ptC))
	 {
		ptA = (CPoint3D*) temp;
		bPTnew = FALSE;
		m_nStep++;
	 }
	if ((bPTnew) && (ptA) && (!ptB) && (!ptC))
		if (temp != ptA)
		 {	
			ptB = (CPoint3D*) temp;
			bPTnew = FALSE;
			m_nStep++;
		 }
		else
		 {
			ShowErrorMsg(ERR_2SAMEPTS);
			return;
		  }
	if ((bPTnew) && (ptA) && (ptB) && (!ptC))
		if ((temp != ptA) && (temp != ptB))
		{
			ptC = (CPoint3D*) temp;
			CreateObject3D();
		}
		else
		 {
			ShowErrorMsg(ERR_2SAMEPTS);
			return;
		  }
	GetContextualHelp();
}

void CPlan3DTask::OnMouseR(UINT, CPoint)
{
}

void CPlan3DTask::OnMouseMove(UINT, CPoint thepoint)
{
	FindObject(thepoint,GetMask(),TRUE);
	m_ptOld = thepoint;
	if (ptA)
		m_pParent->Invalidate(FALSE);
}

void CPlan3DTask::CreateObject3D()
{
	CPlan3D* temp = new CPlan3D(ptA,ptB,ptC);
	UINT a = temp->CalculConceptuel();
	if (a)
	{
		temp->HandleObjectError(a,TRUE);
		delete temp;
	}
	else
	{
		BOOL test = PrepareAddedObject(temp);
		if (test && temp)
			temp->CalculVisuel(m_pParent->GetVisualParam());
	}
	ptA->SetSelected(FALSE);
	ptB->SetSelected(FALSE);
	ptC->SetSelected(FALSE);
	m_pParent->Invalidate();
	m_nStep = 0;
	ptA=ptB=ptC=NULL;
	GetContextualHelp();
}

void CPlan3DTask::DrawFeedBack(CDC *)
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCercle3DTask::CCercle3DTask(CView *AParent,UINT taskID) : CPlan3DTask(AParent,taskID)
{
}

void CCercle3DTask::CreateObject3D()
{
	CObject3D* temp=NULL;
	if (m_nTaskID == ID_OBJECT_ARCCIRCLE)
		temp = new CArcCercle3D(ptA,ptB,ptC);
	else
		temp = new CCercle3D(ptA,ptB,ptC);
	if (!temp) return;
	UINT a = temp->CalculConceptuel();
	if (a)
	{
		temp->HandleObjectError(a,TRUE);
		delete temp;
	}
	else
	{
		BOOL test = PrepareAddedObject(temp);
		if (test && temp)
			temp->CalculVisuel(m_pParent->GetVisualParam());
	}
	ptA->SetSelected(FALSE);
	ptB->SetSelected(FALSE);
	ptC->SetSelected(FALSE);
	m_pParent->Invalidate();
	m_nStep = 0;
	ptA=ptB=ptC=NULL;
	GetContextualHelp();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCylindreTask::CCylindreTask(CView *AParent,UINT taskID): CPlan3DTask(AParent,taskID)
{
}

void CCylindreTask::CreateObject3D()
{
	CCylinder3D* temp= new CCylinder3D(ptA,ptB,ptC);
	if (!temp) return;
	UINT a = temp->CalculConceptuel();
	if (a)
	{
		temp->HandleObjectError(a,TRUE);
		delete temp;
	}
	else
	{
		BOOL test = PrepareAddedObject(temp);
		if (test && temp)
			temp->CalculVisuel(m_pParent->GetVisualParam());
	}
	ptA->SetSelected(FALSE);
	ptB->SetSelected(FALSE);
	ptC->SetSelected(FALSE);
	m_pParent->Invalidate();
	m_nStep = 0;
	ptA=ptB=ptC=NULL;
	GetContextualHelp();
}

void CCylindreTask::DrawFeedBack(CDC *pDC)
{
	CPlan3DTask::DrawFeedBack(pDC);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConeTask::CConeTask(CView *AParent,UINT taskID): CPlan3DTask(AParent,taskID)
{
}

void CConeTask::CreateObject3D()
{
	CCone3D* temp= new CCone3D(ptA,ptB,ptC);
	if (!temp) return;
	UINT a = temp->CalculConceptuel();
	if (a)
	{
		temp->HandleObjectError(a,TRUE);
		delete temp;
	}
	else
	{
		BOOL test = PrepareAddedObject(temp);
		if (test && temp)
			temp->CalculVisuel(m_pParent->GetVisualParam());
	}
	ptA->SetSelected(FALSE);
	ptB->SetSelected(FALSE);
	ptC->SetSelected(FALSE);
	m_pParent->Invalidate();
	m_nStep = 0;
	ptA=ptB=ptC=NULL;
	GetContextualHelp();
}

void CConeTask::DrawFeedBack(CDC *pDC)
{
	if (ptA && m_nStep==1)
	{	
		// Draw feedback for the axis
		CPoint thePt = ptA->Visual_pt;
		CPoint endPt(m_ptOld.x,m_ptOld.y);
		CSize dl = endPt - thePt;
		dl.cx =dl.cx/2;
		dl.cy =dl.cy/2;
		endPt+=dl;

		pDC->MoveTo(thePt);
		pDC->LineTo(endPt);
	}
	if (ptA && ptB && m_nStep==2)
	{
		CVector4 pta;
		CVector4 ptb;
		{
			// Draw feedback for the axis
			CPoint thePt = ptA->Visual_pt;
			CPoint endPt = ptB->Visual_pt;
			pta = CVector4(thePt.x,thePt.y,0);
			ptb = CVector4(endPt.x,endPt.y,0);
			CSize dl = endPt - thePt;
			dl.cx =dl.cx/4;
			dl.cy =dl.cy/4;
			endPt+=dl;
			pDC->MoveTo(thePt);
			pDC->LineTo(endPt);
		}
		{
			// Draw feedback for first ray
			CPoint thePt = ptA->Visual_pt;
			CPoint endPt(m_ptOld.x,m_ptOld.y);
			CSize dl = endPt - thePt;
			dl.cx =dl.cx/4;
			dl.cy =dl.cy/4;
			endPt+=dl;
			CVector4 ptc(endPt.x,endPt.y,0);
			pDC->MoveTo(thePt);
			pDC->LineTo(endPt);

			// Draw feedback for first ray
			CVector4 norm = ptb-pta;
			norm = norm.Normalized();
			FCoord proj = norm * (ptc - pta);
			norm = pta + norm*proj;
			CVector4 sim = norm*2. - ptc;
			pDC->MoveTo(pta);
			pDC->LineTo(sim);
		}
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTranslatTask::CTranslatTask(CView *AParent,UINT taskID) : CPlan3DTask(AParent,taskID)
{
}
	
void CTranslatTask::CreateObject3D()
{
	CPointTranslat3D* temp= new CPointTranslat3D(ptA,ptB,ptC);
	if (!temp) return;
	UINT a = temp->CalculConceptuel();
	if (a)
	{
		temp->HandleObjectError(a,TRUE);
		delete temp;
	}
	else
	{
		BOOL test = PrepareAddedObject(temp);
		if (test && temp)
			temp->CalculVisuel(m_pParent->GetVisualParam());
	}
	ptA->SetSelected(FALSE);
	ptB->SetSelected(FALSE);
	ptC->SetSelected(FALSE);
	m_pParent->Invalidate();
	m_nStep = 0;
	ptA=ptB=ptC=NULL;
	GetContextualHelp();
}

void CTranslatTask::DrawFeedBack(CDC* pDC)
{
	if (!ptA) return;
	CPen thePen(PS_DOT,1,RGB(0,0,0));
	
	if (ptA && !ptB && !ptC)
	{
		CPoint thePt = ptA->Visual_pt;
		CPoint endPt = m_ptOld;
		CPen *oldP = pDC->SelectObject(&thePen);
		pDC->MoveTo(thePt);
		pDC->LineTo(endPt);
		pDC->SelectObject(oldP);
	 }
	if (ptA && ptB && !ptC)
	 {
		CPoint thePt = m_ptOld;
		CSize dl = ptB->Visual_pt - ptA->Visual_pt;
		CPoint endPt = thePt;
		endPt += dl;
		CPen *oldP = pDC->SelectObject(&thePen);
		pDC->MoveTo(thePt);
		pDC->LineTo(endPt);
		pDC->SelectObject(oldP);
		pDC->Ellipse(endPt.x-3,endPt.y-3,endPt.x+4,endPt.y+4);
	 }
}
