// ViewK23Exam.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewK23Exam.h"


// CViewK23Exam

IMPLEMENT_DYNCREATE(CViewK23Exam, CBCGPFormView)

CViewK23Exam::CViewK23Exam()
	: CBCGPFormView(CViewK23Exam::IDD)
{
	EnableVisualManagerStyle();
}

CViewK23Exam::~CViewK23Exam()
{
}

void CViewK23Exam::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewK23Exam, CBCGPFormView)
END_MESSAGE_MAP()


// CViewK23Exam 诊断

#ifdef _DEBUG
void CViewK23Exam::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewK23Exam::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewK23Exam 消息处理程序
