// DlgNoPass.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgNoPass.h"
#include "afxdialogex.h"


// CDlgNoPass 对话框

IMPLEMENT_DYNAMIC(CDlgNoPass, CBCGPDialog)

CDlgNoPass::CDlgNoPass(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgNoPass::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgNoPass::~CDlgNoPass()
{
}

void CDlgNoPass::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_S1, m_S1);
	DDX_Text(pDX, IDC_E1, m_strReason);
}


BEGIN_MESSAGE_MAP(CDlgNoPass, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CDlgNoPass::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNoPass 消息处理程序


void CDlgNoPass::OnBnClickedOk()
{
	UpdateData();
	OnOK();
}


BOOL CDlgNoPass::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	CString str;
	str.Format("请输入%s审核不通过的原因", m_strStuName);
	m_S1.SetWindowTextA(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
