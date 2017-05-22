// DlgFee.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgFee.h"
#include "afxdialogex.h"


// CDlgFee 对话框

IMPLEMENT_DYNAMIC(CDlgFee, CBCGPDialog)

CDlgFee::CDlgFee(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgFee::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgFee::~CDlgFee()
{
}

void CDlgFee::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFee, CBCGPDialog)
END_MESSAGE_MAP()


// CDlgFee 消息处理程序


BOOL CDlgFee::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
