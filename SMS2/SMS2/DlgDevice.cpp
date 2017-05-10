// DlgDevice.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgDevice.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgDevice 对话框

IMPLEMENT_DYNAMIC(CDlgDevice, CBCGPDialog)

CDlgDevice::CDlgDevice(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgDevice::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgDevice::~CDlgDevice()
{
}

void CDlgDevice::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation);
}


BEGIN_MESSAGE_MAP(CDlgDevice, CBCGPDialog)
	ON_BN_CLICKED(IDC_UPDATE, &CDlgDevice::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_NEWITEM, &CDlgDevice::OnBnClickedNewitem)
	ON_BN_CLICKED(IDC_SAVE, &CDlgDevice::OnBnClickedSave)
END_MESSAGE_MAP()


// CDlgDevice 消息处理程序

static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgDevice* pThis = (CDlgDevice*)lp;

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


BOOL CDlgDevice::OnInitDialog()
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
	switch (m_nQueryType)
	{
	case QUERY_DEVICES:
		arrColumns.push_back("名称");
		arrColumns.push_back("型号");
		arrColumns.push_back("购车日期");
		arrColumns.push_back("牌号");
		arrColumns.push_back("数量");
		arrColumns.push_back("产地");
		break;
	case QUERY_INSURANCES:
		arrColumns.push_back("名称");
		arrColumns.push_back("牌号");
		arrColumns.push_back("编号");
		arrColumns.push_back("车架号");
		arrColumns.push_back("金额");
		arrColumns.push_back("有效期");
		arrColumns.push_back("备保公司");
		break;
	case QUERY_CLAIMS:
		arrColumns.push_back("牌号");
		arrColumns.push_back("日期");
		arrColumns.push_back("金额");
		arrColumns.push_back("车辆损失");
		arrColumns.push_back("其它损失");
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

	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);

	m_nOldRows = 0;
	Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgDevice::Refresh()
{
	CString strMsg, strSQL;
	switch (m_nQueryType)
	{
	case QUERY_DEVICES:
		strSQL.Format("SELECT * FROM cars ORDER BY BUY_DAY DESC");
		break;
	case QUERY_INSURANCES:
		strSQL.Format("SELECT * FROM insurances ORDER BY PLATE_NUM DESC");
		break;
	case QUERY_CLAIMS:
		strSQL.Format("SELECT * FROM claims ORDER BY CLAIM_DATE DESC");
		break;
	}

	m_datas.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);

	m_nOldRows = m_datas.size();
	m_wndGrid.GridRefresh(m_nOldRows);
}

void CDlgDevice::OnBnClickedUpdate()
{
	Refresh();
}


void CDlgDevice::OnBnClickedNewitem()
{
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


void CDlgDevice::OnBnClickedSave()
{
	int nRows = m_datas.size();
	if (m_nOldRows >= nRows) return;

	for (int nRow = m_nOldRows; nRow < nRows; nRow++)
	{
		CBCGPGridRow* pRow = m_wndGrid.GetRow(nRow);

		BOOL isValid = FALSE; //一行数据至少有一个非空才有效
		for (int i = 0; i < m_nColumns; i++)
		{
			CBCGPGridItem* pItem = pRow->GetItem(i);
			CString d = pItem->GetValue();
			m_datas[nRow][i] = d;
			if (!d.IsEmpty())
			{
				isValid = TRUE;
			}
		}

		if (isValid)
			AddNewRowToDB(m_datas[nRow]);
	}


	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
}

void CDlgDevice::AddNewRowToDB(CStrs strs)
{
	CString strMsg, strSQL;
	switch (m_nQueryType)
	{
	case QUERY_DEVICES:
		strSQL.Format("INSERT INTO cars (CAR_NAME, TYPE, BUY_DAY, PLATE_NUM, CAR_NUM, PRODUCTION_PLACE)\
					  VALUES('%s', '%s', '%s', '%s', '%s', '%s')",
					  strs[0], strs[1], strs[2], strs[3], strs[4], strs[5]);
		break;
	case QUERY_INSURANCES:
		strSQL.Format("INSERT INTO insurances (ITEM_NAME, PLATE_NUM, ITEM_NUM, CHEJIA_NUM, FEE, DUE_DATE, COMPANY)\
					  	VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
						strs[0], strs[1], strs[2], strs[3], strs[4], strs[5], strs[6]);
		break;
	case QUERY_CLAIMS:
		strSQL.Format("INSERT INTO claims (PLATE_NUM, CLAIM_DATE, FEE, CAR_LOSS, OTHER_LOSS)\
					  	VALUES('%s', '%s', '%s', '%s', '%s')",
						strs[0], strs[1], strs[2], strs[3], strs[4]);
		break;
	}

	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);
}