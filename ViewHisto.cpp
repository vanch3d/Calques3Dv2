// ViewHisto.cpp : implementation file
//

#include "stdafx.h"
#include "Calques3D.h"

#include "Calques3DDoc.h"

#include "ViewHisto.h"
#include "objects\Text3D.h"
#include "objects\CompositeObj3D.h"

#include "Prefs\Prefs.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewHisto

IMPLEMENT_DYNCREATE(CViewHisto, CTreeView)//CListView)

CViewHisto::CViewHisto()
{
	m_ImageList.Create(IDB_VAR_ICONS,14,0,RGB(255,255,255));
	m_bRefit = FALSE;
}

CViewHisto::~CViewHisto()
{
	int nb = GetDocument()->m_cObjectSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
		if (!pObj) continue;
		//if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
		pObj->ClearHistorique();
	}

}


BEGIN_MESSAGE_MAP(CViewHisto, CTreeView)//CListView)
	//{{AFX_MSG_MAP(CViewHisto)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelChanging)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HISTORY_EXPANDALL, OnExpandAll)
	ON_COMMAND(ID_HISTORY_COLLAPSEALL, OnCollapseAll)
	ON_COMMAND(ID_HISTORY_EXPORTSYMBOLIC, OnHistoryExport)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemExpanded)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewHisto drawing

void CViewHisto::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CViewHisto diagnostics

#ifdef _DEBUG
void CViewHisto::AssertValid() const
{
	CTreeView::AssertValid();
}

void CViewHisto::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CCalques3DDoc* CViewHisto::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalques3DDoc)));
	return (CCalques3DDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewHisto message handlers

void CViewHisto::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//CListCtrl& mListCtrl = GetListCtrl();
	CTreeCtrl& mListCtrl = GetTreeCtrl();

	if (mListCtrl)
	{
		DWORD dwStyle = mListCtrl.GetStyle();
		//dwStyle |= /*WS_CHILD | WS_VISIBLE| */LVS_LIST | /*LVS_AUTOARRANGE | /*LVS_ALIGNTOP | /*LVS_ALIGNLEFT | */
		//		LVS_SINGLESEL | LVS_SHOWSELALWAYS ;
		dwStyle |= TVS_HASLINES | TVS_LINESATROOT |  TVS_HASBUTTONS | TVS_SHOWSELALWAYS ;
		::SetWindowLong(mListCtrl.m_hWnd,GWL_STYLE,dwStyle);
		//DWORD dwExtStyle = mListCtrl.GetExStyle();
		//dwExtStyle |= LVS_EX_INFOTIP | TVS_FULLROWSELECT   ;
		//mListCtrl.SetExtendedStyle(dwExtStyle);
		mListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
		int nb = GetDocument()->m_cObjectSet.GetSize();
		for (int i=0;i<nb;i++)
		{
			CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
			if (!pObj) continue;
			//if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;

			pObj->DrawHistorique(mListCtrl);

/*			CString mstr = pObj->GetObjectDef();

			TV_INSERTSTRUCT     curTreeItem;

			curTreeItem.hParent = TVI_ROOT;
			curTreeItem.hInsertAfter = TVI_LAST;
			curTreeItem.item.mask = TVIF_TEXT | TVIF_PARAM;
			if (!pObj->bVisible || !pObj->bValidate) 
			{
				curTreeItem.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE ;
				curTreeItem.item.iImage = (!pObj->bVisible) ? 2 : 1;
				curTreeItem.item.iSelectedImage = (!pObj->bVisible) ? 2 : 1;
				//curTreeItem.item.iSelectedImage = curTreeItem.item.iImage + 1;
			}
			curTreeItem.item.pszText = mstr.GetBuffer(mstr.GetLength());
			curTreeItem.item.lParam = (LPARAM)pObj;

			//pObj->CalculVisuel(GetVisualParam());
			//pObj->Draw(pDC,GetVisualParam());
			HTREEITEM ppTree =mListCtrl.InsertItem(&curTreeItem);
			if (ppTree)
				pObj->pHistItem = ppTree;*/
		}
	}
}

