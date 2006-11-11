// Shape.cpp: implementation of the CShape2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../calques3d.h"
#include "../Prefs/Prefs.h"
#include "Shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Definition de TShape
////////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CShape, CObject, VERSIONABLE_SCHEMA | 1)

CShape::CShape(int nType,int id)
{
	nShapeId = id;
	//pObj =obj;
	clrObject = (COLORREF)TPref::Color;
	SetShapeType(nType);
}

CShape::CShape(CShape& src)
{
	nShapeId = src.nShapeId;
	//pObj =src.pObj;
	clrObject = src.clrObject;
	SetShapeType(src.nShapeType);
}


void CShape::operator =(const CShape& other)
{
	//pObj =other.pObj;
	clrObject = other.clrObject;
	nShapeId = other.nShapeId;
	nShapeNb = other.nShapeNb;
	SetShapeType(other.nShapeType);
}

void CShape::SetShapeType(int nType)
{
	nShapeType = nType;
	if (nType==LineShape)
	{
		nShapeNb = 8;
		nShapeDef = 4;
	}
	else if (nType==PointShape)
	{
		nShapeNb = 6;
		nShapeDef = 0;
	}
	else if (nType==PolShape)
	{
		nShapeNb = 10;
		nShapeDef = 1;
	}
	else
	{
		nShapeNb = 3;
		nShapeDef = 0;
	}
	if ((nShapeId <0) || (nShapeId >= nShapeNb))
		nShapeId = nShapeDef; 
}

CString CShape::GetShapeDef(int type,int index)
{
	CShape temp(type);
	return temp.GetShapeDef(index);
}

CString CShape::GetShapeDef(int i)
{
	if (i==-1) i = nShapeDef;
	CString mstr;
	if (nShapeType==LineShape)
		mstr.LoadString(IDS_LINESHAPE+i);
	else if (nShapeType==PointShape)
		mstr.LoadString(IDS_POINTSHAPE+i);
	else if (nShapeType==PolShape)
		mstr.LoadString(IDS_POLSHAPE+i);
	else
		mstr.LoadString(IDS_VOLSHAPE+i);
	return mstr;
}

LOGPEN  CShape::GetHiddenPenStyle(COLORREF clr)
{
	const int PenStyle[8][2] =
		{
			{1,PS_DOT},
			{1,PS_DASH},
			{1,PS_DASHDOT},
			{1,PS_DASHDOTDOT},
			{1,PS_SOLID},
			{2,PS_SOLID},
			{3,PS_SOLID},
			{4,PS_SOLID},
		};
	CPen shapeP(PenStyle[nShapeId][1],PenStyle[nShapeId][0],clr);
	LOGPEN pLogPen;
	shapeP.GetLogPen(&pLogPen);
	return pLogPen;
}

LOGPEN  CShape::GetPenStyle(int nb/*=-1*/)
{
	const int PenStyle[8][2] =
		{
			{1,PS_DOT},
			{1,PS_DASH},
			{1,PS_DASHDOT},
			{1,PS_DASHDOTDOT},
			{1,PS_SOLID},
			{2,PS_SOLID},
			{3,PS_SOLID},
			{4,PS_SOLID},
		};

	int nIdx = (nb==-1) ? nShapeId : nb;
	CPen shapeP(PenStyle[nIdx][1],PenStyle[nIdx][0],clrObject);
	LOGPEN pLogPen;
	shapeP.GetLogPen(&pLogPen);
	return pLogPen;
}

COLORREF CShape::GetObjectColor()
{
	return clrObject;
}

COLORREF CShape::GetObjectHiddenColor()
{
	int m = -1;
	COLORREF ObjCl = GetObjectColor();
	for (int g=0;g<8;g++)
		if (TPref::custColors[g] == ObjCl)
			m = g+8;
	return ((TPref::GrayedHidden || (m==-1)) ?
			RGB(192,192,192) : TPref::custColors[m]);
	return RGB(192,192,192);
}

int	CShape::GetColorIdx()
{
	int Idx = -1;
	COLORREF ObjCl = GetObjectColor();
	for (int g=0;g<8;g++)
	{
		if (TPref::custColors[g] == ObjCl)
			return g;
	}
	return Idx;
}

void CShape::Draw(CDC *pDC,CRect mrect)
{
	/*if (nShapeType==LineShape)
		DrawLine(pDC,mrect);
	else if (nShapeType==PointShape)
		DrawPoint(pDC,mrect);
	else
		DrawObj(pDC,mrect);*/
if (nShapeType==PointShape)
		DrawPoint(pDC,mrect);
}

