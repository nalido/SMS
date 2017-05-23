// ViewKPI.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewKPI.h"


// CViewKPI

IMPLEMENT_DYNCREATE(CViewKPI, CBCGPFormView)

CViewKPI::CViewKPI()
	: CBCGPFormView(CViewKPI::IDD)
	, m_threadMySQL(this, ThreadMySQLCallback)
{
	EnableVisualManagerStyle();
}

CViewKPI::~CViewKPI()
{
	m_threadMySQL.StopThread();
}

void CViewKPI::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);
}

BEGIN_MESSAGE_MAP(CViewKPI, CBCGPFormView)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_EXPORT, &CViewKPI::OnBnClickedExport)
	ON_BN_CLICKED(IDC_TALK, &CViewKPI::OnBnClickedTalk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_UPDATE, &CViewKPI::OnBnClickedUpdate)
END_MESSAGE_MAP()


// CViewKPI ���

#ifdef _DEBUG
void CViewKPI::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewKPI::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewKPI ��Ϣ�������

static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewKPI* pThis = (CViewKPI*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas[nRow][nCol];

			int satisfied = atoi(pThis->m_datas[nRow][3]);
			if (satisfied < 60)
			{
				CString strCoachID = pThis->m_datas[nRow][0];
				if (pThis->m_arrAdmonish[strCoachID]<2)
					pdi->item.clrBackground = COLOR_LITTLE;
				else
					pdi->item.clrBackground = COLOR_HALF;
			}
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
	}

	return TRUE;
}


void CViewKPI::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


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
	m_arrColumns.push_back("����");
	m_arrColumns.push_back("����");
	m_arrColumns.push_back("ȡ����");
	m_arrColumns.push_back("�����");
	m_arrColumns.push_back("������");
	m_arrColumns.push_back("�ϸ���");
	m_arrColumns.push_back("�豸�����");
	m_arrColumns.push_back("����ѧԱ��");
	m_arrColumns.push_back("����Ŀ����");
	m_arrColumns.push_back("�ܷ�");

	int nColumns = m_arrColumns.size();
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGrid.InsertColumn(i, m_arrColumns[i], nColumnWidth);
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);

	m_tToday = GetServerTime();//CTime::GetCurrentTime();

}

void CViewKPI::Refresh()
{
	//��ȡ�ϸ����·�
	CString strLastMonth = GetLastMonth(m_tToday);

	m_datas.clear();
	CString strSQL, strMsg;
	strSQL.Format("SELECT COACH_ID, COACH, SAMPLE_NUM, SATISFIED_DEGREE, SATISFIED_PERCENT,\
				  	SKILL_PERCENT, DEVICE_GOOD, STUDENT_NEWS, STUDENT_TARGET, SCORE\
					FROM KPIS WHERE KMONTH='%s' ORDER BY SCORE DESC",
					strLastMonth);
	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);

	//��ѯ�Ѿ���ӡ���㵥��״̬
	strSQL.Format("SELECT COACH_ID, ADMONISH_DATE FROM admonishment WHERE ALARM_MONTH='%s'", m_tToday.Format("%Y/%m"));
	CDStrs datas;
	m_arrAdmonish.clear(); 
	if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg) && datas.size() > 0)
	{
		int na = datas.size();
		for (int i = 0; i < na; i++)
			m_arrAdmonish[datas[i][0]] = datas[i][1] == '0' ? 1 : 2;
	}
	ShowMsg2Output1(strMsg);


	if (m_datas.size() == 0) //û���ϸ��µ���Ϣ
	{
		m_threadMySQL.StartThread();
	}
	else 
		m_wndGrid.GridRefresh(m_datas.size());
}


LRESULT CViewKPI::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		Refresh();
		break;
	case 3:
		m_threadMySQL.StopThread();
	case 2:
		m_wndGrid.GridRefresh(m_datas.size());
		break;
	}

	return 0;
}

void CViewKPI::OnBnClickedExport()
{
	ExportExcel(m_arrColumns, m_datas);
}


