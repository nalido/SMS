// DlgDateItem.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgDateItem.h"
#include "afxdialogex.h"


// CDlgDateItem 对话框

IMPLEMENT_DYNAMIC(CDlgDateItem, CBCGPDialog)

CDlgDateItem::CDlgDateItem(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgDateItem::IDD, pParent)
{
	EnableVisualManagerStyle();
	m_strDate = "";
}

CDlgDateItem::~CDlgDateItem()
{
}

void CDlgDateItem::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDateItem, CBCGPDialog)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR1, &CDlgDateItem::OnMcnSelectMonthcalendar1)
END_MESSAGE_MAP()


// CDlgDateItem 消息处理程序


void CDlgDateItem::OnMcnSelectMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	
	m_strDate.Format("%d/%02d/%02d", pSelChange->stSelStart.wYear
		, pSelChange->stSelStart.wMonth, pSelChange->stSelStart.wDay);

	*pResult = 0;

	OnOK();
}


//BOOL CDlgDateItem::PreCreateWindow(CREATESTRUCT& cs)
//{
//	cs.cx = 0;
//	//MoveWindow()
//
//	return CBCGPDialog::PreCreateWindow(cs);
//}


BOOL CDlgDateItem::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	CPoint point;
	GetCursorPos(&point);

	MoveWindow(point.x, point.y, rect.Width(), rect.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
