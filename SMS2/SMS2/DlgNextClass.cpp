// DlgNextClass.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgNextClass.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgNextClass 对话框

IMPLEMENT_DYNAMIC(CDlgNextClass, CBCGPDialog)

CDlgNextClass::CDlgNextClass(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgNextClass::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_strSelectedClass = "";
	for (int i = 0; i < 8; i++)
	{
		m_arrClassText.push_back("");
	}
	m_nFlag = 0;
}

CDlgNextClass::~CDlgNextClass()
{
}

void CDlgNextClass::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);
	DDX_Control(pDX, IDC_S5, m_S[4]);
	DDX_Control(pDX, IDC_S6, m_S[5]);
	DDX_Control(pDX, IDC_S7, m_S[6]);
	DDX_Control(pDX, IDC_S8, m_S[7]);
	DDX_Control(pDX, IDC_S9, m_S[8]);
	DDX_Control(pDX, IDC_S10, m_S[9]);
	DDX_Control(pDX, IDC_S11, m_S[10]);


	DDX_Text(pDX, IDC_S3, m_arrClassText[0]);
	DDX_Text(pDX, IDC_S4, m_arrClassText[1]);
	DDX_Text(pDX, IDC_S5, m_arrClassText[2]);
	DDX_Text(pDX, IDC_S6, m_arrClassText[3]);
	DDX_Text(pDX, IDC_S7, m_arrClassText[4]);
	DDX_Text(pDX, IDC_S8, m_arrClassText[5]);
	DDX_Text(pDX, IDC_S9, m_arrClassText[6]);
	DDX_Text(pDX, IDC_S10, m_arrClassText[7]);


	DDX_Control(pDX, IDC_E1, m_E[0]);
	DDX_Control(pDX, IDC_E2, m_E[1]);
	DDX_Control(pDX, IDC_E3, m_E[2]);
	DDX_Control(pDX, IDC_E4, m_E[3]);
	DDX_Control(pDX, IDC_E5, m_E[4]);
	DDX_Control(pDX, IDC_E6, m_E[5]);
	DDX_Control(pDX, IDC_E7, m_E[6]);
	DDX_Control(pDX, IDC_E8, m_E[7]);
	DDX_Text(pDX, IDC_E1, m_arrClassText[0]);
	DDX_Text(pDX, IDC_E2, m_arrClassText[1]);
	DDX_Text(pDX, IDC_E3, m_arrClassText[2]);
	DDX_Text(pDX, IDC_E4, m_arrClassText[3]);
	DDX_Text(pDX, IDC_E5, m_arrClassText[4]);
	DDX_Text(pDX, IDC_E6, m_arrClassText[5]);
	DDX_Text(pDX, IDC_E7, m_arrClassText[6]);
	DDX_Text(pDX, IDC_E8, m_arrClassText[7]);

	DDX_Control(pDX, IDC_COMBO1, m_Comb);
}


BEGIN_MESSAGE_MAP(CDlgNextClass, CBCGPDialog)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgNextClass::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CDlgNextClass::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CDlgNextClass::OnBnClickedOk2)
END_MESSAGE_MAP()


// CDlgNextClass 消息处理程序


HBRUSH CDlgNextClass::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S11:
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
	case IDC_S4:
	case IDC_S5:
	case IDC_S6:
	case IDC_S7:
	case IDC_S8:
	case IDC_S9:
	case IDC_S10:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(25, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //返回一个空画刷
				   break;
	}
	case IDC_E1:
	case IDC_E2:
	case IDC_E3:
	case IDC_E4:
	case IDC_E5:
	case IDC_E6:
	case IDC_E7:
	case IDC_E8:
	{
				   //pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(25, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
				   //pDC->SelectObject(&font1);
				   //pWnd->SetFont(&font1);
				   //return HBRUSH(GetStockObject(NULL_BRUSH)); //返回一个空画刷
				   break;
	}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CDlgNextClass::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	for (int i = 0; i < g_nMaxBooking; i++)
	{
		CString str;
		str.Format("%d", i + 1);
		m_Comb.AddString(str);
	}

	m_fontE.CreateFontA(24, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
	for (int i = 0; i < 8; i++)
	{
		m_arrClassText.push_back("");
		m_E[i].SetFont(&m_fontE);
		m_E[i].SetLimitText(23);
	}

	//窗口大小设置
	if (m_nFlag == 0)
	{
		CRect rect;
		GetDlgItem(IDC_LINE)->GetClientRect(&rect);
		GetDlgItem(IDC_LINE)->MapWindowPoints(this, &rect);
		int nHeight = GetSystemMetrics(SM_CYCAPTION); //标题高度
		SetWindowPos(NULL, 0, 0, rect.left, nHeight+rect.Height(), SWP_NOMOVE);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgNextClass::OnCbnSelchangeCombo1()
{
	int pos = m_Comb.GetCurSel();
	m_strSelectedClass.Format("%d", pos + 1);

	CString strClassID;
	strClassID.Format("CLASS%d", pos + 1);
	int nItem = xPublic::GETINT2(strClassID, "nItem", 0);
	CString strItemID;
	m_arrClassText.clear();
	for (int i = 1; i <= nItem; i++)
	{
		strItemID.Format("item%d", i);
		CString strClassText = xPublic::GETSTR2(strClassID, strItemID, "");
		m_arrClassText.push_back(strClassText);
	}

	int size = m_arrClassText.size();
	if (size < 8)
	{
		size = 8 - size;
		for (int i = 0; i < size; i++)
		{
			m_arrClassText.push_back("");
		}
	}

	UpdateData(FALSE);
	Invalidate();
}


void CDlgNextClass::OnBnClickedOk()
{
	UpdateData();
	int pos = m_Comb.GetCurSel();
	m_strSelectedClass.Format("%d", pos + 1);
	OnOK();
}


void CDlgNextClass::OnBnClickedOk2()
{
	UpdateData();

	int pos = m_Comb.GetCurSel();
	CString strClassID;
	strClassID.Format("CLASS%d", pos + 1);
	int nItem = m_arrClassText.size();
	xPublic::WRIINT2(strClassID, "nItem", nItem);
	for (int i = 0; i < nItem; i++)
	{
		CString strItemID;
		strItemID.Format("item%d", i+1);
		xPublic::WRISTR2(strClassID, strItemID, m_arrClassText[i]);
	}

	MessageBox("修改成功！");
}

