// ViewBooking2.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewBooking2.h"
#include "MainFrm.h"
#include "xPublic\MyPrint.h"
#include "OrderDetail.h"
#include "Orders.h"


// CViewBooking2

IMPLEMENT_DYNCREATE(CViewBooking2, CBCGPFormView)

CViewBooking2::CViewBooking2()
	: CBCGPFormView(CViewBooking2::IDD)
{
	EnableVisualManagerStyle();

	m_isToday = FALSE;
	m_canChangeOrder = TRUE;
}

CViewBooking2::~CViewBooking2()
{
}

void CViewBooking2::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation1);
	DDX_Control(pDX, IDC_GRID2, m_wndGridLocation2);
	DDX_Control(pDX, IDC_ORDERS, m_wndGridLocation3);
	DDX_Control(pDX, IDC_PRINT, m_wndPrint); 
	DDX_Control(pDX, IDC_DATE, m_wndDate);
	DDX_Control(pDX, IDC_CARS, m_Combo_Cars);
}

BEGIN_MESSAGE_MAP(CViewBooking2, CBCGPFormView)
	ON_BN_CLICKED(IDC_SEL_DAY, &CViewBooking2::OnBnClickedSelDay)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_DO_PRINT, &CViewBooking2::OnBnClickedDoPrint)
//	ON_STN_CLICKED(IDC_GRID1, &CViewBooking2::OnStnClickedGrid1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ORDER, &CViewBooking2::OnBnClickedOrder)
	ON_BN_CLICKED(IDC_RESET_PRINT, &CViewBooking2::OnBnClickedResetPrint)
	ON_CBN_SELCHANGE(IDC_CARS, &CViewBooking2::OnCbnSelchangeCars)
	ON_BN_CLICKED(IDC_ORDER_CHANGE, &CViewBooking2::OnBnClickedOrderChange)
	ON_BN_CLICKED(IDC_ORDER_QUERY, &CViewBooking2::OnBnClickedOrderQuery)
	ON_BN_CLICKED(IDC_AUTO_ORDER, &CViewBooking2::OnBnClickedAutoOrder)
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
			if (nCol == 2) //课程时间
			{
				int nClass = atoi(pThis->m_datas1[nRow][nCol]) - 1;
				pdi->item.varValue = pThis->m_wndPrint.m_printData.GetClassTime(nClass);
			}
			else
				pdi->item.varValue = pThis->m_datas1[nRow][nCol];

			//颜色控制
			//int cols = pThis->m_datas1[nRow].size() - 1;
			if (pThis->m_datas1[nRow][5] == "0") //未派工
			{
				pdi->item.clrBackground = RGB(255,255,255);

				pdi->item.clrText = RGB(0, 0, 0);
				if (!pThis->CanBeSelected(nRow)) 
				{
					pdi->item.clrText = COLOR_TEXTNONE;
				}
			}
			else if (pThis->m_datas1[nRow][5] == "1") //已派工
			{
				pdi->item.clrBackground = COLOR_MANY;
			}
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

static BOOL CALLBACK Grid3Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewBooking2* pThis = (CViewBooking2*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_orderIndexes.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<Indexes>::iterator it = pThis->m_orderIndexes.begin() + nRow;
		if (!it->empty() && nCol<pThis->m_orderIndexes[nRow].size())
		{
			int index = (*it)[nCol];
			switch (nCol)
			{
			case 0:
				pdi->item.varValue = pThis->m_datas3[index][0];
				break;
			case 1:
				pdi->item.varValue = pThis->m_datas2[index][0];
				break;
			default:
				pdi->item.varValue = pThis->m_datas1[index][0];
				break;
			}
		}
		else
		{
			pdi->item.varValue = "";
		}
	}

	return TRUE;
}


void CALLBACK CViewBooking2::OnGrid1DbClick(LPVOID lParam)
{
	CViewBooking2* pThis = (CViewBooking2*)lParam;

	CBCGPGridRow* pRow = pThis->m_wndGrid1.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString student = pThis->m_datas1[nRow][6];
		CString classID = pThis->m_datas1[nRow][2];
		CString bookDate = pThis->m_isToday ? pThis->m_tToday.Format("%Y/%m/%d") : pThis->m_tTomorrow.Format("%Y/%m/%d");

		COrders dlg;
		dlg.m_strToday = pThis->m_tToday.Format("%Y/%m/%d");
		dlg.DoModal();
	}
}


