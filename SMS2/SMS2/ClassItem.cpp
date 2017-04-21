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
	// TODO:  �ڴ˴������Ϣ����������
	
	CRect rect;
	GetClientRect(&rect);


	CFont font1, font2; //��С����
	CFont* pOldFont;
	font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�"); 
	font2.CreateFontA(60, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�"); 

	CBrush brush1, brush2, brush3, brush4;
	CBrush* pOldBrush;
	brush1.CreateSolidBrush(COLOR_MANY); //��ɫ
	brush2.CreateSolidBrush(COLOR_LITTLE); //��ɫ
	brush3.CreateSolidBrush(COLOR_NONE); //��ɫ
	brush4.CreateSolidBrush(COLOR_SELECTED); //��ɫ

	if (m_nSelected)
	{
		dc.SetTextColor(RGB(255, 255, 255));
		dc.FillRect(&rect, &brush4);
	}
	else
	{
		int np = m_nUsed * 100 / m_nTotal; //��ʱ���г̶�
		if (np >= 100) dc.FillRect(&rect, &brush3);
		else if (np > 80) dc.FillRect(&rect, &brush2);
		else dc.FillRect(&rect, &brush1);
		dc.SetTextColor(RGB(0, 0, 0));
	}


	dc.SetBkMode(TRANSPARENT); //���ֱ���͸��

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

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CClassItem::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
