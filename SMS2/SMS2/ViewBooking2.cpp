// ViewBooking2.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewBooking2.h"


// CViewBooking2

IMPLEMENT_DYNCREATE(CViewBooking2, CBCGPPlannerManagerView)

CViewBooking2::CViewBooking2()
	//: CBCGPPlannerManagerView(CViewBooking2::IDD)
{
	//EnableVisualManagerStyle();
}

CViewBooking2::~CViewBooking2()
{
}

void CViewBooking2::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPlannerManagerView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewBooking2, CBCGPPlannerManagerView)
END_MESSAGE_MAP()


// CViewBooking2 诊断

#ifdef _DEBUG
void CViewBooking2::AssertValid() const
{
	CBCGPPlannerManagerView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewBooking2::Dump(CDumpContext& dc) const
{
	CBCGPPlannerManagerView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewBooking2 消息处理程序


void CViewBooking2::OnInitialUpdate()
{
	CBCGPPlannerManagerView::OnInitialUpdate();

	SetType(CBCGPPlannerManagerCtrl::BCGP_PLANNER_TYPE_MONTH);
}