void CALLBACK CViewBooking2::OnGrid1Click(LPVOID lParam)
{
	CViewBooking2* pThis = (CViewBooking2*)lParam;


	if (!pThis->m_canChangeOrder)
	{
		pThis->MessageBox("非法操作！请重置派工单或者点击修改按钮修改此派工单");
		return;
	}

	CBCGPGridRow* pRow = pThis->m_wndGrid1.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CSheetCtrl* pPrint = &pThis->m_wndPrint;

		int nStudent = pPrint->m_printData.m_students.size();
		int i = 0;
		for (; i < nStudent; i++)
		{
			if (pThis->m_order[2 + i] == nRow) //点击第二次为取消重复的选择
			{
				pThis->m_order.erase(pThis->m_order.begin() + 2 + i);
				pPrint->m_printData.RemoveStudentAt(i);
				pThis->m_wndPrint.Invalidate();
				nStudent--;

				pThis->m_datas1[nRow][5] = "0"; //未派工
				pThis->PostMessageA(WM_USER_UPDATE_VIEW, (WPARAM)2);
				return;
			}
		}

		if (nStudent < 3) //最多三个学员
		{
			//已经派工的不能重复派工
			if (pThis->m_datas1[nRow][5] == "1") return; 

			//不是同一节课的不能派工
			if (!pThis->CanBeSelected(nRow)) return;

			if (pThis->m_order.size() <= 2 + nStudent) pThis->m_order.push_back(nRow);
			else pThis->m_order[2 + nStudent] = nRow;

			//CString strMsg;
			//TRACE("=============\r\n");
			//for (int n = 0; n < pThis->m_order.size(); n++)
			//	TRACE("[%d]%d\r\n", n, pThis->m_order[n]);
			//TRACE("=============\r\n");


			CString name = pThis->m_datas1[nRow][0];
			CString date = pThis->m_isToday ? pThis->m_tToday.Format("%y/%m/%d") : pThis->m_tTomorrow.Format("%y/%m/%d");
			int classStep = atoi(pThis->m_datas1[nRow][3]); 
			int classID = atoi(pThis->m_datas1[nRow][2]) - 1;

			//第一个加入的学员决定派工单的授课内容
			if (pPrint->m_classInfo.nClassID == 0)
			{
				CString classType = pThis->m_datas1[nRow][4];
				CString cn;
				cn.Format("c%d", classStep + 1);
				pPrint->m_classInfo.nClassID =  xPublic::GETINT2(classType, cn, 0);
			}
			xPublic::STUDENTINFO student(name, date, classID, classStep, g_nMaxBooking);
			pPrint->m_printData.AddStudent(student);
			pThis->m_wndPrint.Invalidate();

			pThis->m_datas1[nRow][5] = "1"; //已派工
			pThis->PostMessageA(WM_USER_UPDATE_VIEW, (WPARAM)2);
		}
	}
}

void CALLBACK CViewBooking2::OnOrdersClick(LPVOID lParam)
{
	CViewBooking2* pThis = (CViewBooking2*)lParam;

	CBCGPGridRow* pRow = pThis->m_wndGrid3.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		pThis->m_nChangingOrderIndex = nRow;
		pThis->RestOrder(!pThis->m_canChangeOrder);
		Indexes indexes = pThis->m_orderIndexes[nRow];
		pThis->m_order.clear();
		pThis->m_order = indexes;

		CSheetCtrl* pPrint = &pThis->m_wndPrint;
		pPrint->m_printData.Reset();
		pPrint->Invalidate();

		pPrint->m_sheetInfo.strCarID = pThis->m_datas3[indexes[0]][0];
		pPrint->m_sheetInfo.strCoach = pThis->m_datas2[indexes[1]][0];
		pPrint->m_sheetInfo.strCoachID = pThis->m_datas2[indexes[1]][2];

		int nStudent = indexes.size() - 2;
		for (int i = 0; i < nStudent; i++)
		{
			int stuRow = indexes[i + 2];
			CString name = pThis->m_datas1[stuRow][0];
			CString date = pThis->m_isToday ? pThis->m_tToday.Format("%y/%m/%d") : pThis->m_tTomorrow.Format("%y/%m/%d");
			int classStep = atoi(pThis->m_datas1[stuRow][3]);
			int classID = atoi(pThis->m_datas1[stuRow][2]) - 1;

			//第一个加入的学员决定派工单的授课内容
			if (pPrint->m_classInfo.nClassID == 0)
			{
				CString classType = pThis->m_datas1[nRow][4];
				CString cn;
				cn.Format("c%d", classStep + 1);
				pPrint->m_classInfo.nClassID = xPublic::GETINT2(classType, cn, 0);
			}

			xPublic::STUDENTINFO student(name, date, classID, classStep, g_nMaxBooking);
			pPrint->m_printData.AddStudent(student);
		}

		pPrint->Invalidate();

		pThis->GetDlgItem(IDC_ORDER_CHANGE)->EnableWindow(TRUE);
		pThis->m_canChangeOrder = FALSE;
	}
}

