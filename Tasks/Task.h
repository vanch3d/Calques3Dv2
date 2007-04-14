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
/// @file Task.h
/// @brief interface of the CTask class.
///
/// \todo Need to break down the interfaces into individual files
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_TASK_H__9A6BD920_D2C0_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_TASK_H__9A6BD920_D2C0_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalques3DDoc;
class CViewUniv;
class CDroite3D;
class CSphere3D;
class CPlan3D;
class CCercle3D;
class CSegment3D;
class CMacro3D;
class CCylinder3D;


/////////////////////////////////////////////////////////////////////////////
/// CTask:
/// See Task.cpp for the implementation of this class
///
/////////////////////////////////////////////////////////////////////////////
class CTask
{
public:
    CViewUniv*      m_pParent;          ///< Pointer to the View (Universe or Tracing) associated with the task
    UINT            m_nTaskID;          ///< ID of the task
    int             m_nStep;            ///< Zero-based index of the current step in the task
    BOOL            m_bIsRepereFixed;   ///< TRUE if the referential cannot be rotated, FALSE otherwise
protected:
    CxObject3DSet    m_cObjectFound;     ///< List of geometrical objects found at the last mouse-click
//    CStringArray    m_cStrObjFound;     ///< List of descriptions for objects in the selection list. \deprecated Not in use anymore.
    CObject3D*      m_pObject;          ///< Last selected object from the list
    CPoint          m_ptOld;            ///<
    BOOL            m_bPersistent;      ///< TRUE if the task is persistent, FALSE otherwise

public:
    CTask(CView *pParent,UINT nID);
    virtual ~CTask();

    CCalques3DDoc*  GetDocument();

    /// @name Helpers
    /// These functions are used to retrieve information relative to the current task
    //@{
    virtual unsigned GetTaskResID() const;
    virtual unsigned GetHelpResID();
    virtual DWORD GetMask();
    virtual UINT GetTaskCursor();
    virtual CString GetContextualHelp();
    //@}

    /// @name Display Functions
    /// These functions are used to render the current task into the main view
    //@{
    virtual void CalculFeedBack();
    virtual void DrawFeedBack(CDC *);
    virtual void InvalidateParent(BOOL bForce=FALSE);
    //@}

    /// @name Event Functions
    /// These functions are used to intercept and deal with user and system-generated events
    //@{
    virtual BOOL OnUpdateTasksOption(CCmdUI* pCmdUI);
    virtual BOOL OnDoTasksOption(UINT nID);
    virtual void DoContextMenu(UINT, CPoint&);
	virtual void DoLaunchView(UINT nID);

    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint);
    virtual void OnMouseLDC(UINT, CPoint);
    virtual void OnTimer(UINT) {};
    //@}

    /// @name Object Handling Functions
    /// These functions are used by the task to deal with its outcome
    //@{
    virtual BOOL PrepareAddedObject(CObject3D *);
    virtual void CreateObject3D();
    virtual void CancelTask();
    virtual BOOL ShowErrorMsg(UINT sTextID,UINT sTextSubID=-1,UINT flag = MB_OK | MB_ICONERROR);
    //@}

    /// @name Target Selection Functions
    /// These functions are used by the task to retrieve and select its target objects
    //@{
    virtual int  FindObject(CPoint theLoc,DWORD mask,BOOL bShowLabel=TRUE,BOOL bSub=TRUE);
    virtual CObject3D* GetObject();
    //@}

};

/////////////////////////////////////////////////////////////////////////////
/// CDefTask
///
/////////////////////////////////////////////////////////////////////////////
class CDefTask  : public CTask
{
public:
    BOOL        bMoveRep;
    CPoint      ptRepCenter;

    CDefTask(CView *pParent,UINT nID);
    virtual ~CDefTask();

    virtual void OnMouseLDC(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint);
};

