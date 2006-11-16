// Object3D.cpp: implementation of the CObject3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "..\Calques3DDoc.h"

#include "..\Prefs\Prefs.h"

#include "Vector4.h"
#include "Object3D.h"

#include "Locus3D.h"

#include "ObjectPropPage.h"
#include "..\HelpPrSheet.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern CxObject3DSet*   GLOBALObjectSet;

//----------------------------------------------------------------------
// CRedefineScheme
//----------------------------------------------------------------------

IMPLEMENT_SERIAL(CRedefineScheme, CObject, VERSIONABLE_SCHEMA | 1)

////////////////////////////////////////////////////////////////////////
/// Default constructor for the redefinition scheme
////////////////////////////////////////////////////////////////////////
CRedefineScheme::CRedefineScheme()
{
    pNewObj = NULL;
    nObjResID = -1;
    nAvail = -1;
    bRelax = FALSE;
    for (int i=0;i<10;i++)
    {
        nStepID[i] = -1;
        nTaskResID[i] = -1;
    }
    nRedefStep = -1;

}

////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////
CRedefineScheme::~CRedefineScheme()
{
    if (pNewObj) delete pNewObj;
}


//----------------------------------------------------------------------
// CObject3D
//----------------------------------------------------------------------

IMPLEMENT_SERIAL(CObject3D, CObject, VERSIONABLE_SCHEMA | 1)


//----------------------------------------------------------------------
// Construction/Destruction
//----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
/// Default constructor.
//////////////////////////////////////////////////////////////////////
CObject3D::CObject3D()
{
//  pParent = NULL;
    bShowTrace = FALSE;
    pHistItem = NULL;
    rGraphRect=CRect(-1,-1,-1,-1);
    pComposite = NULL;
    bValidate = bVisible = TRUE;
    bIsSelected = bMarked = FALSE;
    bNotInHisto = FALSE;

    strObjectName = strObjectDef = strObjectHelp = _T("");
    //cDependList.SetSize(10,10);

    nDepth = 0;
    nSortKind = 1;
    //nZOrder = 0.0;
    nCalque = 1;

    //pObjectShape; = new CShape(this);
    bDrawInGraph = TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Copy constructor: build an object by duplicating the attributes of the target object.
/// \param mObj A reference to the object to copy
//////////////////////////////////////////////////////////////////////
CObject3D::CObject3D(const CObject3D& mObj)
{
//  pParent = mObj.pParent;
    pHistItem = mObj.pHistItem;
    rGraphRect = mObj.rGraphRect;
    pComposite = mObj.pComposite;
    bValidate = mObj.bValidate;
    bVisible = mObj.bVisible;
    bIsSelected = mObj.bIsSelected;
    bMarked = mObj.bMarked;
    bDrawInGraph = TRUE;
    bNotInHisto = FALSE;
    bShowTrace = FALSE;
    strObjectName = mObj.strObjectName;
    strObjectDef = mObj.strObjectDef;
    strObjectHelp = mObj.strObjectHelp ;

    cDependList.RemoveAll();
    for (int i=0;i<mObj.cDependList.GetSize();i++)
    {
        CObject3D *pObj = mObj.cDependList.GetAt(i);
        if (pObj) cDependList.Add(pObj);
    }

    nDepth = mObj.nDepth;
    nSortKind = mObj.nSortKind ;
    //nZOrder = mObj.nZOrder;
    nCalque = mObj.nCalque ;
    nObjectId = mObj.nObjectId;

    pComposite = mObj.pComposite;

    pObjectShape = mObj.pObjectShape;
}

//////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////
CObject3D::~CObject3D()
{
    //if (pObjectShape) delete pObjectShape;
}

//////////////////////////////////////////////////////////////////////
/// Create a clone of this object
///
/// \return     A pointer to the clone of this object.
//////////////////////////////////////////////////////////////////////
CObject3D* CObject3D::CopyObject()
{
    CObject3D *pObj = new CObject3D(*this);
    return pObj;
}

//----------------------------------------------------------------------
// Serialisation
//----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
/// Store and load the object into and from an archive.
///
/// \param ar A CArchive object to serialize to or from.
//////////////////////////////////////////////////////////////////////
void CObject3D::Serialize( CArchive& ar )
{
    CObject::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << bValidate;
        ar << bVisible;
        ar << bMarked;
        ar << bIsSelected;
        ar << bNotInHisto;
        ar << nObjectId;
        ar << strObjectName;
        // ar << strObjectHelp;
        // ar << strObjectDef;
        // ar << nSortKind;
        ar << nDepth;
        ar << nCalque;
        ar << pComposite;
        ar << rGraphRect;               // Rect in the Graph Window
    }
    else
    {
        ar >> bValidate;
        ar >> bVisible;
        ar >> bMarked;
        ar >> bIsSelected;
        ar >> bNotInHisto;
        ar >> nObjectId;
        ar >> strObjectName;
        // ar >> strObjectHelp;
        // ar >> strObjectDef;
        // ar >> nSortKind;
        ar >> nDepth;
        ar >> nCalque;
        ar >> pComposite;
        ar >> rGraphRect;

        bIsSelected = FALSE;
        bShowTrace = FALSE;
        //bNotInHisto = FALSE;
		CCalques3DDoc::GLOBALObjectSet->Add(this);
    }
    pObjectShape.Serialize(ar);
}


