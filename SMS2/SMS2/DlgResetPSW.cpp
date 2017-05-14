// DlgResetPSW.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgResetPSW.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgResetPSW 对话框

IMPLEMENT_DYNAMIC(CDlgResetPSW, CBCGPDialog)

CDlgResetPSW::CDlgResetPSW(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgResetPSW::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgResetPSW::~CDlgResetPSW()
{
}

void CDlgResetPSW::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);
	DDX_Control(pDX, IDC_S5, m_S[4]);
	DDX_Control(pDX, IDC_S6, m_S[5]);

	DDX_Text(pDX, IDC_E1, m_strTel);
	DDX_Text(pDX, IDC_E2, m_strSMS);
	DDX_Text(pDX, IDC_E3, m_strPSW1);
	DDX_Text(pDX, IDC_E4, m_strPSW2);
}


BEGIN_MESSAGE_MAP(CDlgResetPSW, CBCGPDialog)
	ON_BN_CLICKED(IDC_SMS, &CDlgResetPSW::OnBnClickedSms)
	ON_BN_CLICKED(IDC_CONFIRM, &CDlgResetPSW::OnBnClickedConfirm)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RESET, &CDlgResetPSW::OnBnClickedReset)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_E3, &CDlgResetPSW::OnEnChangeE3)
	ON_EN_CHANGE(IDC_E4, &CDlgResetPSW::OnEnChangeE4)
END_MESSAGE_MAP()


HBRUSH CDlgResetPSW::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S3:
	case IDC_S4:
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
	case IDC_S5:
	case IDC_S6:
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

// CDlgResetPSW 消息处理程序


void CDlgResetPSW::OnBnClickedSms()
{
	UpdateData();

	if (m_strTel.IsEmpty())
		return;


	CString strMsg, strSQL;
	CDStrs datas;
	strSQL.Format("SELECT UPSWD FROM stuff WHERE UTEL='%s'", m_strTel);
	if (!g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg))
	{
		strMsg.Format("此手机号无注册记录，请重新输入");
		MessageBox(strMsg);
		return;
	}
	if (datas.size() == 0)
	{
		strMsg.Format("此手机号无注册记录，请重新输入");
		MessageBox(strMsg);
		return;
	}

	GetDlgItem(IDC_CONFIRM)->EnableWindow(TRUE);
	m_S[5].ShowWindow(FALSE);
	GetDlgItem(IDC_SMS)->EnableWindow(FALSE);
	m_nCount = 60;
	SetTimer(0, 1000, NULL);

	//数据打包
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if (pFrame->m_pSendBuf != NULL)
	{
		MessageBox("上一个信息还未处理完毕，请稍等重试。");
		return;
	}
	else
	{
		CString strSMS;
		m_strSMS0 = datas[0][0].Right(4);
		strSMS.Format("%s>%s", m_strTel, m_strSMS0);
		int nCount = strlen(strSMS);
		int len = 6 + nCount; //Type(1) Flag(1) Number(4) FileNums(Number*8)
		pFrame->m_isSendReady = FALSE;
		pFrame->m_pSendBuf = new BYTE[len];//发送完删除
		pFrame->m_nSendLen = len;
		pFrame->m_pSendBuf[0] = 2; //发送短信平台数据
		pFrame->m_pSendBuf[1] = 6; //验证码短信
		memcpy(pFrame->m_pSendBuf + 2, &nCount, 4); //字符串长度

		char* data = strSMS.GetBuffer();
		memcpy(pFrame->m_pSendBuf + 6, data, nCount);
		m_strSMS0.ReleaseBuffer();
		pFrame->m_isSendReady = TRUE;
	}
}


void CDlgResetPSW::OnBnClickedConfirm()
{
	UpdateData();

	if (m_strSMS.IsEmpty())
	{
		MessageBox("请输入验证码");
		return;
	}

	m_strSMS.MakeLower();
	m_strSMS0.MakeLower();
	if (m_strSMS == m_strSMS0)
	{
		m_S[2].ShowWindow(TRUE);
		m_S[3].ShowWindow(TRUE);
		GetDlgItem(IDC_E3)->ShowWindow(TRUE);
		GetDlgItem(IDC_E4)->ShowWindow(TRUE);
		GetDlgItem(IDC_RESET)->ShowWindow(TRUE);
	}
	else
	{
		m_S[5].ShowWindow(TRUE);
	}
}




BOOL CDlgResetPSW::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_S[2].ShowWindow(FALSE);
	m_S[3].ShowWindow(FALSE);
	GetDlgItem(IDC_E3)->ShowWindow(FALSE);
	GetDlgItem(IDC_E4)->ShowWindow(FALSE);
	GetDlgItem(IDC_RESET)->ShowWindow(FALSE);

	GetDlgItem(IDC_CONFIRM)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgResetPSW::OnBnClickedReset()
{

	UpdateData();

	if (m_strPSW1.IsEmpty() || m_strPSW2.IsEmpty() || m_strPSW1 != m_strPSW2)
	{
		m_S[4].ShowWindow(TRUE);
		return;
	}

	CString strMsg, strSQL;
	strSQL.Format("UPDATE stuff SET UPSWD=PASSOWRD('%s') WHERE UTEL='%s'", m_strPSW1, m_strTel);
	if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
	{
		strMsg.Format("修改密码成功，您可以登录了");
		MessageBox(strMsg);
		return;
	}
	else
	{
		strMsg.Format("修改密码失败，请检查网络");
		MessageBox(strMsg);
		return;
	}
}


void CDlgResetPSW::OnTimer(UINT_PTR nIDEvent)
{
	m_nCount--;
	CString str; 
	if (m_nCount <= 0)
	{
		KillTimer(0);
		str.Format("获取验证码");
		GetDlgItem(IDC_SMS)->SetWindowTextA(str);
		GetDlgItem(IDC_SMS)->EnableWindow(TRUE);
	}
	else
	{
		str.Format("%02d秒后重发", m_nCount);
		GetDlgItem(IDC_SMS)->SetWindowTextA(str);
	}

	CBCGPDialog::OnTimer(nIDEvent);
}


void CDlgResetPSW::OnEnChangeE3()
{
	m_S[4].ShowWindow(FALSE);
}


void CDlgResetPSW::OnEnChangeE4()
{
	m_S[4].ShowWindow(FALSE);
}
