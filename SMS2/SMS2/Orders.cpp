// Orders.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "Orders.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "xPublic\MyPrint.h"

// COrders �Ի���

IMPLEMENT_DYNAMIC(COrders, CBCGPDialog)

COrders::COrders(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(COrders::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_strToday = "";
}

COrders::~COrders()
{
}

void COrders::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation1);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_Date_Start);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_Date_End);
}


BEGIN_MESSAGE_MAP(COrders, CBCGPDialog)
	ON_BN_CLICKED(IDC_QUERY, &COrders::OnBnClickedQuery)
	ON_BN_CLICKED(IDC_RESET, &COrders::OnBnClickedReset)
END_MESSAGE_MAP()


// COrders ��Ϣ�������

CString GetClassTime(int n) //����ʱ�α�Ż�þ���ʱ��
{
	CString res("");
	switch (n)
	{
	case 0:
		res = "A8:00-10:00";
		break;
	case 1:
		res = "A10:00-12:00";
		break;
	case 2:
		res = "P2:00 - 4:00";
		break;
	case 3:
		res = "P4:00 - 6:00";
		break;
	case 4:
		res = "P6:00 - 8:00";
		break;
	default:
		res = "δ����";
		break;
	}
	return res;
}

static BOOL CALLBACK Grid1Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	COrders* pThis = (COrders*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas1.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas1.begin() + nRow;
		if (!it->empty())
		{
			if (nCol == 1) //�γ�ʱ��
			{
				int nClass = atoi(pThis->m_datas1[nRow][nCol]) - 1;
				pdi->item.varValue = GetClassTime(nClass);
			}
			else if (nCol == 5) //ԤԼ������
			{
				if (pThis->m_datas1[nRow][nCol] == "1")
					pdi->item.varValue = "���ɹ����ȴ�ѵ��";
				else if (pThis->m_datas1[nRow][nCol] == "2")
					pdi->item.varValue = "�����ѵ��";
			}
			else
				pdi->item.varValue = pThis->m_datas1[nRow][nCol];

			//��ɫ����
			CString strOrderDate = pThis->m_datas1[nRow][0];
			if (strOrderDate < pThis->m_strToday)
			{
				if (pThis->m_datas1[nRow][5] < "2") //δ���ѵ��
				{
					pdi->item.clrBackground = COLOR_LITTLE;
					if (nCol == 5) pdi->item.varValue = "���ɹ���ȱϯѵ��";
				}
				else if (pThis->m_datas1[nRow][5] == "2") //�����ѵ��
				{
					pdi->item.clrBackground = COLOR_MANY;
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


BOOL COrders::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	UpdateData(TRUE);


	//grid
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


	LPCTSTR arrColumns[] = { _T("�ɹ�����"), _T("ѵ��ʱ��"), _T("�������"), _T("����"), _T("ѧ��"), _T("������") };
	int hw = m_wndGrid1.GetRowHeaderWidth() * 2 / 3; //��ͷ���
	int w = (rectGrid.Width()-hw) / 6;
	for (int nColumn = 0; nColumn < 6; nColumn++)
	{
		m_wndGrid1.InsertColumn(nColumn, arrColumns[nColumn], w);
		m_wndGrid1.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid1.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid1.EnableVirtualMode(Grid1Callback, (LPARAM)this);



	m_Date_Start.SetFormat("yyyy/MM/dd");
	m_Date_End.SetFormat("yyyy/MM/dd");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void COrders::OnBnClickedQuery()
{
	UpdateData();

	CString dateS, dateE;
	m_Date_Start.GetWindowText(dateS);
	m_Date_End.GetWindowText(dateE);

	CString strMsg, strSQL;//bookings.ORDER_COACH
	strSQL.Format("SELECT bookings.ORDER_DATE, bookings.CLASS_ID, bookings.ORDER_CAR, coachinfo.SName\
				  	, students.SNAME, bookings.FLAG, bookings.FILE_NAME\
					FROM bookings \
					inner join students ON bookings.FILE_NAME = students.FILE_NAME \
					inner join coachinfo ON bookings.ORDER_COACH = coachinfo.FILE_NUM\
					WHERE ORDER_DATE>='%s' AND ORDER_DATE<='%s' \
					ORDER BY bookings.ORDER_DATE, bookings.CLASS_ID, bookings.ORDER_CAR",
					dateS, dateE);
	m_datas1.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg);
	ShowMsg2Output1(strMsg);

	m_wndGrid1.GridRefresh(m_datas1.size());
}


void COrders::OnBnClickedReset()
{
	CBCGPGridRow* pRow = m_wndGrid1.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strStudent = m_datas1[nRow][6];
		CString strClassID = m_datas1[nRow][1];
		CString strOrderDate = m_datas1[nRow][0];

		CString strMsg, strSQL;
		strSQL.Format("UPDATE bookings SET FLAG='0', ORDER_DATE='0', ORDER_COACH='0', ORDER_CAR='0' \
					    WHERE FILE_NAME='%s' AND ORDER_DATE='%s' AND CLASS_ID='%s'",
						strStudent, strOrderDate, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		CDStrs::iterator it = m_datas1.begin() + nRow;
		m_datas1.erase(it);
		m_wndGrid1.GridRefresh(m_datas1.size());
	}
}
