// OrderRsp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewOrderRsp.h"
#include "MainFrm.h"
#include "Response.h"


// CViewOrderRsp

IMPLEMENT_DYNCREATE(CViewOrderRsp, CBCGPFormView)

CViewOrderRsp::CViewOrderRsp()
	: CBCGPFormView(CViewOrderRsp::IDD)
{
	EnableVisualManagerStyle();
}

CViewOrderRsp::~CViewOrderRsp()
{
}

void CViewOrderRsp::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);
}

BEGIN_MESSAGE_MAP(CViewOrderRsp, CBCGPFormView)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
END_MESSAGE_MAP()


// CViewOrderRsp ���

#ifdef _DEBUG
void CViewOrderRsp::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewOrderRsp::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewOrderRsp ��Ϣ�������

static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewOrderRsp* pThis = (CViewOrderRsp*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas.begin() + nRow;
		if (!it->empty())
		{
			if (nCol == 1) //�γ�ʱ��
			{
				int nClass = atoi(pThis->m_datas[nRow][nCol]) - 1;
				pdi->item.varValue = GetClassTime(nClass);
			}
			else if (nCol == 4) //ԤԼ������
			{
				if (pThis->m_datas[nRow][nCol] == "1")
					pdi->item.varValue = "���ɹ����ȴ�ѵ��";
				else if (pThis->m_datas[nRow][nCol] == "2")
					pdi->item.varValue = "�����ѵ��";
				else if (pThis->m_datas[nRow][nCol] == "-1")
					pdi->item.varValue = "ѧԱȱ��";
				else if (pThis->m_datas[nRow][nCol] == "-2")
					pdi->item.varValue = "����ȱ��";
			}
			else
				pdi->item.varValue = pThis->m_datas[nRow][nCol];

			//��ɫ����
			if (pThis->m_datas[nRow][4] == "1")
			{
				//pdi->item.clrBackground = COLOR_DOING;
			}
			else 
			{
				pdi->item.clrBackground = COLOR_DONE;
			}
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
	}

	return TRUE;
}

void CALLBACK CViewOrderRsp::OnGridDbClick(LPVOID lParam)
{
	CViewOrderRsp* pThis = (CViewOrderRsp*)lParam;


	CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
	int nRow = pRow->GetRowId();
	if (pThis->m_datas[nRow][4] != "1") return; //�Ѿ�������

	CResponse dlg;
	if (dlg.DoModal() == IDOK)
	{
		int rspType = dlg.m_nRspType;
		CString strService = dlg.m_strServiceScore;
		CString strSelf = dlg.m_strSelfScore;

		//CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
		//int nRow = pRow->GetRowId();

		pThis->m_datas[nRow][7] = strService;
		pThis->m_datas[nRow][8] = strSelf;
		switch (dlg.m_nRspType)
		{
		case 0:
			pThis->m_datas[nRow][4] = "2";
			pThis->UpdateCoach(nRow, 0);
			pThis->UpdateStudent(nRow, 0);
			break;
		case 1:
			pThis->m_datas[nRow][4] = "-2"; 
			pThis->UpdateCoach(nRow, 1);
			pThis->UpdateStudent(nRow, 2);
			break;
		case 2:
			pThis->m_datas[nRow][4] = "-1";
			pThis->UpdateCoach(nRow, 2);
			pThis->UpdateStudent(nRow, 1);
			break;
		}
		pThis->PostMessageA(WM_USER_UPDATE_VIEW, (WPARAM)2);
	}
}

void CViewOrderRsp::OnInitialUpdate()
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
	LPCTSTR arrColumns[] = { _T("ѧ��"), _T("ѵ��ʱ��"), _T("�������"), _T("����"), _T("������")
		, _T("�ɹ�����"), _T("ѵ������"), _T("��������"), _T("��������") };
	const int nColumns = sizeof (arrColumns) / sizeof (LPCTSTR);
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
	m_wndGrid.SetCallBack_DblClk(OnGridDbClick);

	CTime t = GetServerTime();//CTime::GetCurrentTime();
	m_strToday = t.Format("%Y/%m/%d");

	Refresh();
}


void CViewOrderRsp::Refresh()
{
	CString strMsg, strSQL;
	strSQL.Format("SELECT students.SNAME, bookings.CLASS_ID, bookings.ORDER_CAR, coachinfo.SName\
				  	, bookings.FLAG, bookings.ORDER_DATE, bookings.BOOK_DATE, bookings.SERVICE_SCORE,\
					bookings.SELF_SCORE, bookings.FILE_NAME, \
					bookings.ORDER_COACH\
					FROM bookings \
					inner join students ON bookings.FILE_NAME = students.FILE_NAME \
					inner join coachinfo ON bookings.ORDER_COACH = coachinfo.FILE_NUM\
					WHERE BOOK_DATE='%s' \
					ORDER BY bookings.CLASS_ID, bookings.ORDER_CAR",
					m_strToday);
	m_datas.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);


	m_wndGrid.GridRefresh(m_datas.size());
}

