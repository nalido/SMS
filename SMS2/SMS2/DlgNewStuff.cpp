// DlgNewStuff.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgNewStuff.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgNewStuff 对话框

IMPLEMENT_DYNAMIC(CDlgNewStuff, CBCGPDialog)

CDlgNewStuff::CDlgNewStuff(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgNewStuff::IDD, pParent)
{
	EnableVisualManagerStyle();

}

CDlgNewStuff::~CDlgNewStuff()
{
}

void CDlgNewStuff::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);
	DDX_Control(pDX, IDC_S7, m_S[6]);
	DDX_Control(pDX, IDC_S8, m_S[7]);

	DDX_Text(pDX, IDC_E1, m_strName); 
	DDX_Text(pDX, IDC_E2, m_strID);
	DDX_Text(pDX, IDC_E3, m_strTel);
	DDX_Text(pDX, IDC_E4, m_strPSW1);
	DDX_Text(pDX, IDC_E5, m_strPSW2);
}


BEGIN_MESSAGE_MAP(CDlgNewStuff, CBCGPDialog)
	ON_BN_CLICKED(IDC_CANCEL, &CDlgNewStuff::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_REGISTER, &CDlgNewStuff::OnBnClickedRegister)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


HBRUSH CDlgNewStuff::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S3:
	case IDC_S4:
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
	case IDC_S8:
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
// CDlgNewStuff 消息处理程序


void CDlgNewStuff::OnBnClickedCancel()
{
	OnCancel();
}


void CDlgNewStuff::OnBnClickedRegister()
{
	UpdateData();
	if (m_strPSW1 != m_strPSW2)
	{
		m_S[7].ShowWindow(TRUE);
		return;
	}

	if (m_strID.IsEmpty() || m_strName.IsEmpty() || m_strPSW1.IsEmpty()
		|| m_strTel.IsEmpty())
	{
		MessageBox("必须填写所有信息！");
		return;
	}

	CString strMsg, strSQL;
	strSQL.Format("SELECT UPSWD FROM stuff WHERE UTEL='%s'", m_strTel);
	if (g_mysqlCon.ExecuteQueryExist(strSQL, strMsg))
	{
		strMsg.Format("此手机号已有注册记录，请重新输入");
		MessageBox(strMsg);
		return;
	}

	strSQL.Format("INSERT INTO stuff (UNAME, UPSWD, UID, UTEL) VALUES ('%s', PASSWORD('%s'), '%s', '%s')"
		, m_strName, m_strPSW1, m_strID, m_strTel);
	if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
	{
		strMsg.Format("注册成功！现在您可以使用%s登录啦~", m_strName);
		MessageBox(strMsg);
		OnOK();
	}
	else
	{
		strMsg.Format("注册失败！请检查网络状态后再注册");
		MessageBox(strMsg);
	}
	ShowMsg2Output1(strMsg);
}




BOOL CDlgNewStuff::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