/*void CShape::DrawLine(CDC *pDC,CRect mrect)
{
	CSize	s = mrect.Size();
	CPoint 	theP1(mrect.left + 3,mrect.top + s.cy /2),
			theP2(mrect.right -3,mrect.top + s.cy /2);

	COLORREF shapeC = RGB(255,255,255);
	if ((nShapeId <0) || (nShapeId >= 8)) nShapeId = 4;
	LOGPEN pLogPen = GetPenStyle();
	CPen shapeP;
	shapeP.CreatePenIndirect(&pLogPen);
	CPen *olpP = pDC->SelectObject(&shapeP);
	pDC->MoveTo(theP1);
	pDC->LineTo(theP2);

	if (olpP) pDC->SelectObject(olpP);
}*/

void CShape::DrawPoint(CDC *pDC,CRect mrect)
{
	CSize	s = mrect.Size();
	CPoint theP(mrect.left + s.cx / 2,mrect.top + s.cy /2);
	int Ds = TPref::TPoint.nSize;

	COLORREF shapeC = RGB(255,255,255);
	CBrush	shapeB1(shapeC),
			*oldB = NULL;

	switch (nShapeId)
	 {	case 1 : // rond vide opaque
			oldB = pDC->SelectObject(&shapeB1);
			pDC->Ellipse(theP.x-Ds+1, theP.y-Ds+1, theP.x+Ds, theP.y+Ds);
			break;
		case 2 :	// carre plein
			//hdc.SelectObject(shapeB2);
			pDC->Rectangle(theP.x-Ds+1, theP.y-Ds+1, theP.x+Ds, theP.y+Ds);
			break;
		case 3 : // carre vide opaque
			oldB = pDC->SelectObject(&shapeB1);
			pDC->Rectangle(theP.x-Ds+1, theP.y-Ds+1, theP.x+Ds, theP.y+Ds);
			break;
		case 4 : // croix X
			//hdc.SelectObject(shapeB2);
			pDC->MoveTo(theP.x, theP.y);
			pDC->LineTo(theP.x-Ds, theP.y-Ds);
			pDC->MoveTo(theP.x, theP.y);
			pDC->LineTo(theP.x+Ds, theP.y-Ds);
			pDC->MoveTo(theP.x, theP.y);
			pDC->LineTo(theP.x+Ds, theP.y+Ds);
			pDC->MoveTo(theP.x, theP.y);
			pDC->LineTo(theP.x-Ds, theP.y+Ds);
			break;
		case 5 : // croix +
			//hdc.SelectObject(shapeB2);
			pDC->MoveTo(theP.x, theP.y);
			pDC->LineTo(theP.x-Ds, theP.y);
			pDC->MoveTo(theP.x, theP.y);
			pDC->LineTo(theP.x+Ds, theP.y);
			pDC->MoveTo(theP.x, theP.y);
			pDC->LineTo(theP.x, theP.y-Ds);
			pDC->MoveTo(theP.x, theP.y);
			pDC->LineTo(theP.x, theP.y+Ds);
			break;
		case 0 :	// rond plein
		default:
			//hdc.SelectObject(shapeB2);
			pDC->Ellipse(theP.x-Ds+1, theP.y-Ds+1, theP.x+Ds, theP.y+Ds);
			break;
	 }
	if (oldB) pDC->SelectObject(oldB);

}
/*void CShape::DrawObj(CDC*,CRect)
{
}*/


void CShape::Serialize( CArchive& ar )
{
	if (ar.IsStoring())
	{
		ar << nShapeType;
		ar << nShapeNb;
		ar << nShapeDef;
		ar << nShapeId;
		ar << clrObject;
		ar << nShapeId;
	}
	else
	{
		ar >> nShapeType;
		ar >> nShapeNb;
		ar >> nShapeDef;
		ar >> nShapeId;
		ar >> clrObject;
		ar >> nShapeId;
	}
}


void CShape::DrawShapes(int type,CDC* pDC, CRect rect,int nIndex)
{
	switch(type)
	{
		case TShapeType::PointShape:
			CShape::DrawShapesPt(pDC,rect,nIndex);
			break;
		case TShapeType::LineShape:
			CShape::DrawShapesLn(pDC,rect,nIndex);
			break;
		case TShapeType::VolShape:
			CShape::DrawShapesVol(pDC,rect,nIndex);
			break;
		case TShapeType::PolShape:
			CShape::DrawShapesPol(pDC,rect,nIndex);
			break;
		default:
			break;

	}
};