//////////////////////////////////////////////////////////////////////
/// Called to serialize a parent, in the dependence graph, of the object.
///
/// Storing a parent is simply done by serializing its index in the dependence graph.
/// Loading the parent consist in retrieving the object by its index from the global storage (GLOBALObjectSet)
/// build step-by-step when the construction is loaded. See CCalques3DDoc::Serialize for more
/// information about this procedure,
///
/// \param ar   A CArchive object to serialize to or from.
/// \return     A pointer to the stored/retrieved parent.
//////////////////////////////////////////////////////////////////////
CObject3D *CObject3D::SerializeObj(CArchive& ar)
{
    CObject3D *pSObj = NULL;
    if (ar.IsStoring()) return pSObj;

    int nb =CCalques3DDoc::GLOBALObjectSet->GetSize();
    int nObj;
    ar >> nObj;
    if (nObj != -1)
        pSObj = CCalques3DDoc::GLOBALObjectSet->GetAt(nObj-1);
    return pSObj;
}

//----------------------------------------------------------------------
// Identification
//----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
/// Get the resource ID of the string containing the default name of the object.
//////////////////////////////////////////////////////////////////////
UINT CObject3D::GetNameID() const
{
    return IDS_NAME_OBJECT;
}

//////////////////////////////////////////////////////////////////////
/// Get the resource ID of the string containing the designation of the object.
//////////////////////////////////////////////////////////////////////
UINT CObject3D::GetHelpID() const
{
    return IDS_HELP_OBJECT;
}

//////////////////////////////////////////////////////////////////////
/// Get the resource ID of the string containing the definition of the object.
//////////////////////////////////////////////////////////////////////
UINT CObject3D::GetDefID() const
{
    return IDS_DEF_OBJECT;
}

//////////////////////////////////////////////////////////////////////
/// Get the definition of the object.
///
/// The definition of an object is a string containing a complete description of the object,
/// including references to the objects to which it depends for its construction.
/// Names of the objects and its parents are automatically updated.
/// For example, a segment will be defined by "S is a segment line going through A and B".
/// This definition is mainly used to populate the History view (see CViewHisto) and the status bar
/// when an object is designated (see CViewUniv::OnUpdateObjTooltip).
//////////////////////////////////////////////////////////////////////
CString CObject3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());
    mstr.Format(sFormat,sName);
    return mstr;
}

//////////////////////////////////////////////////////////////////////
/// Get the designation of the object.
///
/// The designation of an object is a short string containing its name and its type. For example,
/// a segment will be designated by "The segment S".
/// The designation is mainly used in the tooltips appearing in the views when the user designates
/// the object (see CViewUniv::OnUpdateObjTooltip).
//////////////////////////////////////////////////////////////////////
CString CObject3D::GetObjectHelp()
{
    CString mstr,mstr2;
    mstr.LoadString(GetHelpID());
    mstr2.Format(mstr,GetObjectName());
    return mstr2;
}

//////////////////////////////////////////////////////////////////////
/// Get the name of the object.
///
/// The method returns a string containing either the name as edited by the user (and stored in #strObjectName)
/// or, if empty, the default name as retrieved by GetNameID,
//////////////////////////////////////////////////////////////////////
CString CObject3D::GetObjectName()
{
    if (strObjectName != _T(""))
        return strObjectName;

    CString mstr,mstr2;
    mstr.LoadString(GetNameID());
    mstr2.Format("%s%d",mstr,nObjectId);
    return mstr2;
}

//////////////////////////////////////////////////////////////////////
/// Get the identifier representing the type of the object.
/// See TObject3DClass and others identifiers
//////////////////////////////////////////////////////////////////////
unsigned long CObject3D::isA() const
{
    return TObject3DClass;
}

