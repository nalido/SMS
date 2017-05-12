// MyPicCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "MyPicCtrl.h"


// CMyPicCtrl

IMPLEMENT_DYNAMIC(CMyPicCtrl, CStatic)

CMyPicCtrl::CMyPicCtrl()
{
	m_pPic = NULL;
}

CMyPicCtrl::~CMyPicCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyPicCtrl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyPicCtrl ��Ϣ�������


void CMyPicCtrl::InitPicSource(cv::Mat* mat)
{
	m_pPic = mat;
}


void CMyPicCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������

	//˫�������
	CRect   rect;
	CDC     MemDC;
	CBitmap MemMap;

	GetClientRect(&rect);
	MemDC.CreateCompatibleDC(&dc);
	MemMap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	MemDC.SelectObject(&MemMap);
	MemDC.FillSolidRect(&rect, RGB(255, 255, 255));


	if (!m_pPic->empty())
	{
		HDC hdc = MemDC.GetSafeHdc();
		IplImage* frame;
		frame = &IplImage(*m_pPic);
		CvvImage cvvImage;
		cvvImage.CopyOf(frame);
		cvvImage.DrawToHDC(hdc, rect);
	}
	else
	{
		MemDC.TextOutA(rect.CenterPoint().x, rect.CenterPoint().y, "����Ƭ");
	}

	//���   
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
	MemMap.DeleteObject();
}
