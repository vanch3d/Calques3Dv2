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
// Greffe.cpp: implementation of the CGreffe3DTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\Calques3DDoc.h"
#include "..\ViewUniv.h"

#include "..\prefs\prefs.h"

#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Objects\Plan3D.h"

#include "..\objects\Macro3D.h"
#include "..\MacroInfoDlg.h"

#include "Task.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGreffe3DTask::CGreffe3DTask(CView *AParent,UINT nID) : CTask(AParent,nID)
{
	pMacro = NULL;
	bLoaded = FALSE;
	bShifted = FALSE;
	OnLoadMacro(nID);
}

CGreffe3DTask::~CGreffe3DTask()
{
	if (pMacro)	delete pMacro;
}


void CGreffe3DTask::OnLoadMacro(UINT nID)
{
	CString strPath;
	if (nID >= ID_MACROS_USER1 && nID <= ID_MACROS_USER15)
	{
		int nbMacros = TPref::TMacroList.GetCount();
		int nItem = nID-ID_MACROS_USER1+1;
		if (nItem <=0 || nItem > nbMacros) return;

		int i=0;
		CMacroUserTool* pTool = NULL;
		for(POSITION pos = TPref::TMacroList.GetHeadPosition(); pos != NULL; )
		{
			CMacroUserTool *pT = (CMacroUserTool*)TPref::TMacroList.GetNext( pos );
			i++;
			if (pT && (i==nItem))
			{
				pTool = pT;
				break;
			}
		}
		if (!pTool) return;
		if (pTool->m_strFile.IsEmpty()) return;

		strPath = pTool->m_strFile;
	}
	else if (nID==ID_MACROS_LOAD)
	{
		static char BASED_CODE szFilter[] = "Macro-construction (*.m3d)||";
 
		CFileDialog mdlg(TRUE,"*.m3d","*.m3d",
			OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
			szFilter,
			m_pParent);

	/*	CString strCaption;
		strCaption.LoadString (AFX_IDS_APP_TITLE);

		CBCGFileDialog dlg (strCaption, FALSE,"*.m3d","*.m3d",
				OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
				szFilter,m_pParent);

		CImageList newItemImages;
		newItemImages.Create (32, 32, ILC_MASK, 0, 1);
		newItemImages.Add (AfxGetApp ()->LoadIcon (IDR_VIEWUNIVERSE));
		//newItemImages.Add (LoadIcon (IDI_ICON_TEXT));
		//newItemImages.Add (LoadIcon (IDI_ICON_WRITE));

		dlg.SetNewImagesList (&newItemImages);

		//CBitmap bmpLogo;
		//bmpLogo.LoadBitmap (IDB_SPLASH);
		//dlg.SetLogoBitmap (&bmpLogo);

		CString strItem;

		strItem.LoadString (IDR_VIEWUNIVERSE);
		dlg.AddNewItem (strItem.SpanExcluding (_T("\n")), 0);

		//strItem.LoadString (IDS_TEXT_DOC);
		//dlg.AddNewItem (strItem.SpanExcluding (_T("\n")), 1);

		//strItem.LoadString (IDS_WRITE_DOC);
		//dlg.AddNewItem (strItem.SpanExcluding (_T("\n")), 2);

		HICON hiconOpenDlg = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);
		dlg.SetDlgIcon (hiconOpenDlg);

		BOOL bOk = (dlg.DoModal () == IDOK);

		::DestroyIcon (hiconOpenDlg);
		return; */

		int ret = mdlg.DoModal();
		if (ret==IDOK)
			strPath = mdlg.m_ofn.lpstrFile;
		else return;
	}
	else return;

	CCalques3DMacroDoc mdoc;
	mdoc.OnOpenDocument(strPath);
	int nb = mdoc.m_cObjectSet.GetSize();
	
	pMacro = new CMacro3D();

	pMacro->strObjectDef = mdoc.strObjectDef;
	pMacro->strObjectHelp = mdoc.strObjectHelp;
	//pMacro->strObjectDef = mdoc.strObjectDef;
	for (int i=0;i<nb;i++)
	{
		CObject3D *pObj = mdoc.m_cObjectSet.GetAt(i);
		pObj->bDrawInGraph = FALSE;
		pObj->SetHistoryVisibility();
		pMacro->m_cSubObjects.Add(pObj);
	}
	mdoc.m_cObjectSet.RemoveAll();
	//int nb1 = mdoc.m_cObjectSet.GetSize();
	int nb2 = pMacro->m_cSubObjects.GetSize();
	//for (i=0;i<nb2;i++)
	//{
	//	CObject3D *pObj = pMacro->m_cSubObjects.GetAt(i);
	//}
	
	//GetDocument()->m_cObjectSet.Add(pObj);
	//BOOL bRes = GetDocument()->AddObject(pMacro);
	//if (bRes)
	//{
	//	GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_ADD,pMacro);
	//	Invalidate();
	//	UpdateWindow();
	//}
	//delete pMacro;
	m_cBasePoints.RemoveAll();
	for (i=0;i<nb2;i++)
	{
		CObject3D *pObj = pMacro->m_cSubObjects.GetAt(i);
		if (!pObj) continue;
		if (pObj->isA() == TPoint3DClass)
			m_cBasePoints.Add(pObj);
	}
	m_nStep = 0;
}

