// TmpOrder.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "TmpOrder.h"
#include "afxdialogex.h"


// CTmpOrder �Ի���

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


// CTmpOrder ��Ϣ�������
