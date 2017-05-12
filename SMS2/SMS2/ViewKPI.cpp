// ViewKPI.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewKPI.h"


// CViewKPI

IMPLEMENT_DYNCREATE(CViewKPI, CBCGPFormView)

CViewKPI::CViewKPI()
	: CBCGPFormView(CViewKPI::IDD)
{
	EnableVisualManagerStyle();
}

CViewKPI::~CViewKPI()
{
}

void CViewKPI::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewKPI, CBCGPFormView)
END_MESSAGE_MAP()


// CViewKPI 诊断

#ifdef _DEBUG
void CViewKPI::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewKPI::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewKPI 消息处理程序
