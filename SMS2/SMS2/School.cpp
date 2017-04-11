// School.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "School.h"


// CSchool

IMPLEMENT_DYNCREATE(CSchool, CBCGPFormView)

CSchool::CSchool()
	: CBCGPFormView(CSchool::IDD)
{
	EnableVisualManagerStyle();
}

CSchool::~CSchool()
{
}

void CSchool::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSchool, CBCGPFormView)
END_MESSAGE_MAP()


// CSchool 诊断

#ifdef _DEBUG
void CSchool::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSchool::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSchool 消息处理程序
