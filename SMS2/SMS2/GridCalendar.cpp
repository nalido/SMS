#include "stdafx.h"
#include "GridCalendar.h"
#include "MainFrm.h"




CGridCalendar::CGridCalendar()
{
	m_OnWork_Click = NULL;
	//m_OnDrawItem = NULL;
}


CGridCalendar::~CGridCalendar()
{
}


void CGridCalendar::SetCallBack_Clk(ONWORK onWork)
{
	m_OnWork_Click = onWork;
}

void CGridCalendar::OnSelChanged(const CBCGPGridRange&range, BOOL bSelect)
{
	if (m_OnWork_Click != NULL)
	{
		m_OnWork_Click(this->GetParent(), bSelect);
	}
}
BEGIN_MESSAGE_MAP(CGridCalendar, CBCGPGridCtrl)
//	ON_WM_PAINT()
END_MESSAGE_MAP()


void CGridCalendar::InitGrid(int wndWid)
{
	//表格初始化
	//SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	SetReadOnly();
	//SetWholeRowSel();
	SetSingleSel(); //只能选一个
	//EnableRowHeader(TRUE);
	//EnableLineNumbers();
	CFont font;
	VERIFY(font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_LIGHT,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename
	//SetFont(&font);

	LPCTSTR arrColumns[] = { _T("星期日"), _T("星期一"), _T("星期二"), _T("星期三"), _T("星期四"), _T("星期五"), _T("星期六") };
	const int nColumns = sizeof (arrColumns) / sizeof (LPCTSTR);
	int rowheaderW = GetRowHeaderWidth();
	int nColumnWidth = (wndWid - rowheaderW) / 7;
	for (int nColumn = 0; nColumn < nColumns; nColumn++)
	{
		InsertColumn(nColumn, arrColumns[nColumn], nColumnWidth);
		SetHeaderAlign(nColumn, HDF_CENTER);
		SetColumnAlign(nColumn, HDF_CENTER);
	}

	for (int nRow = 0; nRow < 15; nRow++)
	{
		CBCGPGridRow* pRow = CreateMultiLineRow(2);
		pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
		AddRow(pRow, FALSE);

		CBCGPGridRow* pRow1 = CreateMultiLineRow(5);
		pRow1->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
		AddRow(pRow1, FALSE);
	}
	AdjustLayout();
	/***********************************************************/

	//数据初始化
	CTime t = CTime::GetCurrentTime();
	m_tToday = CTime(t.GetYear(), t.GetMonth(), t.GetDay(), 0, 0, 0);
	m_PointToday = GetDay0Pos();
	//UpdateGrid();
}

void CGridCalendar::DrawItems()
{
	CBCGPGridRow* pRow;
	CBCGPGridItem* pItem;
	for (int r = 0; r < 15; r++)
	for (int c = 0; c < 7; c++)
	{
		int nday = r * 7 + c - m_PointToday.x;
		if (nday < 0) continue;
		if (nday > 89) break;

		CTime tmp = m_tToday + CTimeSpan(nday, 0, 0, 0);
		pRow = GetRow(r * 2);
		pItem = pRow->GetItem(c);
		pRow->GetItem(c)->SetValue((LPCTSTR)tmp.Format("%Y/%m/%d"));


		pRow = GetRow(r * 2 + 1);
		pItem = pRow->GetItem(c);
		CString strText;
		int m = m_nStatus[r][c][0] + m_nStatus[r][c][1] + m_nStatus[r][c][2] + m_nStatus[r][c][3];
		int n = g_nClassTotal*4 - m;
		strText.Format("可预约数\n\n%d", n);
		pItem->SetValue((LPCTSTR)strText);
		int np = m * 25 / g_nClassTotal; //课时预约比例
		if (np >= 100)
		{
			pItem->SetBackgroundColor(COLOR_NONE);
			pItem->SetTextColor(COLOR_TEXTNONE);
		}
		else if (np > 80)
		{
			pItem->SetBackgroundColor(COLOR_LITTLE); 
			pItem->SetTextColor(COLOR_TEXTNORMAL);
		}
		else
		{
			pItem->SetBackgroundColor(COLOR_MANY);
			pItem->SetTextColor(COLOR_TEXTNORMAL);
		}
	}
}

void CGridCalendar::UpdateGrid()
{
	CString strMaxDay, strMaxDay0;
	CTime t = CTime::GetCurrentTime();
	strMaxDay0 = t.Format("%Y/%m/%d");
	//CTime t(2017, 12, 1, 0, 0, 0);
	t += CTimeSpan(89, 0, 0, 0);    //该函数第一个参数为相差的天数  
	strMaxDay = t.Format("%Y/%m/%d");

	//查询数据库，若最大记录天数小于90天，则增加记录
	CString strMsg, strSQL;
	CDStrs data;
	strSQL.Format("SELECT * from workdaystat WHERE WORKDAY>='%s' AND WORKDAY<='%s'", strMaxDay0, strMaxDay);
	if (g_mysqlCon.ExecuteQuery(strSQL, data, strMsg))
	{
		ShowMsg2Output1("查询课时信息成功");
		GetClassStatus(data);
		DrawItems();
	}
	else 
	{
		ShowMsg2Output1(strMsg); 
		return;
	}
}

CTime CGridCalendar::Str2Time(CString str)
{
	int pos1, pos2;
	pos1 = str.Find('/');
	pos2 = str.ReverseFind('/');

	int nYear = atoi(str.Left(pos1));
	int nMonth = atoi(str.Mid(pos1 + 1, pos2));
	int nDay = atoi(str.Mid(pos2 + 1));

	CTime tmp(nYear, nMonth, nDay, 0, 0, 0);

	return tmp;
}

void CGridCalendar::GetClassStatus(CDStrs &data)
{
	int nCount = data.size();
	if (nCount == 0) return;

	for (int i = 0; i < 15; i++)
	for (int j = 0; j < 7; j++)
	for (int k = 0; k < 6; k++)
		m_nStatus[i][j][k] = 0; //默认状态为全空

	vector<CStrs>::iterator it = data.begin();
	for (; it < data.end(); it++)
	{
		CStrs row = *it;
		CString strDay = row[0];
		CPoint pos = GetDayPos(strDay);
		m_nStatus[pos.y][pos.x][0] = atoi(row[1]);
		m_nStatus[pos.y][pos.x][1] = atoi(row[2]);
		m_nStatus[pos.y][pos.x][2] = atoi(row[3]);
		m_nStatus[pos.y][pos.x][3] = atoi(row[4]);
		m_nStatus[pos.y][pos.x][4] = 1;

		//TRACE("[%d,%d] %d %d %d => %d\n", pos.y, pos.x, s1, s2, s3, m_nStatus[pos.y][pos.x]);
	}
}

CPoint CGridCalendar::GetDayPos(CString strDay)
{
		CTime t = Str2Time(strDay);
		CTimeSpan span = t - m_tToday;
		int nday = span.GetDays();

		nday = nday + m_PointToday.x; //从（0，0)算起的天数

		int y = nday / 7;
		int x = nday % 7;

		return CPoint(x, y);
}

CPoint CGridCalendar::GetDay0Pos()
{
	int weekday = m_tToday.GetDayOfWeek();
	weekday -= 1; //0=sunday

	return CPoint(weekday, 0);
}

void CGridCalendar::DrawSelectedItem(CPoint& pos)
{
	CBCGPGridRow* pRow;
	CBCGPGridItem* pItem;
	pRow = GetRow(pos.y * 2 + 1);
	pItem = pRow->GetItem(pos.x);
	pItem->SetBackgroundColor(COLOR_SELECTED);
	pItem->SetTextColor(COLOR_TEXTSEL);
}

void CGridCalendar::DrawSelectedItem(int r, int c)
{
	CBCGPGridRow* pRow;
	CBCGPGridItem* pItem;
	pRow = GetRow(r * 2 + 1);
	pItem = pRow->GetItem(c);
	CString strText;
	int m = m_nStatus[r][c][0] + m_nStatus[r][c][1] + m_nStatus[r][c][2] + m_nStatus[r][c][3];
	int n = g_nClassTotal * 4 - m;
	strText.Format("可预约数\n\n%d", n);
	pItem->SetValue((LPCTSTR)strText);
	pItem->SetBackgroundColor(COLOR_SELECTED);
	pItem->SetTextColor(COLOR_TEXTSEL);
}

void CGridCalendar::DrawSelectedItems()
{
	for (int r = 0; r < 15; r++)
	for (int c = 0; c < 7; c++)
	{
		int nday = r * 7 + c - m_PointToday.x;
		if (nday < 0) continue;
		if (nday > 89) break;

		if (m_nStatus[r][c][5])
		{
			CBCGPGridRow* pRow;
			CBCGPGridItem* pItem;
			pRow = GetRow(r * 2 + 1);
			pItem = pRow->GetItem(c);
			pItem->SetBackgroundColor(COLOR_SELECTED);
			pItem->SetTextColor(COLOR_TEXTSEL);
		}
	}
}


CTime CGridCalendar::GetSelectedDay(int r, int c)
{
	r = r / 2;
	int nday = r * 7 + c - m_PointToday.x;
	if (nday < 0 || nday > 89) return m_tToday - CTimeSpan(1, 0 ,  0, 0); //选择范围之外的时间

	return m_tToday + CTimeSpan(nday, 0, 0, 0);
}

//void CGridCalendar::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO:  在此处添加消息处理程序代码
//
//}