void CViewHisto::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
//	BOOL bRedraw = TRUE;
	switch (lHint)
	{
	case WM_UPDATEOBJ_ADD:	// Object Added
		{
			CTreeCtrl& mListCtrl = GetTreeCtrl();
			if (!mListCtrl) break;
			m_bRefit = TRUE;
			SetRedraw(FALSE);
			CObject3D *pObj=NULL;
			pObj = DYNAMIC_DOWNCAST(CObject3D,pHint);
			//if (!pObj) break;
			pObj->DrawHistorique(mListCtrl);
			m_bRefit = FALSE;
			SetRedraw(TRUE);
		}
		break;
	case WM_UPDATEOBJ_MOD:	// Object Modified
		{
			CTreeCtrl& mListCtrl = GetTreeCtrl();
			if (!mListCtrl) break;
			CObject3D *pObj = DYNAMIC_DOWNCAST(CObject3D,pHint);
			if (!pObj) break;
			pObj->DrawHistorique(mListCtrl);

			CxObject3DSet mySet;
			pObj->GetDependList(&mySet);

			int nb = mySet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D* pObj2 = mySet.GetAt(i);
				if (!pObj2) continue;

				pObj2->DrawHistorique(mListCtrl);
			}
		}
		break;
	case WM_UPDATEOBJ_MOV:	// Object Moved
		{
			CTreeCtrl& mListCtrl = GetTreeCtrl();
			if (!mListCtrl) break;
			CObject3D *pObj2 = DYNAMIC_DOWNCAST(CObject3D,pHint);
			if (!pObj2) break;

			CxObject3DSet plist;
			pObj2->GetDependList(&plist);
			for (int i=0;i<plist.GetSize();i++)
			{
				CObject3D* pObj = plist[i];
				if (!pObj) continue;
				if (pObj->pComposite) continue;

				pObj->DrawHistorique(mListCtrl);
			}
		}
		break;
	case WM_UPDATEOBJ_SEL:	// Object Selected
		{
			CTreeCtrl& mListCtrl = GetTreeCtrl();
			if (!mListCtrl) break;

			CObject3D *pObj2 = DYNAMIC_DOWNCAST(CObject3D,pHint);

			//CEquation3D *pEqu = DYNAMIC_DOWNCAST(CEquation3D,pObj2);
			//if (pEqu)
			{
				//pObj2 = pEqu->pSource;
			}

			HTREEITEM pItem = NULL;
			if (pObj2 && pObj2->pHistItem)
				pItem = pObj2->pHistItem;

			m_bRefit = TRUE;
			mListCtrl.Select(pItem,TVGN_CARET);
			m_bRefit = FALSE;
		}
		break;
	case WM_UPDATEOBJ_DEL:	// Object Deleted
		{
			CTreeCtrl& mListCtrl = GetTreeCtrl();
			if (!mListCtrl) break;
			SetRedraw(FALSE);


			CObject3D *pObj2 = DYNAMIC_DOWNCAST(CObject3D,pHint);
			if (pObj2 && pObj2->pHistItem)
			{
				mListCtrl.DeleteItem(pObj2->pHistItem);
				pObj2->ClearHistorique();
			}

			int nb = GetDocument()->m_cObjectSet.GetSize();
			for (int i=0;i<nb;i++)
			{
				CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
				//if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
				if (!pObj) continue;

				pObj->DrawHistorique(mListCtrl);
			}
			SetRedraw(TRUE);
		}
	default:
//		bRedraw = FALSE;
		break;
	}
//	if (bRedraw)
//	{
//		Invalidate();
//		UpdateWindow();
//	}
}

void CViewHisto::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bRefit) return;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
//	TVITEM pOld = pNMTreeView->itemOld;
	TVITEM pNew = pNMTreeView->itemNew;

	BOOL bRedraw = FALSE;
	CObject3D* pObj = NULL;

	if (pNew.hItem)
	{
		pObj = (CObject3D*)pNew.lParam;
		if (pObj)
		{
		//	pObj->bIsSelected = TRUE;
			bRedraw = TRUE;
		}
	}
	
	if (bRedraw)
		GetDocument()->UpdateAllViews(this,WM_UPDATEOBJ_SEL,pObj);

	*pResult = 0;
}

void CViewHisto::OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bRefit) return;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
/*	
	BOOL bRedraw = FALSE;
	CObject3D* pObj =NULL;
	TVITEM pNew = pNMTreeView->itemOld;
	if (pNew.hItem)
	{
		pObj = (CObject3D*)pNew.lParam;
		if (pObj)
		{
			//pObj->bIsSelected = FALSE;
			bRedraw = TRUE;
		}

	}
	
	if (bRedraw)
		GetDocument()->UpdateAllViews(this,WM_UPDATEOBJ_MOD,pObj);
*/

	*pResult = 0;
}