DWORD CGreffe3DTask::GetMask()
{
	return TAllPointClass;
}

unsigned CGreffe3DTask::GetHelpResID()
{
	if (!bLoaded)
		return CTX_MACRO_UNLOAD;
	else
		return CTX_MACRO_POINT;
}



void CGreffe3DTask::OnMouseLDC(UINT, CPoint thepos)
{

	CreateObject3D();
}


void CGreffe3DTask::OnMouseL(UINT, CPoint thepos)
{
	CObject3D *temp=0;

	int index = FindObject(thepos,GetMask(),FALSE);
//	if (!index) return;
	temp = GetObject();

	if (!bLoaded)
	{
		if (!pMacro)
			OnLoadMacro(m_nTaskID);
		if (!pMacro) return;
		bLoaded = TRUE;
	}

	if (temp)
	{
		int nbB = m_cBasePoints.GetSize();
		if (nbB)
		{
			CObject3D *pObj = m_cBasePoints.GetAt(0);
			if (pObj)
			{
				if (pObj->GraftOn(temp))
				{
					//pMacro->cParentList.Add(temp);
					pMacro->AddParent(temp);
					m_cBasePoints.RemoveAt(0);
					pMacro->RemoveSubObject(0);
				}
			}
			CVisualParam *mp = m_pParent->GetVisualParam();

			ShiftTo(thepos);
			pMacro->CalculVisuel(mp);
			m_ptOld = thepos;
			m_nStep++;
			GetContextualHelp();
		}
		nbB = m_cBasePoints.GetSize();
		if (!nbB)
		{
			CreateObject3D();
			return;
		}
	}
}

void CGreffe3DTask::OnMouseR(UINT, CPoint)
{
}

void CGreffe3DTask::ShiftTo(CPoint thepoint)
{
	int nb= m_cBasePoints.GetSize();
	if (nb && pMacro)
	{
		CVisualParam *mp = m_pParent->GetVisualParam();

		CPoint3D a(CVector4(0,0,0,0));
		CPoint3D b(CVector4(100,0,0,0));
		CPoint3D c(CVector4(0,100,0,0));
		CVector4 newPt1(0,0,0,0);

		CPlan3D pl(&a,&b,&c);
		pl.CalculConceptuel();
		CPointSurP3D pton(&pl);
		pton.MoveObject(mp,0,thepoint,newPt1);
		pton.CalculConceptuel();
		newPt1 = pton.Concept_pt;

		CPoint3D *pt = (CPoint3D*)m_cBasePoints.GetAt(0);
		if (pt)
		{
			CVector4 mv = newPt1 - pt->Concept_pt;
			for (int i=0;i<nb;i++)
			{
				pt = (CPoint3D*)m_cBasePoints.GetAt(i);
				if (!pt) continue;
				pt->Concept_pt = pt->Concept_pt + mv;
			}
		}
		pMacro->CalculConceptuel();
		m_pParent->Invalidate(FALSE);
	}
}

void CGreffe3DTask::OnMouseMove(UINT, CPoint thepoint)
{
	FindObject(thepoint,GetMask(),TRUE);
	m_ptOld = thepoint;
	ShiftTo(thepoint);
}

void CGreffe3DTask::DrawFeedBack(CDC *pDC)
{
	if (!bLoaded || !pMacro) return;
	CVisualParam *mp = m_pParent->GetVisualParam();
	pMacro->CalculVisuel(mp);
	pMacro->Draw(pDC,mp);
}

void CGreffe3DTask::CreateObject3D()
{
	//CDroite3D* temp = new CDroite3D(ptA,ptB);
	if (!pMacro) return;

//	pMacro->SetDepth();
	pMacro->CalculConceptuel();
	PrepareAddedObject(pMacro);

	pMacro = NULL;
	//if (ptA) ptA->bIsSelected = FALSE;
	//if (ptB) ptB->bIsSelected = FALSE;
	InvalidateParent();
	//m_pParent->Invalidate();
	//ptA=ptB=NULL;
	m_nStep = 0;
	bLoaded = FALSE;
}

