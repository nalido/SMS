// ViewKPI.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewKPI.h"


// CViewKPI

IMPLEMENT_DYNCREATE(CViewKPI, CBCGPFormView)

CViewKPI::CViewKPI()
	: CBCGPFormView(CViewKPI::IDD)
	, m_threadMySQL(this, ThreadMySQLCallback)
{
	EnableVisualManagerStyle();
}

CViewKPI::~CViewKPI()
{
	m_threadMySQL.StopThread();
}

void CViewKPI::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);
}

BEGIN_MESSAGE_MAP(CViewKPI, CBCGPFormView)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_EXPORT, &CViewKPI::OnBnClickedExport)
END_MESSAGE_MAP()


// CViewKPI 诊断

#ifdef _DEBUG
void CViewKPI::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewKPI::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewKPI 消息处理程序

static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewKPI* pThis = (CViewKPI*)lp;

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


void CViewKPI::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


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
	m_arrColumns.push_back("工号");
	m_arrColumns.push_back("姓名");
	m_arrColumns.push_back("取样数");
	m_arrColumns.push_back("满意度");
	m_arrColumns.push_back("满意率");
	m_arrColumns.push_back("合格率");
	m_arrColumns.push_back("设备完好率");
	m_arrColumns.push_back("介绍学员数");
	m_arrColumns.push_back("介绍目标数");
	m_arrColumns.push_back("总分");

	int nColumns = m_arrColumns.size();
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGrid.InsertColumn(i, m_arrColumns[i], nColumnWidth);
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);

	m_tToday = GetServerTime();//CTime::GetCurrentTime();

}

void CViewKPI::Refresh()
{
	//获取上个月月份
	CString strLastMonth = GetLastMonth(m_tToday);

	m_datas.clear();
	CString strSQL, strMsg;
	strSQL.Format("SELECT COACH_ID, COACH, SAMPLE_NUM, SATISFIED_DEGREE, SATISFIED_PERCENT,\
				  	SKILL_PERCENT, DEVICE_GOOD, STUDENT_NEWS, STUDENT_TARGET, SCORE\
					FROM KPIS WHERE KMONTH='%s' ORDER BY SCORE DESC",
					strLastMonth);
	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);

	if (m_datas.size() == 0) //没有上个月的信息
	{
		m_threadMySQL.StartThread();
	}
	else 
		m_wndGrid.GridRefresh(m_datas.size());
}


LRESULT CViewKPI::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		Refresh();
		break;
	case 3:
		m_threadMySQL.StopThread();
	case 2:
		m_wndGrid.GridRefresh(m_datas.size());
		break;
	}

	return 0;
}

void CViewKPI::OnBnClickedExport()
{
	ExportExcel(m_arrColumns, m_datas);
}


///快速排序
void QuikSort(std::vector<pair<int, int>>& v, int left, int right)
{
	if (left < right)
	{
		int key = v[left].second;
		int index = v[left].first;
		//printf("%d\n\n", key);
		int low = left;
		int high = right;
		while (low < high)
		{
			while (low < high && v[high].second <= key)
			{
				high--;
			}
			v[low].first = v[high].first;
			v[low].second = v[high].second;

			while (low < high && v[low].second >= key)
			{
				low++;
			}
			v[high] = v[low];
		}
		v[low].second = key;
		v[low].first = index;

		//CString strLog, strLine("");
		//int size = v.size();
		//for (int i = 0; i < size; i++)
		//{
		//	strLog.Format("%d ", v[i].second);
		//	strLine = strLine + strLog;
		//}
		//LOG("quik.log", strLine, 0);

		if (low > 0)
		{
			QuikSort(v, left, low - 1);
			QuikSort(v, low + 1, right);
		}
	}
}

void CALLBACK CViewKPI::ThreadMySQLCallback(LPVOID pParam, HANDLE hCloseEvent)
{
	CViewKPI* pThis = (CViewKPI*)pParam;
	if (pThis->m_hWnd == NULL) return;

	CString strSQL, strMsg;
	CString thisMonth = pThis->m_tToday.Format("%Y/%m%%");

	CDStrs coaches;
	strSQL.Format("SELECT COACH_ID, COACH FROM kpis WHERE KMONTH LIKE '%s'", thisMonth);
	g_mysqlCon.ExecuteQuery(strSQL, coaches, strMsg);

	//更新KPI数据
	int nc = coaches.size();
	if (nc == 0) return;
	//排序索引
	std::vector<pair<int, int>> c;
	for (int nCoach = 0; nCoach < nc; nCoach++)
	{
		pair<int, int> item;
		item.first = nCoach;

		CDStrs datas;
		//学员评分记录
		strSQL.Format("SELECT BOOK_DATE, SERVICE_SCORE, SELF_SCORE FROM bookings WHERE ORDER_COACH='%s' \
					  	AND BOOK_DATE LIKE '%s' AND FLAG='2' ORDER BY BOOK_DATE",
						coaches[nCoach][0], thisMonth);
		g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);

		int nStudents = datas.size();
		if (nStudents == 0)
		{
			for (int i = 0; i < 8; i++)
			{
				coaches[nCoach].push_back("0");
			}
			item.second = 0;
			c.push_back(item);
			continue;
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

		CString strData;
		strData.Format("%d", nStudents);
		coaches[nCoach].push_back(strData);
		double d = service*1.0 / 100;
		strData.Format("%.2f", d);
		coaches[nCoach].push_back(strData);
		d = good*1.0 / 100;
		strData.Format("%.2f", d);
		coaches[nCoach].push_back(strData);
		d = self*1.0 / 100;
		strData.Format("%.2f", d);
		coaches[nCoach].push_back(strData);
		coaches[nCoach].push_back("100"); //设备完好率
		coaches[nCoach].push_back("0"); //介绍学生数
		coaches[nCoach].push_back("0"); //介绍学生目标数
		strData.Format("%.2f", KPI);
		coaches[nCoach].push_back(strData);

		//更新数据库
		int kpi = KPI * 100;
		item.second = kpi;
		strSQL.Format("UPDATE KPIS SET SAMPLE_NUM='%d', SATISFIED_DEGREE='%d', SATISFIED_PERCENT='%d',\
						SKILL_PERCENT='%d', SCORE='%d'\
						WHERE COACH_ID='%s' AND KMONTH='%s'",
						nStudents, service, good, self, kpi, coaches[nCoach][0], thisMonth.Left(7));
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		c.push_back(item);
	}

	//排序
	QuikSort(c, 0, nc-1);

	pThis->m_datas.clear();
	for (int i = 0; i < nc; i++)
	{
		pThis->m_datas.push_back(coaches[c[i].first]);
	}
	//pThis->m_datas = coaches;
	try
	{
		pThis->PostMessageA(WM_USER_UPDATE_VIEW, (WPARAM)3);
	}
	catch (...){}
}
