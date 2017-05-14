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
		CString strSQL("");
		strSQL.Format("INSERT INTO coachinfo \
			(SNAME, GENDER, BIRTH, PARTY_STAT, TEL, HOME, SIGN_DATE, FILE_NUM) \
			VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
			name, gender, birth, party, tel, home, date, filenum);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		{
			MessageBox("����Ա��ӳɹ���");
			ShowMsg2Output1(name + "����Ա��ӳɹ���");
		}
		else
		{
			MessageBox("����Ա��Ӵ���");
			ShowMsg2Output1("����Ա��Ӳ�������" + strSQL);
			LOG("SQLERROR.log", strSQL);
			LOG("SQLERROR.log", g_mysqlCon.GetError());
		}


		strSQL.Format("INSERT INTO coachstat (FILE_NUM) VALUES('%s')", filenum);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		CTime t = CTime::GetCurrentTime();
		CString month = t.Format("%Y/%m");
		strSQL.Format("INSERT INTO kpis (KMONTH, COACH_ID, COACH) VALUES('%s','%s','%s')",month, filenum, name);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	}

	OnOK();
}


BOOL CAddCoach::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_Comb_Gen.AddString("��");
	m_Comb_Gen.AddString("Ů");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
