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
/// @file Calques3DDoc.h
/// @brief interface of the CCalques3DDoc class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CALQUES3DDOC_H__1B2AE4AC_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_CALQUES3DDOC_H__1B2AE4AC_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "objects\Object3d.h"
#include "prefs\POVTool.h"

/////////////////////////////////////////////////////////////////////////////
/// Storage of the windows' positions
/////////////////////////////////////////////////////////////////////////////
class CWinPlacement :  public CObject
{
public:
	DECLARE_SERIAL(CWinPlacement);

	WINDOWPLACEMENT m_pl;
	UINT			m_nIDWin;
	CWinPlacement();
	CWinPlacement(UINT str,WINDOWPLACEMENT tt);
	CWinPlacement(const CWinPlacement& src);
	void operator =(const CWinPlacement& other);
	void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////
// List of positions of every open views in Calques 3D
/////////////////////////////////////////////////////////////////////////////
typedef CMap<UINT ,UINT,CWinPlacement,CWinPlacement> CxWindowLoc;

/////////////////////////////////////////////////////////////////////////////
/// Wrapper for handling undo/redo operations
/////////////////////////////////////////////////////////////////////////////
class CUndoObject: public CObject
{
public:
	DECLARE_SERIAL(CUndoObject);

	CUndoObject();
	~CUndoObject();

	void Serialize(CArchive& ar);
	CString GetUndoText();
public:
	CxObject3DSet	m_cUndoSet;		///< List of objects to undo
	BOOL			m_bCanDelete;	///< TRUE if the objects need to be deleted when cleaning the UndoObject
	int				m_nUndoState;	///< Type of action to undo

	CVector4		m_ptOldPos;		///< Old position of the object (for dragging)
	CObject3DAttr	m_pAttr;		///< Old attributes of the object (name, color, shape) 
};

/////////////////////////////////////////////////////////////////////////////
/// List of undoable/redoable objects
/////////////////////////////////////////////////////////////////////////////
class CxUndoObjectSet : public CList<CUndoObject*,CUndoObject*>
{};

/////////////////////////////////////////////////////////////////////////////
/// Map storing the number of objects in the document, categorized by their type.
/// The key of the map is the object type identifier
/// The value of the map is the number of object of this type
/// @todo add setters/getters for accessing more easily each object counts
/////////////////////////////////////////////////////////////////////////////
class CxObjectCount : public CMap<UINT,UINT,int,int>
{};

/////////////////////////////////////////////////////////////////////////////
/// Implement the functionality for storing, retrieving and managing Calques 3D documents.
///
///
///
///
///
/////////////////////////////////////////////////////////////////////////////
class CCalques3DDoc : public CDocument
{
protected: // create from serialization only
	CCalques3DDoc();
	//DECLARE_DYNCREATE(CCalques3DDoc)
	DECLARE_SERIAL(CCalques3DDoc);

// Attributes
public:

	static CxObject3DSet*	GLOBALObjectSet;

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the current undoable action.
	//////////////////////////////////////////////////////////////////////
	enum TUndo {
			UndoNone,			///< No undoable action
			UndoDelete,			///< Undo an object deletion
			UndoAppend,			///< Undo an object addition
			UndoModify,			///< Undo an object modification (properties)
			UndoMove,			///< Undo an object move
			UndoRedefine		///< Undo an object redefinition
			};

	int				m_nDocUndoState;	///< Current undoable action

	CxUndoObjectSet	m_cNewUndoSet;	///< List of the actions to undo
	CxUndoObjectSet	m_cNewRedoSet;	///< List of the actions to redo

	CxObject3DSet	m_cObjectSet;	///< List of all objects in the universe
	CxObject3DSet	m_cPolygonSet;	///< List of all polygons in the universe
	
	CxObjectCount	m_cObjectSize;	///< The number of objects present in the document

	CxWindowLoc		m_cWinPos;		///< The location of every view used with this document

	CxPOVObjectSet	m_cPOVlist;

protected:
	BOOL	m_bMathPadUsed;			///< The document contains MathPad objects
	BOOL	m_bLegacyVersion;		///< This document is loaded using the old serialization method
// Operations
public:
	BOOL IsTaskAvailable(UINT m_nID);
	BOOL IsMathPadUsed() { return m_bMathPadUsed;};
	void CleanMathPadUsed() {m_bMathPadUsed = FALSE;};

	int GetObjectSize(UINT nID);

	BOOL AddObject(CObject3D* pObj,BOOL bUndo=TRUE);
	BOOL RemoveObject(CObject3D* pObj,BOOL bUndo=TRUE);
	BOOL ModifyPropObject(CObject3D* pObj,CObject3DAttr *pAttr = NULL);
	BOOL MoveObject(CObject3D* pObj);

	void OnPrepareUndo();
	void CleanUndo();
	BOOL OnDoUndo(CView *pView=NULL);
	BOOL OnDoRedo(CView *pView=NULL);
	BOOL OnSetUndo();
	BOOL AddUndoObject(CUndoObject *pUndoObj=NULL);

	void UpdateAllViews(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalques3DDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL
	
	void SerializeLegacy(CArchive& ar);

// Implementation
public:
	void SaveWindowPos();
	void RestoreWindowPos();
	virtual ~CCalques3DDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnCountObjects(CObject3D *,BOOL bAdd);
	void RenumberObjects();
// Generated message map functions
protected:
	//{{AFX_MSG(CCalques3DDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnDiscovery();
	afx_msg void OnViewPlacementSave();
	afx_msg void OnViewPlacementRestore();
	afx_msg void OnUpdatePlacementRestore(CCmdUI* pCmdUI);
	//}}AFX_MSG

	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// Specialization of the Calques 3D document to deal with macro-constructions
///
///
///
/////////////////////////////////////////////////////////////////////////////
class CCalques3DMacroDoc : public CCalques3DDoc
{
public: 
	/// create from serialization only
	CString		strObjectName,			///< Name of the macro
				strObjectHelp,			///< Designation of the macro
				strObjectDef;			///< Definition of the macro
	UINT		nSortKind;				///< @deprecated Not in use anymore

	CCalques3DMacroDoc();
	DECLARE_DYNCREATE(CCalques3DMacroDoc)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalques3DMacroDoc)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CCalques3DMacroDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALQUES3DDOC_H__1B2AE4AC_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
