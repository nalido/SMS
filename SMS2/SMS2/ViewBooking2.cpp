// ViewBooking2.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewBooking2.h"
#include "MainFrm.h"


// CViewBooking2

IMPLEMENT_DYNCREATE(CViewBooking2, CBCGPFormView)

CViewBooking2::CViewBooking2()
	: CBCGPFormView(CViewBooking2::IDD)
{
	EnableVisualManagerStyle();

	m_isToday = FALSE;
}

CViewBooking2::~CViewBooking2()
{
}

void CViewBooking2::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation1);
	DDX_Control(pDX, IDC_GRID2, m_wndGridLocation2);
	DDX_Control(pDX, IDC_GRID3, m_wndGridLocation3);
	DDX_Control(pDX, IDC_PRINT, m_wndPrint);
}

BEGIN_MESSAGE_MAP(CViewBooking2, CBCGPFormView)
	ON_BN_CLICKED(IDC_SEL_DAY, &CViewBooking2::OnBnClickedSelDay)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
END_MESSAGE_MAP()


// CViewBooking2 诊断

#ifdef _DEBUG
void CViewBooking2::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewBooking2::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewBooking2 消息处理程序

static BOOL CALLBACK Grid1Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewBooking2* pThis = (CViewBooking2*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas1.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas1.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas1[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
		}
	}

	return TRUE;
}

static BOOL CALLBACK Grid2Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewBooking2* pThis = (CViewBooking2*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas2.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas2.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas2[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
		}
	}

	return TRUE;
}


void CALLBACK CViewBooking2::OnGrid1Click(LPVOID lParam)
{
	CViewBooking2* pThis = (CViewBooking2*)lParam;

	CBCGPGridRow* pRow = pThis->m_wndGrid1.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
	}
}

void CALLBACK CViewBooking2::OnGrid2Click(LPVOID lParam)
{
	CViewBooking2* pThis = (CViewBooking2*)lParam;

	CBCGPGridRow* pRow = pThis->m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
	}
}

void CALLBACK CViewBooking2::OnGrid3DbClick(LPVOID lParam)
{
	CViewBooking2* pThis = (CViewBooking2*)lParam;

	CBCGPGridRow* pRow = pThis->m_wndGrid3.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
	}
}

