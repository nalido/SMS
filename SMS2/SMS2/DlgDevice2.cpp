// DlgDevice2.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgDevice2.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DlgDateItem.h"


// CDlgDevice2 对话框

IMPLEMENT_DYNAMIC(CDlgDevice2, CBCGPDialog)

CDlgDevice2::CDlgDevice2(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgDevice2::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgDevice2::~CDlgDevice2()
{
}

void CDlgDevice2::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation);
	DDX_Control(pDX, IDC_GRID2, m_wndGridLocationC);
	DDX_Radio(pDX, IDC_RADIO1, m_nShowType);
}


BEGIN_MESSAGE_MAP(CDlgDevice2, CBCGPDialog)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_UPDATE, &CDlgDevice2::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_NEWITEM, &CDlgDevice2::OnBnClickedNewitem)
	ON_BN_CLICKED(IDC_SAVE, &CDlgDevice2::OnBnClickedSave)
	ON_BN_CLICKED(IDC_DELITEM, &CDlgDevice2::OnBnClickedDelitem)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgDevice2::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgDevice2::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgDevice2::OnBnClickedRadio4)
END_MESSAGE_MAP()


// CDlgDevice2 消息处理程序

static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgDevice2* pThis = (CDlgDevice2*)lp;

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

	CDlgDevice2* pThis = (CDlgDevice2*)lp;

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

	CDlgDevice2* pThis = (CDlgDevice2*)lp;

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

static BOOL CALLBACK GridCCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgDevice2* pThis = (CDlgDevice2*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datasC.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datasC.begin() + nRow;
		if (!it->empty())
		{
			if (nCol==1)
				pdi->item.varValue = pThis->m_datasC[nRow][2];
			else
				pdi->item.varValue = pThis->m_datasC[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
		}
	}

	return TRUE;
}


