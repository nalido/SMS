// Response.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "Response.h"
#include "afxdialogex.h"


// CResponse �Ի���

IMPLEMENT_DYNAMIC(CResponse, CBCGPDialog)

CResponse::CResponse(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CResponse::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_nRspType = 0;
}

CResponse::~CResponse()
{
}

void CResponse::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nRspType);

}


BEGIN_MESSAGE_MAP(CResponse, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CResponse::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CResponse::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CResponse::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CResponse::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CResponse ��Ϣ�������


void CResponse::OnBnClickedOk()
{
	UpdateData();

	if (m_nRspType != 0)
	{
		m_strServiceScore = "δ����";
		m_strSelfScore = "δ����";
	}
	else
	{
		GetDlgItem(IDC_SERVICE_SCORE)->GetWindowTextA(m_strServiceScore);
		GetDlgItem(IDC_SELF_SCORE)->GetWindowTextA(m_strSelfScore);

		if (m_strServiceScore.IsEmpty() || m_strSelfScore.IsEmpty())
		{
			MessageBox("����д����!");
			return;
		}
	}

	OnOK();
}


void CResponse::OnBnClickedRadio1()
{
	GetDlgItem(IDC_SERVICE_SCORE)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELF_SCORE)->EnableWindow(TRUE);
}


void CResponse::OnBnClickedRadio2()
{
	GetDlgItem(IDC_SERVICE_SCORE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELF_SCORE)->EnableWindow(FALSE);
}


void CResponse::OnBnClickedRadio3()
{
	GetDlgItem(IDC_SERVICE_SCORE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELF_SCORE)->EnableWindow(FALSE);
}