///��������
void QuikSort(std::vector<pair<int, int>>& v, int left, int right)
{
	if (left < right)
	{
		int key = v[left].second;
		int index = v[left].first;
		//printf("%d\n\n", key);
		int low = left;
		int high = right;
		while (low < high)
		{
			while (low < high && v[high].second <= key)
			{
				high--;
			}
			v[low].first = v[high].first;
			v[low].second = v[high].second;

			while (low < high && v[low].second >= key)
			{
				low++;
			}
			v[high] = v[low];
		}
		v[low].second = key;
		v[low].first = index;

		//CString strLog, strLine("");
		//int size = v.size();
		//for (int i = 0; i < size; i++)
		//{
		//	strLog.Format("%d ", v[i].second);
		//	strLine = strLine + strLog;
		//}
		//LOG("quik.log", strLine, 0);

		if (low > 0)
		{
			QuikSort(v, left, low - 1);
			QuikSort(v, low + 1, right);
		}
	}
}

void CALLBACK CViewKPI::ThreadMySQLCallback(LPVOID pParam, HANDLE hCloseEvent)
{
	CViewKPI* pThis = (CViewKPI*)pParam;
	if (pThis->m_hWnd == NULL) return;

	CString strSQL, strMsg;
	CString thisMonth = pThis->m_tToday.Format("%Y/%m%%");

	CDStrs coaches;
	strSQL.Format("SELECT COACH_ID, COACH FROM kpis WHERE KMONTH LIKE '%s'", thisMonth);
	g_mysqlCon.ExecuteQuery(strSQL, coaches, strMsg);

	//����KPI����
	int nc = coaches.size();
	if (nc == 0) return;
	//��������
	std::vector<pair<int, int>> c;
	for (int nCoach = 0; nCoach < nc; nCoach++)
	{
		pair<int, int> item;
		item.first = nCoach;

		CDStrs datas;
		//ѧԱ���ּ�¼
		strSQL.Format("SELECT BOOK_DATE, SERVICE_SCORE, SELF_SCORE FROM bookings WHERE ORDER_COACH='%s' \
					  	AND BOOK_DATE LIKE '%s' AND FLAG='2' ORDER BY BOOK_DATE",
						coaches[nCoach][0], thisMonth);
		g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg);

		int nStudents = datas.size();
		if (nStudents == 0) //����û��ѧԱ��, ��Ϊ����60��
		{
			for (int i = 0; i < 8; i++)
			{
				coaches[nCoach].push_back("0");
			}
			coaches[nCoach][9] = "60.00";
			item.second = 0; //����������
			c.push_back(item);
			continue;
		}

		int service = 0; //�������� �����
		int self = 0; //�������� �ϸ���
		int good = 0; //������
		for (int i = 0; i < nStudents; i++)
		{
			int d1 = atoi(datas[i][1]);
			int d2 = atoi(datas[i][2]);

			service += d1;
			self += d2;
			if (d1 >= 80) good++;
		}
		service = service * 100 / nStudents;
		self = self * 100 / nStudents;
		good = good * 10000 / nStudents;

		double KPI = (service + self)*1.0 / 200;

		CString strData;
		strData.Format("%d", nStudents);
		coaches[nCoach].push_back(strData);
		double d = service*1.0 / 100;
		strData.Format("%.2f", d);
		coaches[nCoach].push_back(strData);
		d = good*1.0 / 100;
		strData.Format("%.2f", d);
		coaches[nCoach].push_back(strData);
		d = self*1.0 / 100;
		strData.Format("%.2f", d);
		coaches[nCoach].push_back(strData);
		coaches[nCoach].push_back("100"); //�豸�����
		coaches[nCoach].push_back("0"); //����ѧ����
		coaches[nCoach].push_back("0"); //����ѧ��Ŀ����
		strData.Format("%.2f", KPI);
		coaches[nCoach].push_back(strData);

		//�������ݿ�
		int kpi = KPI * 100;
		item.second = kpi;
		strSQL.Format("UPDATE KPIS SET SAMPLE_NUM='%d', SATISFIED_DEGREE='%d', SATISFIED_PERCENT='%d',\
						SKILL_PERCENT='%d', SCORE='%d'\
						WHERE COACH_ID='%s' AND KMONTH='%s'",
						nStudents, service, good, self, kpi, coaches[nCoach][0], thisMonth.Left(7));
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		c.push_back(item);
	}

	//����
	QuikSort(c, 0, nc-1);

	pThis->m_datas.clear();
	for (int i = 0; i < nc; i++)
	{
		pThis->m_datas.push_back(coaches[c[i].first]);
	}
	//pThis->m_datas = coaches;
	try
	{
		pThis->PostMessageA(WM_USER_UPDATE_VIEW, (WPARAM)3);
	}
	catch (...){}
}


