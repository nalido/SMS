// Coaches.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "Coaches.h"
#include "AddCoach.h"


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
END_MESSAGE_MAP()


// CCoaches ���

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


// CCoaches ��Ϣ�������
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
			pdi->item.varValue = "�����ڴ����";
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
	m_wndCoaches.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndCoaches.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndCoaches.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndCoaches.SetReadOnly();
	m_wndCoaches.SetWholeRowSel();
	m_wndCoaches.SetSingleSel(); //ֻ��ѡһ��
	m_wndCoaches.EnableRowHeader(TRUE);
	m_wndCoaches.EnableLineNumbers();
	//m_wndCoaches.

	int nColumn = 0;
	int hw = m_wndCoaches.GetRowHeaderWidth();
	LPCTSTR arrColumns[] = { _T("����"), _T("�Ա�"), _T("��������"), _T("��ò"), _T("�ֻ�")
		, _T("��ͥסַ"), _T("��������"), _T("������") };
	const int nColumns = sizeof (arrColumns) / sizeof (LPCTSTR);
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndCoaches.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndCoaches.SetColumnAlign(i, HDF_CENTER);
		m_wndCoaches.SetHeaderAlign(i, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndCoaches.EnableVirtualMode(GridCallback, (LPARAM)this);

	Refresh();
}

void CCoaches::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT * FROM coachinfo");
	m_arCoaches.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_arCoaches, strMsg))
	{
		ShowMsg2Output1("��ѯ������Ϣ�ɹ�");
	}
	else ShowMsg2Output1(strMsg);

	m_wndCoaches.GridRefresh(m_arCoaches.size());
}

void CCoaches::OnBnClickedAddcoach()
{
	CAddCoach dlg;
	if (dlg.DoModal() == IDOK)
	{
		Refresh();
	}
}
