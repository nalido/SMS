// DlgDevice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgDevice.h"
#include "afxdialogex.h"


// CDlgDevice �Ի���

IMPLEMENT_DYNAMIC(CDlgDevice, CBCGPDialog)

CDlgDevice::CDlgDevice(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgDevice::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgDevice::~CDlgDevice()
{
}

void CDlgDevice::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDevice, CBCGPDialog)
END_MESSAGE_MAP()


// CDlgDevice ��Ϣ�������
