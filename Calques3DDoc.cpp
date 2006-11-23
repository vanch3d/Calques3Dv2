// Calques3DDoc.cpp : implementation of the CCalques3DDoc class
//

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CxObject3DSet*	GLOBALObjectSet;
CxObject3DSet* CCalques3DDoc::GLOBALObjectSet = NULL;

/////////////////////////////////////////////////////////////////////////////
// CWinPlacement
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


/////////////////////////////////////////////////////////////////////////////
// CUndoObject
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


/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc

//IMPLEMENT_DYNCREATE(CCalques3DDoc, CDocument)
IMPLEMENT_SERIAL(CCalques3DDoc, CDocument, VERSIONABLE_SCHEMA | 1)

BEGIN_MESSAGE_MAP(CCalques3DDoc, CDocument)
	//{{AFX_MSG_MAP(CCalques3DDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_EXPLORATION_DISCOVERY, OnDiscovery)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc construction/destruction

CCalques3DDoc::CCalques3DDoc()
{
	// TODO: add one-time construction code here
	m_nUndoState = UndoNone;

	m_cObjectSet.RemoveAll();
	m_cPolygonSet.RemoveAll();
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
	//if (m_pMemObj)
	//	delete m_pMemObj;
	//m_pMemObj = NULL;

	for (POSITION pos = m_cNewUndoSet.GetHeadPosition (); pos != NULL;)
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

	//OnPrepareUndo();
}

BOOL CCalques3DDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	int idx = 1;

	return TRUE;
}

BOOL CCalques3DDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

BOOL CCalques3DDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CCalques3DDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
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

void CCalques3DDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		m_cObjectSet.Serialize(ar);
	}
	else
	{
		UINT schema = ar.GetObjectSchema();
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
			AddObject(pObj);
		}
		CCalques3DDoc::GLOBALObjectSet = NULL;
	}
