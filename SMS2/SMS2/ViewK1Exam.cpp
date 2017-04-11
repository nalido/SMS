// ViewViewK1Exam.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewK1Exam.h"


// CViewK1Exam

IMPLEMENT_DYNCREATE(CViewK1Exam, CBCGPFormView)

CViewK1Exam::CViewK1Exam()
	: CBCGPFormView(CViewK1Exam::IDD)
{
	EnableVisualManagerStyle();
}

CViewK1Exam::~CViewK1Exam()
{
}

void CViewK1Exam::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewK1Exam, CBCGPFormView)
END_MESSAGE_MAP()


// CViewK1Exam 诊断

#ifdef _DEBUG
void CViewK1Exam::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewK1Exam::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewK1Exam 消息处理程序
