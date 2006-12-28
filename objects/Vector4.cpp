// Vector4.cpp: implementation of the CVector4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "Vector4.h"
#include "Point3D.h"
#include "Plan3D.h"

#include "..\Prefs\Prefs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Handy constant
#define MRG_ZERO 1.0e-8


/**FCoord DTR(FCoord f)
{	return (FCoord)((f*M_PI)/180);}

FCoord RTD(FCoord f)
{	return (int)((f*180)/M_PI);}*/

#define DTR(x)	((x*M_PI)/180)
#define RTD(x)	((x*180)/M_PI)

#define s(x)	sin(DTR(x))
#define c(x)	cos(DTR(x))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

//////////////////////////////////////////////////////////////////////
// CVector4 Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*IMPLEMENT_SERIAL(CVector4, CObject, VERSIONABLE_SCHEMA | 1)

void CVector4::Serialize( CArchive& ar )
{
	if (ar.IsLoading())
	{
		ar >> x;
		ar >> y;
		ar >> z;
		ar >> w;
		ar >> N;
	}
	else
	{
		ar << x;
		ar << y;
		ar << z;
		ar << w;
		ar << N;
	}
}*/

//////////////////////////////////////////////////////////////////////
/// Write the vector into the archive
//////////////////////////////////////////////////////////////////////
CArchive& operator<<(CArchive& ar, const CVector4& pt)
{
	ar << pt.x;
	ar << pt.y;
	ar << pt.z;
	ar << pt.w;
	ar << pt.N;
	return ar;
}

//////////////////////////////////////////////////////////////////////
/// Read the vector from the archive
//////////////////////////////////////////////////////////////////////
CArchive& operator>>(CArchive& ar, CVector4& pt)
{
	ar >> pt.x;
	ar >> pt.y;
	ar >> pt.z;
	ar >> pt.w;
	ar >> pt.N;
	return ar;
}

//////////////////////////////////////////////////////////////////////
// CVisualParam Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVisualParam::CVisualParam()
{
	SProjection MySProj= TPref::TUniv.sDefParam;

	nVisuKind = VisuNone;	
	ST = SP = CT = CP = 0.0;
	ProjParam = MySProj;

	nWndWidth = 600;
	nWndHeight=800;
	ptRepCoord = CPoint(0,0);
	nCalqueNum = 0;
	nZoom = 1.0;
	bDrawMark = FALSE;
	bParProj = TRUE;
	bFixed = FALSE;
	bKeepProj = FALSE;
	bFeedPlane = FALSE;
	F = G = H = NULL;
	Orig = NULL;
	Orig = new CPoint3D(CVector4(0,0,0));
	//Parent=src.Parent;
	F = new CPlan3D(Orig,CVector4(100,0,0));
	G = new CPlan3D(Orig,CVector4(0,100,0));
	H = new CPlan3D(Orig,CVector4(0,0,100));
	F->CalculConceptuel();
	G->CalculConceptuel();
	H->CalculConceptuel();
}

CVisualParam::CVisualParam(const CVisualParam& param)
{
	nVisuKind = param.nVisuKind;
	ProjParam = param.ProjParam;
	nCalqueNum = param.nCalqueNum;
	nZoom = param.nZoom;
	bParProj = param.bParProj;
	nWndWidth = param.nWndWidth;
	nWndHeight = param.nWndHeight;
	ptRepCoord = param.ptRepCoord;
	bDrawMark = param.bDrawMark;
	bFixed	= param.bFixed;
	bKeepProj = param.bKeepProj;
	bFeedPlane = param.bFeedPlane;
	ST= param.ST;
	SP = param.SP;
	CT= param.CT;
	CP= param.CP;

	Orig = new CPoint3D(CVector4(0,0,0));
	//Parent=src.Parent;
	F = new CPlan3D(Orig,CVector4(100,0,0));
	G = new CPlan3D(Orig,CVector4(0,100,0));
	H = new CPlan3D(Orig,CVector4(0,0,100));
	F->CalculConceptuel();
	G->CalculConceptuel();
	H->CalculConceptuel();
	SetProjParam(ProjParam);
}