BOOL CDlgDevice2::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();


	CRect rect;
	m_wndGridLocation.GetClientRect(&rect);
	m_wndGridLocation.MapWindowPoints(this, &rect);

	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;

	//---------------按天显示-------------------------------------------------------
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
	switch (m_nQueryType)
	{
	case QUERY_OIL:
		arrColumns.push_back("记录日期");
		arrColumns.push_back("金额");
		arrColumns.push_back("负责人");
		break;
	case QUERY_MAINTENANCE:
		arrColumns.push_back("维保日期");
		arrColumns.push_back("维保项目");
		arrColumns.push_back("完工日期");
		arrColumns.push_back("金额");
		arrColumns.push_back("负责人");
		break;
	case QUERY_MOT:
		arrColumns.push_back("年检日期");
		arrColumns.push_back("年检结果");
		arrColumns.push_back("负责人");
		break;
	}

	int nColumns = arrColumns.size();
	m_nColumns = nColumns;
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
	m_wndGrid.SetCallBack_Clk(OnGridClick);

	//--------------按月显示---------------------------------
	m_wndGridM.Create(nStyle, rect, this, IDC_GRID_STUPRO);
	m_wndGridM.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGridM.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGridM.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGridM.SetReadOnly();
	m_wndGridM.SetWholeRowSel();
	m_wndGridM.SetSingleSel(); //只能选一个
	m_wndGridM.EnableRowHeader(TRUE);
	m_wndGridM.EnableLineNumbers();

	nColumn = 0;
	hw = m_wndGridM.GetRowHeaderWidth();
	arrColumns.clear();
	switch (m_nQueryType)
	{
	case QUERY_OIL:
		arrColumns.push_back("月份");
		arrColumns.push_back("记录次数");
		arrColumns.push_back("汇总金额");
		break;
	case QUERY_MAINTENANCE:
		arrColumns.push_back("月份");
		arrColumns.push_back("维保次数");
		arrColumns.push_back("汇总金额");
		break;
	case QUERY_MOT:
		arrColumns.push_back("年检日期");
		arrColumns.push_back("年检结果");
		arrColumns.push_back("负责人");
		break;
	}

	nColumns = arrColumns.size();
	w = rect.Width() - hw;
	nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGridM.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndGridM.SetColumnAlign(i, HDF_CENTER);
		m_wndGridM.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGridM.EnableVirtualMode(GridMCallback, (LPARAM)this);
	m_wndGridM.EnableWindow(FALSE);

	//---------------按年显示-------------------------------
	m_wndGridY.Create(nStyle, rect, this, IDC_GRID_STUPRO);
	m_wndGridY.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGridY.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGridY.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGridY.SetReadOnly();
	m_wndGridY.SetWholeRowSel();
	m_wndGridY.SetSingleSel(); //只能选一个
	m_wndGridY.EnableRowHeader(TRUE);
	m_wndGridY.EnableLineNumbers();

	nColumn = 0;
	hw = m_wndGridY.GetRowHeaderWidth();
	arrColumns.clear();
	switch (m_nQueryType)
	{
	case QUERY_OIL:
		arrColumns.push_back("年份");
		arrColumns.push_back("记录次数");
		arrColumns.push_back("汇总金额");
		break;
	case QUERY_MAINTENANCE:
		arrColumns.push_back("年份");
		arrColumns.push_back("维保次数");
		arrColumns.push_back("汇总金额");
		break;
	case QUERY_MOT:
		arrColumns.push_back("年检日期");
		arrColumns.push_back("年检结果");
		arrColumns.push_back("负责人");
		break;
	}

	nColumns = arrColumns.size();
	w = rect.Width() - hw;
	nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGridY.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndGridY.SetColumnAlign(i, HDF_CENTER);
		m_wndGridY.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGridY.EnableVirtualMode(GridYCallback, (LPARAM)this);
	m_wndGridY.EnableWindow(FALSE);

	//---------------车辆信息-------------------------------
	CRect rectC;
	m_wndGridLocationC.GetClientRect(&rectC);
	m_wndGridLocationC.MapWindowPoints(this, &rectC);
	m_wndGridC.Create(nStyle, rectC, this, IDC_GRID_STUPRO);
	m_wndGridC.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGridC.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGridC.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGridC.SetReadOnly();
	m_wndGridC.SetWholeRowSel();
	m_wndGridC.SetSingleSel(); //只能选一个
	m_wndGridC.EnableRowHeader(TRUE);
	m_wndGridC.EnableLineNumbers();

	nColumn = 0;
	hw = m_wndGridC.GetRowHeaderWidth();
	arrColumns.clear();
	arrColumns.push_back("车辆编号");
	arrColumns.push_back("车辆牌照");

	nColumns = arrColumns.size();
	w = rectC.Width() - hw;
	nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGridC.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndGridC.SetColumnAlign(i, HDF_CENTER);
		m_wndGridC.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGridC.EnableVirtualMode(GridCCallback, (LPARAM)this);
	m_wndGridC.SetCallBack_Clk(OnGridCClick);

	m_nShowType = 0;
	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_NEWITEM)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELITEM)->EnableWindow(FALSE);

	m_nOldRows = 0;
	UpdateData(FALSE);

	RefreshCar();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CALLBACK CDlgDevice2::OnGridClick(LPVOID lParam)
{
	CDlgDevice2* pThis = (CDlgDevice2*)lParam;

	if (pThis->m_wndGrid.IsWholeRowSel()) return;

	CBCGPGridItem* pItem = pThis->m_wndGrid.GetCurSelItem();
	if (pItem != NULL)
	{
		int nCol = pItem->GetColumnId();
		if (nCol == 0)
		{
			CDlgDateItem dlg;
			if (dlg.DoModal() == IDOK)
			{
				pItem->SetValue(dlg.m_strDate.GetBuffer(0));
				dlg.m_strDate.ReleaseBuffer();
			}
		}
	}
}


void CALLBACK CDlgDevice2::OnGridCClick(LPVOID lParam)
{
	CDlgDevice2* pThis = (CDlgDevice2*)lParam;

	CBCGPGridRow* pRow = pThis->m_wndGridC.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		pThis->m_strCarID = pThis->m_datasC[nRow][0];
		pThis->m_strPlateNum = pThis->m_datasC[nRow][2];
		pThis->PostMessageA(WM_USER_UPDATE_VIEW, (WPARAM)1);
	}
}

LRESULT CDlgDevice2::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;
	if (flag == 1) //选择了新的车辆
	{
		Refresh();
		GetDlgItem(IDC_NEWITEM)->EnableWindow(TRUE);
		GetDlgItem(IDC_DELITEM)->EnableWindow(TRUE);

		m_wndGrid.EnableWindow(TRUE);
		m_wndGridM.EnableWindow(FALSE);
		m_wndGridY.EnableWindow(FALSE);
	}

	return 0;
}

void CDlgDevice2::RefreshCar()
{
	CString strMsg, strSQL;

	strSQL.Format("SELECT * FROM carinfo");
	m_datasC.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datasC, strMsg);
	ShowMsg2Output1(strMsg);
	m_wndGridC.GridRefresh(m_datasC.size());
}

