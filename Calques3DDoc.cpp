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
// Calques3DDoc.cpp: implementation of the CCalques3DDoc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Calques3D.h"

#include <io.h>
#include <stdio.h>
#include <process.h>

#include "Prefs\Prefs.h"
#include "prefs\UndoBar.h"

#include "Calques3DDoc.h"


#include "objects\Point3D.h"
#include "objects\Droite3D.h"
#include "objects\Sphere3D.h"
#include "objects\Plan3D.h"
#include "objects\Cylinder3D.h"
#include "objects\CompositeObj3D.h"
#include "objects\Text3D.h"

#include "MainFrm.h"
#include "FormatToolBar.h"
#include "WarningDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CxObject3DSet*	GLOBALObjectSet;
CxObject3DSet* CCalques3DDoc::GLOBALObjectSet = NULL;

//***************************************************************************
// CWinPlacement
//***************************************************************************
IMPLEMENT_SERIAL(CWinPlacement, CObject, VERSIONABLE_SCHEMA | 1)

CWinPlacement::CWinPlacement()
{
	m_pl.flags = 0;
	m_pl.length = 0;
	m_pl.ptMaxPosition = CPoint(0,0);
	m_pl.ptMinPosition = CPoint(0,0);
	m_pl.rcNormalPosition = CRect();
	m_pl.showCmd = 0;
	m_nIDWin = 0;
} 

CWinPlacement::CWinPlacement(const CWinPlacement& src)
{
	m_pl = src.m_pl;
	m_nIDWin = src.m_nIDWin;
} 

CWinPlacement::CWinPlacement(UINT str,WINDOWPLACEMENT tt)
{
	m_pl = tt;
	m_nIDWin = str;
} 

void CWinPlacement::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nIDWin;
		ar << m_pl.flags;
		ar << m_pl.ptMaxPosition;
		ar << m_pl.ptMinPosition;
		ar << m_pl.rcNormalPosition;
		ar << m_pl.showCmd;
	}
	else
	{
		ar >> m_nIDWin;
		ar >> m_pl.flags;
		ar >> m_pl.ptMaxPosition;
		ar >> m_pl.ptMinPosition;
		ar >> m_pl.rcNormalPosition;
		ar >> m_pl.showCmd;
		m_pl.length = 0;
	}

}

void CWinPlacement::operator =(const CWinPlacement& other) 
{
	m_pl = other.m_pl;
	m_nIDWin = other.m_nIDWin;
}


//***************************************************************************
// CUndoObject
//***************************************************************************
IMPLEMENT_SERIAL(CUndoObject, CObject, VERSIONABLE_SCHEMA | 1)

CUndoObject::CUndoObject()
{
	m_bCanDelete = FALSE;
	m_nUndoState = CCalques3DDoc::UndoNone;
} 

CUndoObject::~CUndoObject()
{
	if (m_bCanDelete)
		for (int i=0;i<m_cUndoSet.GetSize();i++)
		{
			CObject3D* pObj = m_cUndoSet.GetAt(i);
			if (pObj) delete pObj;
		}
	m_cUndoSet.RemoveAll();
} 

void CUndoObject::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar >> m_bCanDelete;
	}
	else
	{
		ar >> m_bCanDelete;
	}
}

CString CUndoObject::GetUndoText()
{
	CString m_strUndoObj;
	CString strDesc,strName;

	UINT nCmdID = m_nUndoState + IDS_UNDO_NONE;

	strDesc.LoadString(nCmdID);
	strName.Empty();

	if (m_cUndoSet.GetSize())
	{
		CObject3D* pCObj = m_cUndoSet.GetAt(0);
		if (pCObj) strName = pCObj->GetObjectName();
	}
	if (!strName.IsEmpty())
	{
		strName = _T(" (") + strName + _T(")");
	}
	m_strUndoObj = strDesc + strName;

	return m_strUndoObj;
}


//***************************************************************************
// CCalques3DDoc
//***************************************************************************
//IMPLEMENT_DYNCREATE(CCalques3DDoc, CDocument)
IMPLEMENT_SERIAL(CCalques3DDoc, CDocument, VERSIONABLE_SCHEMA | 1)

BEGIN_MESSAGE_MAP(CCalques3DDoc, CDocument)
	//{{AFX_MSG_MAP(CCalques3DDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_EXPLORATION_DISCOVERY, OnDiscovery)
	//ON_COMMAND(ID_VIEW_PLACEMENT_SAVE, OnViewPlacementSave)
	//ON_COMMAND(ID_VIEW_PLACEMENT_RESTORE, OnViewPlacementRestore)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_PLACEMENT_RESTORE, OnUpdatePlacementRestore)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc construction/destruction

CCalques3DDoc::CCalques3DDoc()
{
	// TODO: add one-time construction code here
	m_nDocUndoState = UndoNone;
	m_bMathPadUsed = false;
	m_bLegacyVersion = FALSE;

	m_cObjectSet.RemoveAll();
	m_cPolygonSet.RemoveAll();
	m_cPOVlist.RemoveAll();

	//m_cUndoSet.RemoveAll();
	m_cNewUndoSet.RemoveAll();
	//m_pMemObj = NULL;
	CCalques3DDoc::GLOBALObjectSet = NULL;
}

CCalques3DDoc::~CCalques3DDoc()
{
	m_cPolygonSet.RemoveAll();

	int nbObj = m_cObjectSet.GetSize();
	for (int i=0;i<nbObj;i++)
	{
		CObject3D *pObj = m_cObjectSet.GetAt(i);
		delete pObj;
	}
	m_cObjectSet.RemoveAll();

	for (POSITION pos = m_cPOVlist.GetHeadPosition (); pos != NULL;)
	{
		CPOVUserTool *pObj = m_cPOVlist.GetNext (pos);
		delete pObj;
	}
	m_cPOVlist.RemoveAll();
	CleanUndo();
}

/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc file management

//////////////////////////////////////////////////////////////////////
/// Called by the framework as part of the File New command.
//////////////////////////////////////////////////////////////////////
BOOL CCalques3DDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	int idx = 1;
	CPOVUserTool *tool = new CPOVUserTool();
	tool->m_bCanRemove = FALSE;
	tool->m_strLabel = _T("Default");
	tool->m_projParam = TPref::TUniv.sDefParam;
	m_cPOVlist.AddHead(tool);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework as part of the File Open command.
