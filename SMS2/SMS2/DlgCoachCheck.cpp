// DlgCoachCheck.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgCoachCheck.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgCoachCheck 对话框

IMPLEMENT_DYNAMIC(CDlgCoachCheck, CBCGPDialog)

CDlgCoachCheck::CDlgCoachCheck(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgCoachCheck::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgCoachCheck::~CDlgCoachCheck()
{
}

void CDlgCoachCheck::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation);

	DDX_Control(pDX, IDC_COMBO1, m_Comb_YearS);
	DDX_Control(pDX, IDC_COMBO2, m_Comb_MonthS);
	DDX_Control(pDX, IDC_COMBO3, m_Comb_DayS);
	DDX_Control(pDX, IDC_COMBO4, m_Comb_YearE);
	DDX_Control(pDX, IDC_COMBO5, m_Comb_MonthE);
	DDX_Control(pDX, IDC_COMBO6, m_Comb_DayE);
	DDX_Text(pDX, IDC_COMBO1, m_strYearS);
	DDX_Text(pDX, IDC_COMBO2, m_strMonthS);
	DDX_Text(pDX, IDC_COMBO3, m_strDayS);
	DDX_Text(pDX, IDC_COMBO4, m_strYearE);
	DDX_Text(pDX, IDC_COMBO5, m_strMonthE);
	DDX_Text(pDX, IDC_COMBO6, m_strDayE);

	DDX_Radio(pDX, IDC_RADIO1, m_nQueryType);

	DDX_Text(pDX, IDC_MSG1, m_strMsg1);
	DDX_Text(pDX, IDC_MSG2, m_strMsg2);
	DDX_Text(pDX, IDC_MSG3, m_strMsg3);


	DDX_Control(pDX, IDC_MSG1, m_Static_Msg1);
	DDX_Control(pDX, IDC_MSG2, m_Static_Msg2);
	DDX_Control(pDX, IDC_MSG3, m_Static_Msg3);
}


BEGIN_MESSAGE_MAP(CDlgCoachCheck, CBCGPDialog)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgCoachCheck::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlgCoachCheck::OnCbnSelchangeCombo5)
//	ON_CBN_CLOSEUP(IDC_COMBO2, &CDlgCoachCheck::OnCbnCloseupCombo2)
ON_BN_CLICKED(IDC_RADIO1, &CDlgCoachCheck::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, &CDlgCoachCheck::OnBnClickedRadio2)
ON_BN_CLICKED(IDC_RADIO4, &CDlgCoachCheck::OnBnClickedRadio4)
ON_BN_CLICKED(IDC_QUERY, &CDlgCoachCheck::OnBnClickedQuery)
ON_BN_CLICKED(IDC_SETTODAY, &CDlgCoachCheck::OnBnClickedSettoday)
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgCoachCheck 消息处理程序

static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgCoachCheck* pThis = (CDlgCoachCheck*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
		}
	}

	return TRUE;
}