CVisualParam::CVisualParam(SProjection proj)
{
	//Parent=AParent;
	ProjParam = proj;
	nWndWidth = 600;
	nWndHeight=800;
	ptRepCoord = CPoint(0,0);
	nVisuKind = VisuNone;
	nCalqueNum = 0;
	bParProj = TRUE;
	bDrawMark = FALSE;
	nZoom = TPref::TUniv.fZoom;
	bFixed = FALSE;
	bKeepProj = FALSE;
	bFeedPlane = FALSE;
	//WndWidth = DefWidth;
	//WndHeight = DefHeight;
	//RepCoord.x = WndWidth / 2 ;
	//RepCoord.y = WndHeight / 2;
	Orig = new CPoint3D(CVector4(0,0,0));
	//Parent=src.Parent;
	F = new CPlan3D(Orig,CVector4(100,0,0));
	G = new CPlan3D(Orig,CVector4(0,100,0));
	H = new CPlan3D(Orig,CVector4(0,0,100));
	F->CalculConceptuel();
	G->CalculConceptuel();
	H->CalculConceptuel();
	SetProjParam(ProjParam);
}

CVisualParam::~CVisualParam()
{
	if (F) delete F;
	if (G) delete G;
	if (H) delete H;
	if (Orig) delete Orig;
}

void CVisualParam::Serialize( CArchive& ar )
{
	if (ar.IsLoading())
	{
	}
	else
	{
	}
}

void CVisualParam::SetProjParam(FCoord NewVal,int Id)
{
	if (bFixed) return;
	switch (Id){	
		case ID_THETA:
		case ID_PHI:
			while (NewVal >= 360) NewVal= NewVal - 360;
			while (NewVal < 0) NewVal+=360;
			(Id == ID_THETA) ? 	ProjParam.theta=NewVal :
								ProjParam.phi=NewVal;
			break;
		case ID_RHO:
		case ID_DIS:
			if (NewVal <= 0) NewVal=1.0;
			(Id == ID_RHO) ?	ProjParam.rho=NewVal :
								ProjParam.dis=NewVal;
			break;
	}
}

void CVisualParam::AddProjParam(FCoord NewVal,TVisuParam Id)
{
	if (bFixed) return;
	switch (Id){
		case ID_THETA:
			{
				ProjParam.theta+=NewVal;
				//int nb = (int)((ProjParam.theta + NewVal) * NewVal);
				//nb +=NewVal;
				//if (!FCZero(NewVal))
				//	nb /= (int)NewVal;
				//int nb = 
			}
			break;
		case ID_PHI:
			ProjParam.phi+=NewVal;
			break;
		case ID_RHO:
		case ID_DIS:
			(Id == ID_RHO) ?	ProjParam.rho+=NewVal :
								ProjParam.dis+=NewVal;
			break;
	}
	SetProjParam(ProjParam);
}

void CVisualParam::SetProjParam()
{
	if (bFixed) return;
	SProjection MySProj=TPref::TUniv.sDefParam;

	SetProjParam(MySProj);
}

void CVisualParam::SetProjParam(SProjection Param)
{
	if (bFixed) return;
	SetProjParam(Param.theta,ID_THETA);
	SetProjParam(Param.phi,ID_PHI);
	SetProjParam(Param.rho,ID_RHO);
	SetProjParam(Param.dis,ID_DIS);
	ST = s(ProjParam.theta);
	SP = s(ProjParam.phi);
	CT = c(ProjParam.theta);
	CP = c(ProjParam.phi);
}


SProjection CVisualParam::GetProjParam()
{
	return ProjParam;
}

CVector4 CVisualParam::GetEyePos()
{
	CVector4 temp(ProjParam.rho*CT*CP,ProjParam.rho*ST*CP,ProjParam.rho*SP,1);
	return temp;
}