///
/// The current implementation checks for the old serialization method (prior to version 2.4.0) 
/// in case the current method (with version check) is failing.
///
/// @param lpszPathName	Points to the path of the document to be opened.
///
/// @todo In case of a legacy document, the warning message needs to be modified in order to 
///		  take macro-constructions into account
//////////////////////////////////////////////////////////////////////
BOOL CCalques3DDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName, CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;

	try
	{
		// Try the current serialization method
		CWaitCursor wait;
		if (pFile->GetLength() != 0) 
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	catch (CException* e)
	{
		// If it failed, close the file and reopen it using the SerializeLegacy method
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
		pFile = GetFile(lpszPathName, CFile::modeRead|CFile::shareDenyWrite, &fe);
		CArchive loadArchive2(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
		loadArchive2.m_pDocument = this;
		loadArchive2.m_bForceFlat = FALSE;
		try
		{
			// Try the legacy serialization method
			CWaitCursor wait;
			SerializeLegacy(loadArchive2);     // load me
			loadArchive2.Close();
			ReleaseFile(pFile, FALSE);
			//ReportSaveLoadException(lpszPathName, NULL, FALSE, AFX_IDP_C3D_OLDVERSION);
			m_bLegacyVersion = TRUE;
		}
		catch (CException* e2)
		{
			// If it failed, the document cannot be read by Calques 3D
			ReleaseFile(pFile, TRUE);
			DeleteContents();   // remove failed contents
			ReportSaveLoadException(lpszPathName, e, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
			e->Delete();
			e2->Delete();
			return FALSE;
		
		}
		e->Delete();
	}

	// start off with unmodified or modified if legacy serialization
	SetModifiedFlag(m_bLegacyVersion);     

	if (m_bMathPadUsed)
	{
		if (TPref::TMathPad.nShowView==1)
		{
			CMainFrame *pMFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
			if (pMFrame) pMFrame->LaunchView(this,ID_VIEW_ANALYTIC);
			m_bMathPadUsed = FALSE;
		}
	}
	return TRUE;
}

BOOL CCalques3DDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CCalques3DDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	return CDocument::CanCloseFrame(pFrame);
}


void CCalques3DDoc::OnFileSave()
{
	CDocument::OnFileSave();
}

void CCalques3DDoc::OnFileSaveAs()
{
	CDocument::OnFileSaveAs();
}


/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc serialization

//////////////////////////////////////////////////////////////////////
/// Called by the framework to read or write this document from or to an archive. 
/// This method is a legacy of older Calques 3D versions (prior to version 2.4.0 and
/// versionable serialization) and is maintained to allow access to old document.
/// 
/// @warning This method is assumed to be used only for loading document.
///
/// @param ar	A CArchive object to serialize to or from.
//////////////////////////////////////////////////////////////////////
void CCalques3DDoc::SerializeLegacy(CArchive& ar)
{
	if (ar.IsLoading())
	{
		CxObject3DSet	pObjectSet;
		CxObject3DSet	pGlobalObjectSet;
		CCalques3DDoc::GLOBALObjectSet = &pGlobalObjectSet;
		pObjectSet.Serialize(ar);
		int nb = pObjectSet.GetSize();
		for (int i=0;i<nb;i++)
		{
			CObject3D *pObj = pObjectSet.GetAt(i);
			if (!pObj) continue;
			if (DYNAMIC_DOWNCAST(CEquation3D,pObj))
			{
				m_bMathPadUsed = TRUE;
			}
			AddObject(pObj,FALSE);
		}
		CCalques3DDoc::GLOBALObjectSet = NULL;

		CPOVUserTool *tool = new CPOVUserTool();
		tool->m_bCanRemove = FALSE;
		tool->m_strLabel = _T("Default");
		tool->m_projParam = TPref::TUniv.sDefParam;
		m_cPOVlist.AddHead(tool);

	}
}

//////////////////////////////////////////////////////////////////////
/// Called by the framework to read or write this document from or to an archive. 
/// @param ar	A CArchive object to serialize to or from.
//////////////////////////////////////////////////////////////////////
void CCalques3DDoc::Serialize(CArchive& ar)
{
	ar.SerializeClass(RUNTIME_CLASS(CCalques3DDoc));
	UINT schema = ar.GetObjectSchema();

	if (ar.IsStoring())
	{
		// TODO: add storing code here
		m_cObjectSet.Serialize(ar);
		m_cPOVlist.Serialize(ar);
	}
	else
	{
		// TODO: add loading code here
		CxObject3DSet	pObjectSet;
		CxObject3DSet	pGlobalObjectSet;
		CCalques3DDoc::GLOBALObjectSet = &pGlobalObjectSet;
		pObjectSet.Serialize(ar);
		int nb = pObjectSet.GetSize();
		for (int i=0;i<nb;i++)
		{
			CObject3D *pObj = pObjectSet.GetAt(i);
			if (!pObj) continue;
			if (DYNAMIC_DOWNCAST(CEquation3D,pObj))
			{
				m_bMathPadUsed = TRUE;
			}
			AddObject(pObj,FALSE);
		}
		CCalques3DDoc::GLOBALObjectSet = NULL;
		m_cPOVlist.Serialize(ar);
	}

// 	try
// 	{	
// 		m_cWinPos.Serialize(ar);
// 	}
// 	catch (CException* pEx)
// 	{
// 		pEx->Delete ();
// 		m_strPathName.Empty();      // no path name yet
// 		SetModifiedFlag(TRUE);     // make clean
// 	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc diagnostics

#ifdef _DEBUG
void CCalques3DDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCalques3DDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc commands

int CCalques3DDoc::GetObjectSize(UINT nID)
{
	int		nb = 0,
			nb2=0;
	BOOL	bRet = FALSE;
	switch (nID){
	case IDS_NAME_OBJECT:
	case IDS_NAME_POINT:
	case IDS_NAME_SEG:
	case IDS_NAME_CIRCLE:
	case IDS_NAME_PLANE:
	case IDS_NAME_CUBE:
	case IDS_NAME_SPHERE:
	case IDS_NAME_CYLIN:
	case IDS_NAME_CONE:
		bRet= m_cObjectSize.Lookup(nID,nb);
		break;
	case IDS_NAME_LINE:
		bRet= m_cObjectSize.Lookup(IDS_NAME_LINE,nb2);
		bRet= m_cObjectSize.Lookup(IDS_NAME_SEG,nb);
		nb+=nb2;
		break;
	default:
		nb = 0;//m_cObjectSet.GetSize();
		break;
	}
	return nb;
}

void CCalques3DDoc::RenumberObjects()
{
	int nIdx = 1;
	int nbObj = m_cObjectSet.GetSize();
	for (int i=0;i<nbObj;i++)
	{
		CObject3D *pObj = m_cObjectSet.GetAt(i);
		if (!pObj) continue;
		nIdx = pObj->SetObjectID(nIdx);
		nIdx++;
	}
}


void CCalques3DDoc::OnCountObjects(CObject3D *pObj,BOOL bAdd)
{
	if (!pObj) return;

	UINT nID = pObj->GetNameID();
	int nSize = 0;
	int nTotSize = 0;
	BOOL bFound = m_cObjectSize.Lookup(nID,nSize);
	bFound = m_cObjectSize.Lookup(IDS_NAME_OBJECT,nTotSize);

	if (bAdd)
	{
		nTotSize++;
		nSize++;
	}
	else
	{
		nTotSize--;
		nSize--;
	}
	m_cObjectSize.SetAt(nID,nSize);
	m_cObjectSize.SetAt(IDS_NAME_OBJECT,nTotSize);

	CCompositeObj3D *pCptObj= DYNAMIC_DOWNCAST(CCompositeObj3D,pObj);
	if (pCptObj)
	{
		for (int i=pCptObj->nStartShow;i<pCptObj->m_cSubObjects.GetSize();i++)
		{
			CObject3D *pobj2 = pCptObj->m_cSubObjects.GetAt(i);
			OnCountObjects(pobj2,bAdd);
		}
	}

}

void CCalques3DDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_cNewUndoSet.IsEmpty());
}