CString arrType[] = { "出勤统计", "缺勤统计", "工时统计", "绩效统计", "KPI" };
BOOL CDlgCoachCheck::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();


	CRect rect;
	m_wndGridLocation.GetClientRect(&rect);
	m_wndGridLocation.MapWindowPoints(this, &rect);

	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid.Create(nStyle, rect, this, IDC_GRID_STUPRO);
	m_wndGrid.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	m_wndGrid.SetSingleSel(); //只能选一个
	m_wndGrid.EnableRowHeader(TRUE);
	m_wndGrid.EnableLineNumbers();

	int nColumn = 0;
	int hw = m_wndGrid.GetRowHeaderWidth();
	std::vector<CString> arrColumns;
	switch (m_nCheckType)
	{
	case CHECK_CHUQIN:
		arrColumns.push_back("出勤日期");
		arrColumns.push_back("出勤时间");
		arrColumns.push_back("学员个数");
		break;
	case CHECK_QUEQIN:
		arrColumns.push_back("缺勤日期");
		arrColumns.push_back("缺勤时间");
		arrColumns.push_back("学员个数");
		break;
	case CHECK_WORKTIME:
		arrColumns.push_back("工作日期");
		arrColumns.push_back("工作时间");
		arrColumns.push_back("工作时长");
		break;
	case CHECK_JIXIAO:
		arrColumns.push_back("记录日期");
		arrColumns.push_back("记录时间");
		arrColumns.push_back("学员个数");
		break;
	case CHECK_KPI:
		arrColumns.push_back("记录日期");
		arrColumns.push_back("取样数");
		arrColumns.push_back("满意度");
		arrColumns.push_back("满意率");
		arrColumns.push_back("合格率");
		arrColumns.push_back("设备完好率");
		arrColumns.push_back("介绍学员数");
		arrColumns.push_back("介绍目标数");
		arrColumns.push_back("总分");
		break;
	}

	int nColumns = arrColumns.size();
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGrid.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);

	CTime t = CTime::GetCurrentTime();
	//默认统计当月
	m_strYearS = t.Format("%Y");
	m_strMonthS = t.Format("%m");
	//int month = atoi(m_strMonthS) - 1;
	m_strDayS = "01";
	m_strYearE = t.Format("%Y");
	m_strMonthE = t.Format("%m");
	m_strDayE = t.Format("%d");
	int month = atoi(m_strMonthE) + 1;
	m_strMonthE.Format("%02d", month);
	//UpdateData(FALSE);

	int year = atoi(m_strYearS) + 1; //最多查询近三年的数据
	for (int i = 3; i >= 0; i--)
	{
		CString strYear; 
		strYear.Format("%d", year - i);
		m_Comb_YearS.AddString(strYear);
		m_Comb_YearE.AddString(strYear);
	}

	for (int i = 1; i <= 12; i++)
	{
		CString strMonth;
		strMonth.Format("%02d", i);
		m_Comb_MonthS.AddString(strMonth);
		m_Comb_MonthE.AddString(strMonth);
	}


	m_nQueryType = 0; 


	m_Static_Msg1.GetClientRect(&m_rectMsg1);
	m_Static_Msg1.MapWindowPoints(this, &m_rectMsg1);
	m_Static_Msg2.GetClientRect(&m_rectMsg2);
	m_Static_Msg2.MapWindowPoints(this, &m_rectMsg2);
	m_Static_Msg3.GetClientRect(&m_rectMsg3);
	m_Static_Msg3.MapWindowPoints(this, &m_rectMsg3);

	m_strMsg1.Format("教练员%s:", m_strCoach);
	m_strMsg20.Format("在该时间段内的%s为", arrType[m_nCheckType]);
	m_strMsg3 = "";
	UpdateData(FALSE);

	if (m_nCheckType == CHECK_JIXIAO) //绩效只能按月或者年查询
	{
		//GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		m_Comb_DayS.EnableWindow(FALSE);
		m_Comb_DayE.EnableWindow(FALSE);
		m_nQueryType = 1;
		m_strMsg20.Format("本月绩效为");
		UpdateData(FALSE);
		GetJiXiao();
		UpdateCStatic();
	}
	else if (m_nCheckType == CHECK_KPI) //KPI只能按月或者年查询
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		m_Comb_DayS.EnableWindow(FALSE);
		m_Comb_DayE.EnableWindow(FALSE);
		m_nQueryType = 1;
		m_strMsg20.Format("本月KPI为");
		UpdateData(FALSE);
		GetKPI();
		UpdateCStatic();
	}


	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

int GetDateNum(int year, int month)
{
	int days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		days[2] = 29;
	return days[month];
}

void CDlgCoachCheck::OnCbnSelchangeCombo2()
{
	UpdateData();
	int pos = 0;
	pos = m_Comb_YearS.GetCurSel();
	if (pos < 0)
	{
		m_Comb_YearS.SetCurSel(2);
		pos = 2;
	}
	m_Comb_YearS.GetLBText(pos, m_strYearS);
	pos = m_Comb_MonthS.GetCurSel();
	m_Comb_MonthS.GetLBText(pos, m_strMonthS);
	int year = atoi(m_strYearS);
	int month = atoi(m_strMonthS);
	int days = GetDateNum(year, month);
	m_Comb_DayS.ResetContent();
	for (int i = 1; i <= days; i++)
	{
		CString strDay;
		strDay.Format("%02d", i);
		m_Comb_DayS.AddString(strDay);
	}
	m_Comb_DayS.GetCurSel();
}


void CDlgCoachCheck::OnCbnSelchangeCombo5()
{
	UpdateData();
	int pos = 0;
	pos = m_Comb_YearE.GetCurSel();
	if (pos < 0) 
	{
		m_Comb_YearE.SetCurSel(2);
		pos = 2;
	}
	m_Comb_YearE.GetLBText(pos, m_strYearE);
	pos = m_Comb_MonthE.GetCurSel();
	m_Comb_MonthE.GetLBText(pos, m_strMonthE);
	int year = atoi(m_strYearE);
	int month = atoi(m_strMonthE);
	int days = GetDateNum(year, month);
	m_Comb_DayE.ResetContent();
	for (int i = 1; i <= days; i++)
	{
		CString strDay;
		strDay.Format("%02d", i);
		m_Comb_DayE.AddString(strDay);
	}
}




