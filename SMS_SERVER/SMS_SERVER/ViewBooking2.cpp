// ViewBooking2.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS_SERVER.h"
#include "ViewBooking2.h"


// CViewBooking2

IMPLEMENT_DYNCREATE(CViewBooking2, CBCGPFormView)

CViewBooking2::CViewBooking2()
	: CBCGPFormView(CViewBooking2::IDD)
{
	EnableVisualManagerStyle();
}

CViewBooking2::~CViewBooking2()
{
}

void CViewBooking2::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewBooking2, CBCGPFormView)
END_MESSAGE_MAP()


// CViewBooking2 诊断

#ifdef _DEBUG
void CViewBooking2::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewBooking2::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewBooking2 消息处理程序
