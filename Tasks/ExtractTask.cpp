// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
//#include "..\Objects\Point3D.h"
//#include "..\Objects\Plan3D.h"
#include "..\Objects\Text3D.h"

#include "..\Calques3DDoc.h"
#include "..\ViewUniv.h"
#include "..\MainFrm.h"
#include "..\ViewCalque.h"

#include "Task.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CExtractCalcTask::CExtractCalcTask(CView *AParent,UINT taskID):
	CTask(AParent,taskID)
{
	bAddInCalc = TRUE;
	nCalcNum = -1;
	m_cObjExtracted.RemoveAll();
}

CExtractCalcTask::~CExtractCalcTask()
{
	for (int i=0;i<m_cObjExtracted.GetSize();i++)
		m_cObjExtracted.GetAt(i)->SetSelected(FALSE);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_EXT,NULL);
	if (m_pParent->GetSafeHwnd())
	{
		InvalidateParent(TRUE);
		//m_pParent->Invalidate();
		//m_pParent->UpdateWindow();
	}
	m_cObjExtracted.RemoveAll();
}

DWORD CExtractCalcTask::GetMask()
{
	if (nCalcNum==-1)
		return 0;
	else 
		return CTask::GetMask();
}

unsigned CExtractCalcTask::GetHelpResID()
{
	if (nCalcNum==-1)
		return CTX_SELECT_TASKOPTION;
	else 
		return CTX_SELECT_OBJECT;
}



void CExtractCalcTask::OnMouseL(UINT modkey, CPoint thepos)
{
}

void CExtractCalcTask::OnMouseMove(UINT modkey, CPoint thepos)
{
	BOOL bSubobj = (modkey & MK_SHIFT);
	FindObject(thepos,GetMask(),TRUE,bSubobj);

}

void CExtractCalcTask::OnMouseLUp(UINT modkey, CPoint thepos)
{
	//CTask::OnMouseLUp(modkey,thepos);
	BOOL bSubobj = (modkey & MK_SHIFT);
	m_pObject = NULL;
	int nb = FindObject(thepos,GetMask(),FALSE,bSubobj);
	//CObject3D *pObj = GetObject();
	//if (m_pParent->m_pSelObj || pObj)
	//if (nCalcNum==-1) return;
	//OnMouseR(modkey,thepos);
	//	GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_SEL,pObj);
	CreateObject3D();
}

void CExtractCalcTask::OnMouseR(UINT, CPoint point)
{
	int rr=0;

	theApp.ShowPopupMenu (IDR_POPUP_EXTRACT, point, m_pParent);
	//CBCGPopupMenu *pMen= new CBCGPopupMenu();
	//pMen->Create();
}

void CExtractCalcTask::CreateObject3D()
{
	int rr=0;
	CObject3D* pObj = GetObject();
	if (!pObj) return;

	if (bAddInCalc)
	{
		if (nCalcNum==-1) return;
		for (int i=0;i<m_cObjExtracted.GetSize();i++)
		{
			CObject3D *pO = m_cObjExtracted.GetAt(i);
			if (pO == pObj) return;
		}
		m_cObjExtracted.Add(pObj);
		pObj->AddInCalque(nCalcNum,bAddInCalc);
	}
	else
	{
		int nCal = m_pParent->GetVisualParam()->nCalqueNum;
		pObj->AddInCalque(nCal,bAddInCalc);
	}


/*	if (bAddInCalc)
	{
		pObj->bIsSelected = TRUE;
	}
	else
	{

	}*/
	pObj->SetSelected(bAddInCalc);
	if (bAddInCalc) GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,pObj);
	GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_EXT,pObj);
	InvalidateParent(TRUE);
	//m_pParent->Invalidate();
	//m_pParent->UpdateWindow();

}


