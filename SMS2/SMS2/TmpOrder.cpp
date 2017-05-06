// TmpOrder.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "TmpOrder.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CTmpOrder �Ի���

IMPLEMENT_DYNAMIC(CTmpOrder, CBCGPDialog)

CTmpOrder::CTmpOrder(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CTmpOrder::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CTmpOrder::~CTmpOrder()
{
}

void CTmpOrder::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_Comb_Type);
	DDX_Control(pDX, IDC_COMBO_CAR, m_Comb_Car);
	DDX_Control(pDX, IDC_COMBO_COACH, m_Comb_Coach);
	DDX_Control(pDX, IDC_COMBO_STU, m_Comb_Stu);
	DDX_Control(pDX, IDC_COMBO_TIME, m_Comb_Time);
	DDX_Control(pDX, IDC_DATE, m_Date);
	DDX_Control(pDX, IDC_COACH, m_SCoach);
	DDX_Control(pDX, IDC_STU, m_SStu);
	DDX_Control(pDX, IDC_NCLASS, m_SNClass);

	DDX_Text(pDX, IDC_COMBO_TYPE, m_strType);
	DDX_Text(pDX, IDC_COMBO_CAR, m_strCar);
	DDX_Text(pDX, IDC_COMBO_COACH, m_strCoach);
	DDX_Text(pDX, IDC_COMBO_STU, m_strStu);
	DDX_Text(pDX, IDC_COMBO_TIME, m_strTime);
	DDX_Text(pDX, IDC_DATE, m_strDate);
	DDX_Text(pDX, IDC_COACH, m_strCoachID);
	DDX_Text(pDX, IDC_STU, m_strStuID);
	DDX_Text(pDX, IDC_NCLASS, m_strNClass);
}


BEGIN_MESSAGE_MAP(CTmpOrder, CBCGPDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_COACH, &CTmpOrder::OnCbnSelchangeComboCoach)
	ON_CBN_EDITCHANGE(IDC_COMBO_COACH, &CTmpOrder::OnCbnEditchangeComboCoach)
	ON_CBN_EDITCHANGE(IDC_COMBO_STU, &CTmpOrder::OnCbnEditchangeComboStu)
	ON_CBN_SELCHANGE(IDC_COMBO_STU, &CTmpOrder::OnCbnSelchangeComboStu)
	ON_BN_CLICKED(IDC_PRINT, &CTmpOrder::OnBnClickedPrint)
END_MESSAGE_MAP()


// CTmpOrder ��Ϣ�������


BOOL CTmpOrder::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_Comb_Type.AddString("��Ŀһ");
	m_Comb_Type.AddString("��Ŀ��");
	m_Comb_Type.AddString("��Ŀ��");

	m_Comb_Time.AddString("A8:00-10:00");
	m_Comb_Time.AddString("A10:00-12:00");
	m_Comb_Time.AddString("P2:00-4:00");
	m_Comb_Time.AddString("P4:00-6:00");

	for (int i = 0; i < m_datasCar.size(); i++)
		m_Comb_Car.AddString(m_datasCar[i][0]);

	for (int i = 0; i < m_datasCoach.size(); i++)
		m_Comb_Coach.AddString(m_datasCoach[i][0]);

	m_Date.SetFormat("yyyy/MM/dd");

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CTmpOrder::Refresh()
{
	CString strMsg, strSQL;

	//ȫ������ѧԱ��Ϣ
	strSQL.Format("SELECT students.SNAME, students.CAR_TYPE, students.CLASS_NUM, students.FILE_NAME\
				  	FROM students WHERE STEP>='7' AND STEP<'1000' ORDER BY SNAME");
	m_datasStu.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datasStu, strMsg))
	{
		ShowMsg2Output1("��ѯѧ����Ϣ�ɹ�");
	}
	else ShowMsg2Output1(strMsg);

	for (int i = 0; i < m_datasStu.size(); i++)
	{
		m_Comb_Stu.AddString(m_datasStu[i][0]);
	}
}

void CTmpOrder::OnCbnSelchangeComboCoach()
{
	UpdateData(TRUE);

	int pos = m_Comb_Coach.GetCurSel();
	m_strCoachID = m_datasCoach[pos][2];

	CRect rect;
	m_SCoach.GetClientRect(&rect);
	m_SCoach.MapWindowPoints(this, &rect);
	InvalidateRect(rect);

	UpdateData(FALSE);
}