/*	try
	{	
		m_cWinPos.Serialize(ar);
	}
	catch (CException* pEx)
	{
		pEx->Delete ();
		m_strPathName.Empty();      // no path name yet
		SetModifiedFlag(TRUE);     // make clean
	}*/
	
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
	case IDS_NAME_POINT:
	case IDS_NAME_SEG:
	case IDS_NAME_CIRCLE:
	case IDS_NAME_PLANE:
	case IDS_NAME_SPHERE:
	case IDS_NAME_CUBE:
	case IDS_NAME_CYLIN:
	case IDS_NAME_OBJECT:
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
		m_nUndoState = UndoAppend;
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
		m_nUndoState = UndoNone;
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
		for (int i=0;i<nbObj;i++)
		{
			CObject3D* pO = m_cObjectSet.GetAt(i);
			if (pO != zObj) continue;

			pMyObj = zObj;
			break;
		}
		if (!pMyObj) continue;

		OnCountObjects(pMyObj,FALSE);
		//m_cUndoSet.Add(pMyObj);
		pUndoObj->m_cUndoSet.Add(pMyObj);
		m_cObjectSet.RemoveAt(i);
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
	m_nUndoState = UndoDelete;
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
			m_nUndoState = UndoModify;
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
		m_nUndoState = UndoModify;
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
	m_nUndoState = UndoMove;
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
			CObject3D *pObj= DYNAMIC_DOWNCAST(CObject3D,pHint);
			CMainFrame *pWnd = DYNAMIC_DOWNCAST (CMainFrame, AfxGetMainWnd());
			if (pObj && pWnd) 
			{
		        pWnd->GetPropertyBar()->AddProperties(pObj);
		        pWnd->GetDependentBar()->AddProperties(pObj);
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
	case ID_OBJECT_SPHERE:
	case ID_EXPLORATION_LOCUS:
	case ID_EXPLORATION_MEASURE:
		bEnab = (nbPt>=2);
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
	case ID_CONSTRUCTION_TRANSLATION:
	case ID_EXPLORATION_ANIMATION:
		bEnab = (nbPt>=3);
		break;
	case ID_CONSTRUCTION_POINTON_CYLINDER:
		bEnab = nbCy;
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
	case ID_CONSTRUCTION_INTERSECTION_SPHERESPHERE:
		bEnab = (nbSp>=2);
		break;
	case ID_CONSTRUCTION_INTERSECTION_PLANEPLANE:
		bEnab = (nbPl>=2);
		break;
	case ID_CONSTRUCTION_INTERSECTION_PLANECYLINDER:
		bEnab = nbPl && nbCy;
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
	return;
	static BOOL bD = TRUE;
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

		CString mstr = pObj->DrawSymbolic();
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

/*	mina.x --;
	mina.y --;
	mina.z --;
	maxa.x ++;
	maxa.y ++;
	maxa.z ++;
*/
	CString strRange;
	strRange.Format(_T("Range[%.1f,%.1f,%.1f,%.1f,%.1f,%.1f];\n"),
						mina.x,maxa.x,mina.y,maxa.y,mina.z,maxa.z);

	strSymb = strRange + strSymb;

//	CFileFind mfile;
//	mfile.FindFile(TPref::Verif.strConvert);
//	BOOL bRes = mfile.FindNextFile();
//	CString strff = mfile.GetFilePath();



	int nErrNo = _access(TPref::Verif.strConvert,00);
	if (nErrNo!=0)
	{
	}
	nErrNo = _access(TPref::Verif.strRunTime,00);
	if (nErrNo!=0)
	{
	}

	CString lpszCommandLine;

	//lpszCommandLine = _T("command.com /E:2048 /C c:\progra~1\cocoa-4.1\cocoa.bat -q c:\\progra~1\\cocoa-4.1\\test.coc");
	lpszCommandLine.Format(_T("%s %s %s"),
		TPref::Verif.strCommand,
		TPref::Verif.strRunTime,
		TPref::Verif.strConvert);

	STARTUPINFO siStartInfo;
	PROCESS_INFORMATION piProcInfo;
	memset(&siStartInfo, 0, sizeof(siStartInfo));
	memset(&piProcInfo, 0, sizeof(piProcInfo));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.wShowWindow = (WORD)SW_SHOW;//((bD)?SW_SHOW:SW_HIDE);
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW;


	
	// Load the predefined Windows "up arrow" cursor.
	CWaitCursor wait;
	// export the selected file into registry using command line

//	int nrr = _spawnl(_P_WAIT,lpszCommandLine.GetBuffer(lpszCommandLine.GetLength())," ",NULL,NULL);

	if (CreateProcess (NULL, lpszCommandLine.GetBuffer(lpszCommandLine.GetLength()), 
							NULL, NULL, FALSE, 0, 
							NULL, NULL, &siStartInfo, &piProcInfo))
	{
		DWORD dResult = WAIT_TIMEOUT;
		while (dResult!= WAIT_OBJECT_0)
		{
			CWaitCursor wait;
			dResult = WaitForSingleObject (piProcInfo.hProcess, 500);
			AfxGetMainWnd()->Invalidate();
			AfxGetMainWnd()->UpdateWindow();
		}

		//DWORD dResult = WaitForSingleObject (piProcInfo.hProcess, INFINITE);
		DWORD d1 = WAIT_ABANDONED;
		DWORD d2 = WAIT_OBJECT_0;
		DWORD d3 = WAIT_TIMEOUT;
		DWORD d4 = WAIT_FAILED;
		if (dResult==d2)
		{
			int nErrNo = _access(TPref::Verif.strResult,00);
			if (nErrNo!=0)
			{
			}
		FILE *fp = NULL;
			fp = fopen(TPref::Verif.strResult,"r");
		TRY
		{
	
			CStdioFile f(fp);
			f.ReadString(strSymb);
			f.Close();
		}
		CATCH( CFileException, e )
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		}
		END_CATCH

		AfxGetMainWnd()->MessageBox(strSymb);
		}
		else if (dResult==d3)
		{
		}
		else if (dResult==d4)
		{
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL);
			// Display the string.
			::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
			// Free the buffer.
			LocalFree( lpMsgBuf );
 		}
	}

	bD = !bD;
}


/////////////////////////////////////////////////////////////////////////////
// CCalques3DDoc

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
