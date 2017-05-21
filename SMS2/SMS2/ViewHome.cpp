// ViewHome.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewHome.h"


// CViewHome

IMPLEMENT_DYNCREATE(CViewHome, CBCGPFormView)

CViewHome::CViewHome()
	: CBCGPFormView(CViewHome::IDD)
{
	EnableVisualManagerStyle();
}

CViewHome::~CViewHome()
{
}

void CViewHome::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POS1, m_BKpos1);
	DDX_Control(pDX, IDC_POS2, m_BKpos2);
}

BEGIN_MESSAGE_MAP(CViewHome, CBCGPFormView)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_WM_PAINT()
END_MESSAGE_MAP()


LRESULT CViewHome::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;
	if (flag == 1)
		m_nEraseBkgnd = 0; //不重绘背景 以免闪烁

	return 0;
}

BOOL CViewHome::OnEraseBkgnd(CDC* pDC)
{
	if (m_nEraseBkgnd<6)
	{
		m_nEraseBkgnd++;
		return TRUE;
	}
	else
		return CBCGPFormView::OnEraseBkgnd(pDC);
}


void CViewHome::OnPaint()
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
	int  W = GetSystemMetrics(SM_CXSCREEN);  //得到屏幕宽度 
	int  H = GetSystemMetrics(SM_CYSCREEN);
	width = max(W, width);
	height = max(H, height);

	bitmp.CreateCompatibleBitmap(&dc, width, height);
	MemDC.SelectObject(&bitmp);

	Graphics graph(MemDC.m_hDC);

	Image img(L"res//0.bmp");
	graph.DrawImage(&img, Rect(p1.X, p1.Y, width, height));


	CFont fontt1;
	fontt1.CreateFontA(80, 0, 0, 0, FW_BOLD, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
	CFont* oldF = MemDC.SelectObject(&fontt1);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextAlign(TA_CENTER);
	CString stri;
	stri.Format("东 华 驾 校 管 理 系 统");
	CPoint pT(width / 2, height / 4);
	MemDC.SetTextColor(RGB(241, 241, 241));
	MemDC.TextOutA(pT.x + 3, pT.y + 3, stri);
	MemDC.SetTextColor(RGB(51, 103, 155));
	MemDC.SetTextColor(RGB(65, 57, 36));
	MemDC.TextOutA(pT.x, pT.y, stri);

	//复制内存DC到屏幕上
	CPoint pos = GetScrollPosition();
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);


	bitmp.DeleteObject();
	MemDC.DeleteDC();
}

// CViewHome 诊断

#ifdef _DEBUG
void CViewHome::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewHome::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewHome 消息处理程序



void CViewHome::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	//获取背景定位点位置
	m_BKpos1.GetClientRect(&m_rctBK1);
	m_BKpos1.MapWindowPoints(this, &m_rctBK1);
	m_BKpos2.GetClientRect(&m_rctBK2);
	m_BKpos2.MapWindowPoints(this, &m_rctBK2);

}