//////////////////////////////////////////////////////////////////////
/// Test if this object is filtered by the given combination of identifiers.
///
/// This methods, coupled with the bitwise definition of the object identifiers (see isA), allows
/// a fast selection of particular object types in a list.
///
/// All identifiers have a common form : MAKELONG(X,Y), where
/// - X (low-order) is the category of the object (i.e. point, line, sphere, etc.)
/// - Y (high-order) is the particular type of the object (i.e. point on a line, etc.)
/// - Y=1 if the identifier related to the generic type (ie all points)
/// - X=1 and Y=1 for the generic object (TObject3DClass)
///
/// The mask is a combination of object identifiers describing all the possible objects that could be recognised.
/// For easier definition, some of the generic combinations have been hard-coded in the identifiers (see
/// TAllPointClass for retrieving all points, TAllDroiteClass for all lines, etc.).
/// This method combine bitwise both the mask and the object identifier in order to verify if this
/// object is recognised by the mask.
///
/// \param mask A bitwise combination of object identifiers
/// \return     TRUE if this object fits the mask, FALSE otherwise.
//////////////////////////////////////////////////////////////////////
BOOL CObject3D::MaskObject(unsigned long mask)
{
    unsigned long objID = isA();
    unsigned int hiMask = HIWORD(mask);
    unsigned int loMask = LOWORD(mask);
    unsigned int hiID = HIWORD(objID);
    unsigned int loID = LOWORD(objID);

    if (loMask==1) return TRUE;
    if (hiMask==1) return (loMask & loID);
    BOOL ret =  (loMask & loID) &&  (hiMask & hiID);
    return ret;
}


//----------------------------------------------------------------------
// Dependence Graph
//----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
/// Update the depth of this object in the dependence graph.
/// Basically, the depth of an object is either 0 if it has no parents or the
/// maximum of its parents' depth +1.
///
/// \return     The current depth of this object
//////////////////////////////////////////////////////////////////////
int CObject3D::SetDepth()
{
    return nDepth=0;
}

//////////////////////////////////////////////////////////////////////
/// Get all the parents of this object from the dependence graph.
///
/// \return     A pointer to a list containing the parents, NULL if no parents.
//////////////////////////////////////////////////////////////////////
CxObject3DSet* CObject3D::GetParents()
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////
/// Get the list of all or immediate dependents of this object.
///
/// Note that the list contains no duplicate object and is sorted by the object's depth
/// (see AddObjToDependList).
///
/// \param pList    A pointer to the object list to fill in.
/// \param bAll     TRUE if exploring the complete graph, FALSE to explore only the direct children.
//////////////////////////////////////////////////////////////////////
void CObject3D::GetDependList(CxObject3DSet* pList,BOOL bAll)
{
    if (!pList) return;
    TRACE("CObject3D::GetDependList : %s\n",GetObjectDef());

    BOOL bAdded = AddObjToDependList(pList);
    if (!bAdded) return;

    int nb = cDependList.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = cDependList.GetAt(i);
        if (!pObj) continue;
        //if (pObj->pComposite) continue;
        TRACE("\t\tcDependList : %s\n",pObj->GetObjectDef());
        if (!bAll || bAll==2)
            pObj->AddObjToDependList(pList);
        else
            pObj->GetDependList(pList);
    }
}

//////////////////////////////////////////////////////////////////////
/// Add this object into the list of ordered, non-duplicate object
///
/// The object is added to the list only if not yet included and at a position
/// sorted by increasing depth.
///
/// \param pList    A pointer to the object list to fill in.
/// \return         TRUE if the object has been added, FALSE otherwise
//////////////////////////////////////////////////////////////////////
BOOL CObject3D::AddObjToDependList(CxObject3DSet* pList)
{
    BOOL bRet = FALSE;
    if (!pList) return bRet;

    BOOL bAdded = FALSE;
    BOOL bFound = FALSE;
    int nb1 = pList->GetSize();
    for (int i=0;i<nb1;i++)
    {
        CObject3D *pObj = pList->GetAt(i);
        if (!pObj) continue;
        if (pObj == this)
        {
            bFound = TRUE;
            break;
        }
        if (nDepth < pObj->nDepth)
        {
            pList->InsertAt(i,this);
            bAdded =TRUE;
            bRet = TRUE;
            break;
        }
    }
    if (!bAdded && !bFound)
    {
        pList->Add(this);
        bRet = TRUE;
    }
    return bRet;
}

//////////////////////////////////////////////////////////////////////
/// Insert (or remove) this object into (or from) the dependence graph.
///
/// The method maintains this object in the dependence graph resulting from its introduction
/// in a construction. This method checks for the parents (if any) of this object and
/// add it in their dependents list (or remove it from).
///
/// @param bAdd     TRUE for adding the object, FALSE for removing it.
//////////////////////////////////////////////////////////////////////
void CObject3D::SetInGraph(BOOL bAdd)
{
    CxObject3DSet *pList = GetParents();
    if (!pList) return;
    int nb = pList->GetSize();

    for (int i=0;i<nb;i++)
    {
        CObject3D* pPar = pList->GetAt(i);
        if (!pPar) continue;
        int nbD = pPar->cDependList.GetSize();
        BOOL bFound = FALSE;
        for (int j=0;j<nbD && !bFound;j++)
        {
            CObject3D *pDep = pPar->cDependList.GetAt(j);
            if (!pDep) continue;
            if (pDep == this)
            {
                bFound = TRUE;
                break;
            }
        }
        if (bAdd && !bFound)
            pPar->cDependList.Add(this);
        else if (!bAdd && bFound)
            pPar->cDependList.RemoveAt(j);
    }
    delete pList;
}

