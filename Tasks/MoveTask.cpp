// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
//#include "..\Objects\Plan3D.h"

#include "..\Calques3DDoc.h"
#include "..\ViewUniv.h"
//#include "..\MainFrm.h"
#include "..\Objects\Locus3D.h"

#include "Task.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMoveObjectTask::CMoveObjectTask(CView *AParent,UINT taskID):
	CTask(AParent,taskID)
{
	m_pObjParent = NULL;
	m_pObjMov = NULL;
	m_bDragPoint = FALSE;
	m_bTracePoint = FALSE;

	m_pSelObject = NULL;
	m_cRedrawObject.RemoveAll();
	m_cParentFound.RemoveAll();
	m_cTraceObject.RemoveAll();

//	CVisualParam *myVisuParam = m_pParent->GetVisualParam();
//	myVisuParam->bFeedPlane = TRUE;
}

CMoveObjectTask::~CMoveObjectTask()
{
//	CVisualParam *myVisuParam = m_pParent->GetVisualParam();
//	myVisuParam->bFeedPlane = FALSE;
	if (m_pSelObject)
	{
		m_pSelObject->SetSelected(FALSE);
		InvalidateParent();
		//if (m_pParent && m_pParent->GetSafeHwnd()) m_pParent->Invalidate();
	}

	for (int i=0;i<m_cTraceObject.GetSize();i++)
	{
		CObject3D *pObj = m_cTraceObject.GetAt(i);
		if (!pObj) continue;

		pObj->cTracePosition.RemoveAll();
		pObj->bShowTrace = FALSE;
	}
}

unsigned CMoveObjectTask::GetHelpResID()
{
	if (m_bTracePoint)
		return CTX_MOVE_TRACE;
	else
		return ((m_nStep) ? CTX_POINT_DRAG : CTX_MOVE_INIT);
}


unsigned long CMoveObjectTask::GetMask()
{
	unsigned long mask = (
		TPoint3DClass | TPointSurD3DClass | TPointSurC3DClass | 
		TPointSurP3DClass | TPointSurS3DClass | TPointSurCyl3DClass);

	if (m_bTracePoint)
		mask = TAllPointClass;
	else
		mask = TAllBasePointClass;


	return mask;
}


void CMoveObjectTask::OnMouseL(UINT, CPoint thePt)
{
	int nb = FindObject(thePt,GetMask(),TRUE,TRUE);

	int nbO = m_cObjectFound.GetSize();

	if (m_bTracePoint)
	{
		CObject3D *pObj = GetObject();
		if (pObj)
		{
			pObj->bShowTrace = TRUE;
			m_cTraceObject.Add(pObj);
		}

		m_bTracePoint = FALSE;
		return;
	}
	if (!nb)
	{	
		if (m_pSelObject)
		{	
			m_pSelObject->SetSelected(FALSE);
			m_pParent->Invalidate(FALSE);
			m_pParent->GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,NULL);
		}
		m_pSelObject = 0;
		m_nStep = 0;
		m_pObject = NULL;
		return;
	}

	BOOL bFound = FALSE;
	if (m_pSelObject)
	{
		for (int i=0;i<nbO;i++)
		{
			CObject3D *pp = m_cObjectFound[i];
			if (pp == m_pSelObject)
			{
				bFound = TRUE;
				break;
			}
		}
		if (!bFound)
		{
			m_pSelObject->SetSelected(FALSE);
			m_pParent->Invalidate(FALSE);
		}
	}
	if (!bFound)
		m_pSelObject = GetObject();

	m_pParent->GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,m_pSelObject);
	InvalidateParent(TRUE);