void CShape::DrawShapesPol(CDC* pDC, CRect rect,int nIndex)
{
	switch(nIndex)
	{
		case 0:	// Transparent
		case 1:	// Opaque
			CShape::DrawShapesVol(pDC, rect,nIndex);
			break;
		default:
			CShape::DrawShapesLn(pDC,rect,nIndex-2);


	}
}

void CShape::DrawShapesVol(CDC* pDC, CRect rect,int nIndex)
{
    rect.DeflateRect(1,1);
	int ww = min(rect.Height(),rect.Width());

	CPen pen,pen2;
	CBrush brush;
	if (!pen.CreatePen(PS_SOLID, 1, RGB(0,0,0))) return;
	if (!pen2.CreatePen(PS_DOT, 1, RGB(0,0,0))) return;
	if (!brush.CreateSolidBrush(RGB(0,0,0))) return;
	CPen *pOldPen = NULL; 
	CBrush *pOldBrush = NULL;

	CRect mrect(rect.left + (rect.Width() - ww*3)/2,
				rect.top+(rect.Height() - ww)/2,
				rect.left + (rect.Width() - ww*3)/2 + ww*3,
				rect.top+(rect.Height() - ww)/2 + ww);

	CRect mrect2(rect.left + (rect.Width() - ww*5)/2,
				rect.top+(rect.Height() - ww)/2,
				rect.left + (rect.Width() - ww*5)/2 + ww*2,
				rect.top+(rect.Height() - ww)/2 + ww);

	switch(nIndex){
	case 0:	// Transparent
		{
			pOldPen = pDC->SelectObject(&pen);
			pDC->Rectangle(mrect);

			pDC->MoveTo(rect.left,(rect.top+rect.bottom)/2-1);
			pDC->LineTo(rect.right, (rect.top+rect.bottom)/2-1);
			pDC->MoveTo(rect.left,(rect.top+rect.bottom)/2);
			pDC->LineTo(rect.right, (rect.top+rect.bottom)/2);
			break;
		}
	case 1: // Opaque
		{
			pOldPen = pDC->SelectObject(&pen);
			pDC->Rectangle(mrect);

			pDC->MoveTo(rect.left,(rect.top+rect.bottom)/2-1);
			pDC->LineTo(rect.left + (rect.Width() - ww*3)/2, (rect.top+rect.bottom)/2-1);
			pDC->MoveTo(rect.left + (rect.Width() - ww*3)/2 + ww*3,(rect.top+rect.bottom)/2-1);
			pDC->LineTo(rect.right, (rect.top+rect.bottom)/2-1);

			pDC->MoveTo(rect.left,(rect.top+rect.bottom)/2);
			pDC->LineTo(rect.left + (rect.Width() - ww*3)/2, (rect.top+rect.bottom)/2);
			pDC->MoveTo(rect.left + (rect.Width() - ww*3)/2 + ww*3,(rect.top+rect.bottom)/2);
			pDC->LineTo(rect.right, (rect.top+rect.bottom)/2);

			int oldM = pDC->SetBkMode(TRANSPARENT);
			pDC->SelectObject(&pen2);
			pDC->MoveTo(rect.left + (rect.Width() - ww*3)/2, (rect.top+rect.bottom)/2-1);
			pDC->LineTo(rect.left + (rect.Width() - ww*3)/2 + ww*3,(rect.top+rect.bottom)/2-1);
			pDC->MoveTo(rect.left + (rect.Width() - ww*3)/2, (rect.top+rect.bottom)/2);
			pDC->LineTo(rect.left + (rect.Width() - ww*3)/2 + ww*3,(rect.top+rect.bottom)/2);
			pDC->SetBkMode(oldM);
			break;
		}
	case 2: // Composite
		{
			pOldPen = pDC->SelectObject(&pen);
			pDC->Rectangle(mrect2);

			pDC->MoveTo(rect.left,(rect.top+rect.bottom)/2-1);
			pDC->LineTo(rect.right, (rect.top+rect.bottom)/2-1);
			pDC->MoveTo(rect.left,(rect.top+rect.bottom)/2);
			pDC->LineTo(rect.right, (rect.top+rect.bottom)/2);

			mrect2.OffsetRect(ww*3,0);
			pDC->Rectangle(mrect2);

			int oldM = pDC->SetBkMode(TRANSPARENT);
			pDC->SelectObject(&pen2);
			pDC->MoveTo(mrect2.left, (mrect2.top+mrect2.bottom)/2-1);
			pDC->LineTo(mrect2.right,(rect.top+rect.bottom)/2-1);
			pDC->MoveTo(mrect2.left, (mrect2.top+mrect2.bottom)/2);
			pDC->LineTo(mrect2.right,(rect.top+rect.bottom)/2);
			pDC->SetBkMode(oldM);

		    break;
		}
	}

	if (pOldPen)	pDC->SelectObject(pOldPen);
	if (pOldBrush)	pDC->SelectObject(pOldBrush);

};

