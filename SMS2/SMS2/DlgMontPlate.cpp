// DlgMontPlate.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgMontPlate.h"
#include "afxdialogex.h"


// CDlgMontPlate 对话框

IMPLEMENT_DYNAMIC(CDlgMontPlate, CBCGPDialog)

CDlgMontPlate::CDlgMontPlate(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgMontPlate::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgMontPlate::~CDlgMontPlate()
{
}

void CDlgMontPlate::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CALENDAR, m_calendarLocation);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);

}


BEGIN_MESSAGE_MAP(CDlgMontPlate, CBCGPDialog)
	ON_BN_CLICKED(IDC_ADD, &CDlgMontPlate::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DEL, &CDlgMontPlate::OnBnClickedDel)
END_MESSAGE_MAP()


// CDlgMontPlate 消息处理程序

static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgMontPlate* pThis = (CDlgMontPlate*)lp;

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


BOOL CDlgMontPlate::OnInitDialog()
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
	arrColumns.push_back("休假日期");

	int nColumns = arrColumns.size();
	//m_nColumns = nColumns;
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


	CRect rectCalendar;
	m_calendarLocation.GetClientRect(&rectCalendar);
	m_calendarLocation.MapWindowPoints(this, &rectCalendar);

	m_wndCalendarCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
		rectCalendar, this, (UINT)-1);

	m_wndCalendarCtrl.EnableVisualManagerStyle();
	m_wndCalendarCtrl.SetSingleMonthMode();
	m_wndCalendarCtrl.EnableMutipleSelection(0); //不能多选

	m_tToday = CTime::GetCurrentTime();
	m_strMonth = m_tToday.Format("%Y/%m/__");

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgMontPlate::Refresh()
{
	CString strMsg, strSQL;
	strSQL.Format("SELECT LEAVE_DATE, LEAVE_LEN FROM askforleave WHERE COACH_ID='%s' AND LEAVE_DATE LIKE '%s' ORDER BY LEAVE_DATE", m_strCoachID, m_strMonth);
	m_datas.clear();
	m_arMarkedDates.RemoveAll();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);

	int n = m_datas.size();
	for (int i = 0; i < n; i++)
	{
		CString strDate = m_datas[i][0];
		CTime t = Str2Time(strDate);
		COleDateTime dayColor(t.GetYear(), t.GetMonth(), t.GetDay(), 0, 0, 0);
		m_arMarkedDates.Add(dayColor.m_dt);
		m_wndCalendarCtrl.SetDateColor(dayColor, RGB(255, 0, 0));
	}
	m_wndCalendarCtrl.MarkDates(m_arMarkedDates);
	m_wndGrid.GridRefresh(m_datas.size()); 
}

void CDlgMontPlate::OnBnClickedAdd()
{
	UpdateData();

	CString strDay;
	COleDateTime t = m_wndCalendarCtrl.GetDate();
	if (m_wndCalendarCtrl.IsDateMarked(t)) //已被选过
		return;

	strDay = t.Format("%Y/%m/%d");

	CString strMsg, strSQL;
	strSQL.Format("INSERT INTO askforleave (LEAVE_DATE, LEAVE_LEN, COACH_ID) VALUES('%s', '1', '%s')", strDay, m_strCoachID);
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);

	Refresh();
}


void CDlgMontPlate::OnBnClickedDel()
{
	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strDate, strSQL, strMsg;
		strDate = m_datas[nRow][0];
		strSQL.Format("DELETE FROM askforleave WHERE LEAVE_DATE='%s' AND COACH_ID = '%s'", strDate, m_strCoachID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);


		CTime t = Str2Time(strDate);
		COleDateTime dayColor(t.GetYear(), t.GetMonth(), t.GetDay(), 0, 0, 0);
		m_wndCalendarCtrl.SetDateColor(dayColor, RGB(0, 0, 0));

		Refresh();
	}
}
