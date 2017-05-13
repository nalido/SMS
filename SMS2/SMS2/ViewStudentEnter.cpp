// ViewStudentEnter.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewStudentEnter.h"


// CViewStudentEnter

IMPLEMENT_DYNCREATE(CViewStudentEnter, CBCGPFormView)

CViewStudentEnter::CViewStudentEnter()
	: CBCGPFormView(CViewStudentEnter::IDD)
{
	EnableVisualManagerStyle();
}

CViewStudentEnter::~CViewStudentEnter()
{
}

void CViewStudentEnter::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POS1, m_BKpos1);
	DDX_Control(pDX, IDC_POS2, m_BKpos2);
}

BEGIN_MESSAGE_MAP(CViewStudentEnter, CBCGPFormView)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CViewStudentEnter 诊断

#ifdef _DEBUG
void CViewStudentEnter::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewStudentEnter::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewStudentEnter 消息处理程序


void CViewStudentEnter::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	MapWindowPoints(this, &rect);

	CDC     MemDC;
	//HBITMAP hbitmap;
	CBitmap bitmp;
	MemDC.CreateCompatibleDC(&dc);

	Point p1(m_rctBK1.left, m_rctBK1.top);
	Point p2(m_rctBK2.right, m_rctBK2.bottom);
	int width = m_rctBK2.right - m_rctBK1.left;
	int height = m_rctBK2.bottom - m_rctBK1.top;

	bitmp.CreateCompatibleBitmap(&dc, width, height);
	MemDC.SelectObject(&bitmp);

	Graphics graph(MemDC.m_hDC);
	//LinearGradientBrush lgb(p1, p2, Color(145, 168, 201), Color(213, 202, 181));
	LinearGradientBrush lgb(p1, p2, Color(156, 187, 232), Color(213, 202, 181));
	graph.FillRectangle(&lgb, m_rctBK1.left, m_rctBK1.top, width, height);



	//复制内存DC到屏幕上
	CPoint pos = GetScrollPosition();
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);


	bitmp.DeleteObject();
	MemDC.DeleteDC();
}


void CViewStudentEnter::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


	//获取背景定位点位置
	m_BKpos1.GetClientRect(&m_rctBK1);
	m_BKpos1.MapWindowPoints(this, &m_rctBK1);
	m_BKpos2.GetClientRect(&m_rctBK2);
	m_BKpos2.MapWindowPoints(this, &m_rctBK2);
}