//////////////////////////////////////////////////////////////////////
/// Method CVisualParam::ProjectPoint
///
/// \verbatim
///    | Px |   | -sin(t)        cos(t)         0        0 |   | Sx |
///    | Py |   | -cos(t)sin(p)  -sin(t)sin(p)  cos(p)   0 |   | Sy |
///    | Pz | = | -cos(t)cos(p)  -sin(t)cos(p)  -sin(p)  r | * | Sz |
///    | Pw |   | 0              0              0        1 |   | Sw |
/// \endverbatim
///
//////////////////////////////////////////////////////////////////////
CVector4 CVisualParam::ProjectPoint(CVector4 src)
{
	CVector4 temp = src;
	
	////////////// ZOOM
	temp = temp*nZoom;
	FCoord 	x=temp.x,
			y=temp.y,
			z=temp.z;

	temp.x = y*CT - x*ST;
	temp.y = z*CP - x*CT*SP -y*ST*SP;
	temp.z = ProjParam.rho - x*CT*CP -y*ST*CP -z*SP;
	if (!bParProj)
	{
		temp.x = ProjParam.dis*temp.x/temp.z;
		temp.y = ProjParam.dis*temp.y/temp.z;
	}
	temp.x += ptRepCoord.x;
	temp.y = ptRepCoord.y - temp.y;
	return temp;
}


CVector4 CVisualParam::GetProjectedPoint(CPoint MouseClic)
{
	CVector4 temp(MouseClic.x,MouseClic.y,0,1);

/*	MouseClic.x-= ptRepCoord.x;
	MouseClic.y = ptRepCoord.y - MouseClic.y ;

	////////////// ZOOM
	MouseClic.x = (long)(1/nZoom)*MouseClic.x;
	MouseClic.y = (long)(1/nZoom)*MouseClic.y;

	FCoord 	theX = (FCoord) MouseClic.x,
			theY = (FCoord) MouseClic.y;

	FCoord sphi = SP;
	if (sphi == 0) sphi+=0.001;

	CVector4 ConceptPt(
		-(theY*CT + theX*SP*ST) / sphi ,
		(theX*CT*SP - theY*ST) / sphi,
		0,1 );*/
	if (!bParProj)
	{
		FCoord den = nZoom*(CT*CT + ST*ST)*(-ProjParam.dis*SP + CP*nZoom*(ptRepCoord.y - temp.y));
		FCoord aa = ProjParam.rho*(SP*ST*(temp.x - ptRepCoord.x) + nZoom*CT*(ptRepCoord.y-temp.y));
		FCoord bb = ProjParam.rho*(CT*SP*(ptRepCoord.x - temp.x) + nZoom*ST*(ptRepCoord.y-temp.y));
		temp.x = aa/den;
		temp.y = bb/den;
	}
	else
	{
		FCoord sphi = SP;
		if (sphi == 0) sphi+=0.001;
		FCoord den = nZoom*sphi;
		FCoord aa = CT*(temp.y-ptRepCoord.y) + SP*ST*(ptRepCoord.x - temp.x);
		FCoord bb = CT*SP*(temp.x-ptRepCoord.x) + ST*(temp.y- ptRepCoord.y);
		temp.x = aa/den;
		temp.y = bb/den;
	}
	//TRACE(" >>>>> %d %d \n",(int)temp.x,(int)temp.y);
	//return (CPoint) ConceptPt;
	return temp;
}

CVector4 CVisualParam::GetScreenProjection(CVector4 vec)
{
	FCoord
		st = ST,
		sp = SP,
		ct = CT,
		cp = CP,
		RCx = ptRepCoord.x,
		RCy = ptRepCoord.y,
		x0 = vec.x-RCx,
		y0 = RCy-vec.y;

	////////////// ZOOM
	x0 = (1/nZoom)*x0;
	y0 = (1/nZoom)*y0;

	FCoord
		z0 = ProjParam.dis,
		Rh = ProjParam.rho,
		z = -sp*z0 + sp*Rh + y0*cp,
		y = -sp*y0*st + x0*ct -cp*z0*st + cp*Rh*st,
		x = -ct*sp*y0 - ct*cp*z0 + ct*cp*Rh - x0*st;
	CVector4 ecran(x,y,z);
	return ecran;
}