///	m_pParent->Invalidate();
	//m_pParent->UpdateWindow();

	if (!m_pSelObject) return;

	m_pSelObject->SetSelected(TRUE);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,m_pSelObject);
	m_pObjMov = (CPoint3D *)m_pSelObject;
	//int posPrt = ObjFound->Find(SelectedObject);
	//if (posPrt != INT_MAX )
	// {
	//	ObjParent = ((TObject3D*)(*ParentFound)[posPrt]);
	//	if (ObjParent == ObjMov)
	//		ObjParent = 0;
	//}
	m_ptOld = thePt;
	//((calque3dDoc&)((TWindowView*)Parent)->GetDocument()).ModifyObject(SelectedObject,1);

	// prepare Set of redrawed objectd
	m_cRedrawObject.RemoveAll();
	//if (ObjParent)
	//	RedrawObject3D = CreateDependList(ObjParent);
	//else
	//	RedrawObject3D = CreateDependList(SelectedObject);

	//CreateDependList(m_pSelObject,&m_cRedrawObject);
/*	CObject3D *pTop = m_pSelObject->pComposite;
	CObject3D *pStart = pTop;
	while (pStart)
	{
		pStart = pStart->pComposite;
		if (pStart)
			pTop = pStart;
	}
	if (pTop)
	{
		pTop->GetDependList(&m_cRedrawObject);
		pTop->PrepareMoveObject(TRUE);
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_DEP,pTop);
	}
	else
	{
		TRACE("\n--------------------------\n");
		m_pSelObject->GetDependList(&m_cRedrawObject);
		m_pSelObject->PrepareMoveObject(TRUE);
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_DEP,m_pSelObject);
	}*/

	if (m_pSelObject->pComposite)
	{
		TRACE("\n--------------------------\n");
		m_pSelObject->pComposite->GetDependList(&m_cRedrawObject);
		m_pSelObject->pComposite->PrepareMoveObject(TRUE);
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_DEP,m_pSelObject->pComposite);
	}
	else
	{
		TRACE("\n--------------------------\n");
		m_pSelObject->GetDependList(&m_cRedrawObject);
		m_pSelObject->PrepareMoveObject(TRUE);
		GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_DEP,m_pSelObject);
	}



	//for (int i=0;i<m_cRedrawObject.GetSize();i++)
	//{
	//	CObject3D *pp = m_cRedrawObject[i];
	//	CString mstr = pp->GetObjectHelp();
	//}
	m_pParent->GetDocument()->MoveObject(m_pSelObject);

	m_nStep++;
	GetContextualHelp();
	m_pParent->SendMessage(WM_SHOWOBJ_TOOLTIPS,(WPARAM)NULL,(LPARAM)FALSE);
	m_pParent->SetCapture();
}

void CMoveObjectTask::OnMouseLUp(UINT, CPoint)
{
	m_cRedrawObject.RemoveAll();
	if (m_pSelObject)
	{
		if (m_pSelObject->pComposite)
			m_pSelObject->pComposite->PrepareMoveObject(FALSE);
		else
			m_pSelObject->PrepareMoveObject(FALSE);
		if (m_pObjParent)
		 {
			//TCube3D* tmpcb= TYPESAFE_DOWNCAST(ObjParent, TCube3D);
			//if (tmpcb)
			 //tmpcb->cr->Visible = 0;
		 }
		ReleaseCapture();
		GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_MOV,m_pSelObject);
		m_pParent->Invalidate(0);
		m_pParent->UpdateWindow();
		m_nStep = 0;
		GetContextualHelp();
	}
}