void CViewBooking2::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


	//初始化表格

	//Grid1
	CRect rectGrid;
	m_wndGridLocation1.GetClientRect(&rectGrid);
	m_wndGridLocation1.MapWindowPoints(this, &rectGrid); //转为桌面坐标
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid1.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid1.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid1.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid1.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid1.SetReadOnly();
	m_wndGrid1.SetWholeRowSel();
	m_wndGrid1.SetSingleSel(); //只能选一个
	m_wndGrid1.EnableRowHeader(TRUE);
	m_wndGrid1.EnableLineNumbers();


	int hw = m_wndGrid1.GetRowHeaderWidth();
	LPCTSTR arrColumns[] = { _T("姓名"), _T("性别"), _T("申领"), _T("课时") };
	int w[4] = { 70, 50, 80, 0 };
	w[3] = m_wndGrid1.GetLastColWidth(w, 4, rectGrid.Width());
	for (int nColumn = 0; nColumn < 4; nColumn++)
	{
		m_wndGrid1.InsertColumn(nColumn, arrColumns[nColumn], w[nColumn]);
		m_wndGrid1.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid1.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid1.EnableVirtualMode(Grid1Callback, (LPARAM)this);
	m_wndGrid1.SetCallBack_Clk(OnGrid1Click);


	//Grid2
	m_wndGridLocation2.GetClientRect(&rectGrid);
	m_wndGridLocation2.MapWindowPoints(this, &rectGrid); //转为桌面坐标
	m_wndGrid2.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid2.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid2.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid2.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid2.SetReadOnly();
	m_wndGrid2.SetWholeRowSel();
	m_wndGrid2.SetSingleSel(); //只能选一个
	m_wndGrid2.EnableRowHeader(TRUE);
	m_wndGrid2.EnableLineNumbers();


	hw = m_wndGrid2.GetRowHeaderWidth();
	LPCTSTR arrColumns2[] = { _T("姓名"), _T("性别"), _T("绩效") };
	int w2[3] = { 70, 50, 0 };
	w2[2] = m_wndGrid2.GetLastColWidth(w2, 3, rectGrid.Width());
	for (int nColumn = 0; nColumn < 3; nColumn++)
	{
		m_wndGrid2.InsertColumn(nColumn, arrColumns2[nColumn], w2[nColumn]);
		m_wndGrid2.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid2.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid2.EnableVirtualMode(Grid2Callback, (LPARAM)this);
	m_wndGrid2.SetCallBack_Clk(OnGrid2Click);


	//Grid3
	m_wndGridLocation3.GetClientRect(&rectGrid);
	m_wndGridLocation3.MapWindowPoints(this, &rectGrid); //转为桌面坐标
	m_wndGrid3.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid3.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid3.EnableHeader(TRUE, 0); //不允许表头移动
	//// Set grid tab order (first):
	//m_wndGrid3.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid3.SetReadOnly();
	m_wndGrid3.SetWholeRowSel();
	m_wndGrid3.SetSingleSel(); //只能选一个
	m_wndGrid3.EnableRowHeader(0,0,0);

	//CBCGPGridColors theme;
	//CBCGPVisualManager::GetInstance()->OnSetGridColorTheme(&m_wndGrid3, theme);
	////m_wndGrid3.SendMessage(BCGM_ONSETCONTROLVMMODE, 1);
	//m_wndGrid3.SetColorTheme(theme);

	hw = m_wndGrid3.GetRowHeaderWidth()*2/3;
	m_wndGrid3.InsertColumn(0, "派工单", rectGrid.Width() - hw);
	m_wndGrid3.SetColumnAlign(0, HDF_CENTER);
	m_wndGrid3.SetHeaderAlign(0, HDF_CENTER);

	CStrs strs;
	strs.push_back("000");
	strs.push_back("教练员名字");
	strs.push_back("00");
	strs.push_back("学员1");
	strs.push_back("学员2");
	strs.push_back("学员3");
	m_wndGrid3.AddCaptionRow("车辆编号");
	CBCGPGridRow* pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[0]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("教练员");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[1]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("授教日期");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[2]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("授教时段");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[2]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("授教课时");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[2]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("同车学员");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[3]);
	m_wndGrid3.AddRow(pRow, FALSE);
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[4]);
	m_wndGrid3.AddRow(pRow, FALSE);
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[5]);
	m_wndGrid3.AddRow(pRow, FALSE);
	//注册虚拟列表回调函数
	m_wndGrid3.SetCallBack_DblClk(OnGrid2Click);


	//派工日期 默认为明天
	m_isToday = FALSE;
	CString strDate;
	m_tToday = CTime::GetCurrentTime();
	m_tTomorrow = m_tToday + CTimeSpan(1, 0, 0, 0);
	strDate.Format("当前派工日期：%s(点击转到今天派工)", m_tTomorrow.Format("%Y/%m/%d"));
	GetDlgItem(IDC_SEL_DAY)->SetWindowTextA(strDate);
}

void CViewBooking2::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	CString strDate("");
	strDate = m_isToday ? m_tToday.Format("%Y/%m/%d") : m_tTomorrow.Format("%Y/%m/%d");
	strSQL.Format("SELECT students.SNAME, students.GENDER, students.CAR_TYPE, bookings.CLASS_ID\
				   FROM bookings inner join students on bookings.FILE_NAME = students.FILE_NAME \
				   WHERE BOOK_DATE='%s' AND FLAG='0'", strDate);
	m_datas1.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg))
	{
		ShowMsg2Output1("查询学生信息成功");
	}
	else ShowMsg2Output1(strMsg);

	m_wndGrid1.GridRefresh(m_datas1.size());
}

void CViewBooking2::OnBnClickedSelDay()
{
	m_isToday = !m_isToday;
	CString strDate;

	if (m_isToday)
	{
		strDate.Format("当前派工日期：%s(点击转到明天派工)", m_tToday.Format("%Y/%m/%d"));
	}
	else
	{
		strDate.Format("当前派工日期：%s(点击转到今天派工)", m_tTomorrow.Format("%Y/%m/%d"));
	}
	GetDlgItem(IDC_SEL_DAY)->SetWindowTextA(strDate);
}


LRESULT CViewBooking2::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;

	if (flag == 1) //update data
	{
		Refresh();
	}

	return 0;
}