CVector4 CVisualParam::GetScreenProjectionInf(CVector4 vec)
{
	FCoord
		st = ST,
		sp = SP,
		ct = CT,
		cp = CP,
		RCx = ptRepCoord.x,
		RCy = ptRepCoord.y,
		x0 = vec.x-RCx,
		y0 = RCy-vec.y;

	////////////// ZOOM
	x0 = (1/nZoom)*x0;
	y0 = (1/nZoom)*y0;

	FCoord
		z0 = vec.z,
		Rh = ProjParam.rho,
		z = -sp*z0 + sp*Rh + y0*cp,
		y = -sp*y0*st + x0*ct -cp*z0*st + cp*Rh*st,
		x = -ct*sp*y0 - ct*cp*z0 + ct*cp*Rh - x0*st;
	CVector4 ecran(x,y,z);
	return ecran;
}


void CVisualParam::ApplyMagnet(CVector4& pt)
{
	FCoord a;

	a = (pt.x / (FCoord) TPref::TUniv.nUnitRep) - (int)(pt.x / (FCoord) TPref::TUniv.nUnitRep);
	pt.x= (int)((pt.x / (FCoord) TPref::TUniv.nUnitRep) + ((a < 0.5) ? 0 : 1) ) * TPref::TUniv.nUnitRep;

	a = (pt.y / (FCoord) TPref::TUniv.nUnitRep) - (int)(pt.y / (FCoord) TPref::TUniv.nUnitRep);
	pt.y= (int)((pt.y / (FCoord) TPref::TUniv.nUnitRep) + ((a < 0.5) ? 0 : 1) ) * TPref::TUniv.nUnitRep;

	a = (pt.z / (FCoord) TPref::TUniv.nUnitRep) - (int)(pt.z / (FCoord) TPref::TUniv.nUnitRep);
	pt.z= (int)((pt.z / (FCoord) TPref::TUniv.nUnitRep) + ((a < 0.5) ? 0 : 1) ) * TPref::TUniv.nUnitRep;
}


//////////////////////////////////////////////////////////////////////
// CVisuNone Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVisuNone::CVisuNone(): 
	CVisualParam()
{
	nVisuKind = VisuNone;	
}

CVisuNone::CVisuNone(const CVisualParam& param): 
	CVisualParam(param)
{
	nVisuKind = VisuNone;	
}

void CVisuNone::Draw(CDC*)
{
}

void CVisuNone::DrawFeedBack(CDC*,CPoint3D*)
{
}

//////////////////////////////////////////////////////////////////////
// CVisuRep Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVisuRep::CVisuRep(): 
	CVisualParam()
{
	nVisuKind = VisuRep;	
}

CVisuRep::CVisuRep(const CVisualParam& param): 
	CVisualParam(param)
{
	nVisuKind = VisuRep;	
}

void CVisuRep::Draw(CDC* pDC)
{
	int 	RepScale=TPref::TUniv.nUnitRep,
			RepPos=RepScale*10,
			SclSz = 5;
	CVector4 rep[7] = {
				CVector4(0,0,0,1),
				CVector4(RepPos,0,0,1),
				CVector4(0,RepPos,0,1),
				CVector4(0,0,RepPos,1),
				CVector4(-RepPos/3,0,0,1),
				CVector4(0,-RepPos/3,0,1),
				CVector4(0,0,-RepPos/3,1)
				};
	CVector4 scl[6] = {
				CVector4(RepScale,-SclSz,0,1),
				CVector4(RepScale,SclSz,0,1),
				CVector4(-SclSz,RepScale,0,1),
				CVector4(SclSz,RepScale,0,1),
				CVector4(0,-SclSz,RepScale,1),
				CVector4(0,SclSz,RepScale,1)
				};

	CPen	frontPen(PS_SOLID,1,RGB(0,0,0)),
			backPen(PS_SOLID,1,RGB(192,192,192));
	CPen*	oldPen=NULL;

	oldPen = pDC->SelectObject(&frontPen);

	DrawArrow(pDC,rep[0],rep[1],rep[2]);
	DrawArrow(pDC,rep[0],rep[2],rep[1]);
	DrawArrow(pDC,rep[0],rep[3],rep[2]);

	for (int i=0;i<7;i++)
		rep[i] = ProjectPoint(rep[i]);
	for (i=0;i<6;i++)
		scl[i] = ProjectPoint(scl[i]);

	// draw rep lines
	pDC->SelectObject(&backPen);
	for (i=4;i<7;i++)
	 {
		pDC->MoveTo(rep[0]);
		pDC->LineTo(rep[i]);
	 }
	pDC->SelectObject(&frontPen);
	for (i=1;i<4;i++)
	 {
		pDC->MoveTo(rep[0]);
		pDC->LineTo(rep[i]);
	 }
	// draw rep scales
	for (i=0;i<3;i++)
	 {
		pDC->MoveTo(scl[2*i]);
		pDC->LineTo(scl[2*i+1]);
	 }
	pDC->SelectObject(oldPen);

}