void CCalques3DDoc::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_cNewRedoSet.IsEmpty());
}


BOOL CCalques3DDoc::AddUndoObject(CUndoObject *pUndoObj/*=NULL*/)
{
	if (!pUndoObj) return FALSE;

	for (POSITION pos = m_cNewRedoSet.GetHeadPosition (); pos != NULL;)
	{
		CUndoObject *pObj = m_cNewRedoSet.GetNext (pos);
		delete pObj;
	}
	m_cNewRedoSet.RemoveAll();
	m_cNewUndoSet.AddHead(pUndoObj);

	return TRUE;
}


BOOL CCalques3DDoc::AddObject(CObject3D* pObj,BOOL bUndo)
{
	if (!pObj) return FALSE;

	pObj->SetDepth();
	int nbObj = m_cObjectSet.GetSize();
	pObj->CalculConceptuel();
	pObj->nObjectId = nbObj+1;
	m_cObjectSet.Add(pObj);
	RenumberObjects();

	//if (DYNAMIC_DOWNCAST(CPolygon3D,pObj))
	//	m_cPolygonSet.Add(pObj);
	pObj->GetPolygons(&m_cPolygonSet);

	pObj->SetInGraph();

	OnCountObjects(pObj,TRUE);

	//OnPrepareUndo();
	if (bUndo)
	{
		m_nDocUndoState = UndoAppend;
		//m_cUndoSet.Add(pObj);

		CUndoObject * pUndoObj = new CUndoObject();
		pUndoObj->m_cUndoSet.Add(pObj);
		pUndoObj->m_bCanDelete = FALSE;
		pUndoObj->m_nUndoState = UndoAppend;

		//int nbO = CUndoButton::m_lstActions.GetCount();
		//UINT nCmdID = m_nUndoState + IDS_UNDO_NONE;
		//CString mstr,strID;
		//mstr.LoadString(nCmdID);
		//strID.Format(_T("%d - "),nbO+1);
		//mstr = strID + mstr;
		//CUndoButton::m_lstActions.AddHead (mstr);
		//CRedoButton::m_lstActions.AddHead (mstr);
		AddUndoObject(pUndoObj);
	}
	else
	{
		CleanUndo();
		m_nDocUndoState = UndoNone;
	}

	SetModifiedFlag(TRUE);

	//delete pList;
	return TRUE;
}

BOOL CCalques3DDoc::RemoveObject(CObject3D* pObj,BOOL bUndo)
{
	if (!pObj) return FALSE;

	CxObject3DSet mySet;
	pObj->GetDependList(&mySet);

	OnPrepareUndo();

	CUndoObject * pUndoObj = new CUndoObject();
	pUndoObj->m_bCanDelete = TRUE;
	pUndoObj->m_nUndoState = UndoDelete;


	int nbDel = mySet.GetSize();
	for (int j=nbDel-1;j>=0;j--)
	{
		CObject3D *zObj = mySet.GetAt(j);
		CObject3D *pMyObj = NULL;
		int nbObj = m_cObjectSet.GetSize();
		int nCount = -1;
		for (int i=0;i<nbObj;i++)
		{
			CObject3D* pO = m_cObjectSet.GetAt(i);
			if (pO != zObj) continue;

			pMyObj = zObj;
			nCount = i;
			break;
		}
		if (!pMyObj) continue;

		OnCountObjects(pMyObj,FALSE);
		//m_cUndoSet.Add(pMyObj);
		pUndoObj->m_cUndoSet.Add(pMyObj);
		m_cObjectSet.RemoveAt(nCount);
		pMyObj->SetInGraph(FALSE);
		RenumberObjects();


		CxObject3DSet polylist;
		pMyObj->GetPolygons(&polylist);
		for (int z=0;z<polylist.GetSize();z++)
		{
			CObject3D* ppoly = polylist.GetAt(z);
			if (!ppoly) continue;
			for (int j=0;j<m_cPolygonSet.GetSize();j++)
			{
				CObject3D* pO2 = m_cPolygonSet.GetAt(j);
				if (pO2==ppoly)
				{
					m_cPolygonSet.RemoveAt(j);
					break;
				}
			}
		}
		/*if (DYNAMIC_DOWNCAST(CPolygon3D,pMyObj))
		{
			for (int j=0;j<m_cPolygonSet.GetSize();j++)
			{
				CObject3D* pO2 = m_cPolygonSet.GetAt(j);
				if (pO2==pMyObj)
				{
					m_cPolygonSet.RemoveAt(j);
					break;
				}
			}
		}*/

		UpdateAllViews(NULL,WM_UPDATEOBJ_DEL,pMyObj);
	}

	AddUndoObject(pUndoObj);
	m_nDocUndoState = UndoDelete;
	SetModifiedFlag(TRUE);

	return TRUE;
}

BOOL CCalques3DDoc::ModifyPropObject(CObject3D* pObj,CObject3DAttr *pAttr /*= NULL*/)
{
	if (!pObj) return FALSE;
//	CObject3D *temp = pObj->CopyObject();
//	if (!temp) return FALSE;

	CUndoObject * pUndoObj = new CUndoObject();

	pUndoObj->m_pAttr = pObj->GetAttributes();
	//pUndoObj->m_pAttr.m_bVisible = pObj->bVisible;
	//pUndoObj->m_pAttr.m_bMarked = pObj->bMarked;
	//pUndoObj->m_pAttr.m_strObjectName = pObj->GetObjectName();
	//pUndoObj->m_pAttr.m_nCalque = pObj->nCalque;
	//pUndoObj->m_pAttr.m_pObjectShape = pObj->pObjectShape;

	BOOL bRet = TRUE;
	if (!pAttr)
	{
		bRet = pObj->SetProperties();
		if (bRet)
		{
			/*CShape pNew = CShape(temp->pObjectShape);
			//delete pObj->pObjectShape;
			pObj->pObjectShape = pNew;
			pObj->bVisible = temp->bVisible;
			pObj->bMarked = temp->bMarked;
			pObj->nCalque = pObj->nCalque & 1;*/

			OnPrepareUndo();
			m_nDocUndoState = UndoModify;
			//m_cUndoSet.Add(pObj);
			//m_pMemObj = temp;

			pUndoObj->m_cUndoSet.Add(pObj);
			pUndoObj->m_bCanDelete = FALSE;
			pUndoObj->m_nUndoState = UndoModify;

			//CUndoButton::m_lstActions.AddHead (mstr);
			m_cNewUndoSet.AddHead(pUndoObj);
			SetModifiedFlag(TRUE);
			UpdateAllViews(NULL,WM_UPDATEOBJ_MOD,pObj);
		}
		else
		{
			//delete temp;
			delete pUndoObj;
		}
	}
	else
	{
		pObj->SetAttributes(*pAttr);
		OnPrepareUndo();
		m_nDocUndoState = UndoModify;
		//m_cUndoSet.Add(pObj);
		//m_pMemObj = temp;

		pUndoObj->m_cUndoSet.Add(pObj);
		pUndoObj->m_bCanDelete = FALSE;
		pUndoObj->m_nUndoState = UndoModify;

		//CUndoButton::m_lstActions.AddHead (mstr);
		m_cNewUndoSet.AddHead(pUndoObj);
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL,WM_UPDATEOBJ_MOD,pObj);
	}

	return bRet;
}

BOOL CCalques3DDoc::MoveObject(CObject3D* pObj)
{
	if (!pObj) return FALSE;
//	CObject3D *temp = pObj->CopyObject();
//	if (!temp) return FALSE;

	OnPrepareUndo();
	m_nDocUndoState = UndoMove;
	//m_cUndoSet.Add(pObj);
	//m_pMemObj = temp;

		CUndoObject * pUndoObj = new CUndoObject();
		pUndoObj->m_cUndoSet.Add(pObj);
		pUndoObj->m_bCanDelete = FALSE;
		pUndoObj->m_nUndoState = UndoMove;
		
		CPoint3D *ptDes= DYNAMIC_DOWNCAST(CPoint3D,pObj);
		pUndoObj->m_ptOldPos = ptDes->Concept_pt;

		//int nbO = CUndoButton::m_lstActions.GetCount();
		//UINT nCmdID = m_nUndoState + IDS_UNDO_NONE;
		//CString mstr,strID;
		//mstr.LoadString(nCmdID);
		//strID.Format(_T("%d - "),nbO+1);
		//mstr = strID + mstr;
		//CUndoButton::m_lstActions.AddHead (mstr);

		AddUndoObject(pUndoObj);
		SetModifiedFlag(TRUE);

	return TRUE;
}


void CCalques3DDoc::OnPrepareUndo()
{
	/*int nb = m_cUndoSet.GetSize();
	int i;

	if (m_pMemObj) delete m_pMemObj;
	m_pMemObj = NULL;

	switch (m_nUndoState){
	case UndoDelete:
		for (i=0;i<nb;i++)
			delete 	m_cUndoSet.GetAt(i);
	case UndoModify:
	case UndoMove:
	case UndoAppend:
	case UndoNone:
		break;
	}
	m_cUndoSet.RemoveAll();*/
}

BOOL CCalques3DDoc::OnSetUndo()
{
	return TRUE;
}

void CCalques3DDoc::CleanUndo()
{
	POSITION pos = NULL;
	for (pos = m_cNewUndoSet.GetHeadPosition (); pos != NULL;)
	{
		CUndoObject *pObj = m_cNewUndoSet.GetNext (pos);
		delete pObj;
	}
	m_cNewUndoSet.RemoveAll();
	for (pos = m_cNewRedoSet.GetHeadPosition (); pos != NULL;)
	{
		CUndoObject *pObj = m_cNewRedoSet.GetNext (pos);
		delete pObj;
	}
	m_cNewRedoSet.RemoveAll();

}