void CDlgCoachCheck::OnBnClickedRadio1()
{
	UpdateData();
	m_Comb_YearS.EnableWindow(TRUE);
	m_Comb_MonthS.EnableWindow(TRUE);
	m_Comb_DayS.EnableWindow(TRUE);
	m_Comb_YearE.EnableWindow(TRUE);
	m_Comb_MonthE.EnableWindow(TRUE);
	m_Comb_DayE.EnableWindow(TRUE);
}


void CDlgCoachCheck::OnBnClickedRadio2()
{
	UpdateData();
	m_Comb_YearS.EnableWindow(TRUE);
	m_Comb_MonthS.EnableWindow(TRUE);
	m_Comb_DayS.EnableWindow(FALSE);
	m_Comb_YearE.EnableWindow(TRUE);
	m_Comb_MonthE.EnableWindow(TRUE);
	m_Comb_DayE.EnableWindow(FALSE);
}


void CDlgCoachCheck::OnBnClickedRadio4()
{
	UpdateData();
	m_Comb_YearS.EnableWindow(TRUE);
	m_Comb_MonthS.EnableWindow(FALSE);
	m_Comb_DayS.EnableWindow(FALSE);
	m_Comb_YearE.EnableWindow(TRUE);
	m_Comb_MonthE.EnableWindow(FALSE);
	m_Comb_DayE.EnableWindow(FALSE);
}


void CDlgCoachCheck::GetKPI()
{
	CString strMsg, strSQL;
	CString strDayStart = GetSelDate(0);
	CString strDayEnd = GetSelDate(1);
	CDStrs datas;

	//学员评分记录
	strSQL.Format("SELECT BOOK_DATE, SERVICE_SCORE, SELF_SCORE FROM bookings WHERE ORDER_COACH='%s' \
				  	AND BOOK_DATE>='%s' AND BOOK_DATE<='%s' AND FLAG='2' ORDER BY BOOK_DATE",
					m_strCoachID, strDayStart, strDayEnd);
	g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);
	ShowMsg2Output1(strMsg);

	int nStudents = datas.size();
	if (nStudents == 0)
	{
		m_strMsg2.Format("%s0", m_strMsg20);
		m_strMsg3 = "";
		return;
	}

	int service = 0; //服务评价 满意度
	int self = 0; //技能评价 合格率
	int good = 0; //满意率
	for (int i = 0; i < nStudents; i++)
	{
		int d1 = atoi(datas[i][1]);
		int d2 = atoi(datas[i][2]);

		service += d1;
		self += d2;
		if (d1 >= 80) good++;
	}
	service = service * 100 / nStudents;
	self = self * 100 / nStudents;
	good = good * 10000 / nStudents;

	double KPI = (service + self)*1.0 / 200;

	m_strMsg2.Format("%s%.2f", m_strMsg20, KPI);
	m_strMsg3 = "";

	//更新数据库
	int kpi = KPI * 100;
	strSQL.Format("UPDATE KPIS SET SAMPLE_NUM='%d', SATISFIED_DEGREE='%d', SATISFIED_PERCENT='%d',\
				   SKILL_PERCENT='%d', SCORE='%d'\
				   WHERE COACH_ID='%s' AND KMONTH='%s'",
				   nStudents, service, good, self, kpi, m_strCoachID, strDayStart.Left(7));
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);
}

void CDlgCoachCheck::GetJiXiao()
{
	CString strMsg, strSQL;
	CString strDayStart = GetSelDate(0);
	CString strDayEnd = GetSelDate(1);
	CDStrs datas;

	//原始绩效
	strSQL.Format("SELECT CHECK_DATE, CLASS_ID, STUDENT FROM coachCheck WHERE COACH_ID='%s' \
				  	AND CHECK_DATE>='%s' AND CHECK_DATE<='%s' AND STUDENT='1' ORDER BY CHECK_DATE, CLASS_ID",
					m_strCoachID, strDayStart, strDayEnd);
	g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);
	ShowMsg2Output1(strMsg);

	int nStudents = datas.size();

	//当月缺勤数
	datas.clear();
	strSQL.Format("SELECT CHECK_DATE, CLASS_ID, STUDENT FROM coachCheck WHERE COACH_ID='%s' \
				  	AND CHECK_DATE>='%s' AND CHECK_DATE<='%s' AND TYPE='0' ORDER BY CHECK_DATE, CLASS_ID",
					m_strCoachID, strDayStart, strDayEnd);
	g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);
	ShowMsg2Output1(strMsg);

	int nLeave = datas.size();

	m_nStudents = nStudents;
	m_nLeave = nLeave;

	nStudents = nStudents - nLeave*g_nSubForLeave;
	m_strMsg2.Format("%s%d人", m_strMsg20, nStudents);
	m_strMsg3.Format("本月缺勤%d次，减去绩效%d人", nLeave, nLeave*g_nSubForLeave);
}

