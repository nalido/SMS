// Coaches.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "Coaches.h"
#include "AddCoach.h"
#include "DlgCoachCheck.h"
#include "DlgMontPlate.h"


// CCoaches

IMPLEMENT_DYNCREATE(CCoaches, CBCGPFormView)

CCoaches::CCoaches()
	: CBCGPFormView(CCoaches::IDD)
{
	EnableVisualManagerStyle();
}

CCoaches::~CCoaches()
{
}

void CCoaches::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COACHLIST, m_wndCoachesLoc);
}

BEGIN_MESSAGE_MAP(CCoaches, CBCGPFormView)
	ON_BN_CLICKED(IDC_ADDCOACH, &CCoaches::OnBnClickedAddcoach)
	ON_BN_CLICKED(IDC_CHUQIN, &CCoaches::OnBnClickedChuqin)
	ON_BN_CLICKED(IDC_QUEQIN, &CCoaches::OnBnClickedQueqin)
	ON_BN_CLICKED(IDC_WORKTIME, &CCoaches::OnBnClickedWorktime)
	ON_BN_CLICKED(IDC_JIXIAO, &CCoaches::OnBnClickedJixiao)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_KPI, &CCoaches::OnBnClickedKpi)
	ON_BN_CLICKED(IDC_MONTHPLATE, &CCoaches::OnBnClickedMonthplate)
END_MESSAGE_MAP()


// CCoaches 诊断

#ifdef _DEBUG
void CCoaches::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCoaches::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCoaches 消息处理程序
static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CCoaches* pThis = (CCoaches*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_arCoaches.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_arCoaches.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_arCoaches[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
			return TRUE;
		}

		//颜色控制
		int nBlack = atoi(pThis->m_arCoaches[nRow][11]);
		if (nBlack > 0)
		{
			pdi->item.clrBackground = COLOR_NONE;
			pdi->item.clrText = COLOR_TEXTNONE;
		}
	}

	return TRUE;
}

void CCoaches::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	CRect rect;
	m_wndCoachesLoc.GetClientRect(&rect);
	m_wndCoachesLoc.MapWindowPoints(this, &rect);

	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndCoaches.Create(nStyle, rect, this, IDC_GRID_STUPRO);
	m_wndCoaches.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndCoaches.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndCoaches.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndCoaches.SetReadOnly();
	m_wndCoaches.SetWholeRowSel();
	m_wndCoaches.SetSingleSel(); //只能选一个
	m_wndCoaches.EnableRowHeader(TRUE);
	m_wndCoaches.EnableLineNumbers();
	//m_wndCoaches.

	int nColumn = 0;
	int hw = m_wndCoaches.GetRowHeaderWidth();
	LPCTSTR arrColumns[] = { _T("姓名"), _T("性别"), _T("出生日期"), _T("面貌"), _T("手机")
		, _T("家庭住址"), _T("进厂日期"), _T("档案号"), _T("上月KPI"), _T("缺勤数"), _T("本月课时") };
	const int nColumns = sizeof (arrColumns) / sizeof (LPCTSTR);
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndCoaches.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndCoaches.SetColumnAlign(i, HDF_CENTER);
		m_wndCoaches.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndCoaches.EnableVirtualMode(GridCallback, (LPARAM)this);

	Refresh();
}

void CCoaches::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT coachinfo.SNAME, coachinfo.GENDER, coachinfo.BIRTH, coachinfo.PARTY_STAT, \
				  coachinfo.TEL, coachinfo.HOME, coachinfo.SIGN_DATE, coachinfo.FILE_NUM, \
				  coachstat.PERFORMANCE, coachstat.LEAVE_N, coachstat.CLASS_TIME, coachstat.BLACK_NAME FROM coachinfo \
				  INNER JOIN coachstat ON coachinfo.FILE_NUM=coachstat.FILE_NUM \
				  ORDER BY coachstat.PERFORMANCE DESC");
	m_arCoaches.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_arCoaches, strMsg))
	{
		ShowMsg2Output1("查询教练信息成功");

		int n = m_arCoaches.size();
		for (int i = 0; i < n; i++)
		{
			double d = atoi(m_arCoaches[i][8])*1.0 / 100;
			m_arCoaches[i][8].Format("%.2f", d);
		}
	}
	else ShowMsg2Output1(strMsg);

	m_wndCoaches.GridRefresh(m_arCoaches.size());

	//CTime t = CTime::GetCurrentTime();
	//int year = t.GetYear();
	//CTime midYear(year, 6, 1, 0, 0, 0); //以每年6月1号作为半年检查的标志
	////半年内 超过3次请假为不合格, 超过半年按6次算
	//m_nMaxLeave = 3;
	//if (midYear < t) m_nMaxLeave = 6;
}

void CCoaches::OnBnClickedAddcoach()
{
	CAddCoach dlg;
	if (dlg.DoModal() == IDOK)
	{
		Refresh();
	}
}


void CCoaches::OnBnClickedChuqin()
{
	CBCGPGridRow* pRow = m_wndCoaches.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CDlgCoachCheck dlg;
		dlg.m_nCheckType = CHECK_CHUQIN;
		dlg.m_strCoachID = m_arCoaches[nRow][7];
		dlg.m_strCoach = m_arCoaches[nRow][0];
		dlg.DoModal();
	}
}


void CCoaches::OnBnClickedQueqin()
{
	CBCGPGridRow* pRow = m_wndCoaches.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CDlgCoachCheck dlg;
		dlg.m_nCheckType = CHECK_QUEQIN;
		dlg.m_strCoachID = m_arCoaches[nRow][7];
		dlg.m_strCoach = m_arCoaches[nRow][0];
		dlg.DoModal();
	}
}


void CCoaches::OnBnClickedWorktime()
{
	CBCGPGridRow* pRow = m_wndCoaches.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CDlgCoachCheck dlg;
		dlg.m_nCheckType = CHECK_WORKTIME;
		dlg.m_strCoachID = m_arCoaches[nRow][7];
		dlg.m_strCoach = m_arCoaches[nRow][0];
		dlg.DoModal();
	}
}


void CCoaches::OnBnClickedJixiao()
{
	CBCGPGridRow* pRow = m_wndCoaches.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CDlgCoachCheck dlg;
		dlg.m_nCheckType = CHECK_JIXIAO;
		dlg.m_strCoachID = m_arCoaches[nRow][7];
		dlg.m_strCoach = m_arCoaches[nRow][0];
		dlg.DoModal();
	}
}


LRESULT CCoaches::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		Refresh();
		break;
	case 2:
		
		break;
	}

	return 0;
}

void CCoaches::OnBnClickedKpi()
{
	CBCGPGridRow* pRow = m_wndCoaches.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CDlgCoachCheck dlg;
		dlg.m_nCheckType = CHECK_KPI;
		dlg.m_strCoachID = m_arCoaches[nRow][7];
		dlg.m_strCoach = m_arCoaches[nRow][0];
		dlg.DoModal();
	}
}


void CCoaches::OnBnClickedMonthplate()
{
	CBCGPGridRow* pRow = m_wndCoaches.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CDlgMontPlate dlg;
		dlg.m_strCoachID = m_arCoaches[nRow][7];
		dlg.DoModal();
	}
}
