// ViewBooking2.cpp : ʵ���ļ�
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


// CViewBooking2 ���

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


// CViewBooking2 ��Ϣ�������

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
			pdi->item.varValue = "�����ڴ����";
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
			pdi->item.varValue = "�����ڴ����";
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


	//��ʼ�����

	//Grid1
	CRect rectGrid;
	m_wndGridLocation1.GetClientRect(&rectGrid);
	m_wndGridLocation1.MapWindowPoints(this, &rectGrid); //תΪ��������
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid1.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid1.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndGrid1.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndGrid1.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid1.SetReadOnly();
	m_wndGrid1.SetWholeRowSel();
	m_wndGrid1.SetSingleSel(); //ֻ��ѡһ��
	m_wndGrid1.EnableRowHeader(TRUE);
	m_wndGrid1.EnableLineNumbers();


	int hw = m_wndGrid1.GetRowHeaderWidth();
	LPCTSTR arrColumns[] = { _T("����"), _T("�Ա�"), _T("����"), _T("��ʱ") };
	int w[4] = { 70, 50, 80, 0 };
	w[3] = m_wndGrid1.GetLastColWidth(w, 4, rectGrid.Width());
	for (int nColumn = 0; nColumn < 4; nColumn++)
	{
		m_wndGrid1.InsertColumn(nColumn, arrColumns[nColumn], w[nColumn]);
		m_wndGrid1.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid1.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid1.EnableVirtualMode(Grid1Callback, (LPARAM)this);
	m_wndGrid1.SetCallBack_Clk(OnGrid1Click);


	//Grid2
	m_wndGridLocation2.GetClientRect(&rectGrid);
	m_wndGridLocation2.MapWindowPoints(this, &rectGrid); //תΪ��������
	m_wndGrid2.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid2.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndGrid2.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndGrid2.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid2.SetReadOnly();
	m_wndGrid2.SetWholeRowSel();
	m_wndGrid2.SetSingleSel(); //ֻ��ѡһ��
	m_wndGrid2.EnableRowHeader(TRUE);
	m_wndGrid2.EnableLineNumbers();


	hw = m_wndGrid2.GetRowHeaderWidth();
	LPCTSTR arrColumns2[] = { _T("����"), _T("�Ա�"), _T("��Ч") };
	int w2[3] = { 70, 50, 0 };
	w2[2] = m_wndGrid2.GetLastColWidth(w2, 3, rectGrid.Width());
	for (int nColumn = 0; nColumn < 3; nColumn++)
	{
		m_wndGrid2.InsertColumn(nColumn, arrColumns2[nColumn], w2[nColumn]);
		m_wndGrid2.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid2.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid2.EnableVirtualMode(Grid2Callback, (LPARAM)this);
	m_wndGrid2.SetCallBack_Clk(OnGrid2Click);


	//Grid3
	m_wndGridLocation3.GetClientRect(&rectGrid);
	m_wndGridLocation3.MapWindowPoints(this, &rectGrid); //תΪ��������
	m_wndGrid3.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid3.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndGrid3.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	//// Set grid tab order (first):
	//m_wndGrid3.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid3.SetReadOnly();
	m_wndGrid3.SetWholeRowSel();
	m_wndGrid3.SetSingleSel(); //ֻ��ѡһ��
	m_wndGrid3.EnableRowHeader(0,0,0);

	//CBCGPGridColors theme;
	//CBCGPVisualManager::GetInstance()->OnSetGridColorTheme(&m_wndGrid3, theme);
	////m_wndGrid3.SendMessage(BCGM_ONSETCONTROLVMMODE, 1);
	//m_wndGrid3.SetColorTheme(theme);

	hw = m_wndGrid3.GetRowHeaderWidth()*2/3;
	m_wndGrid3.InsertColumn(0, "�ɹ���", rectGrid.Width() - hw);
	m_wndGrid3.SetColumnAlign(0, HDF_CENTER);
	m_wndGrid3.SetHeaderAlign(0, HDF_CENTER);

	CStrs strs;
	strs.push_back("000");
	strs.push_back("����Ա����");
	strs.push_back("00");
	strs.push_back("ѧԱ1");
	strs.push_back("ѧԱ2");
	strs.push_back("ѧԱ3");
	m_wndGrid3.AddCaptionRow("�������");
	CBCGPGridRow* pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[0]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("����Ա");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[1]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("�ڽ�����");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[2]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("�ڽ�ʱ��");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[2]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("�ڽ̿�ʱ");
	pRow = m_wndGrid3.CreateMultiLineRow(2);
	pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
	pRow->GetItem(0)->SetValue((LPCTSTR)strs[2]);
	m_wndGrid3.AddRow(pRow, FALSE);

	m_wndGrid3.AddCaptionRow("ͬ��ѧԱ");
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
	//ע�������б�ص�����
	m_wndGrid3.SetCallBack_DblClk(OnGrid2Click);


	//�ɹ����� Ĭ��Ϊ����
	m_isToday = FALSE;
	CString strDate;
	m_tToday = CTime::GetCurrentTime();
	m_tTomorrow = m_tToday + CTimeSpan(1, 0, 0, 0);
	strDate.Format("��ǰ�ɹ����ڣ�%s(���ת�������ɹ�)", m_tTomorrow.Format("%Y/%m/%d"));
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
		ShowMsg2Output1("��ѯѧ����Ϣ�ɹ�");
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
		strDate.Format("��ǰ�ɹ����ڣ�%s(���ת�������ɹ�)", m_tToday.Format("%Y/%m/%d"));
	}
	else
	{
		strDate.Format("��ǰ�ɹ����ڣ�%s(���ת�������ɹ�)", m_tTomorrow.Format("%Y/%m/%d"));
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