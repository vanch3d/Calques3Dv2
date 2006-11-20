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

DWORD CMilieu3DTask::GetMask()
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

	pDC->Ellipse(thePt.x-3,thePt.y-3,thePt.x+3,thePt.y+3);

}