void CTmpOrder::OnCbnEditchangeComboCoach()
{
	UpdateData(TRUE);
	BOOL isFound = FALSE;
	for (int i = 0; i < m_datasCoach.size(); i++)
	{
		if (m_datasCoach[i][0] == m_strCoach)
		{
			m_strCoachID = m_datasCoach[i][2];

			isFound = TRUE;
			break;
		}
	}

	if (!isFound) m_strCoachID = "���޴���";

	CRect rect;
	m_SCoach.GetClientRect(&rect);
	m_SCoach.MapWindowPoints(this, &rect);
	InvalidateRect(rect);


	UpdateData(FALSE);
}


void CTmpOrder::OnCbnEditchangeComboStu()
{
	UpdateData(TRUE);
	BOOL isFound = FALSE;
	for (int i = 0; i < m_datasStu.size(); i++)
	{
		if (m_datasStu[i][0] == m_strStu)
		{
			m_strStuID = m_datasStu[i][3];
			m_strNClass = m_datasStu[i][2];

			isFound = TRUE;
			break;
		}
	}

	if (!isFound)
	{
		m_strStuID = "���޴���";
		m_strNClass = "";
	}

	CRect rect;
	m_SStu.GetClientRect(&rect);
	m_SStu.MapWindowPoints(this, &rect);
	InvalidateRect(rect);

	m_SNClass.GetClientRect(&rect);
	m_SNClass.MapWindowPoints(this, &rect);
	InvalidateRect(rect);
	UpdateData(FALSE);
}


void CTmpOrder::OnCbnSelchangeComboStu()
{
	UpdateData(TRUE);

	int pos = m_Comb_Stu.GetCurSel();
	m_strStuID = m_datasStu[pos][3];
	m_strNClass = m_datasStu[pos][2];

	CRect rect;
	m_SStu.GetClientRect(&rect);
	m_SStu.MapWindowPoints(this, &rect);
	InvalidateRect(rect);

	m_SNClass.GetClientRect(&rect);
	m_SNClass.MapWindowPoints(this, &rect);
	InvalidateRect(rect);

	UpdateData(FALSE);
}


void CTmpOrder::OnBnClickedPrint()
{
	UpdateData(TRUE);
	if (m_strCar.IsEmpty() || m_strCoach.IsEmpty() || m_strDate.IsEmpty() ||
		m_strNClass.IsEmpty() || m_strStu.IsEmpty() || m_strStuID=="���޴���" ||
		m_strTime.IsEmpty() || m_strType.IsEmpty() || m_strCoachID=="���޴���")
	{
		MessageBox("�ɹ�����Ϣδ���ƣ����ܴ�ӡ");
		return;
	}


	xPublic::CMyPrint printx;
	xPublic::CLASSINFO classInfo;
	xPublic::SHEETINFO sheetInfo;

	//�ɹ�����Ϣ
	int classStep = atoi(m_strNClass);
	CString cn;
	cn.Format("c%d", classStep + 1);
	classInfo.nClassID = xPublic::GETINT2(m_strType, cn, 0);

	sheetInfo.strCarID = m_strCar;
	sheetInfo.strClassType = m_strType;
	sheetInfo.strCoach = m_strCoach;
	sheetInfo.strCoachID = m_strCoachID;
	CTime t = CTime::GetCurrentTime();
	sheetInfo.strDate = t.Format("%Y��%m��%d����");

	//ѧԱ��Ϣ
	int classID = m_Comb_Time.GetCurSel();
	xPublic::STUDENTINFO student(m_strStu, m_strDate.Right(8), classID, classStep, g_nMaxBooking);
	printx.AddStudent(student);

	//�ϴ����ݿ�
	CString strMsg, strSQL;
	CString strClassID;
	strClassID.Format("%d", classID + 1);
	//ɾ���ظ�����
	strSQL.Format("Delete From bookings \
					WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
					m_strStuID, m_strDate, strClassID);
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);

	//����¼�¼
	strSQL.Format("INSERT INTO bookings (FLAG, ORDER_DATE, ORDER_COACH, ORDER_CAR, CLASS_NUM, CLASS_TYPE,\
				  	FILE_NAME, BOOK_DATE, CLASS_ID) VALUES(1, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
					t.Format("%Y/%m/%d"), m_strCoachID, m_strCar, m_strNClass, m_strType
					, m_strStuID, m_strDate, strClassID);
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);

	//��ӡ
	printx.PrinterInit(&sheetInfo, &classInfo);
	printx.Printx(1, strMsg);
}
