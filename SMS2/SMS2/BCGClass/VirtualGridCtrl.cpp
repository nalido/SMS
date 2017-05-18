//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2014 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// VirtualGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "..\stdafx.h"
#include "VirtualGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _BCGSUITE_INC_
	#define TEXT_MARGIN 3
#endif


//////////////////the sample of gridcallback function///////////////////////////////////
//static BOOL CALLBACK GridCallback (BCGPGRID_DISPINFO* pdi, LPARAM lp)
//{
//	ASSERT (pdi != NULL);
//
//	CVirtualGridCtrl* pGridCtrl = (CVirtualGridCtrl*) lp;
//
//    int nRow = pdi->item.nRow;	// Row of an item
//	int nCol = pdi->item.nCol;	// Column of an item
//
//	pdi->item.varValue = (LPCTSTR) pGridCtrl->GetItemValue (nRow, nCol);
//	return TRUE;
//}

/////////////////////////////////////////////////////////////////////////////
// CVirtualGridCtrl

CVirtualGridCtrl::CVirtualGridCtrl()
{
	m_OnWork_DbClick = NULL;
	m_OnWork_Click = NULL;
}

CVirtualGridCtrl::~CVirtualGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CVirtualGridCtrl, CBCGPGridCtrl)
	//{{AFX_MSG_MAP(CVirtualGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
//	ON_WM_MBUTTONDBLCLK()
ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVirtualGridCtrl message handlers

int CVirtualGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	////the sample of create a VirtualGrid///////////////////
	//const int nColumns = 5;
	//const int nRows = 10000000;	// 10 milions
	//
	//for (int nColumn = 0; nColumn < nColumns; nColumn++)
	//{
	//	CString strColumn;
	//	strColumn.Format (_T("%c"), _T('A') + nColumn);

	//	InsertColumn (nColumn, strColumn, 150);
	//}

	//EnableVirtualMode (GridCallback, (LPARAM) this);
	//SetVirtualRows (nRows);

	//SetSingleSel (FALSE);
	//EnableHeader (TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_SELECT);
	//EnableRowHeader (TRUE, BCGP_GRID_HEADER_SELECT, CalcRowHeaderWidth (nRows));
	//EnableLineNumbers ();
	//EnableInvertSelOnCtrl ();

	//SetEditFirstClick (FALSE);
	//SetPrintRange (0, 100);	// print only first 100 rows
	//
	//LoadState (_T("VirtualGrid"));
	//AdjustLayout ();

	return 0;
}

void CVirtualGridCtrl::OnDestroy() 
{
	SaveState (_T("VirtualGrid"));
	CBCGPGridCtrl::OnDestroy();
}


//if don't call SetReadOnly(), this function can used when the item changed.
void CVirtualGridCtrl::OnItemChanged (CBCGPGridItem* pItem, int nRow, int nColumn)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	CBCGPGridRow* pRow = pItem->GetParentRow ();
	if (pRow == NULL)
	{
		return;
	}

	ASSERT_VALID (pRow);

   //TO change the data in data struct.
}


int CVirtualGridCtrl::CalcRowHeaderWidth (int nRowCount)
{
	CString str;
	str.Format (_T("%d"), nRowCount);

	CClientDC dc (this);
	HFONT hfontOld = SetCurrFont (&dc);

	int nExtent = dc.GetTextExtent (str).cx;
	int nResult = max (30, nExtent + 2 * TEXT_MARGIN);

	::SelectObject (dc.GetSafeHdc (), hfontOld);

	return nResult;
}

void CVirtualGridCtrl::SetPrintRange (int nFirstRow, int nLastRow)
{
	ASSERT_VALID (this);
	ASSERT (nFirstRow <= nLastRow);

	m_rangePrint.m_nTop = nFirstRow;
	m_rangePrint.m_nBottom = nLastRow;
}

void CVirtualGridCtrl::OnPreparePrintPages (CPrintInfo* pInfo,
								  int /*nFirstItem*/, int /*nLastItem*/)
{
	CBCGPGridCtrl::OnPreparePrintPages (pInfo, m_rangePrint.m_nTop, m_rangePrint.m_nBottom);
}


//void CVirtualGridCtrl::OnMButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//
//	CBCGPGridCtrl::OnMButtonDblClk(nFlags, point);
//}
void CVirtualGridCtrl::OnSelChanged(const CBCGPGridRange&range, BOOL bSelect)
{
	if (m_OnWork_Click != NULL && bSelect)
	{
		m_OnWork_Click(this->GetParent());
	}
}

void CVirtualGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (IsFocused())
	{
		if (m_OnWork_DbClick != NULL)
		{
			m_OnWork_DbClick(this->GetParent());
		}
	}

	CBCGPGridCtrl::OnLButtonDblClk(nFlags, point);
}

void CVirtualGridCtrl::SetCallBack_DblClk(ONWORK onWork) //注册双击鼠标回调函数
{
	m_OnWork_DbClick = onWork;
}

void CVirtualGridCtrl::SetCallBack_Clk(ONWORK onWork) //注册双击鼠标回调函数
{
	m_OnWork_Click = onWork;
}


void CVirtualGridCtrl::GridRefresh(int nCount)
{
	RemoveAll();
	SetVirtualRows(nCount);
	AdjustLayout();
}

int CVirtualGridCtrl::GetLastColWidth(int* wids, int cols, int rw)
{
	int hw = GetRowHeaderWidth() * 2 / 3; //行头宽度

	int sum = 0;
	for (int i = 0; i < cols; i++)
	{
		sum += wids[i];
	}

	return rw - hw - sum;
}