void CVisuRep::DrawFeedBack(CDC* pDC,CPoint3D* pt)
{
	CPoint		oldF[4];
	CVector4	T= pt->Concept_pt;
	CVector4 M[4] = { CVector4(T.x,0,0,1),
						  CVector4(0,T.y,0,1),
						  CVector4(T.x,T.y,0,1),
						  CVector4(T.x,T.y,T.z,1)};
	for (int i=0;i<4;i++)
	 {
		M[i] = ProjectPoint(M[i]);
		//oldF[i].x = M[i].x ;//- (int) Parent->Scroller->XPos;       sin
		//oldF[i].y = M[i].y ;//- (int) Parent->Scroller->YPos;
		oldF[i] = (CPoint)M[i];
	 }

	COLORREF curC = RGB(0,0,0);
	CBrush br(curC);
	CPen curP(PS_DOT,1,curC);
	
	CPen *oldP = pDC->SelectObject(&curP);
	if ((int)T.y)
	 {
		pDC->MoveTo(oldF[2].x,oldF[2].y);
		pDC->LineTo(oldF[0].x,oldF[0].y);
	 }
	if ((int)T.x)
	 { 
		pDC->MoveTo(oldF[2].x,oldF[2].y);
		pDC->LineTo(oldF[1].x,oldF[1].y);
	 }
	if ((int)T.z)
	 {	
		pDC->MoveTo(oldF[2].x,oldF[2].y);
		pDC->LineTo(oldF[3].x,oldF[3].y);
	 }

	pDC->SelectObject(oldP);

	CBrush *oldB = pDC->SelectObject(&br);
	if ((int)T.y)
	{
		CRect mrect(CPoint(oldF[0].x,oldF[0].y)-CSize(1,1),CSize(3,3));
		pDC->Rectangle(mrect);
	}
	if ((int)T.x)
	{
		CRect mrect(CPoint(oldF[1].x,oldF[1].y)-CSize(1,1),CSize(3,3));
		pDC->Rectangle(mrect);
	}
	pDC->SelectObject(oldB);
}

void CVisuRep::DrawArrow(CDC* pDC,CVector4 p1,CVector4 p2,CVector4 p3)
{
	// fleche
	CVector4 p1p2 = p2-p1;
	p1p2 = p1p2.Normalized();
	CVector4 p1p3 = p3-p1;
	p1p3 = p1p3.Normalized();

	CVector4 lp2 =  p2+p1p2*(-10.);
	CVector4 pend1 = lp2+p1p3*(-5.);
	CVector4 pend2 = lp2+p1p3*(5.);

	CPoint a = (CPoint)ProjectPoint(pend1);
	CPoint b = (CPoint)ProjectPoint(pend2);
	CPoint c = (CPoint)ProjectPoint(p2);
	pDC->MoveTo(a);
	pDC->LineTo(c);
	pDC->MoveTo(b);
	pDC->LineTo(c);
}


//////////////////////////////////////////////////////////////////////
// CVisuPlane Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVisuPlane::CVisuPlane(): 
	CVisualParam()
{
	nVisuKind = VisuPlane;	
}

CVisuPlane::CVisuPlane(const CVisualParam& param): 
	CVisualParam(param)
{
	nVisuKind = VisuPlane;	
}

bool CVisuPlane::IsPointVisible(CPoint3D* pt)
{
	return IsPointVisible(pt->Concept_pt);
}

bool CVisuPlane::IsPointVisible(CVector4 pt)
{
	return (pt.z >= -MRG_ZERO);
}

