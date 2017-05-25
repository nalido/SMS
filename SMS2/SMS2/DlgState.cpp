// DlgState.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgState.h"
#include "afxdialogex.h"


// CDlgState 对话框

IMPLEMENT_DYNAMIC(CDlgState, CBCGPDialog)

CDlgState::CDlgState(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgState::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgState::~CDlgState()
{
}

void CDlgState::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nStat);
}


BEGIN_MESSAGE_MAP(CDlgState, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CDlgState::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgState 消息处理程序


BOOL CDlgState::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_nStat = 0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgState::OnBnClickedOk()
{
	UpdateData();
	OnOK();
}
