// DlgMyBooks.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgMyBooks.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Response.h"


// CDlgMyBooks 对话框

IMPLEMENT_DYNAMIC(CDlgMyBooks, CBCGPDialog)

CDlgMyBooks::CDlgMyBooks(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgMyBooks::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgMyBooks::~CDlgMyBooks()
{
}

void CDlgMyBooks::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);
}


BEGIN_MESSAGE_MAP(CDlgMyBooks, CBCGPDialog)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
END_MESSAGE_MAP()


// CDlgMyBooks 消息处理程序


static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgMyBooks* pThis = (CDlgMyBooks*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas.begin() + nRow;
		if (!it->empty())
		{
			if (nCol == 1) //课程时间
			{
				int nClass = atoi(pThis->m_datas[nRow][nCol]) - 1;
				pdi->item.varValue = GetClassTime(nClass);
			}
			else if (nCol == 4) //预约完成情况
			{
				if (pThis->m_datas[nRow][nCol] == "1")
					pdi->item.varValue = "已派工，等待训练";
				else if (pThis->m_datas[nRow][nCol] == "2")
					pdi->item.varValue = "已完成训练";
				else if (pThis->m_datas[nRow][nCol] == "-1")
					pdi->item.varValue = "学员缺勤";
				else if (pThis->m_datas[nRow][nCol] == "-2")
					pdi->item.varValue = "教练缺勤";
				else if (pThis->m_datas[nRow][nCol] == "0")
					pdi->item.varValue = "未派工";
			}
			else
				pdi->item.varValue = pThis->m_datas[nRow][nCol];

			//颜色控制
			if (pThis->m_datas[nRow][4] == "1" || pThis->m_datas[nRow][4] == "0")
			{
				//pdi->item.clrBackground = COLOR_DOING;
			}
			else
			{
				pdi->item.clrBackground = COLOR_DONE;
			}
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
		}
	}

	return TRUE;
}


void CALLBACK CDlgMyBooks::OnGridDbClick(LPVOID lParam)
{
	CDlgMyBooks* pThis = (CDlgMyBooks*)lParam;


	CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
	int nRow = pRow->GetRowId();
	if (pThis->m_datas[nRow][4] != "1") return; //已经反馈过
	if (pThis->m_datas[nRow][0] != pThis->m_strToday) return; //未来的训练预约 不能反馈

	CResponse dlg;
	if (dlg.DoModal() == IDOK)
	{
		int rspType = dlg.m_nRspType;
		CString strService = dlg.m_strServiceScore;
		CString strSelf = dlg.m_strSelfScore;

		//CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
		//int nRow = pRow->GetRowId();

		pThis->m_datas[nRow][5] = strService;
		pThis->m_datas[nRow][6] = strSelf;
		switch (dlg.m_nRspType)
		{
		case 0:
			pThis->m_datas[nRow][4] = "2";
			pThis->UpdateCoach(nRow, 0);
			pThis->UpdateStudent(nRow, 0);
			break;
		case 1:
			pThis->m_datas[nRow][4] = "-2";
			pThis->UpdateCoach(nRow, 1);
			pThis->UpdateStudent(nRow, 2);
			break;
		case 2:
			pThis->m_datas[nRow][4] = "-1";
			pThis->UpdateCoach(nRow, 2);
			pThis->UpdateStudent(nRow, 1);
			break;
		}
		pThis->PostMessageA(WM_USER_UPDATE_VIEW, (WPARAM)2);
	}
}

BOOL CDlgMyBooks::OnInitDialog()
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
	LPCTSTR arrColumns[] = { _T("训练日期"), _T("训练时间"), _T("车辆编号"), _T("教练"), _T("完成情况")
		, _T("服务评价"), _T("自我评价") };
	const int nColumns = sizeof (arrColumns) / sizeof (LPCTSTR);
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
	m_wndGrid.SetCallBack_DblClk(OnGridDbClick);

	CTime t = CTime::GetCurrentTime();
	m_strToday = t.Format("%Y/%m/%d");

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

LRESULT CDlgMyBooks::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		Refresh();
		break;
	case 2:
		m_wndGrid.GridRefresh(m_datas.size());
		break;
	}

	return 0;
}

