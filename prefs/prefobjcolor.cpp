// prefobjcolor.cpp : implementation file
//

#include "stdafx.h"
#include "..\calques3d.h"
#include "prefobjcolor.h"
#include "Prefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefObjColor property page

IMPLEMENT_DYNCREATE(CPrefObjColor, CBCGPPropertyPage)

CPrefObjColor::CPrefObjColor() : CBCGPPropertyPage(CPrefObjColor::IDD)
{
	//{{AFX_DATA_INIT(CPrefObjColor)
	m_bGrayHidden = (TPref::GrayedHidden);
	//}}AFX_DATA_INIT
}

CPrefObjColor::~CPrefObjColor()
{
}

void CPrefObjColor::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefObjColor)
	DDX_Check(pDX, IDC_SHAPE_HIDDEN, m_bGrayHidden);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_SHAPE_DEFCOLOR, m_cColorlist);
	if (pDX->m_bSaveAndValidate)
	{
		TPref::GrayedHidden = m_bGrayHidden;
		for (int j=0;j<8;j++)
		{
			TPref::custColors[j] = m_cColorlist.GetItemBkColour(j+1,1);
			if (!TPref::GrayedHidden)
			{
				TPref::custColors[j+8] = m_cColorlist.GetItemBkColour(j+1,2);
			}
			TPref::custColorsString[j] = m_cColorlist.GetItemText(j+1,0);
		}
	}
}


BEGIN_MESSAGE_MAP(CPrefObjColor, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPrefObjColor)
	ON_BN_CLICKED(IDC_SHAPE_HIDDEN, OnGrayHidden)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_NOTIFY(NM_DBLCLK , IDC_SHAPE_DEFCOLOR, OnGridClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefObjColor message handlers


void CPrefObjColor::OnDestroy() 
{
	UpdateData();
	CBCGPPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here	
}

BOOL CPrefObjColor::OnInitDialog() 
{
	BOOL bRet = CBCGPPropertyPage::OnInitDialog();
	FillColorList();

	return bRet;

}
void CPrefObjColor::FillColorList()
{
	const char *strDefaultTitle[]={
				"Name",
				"Appar.   ",
				"Non-app."
		};
	CBCGPPropertyPage::OnInitDialog();

	m_cColorlist.SetTextBkColor(RGB(255,255,255));
	m_cColorlist.EnableSelection(FALSE);
	m_cColorlist.SetRowResize(FALSE);
	m_cColorlist.SetColumnResize(FALSE);
	//m_cColorlist.SetEditable(false);
	//m_cColorlist.SetListMode(true);
	//m_cColorlist.EnableDragAndDrop(FALSE);
	//m_cColorlist.SetSingleRowSelection(true);
	//m_cColorlist.SetImageList(&m_ImageList);

	int NBCOLUMNS = 3;

	m_cColorlist.SetColumnCount(NBCOLUMNS);
	//m_cColorlist.SetFixedColumnCount(1);
	m_cColorlist.SetRowCount(8+1);
	m_cColorlist.SetFixedRowCount(1);

	for (int i=0;i<NBCOLUMNS;i++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		
		Item.row = 0;
		Item.col = i;
		Item.nFormat = DT_CENTER |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
			//Item.szText.Format(_T("%s"),CS->m_dataName);
			//Item.strText.Format(_T("%s"),CS->m_dataName);
		Item.strText.Format(_T("%s"),strDefaultTitle[i]);
		m_cColorlist.SetItem(&Item);
	}

	DWORD dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	for (int row = 0; row < 8; row++)
	{
		GV_ITEM Item,Item2;

		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row+1;

		Item.strText.Format(_T("%s"),TPref::custColorsString[row]);
		Item.nFormat = DT_LEFT |DT_WORDBREAK; //DT_CENTER|DT_VCENTER
		Item.col = 0;

		Item2.mask = GVIF_BKCLR|GVIF_STATE;
		Item2.row = row+1;
		Item2.col = 1;
		int nformat = m_cColorlist.GetDefaultCell(FALSE,FALSE)->GetState();
		Item2.nState = nformat | GVIS_READONLY;
		Item2.crBkClr = TPref::custColors[row];

		m_cColorlist.SetItem(&Item);
		m_cColorlist.SetItem(&Item2);
	}
	m_bGrayHidden=!m_bGrayHidden;
	OnGrayHidden();
	m_cColorlist.AutoSizeColumns();
	//m_cColorlist.ExpandColumnsToFit();
	//m_cColorlist.EnableScrollBars(SB_HORZ, FALSE);

    CRect rect;
    m_cColorlist.GetClientRect(rect);
	int nsb = m_cColorlist.GetColumnWidth(2);
	nsb*=2;
	m_cColorlist.SetColumnWidth(0,rect.Width()-nsb+3);
}

void CPrefObjColor::OnGrayHidden() 
{
	// TODO: Add your control notification handler code here
	m_bGrayHidden = ! m_bGrayHidden;
	for (int i=0;i<8;i++)
	{
		GV_ITEM Item;

		Item.mask = GVIF_BKCLR|GVIF_STATE;
		Item.row = i+1;
		Item.col = 2;
		int nformat = m_cColorlist.GetDefaultCell(FALSE,FALSE)->GetState();
		Item.nState = nformat | GVIS_READONLY;
		if (m_bGrayHidden)
			Item.crBkClr = RGB(192,192,192);
		else
			Item.crBkClr = TPref::custColors[i+8];
		m_cColorlist.SetItem(&Item);
	}
	m_cColorlist.Invalidate();
	m_cColorlist.UpdateWindow();
}

void CPrefObjColor::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

    int nRow = pItem->iRow;
    int nCol = pItem->iColumn;

	if (nCol)
	{
		if (nCol==2 && m_bGrayHidden) return;
		COLORREF oldClr = m_cColorlist.GetItemBkColour(nRow,nCol);

		//CColorDialog dlg(oldClr,CC_RGBINIT);
		CBCGPColorDialog dlg(oldClr);
		if (dlg.DoModal() == IDOK)
		{
			//m_cColorlist.SetItemBkColour(nRow,nCol,dlg.m_cc.rgbResult);
			m_cColorlist.SetItemBkColour(nRow,nCol,dlg.GetColor());
			m_cColorlist.RedrawCell(nRow,nCol);
		}
	}
}
