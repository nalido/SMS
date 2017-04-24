#include "stdafx.h"
#include "ClassItem.h"
#include "MainFrm.h"


CClassItem::CClassItem()
{
	m_nSelected = 0;
}


CClassItem::~CClassItem()
{
}
BEGIN_MESSAGE_MAP(CClassItem, CStatic)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CClassItem::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	
	CRect rect;
	GetClientRect(&rect);


	CFont font1, font2; //从小到大
	CFont* pOldFont;
	font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑"); 
	font2.CreateFontA(60, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑"); 

	CBrush brush1, brush2, brush3, brush4;
	CBrush* pOldBrush;
	brush1.CreateSolidBrush(COLOR_MANY); //绿色
	brush2.CreateSolidBrush(COLOR_LITTLE); //红色
	brush3.CreateSolidBrush(COLOR_NONE); //灰色
	brush4.CreateSolidBrush(COLOR_SELECTED); //蓝色

	if (m_nSelected)
	{
		dc.SetTextColor(RGB(255, 255, 255));
		dc.FillRect(&rect, &brush4);
	}
	else
	{
		int np = m_nUsed * 100 / m_nTotal; //课时空闲程度
		if (np >= 100) dc.FillRect(&rect, &brush3);
		else if (np > 80) dc.FillRect(&rect, &brush2);
		else dc.FillRect(&rect, &brush1);
		dc.SetTextColor(RGB(0, 0, 0));
	}


	dc.SetBkMode(TRANSPARENT); //文字背景透明

	pOldFont = dc.SelectObject(&font1);
	CString str;
	switch (m_nClass)
	{
	case 1:
		str.Format("Am08:00-10:00");
		break;
	case 2:
		str.Format("Am10:00-12:00");
		break;
	case 3:
		str.Format("Pm01:00-03:00");
		break;
	case 4:
		str.Format("Pm03:00-05:00");
		break;
	}
	dc.TextOutA(23, 0, str);


	pOldFont = dc.SelectObject(&font2);
	str.Format("%d/%d", m_nUsed, m_nTotal);
	dc.TextOutA(30, 20, str);

	dc.SelectObject(pOldFont);
}


HBRUSH CClassItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CClassItem::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CStatic::OnLButtonDblClk(nFlags, point);
}


void CClassItem::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nUsed >= m_nTotal) return;

	if (m_nSelected == 2) return;

	m_nSelected = 1 - m_nSelected;
	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}


void CClassItem::OnLButtonUp(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonUp(nFlags, point);
}