void CDlgMyBooks::Refresh()
{
	CString strMsg, strSQL;
	strSQL.Format("SELECT bookings.BOOK_DATE, bookings.CLASS_ID, bookings.ORDER_CAR, coachinfo.SName\
				  	, bookings.FLAG, bookings.SERVICE_SCORE, bookings.SELF_SCORE, bookings.FILE_NAME, \
					bookings.ORDER_COACH FROM bookings \
					inner join students ON bookings.FILE_NAME = students.FILE_NAME \
					left join coachinfo ON bookings.ORDER_COACH = coachinfo.FILE_NUM\
					WHERE bookings.FILE_NAME='%s' \
					ORDER BY bookings.BOOK_DATE, bookings.CLASS_ID, bookings.ORDER_CAR",
					m_strStuID);
	m_datas.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);


	m_wndGrid.GridRefresh(m_datas.size());
}

void CDlgMyBooks::UpdateCoach(int nRow, int flag)
{
	CString strMsg, strSQL;
	CString strCoachID = m_datas[nRow][8];
	CString strClassID = m_datas[nRow][1];
	CString strBookDate = m_datas[nRow][0];
	if (flag == 0) //正常，出勤表添加一条记录， 绩效加一, 工时加2小时
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID, STUDENT)\
					  	VALUES('%s', '%s', 1, 2, '%s', 1)", strCoachID, strBookDate, strClassID);
	}
	else if (flag == 1) //旷工，缺勤表添加一条记录，缺勤计数加一， 绩效减设定值g_nSubForLeave
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID)\
					  	VALUES('%s', '%s', 0, 0, '%s')", strCoachID, strBookDate, strClassID);
	}
	else if (flag == 2) //正常出勤，学员缺勤，出勤表添加一条记录， 绩效不变, 工时随绩效改变
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID)\
					  	VALUES('%s', '%s', 1, 2, '%s')", strCoachID, strBookDate, strClassID);
	}

	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);

	if (flag == 1) //旷工一次就更新教练的缺勤次数
	{
		strSQL.Format("SELECT COUNT(COACH_ID) FROM coachCheck WHERE COACH_ID='%s' AND TYPE='0'", strCoachID);
		CDStrs counts;
		int count;
		g_mysqlCon.ExecuteQuery(strSQL, counts, strMsg);
		ShowMsg2Output1(strMsg);
		count = atoi(counts[0][0]);

		strSQL.Format("UPDATE coachstat SET LEAVE_N='%d' WHERE FILE_NUM='%s'", count, strCoachID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		CTime t = CTime::GetCurrentTime();
		int year = t.GetYear();
		CTime midYear(year, 6, 1, 0, 0, 0); //以每年6月1号作为半年检查的标志
		//半年内 超过3次请假为不合格, 超过半年按6次算
		int th = 3;
		if (midYear < t) th = 6;
		if (count >= th)
		{
			strSQL.Format("UPDATE coachstat SET BLACK_NAME='1' WHERE FILE_NUM='%s'", count, strCoachID);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
			ShowMsg2Output1(strMsg);
		}
	}
	else if (flag == 0) //正常完成一次，增加一次本月已上课时数
	{
		strSQL.Format("UPDATE coachstat SET CLASS_NUM=\
					  	(SELECT COUNT(COACH_ID) FROM coachCheck WHERE COACH_ID='%s' AND TYPE='1')\
						WHERE FILE_NUM='%s'", strCoachID, strCoachID);

		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}
}

void CDlgMyBooks::UpdateStudent(int nRow, int flag)
{
	CString strMsg, strSQL;
	CString strStuID = m_datas[nRow][7];
	CString strClassID = m_datas[nRow][1];
	CString strBookDate = m_datas[nRow][0];
	if (flag == 0) //正常完成训练， bookings表FLAG字段更新为“2”， 已上课时数加一
	{
		CString strService = m_datas[nRow][5];
		CString strSelf = m_datas[nRow][6];
		strSQL.Format("UPDATE bookings SET FLAG='2', SERVICE_SCORE='%s', SELF_SCORE='%s'\
					  	WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
						strService, strSelf,
						strStuID, strBookDate, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);


		strSQL.Format("UPDATE students SET CLASS_NUM = CLASS_NUM + 1 WHERE FILE_NAME='%s'", strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}
	else if (flag == 1) //学员缺勤，FLAG字段更新为“-1”， 已上课时数不变
	{
		strSQL.Format("UPDATE bookings SET FLAG='-1'\
					  	WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
						strStuID, strBookDate, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}
	else if (flag == 2) //教练缺勤，FLAG字段更新为“-1”， 已上课时数不变
	{
		strSQL.Format("UPDATE bookings SET FLAG='-2'\
					  	WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
						strStuID, strBookDate, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}

}
