// School.cpp : ʵ���ļ�
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


// CSchool ���

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


// CSchool ��Ϣ�������
