// IconCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "IconCtrl.h"

#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconCtrl
IMPLEMENT_DYNCREATE(CIconCtrl, CWnd)

CIconCtrl::CIconCtrl()
{
	m_nImage = -1;
    RegisterWindowClass();
}

CIconCtrl::~CIconCtrl()
{
}

// Register the window class if it has not already been registered.
BOOL CIconCtrl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, ICONCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        //wndcls.hbrBackground    = (HBRUSH) (-1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = ICONCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BEGIN_MESSAGE_MAP(CIconCtrl, CWnd)
	//{{AFX_MSG_MAP(CIconCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIconCtrl message handlers

void CIconCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC MemDC(&dc);
	OnDraw(&MemDC);
}

BOOL CIconCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

void CIconCtrl::OnDraw(CDC* pDC)
{
    CRect clipRect;
    if (pDC->GetClipBox(&clipRect) == ERROR)
        return;

	//CBrush mBR(RGB(0,0,0));
	//GetClientRect(clipRect);
	//pDC->FillRect(clipRect,&globalData.brBtnFace);
	//pDC->DrawEdge(clipRect,EDGE_SUNKEN,BF_RECT);
		globalData.DrawParentBackground (this, pDC);

	CSize ms = m_cImage.GetImageSize();
	CSize ms2 = clipRect.Size();

	ms2 = ms2 - ms;

	if (m_nImage==-1) return;
	CBCGPDrawState ds;
	m_cImage.PrepareDrawImage (ds);
	m_cImage.Draw(pDC,clipRect.TopLeft().x+ms2.cx/2,clipRect.TopLeft().y+ms2.cy/2,m_nImage);
	m_cImage.EndDrawImage (ds);

}
