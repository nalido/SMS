// DlgFee.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgFee.h"
#include "afxdialogex.h"


// CDlgFee �Ի���

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


// CDlgFee ��Ϣ�������


BOOL CDlgFee::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
