// Sphere3D.cpp: implementation of the CSphere3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\Prefs\Prefs.h"

#include "Vector4.h"
#include "Sphere3D.h"
#include "Point3D.h"
#include "Droite3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Static Functions
//////////////////////////////////////////////////////////////////////
#define MRG_ZERO 1.0e-8

BOOL CSphere3D::IsPointVisible(CVisualParam* myVisuParam,CVector4& P1,CVector4& P)
{
	CVector4 FaceNorm= P - P1;
	CVector4 oeil= myVisuParam->GetEyePos();
	CVector4 origin(0,0,0,1);
	CVector4 VisuNorm= oeil - 
				((myVisuParam->bParProj) ?
					origin : P);
	FCoord dot = VisuNorm * FaceNorm;
	int vis = (dot >= -MRG_ZERO);
	return vis;
}

void CSphere3D::CalcUnit(CVector4 I,CVector4 J,CVector4 K,CVector4 P1,int t,FCoord r,CVector4 & V)
{
	CVector4 U;
	U.x=	r*cos(2*M_PI*t/TPref::TCercle.nDelta);
	U.y=	r*sin(2*M_PI*t/TPref::TCercle.nDelta);
	U.z=	0;
	U.w=	1;

	V.x=	I.x*U.x + J.x*U.y + K.x*U.z + P1.x,
	V.y=	I.y*U.x + J.y*U.y + K.y*U.z + P1.y,
	V.z=	I.z*U.x + J.z*U.y + K.z*U.z + P1.z,
	V.w=	1;
}

void CSphere3D::DrawUnit(CVector4& deb,CVector4& fin,CVector4& P1,
					CDC *pDC,CVisualParam *mV,
					CPen& curPen,CPen& curPenH,CPen& disPen,CPen& disPenH)
{
	BOOL d =	CSphere3D::IsPointVisible(mV,P1,deb);
	BOOL f =	CSphere3D::IsPointVisible(mV,P1,fin);
	BOOL insi = (mV->IsPointVisible(deb) && mV->IsPointVisible(fin));
	CPen *oldP=NULL;
	if (d && f)
		oldP = pDC->SelectObject((insi) ? &curPen : &curPenH);
	else
		oldP = pDC->SelectObject((insi) ? &disPen : &disPenH);
	pDC->MoveTo((CPoint)mV->ProjectPoint(deb));
	pDC->LineTo((CPoint)mV->ProjectPoint(fin));
	pDC->SelectObject(oldP);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CSphere3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CSphere3D::CSphere3D() : CObject3D() 
{
	locVisParam = NULL;
}

CSphere3D::CSphere3D(CPoint3D *p1,CPoint3D *p2) : CObject3D()
{
	locVisParam = NULL;
	P1 = p1;
	P2 = p2;
	nDepth = max(p1->nDepth,p2->nDepth)+1;
	pObjectShape.clrObject = RGB(0,0,255);
}

CSphere3D::CSphere3D(const CObject3D & src): CObject3D(src)
{
	locVisParam = NULL;
	P1 =  ((CSphere3D&)src).P1;
	P2 =   ((CSphere3D&)src).P2;
}

int CSphere3D::SetDepth()
{
	if (P1 && P2)
		nDepth = max(P1->nDepth,P2->nDepth)+1;
	return nDepth;
}


BOOL CSphere3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
	if (P1 == pOld)
	{
		SetInGraph(FALSE);
		P1 = (CPoint3D*)pNew;
		if (bUpGraph) SetInGraph(TRUE);
	}
	else if (P2 == pOld)
	{
		SetInGraph(FALSE);
		P2 = (CPoint3D*)pNew;
		if (bUpGraph) SetInGraph(TRUE);
	}
	else return FALSE;
	return TRUE;
}


CSphere3D::~CSphere3D()
{
}

CObject3D* CSphere3D::CopyObject()
{
	CObject3D *temp = new CSphere3D((CObject3D&)*this);
	return temp;
}

void CSphere3D::GetRange(CVector4 &min,CVector4 &max)
{
	CObject3D::GetRange(min,max);
	if (bValidate && IsVisible() && P1)
	{
		FCoord radius = Rayon.Norme();
		CVector4 vr(radius,radius,radius);

		min = P1->Concept_pt - vr;
		max = P1->Concept_pt + vr;
	}
}



CxObject3DSet* CSphere3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(P1);
	list->Add(P2);
	return list;
}

void CSphere3D::Serialize( CArchive& ar )
{
	CObject3D::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << Rayon;
		ar << ((P1) ? P1->nObjectId : -1);
		ar << ((P2) ? P2->nObjectId : -1);
	}
	else
	{
		ar >> Rayon;
		P1 = (CPoint3D*)SerializeObj(ar);
		P2 = (CPoint3D*)SerializeObj(ar);
	}
}


BOOL CSphere3D::IsEqual(CObject3D &o)
{
	if (!(o.MaskObject(TSphere3DClass))) return false;
	if (!bValidate || !(o.bValidate)) return false;
	CVector4 ray= ((CSphere3D&)o).P1->Concept_pt - P1->Concept_pt;
	BOOL SameCenter	= ray.NullVector();
	
	FCoord 	rad1 = Rayon.Norme(),
			rad2 = ((CSphere3D&)o).Rayon.Norme();
	BOOL	SameRadius	= FCZero(rad1 - rad2);
	return (SameCenter &&  SameRadius);
}

BOOL CSphere3D::IsInActiveArea(CPoint pt)
{
//	bool IsInSeg = FALSE;
	FCoord r = Rayon.Norme();
////////////// ZOOM
	if (!locVisParam) return FALSE;
	r = locVisParam->nZoom*r;
	CPoint p1 = (CPoint)locVisParam->ProjectPoint(P1->Concept_pt);
	CPoint top((int)(p1.x-r),(int)(p1.y-r));
	CPoint bottom((int)(p1.x+r),(int)(p1.y+r));

	int m=3;
	CRgn r1,r2;
	r1.CreateEllipticRgn((int)(p1.x-r-m),(int)(p1.y-r-m),(int)(p1.x+r+m),(int)(p1.y+r+m));
	r2.CreateEllipticRgn((int)(p1.x-r+m),(int)(p1.y-r+m),(int)(p1.x+r-m),(int)(p1.y+r-m));

	CRgn r3;
	r3.CreateRectRgn(0,0,1,1);
	int dd = r3.CombineRgn(&r1,&r2,RGN_DIFF);
	return r3.PtInRegion(pt);
}

CRgn* CSphere3D::InvalideRect()
{
	return NULL;
}


UINT  CSphere3D::CalculConceptuel()
{
	bValidate = ((P1->bValidate) && (P2->bValidate));
	if (!bValidate)
		return ERR_NOSPHERE;

	Rayon = P2->Concept_pt - P1->Concept_pt;
 //	FCoord r = Norme(Rayon);

	if (Rayon.NullVector())
	 {	bValidate = FALSE;
		return ERR_SPHPTSID;
	 }
	bValidate=TRUE;
	return 0;
}

void CSphere3D::CalculVisuel(CVisualParam *myVisuParam)
{
	locVisParam = myVisuParam;
}