void CALLBACK CViewBooking2::OnGrid2Click(LPVOID lParam)
{
	CViewBooking2* pThis = (CViewBooking2*)lParam;

	if (!pThis->m_canChangeOrder)
	{
		pThis->MessageBox("非法操作！请重置派工单或者点击修改按钮修改此派工单");
		return;
	}

	CBCGPGridRow* pRow = pThis->m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();


		CString strCoach = pThis->m_datas2[nRow][0];
		pThis->m_wndPrint.m_sheetInfo.strCoach = strCoach;
		pThis->m_wndPrint.m_sheetInfo.strCoachID = pThis->m_datas2[nRow][2];
		pThis->m_wndPrint.Invalidate();

		pThis->m_order[1] = nRow;
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
	LPCTSTR arrColumns[] = { _T("姓名"), _T("申领"), _T("预约时间"), _T("已用课时"), _T("培训类型") };
	int w[5] = { 60, 70, 100, 70, 0 };
	w[4] = m_wndGrid1.GetLastColWidth(w, 5, rectGrid.Width());
	for (int nColumn = 0; nColumn < 5; nColumn++)
	{
		m_wndGrid1.InsertColumn(nColumn, arrColumns[nColumn], w[nColumn]);
		m_wndGrid1.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid1.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid1.EnableVirtualMode(Grid1Callback, (LPARAM)this);
	m_wndGrid1.SetCallBack_Clk(OnGrid1Click);
	m_wndGrid1.SetCallBack_DblClk(OnGrid1DbClick);

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
	LPCTSTR arrColumns2[] = { _T("姓名"), _T("性别"), _T("工号"), _T("绩效") };
	int nColumns = 4;
	int w2[4] = { 70, 50, 80, 0 };
	w2[3] = m_wndGrid2.GetLastColWidth(w2, nColumns, rectGrid.Width());
	for (int nColumn = 0; nColumn < nColumns; nColumn++)
	{
		m_wndGrid2.InsertColumn(nColumn, arrColumns2[nColumn], w2[nColumn]);
		m_wndGrid2.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid2.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid2.EnableVirtualMode(Grid2Callback, (LPARAM)this);
	m_wndGrid2.SetCallBack_Clk(OnGrid2Click);


	//派工单总览
	m_wndGridLocation3.GetClientRect(&rectGrid);
	m_wndGridLocation3.MapWindowPoints(this, &rectGrid); //转为桌面坐标
	m_wndGrid3.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid3.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid3.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid3.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid3.SetReadOnly();
	m_wndGrid3.SetWholeRowSel();
	m_wndGrid3.SetSingleSel(); //只能选一个
	m_wndGrid3.EnableRowHeader(TRUE);
	m_wndGrid3.EnableLineNumbers();


	hw = m_wndGrid3.GetRowHeaderWidth();
	LPCTSTR arrColumns3[] = { _T("车辆编号"), _T("教练员"), _T("学生1"), _T("学生2"), _T("学生3") };
	int w3[5] = { 70, 50, 50, 50, 50 };
	for (int nColumn = 0; nColumn < 5; nColumn++)
	{
		m_wndGrid3.InsertColumn(nColumn, arrColumns3[nColumn], w3[nColumn]);
		m_wndGrid3.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid3.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid3.EnableVirtualMode(Grid3Callback, (LPARAM)this);
	m_wndGrid3.SetCallBack_Clk(OnOrdersClick);


	//派工日期 默认为明天
	m_isToday = FALSE;
	m_canChangeOrder = TRUE;
	GetDlgItem(IDC_ORDER_CHANGE)->EnableWindow(FALSE);
	m_tToday = CTime::GetCurrentTime();
	m_tTomorrow = m_tToday + CTimeSpan(1, 0, 0, 0);
	m_wndDate.GetClientRect(&m_rectDate);
	m_wndDate.MapWindowPoints(this, &m_rectDate);

	//派工单初始化
	m_wndPrint.m_sheetInfo.strData = m_tToday.Format("%Y年%m月%d日制");
	m_order.push_back(-1); //车辆编号索引
	m_order.push_back(-1); //教练信息索引
}

void CViewBooking2::Refresh()
{
	CString strMsg("");
	CString strSQL("");

	//查询待预约学生信息
	CString strDate("");
	strDate = m_isToday ? m_tToday.Format("%Y/%m/%d") : m_tTomorrow.Format("%Y/%m/%d"); //AND FLAG='0'
	strSQL.Format("SELECT students.SNAME, students.CAR_TYPE, bookings.CLASS_ID, students.CLASS_NUM, students.CLASS_TYPE, bookings.FLAG, students.FILE_NAME\
				   FROM bookings inner join students on bookings.FILE_NAME = students.FILE_NAME \
				   WHERE BOOK_DATE='%s' ORDER BY bookings.CLASS_ID, students.CLASS_NUM", strDate);
	m_datas1.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg))
	{
		ShowMsg2Output1("查询学生信息成功");
	}
	else ShowMsg2Output1(strMsg);

	m_wndGrid1.GridRefresh(m_datas1.size());

	//学员分类
	GetClassIndex();


	//查询待可预约教练员信息
	int year = m_tToday.GetYear();
	CTime midYear(year, 6, 1, 0, 0, 0); //以每年6月1号作为半年检查的标志
	//半年内 超过3次请假为不合格, 超过半年按6次算
	int th = 3;
	if (midYear < m_tToday) th = 6;
	strSQL.Format("select coachinfo.SName, coachinfo.GENDER, coachinfo.FILE_NUM, coachstat.PERFORMANCE from \
				  coachstat INNER JOIN coachinfo ON coachinfo.FILE_NUM=coachstat.FILE_NUM \
				  WHERE coachstat.LEAVE_N<'%d' ORDER BY coachstat.PERFORMANCE DESC", th);
	m_datas2.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas2, strMsg))
	{
		ShowMsg2Output1("查询教练员信息成功");
		int n = m_datas2.size();
		for (int i = 0; i < n; i++)
		{
			m_datas2[i].push_back("0"); //最后一列为已安排课时数，每个教练早上下午晚上各有一次机会
		}
	}
	else ShowMsg2Output1(strMsg);

	m_wndGrid2.GridRefresh(m_datas2.size());

	//查询可用车辆信息
	strSQL.Format("select CAR_ID from carinfo WHERE STATE<'5'");
	m_datas3.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas3, strMsg))
	{
		ShowMsg2Output1("查询可用车辆信息成功");
	}
	else ShowMsg2Output1(strMsg);

	int nCount = m_datas3.size();
	m_Combo_Cars.ResetContent();
	for (int i = 0; i < nCount; i++)
	{
		m_Combo_Cars.AddString(m_datas3[i][0]);
	}
}

