// ViewRegister.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewRegister.h"


// CViewRegister

IMPLEMENT_DYNCREATE(CViewRegister, CBCGPFormView)

CViewRegister::CViewRegister()
	: CBCGPFormView(CViewRegister::IDD)
{
	EnableVisualManagerStyle();
}

CViewRegister::~CViewRegister()
{
}

void CViewRegister::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewRegister, CBCGPFormView)
END_MESSAGE_MAP()


// CViewRegister ���

#ifdef _DEBUG
void CViewRegister::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewRegister::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewRegister ��Ϣ�������
