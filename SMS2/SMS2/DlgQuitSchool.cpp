// DlgQuitSchool.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgQuitSchool.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgQuitSchool �Ի���

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
}


BEGIN_MESSAGE_MAP(CDlgQuitSchool, CBCGPDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgQuitSchool ��Ϣ�������


HBRUSH CDlgQuitSchool::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
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


		int fee = atoi(datas[0][3]);

		int step = atoi(datas[0][6]);
		CString strClassType = datas[0][6];
		int classStep = atoi(datas[0][7]);
		if (step < 2)
		{
			m_strInfo[4] = "�ѱ�����δ��ʼ��Ŀһ����ѧϰ"; //step 

			//δ����ע��ģ� ȫ���˿��100������
			int reMoney = fee;
			m_strInfo[5].Format("%d", reMoney); //Ӧ�˽��
			m_strInfo[9] = "100";
		}
		else if (step < 2)
		{
			m_strInfo[4] = "��ע�ᣬδ��ʼ��ѵ"; //step
		}
		
	}
	ShowMsg2Output1(strMsg);
}