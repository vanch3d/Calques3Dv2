// Text3D.h: interface for the CText3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT3D_H__98199100_EE25_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_TEXT3D_H__98199100_EE25_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CText3D : public CObject3D
{
public:
    DECLARE_SERIAL(CText3D);
    CRect       rActZone;       // Bounding Rect;
    CFont       mTextFont;
    COLORREF    mColorText;
    CString     mStrText;
    UINT        dwFormat;

    CText3D();
    CText3D(const CObject3D & );
    virtual ~CText3D();

    virtual CObject3D* CopyObject();
    virtual void Serialize( CArchive& ar );

    virtual DWORD isA() const { return TLabel3DClass; }

    virtual UINT GetNameID() const { return IDS_NAME_TEXT;};
    virtual UINT GetHelpID() const { return IDS_HELP_TEXT;};
    virtual UINT GetDefID() const { return IDS_DEF_TEXT;};

	virtual BOOL IsMultiLine() { return FALSE;};

    virtual BOOL IsInActiveArea(CPoint);
    virtual CRgn* InvalideRect();
    virtual int SetProperties(CxObject3DSet *pSet=NULL);
    virtual CString GetText();
    virtual void SetText(CString txt);

    virtual void SetFont(LOGFONT *pLogFont);

    virtual UINT  CalculConceptuel();
    virtual void CalculVisuel(CVisualParam *);
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
    virtual HTREEITEM DrawHistorique(CTreeCtrl& mListCtrl,HTREEITEM pParent=TVI_ROOT);
    virtual void DrawDepGraph(CDC* pDC,CImageList *pImgList=NULL,int nTrace=GRAPH_NONE,BOOL bDrawNode=TRUE,BOOL bDrawLink=TRUE);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CLabel3D : public CText3D
{
public:
    CSize           mDeltaPos;
    CObject3D       *pSource;
    DECLARE_SERIAL(CLabel3D);

    CLabel3D();
    CLabel3D(CObject3D* pObj);
    CLabel3D(const CObject3D & );
    virtual ~CLabel3D();

    virtual DWORD isA() const { return TLabel3DClass; }

    virtual CObject3D* CopyObject();
    virtual CxObject3DSet* GetParents();
    virtual void Serialize( CArchive& ar );
    virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

    virtual CString GetText();

    virtual UINT  CalculConceptuel();
    virtual void CalculVisuel(CVisualParam *);
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CEquation3D : public CText3D
{
public:
    CObject3D       *pSource;
    CVector4        m_nValue;
    BOOL            m_bVal;
    //CxObject3DSet  m_cSources;

    DECLARE_SERIAL(CEquation3D);
    CEquation3D();
    CEquation3D(CObject3D* pObj);
    CEquation3D(const CObject3D & );
    virtual ~CEquation3D();

    virtual DWORD isA() const { return TEquation3DClass; }
    virtual CString GetText();

    virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

    CString GetPointText(CObject3D *pObj);
    CString GetLineText(CObject3D *pObj);
    CString GetPlaneText(CObject3D *pObj);
    CString GetSphereText(CObject3D *pObj);

    virtual CxObject3DSet* GetParents();
    virtual void Serialize( CArchive& ar );

    virtual void    SetSelected(BOOL bSel=TRUE);

    virtual int SetProperties(CxObject3DSet *pSet=NULL);

    virtual void CalculVisuel(CVisualParam *);
    virtual UINT  CalculConceptuel();
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
    virtual void DrawMathPad(CDC* pDC);
    virtual HTREEITEM DrawHistorique(CTreeCtrl& mListCtrl,HTREEITEM pParent=TVI_ROOT);


    static FCoord   Round(FCoord nVal);
    static FCoord   RoundNorm(FCoord nVal);
    static CString  FormatNumber(FCoord mValue, BOOL bValide=TRUE,BOOL bSign=FALSE);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CValue3D : public CEquation3D
{
public:
    FCoord  m_nValue;

    DECLARE_SERIAL(CValue3D);
    CValue3D();
    CValue3D(const CObject3D & );

    virtual UINT GetHelpID() const { return IDS_HELP_VALUE;};
    virtual UINT GetDefID() const { return IDS_DEF_VALUE;};

    virtual DWORD isA() const { return TValue3DClass; }
    virtual void Serialize( CArchive& ar );
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDistance3D : public CValue3D
{
public:
    DECLARE_SERIAL(CDistance3D);
    CObject3D       *pSource2;

    CDistance3D();
    CDistance3D(CObject3D* P1,CObject3D* P2);
    CDistance3D(CObject3D* P1);
    CDistance3D(const CObject3D & );
    virtual ~CDistance3D();

    virtual UINT GetDefID() const { return IDS_DEF_DISTANCE;};
    virtual DWORD isA() const { return TDistance3DClass; }
    virtual CString GetText();

    virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);
    virtual void    SetSelected(BOOL bSel=TRUE);

    virtual CxObject3DSet* GetParents();
    virtual void Serialize( CArchive& ar );

    virtual void CalculVisuel(CVisualParam *);
    virtual UINT  CalculConceptuel();
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
    virtual void DrawMathPad(CDC* pDC);
};

class CPoint3D;
class CPlan3D;
class CDroite3D;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CAngle3D : public CValue3D
{
public:
    DECLARE_SERIAL(CAngle3D);
    CxObject3DSet    m_pSource;
    FCoord          m_fAngle;

    CAngle3D();
    CAngle3D(CPoint3D* P1,CPoint3D* P2,CPoint3D* P3);
    CAngle3D(CPlan3D* Pl1,CPlan3D* Pl2);
    CAngle3D(CPlan3D* Pl,CDroite3D* Dr);
    CAngle3D(CDroite3D* Dr1,CDroite3D* Dr2);
    CAngle3D(const CObject3D & );
    virtual ~CAngle3D();

    virtual UINT GetDefID() const { return IDS_DEF_ANGLE;};
    virtual DWORD isA() const { return TAngle3DClass; }
    virtual CString GetText();

    virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);
    virtual void    SetSelected(BOOL bSel=TRUE);

    virtual CxObject3DSet* GetParents();
    virtual void Serialize( CArchive& ar );

    virtual void CalculVisuel(CVisualParam *);
    virtual UINT  CalculConceptuel();
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
    virtual void DrawMathPad(CDC* pDC);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CVolume3D : public CAngle3D
{
public:
    DECLARE_SERIAL(CVolume3D);

    CVolume3D();
    CVolume3D(CObject3D* P1);
    CVolume3D(const CObject3D & );

    virtual UINT GetDefID() const { return IDS_DEF_VOLUME;};
    virtual DWORD isA() const { return TVolume3DClass; }
    virtual CString GetText();
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
    virtual void DrawMathPad(CDC* pDC);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CArea3D : public CAngle3D
{
public:
    DECLARE_SERIAL(CArea3D);

    CArea3D();
    CArea3D(CObject3D* P1);
    CArea3D(const CObject3D & );

    virtual UINT GetDefID() const { return IDS_DEF_AREA;};
    virtual DWORD isA() const { return TArea3DClass; }
    virtual CString GetText();
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
    virtual void DrawMathPad(CDC* pDC);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CComment3D : public CEquation3D
{
public:
    CString m_strComment;

    DECLARE_SERIAL(CComment3D);
    CComment3D();
    CComment3D(CString mstr);
    CComment3D(const CObject3D & );
    virtual ~CComment3D();

    virtual DWORD isA() const { return TCComment3DClass; }
    virtual CString GetText();
	virtual BOOL IsMultiLine() { return TRUE;};

    virtual void Serialize( CArchive& ar );
    virtual int SetProperties(CxObject3DSet *pSet=NULL);

    virtual void CalculVisuel(CVisualParam *);
    virtual UINT  CalculConceptuel();
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
    virtual void DrawMathPad(CDC* pDC);
};

class CMathParser;
class CStrMap;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CMathOp3D : public CAngle3D
{
public:
    CxObject3DSet    m_pSource;
    CMathParser*    m_pMathParser;
    CStrMap*        m_pMathVar;
    double*         m_pMathValue;
    CString         m_strComment;


    DECLARE_SERIAL(CMathOp3D);

    ~CMathOp3D();
    CMathOp3D();
    CMathOp3D(CString mstr);
    CMathOp3D(const CObject3D & );

    virtual UINT GetHelpID() const { return IDS_HELP_VALUE;};
    virtual UINT GetDefID() const { return IDS_DEF_VALUE;};

    virtual CxObject3DSet* GetParents();

    void    OnUpdateVariables(CxObject3DSet *pSet=NULL);


    virtual DWORD isA() const { return TMathOp3DClass; }
    virtual CString GetText();
    virtual void SetText(CString txt);
    virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
    virtual void DrawMathPad(CDC* pDC);
    virtual int SetProperties(CxObject3DSet *pSet=NULL);

    virtual void Serialize( CArchive& ar );
};



#endif // !defined(AFX_TEXT3D_H__98199100_EE25_11D4_A2FA_00D0B71C8709__INCLUDED_)