BOOL CCalques3DDoc::OnDoRedo(CView *pView)
{
	if (m_cNewRedoSet.IsEmpty()) return FALSE;

	CUndoObject *pUndoObj = m_cNewRedoSet.GetHead();
	if (!pUndoObj) return FALSE;

	switch (pUndoObj->m_nUndoState){
	case UndoAppend:
		{
			pUndoObj->m_bCanDelete = FALSE;
			int nb = pUndoObj->m_cUndoSet.GetSize();
			for (int i=nb-1;i>=0;i--)
			{
				CObject3D *pObj = pUndoObj->m_cUndoSet.GetAt(i);
				if (!pObj) continue;
				int nbObj = m_cObjectSet.GetSize();
				pObj->CalculConceptuel();
				pObj->nObjectId = nbObj+1;
				m_cObjectSet.Add(pObj);
				pObj->SetInGraph();
				OnCountObjects(pObj,TRUE);

				//if (DYNAMIC_DOWNCAST(CPolygon3D,pObj))
				//	m_cPolygonSet.Add(pObj);
				pObj->GetPolygons(&m_cPolygonSet);


				UpdateAllViews(pView,WM_UPDATEOBJ_ADD,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
		}
		break;
	case UndoModify:
		{
			int nb = pUndoObj->m_cUndoSet.GetSize();
			//CString			strName;		///< Old name of the object
			//BOOL			bVisible;		///< Old visible flag
			//BOOL			bMarked;		///< Old Marked flag
			//int				nCalque;
			//CShape			pOldShape;	///< Old Shape

			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = pUndoObj->m_cUndoSet.GetAt(i);
				if (!pObj) continue;

				CObject3DAttr mAttr = pObj->GetAttributes();

				//bVisible = pObj->bVisible;
				//bMarked = pObj->bMarked;
				//strName = pObj->GetObjectName();
				//nCalque = pObj->nCalque;
				//pOldShape = pObj->pObjectShape;

				pObj->SetAttributes(pUndoObj->m_pAttr);

				pUndoObj->m_pAttr = mAttr;

				//pObj->bVisible = pUndoObj->m_bVisible;
				//pObj->bMarked = pUndoObj->m_bMarked;
				//pObj->nCalque = pUndoObj->m_nCalque;
				//pObj->pObjectShape = pUndoObj->m_pOldShape;
				//pObj->SetStyle(pObj->pObjectShape.nShapeId);
				//pObj->SetColor(pObj->pObjectShape.clrObject);
				//pObj->SetName(pUndoObj->m_strName);

				//pUndoObj->m_bVisible = bVisible;
				//pUndoObj->m_bMarked = bMarked;
				//pUndoObj->m_strName = strName;
				//pUndoObj->m_nCalque = nCalque;
				//pUndoObj->m_pOldShape = pOldShape;

				//OnPrepareUndo();
				//m_nUndoState = UndoNone;

				UpdateAllViews(pView,WM_UPDATEOBJ_MOD,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
			//if (TPref::Synchron)
		}
		break;
	case UndoMove:
		{
			int nb = pUndoObj->m_cUndoSet.GetSize();
			CVector4 ptNewPos;
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = pUndoObj->m_cUndoSet.GetAt(i);
				CxObject3DSet pSet;
				if (pObj->pComposite)
				{
					pObj->pComposite->GetDependList(&pSet);
				}
				else
				{
					pObj->GetDependList(&pSet);
				}
				int trr = pSet.GetSize();
				//CPoint3D *ptSrc= DYNAMIC_DOWNCAST(CPoint3D,m_pMemObj);
				CPoint3D *ptDes= DYNAMIC_DOWNCAST(CPoint3D,pObj);
				if (ptDes)
				{
					ptNewPos = ptDes->Concept_pt;
					ptDes->Concept_pt = pUndoObj->m_ptOldPos;
					ptDes->CopyPointPosition(pUndoObj->m_ptOldPos);
					pUndoObj->m_ptOldPos = ptNewPos;
				}

				//CObject3D* myObj = pSet.GetAt(j);
				for (int j=0;j<trr;j++)
				{
					CObject3D* myObj = pSet.GetAt(j);
					if (!myObj) continue;
					//if (myObj == pObj) continue;
					//if (myObj == pObj->pComposite) continue;
					myObj->CalculConceptuel();
					//myObj->CalculVisuel(myVisuParam);
				}
				if (pObj->pComposite)
				{
					pObj->CalculConceptuel();
				}
			UpdateAllViews(pView,WM_UPDATEOBJ_MOV,pObj);
			if (pView)
			{
				pView->Invalidate();
				pView->UpdateWindow();
			}
				//OnPrepareUndo();
				//m_nUndoState = UndoNone;
			}
			//if (TPref::Synchron)
		}
		break;
	case UndoDelete:
		{
			pUndoObj->m_bCanDelete = TRUE;
			int nb = pUndoObj->m_cUndoSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = pUndoObj->m_cUndoSet.GetAt(i);
				if (!pObj) continue;
				CObject3D* pMyObj = NULL;
				int nbObj = m_cObjectSet.GetSize();
				int nCount = -1;
				for (int j=0;j<nbObj;j++)
				{
					CObject3D* pO = m_cObjectSet.GetAt(j);
					if (pO != pObj) continue;
	
					pMyObj = pObj;
					nCount = j;
					break;
				}
				if (!pMyObj) continue;

				OnCountObjects(pMyObj,FALSE);
				//m_cUndoSet.Add(pMyObj);
				m_cObjectSet.RemoveAt(nCount);
				pMyObj->SetInGraph(FALSE);
				RenumberObjects();

				CxObject3DSet polylist;
				pObj->GetPolygons(&polylist);
				for (int z=0;z<polylist.GetSize();z++)
				{
					CObject3D* ppoly = polylist.GetAt(z);
					if (!ppoly) continue;
					for (int j=0;j<m_cPolygonSet.GetSize();j++)
					{
						CObject3D* pO2 = m_cPolygonSet.GetAt(j);
						if (pO2==ppoly)
						{
							m_cPolygonSet.RemoveAt(j);
							break;
						}
					}
				}
				/*if (DYNAMIC_DOWNCAST(CPolygon3D,pObj))
				{
					for (int j=0;j<m_cPolygonSet.GetSize();j++)
					{
						CObject3D* pO2 = m_cPolygonSet.GetAt(j);
						if (pO2==pObj)
						{
							m_cPolygonSet.RemoveAt(j);
							break;
						}
					}
				}*/


				UpdateAllViews(pView,WM_UPDATEOBJ_DEL,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
			//m_nUndoState = UndoDelete;
			//OnPrepareUndo();
			//m_nUndoState = UndoNone;
		}
		break;
	default:
		return FALSE;
	}

	m_cNewRedoSet.RemoveHead();
	m_cNewUndoSet.AddHead(pUndoObj);

	return TRUE;
} 

BOOL CCalques3DDoc::OnDoUndo(CView *pView)
{
	if (m_cNewUndoSet.IsEmpty()) return FALSE;

	CUndoObject *pUndoObj = m_cNewUndoSet.GetHead();
	if (!pUndoObj) return FALSE;

	switch (pUndoObj->m_nUndoState){
	case UndoAppend:
		{
			pUndoObj->m_bCanDelete = TRUE;
			int nb = pUndoObj->m_cUndoSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = pUndoObj->m_cUndoSet.GetAt(i);
				if (!pObj) continue;
				CObject3D* pMyObj = NULL;
				int nbObj = m_cObjectSet.GetSize();
				int nCount = -1;
				for (int j=0;j<nbObj;j++)
				{
					CObject3D* pO = m_cObjectSet.GetAt(j);
					if (pO != pObj) continue;
	
					pMyObj = pObj;
					nCount = j;
					break;
				}
				if (!pMyObj) continue;

				OnCountObjects(pMyObj,FALSE);
				//m_cUndoSet.Add(pMyObj);
				m_cObjectSet.RemoveAt(nCount);
				pMyObj->SetInGraph(FALSE);
				RenumberObjects();

				CxObject3DSet polylist;
				pObj->GetPolygons(&polylist);
				for (int z=0;z<polylist.GetSize();z++)
				{
					CObject3D* ppoly = polylist.GetAt(z);
					if (!ppoly) continue;
					for (int j=0;j<m_cPolygonSet.GetSize();j++)
					{
						CObject3D* pO2 = m_cPolygonSet.GetAt(j);
						if (pO2==ppoly)
						{
							m_cPolygonSet.RemoveAt(j);
							break;
						}
					}
				}
				/*if (DYNAMIC_DOWNCAST(CPolygon3D,pObj))
				{
					for (int j=0;j<m_cPolygonSet.GetSize();j++)
					{
						CObject3D* pO2 = m_cPolygonSet.GetAt(j);
						if (pO2==pObj)
						{
							m_cPolygonSet.RemoveAt(j);
							break;
						}
					}
				}*/


				UpdateAllViews(pView,WM_UPDATEOBJ_DEL,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
			//m_nUndoState = UndoDelete;
			//OnPrepareUndo();
			//m_nUndoState = UndoNone;
		}
		break;
	case UndoModify:
		{
			int nb = pUndoObj->m_cUndoSet.GetSize();
			//CString			strName;
			//BOOL			bVisible;
			//BOOL			bMarked;
			//int				nCalque;
			//CShape			pOldShape;
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = pUndoObj->m_cUndoSet.GetAt(i);
				if (!pObj) continue;

				CObject3DAttr mAttr = pObj->GetAttributes();

				pObj->SetAttributes(pUndoObj->m_pAttr);

				pUndoObj->m_pAttr = mAttr;


				/*bVisible = pObj->bVisible;
				bMarked = pObj->bMarked;
				strName = pObj->GetObjectName();
				nCalque = pObj->nCalque;
				pOldShape = pObj->pObjectShape;

				pObj->bVisible = pUndoObj->m_bVisible;
				pObj->bMarked = pUndoObj->m_bMarked;
				pObj->nCalque = pUndoObj->m_nCalque;
				pObj->pObjectShape = pUndoObj->m_pOldShape;
				pObj->SetStyle(pObj->pObjectShape.nShapeId);
				pObj->SetColor(pObj->pObjectShape.clrObject);
				pObj->SetName(pUndoObj->m_strName);

				pUndoObj->m_bVisible = bVisible;
				pUndoObj->m_bMarked = bMarked;
				pUndoObj->m_strName = strName;
				pUndoObj->m_nCalque = nCalque;
				pUndoObj->m_pOldShape = pOldShape;*/

				//OnPrepareUndo();
				//m_nUndoState = UndoNone;

				UpdateAllViews(pView,WM_UPDATEOBJ_MOD,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
			//if (TPref::Synchron)
		}
		break;
	case UndoMove:
		{
			int nb = pUndoObj->m_cUndoSet.GetSize();
			CVector4 ptNewPos;
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = pUndoObj->m_cUndoSet.GetAt(i);
				CxObject3DSet pSet;
				if (pObj->pComposite)
				{
					pObj->pComposite->GetDependList(&pSet);
				}
				else
				{
					pObj->GetDependList(&pSet);
				}
				int trr = pSet.GetSize();
				//CPoint3D *ptSrc= DYNAMIC_DOWNCAST(CPoint3D,m_pMemObj);
				CPoint3D *ptDes= DYNAMIC_DOWNCAST(CPoint3D,pObj);
				if (ptDes)
				{
					ptNewPos = ptDes->Concept_pt;
					ptDes->Concept_pt = pUndoObj->m_ptOldPos;
					ptDes->CopyPointPosition(pUndoObj->m_ptOldPos);
					pUndoObj->m_ptOldPos = ptNewPos;
				}

				//CObject3D* myObj = pSet.GetAt(j);
				for (int j=0;j<trr;j++)
				{
					CObject3D* myObj = pSet.GetAt(j);
					if (!myObj) continue;
					//if (myObj == pObj) continue;
					//if (myObj == pObj->pComposite) continue;
					myObj->CalculConceptuel();
					//myObj->CalculVisuel(myVisuParam);
				}
				if (pObj->pComposite)
				{
					pObj->CalculConceptuel();
				}
			UpdateAllViews(pView,WM_UPDATEOBJ_MOV,pObj);
			if (pView)
			{
				pView->Invalidate();
				pView->UpdateWindow();
			}
				//OnPrepareUndo();
				//m_nUndoState = UndoNone;
			}
			//if (TPref::Synchron)
		}
		break;
	case UndoDelete:
		{
			pUndoObj->m_bCanDelete = FALSE;
			int nb = pUndoObj->m_cUndoSet.GetSize();
			for (int i=nb-1;i>=0;i--)
			{
				CObject3D *pObj = pUndoObj->m_cUndoSet.GetAt(i);
				if (!pObj) continue;
				int nbObj = m_cObjectSet.GetSize();
				pObj->CalculConceptuel();
				pObj->nObjectId = nbObj+1;
				m_cObjectSet.Add(pObj);
				pObj->SetInGraph();
				OnCountObjects(pObj,TRUE);

				//if (DYNAMIC_DOWNCAST(CPolygon3D,pObj))
				//	m_cPolygonSet.Add(pObj);
				pObj->GetPolygons(&m_cPolygonSet);

				UpdateAllViews(pView,WM_UPDATEOBJ_ADD,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
			//m_nUndoState = UndoAppend;
			//OnPrepareUndo();
			//m_nUndoState = UndoNone;
		}
		break;
	default:
		return FALSE;
	}

	m_cNewUndoSet.RemoveHead();
	m_cNewRedoSet.AddHead(pUndoObj);

/*	switch (m_nUndoState){
	case UndoNone:
	case UndoDelete:
		{
			int nb = m_cUndoSet.GetSize();
			for (int i=nb-1;i>=0;i--)
			{
				CObject3D *pObj = m_cUndoSet.GetAt(i);
				if (!pObj) continue;
				int nbObj = m_cObjectSet.GetSize();
				pObj->CalculConceptuel();
				pObj->nObjectId = nbObj+1;
				m_cObjectSet.Add(pObj);
				pObj->SetInGraph();
				OnCountObjects(pObj,TRUE);

				if (DYNAMIC_DOWNCAST(CPolygon3D,pObj))
					m_cPolygonSet.Add(pObj);

				UpdateAllViews(pView,WM_UPDATEOBJ_ADD,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
			m_nUndoState = UndoAppend;
			OnPrepareUndo();
			m_nUndoState = UndoNone;
		}
	case UndoAppend:
		{
			int nb = m_cUndoSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = m_cUndoSet.GetAt(i);
				if (!pObj) continue;
				CObject3D* pMyObj = NULL;
				int nbObj = m_cObjectSet.GetSize();
				for (int j=0;j<nbObj;j++)
				{
					CObject3D* pO = m_cObjectSet.GetAt(j);
					if (pO != pObj) continue;
	
					pMyObj = pObj;
					break;
				}
				if (!pMyObj) continue;

				OnCountObjects(pMyObj,FALSE);
				//m_cUndoSet.Add(pMyObj);
				m_cObjectSet.RemoveAt(j);
				pMyObj->SetInGraph(FALSE);
				RenumberObjects();

				if (DYNAMIC_DOWNCAST(CPolygon3D,pObj))
				{
					for (int j=0;j<m_cPolygonSet.GetSize();j++)
					{
						CObject3D* pO2 = m_cPolygonSet.GetAt(j);
						if (pO2==pObj)
						{
							m_cPolygonSet.RemoveAt(j);
							break;
						}
					}
				}


				UpdateAllViews(pView,WM_UPDATEOBJ_DEL,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
			m_nUndoState = UndoDelete;
			OnPrepareUndo();
			m_nUndoState = UndoNone;
		}
		break;
	case UndoModify:
		{
			int nb = m_cUndoSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = m_cUndoSet.GetAt(i);
				if (!pObj) continue;

				pObj->pObjectShape = m_pMemObj->pObjectShape;
				pObj->bVisible= m_pMemObj->bVisible;
				pObj->bMarked= m_pMemObj->bMarked;
				pObj->SetStyle(pObj->pObjectShape.nShapeId);
				pObj->SetColor(pObj->pObjectShape.clrObject);
				pObj->SetName(m_pMemObj->GetObjectName());

				OnPrepareUndo();
				m_nUndoState = UndoNone;

				UpdateAllViews(pView,WM_UPDATEOBJ_MOD,pObj);
				if (pView)
				{
					pView->Invalidate();
					pView->UpdateWindow();
				}
			}
			//if (TPref::Synchron)
		}
		break;
	case UndoMove:
		{
			int nb = m_cUndoSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D *pObj = m_cUndoSet.GetAt(i);
				CxObject3DSet pSet;
				if (pObj->pComposite)
				{
					pObj->pComposite->GetDependList(&pSet);
				}
				else
				{
					pObj->GetDependList(&pSet);
				}
				int trr = pSet.GetSize();
				CPoint3D *ptSrc= DYNAMIC_DOWNCAST(CPoint3D,m_pMemObj);
				CPoint3D *ptDes= DYNAMIC_DOWNCAST(CPoint3D,pObj);
				if (ptSrc && ptDes)
				{
					ptDes->Concept_pt = ptSrc->Concept_pt;
					ptDes->CopyPointPosition(ptSrc);
				}

				//CObject3D* myObj = pSet.GetAt(j);
				for (int j=0;j<trr;j++)
				{
					CObject3D* myObj = pSet.GetAt(j);
					if (!myObj) continue;
					//if (myObj == pObj) continue;
					//if (myObj == pObj->pComposite) continue;
					myObj->CalculConceptuel();
					//myObj->CalculVisuel(myVisuParam);
				}
				if (pObj->pComposite)
				{
					pObj->CalculConceptuel();
				}
			UpdateAllViews(pView,WM_UPDATEOBJ_MOV,pObj);
			if (pView)
			{
				pView->Invalidate();
				pView->UpdateWindow();
			}
				OnPrepareUndo();
				m_nUndoState = UndoNone;
			}
			//if (TPref::Synchron)
		}
		break;
	}*/
	return TRUE;
}


void CCalques3DDoc::SaveWindowPos()
{
	m_cWinPos.RemoveAll();
	POSITION pos = GetFirstViewPosition();
	while (pos)
	{
		CView *pView = GetNextView(pos);
		if (!pView) continue;
		CString mstr;
		WINDOWPLACEMENT m_pl;

		CRuntimeClass *pRTC = pView->GetRuntimeClass();

		POSITION pos2 = AfxGetApp()->GetFirstDocTemplatePosition();
		while (pos2 != NULL)
		{
			CDocTemplate* ptempl = AfxGetApp()->GetNextDocTemplate(pos2) ;
			CCalques3DDocTemplate *pMyT= DYNAMIC_DOWNCAST(CCalques3DDocTemplate,ptempl);

			if (pMyT)
			{
				CRuntimeClass *pRTC2 = pMyT->GetViewRT();
				
				if (pRTC == pRTC2)
				{
					pView->GetParentFrame()->GetWindowText(mstr);
					pView->GetParentFrame()->GetWindowPlacement(&m_pl);
					int nb = mstr.Find(' ');
					CString tt = mstr.Left(nb);

					CWinPlacement mypl(pMyT->GetViewID(),m_pl);
					m_cWinPos.SetAt(pMyT->GetViewID(),mypl);
					break;
				}
			}
		}


	}
	pos = m_cWinPos.GetStartPosition();
	while (pos)
	{
		UINT mstr;
		CWinPlacement mypl;
		m_cWinPos.GetNextAssoc(pos,mstr,mypl);
	}
}

void CCalques3DDoc::RestoreWindowPos()
{
	POSITION pos = GetFirstViewPosition();
	while (pos)
	{
		CView *pView = GetNextView(pos);
		if (!pView) continue;
		
		CRuntimeClass *pRTC = pView->GetRuntimeClass();

		POSITION pos2 = AfxGetApp()->GetFirstDocTemplatePosition();
		while (pos2 != NULL)
		{
			CDocTemplate* ptempl = AfxGetApp()->GetNextDocTemplate(pos2) ;
			CCalques3DDocTemplate *pMyT= DYNAMIC_DOWNCAST(CCalques3DDocTemplate,ptempl);

			if (pMyT)
			{
				CRuntimeClass *pRTC2 = pMyT->GetViewRT();
				if (pRTC == pRTC2)
				{
					UINT nID = pMyT->GetViewID();
					CWinPlacement mypl;
					BOOL bRet = m_cWinPos.Lookup(nID,mypl);
					if (bRet)
					{
						mypl.m_pl.length = sizeof(WINDOWPLACEMENT);
						pView->GetParentFrame()->SetWindowPlacement(&(mypl.m_pl));
					}

				}
			}
		}
	}
}

void CCalques3DDoc::UpdateAllViews(CView* pSender, LPARAM lHint, CObject* pHint)
	// walk through all views
{
	CDocument::UpdateAllViews(pSender, lHint, pHint);

	switch (lHint)
	{
	case WM_UPDATEOBJ_SEL:	// Object Selected
		{
			//####################### CHANGE FOR RIBBON 
			BOOL useRbn = TPref::TInterface.bUseRibbon;
			if (useRbn) return;
			CObject3D *pObj= DYNAMIC_DOWNCAST(CObject3D,pHint);
			CMainFrame *pWnd = DYNAMIC_DOWNCAST (CMainFrame, AfxGetMainWnd());
			if (pObj && pWnd) 
			{
		        pWnd->GetPropertyBar()->AddProperties(pObj);
		        pWnd->GetDependentBar()->AddProperties(pObj);
				CFormatToolBar::OnUpdateCmdUI(pObj);
			}
			else if (pWnd)
			{	
				pWnd->GetPropertyBar()->AddProperties(NULL);
				pWnd->GetDependentBar()->AddProperties(NULL);
			}
		}
		break;
	}
}


BOOL CCalques3DDoc::IsTaskAvailable(UINT m_nID)
{
	BOOL bEnab = FALSE;
	int		nbObj	= GetObjectSize(IDS_NAME_OBJECT),
			nbPt	= GetObjectSize(IDS_NAME_POINT),
			nbSg	= GetObjectSize(IDS_NAME_SEG),
			nbLn	= GetObjectSize(IDS_NAME_LINE),
			nbCr	= GetObjectSize(IDS_NAME_CIRCLE),
			nbPl	= GetObjectSize(IDS_NAME_PLANE),
			nbSp	= GetObjectSize(IDS_NAME_SPHERE),
			nbCb	= GetObjectSize(IDS_NAME_CUBE),
			nbCn	= GetObjectSize(IDS_NAME_CONE),
			nbCy	= GetObjectSize(IDS_NAME_CYLIN);
	switch (m_nID){
	case ID_EXPLORATION_MOVE:
	case ID_EXPLORATION_LABEL:
	case ID_EXPLORATION_EQUATION:
	case ID_VISUALISATION_CENTERON:
		bEnab = nbPt;
		break;
	case ID_VISUALISATION_DEFAULTTASK:
	case ID_OBJECT_POINT:
	case ID_MACROS_USER1:
	case ID_MACROS_USER2:
	case ID_MACROS_USER3:
	case ID_MACROS_USER4:
	case ID_MACROS_USER5:
	case ID_MACROS_USER6:
	case ID_MACROS_USER7:
	case ID_MACROS_USER8:
	case ID_MACROS_USER9:
	case ID_MACROS_USER10:
	case ID_MACROS_USER11:
	case ID_MACROS_USER12:
	case ID_MACROS_USER13:
	case ID_MACROS_USER14:
	case ID_MACROS_USER15:
		bEnab = TRUE;
		break;
	case ID_MACROS_LOAD:
		//bEnab = TRUE;
		bEnab = TPref::bMacroLoading;

		break;
	case ID_OBJECT_SEGMENT:
	case ID_OBJECT_RAY:
	case ID_OBJECT_LINE:
	case ID_CONSTRUCTION_MIDPOINT:
	case ID_OBJECT_CUBE:
	case ID_EXPLORATION_LOCUS:
	case ID_EXPLORATION_MEASURE:
		bEnab = (nbPt>=2);
		break;
	case ID_OBJECT_SPHERE:
		bEnab = (nbPt>=2) || (nbPt==1 && nbSg);
		break;
	case ID_OBJECT_DELETE:
	case ID_EXPLORATION_EXTRACTION:
	case ID_MACROS_SAVE:
		bEnab = nbObj;
		break;
	case ID_VISUALISATION_APPLYSTYLE:
	case ID_OBJECT_REDEFINE:
	case ID_EXPLORATION_VERIFY:
		bEnab = (nbObj >=2);
		break;
	case ID_OBJECT_CIRCLE:
	case ID_OBJECT_ARCCIRCLE:
	case ID_OBJECT_PLANE:
	case ID_OBJECT_POLYGON:
	case ID_OBJECT_CYLINDRE:
	case ID_OBJECT_CONE:
	case ID_CONSTRUCTION_TRANSLATION:
	case ID_EXPLORATION_ANIMATION:
	case ID_EXPLORATION_VERIFY_PTALIGN:
		bEnab = (nbPt>=3);
		break;
	case ID_EXPLORATION_VERIFY_PAR:
	case ID_EXPLORATION_VERIFY_PERP:
		bEnab = (nbLn>=2) || (nbPl>=2);
		break;		
	case ID_EXPLORATION_VERIFY_PTON:
		bEnab = (nbPt>=1) && (nbPl || nbLn || nbCr || nbSp);
		break;		
	case ID_CONSTRUCTION_POINTON_CYLINDER:
		bEnab = nbCy;
		break;
	case ID_CONSTRUCTION_POINTON_CONE:
		bEnab = nbCn;
		break;
	case ID_CONSTRUCTION_POINTON_LINE:
		bEnab = nbLn;
		break;
	case ID_CONSTRUCTION_POINTON_CIRCLE:
	case ID_CONSTRUCTION_CENTER:
		bEnab = nbCr;
		break;
	case ID_CONSTRUCTION_POINTON_PLANE:
		bEnab = nbPl;
		break;
	case ID_CONSTRUCTION_POINTON_SPHERE:
		bEnab = nbSp;
		break;
	case ID_CONSTRUCTION_INTERSECTION_LINELINE:
	case ID_CONSTRUCTION_CROSSPRODUCT:
		bEnab = (nbLn>=2);
		break;
	case ID_CONSTRUCTION_INTERSECTION_LINEPLANE:
		bEnab = nbLn && nbPl;
		break;
	case ID_CONSTRUCTION_INTERSECTION_LINESPHERE:
		bEnab = nbLn && nbSp;
		break;
	case ID_CONSTRUCTION_INTERSECTION_LINECIRCLE:
		bEnab = nbLn && nbCr;
		break;
	case ID_CONSTRUCTION_INTERSECTION_PLANECIRCLE:
		bEnab = nbPl && nbCr;
		break;
	case ID_CONSTRUCTION_INTERSECTION_LINECONE:
		bEnab = nbLn && nbCn;
		break;
	case ID_CONSTRUCTION_INTERSECTION_SPHEREPLANE:
		bEnab = nbPl && nbSp;
		break;
	case ID_CONSTRUCTION_INTERSECTION_SPHERESPHERE:
		bEnab = (nbSp>=2);
		break;
	case ID_CONSTRUCTION_INTERSECTION_PLANEPLANE:
		bEnab = (nbPl>=2);
		break;
	case ID_CONSTRUCTION_INTERSECTION_PLANECYLINDER:
		bEnab = nbPl && nbCy;
		break;
	case ID_CONSTRUCTION_INTERSECTION_PLANECONE:
		bEnab = nbPl && nbCn;
		break;
	case ID_CONSTRUCTION_PARALLELLINE:
	case ID_CONSTRUCTION_PERPENDICULARLINE:
	case ID_CONSTRUCTION_PERPENDICULARPLANE:
		bEnab = nbLn && nbPt;
		break;
	case ID_CONSTRUCTION_DIVSEGMENT:
		bEnab = nbSg;
		break;
	case ID_CONSTRUCTION_NORMALLINE:
		bEnab = nbPt && nbPl;
		break;
	case ID_CONSTRUCTION_SYMMETRICALPOINT:
		bEnab = (nbPt && (nbLn || nbPl)) || (nbPt >=2);
		break;
	case ID_VISUALISATION_PROJECTION:
		bEnab = (nbPt >= 3) || (nbPl);
		break;
	default:
		bEnab = FALSE;
		break;
	}
	return bEnab;
}

void CCalques3DDoc::OnDiscovery()
{
}

CString CCalques3DDoc::OnExportSymbolic(UINT nFormat) 
{
	// TODO: Add your command handler code here
	CString strSymb;

	//strSymb = _T("Range[-8.0,8.0,-8.0,8.0,-6.0,8.0];\n");
	CVector4 mina(10000,10000,10000),maxa(0,0,0);
	int nb = m_cObjectSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D* pObj = m_cObjectSet.GetAt(i);
		if (!pObj) continue;
		if (DYNAMIC_DOWNCAST(CText3D,pObj)) continue;

		CVector4 omin,omax;
		pObj->GetRange(omin,omax);

		mina.x = min(omin.x,mina.x);
		mina.y = min(omin.y,mina.y);
		mina.z = min(omin.z,mina.z);
		maxa.x = max(omax.x,maxa.x);
		maxa.y = max(omax.y,maxa.y);
		maxa.z = max(omax.z,maxa.z);

		CString mstr = pObj->ExportSymbolic(nFormat);
		//if (!mstr.IsEmpty())
		{
			strSymb += mstr + _T("\n");
		}
	}
	mina.x = (mina.x/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;
	mina.y = (mina.y/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;
	mina.z = (mina.z/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;
	maxa.x = (maxa.x/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;
	maxa.y = (maxa.y/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;
	maxa.z = (maxa.z/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;

	CVector4 delta(0.5,0.5,0.5);

	mina = mina - delta;
	maxa = maxa + delta;

	CString strRange;
	if (nFormat==CObject3D::EXPORT_MATHEMATICA)
		strRange.Format(_T("Range[%.1f,%.1f,%.1f,%.1f,%.1f,%.1f];\n"),
							mina.x,maxa.x,mina.y,maxa.y,mina.z,maxa.z);
	else 
	if (nFormat==CObject3D::EXPORT_MAPLE)
		strRange.Format(_T("Range(%.1f,%.1f,%.1f,%.1f,%.1f,%.1f);\n"),
							mina.x,maxa.x,mina.y,maxa.y,mina.z,maxa.z);
	
	strSymb = strRange + strSymb;

	return strSymb;
}


void CCalques3DDoc::OnViewPlacementSave() 
{
	// TODO: Add your command handler code here
	SaveWindowPos();
}

void CCalques3DDoc::OnViewPlacementRestore() 
{
	// TODO: Add your command handler code here
	RestoreWindowPos();
}

void CCalques3DDoc::OnUpdatePlacementRestore(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_cWinPos.GetCount()!=0);
}


//***************************************************************************
// CCalques3DMacroDoc
//***************************************************************************
IMPLEMENT_DYNCREATE(CCalques3DMacroDoc, CCalques3DDoc)

BEGIN_MESSAGE_MAP(CCalques3DMacroDoc, CCalques3DDoc)
	//{{AFX_MSG_MAP(CCalques3DMacroDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc construction/destruction

CCalques3DMacroDoc::CCalques3DMacroDoc()
{
}

void CCalques3DMacroDoc::Serialize(CArchive& ar)
{
	CCalques3DDoc::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << strObjectName;
		ar << strObjectHelp;
		ar << strObjectDef;
		ar << nSortKind;
	}
	else
	{
		ar >> strObjectName;
		ar >> strObjectHelp;
		ar >> strObjectDef;
		ar >> nSortKind;
	}
}