BOOL CExtractCalcTask::OnUpdateTasksOption(CCmdUI* pCmdUI)
{
	BOOL bDone = TRUE;
	BOOL bCheck = FALSE;	//((myTask) && (myTask->m_nTaskID == pCmdUI->m_nID));
	BOOL bEnab = FALSE;
	int nCal = m_pParent->GetVisualParam()->nCalqueNum;
	switch (pCmdUI->m_nID){
	case ID_EXPLORATION_EXTRACTION_CALQUE1:
	case ID_EXPLORATION_EXTRACTION_CALQUE2:
	case ID_EXPLORATION_EXTRACTION_CALQUE3:
	case ID_EXPLORATION_EXTRACTION_CALQUE4:
		if (bAddInCalc)
		{
			bCheck = (nCalcNum == (int)((pCmdUI->m_nID) - ID_EXPLORATION_EXTRACTION_CALQUE1 +1));
			bEnab = (nCal != (int)((pCmdUI->m_nID) - ID_EXPLORATION_EXTRACTION_CALQUE1 +1));
		}
		else
		{
			bCheck = (nCal == (int)((pCmdUI->m_nID) - ID_EXPLORATION_EXTRACTION_CALQUE1 +1));
			bEnab = FALSE;
		}
		break;
	case ID_EXPLORATION_EXTRACTION_ADDALL:
		bEnab = (nCalcNum !=-1) && bAddInCalc;
		break;
	case ID_EXPLORATION_EXTRACTION_REMOVEALL:
		bEnab = (nCal && !bAddInCalc);
		break;
	case ID_EXPLORATION_EXTRACTION_REMOVEFROM:
		bEnab = nCal;
		bCheck = !bAddInCalc;
		break;
	case ID_EXPLORATION_EXTRACTION_ADDIN:
		bEnab = TRUE;
		bCheck = bAddInCalc;
		break;
	default:
		bDone = FALSE;
		break;
	}
	pCmdUI->Enable(bEnab);
	pCmdUI->SetCheck(bCheck);
	return bDone;
}



BOOL CExtractCalcTask::OnDoTasksOption(UINT nID)
{
	BOOL bDone = TRUE;
	int nCal = m_pParent->GetVisualParam()->nCalqueNum;

	switch (nID){
	case ID_EXPLORATION_EXTRACTION_CALQUE1:
	case ID_EXPLORATION_EXTRACTION_CALQUE2:
	case ID_EXPLORATION_EXTRACTION_CALQUE3:
	case ID_EXPLORATION_EXTRACTION_CALQUE4:
		{
			int oldCN = nCalcNum;
			nCalcNum = (nID - ID_EXPLORATION_EXTRACTION_CALQUE1+1);
			for (int i=0;i<m_cObjExtracted.GetSize();i++)
				m_cObjExtracted.GetAt(i)->SetSelected(FALSE);
			GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_EXT,NULL);
			if (m_pParent->GetSafeHwnd())
			{
				InvalidateParent(TRUE);
				//m_pParent->Invalidate();
				//m_pParent->UpdateWindow();
			}
			m_cObjExtracted.RemoveAll();
			DoLaunchView(ID_VIEW_CALQUE+nCalcNum-1);
		}
		break;
	case ID_EXPLORATION_EXTRACTION_ADDALL:
		{
			int nb = GetDocument()->m_cObjectSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = GetDocument()->m_cObjectSet.GetAt(i);
				if (!pObj) continue;
				if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
				if (pObj->IsInCalque(nCal) && pObj->bVisible && pObj->bValidate)
				{
					pObj->AddInCalque(nCalcNum,TRUE);
				}
			}
			GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_EXT,NULL);
			if (m_pParent->GetSafeHwnd())
			{
				InvalidateParent(TRUE);
			}
		}
		break;
	case ID_EXPLORATION_EXTRACTION_REMOVEALL:
		{
			int nb = GetDocument()->m_cObjectSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = GetDocument()->m_cObjectSet.GetAt(i);
				if (!pObj) continue;
				if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
				if (pObj->IsInCalque(nCal) && pObj->bVisible && pObj->bValidate)
				{
					pObj->AddInCalque(nCal,FALSE);
				}
			}
			GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_EXT,NULL);
			if (m_pParent->GetSafeHwnd())
			{
				InvalidateParent(TRUE);
				//m_pParent->Invalidate();
				//m_pParent->UpdateWindow();
			}
		}
		break;
	case ID_EXPLORATION_EXTRACTION_REMOVEFROM:
		if (bAddInCalc)
		{
			bAddInCalc = !bAddInCalc;
			for (int i=0;i<m_cObjExtracted.GetSize();i++)
				m_cObjExtracted.GetAt(i)->SetSelected(FALSE);
			GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_EXT,NULL);
			if (m_pParent->GetSafeHwnd())
			{
				InvalidateParent(TRUE);
				//m_pParent->Invalidate();
				//m_pParent->UpdateWindow();
			}
			m_cObjExtracted.RemoveAll();
		}
		break;
	case ID_EXPLORATION_EXTRACTION_ADDIN:
		if (!bAddInCalc)
		{
			bAddInCalc = !bAddInCalc;
			for (int i=0;i<m_cObjExtracted.GetSize();i++)
				m_cObjExtracted.GetAt(i)->SetSelected(FALSE);
			GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_EXT,NULL);
			if (m_pParent->GetSafeHwnd())
			{
				InvalidateParent(TRUE);
				//m_pParent->Invalidate();
				//m_pParent->UpdateWindow();
			}
			m_cObjExtracted.RemoveAll();
		}
		break;
	default:
		bDone = FALSE;
		break;
	}
	return bDone;
}