void CViewOrderRsp::UpdateCoach(int nRow, int flag)
{
	CString strMsg, strSQL;
	CString strCoachID = m_datas[nRow][10];
	CString strClassID = m_datas[nRow][1];
	if (flag == 0) //���������ڱ����һ����¼�� ��Ч��һ, ��ʱ��2Сʱ
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID, STUDENT)\
					  VALUES('%s', '%s', 1, 2, '%s', 1)", strCoachID, m_strToday, strClassID);
	}
	else if (flag == 1) //������ȱ�ڱ����һ����¼��ȱ�ڼ�����һ�� ��Ч���趨ֵg_nSubForLeave
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID)\
					  VALUES('%s', '%s', 0, 0, '%s')", strCoachID, m_strToday, strClassID);
	}
	else if (flag == 2) //�������ڣ�ѧԱȱ�ڣ����ڱ����һ����¼�� ��Ч����, ��ʱ�漨Ч�ı�
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID)\
					  VALUES('%s', '%s', 1, 2, '%s')", strCoachID, m_strToday, strClassID);
	}

	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);

	if (flag == 1) //����һ�ξ͸��½�����ȱ�ڴ���
	{
		CString strThisYear;
		strThisYear = m_strToday.Left(4) + "%%";
		strSQL.Format("SELECT COUNT(COACH_ID) FROM coachCheck WHERE COACH_ID='%s' AND TYPE='0' AND CHECK_DATE LIKE '%s'", strCoachID, strThisYear);
		CDStrs counts;
		int count;
		g_mysqlCon.ExecuteQuery(strSQL, counts, strMsg);
		ShowMsg2Output1(strMsg);
		count = atoi(counts[0][0]);

		strSQL.Format("UPDATE coachstat SET LEAVE_N='%d' WHERE FILE_NUM='%s'", count, strCoachID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		CTime t = GetServerTime();//CTime::GetCurrentTime();
		int year = t.GetYear();
		CTime midYear(year, 6, 1, 0, 0, 0); //��ÿ��6��1����Ϊ������ı�־
		//������ ����3�����Ϊ���ϸ�, �������갴6����
		int th = 3;
		if (midYear < t) th = 6;
		if (count >= th)
		{
			strSQL.Format("UPDATE coachstat SET BLACK_NAME='1' WHERE FILE_NUM='%s'", count, strCoachID);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
			ShowMsg2Output1(strMsg);
		}
	}
	else if (flag == 0) //�������һ�Σ�����һ�α������Ͽ�ʱ��
	{
		CString strThisMonth = m_strToday.Left(7) + "%%";
		strSQL.Format("UPDATE coachstat SET CLASS_TIME=\
					  	(SELECT COUNT(COACH_ID) FROM coachCheck WHERE COACH_ID='%s' AND STUDENT='1' AND CHECK_DATE LIKE '%s')\
						WHERE FILE_NUM='%s'", strCoachID, strThisMonth, strCoachID);

		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}
}

void CViewOrderRsp::UpdateStudent(int nRow, int flag)
{
	CString strMsg, strSQL;
	CString strStuID = m_datas[nRow][9];
	CString strClassID = m_datas[nRow][1];
	if (flag == 0) //�������ѵ���� bookings��FLAG�ֶθ���Ϊ��2���� ���Ͽ�ʱ����һ
	{
		CString strService = m_datas[nRow][7];
		CString strSelf = m_datas[nRow][8];
		strSQL.Format("UPDATE bookings SET FLAG='2', SERVICE_SCORE='%s', SELF_SCORE='%s'\
						WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
						strService, strSelf,
						strStuID, m_strToday, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);


		strSQL.Format("UPDATE students SET CLASS_NUM = CLASS_NUM + 1 WHERE FILE_NAME='%s'", strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}
	else if (flag == 1) //ѧԱȱ�ڣ�FLAG�ֶθ���Ϊ��-1���� ���Ͽ�ʱ������
	{
		strSQL.Format("UPDATE bookings SET FLAG='-1'\
					  	WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
						strStuID, m_strToday, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}
	else if (flag == 2) //����ȱ�ڣ�FLAG�ֶθ���Ϊ��-1���� ���Ͽ�ʱ������
	{
		strSQL.Format("UPDATE bookings SET FLAG='-2'\
					  	WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
						strStuID, m_strToday, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}

}

LRESULT CViewOrderRsp::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		Refresh();
		break;
	case 2:
		m_wndGrid.GridRefresh(m_datas.size());
		break;
	}

	return 0;
}