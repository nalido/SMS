// MSGINFO.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "MSGINFO.h"
#include "afxdialogex.h"


// CMSGINFO �Ի���

IMPLEMENT_DYNAMIC(CMSGINFO, CBCGPDialog)

CMSGINFO::CMSGINFO(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CMSGINFO::IDD, pParent)
{
	EnableVisualManagerStyle();
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


// CMSGINFO ��Ϣ�������
