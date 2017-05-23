// DlgState.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgState.h"
#include "afxdialogex.h"


// CDlgState �Ի���

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


// CDlgState ��Ϣ�������


BOOL CDlgState::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_nStat = 0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgState::OnBnClickedOk()
{
	UpdateData();
	OnOK();
}
