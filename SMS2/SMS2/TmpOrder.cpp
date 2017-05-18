// TmpOrder.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "TmpOrder.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MSGINFO.h"  //临时派工不发短信


// CTmpOrder 对话框

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
	DDX_Control(pDX, IDC_COMBO_CLASS, m_Comb_Class);
	DDX_Control(pDX, IDC_DATE, m_Date);
	DDX_Control(pDX, IDC_COACH, m_SCoach);
	DDX_Control(pDX, IDC_STU, m_SStu);
	DDX_Control(pDX, IDC_NCLASS, m_SNClass);

	DDX_Text(pDX, IDC_COMBO_TYPE, m_strType);
	DDX_Text(pDX, IDC_COMBO_CAR, m_strCar);
	DDX_Text(pDX, IDC_COMBO_COACH, m_strCoach);
	DDX_Text(pDX, IDC_COMBO_STU, m_strStu);
	DDX_Text(pDX, IDC_COMBO_TIME, m_strTime);
	DDX_Text(pDX, IDC_COMBO_CLASS, m_strNextClass);
	DDX_Text(pDX, IDC_DATE, m_strBookDate);
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
	ON_BN_CLICKED(IDOK, &CTmpOrder::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CTmpOrder::OnCbnSelchangeComboType)
END_MESSAGE_MAP()


// CTmpOrder 消息处理程序


BOOL CTmpOrder::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_isDataReady = FALSE;

	m_Comb_Type.AddString("科目一");
	m_Comb_Type.AddString("科目二");
	m_Comb_Type.AddString("科目三");

	m_Comb_Time.AddString("A8:00-10:00");
	m_Comb_Time.AddString("A10:00-12:00");
	m_Comb_Time.AddString("P2:00-4:00");
	m_Comb_Time.AddString("P4:00-6:00");

	for (int i = 0; i < m_datasCar.size(); i++)
		m_Comb_Car.AddString(m_datasCar[i][0]);

	for (int i = 0; i < m_datasCoach.size(); i++)
		m_Comb_Coach.AddString(m_datasCoach[i][0]);

	for (int i = 0; i < g_nMaxBooking; i++)
	{
		CString str;
		str.Format("%d", i + 1);
		m_Comb_Class.AddString(str);
	}

	m_Date.SetFormat("yyyy/MM/dd");


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CTmpOrder::Refresh()
{
	CString strMsg, strSQL;

	if (m_strType.IsEmpty()) return;
	//全部可用学员信息
	strSQL.Format("SELECT students.SNAME, students.CAR_TYPE, students.CLASS_NUM, students.FILE_NAME, students.TEL\
				  	FROM students WHERE STEP>='7' AND STEP<'1000' AND CLASS_TYPE='%s' ORDER BY SNAME",
					m_strType);
	m_datasStu.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datasStu, strMsg))
	{
		ShowMsg2Output1("查询学生信息成功");
	}
	else ShowMsg2Output1(strMsg);

	m_Comb_Stu.ResetContent();
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

	if (!isFound) m_strCoachID = "查无此人";

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
		m_strStuID = "查无此人";
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
	m_strTEL = m_datasStu[pos][4];

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
	m_isDataReady = FALSE;

	UpdateData(TRUE);
	if (m_strCar.IsEmpty() || m_strCoach.IsEmpty() || m_strBookDate.IsEmpty() ||
		m_strNextClass.IsEmpty() || m_strStu.IsEmpty() || m_strStuID=="查无此人" ||
		m_strTime.IsEmpty() || m_strType.IsEmpty() || m_strCoachID=="查无此人")
	{
		MessageBox("派工单信息未完善，不能打印");
		return;
	}


	xPublic::CMyPrint printx;
	xPublic::CLASSINFO classInfo;
	xPublic::SHEETINFO sheetInfo;

	//派工单信息
	int classStep = atoi(m_strNextClass);
	CString cn;
	cn.Format("c%d", classStep + 1);
	classInfo.nClassID = xPublic::GETINT2(m_strType, cn, 0);

	sheetInfo.strCarID = m_strCar;
	sheetInfo.strClassType = m_strType;
	sheetInfo.strCoach = m_strCoach;
	sheetInfo.strCoachID = m_strCoachID;
	CTime t = CTime::GetCurrentTime();
	sheetInfo.strDate = t.Format("%Y年%m月%d日制");

	//学员信息
	int classID = m_Comb_Time.GetCurSel();
	xPublic::STUDENTINFO student(m_strStu, m_strBookDate.Right(8), classID, classStep, g_nMaxBooking);
	student.strTEL = m_strTEL;
	printx.AddStudent(student);

	//上传数据库
	CString strClassID;
	strClassID.Format("%d", classID + 1);
	m_strClassID = strClassID;
	m_strOrderDate = t.Format("%Y/%m/%d");
	CString strMsg, strSQL;
	//删除重复数据
	strSQL.Format("Delete From bookings \
					WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
					m_strStuID, m_strBookDate, strClassID);
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);

	//添加新记录
	strSQL.Format("INSERT INTO bookings (FLAG, ORDER_DATE, ORDER_COACH, ORDER_CAR, CLASS_NUM, CLASS_TYPE,\
				  	FILE_NAME, BOOK_DATE, CLASS_ID) VALUES(1, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
					t.Format("%Y/%m/%d"), m_strCoachID, m_strCar, m_strNClass, m_strType
					, m_strStuID, m_strBookDate, strClassID);
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);

	//打印
	printx.PrinterInit(&sheetInfo, &classInfo);
	printx.Printx(1, strMsg);

	m_isDataReady = TRUE;

	OnOK();
}


void CTmpOrder::OnBnClickedOk()
{
	if (m_isDataReady == FALSE)
	{
		UpdateData(TRUE);
		if (m_strCar.IsEmpty() || m_strCoach.IsEmpty() || m_strBookDate.IsEmpty() ||
			m_strNextClass.IsEmpty() || m_strStu.IsEmpty() || m_strStuID == "查无此人" ||
			m_strTime.IsEmpty() || m_strType.IsEmpty() || m_strCoachID == "查无此人")
		{
			MessageBox("派工单信息未完善!");
			return;
		}

		int classID = m_Comb_Time.GetCurSel();
		CString strClassID;
		strClassID.Format("%d", classID + 1);
		m_strClassID = strClassID;
		CTime t = CTime::GetCurrentTime();
		m_strOrderDate = t.Format("%Y/%m/%d");
	}


	//上传数据库
	CString strMsg, strSQL;
	//删除重复数据
	strSQL.Format("Delete From bookings \
				  	WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
					m_strStuID, m_strBookDate, m_strClassID);
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);

	//添加新记录
	strSQL.Format("INSERT INTO bookings (FLAG, ORDER_DATE, ORDER_COACH, ORDER_CAR, CLASS_NUM, CLASS_TYPE,\
				  	FILE_NAME, BOOK_DATE, CLASS_ID) VALUES(1, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
					m_strOrderDate, m_strCoachID, m_strCar, m_strNClass, m_strType
					, m_strStuID, m_strBookDate, m_strClassID);
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);

	OnOK();
}


void CTmpOrder::OnCbnSelchangeComboType()
{
	UpdateData(TRUE);

	int pos = m_Comb_Type.GetCurSel(); 
	switch (pos)
	{
	case 0:
		m_strType = "科目一";
		break;
	case 1:
		m_strType = "科目二";
		break;
	case 2:
		m_strType = "科目三";
		break;
	}

	Refresh();
}