void CSphere3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
	if ((!bVisible) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;

	int mode = pDC->SetBkMode(TRANSPARENT);

	CPen curPen;
	LOGPEN logP = pObjectShape.GetPenStyle(); //(*ObjectColor,0,PS_SOLID);
	curPen.CreatePenIndirect(&logP);
/*	int m = -1;
	for (int g=0;g<8;g++)
		if (TPref::custColors[g] == (ObjectShape->ObjectColor))
			m = g+8;   */

	CPen disPen(PS_DOT,1,pObjectShape.clrObject);
//	TPen disPenH((m==-1) ? TColor(192,192,192) : TPref::custColors[m],1,PS_DOT);
	CPen disPenH(PS_DOT,1,pObjectShape.GetObjectHiddenColor());

//	TPen curPenH = ObjectShape->GetHiddenPenStyle(
//			(m==-1) ? TColor(192,192,192) : TPref::custColors[m]);
	CPen curPenH;
	logP = pObjectShape.GetHiddenPenStyle(
								pObjectShape.GetObjectHiddenColor());
	curPenH.CreatePenIndirect(&logP);


//	TBrush curBrush((m==-1) ? *ObjectColor : custColors[m],0);
	LOGBRUSH toto = {BS_HOLLOW,(COLORREF)pObjectShape.clrObject,HS_CROSS};
	CBrush curBrush;
	curBrush.CreateBrushIndirect(&toto);
//	curBrush.GetObject(toto);
	CBrush *oldB = pDC->SelectObject(&curBrush);

	FCoord r = Rayon.Norme();

	CVector4 	I(1,0,0,1);
	CVector4 	J(0,1,0,1);
	CVector4 	K(0,0,1,1);
	CVector4 	I2,J2,K2;
	CVector4	I3,J3,K3;
	CVector4 	I4,J4,K4;
	CVector4 	d,a,f,d2,a2,f2,d3,a3,f3,d4,a4,f4,d5,a5,f5;
	FCoord 	interc = 0.,
				zr = P1->Concept_pt.z/r;
	bool isPLanZ = ((zr >= -1.00) && (zr <= 1.0));
	if (isPLanZ)
	 {	interc = asinl(P1->Concept_pt.z/r);
		interc = cosl(interc);
		interc = interc*r;
	 }

	// Grand cercle passant par P2
	CVector4 P3(P2->Concept_pt);
	P3.z = P1->Concept_pt.z;
	CVector4 R = P3 - P1->Concept_pt;
	FCoord ptn= R.Norme();
	if (!FCZero(ptn))
	 {
		J2 = R * (1/ptn);
		K2 = CVector4(J2.y,-J2.x,0,1);
		FCoord normK  = K2.Norme();
		K2 = K2 * (1/normK);
		I2 = K2 % J2;
	 }
	else
	 {
		I2 = K;
		J2 = I;
		K2 = J;
	 }

	// Contour apparent
	CVector4 oeil= mV->GetEyePos();
	CVector4 origin(0,0,0,1);
	K3 = oeil - ((mV->bParProj) ? origin : P1->Concept_pt);
	//	K3 = P1->Concept_pt + VisuNorm;
	K3 = K3 * (1/K3.Norme());
	J3 = CVector4(K3.y,-K3.x,0,1);
	J3 = J3 * (1/J3.Norme());
	I3 = J3 % K3;


	CalcUnit(I3,J3,K3,P1->Concept_pt,0,r,d5);
	a5 = d5;
	CalcUnit(I,J,K,P1->Concept_pt,0,r,d);
	a = d;
	if (bMarked && mV->bDrawMark && isPLanZ)
	 {
		CVector4 P = P1->Concept_pt;
		P.z = 0.;
		CalcUnit(I,J,K,P,0,interc,d4);
		a4 = d4;
	 }
	if (!FCZero(ptn) || 1)
	{
		CalcUnit(I2,J2,K2,P1->Concept_pt,0,r,d3);
		a3=d3;
		//CalcUnit(K,I,J,P1->Concept_pt,0,r,d2);
		CalcUnit(I2,K2,J2,P1->Concept_pt,0,r,d2);
		a2 = d2;
	 }

	for (int t=1;t<TPref::TCercle.nDelta;t++)
	 {
		//// contour apparent
		CalcUnit(I3,J3,K3,P1->Concept_pt,t,r,f5);
		DrawUnit(d5,f5,P1->Concept_pt,pDC,mV,curPen,curPenH,disPen,disPenH);
		d5 = f5;

		//// grand cercle horizontal
		if (TPref::TSphere.bEquator)
		{
			CalcUnit(I,J,K,P1->Concept_pt,t,r,f);
			DrawUnit(d,f,P1->Concept_pt,pDC,mV,curPen,curPenH,disPen,disPenH);
			d = f;
		}

		//// grand cercle passant par P2
		if ((!FCZero(ptn) || 1) && (TPref::TSphere.bGCircle))
		 {
			CalcUnit(I2,J2,K2,P1->Concept_pt,t,r,f3);
			DrawUnit(d3,f3,P1->Concept_pt,pDC,mV,curPen,curPenH,disPen,disPenH);
			d3=f3;

			//// grand cercle vertical
			//CalcUnit(K,I,J,P1->Concept_pt,t,r,f2);
			CalcUnit(I2,K2,J2,P1->Concept_pt,t,r,f2);
			DrawUnit(d2,f2,P1->Concept_pt,pDC,mV,curPen,curPenH,disPen,disPenH);
			d2 = f2;
		 }
		
		//// intersection sphere/plancher horizontal
		if (bMarked && mV->bDrawMark	&& isPLanZ)
		 {
			CVector4 P = P1->Concept_pt;
			P.z = 0.;
			CalcUnit(I,J,K,P,t,interc,f4);
			CPen iPen(PS_DOT,1,RGB(0,0,0));
			DrawUnit(d4,f4,P1->Concept_pt,pDC,mV,iPen,iPen,iPen,iPen);
			d4=f4;
		 }
	 }
	DrawUnit(a5,f5,P1->Concept_pt,pDC,mV,curPen,curPenH,disPen,disPenH);
	if (TPref::TSphere.bEquator)
		DrawUnit(a,f,P1->Concept_pt,pDC,mV,curPen,curPenH,disPen,disPenH);
	if ((!FCZero(ptn) || 1) && (TPref::TSphere.bGCircle))
	 {
		DrawUnit(a3,f3,P1->Concept_pt,pDC,mV,curPen,curPenH,disPen,disPenH);
		DrawUnit(a2,f2,P1->Concept_pt,pDC,mV,curPen,curPenH,disPen,disPenH);

	 }
	if (bMarked && mV->bDrawMark	&& isPLanZ)
	 {
			CPen iPen(PS_DOT,1,RGB(0,0,0));
			DrawUnit(a4,f4,P1->Concept_pt,pDC,mV,iPen,iPen,iPen,iPen);
	 }

/*	TPoint p1 = (TPoint)myVisuParamTemp->ProjectPoint(P1->Concept_pt);
////////////// ZOOM
	r = TPref::Zoom*r;
	TPoint top(p1.x-r+1,p1.y-r+1);
	TPoint bottom(p1.x+r+1,p1.y+r+1);
	HandleDC.SelectObject(curPen);
	HandleDC.Ellipse(TRect(top,bottom));*/
	pDC->SetBkMode(mode);
	pDC->SelectObject(oldB);
}