void CViewBooking2::OnBnClickedSelDay()
{
	if (m_orderIndexes.size() > 0) //派工单工作区有数据未保存
	{
		MessageBox("当前派工尚未保存，请保存后再选择");
		return;
	}


	m_isToday = !m_isToday;
	CString strDate;

	if (m_isToday)
	{
		strDate.Format("后一天 >>");
	}
	else
	{
		strDate.Format("<< 前一天");
	}
	GetDlgItem(IDC_SEL_DAY)->SetWindowTextA(strDate);

	InvalidateRect(m_rectDate);
	OnBnClickedResetPrint();
	Refresh();
}


LRESULT CViewBooking2::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;

	if (flag == 1) //update data from database
	{
		if (m_datas1.size() == 0) //只在刚打开界面时初始化，之后保持工作状态
			Refresh();
	}
	else if (flag == 2) //updata grid1
	{
		m_wndGrid1.GridRefresh(m_datas1.size());
	}
	else if (flag == 3) //updata grid2
	{
		m_wndGrid2.GridRefresh(m_datas2.size());
	}

	return 0;
}

void CViewBooking2::OnBnClickedDoPrint()
{
	int printType = 0; //打印类型

	CString strMsg;
	strMsg.Format("是否打印全部派工？（点击是打印全部，否打印当前选择）");
	switch (MessageBox(strMsg, "打印类型选择", MB_YESNOCANCEL))
	{
	case IDYES:
		printType = 2;
		break;
	case IDNO:
		printType = 1;
		break;
	case IDCANCEL:
	default:
		return;
	}

	xPublic::CMyPrint printx;
	xPublic::CLASSINFO classInfo;
	//xPublic::PRINTERINFO printInfo;
	xPublic::SHEETINFO sheetInfo;

	
	//打印单个
	if (printType == 1)
	{
		printx.PrinterInit(m_wndPrint.m_printData.m_sheetInfo, m_wndPrint.m_printData.m_classInfo);
		printx.m_students = m_wndPrint.m_printData.m_students;
		CString strMsg("");
		printx.Printx(1, strMsg);
		TRACE(strMsg);
		OnBnClickedOrder();
	}
	else if (printType == 2)
	{
		//打印多个
		printx.PrinterInit(&sheetInfo, &classInfo);
		int size = m_orderIndexes.size() - 1;
		for (int nRow = size; nRow >= 0; nRow--)
		{
			Indexes indexes = m_orderIndexes[nRow];

			printx.Reset();

			sheetInfo.strCarID = m_datas3[indexes[0]][0];
			sheetInfo.strCoach = m_datas2[indexes[1]][0];
			sheetInfo.strCoachID = m_datas2[indexes[1]][2];

			int nStudent = indexes.size() - 2;
			for (int i = 0; i < nStudent; i++)
			{
				int stuRow = indexes[i + 2];
				CString name = m_datas1[stuRow][0];
				CString date = m_isToday ? m_tToday.Format("%y/%m/%d") : m_tTomorrow.Format("%y/%m/%d");
				int classStep = atoi(m_datas1[stuRow][3]);
				int classID = atoi(m_datas1[stuRow][2]) - 1;

				//第一个加入的学员决定派工单的授课内容
				if (classInfo.nClassID == 0)
				{
					CString classType = m_datas1[nRow][4];
					CString cn;
					cn.Format("c%d", classStep + 1);
					classInfo.nClassID = xPublic::GETINT2(classType, cn, 0);
				}

				xPublic::STUDENTINFO student(name, date, classID, classStep, g_nMaxBooking);
				printx.AddStudent(student);
			}
			printx.m_printerInfo.nCopy = 1;
			printx.Printx(1, strMsg);
			TRACE(strMsg);
		}
	}

	//
}