void CVisuPlane::DrawOnePLane(CDC* pDC,int min1,int max1,int min2,int max2,int co)
{
	int i,j,k;
	FCoord xv = (co == 1) ? 0 : TPref::TUniv.nUnitRep;
	FCoord yv = (co == 2) ? 0 : TPref::TUniv.nUnitRep;
	FCoord zv = (co == 3) ? 0 : TPref::TUniv.nUnitRep;

	CVector4 FaceNorm((co==1),(co==2),(co==3),1);
	CVector4 DirPt= (bParProj)  ?	CVector4(0,0,0,1) : 
							CVector4(xv*min1,yv*min2,zv*((co==1)?min1:min2),1);
	CVector4 VisuNorm= GetEyePos() - DirPt;
	FCoord tt = VisuNorm*FaceNorm;
	BOOL condvisu = ((VisuNorm*FaceNorm) >= 0);

	CBrush 	br(RGB(255,255,210)),
		 	br3(RGB(255,230,230)),
			br2(RGB(255,255,255)),
			*oldB=NULL;
	CPen 	frontPen(PS_SOLID,1,RGB(192,192,192)),
			backPen(PS_SOLID,1,RGB(192,192,192)),
			nullPen(PS_NULL,1,RGB(192,192,192)),
			*oldP=NULL;

	if (condvisu)
	{
		if (co==(3-TPref::TUniv.nMoveType) && bFeedPlane)
			oldB = pDC->SelectObject(&br3);
		else
			oldB = pDC->SelectObject(&br);
	}
	else
		oldB = pDC->SelectObject(&br2);

	oldP = pDC->SelectObject(&nullPen);
	CVector4 ptA[4];
	ptA[0] = CVector4(xv*min1,yv*min2,zv*((co==1)?min1:min2),1);
	ptA[1] = CVector4(xv*max1,yv*min2,zv*((co==1)?(max1):min2),1);
	ptA[2] = CVector4(xv*max1,yv*(max2),zv*((co==1)?(max1):(max2)),1);
	ptA[3] = CVector4(xv*min1,yv*max2,zv*((co==1)?min1:(max2)),1);

	CPoint Pt[4];
	for (k=0;k<4;k++)
		Pt[k] = (CPoint) ProjectPoint(ptA[k]);

	int  R = pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(Pt,4);
	pDC->SetPolyFillMode(R);
	//pDC->RestoreObjects();

	pDC->SelectObject(condvisu ? &frontPen : &backPen);
	for (i=min1;i<(max1+1);i++)
	 {
		CVector4 ptA[2];
		ptA[0] = CVector4(xv*i,yv*min2,zv*((co==1)?i:min2),1);
		ptA[1] = CVector4(xv*i,yv*(max2),zv*((co==1)?i:(max2)),1);
		CPoint Pt[2];
		for (k=0;k<2;k++)
			Pt[k] = (CPoint) ProjectPoint(ptA[k]);
		pDC->MoveTo(Pt[0]);
		pDC->LineTo(Pt[1]);
	 }
	for (j=min2;j<(max2+1);j++)
	 {
		CVector4 ptA[2];
		ptA[0] = CVector4(xv*min1,yv*j,zv*((co==1)?min1:j),1);
		ptA[1] = CVector4(xv*(max1),yv*j,zv*((co==1)?(max1):j),1);
		CPoint Pt[2];
		for (k=0;k<2;k++)
			Pt[k] = (CPoint) ProjectPoint(ptA[k]);
		pDC->MoveTo(Pt[0]);
		pDC->LineTo(Pt[1]);
	 }
	if (oldP) pDC->SelectObject(oldP);
	if (oldB) pDC->SelectObject(oldB);
}

void CVisuPlane::Draw(CDC* pDC)
{
	int max = TPref::TUniv.nRepPas;
	DrawOnePLane(pDC,-max,max,-max,max,3);
}

void CVisuPlane::DrawFeedBack(CDC* pDC,CPoint3D* pt)
{
	CPoint		oldF[2];
	CVector4 T= pt->Concept_pt;
	if ((int)T.z)
	{
		CVector4 M[2] = { CVector4(T.x,T.y,0,1),
						  CVector4(T.x,T.y,T.z,1)};
		for (int i=0;i<2;i++)
		 {	
			M[i] = ProjectPoint(M[i]);
			//oldF[i].x = M[i].x ;//- (int) Parent->Scroller->XPos;
			//oldF[i].y = M[i].y ;//- (int) Parent->Scroller->YPos;
			oldF[i] = (CPoint)M[i];
		 }
		COLORREF curC=RGB(0,0,0),
				 curC2=RGB(192,192,192);	
		CPen curP(PS_DOT,1,curC),
			 curP2(PS_DOT,1,curC2);
		CBrush br2(curC2);
		CBrush br(curC);
		CPen *oldP = pDC->SelectObject((T.z > 0.) ? &curP : &curP2);
		pDC->MoveTo(oldF[1].x,oldF[1].y);
		pDC->LineTo(oldF[0].x,oldF[0].y);

		pDC->SelectObject(oldP );
		CBrush *oldB = pDC->SelectObject(&br);
		CRect mrect(CPoint(oldF[0].x,oldF[0].y)-CSize(1,1),CSize(3,3));
		pDC->Rectangle(mrect);
		pDC->SelectObject(oldB);
	}
}

//////////////////////////////////////////////////////////////////////
// CVisuCloison Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVisuCloison::CVisuCloison(): 
	CVisuPlane()
{
	nVisuKind = VisuClois;	
}

CVisuCloison::CVisuCloison(const CVisualParam& param): 
	CVisuPlane(param)
{
	nVisuKind = VisuClois;	
}

bool CVisuCloison::IsPointVisible(CPoint3D* pt)
{
	return IsPointVisible(pt->Concept_pt);
}

bool CVisuCloison::IsPointVisible(CVector4 pt)
{
	return  (	(pt.x >= -MRG_ZERO) && (pt.y >= -MRG_ZERO) &&
					(pt.z >= -MRG_ZERO));
}

void CVisuCloison::DrawLineRep(CVector4 ptA[],CDC *pDC)
{
	const int r=4;
	CPoint Pt[r];
	for (int k=0;k<r;k++)
		Pt[k] = (CPoint) ProjectPoint(ptA[k]);
	CPen	frontPen(PS_SOLID,1,RGB(0,0,0)),
			frontPen2(PS_SOLID,2,RGB(255,0,0)),
			*oldP=NULL;
	for (k=1;k<r;k++)
	 {
		if (k==(3-TPref::TUniv.nMoveType) && bFeedPlane)
			oldP = pDC->SelectObject(&frontPen2);
		else
			oldP = pDC->SelectObject(&frontPen);
		pDC->MoveTo(Pt[0]);
		pDC->LineTo(Pt[k]);
		if (oldP) pDC->SelectObject(oldP);
	 }
}


void CVisuCloison::DrawScale(int order[],CDC *pDC)
{
	CVector4 ptA[4] = {
			CVector4(0,0,0,1),	CVector4(TPref::TUniv.nUnitRep*(TPref::TUniv.nRepPas+2),0,0,1),
			CVector4(0,TPref::TUniv.nUnitRep*(TPref::TUniv.nRepPas+2),0,1),
			CVector4(0,0,TPref::TUniv.nUnitRep*(TPref::TUniv.nRepPas/2+2),1)};


			TPref::TMoveType gg = (TPref::TMoveType)2;
	for (int i=0;i<6;i++)
	 switch(order[i])
	 {	case 1 :	//DrawFond(hdc);break;
			DrawOnePLane(pDC,-1,TPref::TUniv.nRepPas,-1,TPref::TUniv.nRepPas,3);	
			break;
		case 2 : //DrawGauche(hdc);break;
			DrawOnePLane(pDC,0,TPref::TUniv.nRepPas,0,TPref::TUniv.nRepPas/2,2);
			break;
		case 3 : //DrawArriere(hdc);break;
			DrawOnePLane(pDC,0,TPref::TUniv.nRepPas/2,0,TPref::TUniv.nRepPas,1);
			break;
		case 4 : // lines
			DrawLineRep(ptA,pDC);	break;
		case 5 : // Small Gauche
			DrawOnePLane(pDC,-1,0,0,TPref::TUniv.nRepPas,1);	break;
		case 6 : // small Droit
			DrawOnePLane(pDC,0,TPref::TUniv.nRepPas,-1,0,2);	break;
		default:	break;
	 }
}