void CViewHisto::OnKillFocus(CWnd* pNewWnd) 
{
	CTreeView::OnKillFocus(pNewWnd);
	if (m_bRefit) return;
	
	// TODO: Add your message handler code here
/*	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;

	HTREEITEM pItem = mListCtrl.GetSelectedItem();
	if (!pItem) return;

	CObject3D* pObj = (CObject3D*)(mListCtrl.GetItemData(pItem));
	if (!pObj) return;

	pObj->bIsSelected = FALSE;
	GetDocument()->UpdateAllViews(this,WM_UPDATEOBJ_MOD,pObj);*/
}

void CViewHisto::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeView::OnSetFocus(pOldWnd);
	if (m_bRefit) return;
	
/*	// TODO: Add your message handler code here
	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;

	HTREEITEM pItem = mListCtrl.GetSelectedItem();
	if (!pItem) return;

	CObject3D* pObj = (CObject3D*)(mListCtrl.GetItemData(pItem));
	if (!pObj) return;

	pObj->bIsSelected = TRUE;
	GetDocument()->UpdateAllViews(this,WM_UPDATEOBJ_MOD,pObj);*/
}

void CViewHisto::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bRefit) return;
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;

	HTREEITEM pItem = mListCtrl.GetSelectedItem();
	if (!pItem) return;

	CObject3D* pObj = (CObject3D*)(mListCtrl.GetItemData(pItem));
	if (!pObj) return;

	if (pObj->bValidate)
		GetDocument()->ModifyPropObject(pObj);
	else
	{
		int rr = pObj->CalculConceptuel();
		pObj->HandleObjectError(rr,TRUE);
	}
	//pObj->SetProperties();
	*pResult = 1;
}

void CViewHisto::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	SetRedraw(FALSE);
	*pResult = 0;
}

void CViewHisto::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	SetRedraw(TRUE);
	*pResult = 0;
}

void CViewHisto::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	*pResult = 0;
}

void CViewHisto::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}


void CViewHisto::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVCUSTOMDRAW  *pcd = (NMTVCUSTOMDRAW  *)pNMHDR;
	HTREEITEM               hitem;
         
	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;

	switch ( pcd->nmcd.dwDrawStage )
	{
		case CDDS_PREPAINT: 
			*pResult = CDRF_NOTIFYITEMDRAW;         
			return;
		case CDDS_ITEMPREPAINT : 
			hitem = (HTREEITEM)pcd->nmcd.dwItemSpec;
			NMCUSTOMDRAW mcd = pcd->nmcd;
			CObject3D* pObj = (CObject3D*)mListCtrl.GetItemData(hitem);
			//#define CDIS_SELECTED       0x0001
			//#define CDIS_GRAYED         0x0002
			//#define CDIS_DISABLED       0x0004
			//#define CDIS_CHECKED        0x0008
			//#define CDIS_FOCUS          0x0010
			//#define CDIS_DEFAULT        0x0020
			//#define CDIS_HOT            0x0040
			//#define CDIS_MARKED         0x0080
			//#define CDIS_INDETERMINATE  0x0100

			if (mcd.hdc)
			{
				CDC ppDC;
				CDC *pDC = ppDC.FromHandle(mcd.hdc);

				if (pDC)
				{
					CFont *mFont = pDC->GetCurrentFont();
				}

			}
			//SelectObject(pcd->nmcd.hdc, hfnt);
			if (pObj && !pObj->bValidate)
			{
					//if (!(mcd.uItemState & CDIS_SELECTED))
				pcd->clrText = RGB(255,0,0);
				if (mcd.uItemState & CDIS_FOCUS)
					pcd->clrText = RGB(255,128,128);
					//pcd->clrTextBk =RGB(255,0,0);
			}
			if (pObj && !pObj->bVisible)
			{
				pcd->clrText = RGB(128,128,128);
			}
			*pResult = CDRF_DODEFAULT;// do not set *pResult = CDRF_SKIPDEFAULT
			return;
	}   
}

