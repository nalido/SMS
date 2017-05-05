// TmpOrder.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "TmpOrder.h"
#include "afxdialogex.h"


// CTmpOrder 对话框

IMPLEMENT_DYNAMIC(CTmpOrder, CBCGPDialog)

CTmpOrder::CTmpOrder(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CTmpOrder::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CTmpOrder::~CTmpOrder()
{
}

void CTmpOrder::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTmpOrder, CBCGPDialog)
END_MESSAGE_MAP()


// CTmpOrder 消息处理程序