void CVisuCloison::Draw(CDC* pDC)
{
	int TabOrd[16][6] = {
			{5,6,1,2,3,4},{6,5,1,2,4,3},{5,6,1,4,2,3},{5,6,1,3,4,2},
			{6,5,1,4,2,3},{5,6,1,3,4,2},{6,5,1,2,3,4},{6,5,1,4,2,3},
			{4,2,3,1,6,5},{3,4,2,1,5,6},{3,2,4,1,5,6},{2,4,3,1,6,5},
			{2,3,4,1,5,6},{2,4,3,1,6,5},{4,2,3,1,5,6},{3,4,2,1,5,6}
		};

	int i = ((int)ProjParam.theta)/90;
	int j = ((int)ProjParam.phi)/90;
	int k = i+j*4;
	DrawScale(TabOrd[k],pDC);
/*	if (CalcRgn)
	 {
		Vector4 ptA[4] = {
			Vector4(0,0,0,1),
			Vector4(0,0,TPref::UnitRep*(TPref::NbRepPas/2+2),1),
			Vector4(TPref::UnitRep*(TPref::NbRepPas+2),0,0,1),
			Vector4(0,TPref::UnitRep*(TPref::NbRepPas+2),0,1),
		 };
		DefineRegions(Parent,ptA,4,hdc,*this);
	 }*/
}

void CVisuCloison::DrawFeedBack(CDC* pDC,CPoint3D* pt)
{
	CPoint 		oldF[4];
	CVector4 T= pt->Concept_pt;
	CVector4 M[4] = { CVector4(T.x,0,T.z,1),
						  CVector4(0,T.y,T.z,1),
						  CVector4(T.x,T.y,0,1),
						  CVector4(T.x,T.y,T.z,1)};
	for (int i=0;i<4;i++)
		oldF[i] = (CPoint) ProjectPoint(M[i]);

	COLORREF	curC = RGB(0,0,0),
				curC2 = RGB(192,192,192);
	CPen	curP(PS_DOT,1,curC); 
	CBrush	br(curC),
			br2(curC2);
	CPen	curP2(PS_DOT,1,curC2);

	//DragDC.SelectObject(curP);
	//CPen *oldP = NULL;
	//CPen *oldP = pDC->SelectObject(&curP2);

	if ((int)T.y)
	{	
		CPen *oldP = (CPen *)pDC->SelectObject((T.y > 0.) ? &curP : &curP2);
		pDC->MoveTo(oldF[3]);
		pDC->LineTo(oldF[0]);
		pDC->SelectObject(oldP);
	}
	if ((int)T.x)
	 {	
		CPen *oldP = (CPen *)pDC->SelectObject((T.x > 0.) ? &curP : &curP2);
		pDC->MoveTo(oldF[3]);
		pDC->LineTo(oldF[1]);
		pDC->SelectObject(oldP);
	 }
	if ((int)T.z)
	 {	
		CPen *oldP = (CPen *)pDC->SelectObject((T.z > 0.) ? &curP : &curP2);
		pDC->MoveTo(oldF[3]);
		pDC->LineTo(oldF[2]);
		pDC->SelectObject(oldP);
	 }

	//pDC->SelectObject(oldP);
	CBrush *oldB = pDC->SelectObject(&br);
	if ((int)T.y)	
	{
		CRect mrect(oldF[0]-CSize(1,1),CSize(3,3));
		pDC->Rectangle(mrect);
	}
	if ((int)T.x)
	{
		CRect mrect(oldF[1]-CSize(1,1),CSize(3,3));
		pDC->Rectangle(mrect);
	}

	if ((int)T.z)
	{
		CRect mrect(oldF[2]-CSize(1,1),CSize(3,3));
		pDC->Rectangle(mrect);
	}
	pDC->SelectObject(oldB);

}
