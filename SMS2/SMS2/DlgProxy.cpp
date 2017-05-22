// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgProxy.h"
#include "afxdialogex.h"


// CDlgProxy 对话框

IMPLEMENT_DYNAMIC(CDlgProxy, CDialog)

CDlgProxy::CDlgProxy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProxy::IDD, pParent)
{

}

CDlgProxy::~CDlgProxy()
{
}

void CDlgProxy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProxy, CDialog)
END_MESSAGE_MAP()


// CDlgProxy 消息处理程序