CString CSphere3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???")),sn2(_T("???"));
	if (P1) sn1 = P1->GetObjectHelp();
	if (P2) sn2 = P2->GetObjectHelp();
	mstr.Format(sFormat,sName,sn1,sn2);
	return mstr;
}

CString CSphere3D::DrawSymbolic()
{
	CString mstr;
	mstr.Empty();

	if (bValidate && P1 && P2)
	{
		CString mstr2,strName,strObj1,strObj2,strObj3;
		mstr2.LoadString(GetNameID());
		strName.Format("%s%d",mstr2,nObjectId);
		mstr2.LoadString(P1->GetNameID());
		strObj1.Format("%s%d",mstr2,P1->nObjectId);
		mstr2.LoadString(P2->GetNameID());
		strObj2.Format("%s%d",mstr2,P2->nObjectId);

		mstr.Format(_T("SphereD[%s,%s,%s];"),strName,strObj1,strObj2);
	}
	return mstr;
}


void CSphere3D::DrawRetro(CDC*,CVisualParam *vp)
{
}

BOOL CSphere3D::IntersectLine(CDroite3D *Dr,CVector4 *pt1,CVector4 *pt2)
{
	CVector4 raybase = Dr->GetBasePoint();
	CVector4 raycos = Dr->GetDirVector();
	FCoord t = raycos.Norme();
	raycos = raycos * (1/t);
	CVector4 center = P1->Concept_pt;
	FCoord radius = Rayon.Norme();

	BOOL	hit;			/* True if ray intersects sphere*/
	FCoord	dx, dy, dz;		/* Ray base to sphere center	*/
	FCoord	bsq, u, disc;
	FCoord	root,rin,rout;


	dx   = raybase.x - center.x;
	dy   = raybase.y - center.y;
	dz   = raybase.z - center.z;
	bsq  = dx*raycos.x + dy*raycos.y + dz*raycos.z;
	u    = dx*dx + dy*dy + dz*dz - radius*radius;
	disc = bsq*bsq - u;
	if (FCZero(disc)) disc = 0.0;
	hit  = (disc >= -1E-5);
	if  (hit)
	 { 				/* If ray hits sphere	*/
		root  =  sqrt(disc);
		rin  = -bsq - root;		/*    entering distance	*/
		rout = -bsq + root;		/*    leaving distance	*/

		CVector4 dir2 = raycos * (rin/raycos.Norme());
		*pt1 = raybase+dir2;
		dir2 = raycos * (rout/raycos.Norme());
		*pt2 = raybase+dir2;

	 }
	return (hit);
}
