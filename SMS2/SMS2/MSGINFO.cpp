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
}


BEGIN_MESSAGE_MAP(CMSGINFO, CBCGPDialog)
END_MESSAGE_MAP()


// CMSGINFO 消息处理程序


BOOL CMSGINFO::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();


	CString strKeyName("");
	switch (m_nFlag)
	{
	case 1:
		strKeyName = "K1PrePass";
		break;
	}
	//m_strSMS = xPublic::GETSTR2("SMS", strKeyName, "failed");
	//char* strSMS = EncodeToUTF8(m_strSMS);
	GetDlgItem(IDC_SMSINFO)->SetWindowText(m_strSMS);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CMSGINFO::Init(int flag)
{
	m_nFlag = flag;
}