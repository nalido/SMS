// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgProxy.h"
#include "afxdialogex.h"


// CDlgProxy 对话框

IMPLEMENT_DYNAMIC(CDlgProxy, CBCGPDialog)

CDlgProxy::CDlgProxy(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgProxy::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgProxy::~CDlgProxy()
{
}

void CDlgProxy::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProxy, CBCGPDialog)
END_MESSAGE_MAP()


// CDlgProxy 消息处理程序