void CMoveObjectTask::OnMouseMove(UINT modkey, CPoint thepos)
{
	static BOOL tert= FALSE;

	int nb = m_cRedrawObject.GetSize();

	if (m_pSelObject && (modkey & MK_LBUTTON) && nb)
	{
		//m_bDragPoint = TRUE;
		//Geom3DApp->SetToolTip(0);


		CVector4 TempCpt = ((CPoint3D* )m_pSelObject)->Concept_pt;
		tert = TRUE;
		//CPoint theP = thepos;
		//theP.x+=(int) ((Parent->Scroller) ? Parent->Scroller->XPos : 0);
		//theP.y+=(int) ((Parent->Scroller) ? Parent->Scroller->YPos : 0);

		CPoint MouseClic = (thepos);

		CVisualParam *myVisuParam = m_pParent->GetVisualParam();

		BOOL bOK = ((CPoint3D*)m_pSelObject)->MoveObject(myVisuParam,modkey,MouseClic,TempCpt);
		if (m_pSelObject->bShowTrace)
			m_pSelObject->cTracePosition.Add(((CPoint3D* )m_pSelObject)->Concept_pt);

		if (bOK)
		 {
			// only redraw objects
			//RedrawAllObjStruct GDLS ={1,myVisuParam};
			if (m_pSelObject->MaskObject(TAllPointClass))
				((CPoint3D* )m_pSelObject)->Concept_pt = TempCpt;

	
			for (int i=0;i<nb;i++)
			{
				CObject3D *myObj = m_cRedrawObject.GetAt(i);
				if (!myObj) continue;
			
				if (myObj->MaskObject(TLocus3DClass))
				{
					if (((CLocus3D*)myObj)->m_pSource == m_pSelObject) continue;
				}
				myObj->CalculConceptuel();
				myObj->CalculVisuel(myVisuParam);
				if (myObj->MaskObject(TAllPointClass) && myObj->bShowTrace)
					myObj->cTracePosition.Add(((CPoint3D* )myObj)->Concept_pt);
			}
	/*		if (nb)
			 { //RedrawObject3D->ForEach(RedrawAllObjects,&GDLS);
				TRedrawIter myiter(*RedrawObject3D);
				myiter.Restart();
				while ((int)myiter)
				 { TObject3D *o = myiter.Current();
					o->CalculConceptuel();
					myiter++;
				 }
				myiter.Restart();
				while ((int)myiter)
				 { TObject3D *o = myiter.Current();
					o->CalculVisuel(myVisuParam);
					myiter++;
				 }
			 }*/

			m_ptOld = thepos;
			if (TPref::TUniv.bSynchron)
				GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_MOV,m_pSelObject);
			InvalidateParent(TRUE);
			//m_pParent->Invalidate();
			//m_pParent->UpdateWindow();
//				((calque3dDoc&)((TWindowView*)Parent)->GetDocument()).NotifyViews(
//						vnObjDeform,0,Parent);
		 }
		else
		 {
			int ggg= 0;
			ggg ++;
		 }
	}
	else
	{
		FindObject(thepos,GetMask(),TRUE,TRUE);
	}
}

void CMoveObjectTask::OnMouseR(UINT, CPoint point)
{
	theApp.ShowPopupMenu (IDR_POPUP_MOVE, point, m_pParent);
}

void CMoveObjectTask::CreateObject3D()
{
}

UINT CMoveObjectTask::GetTaskCursor()
{
	UINT curID = -1;

	if (m_bTracePoint)
	{
		int rep = m_cObjectFound.GetSize();
		if (rep) curID = IDC_DROP_MARK;
	}
	else if (m_nStep)
	{	
		BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
		if (bShift)
			curID = IDC_MOVE_VERT;
		else
			curID = IDC_MOVE_HOR;
	 }    
	else
	{
		int rep = m_cObjectFound.GetSize();
		if (rep) curID = IDC_MOVE;
	}
	return curID;
}

int	 CMoveObjectTask::FindObject(CPoint theLoc,unsigned long mask,BOOL bShowLabel,BOOL bSub)
{
	return CTask::FindObject(theLoc,mask,bShowLabel,bSub);
}

BOOL CMoveObjectTask::OnUpdateTasksOption(CCmdUI* pCmdUI)
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
	case ID_EXPLORATION_MOVE_TRACE:
		bEnab = TRUE;//!m_bTracePoint;
		bCheck = m_bTracePoint;
		break;
	default:
		bDone = FALSE;
		break;
	}
	pCmdUI->Enable(bEnab);
	pCmdUI->SetCheck(bCheck);
	return bDone;
}



BOOL CMoveObjectTask::OnDoTasksOption(UINT nID)
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
			//m_pParent->Invalidate();
			//m_pParent->UpdateWindow();
		}
		break;
	case ID_EXPLORATION_MOVE_TRACE:
		m_bTracePoint = TRUE;
		break;
	default:
		bDone = FALSE;
		break;
	}
	return bDone;
}
