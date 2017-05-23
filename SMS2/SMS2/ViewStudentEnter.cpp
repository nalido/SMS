// ViewStudentEnter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewStudentEnter.h"
#include "MainFrm.h"


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
	DDX_Control(pDX, IDC_CONTENT, m_Content);

	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);
	DDX_Control(pDX, IDC_S5, m_S[4]);

	DDX_Text(pDX, IDC_E1, m_strName);
	DDX_Text(pDX, IDC_E2, m_strIDCard);
	DDX_Text(pDX, IDC_E3, m_strTEL);
}

BEGIN_MESSAGE_MAP(CViewStudentEnter, CBCGPFormView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CONFIRM, &CViewStudentEnter::OnBnClickedConfirm)
	ON_EN_CHANGE(IDC_E1, &CViewStudentEnter::OnEnChangeE1)
	ON_EN_CHANGE(IDC_E2, &CViewStudentEnter::OnEnChangeE2)
	ON_EN_CHANGE(IDC_E3, &CViewStudentEnter::OnEnChangeE3)
END_MESSAGE_MAP()

LRESULT CViewStudentEnter::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;
	if (flag == 1)
		m_nEraseBkgnd = 0; //���ػ汳�� ������˸

	return 0;
}


BOOL CViewStudentEnter::OnEraseBkgnd(CDC* pDC)
{
	if (m_nEraseBkgnd<2)
	{
		m_nEraseBkgnd++;
		return TRUE;
	}
	else
		return CBCGPFormView::OnEraseBkgnd(pDC);
}


// CViewStudentEnter ���

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


// CViewStudentEnter ��Ϣ�������


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
	int  W = GetSystemMetrics(SM_CXSCREEN);  //�õ���Ļ��� 
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
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
	CFont* oldF = MemDC.SelectObject(&fontt1);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextAlign(TA_CENTER);
	CString stri;
	stri.Format("�� �� �� У �� �� ϵ ͳ");
	CPoint pT(width / 2, height / 4);
	MemDC.SetTextColor(RGB(241, 241, 241));
	MemDC.TextOutA(pT.x + 3, pT.y + 3, stri);
	MemDC.SetTextColor(RGB(51, 103, 155));
	MemDC.SetTextColor(RGB(65, 57, 36));
	MemDC.TextOutA(pT.x, pT.y, stri);

	SolidBrush brush(Color(150, 230, 230, 230));
	graph.FillRectangle(&brush, m_rctContent.left, m_rctContent.top, m_rctContent.Width(), m_rctContent.Height());

	//�����ڴ�DC����Ļ��
	CPoint pos = GetScrollPosition();
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);


	bitmp.DeleteObject();
	MemDC.DeleteDC();
}


void CViewStudentEnter::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


	//��ȡ������λ��λ��
	m_BKpos1.GetClientRect(&m_rctBK1);
	m_BKpos1.MapWindowPoints(this, &m_rctBK1);
	m_BKpos2.GetClientRect(&m_rctBK2);
	m_BKpos2.MapWindowPoints(this, &m_rctBK2);
	m_Content.GetClientRect(&m_rctContent);
	m_Content.MapWindowPoints(this, &m_rctContent);
}




HBRUSH CViewStudentEnter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S4:
	case IDC_S5:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //����һ���ջ�ˢ
				   break;
	}
	case IDC_S3:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(255, 0, 0));
				   CFont font1;
				   font1.CreateFontA(17, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //����һ���ջ�ˢ
				   break;
	}
	
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CViewStudentEnter::OnBnClickedConfirm()
{
	UpdateData();

	if (m_strIDCard.IsEmpty() && m_strName.IsEmpty() && m_strTEL.IsEmpty()) return;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CString strMsg, strSQL;
	CDStrs datas;
	STUDENTINFO student;
	if (!m_strIDCard.IsEmpty())
	{
		strSQL.Format("SELECT SNAME, GENDER, CAR_TYPE, FILE_NAME \
					  	FROM students WHERE ID='%s'", m_strIDCard);
		if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg) && datas.size()>0)
		{
			student.strName = datas[0][0];
			student.strGender = datas[0][1];
			student.strCarType = datas[0][2];
			student.strFileName = datas[0][3];

			pFrame->SelectView(VIEW_SCAN);
			CView* pView = (CView*)pFrame->GetActiveView();
			pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)VIEW_STUDENTENTER, (LPARAM)3);
			pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)&student, (LPARAM)1);

		}
		else
		{
			m_S[2].ShowWindow(TRUE);
		}
	}
	else if (!m_strName.IsEmpty())
	{
		strSQL.Format("SELECT SNAME, GENDER, CAR_TYPE \
					  	FROM students WHERE FILE_NAME='%s'", m_strName);
		if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg) && datas.size()>0)
		{
			student.strName = datas[0][0];
			student.strGender = datas[0][1];
			student.strCarType = datas[0][2];
			student.strFileName = m_strName;

			pFrame->SelectView(VIEW_SCAN);
			CView* pView = (CView*)pFrame->GetActiveView();
			pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)VIEW_STUDENTENTER, (LPARAM)3);
			pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)&student, (LPARAM)1);
		}
		else
		{
			m_S[2].ShowWindow(TRUE);
		}
	}
	else if (!m_strTEL.IsEmpty())
	{
		strSQL.Format("SELECT SNAME, GENDER, CAR_TYPE, FILE_NAME \
					  	FROM students WHERE TEL='%s'", m_strTEL);
		if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg) && datas.size()>0)
		{
			student.strName = datas[0][0];
			student.strGender = datas[0][1];
			student.strCarType = datas[0][2];
			student.strFileName = datas[0][3];

			pFrame->SelectView(VIEW_SCAN);
			CView* pView = (CView*)pFrame->GetActiveView();
			pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)VIEW_STUDENTENTER, (LPARAM)3);
			pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)&student, (LPARAM)1);
		}
		else
		{
			m_S[2].ShowWindow(TRUE);
		}
	}

	m_strIDCard = "";
	m_strName = "";
	m_strTEL = "";
	UpdateData(FALSE);
}


void CViewStudentEnter::OnEnChangeE1()
{
	m_S[2].ShowWindow(FALSE);
}


void CViewStudentEnter::OnEnChangeE2()
{
	m_S[2].ShowWindow(FALSE);
}


void CViewStudentEnter::OnEnChangeE3()
{
	m_S[2].ShowWindow(FALSE);
}
