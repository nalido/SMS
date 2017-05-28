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

	DDX_Radio(pDX, IDC_RADIO1, m_nQueryType);

	DDX_Text(pDX, IDC_MSG1, m_strMsg1);
	DDX_Text(pDX, IDC_MSG2, m_strMsg2);
	DDX_Text(pDX, IDC_MSG3, m_strMsg3);


	DDX_Control(pDX, IDC_DATES, m_DateS);
	DDX_Control(pDX, IDC_DATE2, m_DateE);

	DDX_Control(pDX, IDC_MSG1, m_Static_Msg1);
	DDX_Control(pDX, IDC_MSG2, m_Static_Msg2);
	DDX_Control(pDX, IDC_MSG3, m_Static_Msg3);
}


BEGIN_MESSAGE_MAP(CDlgCoachCheck, CBCGPDialog)
//	ON_CBN_CLOSEUP(IDC_COMBO2, &CDlgCoachCheck::OnCbnCloseupCombo2)
ON_BN_CLICKED(IDC_RADIO1, &CDlgCoachCheck::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, &CDlgCoachCheck::OnBnClickedRadio2)
ON_BN_CLICKED(IDC_RADIO4, &CDlgCoachCheck::OnBnClickedRadio4)
ON_BN_CLICKED(IDC_QUERY, &CDlgCoachCheck::OnBnClickedQuery)
//ON_BN_CLICKED(IDC_SETTODAY, &CDlgCoachCheck::OnBnClickedSettoday)
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_EXPORT, &CDlgCoachCheck::OnBnClickedExport)
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


static BOOL CALLBACK GridMCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgCoachCheck* pThis = (CDlgCoachCheck*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datasM.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datasM.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datasM[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
		}
	}

	return TRUE;
}

static BOOL CALLBACK GridYCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgCoachCheck* pThis = (CDlgCoachCheck*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datasY.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datasY.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datasY[nRow][nCol];
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
	//按天统计
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

	//按月统计
	m_wndGridM.Create(nStyle, rect, this, IDC_GRID_STUPRO+1);
	m_wndGridM.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGridM.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGridM.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGridM.SetReadOnly();
	m_wndGridM.SetWholeRowSel();
	m_wndGridM.SetSingleSel(); //只能选一个
	m_wndGridM.EnableRowHeader(TRUE);
	m_wndGridM.EnableLineNumbers();

	//按年统计
	m_wndGridY.Create(nStyle, rect, this, IDC_GRID_STUPRO+2);
	m_wndGridY.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGridY.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGridY.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGridY.SetReadOnly();
	m_wndGridY.SetWholeRowSel();
	m_wndGridY.SetSingleSel(); //只能选一个
	m_wndGridY.EnableRowHeader(TRUE);
	m_wndGridY.EnableLineNumbers();

	int nColumn = 0;
	int hw = m_wndGrid.GetRowHeaderWidth();
	std::vector<CString> arrColumns;
	std::vector<CString> arrColumnsM;
	switch (m_nCheckType)
	{
	case CHECK_CHUQIN:
		arrColumns.push_back("出勤日期");
		arrColumns.push_back("出勤时间");
		arrColumns.push_back("学员个数");

		arrColumnsM.push_back("统计时期");
		arrColumnsM.push_back("记录总数");
		arrColumnsM.push_back("出勤统计");
		break;
	case CHECK_QUEQIN:
		arrColumns.push_back("缺勤日期");
		arrColumns.push_back("缺勤时间");
		arrColumns.push_back("学员个数");

		arrColumnsM.push_back("统计时期");
		arrColumnsM.push_back("记录总数");
		arrColumnsM.push_back("缺勤统计");
		break;
	case CHECK_WORKTIME:
		arrColumns.push_back("工作日期");
		arrColumns.push_back("工作时间");
		arrColumns.push_back("工作时长");

		arrColumnsM.push_back("统计时期");
		arrColumnsM.push_back("记录总数");
		arrColumnsM.push_back("工时统计");
		break;
	case CHECK_JIXIAO:
		arrColumns.push_back("记录日期");
		arrColumns.push_back("记录时间");
		arrColumns.push_back("学员个数");

		arrColumnsM.push_back("统计时期");
		arrColumnsM.push_back("记录总数");
		arrColumnsM.push_back("绩效统计");
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

		arrColumnsM = arrColumns;
		break;
	}
	m_arrColumnsM = arrColumnsM;
	m_arrColumns = arrColumns;

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


	nColumns = arrColumnsM.size();
	w = rect.Width() - hw;
	nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGridM.InsertColumn(i, arrColumnsM[i], nColumnWidth);
		m_wndGridM.SetColumnAlign(i, HDF_CENTER);
		m_wndGridM.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGridM.EnableVirtualMode(GridMCallback, (LPARAM)this);
	m_wndGridM.EnableWindow(FALSE);


	nColumns = arrColumnsM.size();
	w = rect.Width() - hw;
	nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGridY.InsertColumn(i, arrColumnsM[i], nColumnWidth);
		m_wndGridY.SetColumnAlign(i, HDF_CENTER);
		m_wndGridY.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGridY.EnableVirtualMode(GridYCallback, (LPARAM)this);
	m_wndGridY.EnableWindow(FALSE);

	//默认统计当月
	m_DateS.SetFormat("yyyy/MM/dd");
	m_DateE.SetFormat("yyyy/MM/dd");
	//获得本月
	CTime t = GetServerTime() + CTimeSpan(31, 0, 0, 0);
	m_DateE.SetTime(&t);
	t = GetServerTime();
	int month = t.GetMonth();
	int year = t.GetYear();
	CTime ts(year, month, 1, 0, 0, 0);
	m_DateS.SetTime(&ts);


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
		m_nQueryType = 1;
		m_strMsg20.Format("本月绩效为");
		UpdateData(FALSE);
		GetJiXiao();
		UpdateCStatic();
	}
	else if (m_nCheckType == CHECK_KPI) //KPI只能按月查询
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO4)->EnableWindow(FALSE);
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


void CDlgCoachCheck::OnBnClickedRadio1()
{
	m_nQueryType = 0;

	m_wndGrid.EnableWindow(TRUE);
	m_wndGridM.EnableWindow(FALSE);
	m_wndGridY.EnableWindow(FALSE);

	m_wndGrid.GridRefresh(m_datas.size());
}


void CDlgCoachCheck::OnBnClickedRadio2()
{
	m_nQueryType = 1;

	m_wndGrid.EnableWindow(FALSE);
	m_wndGridM.EnableWindow(TRUE);
	m_wndGridY.EnableWindow(FALSE);
	m_wndGridM.GridRefresh(m_datasM.size());
}


void CDlgCoachCheck::OnBnClickedRadio4()
{
	m_nQueryType = 2;

	m_wndGrid.EnableWindow(FALSE);
	m_wndGridM.EnableWindow(FALSE);
	m_wndGridY.EnableWindow(TRUE);
	m_wndGridY.GridRefresh(m_datasY.size());
}


void CDlgCoachCheck::GetKPI()
{
	CString strMsg, strSQL;
	CString strDayStart;
	CString strDayEnd;
	m_DateS.GetWindowTextA(strDayStart);
	m_DateE.GetWindowTextA(strDayEnd);

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
	CString strDayStart;
	CString strDayEnd;
	m_DateS.GetWindowTextA(strDayStart);
	m_DateE.GetWindowTextA(strDayEnd);
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
	CString strDayStart;
	CString strDayEnd;
	m_DateS.GetWindowTextA(strDayStart);
	m_DateE.GetWindowTextA(strDayEnd);
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

		m_datasM = m_datasY = m_datas;

		break;
	}

	m_wndGridM.EnableWindow(FALSE);
	m_wndGridY.EnableWindow(FALSE);
	m_wndGrid.EnableWindow(TRUE);
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
	m_datasM.clear();
	m_datasY.clear();

	int n = datas.size();
	if (n == 0) return;

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


	//switch (m_nQueryType)
	{
	//case 0: //按天
		{
				m_datas = datas;
				for (int i = 0; i < n; i++)
				{
					int nClass = atoi(m_datas[i][1]) - 1;
					m_datas[i][1] = GetClassTime(nClass);
				}
				//break;
		}
	//case 1: //按月
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
						CString strSum;
						strSum.Format("%d", sum_m);
						strs.push_back(strSum);
						if (m_nCheckType == CHECK_WORKTIME) sum_m *= 2;
						strSum.Format("%d", sum_m);
						strs.push_back(strSum);
						m_datasM.push_back(strs);

						sum_m = 1;
						strThisMonth = datas[i][0].Left(7);
					}
				}

				CStrs strs;
				strs.push_back(strThisMonth);
				CString strSum;
				strSum.Format("%d", sum_m);
				strs.push_back(strSum);
				if (m_nCheckType == CHECK_WORKTIME) sum_m *= 2;
				strSum.Format("%d", sum_m);
				strs.push_back(strSum);
				m_datasM.push_back(strs);
				//break;
		}
	//case 2: //按年
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
						CString strSum;
						strSum.Format("%d", sum);
						strs.push_back(strSum);
						if (m_nCheckType == CHECK_WORKTIME) sum *= 2;
						strSum.Format("%d", sum);
						strs.push_back(strSum);
						m_datasY.push_back(strs);

						sum = 1;
						strThisYear = datas[i][0].Left(7);
					}
				}
				CStrs strs;
				strs.push_back(strThisYear);
				CString strSum;
				strSum.Format("%d", sum);
				strs.push_back(strSum);
				if (m_nCheckType == CHECK_WORKTIME) sum *= 2;
				strSum.Format("%d", sum);
				strs.push_back(strSum);
				m_datasY.push_back(strs);
				//break;
		}
	}

}


void CDlgCoachCheck::OnBnClickedQuery()
{
	Refresh();
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


void CDlgCoachCheck::OnBnClickedExport()
{
	CString strDayS;
	CString strDayE;
	m_DateS.GetWindowTextA(strDayS);
	m_DateE.GetWindowTextA(strDayE);
	
	if (m_nQueryType == 1)
	{
		strDayS = strDayS.Left(7);
		strDayE = strDayE.Left(7);
	}
	else if (m_nQueryType == 2)
	{
		strDayS = strDayS.Left(4);
		strDayE = strDayE.Left(4);
	}

	CString strFileName = arrType[m_nCheckType] + m_strCoach + m_strCoachID + "-" + strDayS + "-" + strDayE + ".csv"; //g_strOutPath + "\\" + 
	strFileName.Replace('/', '_');

	switch (m_nQueryType)
	{
	case 0:
		ExportExcel(strFileName, m_arrColumns, m_datas);
		break;
	case 1:
		ExportExcel(strFileName, m_arrColumnsM, m_datasM);
		break;
	case 2:
		ExportExcel(strFileName, m_arrColumnsM, m_datasY);
		break;
	}
}
