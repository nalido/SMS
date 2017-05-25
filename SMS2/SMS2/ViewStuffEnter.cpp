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
	m_isLoged = FALSE;

	m_nEraseBkgnd = 0;
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
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_LOGIN, &CViewStuffEnter::OnBnClickedLogin)
	ON_EN_CHANGE(IDC_E1, &CViewStuffEnter::OnEnChangeE1)
	ON_EN_CHANGE(IDC_E2, &CViewStuffEnter::OnEnChangeE2)
	ON_BN_CLICKED(IDC_NEWSTUFF, &CViewStuffEnter::OnBnClickedNewstuff)
	ON_BN_CLICKED(IDC_FORGET, &CViewStuffEnter::OnBnClickedForget)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

LRESULT CViewStuffEnter::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;
	if (flag == 1)
		m_nEraseBkgnd = 0; //不重绘背景 以免闪烁

	return 0;
}


BOOL CViewStuffEnter::OnEraseBkgnd(CDC* pDC)
{
	if (m_nEraseBkgnd<3)
	{
		m_nEraseBkgnd++;
		return TRUE;
	}
	else
		return CBCGPFormView::OnEraseBkgnd(pDC);
}

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

	SolidBrush brush(Color(150, 230, 230, 230));
	graph.FillRectangle(&brush, m_rctContent.left, m_rctContent.top, m_rctContent.Width(), m_rctContent.Height());

	if (m_isLoged)
	{
		CFont font1;
		font1.CreateFontA(40, 0, 0, 0, FW_BOLD, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
		CFont* oldF = MemDC.SelectObject(&font1);
		MemDC.SetTextAlign(TA_CENTER);
		MemDC.SetBkMode(TRANSPARENT);
		MemDC.SetTextColor(RGB(51, 103, 155));
		CPoint cp = m_rctContent.CenterPoint();
		CString stri;
		stri.Format("欢迎您，%s", m_strName);
		MemDC.TextOutA(cp.x, cp.y-40, stri);
	}

	//复制内存DC到屏幕上
	CPoint pos = GetScrollPosition();
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);
	


	bitmp.DeleteObject();
	MemDC.DeleteDC();
}



void CViewStuffEnter::OnBnClickedLogin()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (m_isLoged) //点击退出登录
	{
		m_isLoged = FALSE;
		InvalidateRect(&m_rctContent);
		m_S[0].ShowWindow(TRUE);
		m_S[1].ShowWindow(TRUE);
		GetDlgItem(IDC_E1)->ShowWindow(TRUE);
		GetDlgItem(IDC_E2)->ShowWindow(TRUE);
		GetDlgItem(IDC_NEWSTUFF)->ShowWindow(TRUE);
		GetDlgItem(IDC_FORGET)->ShowWindow(TRUE);
		GetDlgItem(IDC_LOGIN)->SetWindowTextA("登录");

		m_strName = "";
		m_strPSWD = "";
		UpdateData(FALSE);

		pFrame->PostMessageA(WM_USER_MESSAGE, (WPARAM)0);

		return;
	}

	UpdateData();

	if (m_strName.IsEmpty() || m_strPSWD.IsEmpty())
	{
		m_S[2].ShowWindow(TRUE);
		return;
	}

	CString strMsg, strSQL;
	CDStrs datas;
	strSQL.Format("SELECT UID, UPERMISSION, NEW_STUDENTS, STUDENTS, DEVICES, STUFF, ALL_STUDENTS, SCHOOL FROM stuff WHERE UNAME='%s' AND UPSWD=PASSWORD('%s')", m_strName, m_strPSWD);
	if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg))
	{
		//strMsg.Format("欢迎您， %s", m_strName);
		//MessageBox(strMsg);
		ShowMsg2Output1(strMsg);

		//查询权限
		if (datas.size() > 0)  //正确登录成功
		{
			m_isLoged = TRUE;
			g_strUserID = datas[0][0];
			int permission = atoi(datas[0][1]);
			for (int i = 0; i < 6; i++)
			{
				g_nPermissions[i] = atoi(datas[0][i + 2]);
			}
			pFrame->PostMessageA(WM_USER_MESSAGE, (WPARAM)permission);

			//隐藏登录框
			m_S[0].ShowWindow(FALSE);
			m_S[1].ShowWindow(FALSE);
			GetDlgItem(IDC_E1)->ShowWindow(FALSE);
			GetDlgItem(IDC_E2)->ShowWindow(FALSE);
			GetDlgItem(IDC_NEWSTUFF)->ShowWindow(FALSE);
			GetDlgItem(IDC_FORGET)->ShowWindow(FALSE);
			GetDlgItem(IDC_LOGIN)->SetWindowTextA("退出");
			InvalidateRect(&m_rctContent);
		}
		else
		{
			m_S[2].ShowWindow(TRUE);
		}

	}
	else
	{
		ShowMsg2Output1(strMsg);
		m_S[2].ShowWindow(TRUE);
	}
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