void CViewKPI::OnBnClickedTalk()
{
	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();


		int KPI = atoi(m_datas[nRow][9]);
		if (KPI >= 60)
		{
			if (MessageBox("��Ա��KPI�ϸ��Ƿ������ӡ���㵥��", "����", MB_YESNOCANCEL) != IDYES) return;
		}

		CString strCoachID = m_datas[nRow][0];
		CString strCoach = m_datas[nRow][1];
		CTime today = GetServerTime();
		CString strMonth = today.Format("%Y/%m");

		//��ѯ�Ƿ��Ѿ���ӡ��
		CString strMsg, strSQL;
		strSQL.Format("SELECT ADMONISH_NUM FROM admonishment WHERE COACH_ID='%s' AND ALARM_MONTH='%s'", strCoachID, strMonth);
		CDStrs datas;
		if (!g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg)) 
		{
			ShowMsg2Output1(strMsg); 
			return;
		}

		int num = 0;
		CDStrs counts;
		if (datas.size() == 0) //û�д�ӡ�����½���¼
		{
			strSQL.Format("SELECT COUNT(COACH_ID) FROM admonishment");
			if (!g_mysqlCon.ExecuteQuery(strSQL, counts, strMsg))
			{
				ShowMsg2Output1(strMsg);
				return;
			}
			num = atoi(counts[0][0]) + 1;

			strSQL.Format("INSERT INTO admonishment (COACH_ID, COACH_NAME, ALARM_MONTH, ADMONISH_NUM) VALUES('%s', '%s', '%s', '%d')", strCoachID, strCoach, strMonth, num);
			if (!g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1(strMsg);
				return;
			}
		}
		else
		{
			num = atoi(datas[0][0]);
		}
		CString fileNum;
		fileNum.Format("DJ%03d", num);

		//���㵥�ļ���
		strMonth.Replace("/", "-");
		CString strFileName = g_strOutPath + "\\" + fileNum + "_" + strCoach + "_" + strMonth + ".xls";
		::SHCreateDirectory(NULL, CA2W(g_strOutPath + "\\"));

		if (!PathFileExistsA(strFileName)) //������ģ�� ģ�屣��������123456
		{
			if (!CopyFileA("admonishment.xls", strFileName, FALSE))
			{
				MessageBox("δ�ҵ�ģ�壡");
				return;
			}
		}
		else //���ڽ��㵥��ѯ���Ƿ�¼��̸�����
		{
			strMsg = "�Ѿ��������㵥���Ƿ�¼��̸������������ɹ���\r\nѡ���ǡ�¼��̸������������ɹ���\r\nѡ�񡰷񡱴򿪽��㵥�����������ɹ���\r\nѡ��ȡ�������´�ӡ���㵥��";
			int k = MessageBox(strMsg, "������ʾ", MB_YESNOCANCEL);
			if (k == IDYES)
			{
				strSQL.Format("UPDATE admonishment SET ADMONISH_DATE='%s' WHERE COACH_ID='%s' AND ALARM_MONTH='%s'", today.Format("%Y/%m/%d"), strCoachID, today.Format("%Y/%m"));
				g_mysqlCon.ExecuteSQL(strSQL, strMsg);
				ShowMsg2Output1(strMsg);
			}
		}

		MessageBox("���ڴ򿪵ı���б༭������");
		ShellExecuteA(NULL, NULL, strFileName, NULL, NULL, SW_SHOWNORMAL);

		Refresh();
	}
}


void CViewKPI::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������

	CRect rect;
	GetDlgItem(IDC_LABEL1)->GetClientRect(&rect);
	GetDlgItem(IDC_LABEL1)->MapWindowPoints(this, &rect);
	CBrush brush(COLOR_LITTLE);
	dc.FillRect(rect, &brush);

	GetDlgItem(IDC_LABEL2)->GetClientRect(&rect);
	GetDlgItem(IDC_LABEL2)->MapWindowPoints(this, &rect);
	CBrush brush1(COLOR_HALF);
	dc.FillRect(rect, &brush1);
}


void CViewKPI::OnBnClickedUpdate()
{
	Refresh();
}