void CDlgDevice2::Refresh()
{
	CString strMsg, strSQL;

	switch (m_nQueryType)
	{
	case QUERY_OIL:
		strSQL.Format("SELECT * FROM oils WHERE CAR_ID='%s' ORDER BY OIL_DATE DESC", m_strCarID);
		break;
	case QUERY_MAINTENANCE:
		strSQL.Format("SELECT * FROM maintenances WHERE CAR_ID='%s' ORDER BY MDATE DESC", m_strCarID);
		break;
	case QUERY_MOT:
		strSQL.Format("SELECT * FROM MOTs WHERE CAR_ID='%s' ORDER BY MOT_DATE DESC", m_strCarID);
		break;
	}

	m_datas.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);

	m_nOldRows = m_datas.size();
	m_wndGrid.GridRefresh(m_nOldRows);

	CountData();
}

void CDlgDevice2::CountData()
{
	m_datasM.clear();
	m_datasY.clear();

	if (m_nQueryType == QUERY_MOT)
	{
		m_datasM = m_datas;
		m_datasY = m_datas;
		return;
	}

	int n = m_datas.size();
	if (n == 0) return; 

	//按月份统计
	int sum_m = 0; 
	int money_m = 0;
	CString strThisMonth = m_datas[0][0].Left(7);
	for (int i = 0; i < n; i++)
	{
		if (m_datas[i][0].Left(7) == strThisMonth)
		{
			sum_m++;
			money_m += atoi(m_datas[i][3]);
		}
		else
		{
			CStrs strs;
			strs.push_back(strThisMonth);
			CString strSum;
			strSum.Format("%d", sum_m);
			strs.push_back(strSum);
			strSum.Format("%d", money_m);
			strs.push_back(strSum);
			m_datasM.push_back(strs);

			sum_m = 1;
			strThisMonth = m_datas[i][0].Left(7);
		}

	}
	CStrs strs;
	strs.push_back(strThisMonth);
	CString strSum;
	strSum.Format("%d", sum_m);
	strs.push_back(strSum);
	strSum.Format("%d", money_m);
	strs.push_back(strSum);
	m_datasM.push_back(strs);


	//按年份统计
	int m = m_datasM.size();
	if (m == 0) return;
	int sum_y = 0;
	int money_y = 0;
	CString strThisYear = m_datasM[0][0].Left(4);
	for (int i = 0; i < m; i++)
	{
		if (m_datas[i][0].Left(4) == strThisYear)
		{
			sum_y += atoi(m_datasM[i][1]);
			money_y += atoi(m_datasM[i][2]);
		}
		else
		{
			CStrs strsy;
			strsy.push_back(strThisYear);
			CString strSumy;
			strSumy.Format("%d", sum_y);
			strsy.push_back(strSumy);
			strSumy.Format("%d", money_y);
			strsy.push_back(strSumy);
			m_datasY.push_back(strsy);

			sum_y = 1;
			strThisYear = m_datasM[i][0].Left(4);
		}
	}


	CStrs strsy;
	strsy.push_back(strThisYear);
	CString strSumy;
	strSumy.Format("%d", sum_y);
	strsy.push_back(strSumy);
	strSumy.Format("%d", money_y);
	strsy.push_back(strSumy);
	m_datasY.push_back(strsy);
}

void CDlgDevice2::OnBnClickedUpdate()
{
	Refresh();
}


void CDlgDevice2::OnBnClickedNewitem()
{
	GetNewData();
	GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);
	m_wndGrid.SetReadOnly(0);
	CStrs strs;
	for (int i = 0; i < m_nColumns; i++)
	{
		strs.push_back("");
	}
	m_datas.push_back(strs);

	m_wndGrid.GridRefresh(m_datas.size());
}

void CDlgDevice2::GetNewData()
{
	int nRows = m_datas.size();
	if (m_nOldRows >= nRows) return; //没有新行

	//先推出之前临时创建的空行
	for (int nRow = m_nOldRows; nRow < nRows; nRow++)
	{
		m_datas.pop_back();
	}

	for (int nRow = m_nOldRows; nRow < nRows; nRow++)
	{
		CBCGPGridRow* pRow = m_wndGrid.GetRow(nRow);

		BOOL isValid = FALSE; //一行数据至少有一个非空才有效
		CStrs strs;
		for (int i = 0; i < m_nColumns; i++)
		{
			CBCGPGridItem* pItem = pRow->GetItem(i);
			CString d = pItem->GetValue();
			strs.push_back(d);
			if (!d.IsEmpty())
			{
				isValid = TRUE;
			}
		}

		if (isValid)
			m_datas.push_back(strs);
	}

	//不更新m_nOldRows，作为新老数据的分界点
}

void CDlgDevice2::OnBnClickedSave()
{
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);

	GetNewData();
	int nRows = m_datas.size();
	if (m_nOldRows >= nRows) return; //没有新数据

	for (int nRow = m_nOldRows; nRow < nRows; nRow++)
	{
		AddNewRowToDB(m_datas[nRow]);
	}

	Refresh();
}

