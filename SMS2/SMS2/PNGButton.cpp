#include "stdafx.h"
#include "PNGButton.h"


CPNGButton::CPNGButton()
{
	m_strFilePath = "";
	m_isSelected = FALSE;
}


CPNGButton::~CPNGButton()
{
}

void CPNGButton::SetPNGSource(CString & path)
{
	m_strFilePath = path;
}

BEGIN_MESSAGE_MAP(CPNGButton, CStatic)
ON_WM_PAINT()
//ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void CPNGButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	MapWindowPoints(this, &rect);

	if (m_isSelected)
	{
		CBrush brush;
		brush.CreateSolidBrush(RGB(120, 240, 100));
		dc.FillRect(rect, &brush);
	}

	if (m_PNG.IsNull() && !m_strFilePath.IsEmpty())
	{
		m_PNG.Load(m_strFilePath);
		if (m_PNG.IsNull())
		{
			TRACE("加载图片资源失败");
			return;
		}

		if (m_PNG.GetBPP() == 32) //确认图片包含alpha通道
		{
			for (int i = 0; i < m_PNG.GetWidth(); i++)
			for (int j = 0; j < m_PNG.GetHeight(); j++)
			{
				BYTE* pByte = (BYTE*)m_PNG.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}

	m_PNG.Draw(dc.m_hDC, rect);
}

void CPNGButton::SetState(BOOL isSelected)
{
	m_isSelected = isSelected;
	Invalidate();
}

//void CPNGButton::OnMouseMove(UINT nFlags, CPoint point)
//{
//	CRect rect;
//	GetClientRect(&rect);
//	MapWindowPoints(this, &rect);
//	rect.DeflateRect(10, 10);
//	if (rect.PtInRect(point))
//	{
//		m_isSelected = 1;
//	}
//	else
//	{
//		m_isSelected = 0;
//	}
//	Invalidate();
//
//	CStatic::OnMouseMove(nFlags, point);
//}


void CPNGButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_isSelected = !m_isSelected;
	SetState(m_isSelected);

	CStatic::OnLButtonDown(nFlags, point);
}
