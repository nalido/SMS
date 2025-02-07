// MSGINFO.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "MSGINFO.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CMSGINFO 对话框

IMPLEMENT_DYNAMIC(CMSGINFO, CBCGPDialog)

CMSGINFO::CMSGINFO(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CMSGINFO::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_nFlag = 0;
	m_strSMS = "";
}

CMSGINFO::~CMSGINFO()
{
}

void CMSGINFO::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_S1, m_S[6]);
	DDX_Control(pDX, IDC_S2, m_S[0]);
	DDX_Control(pDX, IDC_S3, m_S[1]);
	DDX_Control(pDX, IDC_S4, m_S[2]);
	DDX_Control(pDX, IDC_S5, m_S[3]);
	DDX_Control(pDX, IDC_S6, m_S[4]);
	DDX_Control(pDX, IDC_S7, m_S[5]);
	DDX_Control(pDX, IDC_DATE, m_wndDatePicker);
	DDX_Control(pDX, IDC_TIME, m_wndTimePicker);


	DDX_Control(pDX, IDC_E1, m_E[0]);
	DDX_Control(pDX, IDC_E2, m_E[1]);
	DDX_Control(pDX, IDC_E3, m_E[2]);
	DDX_Control(pDX, IDC_E4, m_E[3]);

	DDX_Control(pDX, IDC_SMSINFO, m_SMS);
	DDX_Control(pDX, IDC_COMBO1, m_Comb);

	DDX_Text(pDX, IDC_E1, m_strE[0]);
	DDX_Text(pDX, IDC_E2, m_strE[1]);
	DDX_Text(pDX, IDC_E3, m_strE[2]);
	DDX_Text(pDX, IDC_E4, m_strE[3]);

	DDX_Text(pDX, IDC_S2, m_strHints[0]);
	DDX_Text(pDX, IDC_S3, m_strHints[1]);
	DDX_Text(pDX, IDC_S4, m_strHints[2]);
	DDX_Text(pDX, IDC_S5, m_strHints[3]);
	DDX_Text(pDX, IDC_S6, m_strHints[4]);
	DDX_Text(pDX, IDC_S7, m_strHints[5]);

	DDX_Text(pDX, IDC_SMSINFO, m_strSMS);
}


BEGIN_MESSAGE_MAP(CMSGINFO, CBCGPDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_DATE, &CMSGINFO::OnBnClickedDate)
	ON_BN_CLICKED(IDC_TIME, &CMSGINFO::OnBnClickedTime)
	ON_EN_CHANGE(IDC_E1, &CMSGINFO::OnEnChangeE1)
	ON_EN_CHANGE(IDC_E2, &CMSGINFO::OnEnChangeE2)
	ON_EN_CHANGE(IDC_E3, &CMSGINFO::OnEnChangeE3)
	ON_EN_CHANGE(IDC_E4, &CMSGINFO::OnEnChangeE4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMSGINFO::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CMSGINFO::OnBnClickedOk)
END_MESSAGE_MAP()


// CMSGINFO 消息处理程序