//void CViewBooking2::OnStnClickedGrid1()
//{
//	// TODO:  在此添加控件通知处理程序代码
//}

CString WeekDays[9] = { "", "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日" };
void CViewBooking2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码

	m_wndDate.GetClientRect(&m_rectDate);
	m_wndDate.MapWindowPoints(this, &m_rectDate);

	CFont font, font1;
	font.CreateFontA(30, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); 
	font1.CreateFontA(20, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体");

	CFont* old = dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);
	if (m_isToday)
		dc.TextOutA(m_rectDate.left, m_rectDate.bottom - 30, m_tToday.Format("%Y/%m/%d"));
	else
		dc.TextOutA(m_rectDate.left, m_rectDate.bottom - 30, m_tTomorrow.Format("%Y/%m/%d"));

	dc.SelectObject(&font1);
	int index = m_tToday.GetDayOfWeek();
	if (m_isToday)
		dc.TextOutA(m_rectDate.left + 170, m_rectDate.bottom - 20, WeekDays[index]);
	else
		dc.TextOutA(m_rectDate.left + 170, m_rectDate.bottom - 20, WeekDays[index + 1]);

	dc.SelectObject(old);
}


void CViewBooking2::OnBnClickedOrder()
{
	int n = m_order.size();

	if (n < 3) //没有学生
	{
		MessageBox("未选定学员！");
		return;
	}

	for (int i = 0; i < n; i++)
	{
		if (m_order[i] == -1)
		{
			MessageBox("派工单未完成！");

			CString strMsg;
			TRACE("=============\r\n");
			for (int nn = 0; nn < m_order.size(); nn++)
				TRACE("[%d]%d\r\n", nn, m_order[nn]);
			TRACE("=============\r\n");
			return;
		}
	}

	//检测派工单的合法性

	//上传数据库
	int nstu = n - 2;
	for (int i = 0; i < nstu; i++)
	{
		CString strStudent = m_datas1[m_order[2 + i]][6];
		CString strCoach = m_datas2[m_order[1]][2];
		CString strDate = m_isToday ? m_tToday.Format("%Y/%m/%d") : m_tTomorrow.Format("%Y/%m/%d");
		CString strCar = m_datas3[m_order[0]][0];
		CString strClassID = m_datas1[m_order[2 + i]][2];

		CString strMsg, strSQL;
		strSQL.Format("UPDATE bookings SET FLAG='1', ORDER_DATE='%s', ORDER_COACH='%s', ORDER_CAR='%s' \
					  WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
					  m_tToday.Format("%Y/%m/%d"), strCoach, strCar, strStudent, strDate, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}

	m_orderIndexes.push_back(m_order);
	m_wndGrid3.GridRefresh(m_orderIndexes.size());


	RestOrder(1);

	m_wndPrint.m_printData.Reset();
	m_wndPrint.Invalidate();
}

BOOL CViewBooking2::CanBeSelected(int nRow)
{
	if (m_order.size() > 2) //已有选中学生
	{
		//课程内容不同的不能选
		int selected = m_arrClassIndex[m_order[2]];
		if (m_arrClassIndex[nRow] != selected)
		{
			return FALSE;
		}

		//课程时间不同的不能选
		int classID = (atoi(m_datas1[nRow][2]) - 1) / 2;
		int selectedID = (atoi(m_datas1[m_order[2]][2]) - 1) / 2;
		if (classID != selectedID) return FALSE;
	}
	return TRUE;
}

void CViewBooking2::GetClassIndex()
{
	int size = m_datas1.size();
	m_arrClassIndex.clear();
	for (int i = 0; i < size; i++)
	{
		int classStep = atoi(m_datas1[i][3]);
		CString classType = m_datas1[i][4];
		CString cn;
		cn.Format("c%d", classStep + 1);
		int classIndex = xPublic::GETINT2(classType, cn, 0);
		m_arrClassIndex.push_back(classIndex);
	}
}

void CViewBooking2::RestOrder(int type)
{
	m_canChangeOrder = TRUE;
	GetDlgItem(IDC_ORDER_CHANGE)->EnableWindow(FALSE);

	if (type == 0)
	{
		int nstudent = m_order.size();
		if (nstudent > 2) //有学员被选中，需要取消选中状态
		{
			for (int i = 2; i < nstudent; i++)
				m_datas1[m_order[i]][5] = "0";
		}
	}

	m_order.clear();
	m_order.push_back(-1);
	m_order.push_back(-1);

	m_wndGrid1.GridRefresh(m_datas1.size());
}

void CViewBooking2::OnBnClickedResetPrint()
{
	RestOrder(!m_canChangeOrder);

	m_wndPrint.m_printData.Reset();
	m_wndPrint.Invalidate();
}


void CViewBooking2::OnCbnSelchangeCars()
{
	if (!m_canChangeOrder)
	{
		MessageBox("非法操作！\r\n请重置派工单或者点击修改按钮修改此派工单");
		return;
	}

	CString strCarID;
	int pos = m_Combo_Cars.GetCurSel();
	m_Combo_Cars.GetLBText(pos, strCarID);
	m_wndPrint.m_sheetInfo.strCarID = strCarID;

	m_wndPrint.Invalidate();

	m_order[0] = pos;
}


void CViewBooking2::OnBnClickedOrderChange()
{
	CString strMsg;
	strMsg.Format("是否确定要修改第%d条派工？（确定后无法撤销）", m_nChangingOrderIndex+1);
	if (MessageBox(strMsg, "警告",MB_OKCANCEL) != IDOK) return;

	m_canChangeOrder = TRUE;
	GetDlgItem(IDC_ORDER_CHANGE)->EnableWindow(FALSE);

	//更新数据库
	int n = m_order.size();
	int nstu = n - 2;
	for (int i = 0; i < nstu; i++)
	{
		CString strStudent = m_datas1[m_order[2 + i]][6];
		CString strDate = m_isToday ? m_tToday.Format("%Y/%m/%d") : m_tTomorrow.Format("%Y/%m/%d");
		CString strClassID = m_datas1[m_order[2 + i]][2];

		CString strMsg, strSQL;
		strSQL.Format("UPDATE bookings SET FLAG='0', ORDER_DATE='0', ORDER_COACH='0', ORDER_CAR='0' \
					  					  WHERE FILE_NAME='%s' AND ORDER_DATE='%s' AND CLASS_ID='%s'",
										  strStudent, strDate, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}



	if (m_nChangingOrderIndex < m_orderIndexes.size())
	{
		Indextable::iterator it = m_orderIndexes.begin() + m_nChangingOrderIndex;
		m_orderIndexes.erase(it);
		m_wndGrid3.GridRefresh(m_orderIndexes.size());
	}
}


void CViewBooking2::OnBnClickedOrderQuery()
{
	COrders dlg;
	dlg.m_strToday = m_tToday.Format("%Y/%m/%d");
	dlg.DoModal();

	Refresh();
}


void CViewBooking2::OnBnClickedAutoOrder()
{
	
}