void CDlgDevice2::AddNewRowToDB(CStrs strs)
{
	CString strMsg, strSQL;
	switch (m_nQueryType)
	{
	case QUERY_OIL:
		strSQL.Format("INSERT INTO oils (OIL_DATE, MONEY, RNAME, CAR_ID, PLATE_NUM)\
						VALUES('%s', '%s', '%s', '%s', '%s')",
						strs[0], strs[1], strs[2], m_strCarID, m_strPlateNum);
		break;
	case QUERY_MAINTENANCE:
		strSQL.Format("INSERT INTO maintenances (MDATE, MITEM, COMPLETE_DATE, FEE, RNAME, CAR_ID, PLATE_NUM)\
						VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
						strs[0], strs[1], strs[2], strs[3], strs[4], m_strCarID, m_strPlateNum);
		break;
	case QUERY_MOT:
		strSQL.Format("INSERT INTO mots (MOT_DATE, MOT_RESULT, RNAME, CAR_ID, PLATE_NUM)\
						VALUES('%s', '%s', '%s', '%s', '%s')",
						strs[0], strs[1], strs[2], m_strCarID, m_strPlateNum);
		break;
	}

	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);
}

void CDlgDevice2::OnBnClickedDelitem()
{
	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow == NULL) return;

	GetNewData();
	int nRow = pRow->GetRowId();
	if (nRow >= m_nOldRows) //有未保存的数据
	{
	}
	else //删除已保存的数据， 需要同时删除数据库内容
	{
		DelRowFromDB(m_datas[nRow]);
		m_nOldRows--; //老数据行数减一
	}

	//for (int i = 0; i < m_nColumns; i++)
	//	pRow->GetItem(i)->SetValue("");
	CDStrs::iterator it = m_datas.begin() + nRow;
	m_datas.erase(it);
	m_wndGrid.GridRefresh(m_datas.size());
}

void CDlgDevice2::DelRowFromDB(CStrs strs)
{
	CString strMsg, strSQL;
	switch (m_nQueryType)
	{
	case QUERY_OIL:
		strSQL.Format("DELETE FROM oils WHERE OIL_DATE='%s' AND MONEY='%s' AND RNAME='%s' AND\
						CAR_ID='%s' AND PLATE_NUM='%s'",
						strs[0], strs[1], strs[2], strs[3], strs[4]);
		break;
	case QUERY_MAINTENANCE:
		strSQL.Format("DELETE FROM maintenances WHERE MDATE='%s' AND MITEM='%s' AND\
						COMPLETE_DATE='%s' AND FEE='%s' AND RNAME='%s' AND CAR_ID='%s' AND PLATE_NUM='%s'",
						strs[0], strs[1], strs[2], strs[3], strs[4], strs[5], strs[6]);
		break;
	case QUERY_MOT:
		strSQL.Format("DELETE FROM mots WHERE MOT_DATE='%s' AND MOT_RESULT='%s' AND RNAME='%s' AND \
						CAR_ID='%s' AND PLATE_NUM='%s'",
						strs[0], strs[1], strs[2], strs[3], strs[4]);
		break;
	}

	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);
}

void CDlgDevice2::OnBnClickedRadio1()
{
	m_wndGrid.EnableWindow(TRUE);
	m_wndGridM.EnableWindow(FALSE);
	m_wndGridY.EnableWindow(FALSE);
	m_wndGrid.GridRefresh(m_datas.size());

	GetDlgItem(IDC_DELITEM)->EnableWindow(TRUE);
	GetDlgItem(IDC_NEWITEM)->EnableWindow(TRUE);
}


void CDlgDevice2::OnBnClickedRadio2()
{
	m_wndGrid.EnableWindow(FALSE);
	m_wndGridM.EnableWindow(TRUE);
	m_wndGridY.EnableWindow(FALSE);
	m_wndGridM.GridRefresh(m_datasM.size());

	GetDlgItem(IDC_DELITEM)->EnableWindow(FALSE);
	GetDlgItem(IDC_NEWITEM)->EnableWindow(FALSE);
}


void CDlgDevice2::OnBnClickedRadio4()
{
	m_wndGrid.EnableWindow(FALSE);
	m_wndGridM.EnableWindow(FALSE);
	m_wndGridY.EnableWindow(TRUE);
	m_wndGridY.GridRefresh(m_datasY.size());

	GetDlgItem(IDC_DELITEM)->EnableWindow(FALSE);
	GetDlgItem(IDC_NEWITEM)->EnableWindow(FALSE);
}