void CDlgCoachCheck::Refresh()
{
	CString strMsg, strSQL;
	CString strDayStart = GetSelDate(0);
	CString strDayEnd = GetSelDate(1);
	CDStrs datas;
	switch (m_nCheckType)
	{
	case CHECK_CHUQIN:
		strSQL.Format("SELECT CHECK_DATE, CLASS_ID, STUDENT FROM coachCheck WHERE COACH_ID='%s' \
					  	AND CHECK_DATE>='%s' AND CHECK_DATE<='%s' AND TYPE='1' ORDER BY CHECK_DATE, CLASS_ID", 
						m_strCoachID, strDayStart, strDayEnd);
		g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);
		ShowMsg2Output1(strMsg);

		RemadeData(datas);
		break;
	case CHECK_QUEQIN:
		strSQL.Format("SELECT CHECK_DATE, CLASS_ID, STUDENT FROM coachCheck WHERE COACH_ID='%s' \
					  	AND CHECK_DATE>='%s' AND CHECK_DATE<='%s' AND TYPE='0' ORDER BY CHECK_DATE, CLASS_ID",
						m_strCoachID, strDayStart, strDayEnd);
		g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);
		ShowMsg2Output1(strMsg);

		RemadeData(datas);
		break;
	case CHECK_WORKTIME:
		strSQL.Format("SELECT CHECK_DATE, CLASS_ID, WORK_TIME FROM coachCheck WHERE COACH_ID='%s' \
					  	AND CHECK_DATE>='%s' AND CHECK_DATE<='%s' AND TYPE='1' AND STUDENT='1' ORDER BY CHECK_DATE, CLASS_ID",
						m_strCoachID, strDayStart, strDayEnd);
		g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);
		ShowMsg2Output1(strMsg);

		RemadeData(datas);
		break;
	case CHECK_JIXIAO:
		strSQL.Format("SELECT CHECK_DATE, CLASS_ID, STUDENT FROM coachCheck WHERE COACH_ID='%s' \
					  	AND CHECK_DATE>='%s' AND CHECK_DATE<='%s' AND STUDENT='1' ORDER BY CHECK_DATE, CLASS_ID",
						m_strCoachID, strDayStart, strDayEnd);
		g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);
		ShowMsg2Output1(strMsg);

		RemadeData(datas);
		break;

	case CHECK_KPI:
		strSQL.Format("SELECT KMONTH, SAMPLE_NUM, SATISFIED_DEGREE, SATISFIED_PERCENT,\
					  SKILL_PERCENT, DEVICE_GOOD, STUDENT_NEWS, STUDENT_TARGET, SCORE\
					  FROM KPIS WHERE COACH_ID='%s' \
					  AND KMONTH>='%s' AND KMONTH<='%s' ORDER BY KMONTH",
					  m_strCoachID, strDayStart.Left(7), strDayEnd.Left(7));
		g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);
		ShowMsg2Output1(strMsg);

		m_datas.clear();
		m_datas = datas;
		int si = m_datas.size();
		for (int i = 0; i < si; i++)
		{
			double d = atoi(m_datas[i][2])*1.0 / 100;
			m_datas[i][2].Format("%.2f", d);
			d = atoi(m_datas[i][3])*1.0 / 100;
			m_datas[i][3].Format("%.2f", d);
			d = atoi(m_datas[i][4])*1.0 / 100;
			m_datas[i][4].Format("%.2f", d);
			d = atoi(m_datas[i][5])*1.0 / 100;
			m_datas[i][5].Format("%.2f", d);
			d = atoi(m_datas[i][8])*1.0 / 100;
			m_datas[i][8].Format("%.2f", d);
		}
		break;
	}


	m_wndGrid.GridRefresh(m_datas.size());
}

void CDlgCoachCheck::UpdateCStatic()
{
	InvalidateRect(&m_rectMsg1);
	InvalidateRect(&m_rectMsg2);
	InvalidateRect(&m_rectMsg3);
	UpdateData(FALSE);
}