#define LEFT_MARGIN 4
#define RIGHT_MARGIN 4
#define TOP_MARGIN 4
#define BOTTOM_MARGIN 4


void CViewHisto::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	CTreeView::OnPrint(pDC, pInfo);
	       // Save dc state
	int nSavedDC = pDC->SaveDC();

	// Set font
	CFont Font;
	LOGFONT lf;
	CFont *pOldFont = GetFont();
	pOldFont->GetLogFont(&lf);
	lf.lfHeight=m_nRowHeight-1;
	lf.lfWidth=0;
	Font.CreateFontIndirect(&lf);
	pDC->SelectObject(&Font);

//	PrintHeadFoot(pDC,pInfo);
	pDC->SetWindowOrg(-1*(LEFT_MARGIN*m_nCharWidth),-m_nRowHeight*TOP_MARGIN);

	int height;
	if (pInfo->m_nCurPage==pInfo->GetMaxPage())
	        height=rcBounds.Height()-((pInfo->m_nCurPage-1)*m_nRowsPerPage*m_nRowHeight);
	else
	        height=m_nRowsPerPage*m_nRowHeight;
	int top=(pInfo->m_nCurPage-1)*m_nRowsPerPage*m_nRowHeight;

	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(0,0,0));

	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	int nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 1 << lpbi->biBitCount;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB;
	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
	        lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + 
	                bmInfo.bmiHeader.biClrUsed) + 
	                ((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
	        lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	HDC hDC=pDC->GetSafeHdc();
	StretchDIBits(hDC,			// hDC
	        0,					// DestX
	        0,					// DestY
	        rcBounds.Width(),	// nDestWidth
	        height,				// nDestHeight
	        rcBounds.left,		// SrcX
	        rcBounds.Height()-top-height,	// SrcY
	        rcBounds.Width(),	// wSrcWidth
	        height,				// wSrcHeight
	        lpDIBBits,			// lpBits
	        &bmInfo,			// lpBitsInfo
	        DIB_RGB_COLORS,		// wUsage
	        SRCCOPY);			// dwROP

	pDC->SelectObject(pOldFont);
	pDC->RestoreDC( nSavedDC );
}

BOOL CViewHisto::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return DoPreparePrinting(pInfo);
	//return CTreeView::OnPreparePrinting(pInfo);
}




