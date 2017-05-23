// DlgNextBookDate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgNextBookDate.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgNextBookDate �Ի���

IMPLEMENT_DYNAMIC(CDlgNextBookDate, CBCGPDialog)

CDlgNextBookDate::CDlgNextBookDate(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgNextBookDate::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgNextBookDate::~CDlgNextBookDate()
{
}

void CDlgNextBookDate::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E1, m_E);
	DDX_Control(pDX, IDC_DATE1, m_Date);

	DDX_Text(pDX, IDC_E1, m_strOldDate);
	
}


BEGIN_MESSAGE_MAP(CDlgNextBookDate, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CDlgNextBookDate::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNextBookDate ��Ϣ�������


BOOL CDlgNextBookDate::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_Date.SetFormat("yyyy/MM/dd");

	Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgNextBookDate::OnBnClickedOk()
{
	CString strNewDate;
	m_Date.GetWindowTextA(strNewDate);

	CString strMsg, strSQL;
	strSQL.Format("UPDATE stuDates SET BOOK_DATE='%s' WHERE STU_ID='%s'", strNewDate, m_strStuID);
	if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
	{
		MessageBox("���óɹ�!");
		OnOK();
	}
}

void CDlgNextBookDate::Refresh()
{
	CString strMsg, strSQL;
	strSQL.Format("SELECT BOOK_DATE FROM stuDates WHERE STU_ID='%s'", m_strStuID);
	CDStrs datas;
	if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg) && datas.size() > 0)
	{
		m_strOldDate = datas[0][0];
	}
	else
		m_strOldDate = "δ����";

	UpdateData(FALSE);
}