BOOL CMSGINFO::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	UINT stateFlags = 0;
	stateFlags |= (CBCGPDateTimeCtrl::DTM_SPIN | CBCGPDateTimeCtrl::DTM_TIME);
	const UINT stateMask =
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR |
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME |
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE |
		CBCGPDateTimeCtrl::DTM_SECONDS;
	m_wndTimePicker.SetState(stateFlags, stateMask);
	m_wndTimePicker.SizeToContent();

	stateFlags = 0;
	stateFlags |= (
		CBCGPDateTimeCtrl::DTM_SPIN | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR);
	m_wndDatePicker.SetState(stateFlags, stateMask);
	m_wndDatePicker.SizeToContent();
	m_wndDatePicker.m_monthFormat = 2;

	CString strKeyName("");

	m_strHints[0] = "______";
	m_strHints[1] = "______";
	m_strHints[2] = "______";
	m_strHints[3] = "______";
	m_strHints[4] = "______";
	m_strHints[5] = "______";

	m_S[0].EnableWindow(FALSE);
	m_S[1].EnableWindow(FALSE);
	m_S[2].EnableWindow(FALSE);
	m_S[3].EnableWindow(FALSE);
	m_S[4].EnableWindow(FALSE);
	m_S[5].EnableWindow(FALSE);

	m_E[0].EnableWindow(FALSE);
	m_E[1].EnableWindow(FALSE);
	m_E[2].EnableWindow(FALSE);
	m_E[3].EnableWindow(FALSE);

	m_arrComb.push_back("科目一");
	m_arrComb.push_back("科目二");
	m_arrComb.push_back("科目三");
	m_Comb.AddString("科目一");
	m_Comb.AddString("科目二");
	m_Comb.AddString("科目三");
	m_Comb.EnableWindow(FALSE);
	switch (m_nFlag)
	{
	case 1: //开班短信
	{
				m_strHints[0] = "学员期数";
				m_strHints[5] = "上课时间";

				m_S[0].EnableWindow(TRUE);
				m_E[0].EnableWindow(TRUE);
				m_S[5].EnableWindow(TRUE);

				strKeyName = "K1PrePass";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");

				strKeyName = "K1Address";
				CString str = xPublic::GETSTR2("SMS", strKeyName, "failed");
				m_strSMSTmp.Replace("A", str);

				m_strSMSTmp.Replace("N", m_strStu);

				m_strE[0] = m_strClassIssue;
				m_strSMS = m_strSMSTmp;
	}
		break;
	case 2: //退款短信
	{
				m_strHints[5] = "退款时间";

				m_S[5].EnableWindow(TRUE);

				strKeyName = "K1NOPass";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");

				m_strSMSTmp.Replace("N", m_strStu);

				m_strSMS = m_strSMSTmp;
	}
		break;
	case 3: //科一考试短信
	{
				m_strHints[5] = "考试时间";

				m_S[5].EnableWindow(TRUE);

				strKeyName = "K1Exame";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
				m_strSMSTmp.Replace("N", m_strStu);
				m_strSMS = m_strSMSTmp;
	}
		break;
	case 4: //培训预约短信
	{
				m_strHints[5] = "预约时间";

				m_S[5].EnableWindow(TRUE);

				strKeyName = "K23Booking";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
				m_strSMSTmp.Replace("N", m_strStu);
				m_strSMS = m_strSMSTmp;
	}
		break;
	case 44: //非第一次预约
	{
				 m_strHints[5] = "预约时间";

				 m_S[5].EnableWindow(TRUE);

				 m_wndTimePicker.EnableWindow(FALSE);

				 strKeyName = "K23Booking2";
				 m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
				 m_strSMSTmp.Replace("N", m_strStu);
				 m_strSMS = m_strSMSTmp;
	}
		break;
	case 45: //非第一次预约
	{
				 m_wndDatePicker.EnableWindow(FALSE);
				 m_wndTimePicker.EnableWindow(FALSE);

				 strKeyName = "K23Booking2";
				 m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
				 m_strSMSTmp.Replace("N", m_strStu);
				 m_strSMSTmp.Replace("T", m_strDate);
				 m_strSMS = m_strSMSTmp;
	}
		break;
	case 5: //上课提醒短信
		strKeyName = "K1PrePass";
		break;
	case 6: //科二考试短信
	{
				m_strHints[5] = "考试时间";

				m_S[5].EnableWindow(TRUE);

				strKeyName = "K2Exame";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
				m_strSMSTmp.Replace("N", m_strStu);
				m_strSMS = m_strSMSTmp;
	}
		break;
	case 7: //科三考试短信
	{
				m_strHints[5] = "考试时间";

				m_S[5].EnableWindow(TRUE);

				strKeyName = "K3Exame";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
				m_strSMSTmp.Replace("N", m_strStu);
				m_strSMS = m_strSMSTmp;
	}
		break;
	}

	//char* strSMS = EncodeToUTF8(m_strSMS);

	UpdateData(FALSE); //显示预设信息
	UpdateSMS();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CMSGINFO::Init(int flag)
{
	CString strKeyName("");
	switch (flag)
	{
	case 5: //上课提醒短信
		strKeyName = "NextClass";
		m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
		m_strSMSTmp.Replace("N", m_strStu);
		m_strSMSTmp.Replace("L", m_strCoach); 
		m_strSMSTmp.Replace("M", m_strCar);
		m_strSMSTmp.Replace("K", m_strClassIndex);
		m_strDate.Replace("A", "上午");
		m_strDate.Replace("P", "下午");
		m_strSMSTmp.Replace("T", m_strDate);

		m_strSMS = m_strSMSTmp;
		break;
	}
}

HBRUSH CMSGINFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S3:
	case IDC_S4:
	case IDC_S5:
	case IDC_S6:
	case IDC_S7:
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
	case IDC_SMSINFO:
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
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CMSGINFO::OnBnClickedDate()
{
	UpdateSMS();
}


void CMSGINFO::OnBnClickedTime()
{
	UpdateSMS();
}

void CMSGINFO::UpdateSMS()
{
	UpdateData(); //获得输入
	m_strSMS = m_strSMSTmp;

	COleDateTime dt = m_wndTimePicker.GetDate();

	CString AP = dt.Format("%p");
	if (AP == "AM")
	{
		AP = "上午";
	}
	else
		AP = "下午";
	CString strTime = dt.Format("%I点%M分");
	m_strTime = AP + strTime;

	COleDateTime dd = m_wndDatePicker.GetDate();
	m_strDate = dd.Format("%Y年%m月%d日");

	switch (m_nFlag)
	{
	case 1: //开班短信
	{
			  CString str = m_strDate + m_strTime;
			  m_strSMS.Replace("T", str);
			  m_strSMS.Replace("Q", m_strE[0]);
			  m_strClassIssue = m_strE[0];
	}
		break;
	case 2: //退款短信
	{
				m_strSMS.Replace("T", m_strDate);
				m_strSMS.Replace("R", m_strReason);
	}
		break;
	case 3: //科一考试短信
	case 6: //科二考试短信
	case 7: //科三考试短信
	case 4: //培训预约短信
	{
				CString str = m_strDate + m_strTime;
				m_strSMS.Replace("T", str);
	}
		break;
	case 44: //培训预约短信
	{
				m_strSMS.Replace("T", m_strDate);
	}
		break;
	case 45: //培训预约短信
		break;
	case 5: //上课提醒短信
		
		break;
	}

	//刷新
	CRect rect;
	m_SMS.GetClientRect(&rect);
	m_SMS.MapWindowPoints(this, &rect);

	UpdateData(FALSE);
	InvalidateRect(&rect);
}



void CMSGINFO::OnEnChangeE1()
{
	UpdateData();

	UpdateSMS();
}


void CMSGINFO::OnEnChangeE2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMSGINFO::OnEnChangeE3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMSGINFO::OnEnChangeE4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMSGINFO::OnCbnSelchangeCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CMSGINFO::OnBnClickedOk()
{
	switch (m_nFlag)
	{
	case 1: //开班短信
	{		
				if (m_strClassIssue.IsEmpty())
				{
					MessageBox("请输入学员期数");
					return;
				}

	}
		break;
	}

	OnOK();
}
