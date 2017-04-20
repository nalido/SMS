// AddCoach.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "AddCoach.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CAddCoach �Ի���

IMPLEMENT_DYNAMIC(CAddCoach, CBCGPDialog)

CAddCoach::CAddCoach(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CAddCoach::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CAddCoach::~CAddCoach()
{
}

void CAddCoach::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NUM, m_SFilenum);
	DDX_Control(pDX, IDC_DATE_ENTER, m_Date_Sign);
	DDX_Control(pDX, IDC_GENDER, m_Comb_Gen);
	DDX_Control(pDX, IDC_NAME, m_SName);
	DDX_Control(pDX, IDC_DATE_BIRTH, m_Date_Birth);
	DDX_Control(pDX, IDC_TEL, m_STel);
	DDX_Control(pDX, IDC_HOME, m_SHome);
	DDX_Control(pDX, IDC_PARTY, m_SParty);
}


BEGIN_MESSAGE_MAP(CAddCoach, CBCGPDialog)
	ON_BN_CLICKED(IDC_ADD, &CAddCoach::OnBnClickedAdd)
END_MESSAGE_MAP()


// CAddCoach ��Ϣ�������


void CAddCoach::OnBnClickedAdd()
{
	UpdateData(TRUE); //��ȡ����


	CString name, tel, filenum, home, birth, date, gender, party;
	m_SName.GetWindowText(name);
	m_STel.GetWindowText(tel);
	m_SHome.GetWindowText(home);
	m_SParty.GetWindowText(party);
	m_SFilenum.GetWindowText(filenum);
	m_Comb_Gen.GetWindowText(gender);
	m_Date_Sign.GetWindowText(date);
	m_Date_Birth.GetWindowText(birth);
	if (name.IsEmpty() || filenum.IsEmpty() || tel.IsEmpty()
		|| party.IsEmpty() || home.IsEmpty()
		|| birth.IsEmpty() || date.IsEmpty() || gender.IsEmpty())
	{
		MessageBox("����ÿһ������ٵ����ӣ�");
		return;
	}
	else
	{
		CString strMsg("");
		CString strMsg1("");
		CString strSQL("");
		CString strSQL1("");
		strSQL.Format("INSERT INTO coachinfo \
			(SNAME, GENDER, BIRTH, PARTY_STAT, TEL, HOME, SIGN_DATE, FILE_NUM) \
			VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
			name, gender, birth, party, tel, home, date, filenum);
		strSQL1.Format("ALTER TABLE coachstat ADD COLUMN %s int(2)", name);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg) && g_mysqlCon.ExecuteSQL(strSQL1, strMsg1))
		{
			MessageBox("����Ա��ӳɹ���");
			ShowMsg2Output1(name + "����Ա��ӳɹ���");
		}
		else
		{
			MessageBox("����Ա��Ӵ���");
			ShowMsg2Output1("����Ա��Ӳ�������" + strSQL);
			ShowMsg2Output1("����Ա��Ӳ�������" + strSQL1);
			LOG("SQLERROR.log", strSQL);
			LOG("SQLERROR.log", strSQL1);
			LOG("SQLERROR.log", g_mysqlCon.GetError());
		}
	}

	OnOK();
}