void CShape::DrawShapesPt(CDC* pDC, CRect rect,int nIndex)
{
    rect.DeflateRect(2,2);
	int ww = min(rect.Height(),rect.Width())-2;

	CPen pen,pen2;
	CBrush brush;
	if (!pen.CreatePen(PS_SOLID, 1, RGB(0,0,0))) return;
	if (!pen2.CreatePen(PS_SOLID, 2, RGB(0,0,0))) return;
	if (!brush.CreateSolidBrush(RGB(0,0,0))) return;
	CPen *pOldPen = NULL; 
	CBrush *pOldBrush = NULL;

	CRect mrect(rect.left + (rect.Width() - ww)/2,
				rect.top+(rect.Height() - ww)/2,
				rect.left + (rect.Width() - ww)/2 + ww,
				rect.top+(rect.Height() - ww)/2 + ww);

	switch(nIndex){
	case 0:
		{
			pOldPen = pDC->SelectObject(&pen);
			pOldBrush = pDC->SelectObject(&brush);
			pDC->Ellipse(mrect);
			break;
		}
	case 1:
		{
			pOldPen = pDC->SelectObject(&pen);
			pDC->Ellipse(mrect);
			break;
		}
	case 2:
		{
			pOldPen = pDC->SelectObject(&pen);
			pOldBrush = pDC->SelectObject(&brush);
			pDC->Rectangle(mrect);
		    break;
		}
	case 3:
		{
			pOldPen = pDC->SelectObject(&pen);
			pDC->Rectangle(mrect);
		    break;
		}
	case 4:
		{
			pOldPen = pDC->SelectObject(&pen2);
			pDC->MoveTo(mrect.TopLeft());
			pDC->LineTo(mrect.BottomRight()+CSize(0,-1));
			pDC->MoveTo(mrect.right,mrect.top);
			pDC->LineTo(mrect.left,mrect.bottom-1);
		    break;
		}
	case 5:
		{
			pOldPen = pDC->SelectObject(&pen2);
			pDC->MoveTo(mrect.left-2,1+mrect.top+mrect.Height()/2-1);
			pDC->LineTo(mrect.right,1+mrect.top+mrect.Height()/2-1);
			pDC->MoveTo(mrect.left+mrect.Width()/2,mrect.top);
			pDC->LineTo(mrect.left+mrect.Width()/2,mrect.bottom-1);
		    break;
		}
	}

	if (pOldPen)	pDC->SelectObject(pOldPen);
	if (pOldBrush)	pDC->SelectObject(pOldBrush);

};

void CShape::DrawShapesLn(CDC* pDC, CRect rect,int nIndex)
{
//	CRect rectColor = rect;
	int nStyle[]={PS_DOT,PS_DASH,PS_DASHDOT,PS_DASHDOTDOT,PS_SOLID,PS_SOLID,PS_SOLID,PS_SOLID};
	int nSize[]={1,1,1,1,1,2,3,4};
	CPen pen;
	CBrush brush;
	//int ww = min(rect.Height(),rect.Width())-2;
	if (!pen.CreatePen(nStyle[nIndex], nSize[nIndex], RGB(0,0,0))) return;
	if (!brush.CreateSolidBrush(RGB(0,0,0))) return;
	CPen *pOldPen = NULL; 
	CBrush *pOldBrush = NULL;
	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = pDC->SelectObject(&brush);
	int mode = pDC->GetBkMode();
	pDC->SetBkMode(TRANSPARENT   );

	//#rect.DeflateRect(2,2);

	pDC->MoveTo(rect.left, (rect.top+rect.bottom)/2 -1);
	pDC->LineTo(rect.right, (rect.top+rect.bottom)/2 -1);
	pDC->MoveTo(rect.left, (rect.top+rect.bottom)/2);
	pDC->LineTo(rect.right, (rect.top+rect.bottom)/2);
	pDC->MoveTo(rect.left, (rect.top+rect.bottom)/2 +1);
	pDC->LineTo(rect.right, (rect.top+rect.bottom)/2 +1);

	pDC->SetBkMode(mode   );
	if (pOldPen)	pDC->SelectObject(pOldPen);
	if (pOldBrush)	pDC->SelectObject(pOldBrush);

}