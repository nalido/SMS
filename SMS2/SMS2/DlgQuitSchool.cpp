// DlgQuitSchool.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgQuitSchool.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgQuitSchool 对话框

IMPLEMENT_DYNAMIC(CDlgQuitSchool, CBCGPDialog)

CDlgQuitSchool::CDlgQuitSchool(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgQuitSchool::IDD, pParent)
{
	EnableVisualManagerStyle();

}

CDlgQuitSchool::~CDlgQuitSchool()
{
}

void CDlgQuitSchool::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);
	DDX_Control(pDX, IDC_S5, m_S[4]);
	DDX_Control(pDX, IDC_S6, m_S[5]);
	DDX_Control(pDX, IDC_S7, m_S[6]);
	DDX_Control(pDX, IDC_S8, m_S[7]);
	DDX_Control(pDX, IDC_S9, m_S[8]);
	DDX_Control(pDX, IDC_S10, m_S[9]);
	DDX_Control(pDX, IDC_S11, m_S[10]);

	DDX_Control(pDX, IDC_E1, m_E[0]);
	DDX_Control(pDX, IDC_E2, m_E[1]);
	DDX_Control(pDX, IDC_E3, m_E[2]);
	DDX_Control(pDX, IDC_E4, m_E[3]);
	DDX_Control(pDX, IDC_E5, m_E[4]);
	DDX_Control(pDX, IDC_E6, m_E[5]);
	DDX_Control(pDX, IDC_E7, m_E[6]);
	DDX_Control(pDX, IDC_E8, m_E[7]);
	DDX_Control(pDX, IDC_E9, m_E[8]);
	DDX_Control(pDX, IDC_E10, m_E[9]);
	DDX_Control(pDX, IDC_E11, m_E[10]);

	DDX_Text(pDX, IDC_E1, m_strInfo[0]);
	DDX_Text(pDX, IDC_E2, m_strInfo[1]);
	DDX_Text(pDX, IDC_E3, m_strInfo[2]);
	DDX_Text(pDX, IDC_E4, m_strInfo[3]);
	DDX_Text(pDX, IDC_E5, m_strInfo[4]);
	DDX_Text(pDX, IDC_E6, m_strInfo[5]);
	DDX_Text(pDX, IDC_E7, m_strInfo[6]);
	DDX_Text(pDX, IDC_E8, m_strInfo[7]);
	DDX_Text(pDX, IDC_E9, m_strInfo[8]);
	DDX_Text(pDX, IDC_E10, m_strInfo[9]);
	DDX_Text(pDX, IDC_E11, m_strInfo[10]);
}


