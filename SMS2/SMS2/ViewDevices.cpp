// ViewDevices.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewDevices.h"


// CViewDevices

IMPLEMENT_DYNCREATE(CViewDevices, CBCGPFormView)

CViewDevices::CViewDevices()
	: CBCGPFormView(CViewDevices::IDD)
{

}

CViewDevices::~CViewDevices()
{
}

void CViewDevices::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewDevices, CBCGPFormView)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CViewDevices ���

#ifdef _DEBUG
void CViewDevices::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewDevices::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewDevices ��Ϣ�������


void CViewDevices::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CBCGPFormView::OnPaint()
}