void CViewHisto::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;

	CTreeCtrl* Tree = &mListCtrl ;

	HTREEITEM hItem=Tree->GetRootItem();
	Tree->GetItemRect(hItem,rcBounds,TRUE);
    m_nRowHeight = rcBounds.Height();

    // Find the total number of visible items & the right most coordinate
    int ItemCount=0;
    do
	{
		ItemCount++;
		CRect rc;
		Tree->GetItemRect(hItem,rc,TRUE);
		if (rc.right>rcBounds.right)
		        rcBounds.right=rc.right;
		hItem=Tree->GetNextItem(hItem,TVGN_NEXTVISIBLE);
	}
	while (hItem);

	// Find the entire print boundary
	int ScrollMin,ScrollMax;
	GetScrollRange(SB_HORZ,&ScrollMin,&ScrollMax);
	rcBounds.left=0;
	if (ScrollMax>rcBounds.right)
		rcBounds.right=ScrollMax+1;
	rcBounds.top=0;
	rcBounds.bottom=m_nRowHeight*ItemCount;

	// Get text width
	CDC *pCtlDC = Tree->GetDC();
	if (NULL == pCtlDC) return;
	TEXTMETRIC tm;
	pCtlDC->GetTextMetrics(&tm);
	m_nCharWidth = tm.tmAveCharWidth;
	double d = (double)pDC->GetDeviceCaps(LOGPIXELSY)/(double)pCtlDC->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(pCtlDC);

	// Find rows per page
	int nPageHeight = pDC->GetDeviceCaps(VERTRES);
	m_nRowsPerPage = (int)((double)nPageHeight/d)/m_nRowHeight-TOP_MARGIN-BOTTOM_MARGIN;

	// Set maximum pages
	int pages=(ItemCount-1)/m_nRowsPerPage+1;
	pInfo->SetMaxPage(pages);

	// Create a memory DC compatible with the paint DC
	CPaintDC dc(this);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// Select a compatible bitmap into the memory DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rcBounds.Width(), rcBounds.Height() );
	MemDC.SelectObject(&bitmap);

	// Enlarge window size to include the whole print area boundary
	GetWindowPlacement(&WndPlace);
	MoveWindow(0,0,::GetSystemMetrics(SM_CXEDGE)*2+rcBounds.Width(),
		::GetSystemMetrics(SM_CYEDGE)*2+rcBounds.Height(),FALSE);
	ShowScrollBar(SB_BOTH,FALSE);

	// Call the default printing
	Tree->EnsureVisible(Tree->GetRootItem());
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)MemDC.m_hDC, 0 );

	// Now create a mask
	CDC MaskDC;
	MaskDC.CreateCompatibleDC(&dc);
	CBitmap maskBitmap;

	// Create monochrome bitmap for the mask
	maskBitmap.CreateBitmap( rcBounds.Width(), rcBounds.Height(), 1, 1, NULL );
	MaskDC.SelectObject( &maskBitmap );
	MemDC.SetBkColor( ::GetSysColor( COLOR_WINDOW ) );

	// Create the mask from the memory DC
	MaskDC.BitBlt( 0, 0, rcBounds.Width(), rcBounds.Height(), &MemDC,
		rcBounds.left, rcBounds.top, SRCCOPY );

	// Copy image to clipboard
	CBitmap clipbitmap;
	clipbitmap.CreateCompatibleBitmap(&dc, rcBounds.Width(), rcBounds.Height() );
	CDC clipDC;
	clipDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = clipDC.SelectObject(&clipbitmap);
	clipDC.BitBlt( 0, 0, rcBounds.Width(), rcBounds.Height(), &MemDC,
		rcBounds.left, rcBounds.top, SRCCOPY);
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, clipbitmap.GetSafeHandle());
	CloseClipboard();
	clipDC.SelectObject(pOldBitmap);
	clipbitmap.Detach();

	// Copy the image in MemDC transparently
	MemDC.SetBkColor(RGB(0,0,0));
	MemDC.SetTextColor(RGB(255,255,255));
	MemDC.BitBlt(rcBounds.left, rcBounds.top, rcBounds.Width(), rcBounds.Height(),
		&MaskDC, rcBounds.left, rcBounds.top, MERGEPAINT);

	CPalette pal;
	hDIB=DDBToDIB(bitmap, BI_RGB, &pal );
	//CTreeView::OnBeginPrinting(pDC, pInfo);
}

HANDLE CViewHisto::DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal )
                     {
	BITMAP bm;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	DWORD dwLen;
	HANDLE hDIB;
	HANDLE handle;
	HDC hDC;
	HPALETTE hPal;

	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if ( dwCompression == BI_BITFIELDS )
	        return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
	        hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize               = sizeof(BITMAPINFOHEADER);
	bi.biWidth              = bm.bmWidth;
	bi.biHeight             = bm.bmHeight;
	bi.biPlanes             = 1;
	bi.biBitCount           = bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression        = dwCompression;
	bi.biSizeImage          = 0;
	bi.biXPelsPerMeter      = 0;
	bi.biYPelsPerMeter      = 0;
	bi.biClrUsed            = 0;
	bi.biClrImportant       = 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if ( nColors > 256 ) 
	        nColors = 0;
	dwLen = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB)
	{
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
			* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else
	{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
					0L,						// Start scan line
					(DWORD)bi.biHeight,		// # of scan lines
					(LPBYTE)lpbi			// address for bitmap bits
					+ (bi.biSize + nColors * sizeof(RGBQUAD)),
					(LPBITMAPINFO)lpbi,		// address of bitmapinfo
					(DWORD)DIB_RGB_COLORS);	// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);

		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);
	return hDIB;
                     }

void CViewHisto::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	GlobalFree(hDIB);
    SetWindowPlacement(&WndPlace);
    RedrawWindow();	
	//CTreeView::OnEndPrinting(pDC, pInfo);
}

void CViewHisto::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CTreeView::OnPrepareDC(pDC, pInfo);

	// Map logical unit of screen to printer unit
    pDC->SetMapMode(MM_ANISOTROPIC);
    CClientDC dcScreen(NULL);
    pDC->SetWindowExt(dcScreen.GetDeviceCaps(LOGPIXELSX),dcScreen.GetDeviceCaps(LOGPIXELSX));
    pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),pDC->GetDeviceCaps(LOGPIXELSX));
}

void CViewHisto::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*CWnd *pwnd = AfxGetMainWnd();

	CString strHelp;
	strHelp.LoadString(AFX_IDS_IDLEMESSAGE);
	pwnd->SendMessage(WM_SHOWCTXTHELP,(WPARAM)(&strHelp));*/
	
	CTreeView::OnMouseMove(nFlags, point);
}

void CViewHisto::OnExpandAll() 
{
	// TODO: Add your command handler code here
	SetRedraw(FALSE);
	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;
	//mListCtrl.Expand(TVI_ROOT,TVE_TOGGLE);

	HTREEITEM hti = mListCtrl.GetRootItem();
	do
	{
		ExpandBranch( hti );
	}
	while( (hti = mListCtrl.GetNextSiblingItem( hti )) != NULL );
	//ExpandBranch(hti);
	SetRedraw(TRUE);
}

void CViewHisto::OnCollapseAll() 
{
	SetRedraw(FALSE);
	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;
	//mListCtrl.Expand(TVI_ROOT,TVE_TOGGLE);

	HTREEITEM hti = mListCtrl.GetRootItem();
	do
	{
		CollapseBranch( hti );
	}
	while( (hti = mListCtrl.GetNextSiblingItem( hti )) != NULL );
	//ExpandBranch(hti);
	SetRedraw(TRUE);
}


void CViewHisto::ExpandBranch( HTREEITEM hti )
{
	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;

	if( mListCtrl.ItemHasChildren( hti ) && hti != NULL)
	{
		mListCtrl.Expand( hti, TVE_EXPAND );
        hti = mListCtrl.GetChildItem( hti );
        do
		{
			ExpandBranch( hti );
        }
		while( (hti = mListCtrl.GetNextSiblingItem( hti )) != NULL );
	}	
    mListCtrl.EnsureVisible( mListCtrl.GetSelectedItem() );
}

void CViewHisto::CollapseBranch( HTREEITEM hti )
{
	CTreeCtrl& mListCtrl = GetTreeCtrl();
	if (!mListCtrl) return;

	if( mListCtrl.ItemHasChildren( hti ) && hti != NULL)
	{
		mListCtrl.Expand( hti, TVE_COLLAPSE );
        hti = mListCtrl.GetChildItem( hti );
        do
		{
			CollapseBranch( hti );
        }
		while( (hti = mListCtrl.GetNextSiblingItem( hti )) != NULL );
	}	
    mListCtrl.EnsureVisible( mListCtrl.GetSelectedItem() );
}


void CViewHisto::OnHistoryExport() 
{


	static char BASED_CODE szFilter[] = "Symbolic Description (*.txt)|*.txt|DOT Graph Layout (*.dot)|*.dot||";
 
	CFileDialog mdlg(FALSE,"txt","*.txt",
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			szFilter,
			this);

	DWORD mFlag = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	mdlg.m_ofn.Flags |= mFlag;
	int ret = mdlg.DoModal();

	if (ret!=IDOK) return;

	CString m_strPath = mdlg.m_ofn.lpstrFile;
	int nformat = mdlg.m_ofn.nFilterIndex;

	CString strSymb;
	if (nformat==1)
		strSymb = OnExportSymbolic();
	else if (nformat==2)
		strSymb = OnExportDot();
	else 
		return;

	FILE *fp = NULL;
		fp = fopen(m_strPath,"w+");
	TRY
	{

		CStdioFile f(fp);
		f.WriteString(strSymb);
		f.WriteString(_T("\n"));
		f.Close();
	}
	CATCH( CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
}

CString CViewHisto::OnExportDot() 
{
	CString strSymb,strNode,strLink;

	strSymb = _T("digraph Viewfile {\n");
	strSymb += _T("node [ style = filled ];\n\n");

	strNode = _T("");
	strLink = _T("");

	int nb = GetDocument()->m_cObjectSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
		if (!pObj) continue;
		if (DYNAMIC_DOWNCAST(CEquation3D,pObj)) continue;
		if (!pObj->bDrawInGraph) continue;

		CString mstr = pObj->GetObjectName();
		mstr += _T(" [color=green];\n");

		strNode += mstr;

		int nb2 = pObj->cDependList.GetSize();
		for (int j=0;j<nb2;j++)
		{
			CObject3D* pPar = pObj->cDependList.GetAt(j);
			if (!pPar) continue;
			if (!pPar->bDrawInGraph) continue;
			CString mstr = pObj->GetObjectName();
			mstr += _T(" -> ") + pPar->GetObjectName() + _T(" [color=green];\n");
			strLink += mstr;

		}
		if (CCompositeObj3D* pcom = DYNAMIC_DOWNCAST(CCompositeObj3D,pObj)) 
		{
			int nb3 = pcom->m_cSubObjects.GetSize();
			for (int j=0;j<nb3;j++)
			{
				CObject3D* pObj22 = pcom->m_cSubObjects.GetAt(j);
				if (!pObj22) continue;
				if (DYNAMIC_DOWNCAST(CEquation3D,pObj22)) continue;
				if (pObj22->bDrawInGraph) 
				{

					CString mstr = pObj22->GetObjectName();
					mstr += _T(" [color=green];\n");

					strNode += mstr;
				}

				int nb2 = pObj22->cDependList.GetSize();
				for (int k=0;k<nb2;k++)
				{
					CObject3D* pPar = pObj22->cDependList.GetAt(k);
					if (!pPar) continue;
					if (!pPar->bDrawInGraph) continue;
					CString mstr = pObj->GetObjectName();
					mstr += _T(" -> ") + pPar->GetObjectName() + _T(" [color=green];\n");
					strLink += mstr;

				}
			}
		}

	}

	strSymb += strNode + strLink + _T("}\n\n");
	strSymb.Replace(_T("#"),_T("_"));

	return strSymb;
}

CString CViewHisto::OnExportSymbolic() 
{
	// TODO: Add your command handler code here
	CString strSymb;

	//strSymb = _T("Range[-8.0,8.0,-8.0,8.0,-6.0,8.0];\n");
	CVector4 mina(10000,10000,10000),maxa(0,0,0);
	int nb = GetDocument()->m_cObjectSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D* pObj = GetDocument()->m_cObjectSet.GetAt(i);
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
/*	if (strSymb.IsEmpty()) return;

	static char BASED_CODE szFilter[] = "Symbolic Description (*.txt)||";
 
	CFileDialog mdlg(FALSE,"txt","*.txt",
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			szFilter,
			this);

	DWORD mFlag = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	mdlg.m_ofn.Flags |= mFlag;
	int ret = mdlg.DoModal();

	if (ret!=IDOK) return;

	CString m_strPath = mdlg.m_ofn.lpstrFile;


	FILE *fp = NULL;
		fp = fopen(m_strPath,"w+");
	TRY
	{

		CStdioFile f(fp);
		f.WriteString(strSymb);
		f.WriteString(_T("\n"));
		f.Close();
	}
	CATCH( CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH*/

	return strSymb;

/*	CFileFind mfile;
	mfile.FindFile(TPref::Verif.strConvert);
	BOOL bRes = mfile.FindNextFile();
	CString strff = mfile.GetFilePath();



	int nErrNo = _access(TPref::Verif.strConvert,00);
	if (nErrNo!=0)
	{
	}
	nErrNo = _access(TPref::Verif.strRunTime,00);
	if (nErrNo!=0)
	{
	}

	CString lpszCommandLine;

	//lpszCommandLine = _T("command.com /E:2048 /C c:\\progra~1\\cocoa-4.1\\cocoa.bat -q c:\\progra~1\\cocoa-4.1\\test.coc");
	lpszCommandLine.Format(_T("%s %s %s"),
		TPref::Verif.strCommand,
		TPref::Verif.strRunTime,
		TPref::Verif.strConvert);

	STARTUPINFO siStartInfo;
	PROCESS_INFORMATION piProcInfo;
	memset(&siStartInfo, 0, sizeof(siStartInfo));
	memset(&piProcInfo, 0, sizeof(piProcInfo));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.wShowWindow = (WORD)SW_HIDE;
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW;


	// export the selected file into registry using command line
	if (CreateProcess (NULL, lpszCommandLine.GetBuffer(lpszCommandLine.GetLength()), 
							NULL, NULL, FALSE, 0, 
							NULL, NULL, &siStartInfo, &piProcInfo))
	{
		DWORD dResult = WaitForSingleObject (piProcInfo.hProcess, 5000);
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

		MessageBox(strSymb);
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
	}*/


}

