// ViewBooking1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewBooking1.h"


// CViewBooking1

IMPLEMENT_DYNCREATE(CViewBooking1, CBCGPFormView)

CViewBooking1::CViewBooking1()
	: CBCGPFormView(CViewBooking1::IDD)
{
	EnableVisualManagerStyle();
}

CViewBooking1::~CViewBooking1()
{
}

void CViewBooking1::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewBooking1, CBCGPFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CViewBooking1::OnBnClickedButton1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CViewBooking1 ���

#ifdef _DEBUG
void CViewBooking1::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewBooking1::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewBooking1 ��Ϣ�������


void CViewBooking1::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CViewBooking1::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
}


void CViewBooking1::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	if (IsWindow(GetDlgItem(IDC_BUTTON1)->GetSafeHwnd())) //�жϿؼ������Ƿ��Ѿ�����
	{
		CRect rect, rect_btn;
		GetClientRect(&rect);
		GetDlgItem(IDC_BUTTON1)->GetClientRect(&rect_btn);
		GetDlgItem(IDC_BUTTON1)->MapWindowPoints(this, &rect_btn);

		GetDlgItem(IDC_BUTTON1)->MoveWindow(rect.right / 2, rect_btn.top, rect_btn.Width(), rect_btn.Height());
	}
}
