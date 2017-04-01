// outputbar.cpp : implementation of the COutputBar class
//

#include "stdafx.h"
#include "SMS2.h"
#include "outputbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

BEGIN_MESSAGE_MAP(COutputBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar construction/destruction

COutputBar::COutputBar()
{
	// TODO: add one-time construction code here
}

COutputBar::~COutputBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if (!m_wndTabs.Create (CBCGPTabWnd::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create list windows.
	// TODO: create your own window here:
	const DWORD dwViewStyle =	
		LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LVS_REPORT;
	
	m_wndList1.m_bVisualManagerStyle = TRUE;
	m_wndList2.m_bVisualManagerStyle = TRUE;
	m_wndList3.m_bVisualManagerStyle = TRUE;

	if (!m_wndList1.Create (dwViewStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndList2.Create (dwViewStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndList3.Create (dwViewStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}

	// Attach list windows to tab:
	m_wndTabs.AddTab (&m_wndList1, _T("Output 1"), -1);
	m_wndTabs.AddTab (&m_wndList2, _T("Output 2"), -1);
	m_wndTabs.AddTab (&m_wndList3, _T("Output 3"), -1);


	m_wndList1.InsertColumn(0, _T("information"), LVCFMT_LEFT, 800);

	return 0;
}

void COutputBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	m_wndTabs.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputBar::AddItem2List1(CString strMsg)
{
	int count = m_wndList1.GetItemCount();
	if (count > 1000)
	{
		m_wndList1.DeleteAllItems();
		count = 0;
	}

	CString strIndex;
	CTime localtime;
	localtime = CTime::GetCurrentTime();
	strIndex = localtime.Format("[%Y-%m-%d\t%X]") + strMsg;
	//str_log = localtime.Format("\r\n\r\n%Y-%m-%d\t%X\r\n") + str_log;
	m_wndList1.InsertItem(LVIF_TEXT | LVIF_STATE, count, strIndex,
		0, LVIS_SELECTED, 0, 0);
	//m_wndList1.SetItemText(count, 1, strMsg);
	m_wndList1.EnsureVisible(count, FALSE); //滚动至最后一行
}