void CDlgCoachCheck::RemadeData(CDStrs& datas)
{
	m_datas.clear();

	int n = datas.size();
	//显示
	int num = n;
	if (m_nCheckType == CHECK_WORKTIME) num *= 2;
	CString tmp = m_strMsg20;
	switch (m_nCheckType)
	{
	case CHECK_CHUQIN:
	case CHECK_QUEQIN:
		m_strMsg2.Format("%s%d次", tmp, num);
		break;
	case CHECK_JIXIAO:
		//m_strMsg2.Format("%s%d人", tmp, num);
		break;
	case CHECK_WORKTIME:
		m_strMsg2.Format("%s%d小时", tmp, num);
		break;
	}

	UpdateCStatic();

	if (n == 0) return;


	switch (m_nQueryType)
	{
	case 0: //按天
		{
				m_datas = datas;
				for (int i = 0; i < n; i++)
				{
					int nClass = atoi(m_datas[i][1]) - 1;
					m_datas[i][1] = GetClassTime(nClass);
				}
				break;
		}
	case 1: //按月
		{
				int sum_m = 0;
				CString strThisMonth = datas[0][0].Left(7);
				for (int i = 0; i < n; i++)
				{
					if (datas[i][0].Left(7) == strThisMonth)
						sum_m++;
					else
					{
						CStrs strs;
						strs.push_back(strThisMonth);
						strs.push_back("");
						CString strSum;
						if (m_nCheckType == CHECK_WORKTIME) sum_m *= 2;
						strSum.Format("%d", sum_m);
						strs.push_back(strSum);
						m_datas.push_back(strs);

						sum_m = 1;
						strThisMonth = datas[i][0].Left(7);
					}
				}

				CStrs strs;
				strs.push_back(strThisMonth);
				strs.push_back("");
				CString strSum;
				if (m_nCheckType == CHECK_WORKTIME) sum_m *= 2;
				strSum.Format("%d", sum_m);
				strs.push_back(strSum);
				m_datas.push_back(strs);
				break;
		}
	case 2: //按年
		{
				int sum = 0;
				CString strThisYear = datas[0][0].Left(4);
				for (int i = 0; i < n; i++)
				{
					if (datas[i][0].Left(4) == strThisYear)
						sum++;
					else
					{
						CStrs strs;
						strs.push_back(strThisYear);
						strs.push_back("");
						CString strSum;
						if (m_nCheckType == CHECK_WORKTIME) sum *= 2;
						strSum.Format("%d", sum);
						strs.push_back(strSum);
						m_datas.push_back(strs);

						sum = 1;
						strThisYear = datas[i][0].Left(7);
					}
				}
				CStrs strs;
				strs.push_back(strThisYear);
				strs.push_back("");
				CString strSum;
				if (m_nCheckType == CHECK_WORKTIME) sum *= 2;
				strSum.Format("%d", sum);
				strs.push_back(strSum);
				m_datas.push_back(strs);
				break;
		}
	}

}

CString CDlgCoachCheck::GetSelDate(int nID)
{
	UpdateData(TRUE);
	CString date, year, month, day;

	if (nID == 0) //起始时间
	{
		year = m_strYearS;
		month = m_strMonthS;
		day = m_strDayS;
	}
	else if (nID == 1)  //终止时间
	{
		year = m_strYearE;
		month = m_strMonthE;
		day = m_strDayE;
	}

	switch (m_nQueryType)
	{
	case 0: //按天
		date.Format("%s/%s/%s", year, month, day);
		break;
	case 1: //按月
		date.Format("%s/%s/00", year, month);
		break;
	case 2: //按年
		date.Format("%s/00/00", year);
		break;
	}

	return date;
}

void CDlgCoachCheck::OnBnClickedQuery()
{
	Refresh();
}


void CDlgCoachCheck::OnBnClickedSettoday()
{
	CTime t = CTime::GetCurrentTime();

	m_strYearE = t.Format("%Y");
	m_strMonthE = t.Format("%m");
	m_strDayE = t.Format("%d");
	UpdateData(FALSE);
}


HBRUSH CDlgCoachCheck::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_MSG3:
	{
					 CFont font3;
					 font3.CreateFontA(15, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
						 0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体");
					 pDC->SelectObject(&font3);
					 pDC->SetTextColor(COLOR_LITTLE);
					 font3.DeleteObject();
					 break;
	}
	case IDC_MSG2:
	{
					 CFont font2;
					 font2.CreateFontA(20, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
						 0, 0, 0, VARIABLE_PITCH | FF_SWISS, "黑体");
					 pDC->SelectObject(&font2);
					 font2.DeleteObject();
					 break;
	}
	case IDC_MSG1:
	{
					 CFont font1;
					 font1.CreateFontA(20, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
						 0, 0, 0, VARIABLE_PITCH | FF_SWISS, "黑体");
					 pDC->SelectObject(&font1);
					 font1.DeleteObject();
					 break;
	}
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
