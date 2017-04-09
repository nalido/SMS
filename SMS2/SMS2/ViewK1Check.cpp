// ViewK1Check.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewK1Check.h"
#include "MainFrm.h"


static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK1Check* pThis = (CViewK1Check*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		if (nCol == 0)
		{
			CString str;
			str.Format("%d", nRow);
			pdi->item.varValue = str;

		}
		else
		{
			pdi->item.varValue = pThis->m_datas[nRow][nCol-1];
		}
		//if (pThis->m_datas[nRow][nCol] == "nalido")
		//{
		//	pdi->item.clrText = COLORREF(RGB(255, 0, 0));
		//}
		//if (pThis->m_datas[nRow][nCol] == "snow")
		//{
		//	pdi->item.clrBackground = COLORREF(RGB(0, 110, 0));
		//}
	}

	return TRUE;
}

// CViewK1Check

IMPLEMENT_DYNCREATE(CViewK1Check, CBCGPFormView)

CViewK1Check::CViewK1Check()
	: CBCGPFormView(CViewK1Check::IDD)
{
	EnableVisualManagerStyle();
}

CViewK1Check::~CViewK1Check()
{
}

void CViewK1Check::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_wndGridLocation);
}

BEGIN_MESSAGE_MAP(CViewK1Check, CBCGPFormView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_PASS, &CViewK1Check::OnBnClickedBtnPass)
	ON_BN_CLICKED(IDC_BTN_FRESH, &CViewK1Check::OnBnClickedBtnFresh)
	ON_BN_CLICKED(IDC_BTN_NOPASS, &CViewK1Check::OnBnClickedBtnNopass)
END_MESSAGE_MAP()


// CViewK1Check 诊断

#ifdef _DEBUG
void CViewK1Check::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewK1Check::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewK1Check 消息处理程序

int CViewK1Check::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;



	return 0;
}


void CViewK1Check::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();



	//初始化表格
	CRect rectGrid;
	m_wndGridLocation.GetClientRect(&rectGrid);
	m_wndGridLocation.MapWindowPoints(this, &rectGrid); //转为桌面坐标


	m_wndGrid.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	m_wndGrid.SetCustomColors(-1, -1, -1, -1, -1, RGB(0, 0, 0)); //黑色边框
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();

	m_wndGrid.InsertColumn(0, _T("序号"), 50);
	m_wndGrid.InsertColumn(1, _T("姓名"), 100);
	m_wndGrid.InsertColumn(2, _T("性别"), 50);
	m_wndGrid.InsertColumn(3, _T("手机"), 150);
	m_wndGrid.InsertColumn(4, _T("申领"), 100);
	m_wndGrid.InsertColumn(5, _T("档案号"), rectGrid.Width()-450);
	m_wndGrid.SetHeaderAlign(0, HDF_CENTER);
	m_wndGrid.SetHeaderAlign(1, HDF_CENTER);
	m_wndGrid.SetHeaderAlign(2, HDF_CENTER);
	m_wndGrid.SetHeaderAlign(3, HDF_CENTER);
	m_wndGrid.SetHeaderAlign(4, HDF_CENTER);
	m_wndGrid.SetHeaderAlign(5, HDF_CENTER);
	//注册虚拟列表回调函数
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);

	//查询新生信息
	Refresh();
}
void CViewK1Check::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT SNAME, GENDER, TEL, CAR_TYPE, FILE_NUMBER FROM students WHERE STEP='0'");
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("查询新生信息成功");
		ListFresh();
	}
}

void CViewK1Check::ListFresh()
{
	m_wndGrid.RemoveAll();
	m_wndGrid.SetVirtualRows(m_datas.size());
	m_wndGrid.AdjustLayout();
}

void CViewK1Check::OnBnClickedBtnPass()
{
	int nCount = m_datas.size();
	int nSel = 0;
	CString strMsg;
	for (int i = nCount-1; i >=0; i--)
	{
		if (m_wndGrid.IsRowSelected(i))
		{
			nSel++;
			CString fileNum = m_datas[i][4];
			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='1' WHERE FILE_NUMBER='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("更新新生信息成功");
				RemoveData(i);
			}
		}
	}
	ListFresh();
	strMsg.Format("共选中%d行", nSel);
	ShowMsg2Output1(strMsg);
}

void CViewK1Check::RemoveData(int nRow)
{
	std::vector<CStrs>::iterator it = m_datas.begin() + nRow;
	m_datas.erase(it);
}

void CViewK1Check::OnBnClickedBtnFresh()
{
	Refresh();
}


void CViewK1Check::OnBnClickedBtnNopass()
{
	int nCount = m_datas.size();
	int nSel = 0;
	CString strMsg;
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_wndGrid.IsRowSelected(i))
		{
			nSel++;
			CString fileNum = m_datas[i][4];
			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='1000' WHERE FILE_NUMBER='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("更新新生信息成功");
				RemoveData(i);
			}
		}
	}
	ListFresh();
	strMsg.Format("共选中%d行", nSel);
	ShowMsg2Output1(strMsg);
}
