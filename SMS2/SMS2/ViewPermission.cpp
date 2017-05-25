// ViewPermission.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewPermission.h"
#include "MainFrm.h"


// CViewPermission

IMPLEMENT_DYNCREATE(CViewPermission, CBCGPFormView)

CViewPermission::CViewPermission()
	: CBCGPFormView(CViewPermission::IDD)
{
	EnableVisualManagerStyle();
}

CViewPermission::~CViewPermission()
{
}

void CViewPermission::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);
}

BEGIN_MESSAGE_MAP(CViewPermission, CBCGPFormView)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_UPDATE, &CViewPermission::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_DELSTUFF, &CViewPermission::OnBnClickedDelstuff)
	ON_BN_CLICKED(IDC_SELFDOWN, &CViewPermission::OnBnClickedSelfdown)
END_MESSAGE_MAP()


// CViewPermission 诊断

#ifdef _DEBUG
void CViewPermission::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewPermission::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewPermission 消息处理程序


void CViewPermission::OnInitialUpdate()
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
	m_wndGrid.SetWholeRowSel();
	m_wndGrid.SetSingleSel(); //只能选一个
	m_wndGrid.EnableRowHeader(TRUE);
	m_wndGrid.EnableLineNumbers();

	int nColumn = 0;
	int hw = m_wndGrid.GetRowHeaderWidth();
	std::vector<CString> arrColumns;
	arrColumns.push_back("员工");
	arrColumns.push_back("新生管理");
	arrColumns.push_back("学员管理");
	arrColumns.push_back("设备管理");
	arrColumns.push_back("员工管理");
	arrColumns.push_back("信息管理");
	arrColumns.push_back("驾校管理");

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
}

void CViewPermission::Refresh()
{
	CString strMsg, strSQL;
	m_datas.clear();
	strSQL.Format("SELECT UNAME, NEW_STUDENTS, STUDENTS, DEVICES, STUFF, ALL_STUDENTS, SCHOOL, UID FROM stuff WHERE UPERMISSION != '1'");
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg) && m_datas.size() > 0)
	{
		m_wndGrid.RemoveAll();
		int n = m_datas.size();
		for (int i = 0; i < n; i++)
		{
			CBCGPGridRow* pRow = m_wndGrid.CreateRow(m_nColumns);

			pRow->GetItem(0)->AllowEdit(FALSE);
			pRow->GetItem(0)->SetValue(m_datas[i][0].GetBuffer(0));
			m_datas[i][0].ReleaseBuffer();
			for (int c = 1; c < m_nColumns; c++)
			{
				BOOL isZero = m_datas[i][c] == "0";
				pRow->GetItem(i)->AllowEdit(FALSE);
				pRow->ReplaceItem(c, new CBCGPGridCheckItem(!isZero));
			}
			m_wndGrid.AddRow(pRow, FALSE);
		}
		m_wndGrid.AdjustLayout();
	}
	else
		ShowMsg2Output1(strMsg);
}

LRESULT CViewPermission::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;
	if (flag == 1)
		Refresh();

	return 0;
}

void CViewPermission::OnBnClickedUpdate()
{
	int nRows = m_wndGrid.GetRowCount();
	for (int nRow = 0; nRow < nRows; nRow++)
	{
		CBCGPGridRow* pRow = m_wndGrid.GetRow(nRow);
		BOOL isChanged = FALSE;
		for (int nCol = 1; nCol < m_nColumns; nCol++)
		{
			CBCGPGridItem* pItem = pRow->GetItem(nCol);
			CString str = pItem->GetValue();
			CString oldstat = m_datas[nRow][nCol];
			if (str != oldstat)
			{
				isChanged = TRUE;
				m_datas[nRow][nCol] = str;
			}
		}
		if (isChanged)
		{
			CString strMsg, strSQL;
			strSQL.Format("UPDATE stuff SET UPERMISSION='2', NEW_STUDENTS='%s', STUDENTS='%s', DEVICES='%s'\
						  , STUFF='%s', ALL_STUDENTS='%s', SCHOOL='%s' WHERE UID='%s'"
						  , m_datas[nRow][1], m_datas[nRow][2], m_datas[nRow][3], m_datas[nRow][4]
						  , m_datas[nRow][5], m_datas[nRow][6], m_datas[nRow][7]);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
			ShowMsg2Output1(strMsg);
		}
	}
}


void CViewPermission::OnBnClickedDelstuff()
{
	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		if (MessageBox("确定要注销此员工吗？", "警告", MB_OKCANCEL) != IDOK) return;

		int nRow = pRow->GetRowId();
		CString strMsg, strSQL;
		strSQL.Format("DELETE FROM stuff  WHERE UID='%s'", m_datas[nRow][6]);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		{
			m_wndGrid.RemoveRow(nRow, TRUE);
			CDStrs::iterator it = m_datas.begin() + nRow;
			m_datas.erase(it);
		}
		else
		{
			ShowMsg2Output1(strMsg);
			MessageBox("注销失败，请检查网络");
		}

	}
}


void CViewPermission::OnBnClickedSelfdown()
{
	CString strMsg = "放弃管理员身份后将无法管理驾校和员工，请确保有一个管理员身份的用户。新管理员请在注册时用户名后加上“master”字样。确定要放弃您的管理员身份吗？";
	if (MessageBox(strMsg, "警告", MB_OKCANCEL) != IDOK) return;

	CString strSQL;
	strSQL.Format("UPDATE stuff  SET UPERMISSION='0' WHERE UID='%s'", g_strUserID);
	if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->SelectView(VIEW_HOME);
		pFrame->PostMessageA(WM_USER_MESSAGE, (WPARAM)0);
	}
	else
	{
		MessageBox("操作失败，请检查网络");
	}
}
