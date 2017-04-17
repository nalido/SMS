// WorkDay.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "WorkDay.h"


// CWorkDay

IMPLEMENT_DYNCREATE(CWorkDay, CBCGPFormView)

CWorkDay::CWorkDay()
	: CBCGPFormView(CWorkDay::IDD)
{
	EnableVisualManagerStyle();

	m_nDayWid = 0;
	m_nDayHei = 0;
	m_nHeadHei = 0;
}

CWorkDay::~CWorkDay()
{
}

void CWorkDay::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEADER1, m_SHeader);
	DDX_Control(pDX, IDC_DATE0, m_SDate);
	DDX_Control(pDX, IDC_DAY0, m_SDay);
	DDX_Control(pDX, IDC_DAY4, m_SDay4);
}

BEGIN_MESSAGE_MAP(CWorkDay, CBCGPFormView)
//	ON_BN_CLICKED(IDC_BUTTON1, &CWorkDay::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CWorkDay 诊断

#ifdef _DEBUG
void CWorkDay::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWorkDay::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWorkDay 消息处理程序


//void CWorkDay::OnBnClickedButton1()
//{
//	MessageBox("sss");
//}


void CWorkDay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	

	if (IsWindow(m_SHeader.GetSafeHwnd())) //已经创建
	{
		CRect rect, rect1, rect_date0, rect_day0;
		m_SHeader.GetClientRect(&rect);
		m_SHeader.MapWindowPoints(this, &rect);
		m_SDay4.GetClientRect(&rect1);
		m_SDay4.MapWindowPoints(this, &rect1);
		m_SDay.GetClientRect(&rect_day0);
		m_SDay.MapWindowPoints(this, &rect_day0);
		m_SDate.GetClientRect(&rect_date0);
		m_SDate.MapWindowPoints(this, &rect_date0);
		m_nDayWid = rect.Width();
		m_nDayHei = rect1.Height() + rect_date0.Height();
		m_nHeadHei = rect.Height();


		for (int col = 0; col < 8; col++)
		{
			dc.MoveTo(rect.left, rect.top);
			dc.LineTo(rect.left, rect1.bottom);

			rect.MoveToX(rect.right);
		}

		int right = rect.left - rect.Width();
		dc.MoveTo(rect_date0.left, rect.top);
		dc.LineTo(right, rect.top);
		for (int r = 0; r < 5; r++)
		{
			dc.MoveTo(rect_date0.left, rect_date0.top);
			dc.LineTo(right, rect_date0.top);
			dc.MoveTo(rect_date0.left, rect_date0.bottom);
			dc.LineTo(right, rect_date0.bottom);

			rect_date0.MoveToY(rect_date0.bottom + rect1.Height());
		}
		dc.MoveTo(rect_date0.left, rect1.bottom);
		dc.LineTo(right, rect1.bottom);
	}
}


void CWorkDay::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	//CDC* pDc = m_SCalendar.GetDC();
	//if (IsWindow(m_SCalendar.GetSafeHwnd())) //已经创建
	//{
	//	CRect rect;
	//	m_SCalendar.GetClientRect(&rect);
	//	m_SCalendar.MapWindowPoints(this, &rect);

	//	int wid = (rect.Width() + 6) / 7;
	//	pDc->Rectangle(0, 0, wid, rect.Height());
	//}
}





void CWorkDay::OnLButtonDown(UINT nFlags, CPoint point)
{
	CString strMsg;
	strMsg.Format("mouse point: %d, %d\r\n", point.x, point.y);
	TRACE0(strMsg);

	int row = (point.y - m_nHeadHei) / m_nDayHei;
	int col = (point.x) / m_nDayWid;
	strMsg.Format("day place: %d, %d\r\n", row, col);
	TRACE0(strMsg);

	OnClick(row, col);

	CBCGPFormView::OnLButtonDown(nFlags, point);
}

void CWorkDay::OnClick(int row, int col)
{
	if (row > 4 || col > 6){ TRACE0("out of calendar range");  return; }

	CRect rect;
	GetDayRect(row, col, &rect);


}

void CWorkDay::GetDayRect(int row, int col, LPRECT lpRect)
{
	lpRect->left = col * m_nDayWid;
	lpRect->right = lpRect->left + m_nDayWid;
	lpRect->top = row * m_nDayHei + m_nHeadHei;
	lpRect->bottom = lpRect->top + m_nDayHei;
}