BEGIN_MESSAGE_MAP(CDlgQuitSchool, CBCGPDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CDlgQuitSchool::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgQuitSchool 消息处理程序


HBRUSH CDlgQuitSchool::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CDlgQuitSchool::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	for (int i = 0; i < 6; i++)
	{
		m_E[i].EnableWindow(FALSE);
	}
	m_E[7].EnableWindow(FALSE);
	m_E[8].EnableWindow(FALSE);

	Refrsh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgQuitSchool::Refrsh()
{
	CTime today = GetServerTime();
	m_strInfo[7] = today.Format("%Y/%m/%d");

	CString strSQL, strMsg;
	CDStrs datas;
	strSQL.Format("SELECT students.SNAME, students.FILE_NAME, students.ID, \
					students.FEE, students.CAR_TYPE, students.STEP, \
					students.CLASS_TYPE, students.CLASS_NUM FROM students \
					WHERE FILE_NAME='%s'",
					m_strInfo[1]);
	if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg) && datas.size() > 0)
	{
		m_strInfo[0] = datas[0][0]; //name
		m_strInfo[2] = datas[0][2]; //IDcard
		m_strInfo[3] = datas[0][3]; //fee
		m_strInfo[8] = datas[0][4]; //car_type
		m_strStuID = datas[0][1]; //ID

		int fee = atoi(datas[0][3]);

		int step = atoi(datas[0][5]);
		CString strClassType = datas[0][6];
		int classStep = atoi(datas[0][7]);
		m_nQuitFlag = 1002; //默认中途退学;
		if (step < 2)
		{
			m_strInfo[4] = "已报名，未开始科目一理论学习"; //step 

			//未办理注册的， 全部退款，收100手续费
			int reMoney = fee;
			m_strInfo[5].Format("%d", reMoney); //应退金额
			m_strInfo[9] = "100";
		}
		else if (step < 3)
		{
			m_strInfo[4] = "已注册，未开始培训"; //step

			//只退还培训费
			int reMoney = fee - 270;
			m_strInfo[5].Format("%d", reMoney); //应退金额
			m_strInfo[9] = "200";
		}
		else if (step < 7) //科目一
		{
			m_strInfo[4] = "正进行科目一理论培训"; //step

			//退还培训费75%
			int reMoney = fee * 3 / 4;
			m_strInfo[5].Format("%d", reMoney); //应退金额
			m_strInfo[9] = "0";
		}
		else if (step == 1001 || step == 1002)
		{
			m_nQuitFlag = 1001;

			m_strInfo[4] = "政治审核不通过"; //step 

			//未办理注册的， 全部退款，收100手续费
			int reMoney = fee;
			m_strInfo[5].Format("%d", reMoney); //应退金额
			m_strInfo[9] = "100";
		}
		else //科目二、三
		{
			m_strInfo[9] = "0";
			if (strClassType == "科目二")
			{
				if (classStep < g_nMinK2Class)
				{
					m_strInfo[4] = "正进行科目二基础驾驶培训"; //step

					//只退还培训费60%
					int reMoney = fee * 3 / 5;
					m_strInfo[5].Format("%d", reMoney); //应退金额
				}
				else
				{
					m_strInfo[4] = "正进行科目二场内式样驾驶培训"; //step
					//只退还培训费50%
					int reMoney = fee / 2;
					m_strInfo[5].Format("%d", reMoney); //应退金额
				}
			}
			else
			{
				if (classStep < g_nMinK3Class)
				{
					m_strInfo[4] = "正进行科目三道路驾驶培训"; //step

					//只退还培训费20%
					int reMoney = fee / 5;
					m_strInfo[5].Format("%d", reMoney); //应退金额
				}
				else
				{
					m_strInfo[4] = "正进行科目三考前综合驾驶培训"; //step

					//只退还培训费0%
					int reMoney = 0;
					m_strInfo[5].Format("%d", reMoney); //应退金额
				}
			}

		}
	}
	ShowMsg2Output1(strMsg);
	UpdateData(FALSE);
}

void CDlgQuitSchool::OnBnClickedOk()
{
	UpdateData();
	if (m_strInfo[6].IsEmpty() || m_strInfo[9].IsEmpty())
	{
		MessageBox("必须填金额");
		return;
	}

	int rmoney = atoi(m_strInfo[6]);
	int fee = atoi(m_strInfo[9]);
	m_strInfo[6].Format("%d", rmoney);
	m_strInfo[9].Format("%d", fee);

	CString strSQL, strMsg;
	strSQL.Format("UPDATE students SET STEP='%d' WHERE FILE_NAME='%s'", m_nQuitFlag, m_strStuID);
	if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
	{
		strSQL.Format("INSERT INTO stuquits (QUIT_DATE, STU_ID, QUIT_REASON, SHOULD_MONEY, RETURN_MONEY, FEE) \
					  				  VALUES('%s', '%s', '%s', '%s', '%s', '%s')",
									  m_strInfo[7], m_strInfo[1], m_strInfo[10], m_strInfo[5], m_strInfo[6], m_strInfo[9]);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		{
			MessageBox("退款成功");
			OnOK();
		}
		else {
			MessageBox("退款失败，请稍后再试");
		}
	}
	ShowMsg2Output1(strMsg);
}
