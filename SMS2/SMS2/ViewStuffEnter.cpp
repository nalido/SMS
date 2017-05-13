// ViewStuffEnter.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewStuffEnter.h"
#include "MainFrm.h"
#include "DlgNewStuff.h"
#include "DlgResetPSW.h"


// CViewStuffEnter

IMPLEMENT_DYNCREATE(CViewStuffEnter, CBCGPFormView)

CViewStuffEnter::CViewStuffEnter()
	: CBCGPFormView(CViewStuffEnter::IDD)
{
	EnableVisualManagerStyle();

	m_strName = "";
	m_strPSWD = "";
}

CViewStuffEnter::~CViewStuffEnter()
{
}

void CViewStuffEnter::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POS1, m_BKpos1);
	DDX_Control(pDX, IDC_POS2, m_BKpos2);
	DDX_Control(pDX, IDC_CONTENT, m_Content);

	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);

	DDX_Text(pDX, IDC_E1, m_strName);
	DDX_Text(pDX, IDC_E2, m_strPSWD);
}

BEGIN_MESSAGE_MAP(CViewStuffEnter, CBCGPFormView)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_LOGIN, &CViewStuffEnter::OnBnClickedLogin)
	ON_EN_CHANGE(IDC_E1, &CViewStuffEnter::OnEnChangeE1)
	ON_EN_CHANGE(IDC_E2, &CViewStuffEnter::OnEnChangeE2)
	ON_BN_CLICKED(IDC_NEWSTUFF, &CViewStuffEnter::OnBnClickedNewstuff)
	ON_BN_CLICKED(IDC_FORGET, &CViewStuffEnter::OnBnClickedForget)
END_MESSAGE_MAP()


HBRUSH CViewStuffEnter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //返回一个空画刷
				   break;
	}
	case IDC_S3:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(255, 0, 0));
				   CFont font1;
				   font1.CreateFontA(17, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //返回一个空画刷
				   break;
	}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

// CViewStuffEnter 诊断

#ifdef _DEBUG
void CViewStuffEnter::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewStuffEnter::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewStuffEnter 消息处理程序


void CViewStuffEnter::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


	//获取背景定位点位置
	m_BKpos1.GetClientRect(&m_rctBK1);
	m_BKpos1.MapWindowPoints(this, &m_rctBK1);
	m_BKpos2.GetClientRect(&m_rctBK2);
	m_BKpos2.MapWindowPoints(this, &m_rctBK2);

	m_Content.GetClientRect(&m_rctContent);
	m_Content.MapWindowPoints(this, &m_rctContent);
}


void CViewStuffEnter::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	MapWindowPoints(this, &rect);

	CDC     MemDC;
	//HBITMAP hbitmap;
	CBitmap bitmp;
	MemDC.CreateCompatibleDC(&dc);
	//bitmp.LoadBitmapA(IDB_BITMAP1);
	//BITMAP bmp;
	//bitmp.GetBitmap(&bmp); //获取bmp参数

	Point p1(m_rctBK1.left, m_rctBK1.top);
	Point p2(m_rctBK2.right, m_rctBK2.bottom);
	int width = m_rctBK2.right - m_rctBK1.left;
	int height = m_rctBK2.bottom - m_rctBK1.top;

	bitmp.CreateCompatibleBitmap(&dc, width, height);
	MemDC.SelectObject(&bitmp);

	Graphics graph(MemDC.m_hDC);

	Image img(L"res//0.bmp");
	graph.DrawImage(&img, Rect(p1.X, p1.Y, width, height));

	SolidBrush brush(Color(150, 230, 230, 230));
	graph.FillRectangle(&brush, m_rctContent.left, m_rctContent.top, m_rctContent.Width(), m_rctContent.Height());


	//复制内存DC到屏幕上
	CPoint pos = GetScrollPosition();
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);
	//SetStretchBltMode(dc.m_hDC, STRETCH_HALFTONE);
	//dc.StretchBlt(p1.X, p1.Y, width, height, &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	bitmp.DeleteObject();
	MemDC.DeleteDC();
}



void CViewStuffEnter::OnBnClickedLogin()
{
	UpdateData();

	if (m_strName.IsEmpty() || m_strPSWD.IsEmpty())
	{
		m_S[2].ShowWindow(TRUE);
		return;
	}

	CString strMsg, strSQL;
	strSQL.Format("SELECT * FROM stuff WHERE UNAME='%s' AND UPSWD=PASSWORD('%s')", m_strName, m_strPSWD);
	if (g_mysqlCon.ExecuteQueryExist(strSQL, strMsg))
	{
		strMsg.Format("欢迎您， %s", m_strName);
		MessageBox(strMsg);
	}
	else
	{
		m_S[2].ShowWindow(TRUE);
	}
	ShowMsg2Output1(strMsg);
}


void CViewStuffEnter::OnEnChangeE1()
{
	m_S[2].ShowWindow(FALSE);
}


void CViewStuffEnter::OnEnChangeE2()
{
	m_S[2].ShowWindow(FALSE);
}


void CViewStuffEnter::OnBnClickedNewstuff()
{
	CDlgNewStuff dlg;
	dlg.DoModal();
}


void CViewStuffEnter::OnBnClickedForget()
{
	CDlgResetPSW dlg;
	dlg.DoModal();
}