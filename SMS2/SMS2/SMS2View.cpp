// SMS2View.cpp : implementation of the CSMS2View class
//

#include "stdafx.h"
#include "SMS2.h"

#include "SMS2Doc.h"
#include "SMS2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMS2View

IMPLEMENT_DYNCREATE(CSMS2View, CBCGPFormView)

BEGIN_MESSAGE_MAP(CSMS2View, CBCGPFormView)
//	ON_WM_SIZE()
//	ON_WM_PAINT()
//	ON_WM_ERASEBKGND()
//	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
END_MESSAGE_MAP()

//LRESULT CSMS2View::OnUserUpdate(WPARAM wParam, LPARAM lParam)
//{
//	int flag = (int)wParam;
//	if (flag == 1)
//		m_nEraseBkgnd = 0; //不重绘背景 以免闪烁
//
//	return 0;
//}


//BOOL CSMS2View::OnEraseBkgnd(CDC* pDC)
//{
//	if (m_nEraseBkgnd<6)
//	{
//		m_nEraseBkgnd++;
//		return TRUE;
//	}
//	else
//		return CBCGPFormView::OnEraseBkgnd(pDC);
//}

// CSMS2View construction/destruction

CSMS2View::CSMS2View()
	: CBCGPFormView(CSMS2View::IDD)
{

	EnableVisualManagerStyle();

	m_nEraseBkgnd = 0;

}

CSMS2View::~CSMS2View()
{
}

void CSMS2View::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BOOL CSMS2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CBCGPFormView::PreCreateWindow(cs);
}

void CSMS2View::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();

}


// CSMS2View diagnostics

#ifdef _DEBUG
void CSMS2View::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CSMS2View::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CSMS2Doc* CSMS2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMS2Doc)));
	return (CSMS2Doc*)m_pDocument;
}
#endif //_DEBUG


// CSMS2View message handlers


//void CSMS2View::OnSize(UINT nType, int cx, int cy)
//{
//	CBCGPFormView::OnSize(nType, cx, cy);
//}


//void CSMS2View::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//
//	CRect rect;
//	GetClientRect(&rect);
//	MapWindowPoints(this, &rect);
//
//	CDC     MemDC;
//	//HBITMAP hbitmap;
//	CBitmap bitmp;
//	MemDC.CreateCompatibleDC(&dc);
//
//	Point p1(m_rctBK1.left, m_rctBK1.top);
//	Point p2(m_rctBK2.right, m_rctBK2.bottom);
//	int width = m_rctBK2.right - m_rctBK1.left;
//	int height = m_rctBK2.bottom - m_rctBK1.top;
//
//	bitmp.CreateCompatibleBitmap(&dc, width, height);
//	MemDC.SelectObject(&bitmp);
//
//	Graphics graph(MemDC.m_hDC);
//
//	Image img(L"res//0.bmp");
//	graph.DrawImage(&img, Rect(p1.X, p1.Y, width, height));
//
//
//
//	//复制内存DC到屏幕上
//	CPoint pos = GetScrollPosition();
//	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);
//
//
//	bitmp.DeleteObject();
//	MemDC.DeleteDC();
//}


