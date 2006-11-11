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
/// CUndoObject:
///
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
	BOOL			m_bCanDelete;	///< Can delete the objects when clearing the UndoObject
	int				m_nUndoState;	///< Type of action to undo

	CVector4		m_ptOldPos;		///< Old position of the object (for dragging)
	CObject3DAttr	m_pAttr;		///< Old attributes (name, color, shape) of the object
};

/////////////////////////////////////////////////////////////////////////////
/// List of undoable/redoable objects
/////////////////////////////////////////////////////////////////////////////
//typedef CList<CUndoObject*,CUndoObject*>	CxUndoObjectSet;
class CxUndoObjectSet : public CList<CUndoObject*,CUndoObject*>
{};

/////////////////////////////////////////////////////////////////////////////
/// Map storing the number of objects in the document, categorised by their type.
/// The key of the map is the object type identifier
/// The value of the map is the number of object of this type
/// \todo add setters/getters for accessing more easily each object counts
/////////////////////////////////////////////////////////////////////////////
//typedef CMap<UINT,UINT,int,int>	CxObjectCount;
class CxObjectCount : public CMap<UINT,UINT,int,int>
{};

/////////////////////////////////////////////////////////////////////////////
/// List of undoed objects
/////////////////////////////////////////////////////////////////////////////
typedef CMap<UINT ,UINT,CWinPlacement,CWinPlacement> CxWindowLoc;


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
	DECLARE_DYNCREATE(CCalques3DDoc)

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

	int				m_nUndoState;	///< Current undoable action

	CxUndoObjectSet	m_cNewUndoSet;	///< List of the actions to undo
	CxUndoObjectSet	m_cNewRedoSet;	///< List of the actions to redo

	CxObject3DSet	m_cObjectSet;	///< List of all objects in the universe
	CxObject3DSet	m_cPolygonSet;	///< List of all polygons in the universe
	
	CxObjectCount	m_cObjectSize;	///< The number of objects present in the document

	CxWindowLoc		m_cWinPos;		///< The location of every view used with this document

// Operations
public:
	BOOL IsTaskAvailable(UINT m_nID);

	int GetObjectSize(UINT nID);

	BOOL AddObject(CObject3D* pObj,BOOL bUndo=TRUE);
	BOOL RemoveObject(CObject3D* pObj,BOOL bUndo=TRUE);
	BOOL ModifyPropObject(CObject3D* pObj,CObject3DAttr *pAttr = NULL);
	BOOL MoveObject(CObject3D* pObj);

	void OnPrepareUndo();
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
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnDiscovery();
	//}}AFX_MSG

	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// Specialisation of the Calques 3D document to deal with macro-constructions
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
