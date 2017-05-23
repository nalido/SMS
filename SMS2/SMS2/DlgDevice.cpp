// DlgDevice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgDevice.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DlgDateItem.h"
#include "DlgState.h"


// CDlgDevice �Ի���

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
	ON_BN_CLICKED(IDC_DELITEM, &CDlgDevice::OnBnClickedDelitem)
	ON_BN_CLICKED(IDC_SETSTAT, &CDlgDevice::OnBnClickedSetstat)
END_MESSAGE_MAP()


void CALLBACK CDlgDevice::OnGridClick(LPVOID lParam)
{
	CDlgDevice* pThis = (CDlgDevice*)lParam;

	if (pThis->m_wndGrid.IsWholeRowSel()) return;

	CBCGPGridItem* pItem = pThis->m_wndGrid.GetCurSelItem();
	if (pItem != NULL)
	{
		int nCol = pItem->GetColumnId();
		if ((pThis->m_nQueryType == QUERY_DEVICES && nCol == 2) || 
			(pThis->m_nQueryType == QUERY_CLAIMS && nCol == 1))
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

// CDlgDevice ��Ϣ�������

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

			if (pThis->m_nQueryType == QUERY_DEVICES && nCol == 7)
			{
				int state = atoi(pThis->m_datas[nRow][nCol]);
				switch (state)
				{
				case 0:
					pdi->item.varValue = "����";
					break;
				case 1:
					pdi->item.varValue = "ά����";
					break;
				case 2:
					pdi->item.varValue = "ͣ��";
					break;
				}
			}
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
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
	m_wndGrid.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndGrid.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	m_wndGrid.SetSingleSel(); //ֻ��ѡһ��
	m_wndGrid.EnableRowHeader(TRUE);
	m_wndGrid.EnableLineNumbers();

	int nColumn = 0;
	int hw = m_wndGrid.GetRowHeaderWidth();
	std::vector<CString> arrColumns;
	switch (m_nQueryType)
	{
	case QUERY_DEVICES:
		arrColumns.push_back("����");
		arrColumns.push_back("�ͺ�");
		arrColumns.push_back("��������");
		arrColumns.push_back("�ƺ�");
		arrColumns.push_back("����");
		arrColumns.push_back("����");
		arrColumns.push_back("�������");
		arrColumns.push_back("����״̬");
		break;
	case QUERY_INSURANCES:
		arrColumns.push_back("����");
		arrColumns.push_back("�ƺ�");
		arrColumns.push_back("���");
		arrColumns.push_back("���ܺ�");
		arrColumns.push_back("���");
		arrColumns.push_back("��Ч��");
		arrColumns.push_back("������˾");
		break;
	case QUERY_CLAIMS:
		arrColumns.push_back("�ƺ�");
		arrColumns.push_back("����");
		arrColumns.push_back("���");
		arrColumns.push_back("������ʧ");
		arrColumns.push_back("������ʧ");
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
	//ע�������б�ص�����
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);
	m_wndGrid.SetCallBack_Clk(OnGridClick);

	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SETSTAT)->ShowWindow(SW_HIDE);
	if (m_nQueryType == QUERY_DEVICES)
		GetDlgItem(IDC_SETSTAT)->ShowWindow(SW_SHOW);

	m_nOldRows = 0;
	Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CDlgDevice::Refresh()
{
	CString strMsg, strSQL;
	switch (m_nQueryType)
	{
	case QUERY_DEVICES:
		strSQL.Format("SELECT * FROM cars ORDER BY BUY_DAY, CAR_ID");
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
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
}


void CDlgDevice::OnBnClickedNewitem()
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

void CDlgDevice::GetNewData()
{
	int nRows = m_datas.size();
	if (m_nOldRows >= nRows) return; //û������

	//���Ƴ�֮ǰ��ʱ�����Ŀ���
	for (int nRow = m_nOldRows; nRow < nRows; nRow++)
	{
		m_datas.pop_back();
	}

	for (int nRow = m_nOldRows; nRow < nRows; nRow++)
	{
		CBCGPGridRow* pRow = m_wndGrid.GetRow(nRow);

		BOOL isValid = FALSE; //һ������������һ���ǿղ���Ч
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

	//������m_nOldRows����Ϊ�������ݵķֽ��
}

void CDlgDevice::OnBnClickedSave()
{
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);

	GetNewData();
	int nRows = m_datas.size();
	if (m_nOldRows >= nRows) return; //û��������

	for (int nRow = m_nOldRows; nRow < nRows; nRow++)
	{
		AddNewRowToDB(m_datas[nRow]);
	}

	Refresh();
}

void CDlgDevice::AddNewRowToDB(CStrs strs)
{
	CString strMsg, strSQL;
	switch (m_nQueryType)
	{
	case QUERY_DEVICES:
		strSQL.Format("INSERT INTO cars (CAR_NAME, TYPE, BUY_DAY, PLATE_NUM, CAR_NUM, PRODUCTION_PLACE, CAR_ID)\
					  VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
					  strs[0], strs[1], strs[2], strs[3], strs[4], strs[5], strs[6]);
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

void CDlgDevice::OnBnClickedDelitem()
{
	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow == NULL) return;

	GetNewData();
	int nRow = pRow->GetRowId();
	if (nRow >= m_nOldRows) //��δ���������
	{
	}
	else //ɾ���ѱ�������ݣ� ��Ҫͬʱɾ�����ݿ�����
	{
		DelRowFromDB(m_datas[nRow]);
		m_nOldRows--; //������������һ
	}

	//for (int i = 0; i < m_nColumns; i++)
	//	pRow->GetItem(i)->SetValue("");
	CDStrs::iterator it = m_datas.begin() + nRow;
	m_datas.erase(it);
	m_wndGrid.GridRefresh(m_datas.size());
}

void CDlgDevice::DelRowFromDB(CStrs strs)
{
	CString strMsg, strSQL;
	switch (m_nQueryType)
	{
	case QUERY_DEVICES:
		strSQL.Format("DELETE FROM cars WHERE CAR_NAME='%s' AND TYPE='%s' \
						AND BUY_DAY='%s' AND PLATE_NUM='%s'",
						strs[0], strs[1], strs[2], strs[3]);
		break;
	case QUERY_INSURANCES:
		strSQL.Format("DELETE FROM insurances WHERE ITEM_NAME='%s' AND PLATE_NUM='%s' AND \
					  ITEM_NUM='%s' AND CHEJIA_NUM='%s' AND FEE='%s' AND DUE_DATE='%s' AND \
					  COMPANY='%s'",
						strs[0], strs[1], strs[2], strs[3], strs[4], strs[5], strs[6]);
		break;
	case QUERY_CLAIMS:
		strSQL.Format("DELETE FROM claims WHERE PLATE_NUM='%s' AND CLAIM_DATE='%s' AND FEE='%s' AND \
					  CAR_LOSS='%s' AND OTHER_LOSS='%s'",
						strs[0], strs[1], strs[2], strs[3], strs[4]);
		break;
	}

	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);
}

void CDlgDevice::OnBnClickedSetstat()
{
	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow == NULL) return;

	int nRow = pRow->GetRowId();
	CDlgState dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strMsg, strSQL;
		strSQL.Format("UPDATE cars SET CAR_STAT='%d' WHERE CAR_ID='%s'", dlg.m_nStat, m_datas[nRow][6]);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		Refresh();
	}

}
