// AddCoach.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "AddCoach.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CAddCoach 对话框

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


// CAddCoach 消息处理程序


void CAddCoach::OnBnClickedAdd()
{
	UpdateData(TRUE); //获取输入


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
		MessageBox("必须每一项都填完再点击添加！");
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
			MessageBox("教练员添加成功！");
			ShowMsg2Output1(name + "教练员添加成功！");
		}
		else
		{
			MessageBox("教练员添加错误！");
			ShowMsg2Output1("教练员添加操作错误：" + strSQL);
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

	m_Comb_Gen.AddString("男");
	m_Comb_Gen.AddString("女");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