//////////////////////////////////////////////////////////////////////
/// Change one of the parents of the object
/// @param pOld		A pointer to the oarent object to change.
/// @param pNew		A pointer to the object to replace the current parent with.
/// @param bUpGraph	TRUE if the dependence graph needs to be updated, FALSE otherwise
/// @return TRUE is the parent has been properly changed, FALSE otherwise
//////////////////////////////////////////////////////////////////////
BOOL CObject3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
/// Set the parents of the object
//////////////////////////////////////////////////////////////////////
BOOL CObject3D::SetParents(CxObject3DSet* pSet) 
{ 
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
/// Graft this object to the designated target
//////////////////////////////////////////////////////////////////////
BOOL CObject3D::GraftOn(CObject3D * pNew)
{
    return FALSE;
}

//----------------------------------------------------------------------
// Attribute/Information Functions
//----------------------------------------------------------------------


CObject3DAttr CObject3D::GetAttributes()
{
    CObject3DAttr pAttr;

    pAttr.m_bMarked = bMarked;
    pAttr.m_bVisible = bVisible;
    pAttr.m_nCalque = nCalque;
    pAttr.m_pObjectShape = pObjectShape;
    pAttr.m_strObjectName = GetObjectName();

    return pAttr;

}

COLORREF CObject3D::GetDefaultColor()
{
    return TPref::Color;
}


void CObject3D::SetAttributes(CObject3DAttr pAttr)
{
    SetVisible(pAttr.m_bVisible);
    bMarked = pAttr.m_bMarked;
    nCalque = pAttr.m_nCalque;
    pObjectShape = pAttr.m_pObjectShape;

    SetStyle(pObjectShape.nShapeId);
    SetColor(pObjectShape.clrObject);
    SetName(pAttr.m_strObjectName);
}








BOOL CObject3D::operator == ( const CObject3D &other ) const
{
    return TRUE;
}

BOOL CObject3D::operator < ( const CObject3D &other ) const
{
    return TRUE;
}

BOOL CObject3D::IsEqual(CObject3D &other)
{
    return FALSE;
}

UINT CObject3D::IsParallelTo(CObject3D *pObj)
{
    return VER_ERROR;
}

UINT CObject3D::IsAlignedWith(CObject3D *pObj1,CObject3D *pObj2)
{
    return VER_ERROR;
}


UINT CObject3D::IsPerpendicularTo(CObject3D *pObj)
{
    return VER_ERROR;
}

void CObject3D::TranslateBy(CVector4 ptVec)
{
}




CObject3D* CObject3D::HitTest(CPoint pt,UINT mask,int nCalcNum,BOOL bSub,CxObject3DSet* pSet)
{
    CObject3D *pObj=NULL;
    if (bValidate && bVisible && IsInCalque(nCalcNum) &&
        IsInActiveArea(pt)  && MaskObject(mask))
    {
        pObj = this;
        if (pSet) // && (bSub ||(!bSub && !pComposite )))
            pSet->Add(this);
    }
    return pObj;
}




BOOL CObject3D::IsInCalque(int CalcNum)
{
    BOOL bTest = (nCalque & (int)pow(2,CalcNum));
    return bTest;
}

BOOL CObject3D::AddInCalque(int CalcNum,BOOL add)
{
    if (add)    // ajoute dans le calque CalcNum
        nCalque |= (int)pow(2,CalcNum);
    else        // retire du calque CalcNum
        nCalque &= ~((int)pow(2,CalcNum));
    return TRUE;
}

CRgn* CObject3D::InvalideRect()
{
    return NULL;
}

void CObject3D::TestAcces()
{
    MessageBeep(MB_ICONEXCLAMATION);
}

int CObject3D::SetProperties(CxObject3DSet *pSet)
{
    CString mstr = GetObjectName();

    //CHelpPrSheet pSheet("Object Properties");
    CHelpPrSheet pSheet(AFX_IDS_OBJPROP);
    CObjectPropPage pPage;
    CObjectInfoPage pPage2;
    CObjectDepPage pPage3;

    ////// Set Object Property Page
    CLocus3D *pLoc = DYNAMIC_DOWNCAST(CLocus3D,this);

    pPage.m_bIsLocus = (BOOL)(pLoc);
    if (pLoc)
    {
        pPage.m_nLocus = pLoc->nDeltaT;
    }

    pPage.m_bHidden = (!bVisible);
    pPage.m_bMarked = bMarked;
    //pPage.m_nShape = pObjectShape.nShapeId;

	pPage.m_cShape.SetShape((int)pObjectShape.GetShapeType(),pObjectShape.nShapeId);

//    pPage.m_cShapePick.SetType((int)pObjectShape.GetShapeType());
 //   pPage.m_cShapePick.SetShape(pObjectShape.nShapeId);
 //   pPage.m_cStrShape.RemoveAll();
 //   for (int i=0;i<pObjectShape.GetShapeSize();i++)
 //   {
  //      CString mstr = pObjectShape.GetShapeDef(i);
   //     pPage.m_cStrShape.Add(mstr);
   // }
    CString mstr2;
    mstr2.LoadString(GetNameID());
    pPage.m_strDefName.Format("%s%d",mstr2,nObjectId);

    pPage.m_strName = GetObjectName();
    pPage.m_clrObj = pObjectShape.clrObject;
    pPage.m_clrDefObj = GetDefaultColor();//pObjectShape.clrObject;
    //pPage.m_strDepth.Format(_T("%d"),nDepth);

    ////// Set Object Information Page
    pPage2.m_strObjDef = GetObjectDef();
    pPage2.m_strObjHelp = GetObjectHelp();

    ////// Set Object Dependencies Page
    pPage3.m_pList = &cDependList;
    CxObject3DSet pMyList;
    CxObject3DSet pMyList2;
    GetDependList(&pMyList,FALSE);
    for (int jj=0;jj<pMyList.GetSize();jj++)
    {
        CObject3D *pObj = pMyList.GetAt(jj);
        if (!pObj) continue;
        if (pObj == this) continue;

        if (!pObj->pComposite)
            pMyList2.Add(pObj);
    }
    pPage3.m_pList = &pMyList2;

    pSheet.AddPage(&pPage);
    pSheet.AddPage(&pPage2);
    pSheet.AddPage(&pPage3);
    int nRet = pSheet.DoModal();
    if (nRet == IDOK)
    {
        bVisible = !(pPage.m_bHidden);
        bMarked = pPage.m_bMarked;
        SetColor(pPage.m_clrObj);
        //int nn = pPage.m_cShapePick.GetShape();
		int nn = pPage.m_cShape.GetShape();
        SetStyle(nn);
        //SetStyle(pPage.m_nShape);
        SetName(pPage.m_strName);

        //int nn = pPage.m_cShapePick.GetShape();
        if (pLoc)
        {
            pLoc->nDeltaT = pPage.m_nLocus;
            pLoc->CalculConceptuel();
        }
    }

    return (nRet == IDOK);
}

int CObject3D::SetObjectID(int nID)
{
    nObjectId = nID;
    return nID;
}

void CObject3D::SetName(CString mstr)
{
    strObjectName = mstr;
}

void CObject3D::SetColor(COLORREF rColor)
{
    pObjectShape.clrObject = rColor;
}

void CObject3D::SetStyle(int nStyle)
{
    pObjectShape.nShapeId = nStyle;
}

void CObject3D::SetAvailHisto()
{
    bNotInHisto = FALSE;
    if (!bVisible) bNotInHisto = TRUE;
}



void CObject3D::HandleObjectError(int errNo,BOOL bShow)
{
    bValidate = (errNo ==0);
    if (!bShow) return;

    CString mstr;
    mstr.LoadString(errNo);
    AfxGetMainWnd()->MessageBox(mstr,NULL,MB_OK | MB_ICONERROR);
    //Geom3DApp->ShowErrorMsg(errNo);
}

CString CObject3D::DrawSymbolic()
{
    CString mstr;
    //mstr.Format(_T("****"));
    mstr.Empty();
    return mstr;
}

void CObject3D::GetRange(CVector4 &min,CVector4 &max)
{
    CVector4 pt(0,0,0,0);
    min = pt;
    max = pt;
}



//////////////////////////////////////////////////////////////////////
/// Method CObject3D::DrawHistorique
///
/// \param %PARAMNAME% (%PARAMTYPE%)    todo
/// \return HTREEITEM
//////////////////////////////////////////////////////////////////////
HTREEITEM CObject3D::DrawHistorique(CTreeCtrl& mListCtrl,HTREEITEM pParent)
{
    HTREEITEM pHItem = NULL;
    if (!mListCtrl) return NULL;

    if (bNotInHisto) return NULL;

    CString mstr = GetObjectDef();
    if (pHistItem)
    {
        int nImg = (bValidate) ? ((IsVisible()) ? 0 : 2) : 1;
        int nSel = nImg;
        int oldImg = nImg,
            oldSel = nSel;
        CString oldstr=mstr;

        mListCtrl.GetItemImage(pHistItem,oldImg,oldSel);
        if ((oldImg != nImg) && (oldSel != nSel))
            mListCtrl.SetItemImage(pHistItem,nImg,nSel);

        oldstr = mListCtrl.GetItemText(pHistItem);
        if (mstr != oldstr)
            mListCtrl.SetItemText(pHistItem,mstr);
        pHItem = pHistItem;
    }
    else
    {
        TV_INSERTSTRUCT     curTreeItem;
        curTreeItem.hParent = pParent;
        curTreeItem.hInsertAfter = TVI_LAST;
        curTreeItem.item.mask = TVIF_TEXT | TVIF_PARAM;
        curTreeItem.item.pszText = mstr.GetBuffer(mstr.GetLength());
        curTreeItem.item.lParam = (LPARAM)this;
        if (!bVisible || !bValidate)
        {
            curTreeItem.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE ;
            curTreeItem.item.iImage = (bValidate) ? ((IsVisible()) ? 0 : 2) : 1;
            curTreeItem.item.iSelectedImage = (bValidate) ? ((IsVisible()) ? 0 : 2) : 1;
            //curTreeItem.item.iSelectedImage = curTreeItem.item.iImage + 1;
        }
        pHItem = mListCtrl.InsertItem(&curTreeItem);
        if (pHItem)
            pHistItem = pHItem;

    }
    return pHItem;
}

//////////////////////////////////////////////////////////////////////
/// Method CObject3D::DrawDepGraphNode
///
/// \param %PARAMNAME% (%PARAMTYPE%)    todo
/// \return void
//////////////////////////////////////////////////////////////////////
void CObject3D::DrawDepGraphNode(CDC* pDC,CImageList *pImgList/*=NULL*/,
                                 CObject3D *pSrc/*=NULL*/,int nTrace/*=DIS_GRAPH_NONE*/)
{
}

//////////////////////////////////////////////////////////////////////
/// Method CObject3D::DrawGraphArrow
///
/// \param %PARAMNAME% (%PARAMTYPE%)    todo
/// \return void
//////////////////////////////////////////////////////////////////////
void CObject3D::DrawGraphArrow(CDC *pDC,CPoint ptStart, CPoint ptEnd, int nArrowSize ,int nArrowlength,CBrush* pbrush)
{
    CPoint points[4];
    //CRgn rgn;

    int dx=0,dy=0,dx1,dy1;
    CRect rect(ptStart,ptEnd);
        //rect.NormalizeRect();
    CRect saverect=rect;
    long r2=(rect.top-rect.bottom)*(rect.top-rect.bottom)+(rect.left-rect.right)*(rect.left-rect.right);
    int l=(int)sqrt((double)r2);
    if (l)
    {
        dy=(nArrowlength*(rect.top-rect.bottom))/l;
        dx=(nArrowlength*(rect.left-rect.right))/l;
        dy1=(nArrowlength*(rect.top-rect.bottom))/l;
        dx1=(nArrowlength*(rect.left-rect.right))/l;
    }
    if (rect.top > rect.bottom)
    {
        //rect.top -= m_logpen.lopnWidth.y / 2;
        rect.top -= abs(dy1);
        //rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
        rect.bottom += abs(dy);
    }
    else
    {
        //rect.top += (m_logpen.lopnWidth.y + 1) / 2;
        rect.top += abs(dy1);
        //rect.bottom -= m_logpen.lopnWidth.y / 2;
        rect.bottom -=abs(dy);
    }
    if (rect.left > rect.right)
    {
        //rect.left -= m_logpen.lopnWidth.x / 2;
        rect.left -=abs(dx1);
        //rect.right += (m_logpen.lopnWidth.x + 1) / 2;
        rect.right +=abs(dx);
    }
    else
    {
        //rect.left += (m_logpen.lopnWidth.x + 1) / 2;
        rect.left +=abs(dx1);
        //rect.right -= m_logpen.lopnWidth.x / 2;
        rect.right -=abs(dx);
    }

    //actually a triagle
    points[0]=saverect.BottomRight();
    points[1]=points[2]=rect.BottomRight();
    points[1].x+=nArrowSize*dy/nArrowlength;
    points[1].y-=nArrowSize*dx/nArrowlength;
    points[2].x-=nArrowSize*dy/nArrowlength;
    points[2].y+=nArrowSize*dx/nArrowlength;
    pDC->Polygon(points,3);

/*  if (sae)
    {
        points[0]=saverect.TopLeft();
        points[1]=points[2]=rect.TopLeft();
        points[1].x+=arrowSize*dy1/arrowlength;
        points[1].y-=arrowSize*dx1/arrowlength;
        points[2].x-=arrowSize*dy1/arrowlength;
        points[2].y+=arrowSize*dx1/arrowlength;
        pDC->Polygon(points,3);
    }*/

    //rgn.CreatePolygonRgn(points, 3, ALTERNATE);
    //pDC->FillRgn(&rgn,pbrush);
}


//////////////////////////////////////////////////////////////////////
/// Method CObject3D::DrawDepGraphLink
///
/// \param %PARAMNAME% (%PARAMTYPE%)    todo
/// \return void
//////////////////////////////////////////////////////////////////////
void CObject3D::DrawDepGraphLink(CDC* pDC,CObject3D *pSrc/*=NULL*/,CObject3D *pDest/*=NULL*/,
                                 int nTrace/*=DIS_GRAPH_NONE*/)
{
    if (!pSrc || !pDest) return;
    if (!pDest->bDrawInGraph) return;

    CRect rParRect = pDest->rGraphRect;
    CPoint locPt(pSrc->rGraphRect.CenterPoint().x,pSrc->rGraphRect.bottom);
    CPoint parPt(rParRect.CenterPoint().x,rParRect.top);

    COLORREF mclr = RGB(128,128,128);
    if (nTrace==GRAPH_PARENT || nTrace==GRAPH_BASE)
        mclr = RGB(0,128,0);
    else if (nTrace==GRAPH_CHILDREN)
        mclr = RGB(128,0,0);
    else if (nTrace==GRAPH_FULL)
        mclr = RGB(0,0,255);

    CPen mTracePenV(PS_DOT,1,RGB(128,128,128));
    CPen mTracePenVT(PS_SOLID,1,mclr /*RGB(128,128,128)*/);
    CPen mTracePenI(PS_DOT,1,RGB(255,0,0));
    CPen mTracePenIT(PS_SOLID,1,RGB(255,0,0));
    CPen *oldP = NULL;

    if (nTrace)
    {
        if (pSrc->bValidate)
            oldP = pDC->SelectObject(&mTracePenVT);
        else
            oldP = pDC->SelectObject(&mTracePenIT);
    }
    else
    {
        if (pSrc->bValidate)
            oldP = pDC->SelectObject(&mTracePenV);
        else
            oldP = pDC->SelectObject(&mTracePenI);
    }
    CPoint locSt = locPt;
    locPt+=CSize(0,8);
    pDC->MoveTo(locPt);
    pDC->LineTo(parPt);

    if (oldP) pDC->SelectObject(oldP );
    if (pSrc->bValidate)
        oldP = pDC->SelectObject(&mTracePenVT);
    else
        oldP = pDC->SelectObject(&mTracePenIT);

    CBrush mbrush(mclr);//::GetSysColor(COLOR_3DLIGHT));
    CBrush *oldb= pDC->SelectObject(&mbrush);
    DrawGraphArrow(pDC,locPt,locSt,4,8,NULL);
    pDC->SelectObject(oldb);
    if (oldP) pDC->SelectObject(oldP );
}

//////////////////////////////////////////////////////////////////////
/// Method CObject3D::DrawDepGraph
///
/// \param %PARAMNAME% (%PARAMTYPE%)    todo
/// \return void
//////////////////////////////////////////////////////////////////////
void CObject3D::DrawDepGraph(CDC* pDC,CImageList *pImgList,int nTrace,BOOL bDrawNode,BOOL bDrawLink)
{
    //if (pComposite) return;
    if (bDrawNode)
    {
        if (!nTrace)
        {
                if (IsSelected())
    {
        CRect focRect = rGraphRect;
        if (focRect != CRect(-1,-1,-1,-1))
        {
            focRect.InflateRect(-1,-1);
            CBrush mbr(RGB(192,192,192));
            CBrush *oldB = pDC->SelectObject(&mbr);
            pDC->Rectangle(focRect);
            pDC->SelectObject(oldB );
            //pDC->DrawFocusRect(focRect);
        }
    }
    CString mstr = GetObjectName();

            //CString strDepth;
            //strDepth.Format(_T("%d  "),nDepth);
            //mstr = strDepth + mstr;
            CRect rText = rGraphRect;

            if (pImgList)
            {
                int ndelt = (rGraphRect.Height() -4 - 16);
                CPoint pt(rGraphRect.left + 2+ ndelt/2,rGraphRect.top+4);
                int nbImg = GetNameID() - IDS_NAME_OBJECT;
                if (IsVisible())
                    pImgList->Draw(pDC,nbImg,pt,ILD_TRANSPARENT);
                else
                    pImgList->Draw(pDC,nbImg,pt,ILD_MASK/*ILD_FOCUS*/);
                rText.DeflateRect(2,ndelt+2,2,2);
            }
            COLORREF oldC;
            if (bValidate)
            {
                if (IsVisible())
                    oldC = pDC->SetTextColor(RGB(0,0,0));
                else
                    oldC = pDC->SetTextColor(RGB(128,128,128));
            }
            else
                    oldC = pDC->SetTextColor(RGB(255,0,0));
            int oldM = pDC->SetBkMode(TRANSPARENT);
            pDC->DrawText(mstr,rText,DT_END_ELLIPSIS|DT_CENTER|DT_WORDBREAK|DT_SINGLELINE );
            pDC->SetBkMode(oldM);
            pDC->SetTextColor(oldC);
        }
        else
        {
            COLORREF mclr = RGB(0,0,255);
            if (nTrace==GRAPH_PARENT || nTrace==GRAPH_BASE)
                mclr = RGB(0,128,0);
            else if (nTrace==GRAPH_CHILDREN)
                mclr = RGB(128,0,0);
            CPen mTracePenV(PS_SOLID,1,mclr /*RGB(128,128,128)*/);
            CPen mTracePenI(PS_SOLID,1,RGB(255,0,0));
            LOGBRUSH pbrs={BS_HOLLOW,RGB(0,0,0),0};
            CBrush pBrush;
            pBrush.CreateBrushIndirect(&pbrs);
            CPen    *oldP = NULL;
            if (bValidate)
                oldP = pDC->SelectObject(&mTracePenV);
            else
                oldP = pDC->SelectObject(&mTracePenI);
            CBrush  *oldB = pDC->SelectObject(&pBrush);

            pDC->Rectangle(rGraphRect);
            pDC->SelectObject(oldB );
            pDC->SelectObject(oldP );
        }


    }
//  CxObject3DSet *pList = GetParents();
//  if (!pList) return;

//  int nb = pList->GetSize();

    if (!bDrawLink) return;

    int nb = cDependList.GetSize();
    for (int i=0;i<nb;i++)
    {
//      CObject3D* pPar = pList->GetAt(i);
        CObject3D* pPar = cDependList.GetAt(i);
        if (!pPar) continue;

        DrawDepGraphLink(pDC,this,pPar,nTrace);
/*      if (!pPar->bDrawInGraph) continue;

        CRect rParRect = pPar->rGraphRect;
        CPoint locPt(rGraphRect.CenterPoint().x,rGraphRect.bottom);
        CPoint parPt(rParRect.CenterPoint().x,rParRect.top);

        COLORREF mclr = RGB(0,0,255);
        if (nTrace==DIS_GRAPH_PARENT)
            mclr = RGB(0,128,0);
        else if (nTrace==DIS_GRAPH_CHILDREN)
            mclr = RGB(128,0,0);

        CPen mTracePenV(PS_DOT,1,RGB(128,128,128));
        CPen mTracePenVT(PS_SOLID,1,mclr);// RGB(128,128,128));
        CPen mTracePenI(PS_DOT,1,RGB(255,0,0));
        CPen mTracePenIT(PS_SOLID,1,RGB(255,0,0));
        CPen *oldP = NULL;

        if (nTrace)
        {
            if (bValidate)
                oldP = pDC->SelectObject(&mTracePenVT);
            else
                oldP = pDC->SelectObject(&mTracePenIT);
        }
        else
        {
            if (bValidate)
                oldP = pDC->SelectObject(&mTracePenV);
            else
                oldP = pDC->SelectObject(&mTracePenI);
        }
        pDC->MoveTo(locPt);
        pDC->LineTo(parPt);
        if (oldP) pDC->SelectObject(oldP );*/

    }

}

//////////////////////////////////////////////////////////////////////
/// Method CObject3D::Draw
///
/// \param %PARAMNAME% (%PARAMTYPE%)    todo
/// \return void
//////////////////////////////////////////////////////////////////////
void CObject3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSm)
{
}

int CObject3D::CalculConceptuel()
{
    return 0;
}

void CObject3D::CalculVisuel(CVisualParam *mV)
{
}



//////////////////////////////////////////////////////////////////////
// Static Functions
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// Method CObject3D::DoSegRgn
///
/// \param %PARAMNAME% (%PARAMTYPE%)    todo
/// \return CRgn*
//////////////////////////////////////////////////////////////////////
CRgn* CObject3D::DoSegRgn(CPoint p1,CPoint p2)
{
    CPoint pt[4] =
            {   CPoint(p1.x-TPref::TUniv.nDefPres,p1.y-TPref::TUniv.nDefPres),
                CPoint(p2.x-TPref::TUniv.nDefPres,p2.y-TPref::TUniv.nDefPres),
                CPoint(p2.x+TPref::TUniv.nDefPres,p2.y+TPref::TUniv.nDefPres),
                CPoint(p1.x+TPref::TUniv.nDefPres,p1.y+TPref::TUniv.nDefPres)
            };

    CRgn *myReg = new CRgn();
    BOOL ret = myReg->CreatePolygonRgn(pt,4,ALTERNATE);

    //return myReg;
    DWORD dd = GetLastError();
    return myReg;
}
