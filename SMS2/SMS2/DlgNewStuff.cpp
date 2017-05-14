// DlgNewStuff.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgNewStuff.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgNewStuff �Ի���

IMPLEMENT_DYNAMIC(CDlgNewStuff, CBCGPDialog)

CDlgNewStuff::CDlgNewStuff(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgNewStuff::IDD, pParent)
{
	EnableVisualManagerStyle();

}

CDlgNewStuff::~CDlgNewStuff()
{
}

void CDlgNewStuff::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);
	DDX_Control(pDX, IDC_S7, m_S[6]);
	DDX_Control(pDX, IDC_S8, m_S[7]);

	DDX_Text(pDX, IDC_E1, m_strName); 
	DDX_Text(pDX, IDC_E2, m_strID);
	DDX_Text(pDX, IDC_E3, m_strTel);
	DDX_Text(pDX, IDC_E4, m_strPSW1);
	DDX_Text(pDX, IDC_E5, m_strPSW2);
}


BEGIN_MESSAGE_MAP(CDlgNewStuff, CBCGPDialog)
	ON_BN_CLICKED(IDC_CANCEL, &CDlgNewStuff::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_REGISTER, &CDlgNewStuff::OnBnClickedRegister)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


HBRUSH CDlgNewStuff::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S3:
	case IDC_S4:
	case IDC_S7:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //����һ���ջ�ˢ
				   break;
	}
	case IDC_S8:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(255, 0, 0));
				   CFont font1;
				   font1.CreateFontA(17, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //����һ���ջ�ˢ
				   break;
	}
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
// CDlgNewStuff ��Ϣ�������


void CDlgNewStuff::OnBnClickedCancel()
{
	OnCancel();
}


void CDlgNewStuff::OnBnClickedRegister()
{
	UpdateData();
	if (m_strPSW1 != m_strPSW2)
	{
		m_S[7].ShowWindow(TRUE);
		return;
	}

	if (m_strID.IsEmpty() || m_strName.IsEmpty() || m_strPSW1.IsEmpty()
		|| m_strTel.IsEmpty())
	{
		MessageBox("������д������Ϣ��");
		return;
	}

	CString strMsg, strSQL;
	strSQL.Format("SELECT UPSWD FROM stuff WHERE UTEL='%s'", m_strTel);
	if (g_mysqlCon.ExecuteQueryExist(strSQL, strMsg))
	{
		strMsg.Format("���ֻ�������ע���¼������������");
		MessageBox(strMsg);
		return;
	}

	strSQL.Format("INSERT INTO stuff (UNAME, UPSWD, UID, UTEL) VALUES ('%s', PASSWORD('%s'), '%s', '%s')"
		, m_strName, m_strPSW1, m_strID, m_strTel);
	if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
	{
		strMsg.Format("ע��ɹ�������������ʹ��%s��¼��~", m_strName);
		MessageBox(strMsg);
		OnOK();
	}
	else
	{
		strMsg.Format("ע��ʧ�ܣ���������״̬����ע��");
		MessageBox(strMsg);
	}
	ShowMsg2Output1(strMsg);
}




BOOL CDlgNewStuff::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