/////////////////////////////////////////////////////////////////////////////
/// CPoint3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CPoint3DTask : public CTask
{
public :
    CPoint      newPt;
    CPoint3D*   m_pObj;

    SProjection oldProj;
    int         nOldVisu;

    CPoint3DTask(CView *AParent,UINT nID);
    ~CPoint3DTask();
    virtual unsigned GetTaskResID() const { return CTX_POINT; }
    virtual unsigned GetHelpResID();

    virtual BOOL OnUpdateTasksOption(CCmdUI* pCmdUI);
    virtual BOOL OnDoTasksOption(UINT nID);

    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint);
    virtual void CreateObject3D();
    virtual void CancelTache() {};
    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CDroite3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CDroite3DTask : public CTask
{
public :
    CPoint3D    *ptA,*ptB;

    CDroite3DTask(CView *AParent,UINT nID);
    ~CDroite3DTask();
    virtual unsigned GetTaskResID() const { return CTX_DROITE; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint) {};
    virtual void CreateObject3D();
    virtual void CancelTache() {};
    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CSegment3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CSegment3DTask : public CDroite3DTask
{
public :

    CSegment3DTask(CView *AParent,UINT nID);
    virtual unsigned GetTaskResID() const;
    virtual void DrawFeedBack(CDC *);

    virtual void CreateObject3D();
};


/////////////////////////////////////////////////////////////////////////////
/// CMoveObjectTask
///
/// \todo Refactory as a child of CPoint3DTask
/////////////////////////////////////////////////////////////////////////////
class CMoveObjectTask : public CTask
{
public :
    CxObject3DSet        m_cParentFound ;
    CObject3D*          m_pObjParent;
    CPoint3D*           m_pObjMov;
    bool                m_bDragPoint;
    BOOL                m_bTracePoint;

    CObject3D*          m_pSelObject;
    CxObject3DSet        m_cRedrawObject;
    CxObject3DSet        m_cTraceObject;

    CMoveObjectTask(CView *AParent,UINT taskID);
    virtual ~CMoveObjectTask();

    virtual unsigned GetTaskResID() const { return CTX_MOVE; }
    virtual unsigned GetHelpResID();

    virtual BOOL OnUpdateTasksOption(CCmdUI* pCmdUI);
    virtual BOOL OnDoTasksOption(UINT nID);

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint);
    virtual void CreateObject3D();
    virtual void CancelTache() {};
    virtual UINT GetTaskCursor();

    virtual int  FindObject(CPoint theLoc,DWORD mask,BOOL bShowLabel=TRUE,BOOL bSub=TRUE);
};

/////////////////////////////////////////////////////////////////////////////
/// CDeleteObjectTask
///
/////////////////////////////////////////////////////////////////////////////
class CDeleteObjectTask : public CTask
{
public :
    CDeleteObjectTask(CView *AParent,UINT taskID);

    virtual unsigned GetTaskResID() const { return CTX_DELETE_OBJECT; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
};

/////////////////////////////////////////////////////////////////////////////
/// CPlan3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CPlan3DTask : public CTask
{
public :
    CPoint3D *ptA,*ptB,*ptC;

    CPlan3DTask(CView *AParent,UINT taskID);
    ~CPlan3DTask();
    virtual unsigned GetTaskResID() const { return CTX_PLAN; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);

    virtual void CreateObject3D();
    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CCercle3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CCercle3DTask : public CPlan3DTask
{
public :
    CCercle3DTask(CView *AParent,UINT taskID);

    virtual void CreateObject3D();
    virtual unsigned GetTaskResID() const { return CTX_CIRCLE; }
};

/////////////////////////////////////////////////////////////////////////////
/// CInter3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CInter3DTask : public CTask
{
public :

    CDroite3D   *dr1,*dr2;
    CPlan3D     *p,*pl2;
    CPoint3D    *p1,*p2,*p3;
    CSphere3D   *sp;
    CSphere3D   *sp2;
    CCylinder3D *cyl;
	CCercle3D	*circle;

    CInter3DTask(CView *AParent,UINT taskID);
    ~CInter3DTask();
    virtual unsigned GetTaskResID() const;
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void CreateObject3D();
};

/////////////////////////////////////////////////////////////////////////////
/// CPointSur3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CPointSur3DTask : public CTask
{
public :
    CPoint3D    *pCurr;
    CDroite3D   *dr1;
    CCercle3D   *cr1;
    CPlan3D     *pl1;
    CSphere3D   *sp1;
    CCylinder3D *cyl;
    CVector4    ptClic;

    CPointSur3DTask(CView *AParent,UINT taskID);
    ~CPointSur3DTask();
    virtual unsigned GetTaskResID() const;
    virtual unsigned GetHelpResID();

    virtual void CreateObject3D();
    virtual DWORD GetMask();

    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CMilieu3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CMilieu3DTask : public CDroite3DTask
{
public :
    CSegment3D *seg;

    CMilieu3DTask(CView *AParent,UINT taskID);
    ~CMilieu3DTask();

    virtual unsigned GetTaskResID() const { return CTX_MILIEU; }
    virtual unsigned GetHelpResID();
    virtual DWORD GetMask();

    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void CreateObject3D();
    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CCenterTask
///
/////////////////////////////////////////////////////////////////////////////
class CCenterTask : public CTask
{
public :

    CCercle3D   *pl;

    CCenterTask(CView *AParent,UINT taskID);
    virtual ~CCenterTask();

    virtual unsigned GetTaskResID() const { return CTX_CENTER_CIRCLE; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void CreateObject3D();
};

/////////////////////////////////////////////////////////////////////////////
/// CParallele3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CParallele3DTask : public CTask
{
public :
    CPoint3D *pt;
    CDroite3D *dr;
    CPlan3D *pl;

    CParallele3DTask(CView *AParent,UINT taskID);
    ~CParallele3DTask();
    virtual unsigned GetTaskResID() const { return CTX_DROITE_PAR; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void CreateObject3D();

    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CPerpendic3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CPerpendic3DTask : public CParallele3DTask
{
public :

    CPerpendic3DTask(CView *AParent,UINT taskID);
    virtual unsigned GetTaskResID() const;
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void CreateObject3D();
    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CCube3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CCube3DTask : public CTask
{
public :
    CPoint3D    *ptA,*ptB,*ptS;
    CCercle3D   *cr;

    CCube3DTask(CView *AParent,UINT taskID);
    ~CCube3DTask();

    virtual unsigned GetTaskResID() const { return CTX_CUBE; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint);
    virtual void CreateObject3D();
    void drawFirstSeg(CDC*,CPoint);
    void drawCircle(CDC*,CPoint);
    void drawFirstSide(CDC*,CPoint);
    virtual void DrawFeedBack(CDC*);
};

/////////////////////////////////////////////////////////////////////////////
/// CSphere3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CSphere3DTask : public CDroite3DTask
{
public :
    CSphere3DTask(CView *AParent,UINT taskID);
    virtual unsigned GetTaskResID() const { return CTX_SPHERE; }

    virtual void CreateObject3D();
};

/////////////////////////////////////////////////////////////////////////////
/// CCylindreTask
///
/////////////////////////////////////////////////////////////////////////////
class CCylindreTask : public CPlan3DTask
{
public :
    CCylindreTask(CView *AParent,UINT taskID);
    virtual unsigned GetTaskResID() const { return CTX_CYLINDER; }

    virtual void CreateObject3D();
    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CExtractCalcTask
///
/////////////////////////////////////////////////////////////////////////////
class CExtractCalcTask : public CTask
{
public :

    //typedef TISetAsVector<TObject3D> TCopiedObj;
    //typedef TISetAsVectorIterator<TObject3D> TCopiedObjIter;
    //TCopiedObj    *CopiedObj;         // Liste des successeurs

    CxObject3DSet    m_cObjExtracted;        // List of extracte objects

    CObject3D   *obj;
    int         nCalcNum;           // extraire dans calques n° ...
    int         nCalcCur;               // n° du calque courant
    BOOL        bAddInCalc;

    CExtractCalcTask(CView *AParent,UINT taskID);
    virtual ~CExtractCalcTask();

    virtual unsigned GetTaskResID() const { return CTX_EXTRACT_TASK; }
    virtual unsigned GetHelpResID();

    virtual BOOL OnUpdateTasksOption(CCmdUI* pCmdUI);
    virtual BOOL OnDoTasksOption(UINT nID);

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint);
    virtual void CreateObject3D();
};

/////////////////////////////////////////////////////////////////////////////
/// CSymetricTask
///
/////////////////////////////////////////////////////////////////////////////
class CSymetricTask : public CTask
{
public :
    CObject3D   *pSrc,
                *pSym;

    CSymetricTask(CView *AParent,UINT taskID);
    virtual ~CSymetricTask();

    virtual unsigned GetTaskResID() const { return CTX_SYMMETRIC; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void CreateObject3D();

    virtual void DrawFeedBack(CDC*);
};

/////////////////////////////////////////////////////////////////////////////
/// CTranslatTask
///
/////////////////////////////////////////////////////////////////////////////
class CTranslatTask : public CPlan3DTask
{
public :
    CTranslatTask(CView *AParent,UINT taskID);
    virtual unsigned GetTaskResID() const { return CTX_TRANSLAT; }

    virtual void CreateObject3D();
    virtual void DrawFeedBack(CDC*);
};

/////////////////////////////////////////////////////////////////////////////
/// CProjectionTask
///
/////////////////////////////////////////////////////////////////////////////
class CProjectionTask : public CTask
{
public :
    CBCGPToolBar m_wndToolBar;

    BOOL    bTwice;
    int     nProjID;
    CPlan3D     *pl;
    CPoint3D    *pt1,*pt2,*pt3;

    CProjectionTask(CView *AParent,UINT taskID);
    virtual ~CProjectionTask();

    virtual unsigned GetTaskResID() const { return CTX_PROJECTION; }
    virtual unsigned GetHelpResID();
    virtual BOOL OnUpdateTasksOption(CCmdUI* pCmdUI);
    virtual BOOL OnDoTasksOption(UINT nID);

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint);
    virtual void OnMouseLDC(UINT, CPoint pt);
    virtual void CreateObject3D();
    void SelectProjection(int id);
    BOOL ProjectFrontal();
    BOOL ProjectProfil();
    BOOL ProjectMiroir();
    BOOL CenterProject();
};

/////////////////////////////////////////////////////////////////////////////
/// CLabelTask
///
/////////////////////////////////////////////////////////////////////////////
class CLabelTask : public CTask
{
public :
    CxObject3DSet    m_cSelObject;
    CEdit           *pEditZone;
    CObject3D       *pSource;
    BOOL            m_bSingleObj;

    CLabelTask(CView *AParent,UINT taskID);
    virtual ~CLabelTask();
    virtual unsigned GetTaskResID() const;
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void CreateObject3D();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
};

/////////////////////////////////////////////////////////////////////////////
/// CMeasureTask
///
/////////////////////////////////////////////////////////////////////////////
class CMeasureTask : public CLabelTask
{
public :
    CxObject3DSet    m_cObjectsTarget;       // List of selected objects
    int             m_nTargets;
    CObject3D       *pEnd;
    CObject3D       *pEnd2;
    UINT            m_nOptionID;                    // ID of the option

    CMeasureTask(CView *AParent,UINT nID);
    ~CMeasureTask();
    virtual void CancelTask();


    virtual unsigned GetTaskResID() const { return CTX_MEASURE; }
    virtual unsigned GetHelpResID();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint point);

    virtual BOOL OnUpdateTasksOption(CCmdUI* pCmdUI);
    virtual BOOL OnDoTasksOption(UINT nID);

    virtual DWORD GetMask();
    virtual void CreateObject3D();
};


/////////////////////////////////////////////////////////////////////////////
/// CCopyShapeTask
///
/////////////////////////////////////////////////////////////////////////////
class CCopyShapeTask : public CTask
{
public :
    enum { TSHAPE_DUP,TSHAPE_HID,TSHAPE_MAR};


    CShape          *pShape;
    //COLORREF      *color;
    DWORD nMaskTarget;
    int             nSubTaskID; // 0 Duplicate 1 hide 2 Mark

    CCopyShapeTask(CView *AParent,UINT taskID);
    virtual ~CCopyShapeTask();
    virtual unsigned GetTaskResID() const { return CTX_COPYSHAPE; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual UINT GetTaskCursor();

    virtual BOOL OnUpdateTasksOption(CCmdUI* pCmdUI);
    virtual BOOL OnDoTasksOption(UINT nID);
};

/////////////////////////////////////////////////////////////////////////////
/// CLocus3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CLocus3DTask : public CDroite3DTask
{
public :

    CLocus3DTask(CView *AParent,UINT nID);
    virtual unsigned GetTaskResID() const { return CTX_LOCUS; }
    virtual unsigned GetHelpResID();
    virtual void DrawFeedBack(CDC *);
    virtual DWORD GetMask();

    virtual void CreateObject3D();
};

/////////////////////////////////////////////////////////////////////////////
/// CGreffe3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CGreffe3DTask : public CTask
{
public :
    CMacro3D        *pMacro;
    BOOL            bLoaded;
    BOOL            bShifted;
    CxObject3DSet    m_cBasePoints;      // List of base points of the macro


    CGreffe3DTask(CView *AParent,UINT nID);
    virtual ~CGreffe3DTask();
    virtual unsigned GetTaskResID() const { return CTX_GREFFE; }
    virtual unsigned GetHelpResID();

    void ShiftTo(CPoint pt);
    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseLDC(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void DrawFeedBack(CDC *);

    void OnLoadMacro(UINT nID);

    virtual void CreateObject3D();
};

/////////////////////////////////////////////////////////////////////////////
/// CRedefine3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CRedefine3DTask : public CTask
{
public :
    CxObject3DSet        m_cTarget;      // List of base points of the macro
    CxSchemeSet*        m_pOptionSet;
    CRedefineScheme*    m_pOption;
    CObject3D*          m_pObj;

    CRedefine3DTask(CView *AParent,UINT nID);
    void CleanUp();
    virtual ~CRedefine3DTask();
    virtual unsigned GetTaskResID() const { return CTX_REDEFINE; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void DrawFeedBack(CDC *);

    virtual BOOL PrepareAddedObject(CObject3D *);
    virtual void CreateObject3D();
};

class CVerifyDlg;

/////////////////////////////////////////////////////////////////////////////
/// CVerify3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CVerify3DTask : public CTask
{
public :
    CxObject3DSet    m_cObjectTarget;        // List of selected objects
    UINT            m_nCurrTask;

    CVerifyDlg*     m_pVerifyDlg;

    CVerify3DTask(CView *AParent,UINT nID);
    virtual ~CVerify3DTask();

    BOOL GetFreePoint(CObject3D* pObj,CxObject3DSet& pSet);

    virtual void CancelTache();

    virtual unsigned GetTaskResID() const { return CTX_VERIFY; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseR(UINT, CPoint);
    virtual BOOL OnUpdateTasksOption(CCmdUI* pCmdUI);
    virtual BOOL OnDoTasksOption(UINT nID);
    virtual void CreateObject3D();

//  virtual BOOL OnVerifyProp();

    virtual BOOL OnVerifyParallel(CObject3D *pObj1,CObject3D *pObj2);
    virtual BOOL OnVerifyPerpendicular();
    virtual BOOL OnVerifyPointOn(CObject3D *pOPt,CObject3D *pObj);
    virtual BOOL OnVerifyAlign(CObject3D *pOPt,CObject3D *pObj2,CObject3D *pObj3);

//  virtual BOOL PrepareAddedObject(CObject3D *);
};

/////////////////////////////////////////////////////////////////////////////
/// CPolygonTask
///
/////////////////////////////////////////////////////////////////////////////
class CPolygonTask : public CTask
{
public :
    CxObject3DSet    m_cObjectsTarget;       // List of selected objects

    CPolygonTask(CView *AParent,UINT nID);
    virtual ~CPolygonTask();

    virtual unsigned GetTaskResID() const { return CTX_POLYGON; }
    virtual unsigned GetHelpResID();

    virtual void CancelTache();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void CreateObject3D();
    virtual void DrawFeedBack(CDC *);
};

/////////////////////////////////////////////////////////////////////////////
/// CAnimation3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CAnimation3DTask : public CTask
{
public :
    UINT                m_nTimer;
    CPoint3D*           m_pObjAnimated;
    FCoord              m_nOldN;
    CxObject3DSet        m_pList;
    int                 m_nDeltaT,
                        m_nCurrD;
    BOOL                m_bInc;
    double              m_nFreq;


    virtual unsigned GetTaskResID() const { return CTX_ANIMATION; }

    CAnimation3DTask(CView *AParent,UINT nID);
    virtual ~CAnimation3DTask();
    virtual void CancelTache();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void OnMouseLUp(UINT, CPoint);
//  virtual void OnMouseR(UINT, CPoint);
    virtual void CreateObject3D();
    virtual void OnTimer(UINT);
    virtual void DrawFeedBack(CDC *);

//  virtual BOOL PrepareAddedObject(CObject3D *);
};

/////////////////////////////////////////////////////////////////////////////
/// CCenterOn3DTask
///
/////////////////////////////////////////////////////////////////////////////
class CCenterOn3DTask : public CTask
{
public :
    CPoint3D    *ptA;

    CCenterOn3DTask(CView *AParent,UINT taskID);
    ~CCenterOn3DTask();

    virtual unsigned GetTaskResID() const { return CTX_CENTERON; }
    virtual unsigned GetHelpResID();

    virtual DWORD GetMask();
    virtual void OnMouseL(UINT, CPoint);
    virtual void OnMouseMove(UINT, CPoint);
    virtual void CreateObject3D();
};


#endif // !defined(AFX_TASK_H__9A6BD920_D2C0_11D4_A2FA_00D0B71C8709__INCLUDED_)
