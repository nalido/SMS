// DlgMyBooks.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "DlgMyBooks.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Response.h"
#include "DlgFee.h"


// CDlgMyBooks �Ի���

IMPLEMENT_DYNAMIC(CDlgMyBooks, CBCGPDialog)

CDlgMyBooks::CDlgMyBooks(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgMyBooks::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_isPublic = TRUE;
	m_nDlgType = DLG_RESP;
}

CDlgMyBooks::~CDlgMyBooks()
{
}

void CDlgMyBooks::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);

	DDX_Text(pDX, IDC_S1, m_strMsg);
}


BEGIN_MESSAGE_MAP(CDlgMyBooks, CBCGPDialog)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_MONEY, &CDlgMyBooks::OnBnClickedMoney)
END_MESSAGE_MAP()


// CDlgMyBooks ��Ϣ�������


static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CDlgMyBooks* pThis = (CDlgMyBooks*)lp;

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
				else if (pThis->m_datas[nRow][nCol] == "0")
					pdi->item.varValue = "δ�ɹ�";
			}
			else if (nCol == 7)
			{
				if (pThis->m_datas[nRow][9] == "0")
					pdi->item.varValue = "����";
				else if (pThis->m_datas[nRow][9] == "1")
					pdi->item.varValue = "����������ʱ";
				else if (pThis->m_datas[nRow][9] == "2")
					pdi->item.varValue = "��ʱ���ӿ�ʱ";
			}
			else if (nCol == 9)
			{
				if (pThis->m_datas[nRow][9] != "0")
				{
					if (pThis->m_datas[nRow][11] == "0")
						pdi->item.varValue = "δ�ɷ�";
					else if (pThis->m_datas[nRow][11] == "-1")
						pdi->item.varValue = "�ѽ���";
					else
						pdi->item.varValue = "�ѽ�" + pThis->m_datas[nRow][11];
				}
			}
			else if (nCol == 8)
			{
				pdi->item.varValue = pThis->m_datas[nRow][10];
			}
			else
				pdi->item.varValue = pThis->m_datas[nRow][nCol];

			//��ɫ����
			if (pThis->m_nDlgType == DLG_MONEY)
			{
				if (pThis->m_datas[nRow][11] != "-1" && pThis->m_datas[nRow][9] != "0")
				{
					pdi->item.clrBackground = COLOR_LITTLE;
				}
			}


			if (pThis->m_nDlgType == DLG_RESP)
			{
				if (pThis->m_datas[nRow][4] == "1" || pThis->m_datas[nRow][4] == "0")
				{
					//pdi->item.clrBackground = COLOR_DOING;
				}
				else
				{
					pdi->item.clrBackground = COLOR_DONE;
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


void CALLBACK CDlgMyBooks::OnGridDbClick(LPVOID lParam)
{
	CDlgMyBooks* pThis = (CDlgMyBooks*)lParam;


	CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
	int nRow = pRow->GetRowId();
	if (pThis->m_datas[nRow][4] != "1") return; //�Ѿ�������

	//ֻ�ܷ�����������
	CTime t = GetServerTime();
	CString yestoday = t.Format("%Y/%m/%d");
	if (pThis->m_datas[nRow][0] != pThis->m_strToday
		&& pThis->m_datas[nRow][0] != yestoday) return; //δ����ѵ��ԤԼ ���ܷ���

	CResponse dlg;
	if (dlg.DoModal() == IDOK)
	{
		int rspType = dlg.m_nRspType;
		CString strService = dlg.m_strServiceScore;
		CString strSelf = dlg.m_strSelfScore;

		//CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
		//int nRow = pRow->GetRowId();

		pThis->m_datas[nRow][5] = strService;
		pThis->m_datas[nRow][6] = strSelf;
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

BOOL CDlgMyBooks::OnInitDialog()
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

	if (m_nDlgType==DLG_MONEY)
		m_wndGrid.SetSingleSel(0);

	int nColumn = 0;
	int hw = m_wndGrid.GetRowHeaderWidth();
	std::vector<CString> arrColumns;
	arrColumns.push_back("ѵ������");
	arrColumns.push_back("ѵ��ʱ��");
	arrColumns.push_back("�������");
	arrColumns.push_back("����");
	arrColumns.push_back("������");
	arrColumns.push_back("��������");
	arrColumns.push_back("��������");

	if (m_nDlgType == DLG_MONEY)
	{
		arrColumns.push_back("ԤԼ����"); //nCol = 7
		arrColumns.push_back("����");
		arrColumns.push_back("�ɷ����");
	}

	const int nColumns = arrColumns.size();
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

	if (!m_isPublic && m_nDlgType==DLG_MONEY)
	{
		GetDlgItem(IDC_MONEY)->ShowWindow(SW_SHOW);
	}
	else
		GetDlgItem(IDC_MONEY)->ShowWindow(SW_HIDE);

	CTime t = GetServerTime();//CTime::GetCurrentTime();
	m_strToday = t.Format("%Y/%m/%d");

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

LRESULT CDlgMyBooks::OnUserUpdate(WPARAM wParam, LPARAM lParam)
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

void CDlgMyBooks::Refresh()
{
	CString strMsg, strSQL;
	strSQL.Format("SELECT bookings.BOOK_DATE, bookings.CLASS_ID, bookings.ORDER_CAR, coachinfo.SName\
				  	, bookings.FLAG, bookings.SERVICE_SCORE, bookings.SELF_SCORE, bookings.FILE_NAME, \
					bookings.ORDER_COACH, bookings.BOOK_TYPE, bookings.FEE, bookings.FEE_DONE FROM bookings \
					inner join students ON bookings.FILE_NAME = students.FILE_NAME \
					left join coachinfo ON bookings.ORDER_COACH = coachinfo.FILE_NUM\
					WHERE bookings.FILE_NAME='%s' \
					ORDER BY bookings.BOOK_DATE, bookings.CLASS_ID, bookings.ORDER_CAR",
					m_strStuID);
	m_datas.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg);
	ShowMsg2Output1(strMsg);

	if (m_nDlgType == DLG_MONEY)
	{
		int n = m_datas.size();
		int sumN = 0; //��ɷ�����
		int sumM0 = 0; //��ɷ��ܽ��
		int sumM = 0; //δ�ɷѽ��
		for (int i = 0; i < n; i++)
		{
			int type = atoi(m_datas[i][9]);
			int fee = atoi(m_datas[i][10]);
			int feeD = atoi(m_datas[i][11]);
			if (type != 0)
			{
				sumN++;
				if (type == 1) //ѧ���ɷ�
				{
					sumM0 += fee;
					if (feeD != -1)
						sumM += fee - feeD;
				}
				else if (type == 2) //��У�ɷ�
				{
					sumM0 -= fee;
					if (feeD != -1)
						sumM -= fee - feeD;
				}
			}
		}
		m_nSumM = sumM;
		m_strMsg.Format("ͳ����Ϣ�� ����%d�ڿ�ʱ��Ҫ����ɷѣ��ܹ���ɷ�%dԪ��δ�ɷ�%dԪ", sumN, sumM0, sumM);
	}
	else
	{
		m_strMsg = "";
	}
	UpdateData(FALSE);

	m_wndGrid.GridRefresh(m_datas.size());
}

void CDlgMyBooks::UpdateCoach(int nRow, int flag)
{
	CString strMsg, strSQL;
	CString strCoachID = m_datas[nRow][8];
	CString strClassID = m_datas[nRow][1];
	CString strBookDate = m_datas[nRow][0];
	if (flag == 0) //���������ڱ����һ����¼�� ��Ч��һ, ��ʱ��2Сʱ
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID, STUDENT)\
					  	VALUES('%s', '%s', 1, 2, '%s', 1)", strCoachID, strBookDate, strClassID);
	}
	else if (flag == 1) //������ȱ�ڱ����һ����¼��ȱ�ڼ�����һ�� ��Ч���趨ֵg_nSubForLeave
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID)\
					  	VALUES('%s', '%s', 0, 0, '%s')", strCoachID, strBookDate, strClassID);
	}
	else if (flag == 2) //�������ڣ�ѧԱȱ�ڣ����ڱ����һ����¼�� ��Ч����, ��ʱ�漨Ч�ı�
	{
		strSQL.Format("INSERT INTO coachCheck (COACH_ID, CHECK_DATE, TYPE, WORK_TIME, CLASS_ID)\
					  	VALUES('%s', '%s', 1, 2, '%s')", strCoachID, strBookDate, strClassID);
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

void CDlgMyBooks::UpdateStudent(int nRow, int flag)
{
	CString strMsg, strSQL;
	CString strStuID = m_datas[nRow][7];
	CString strClassID = m_datas[nRow][1];
	CString strBookDate = m_datas[nRow][0];
	if (flag == 0) //�������ѵ���� bookings��FLAG�ֶθ���Ϊ��2���� ���Ͽ�ʱ����һ
	{
		CString strService = m_datas[nRow][5];
		CString strSelf = m_datas[nRow][6];
		strSQL.Format("UPDATE bookings SET FLAG='2', SERVICE_SCORE='%s', SELF_SCORE='%s'\
					  	WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
						strService, strSelf,
						strStuID, strBookDate, strClassID);
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
						strStuID, strBookDate, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}
	else if (flag == 2) //����ȱ�ڣ�FLAG�ֶθ���Ϊ��-1���� ���Ͽ�ʱ������
	{
		strSQL.Format("UPDATE bookings SET FLAG='-2'\
					  	WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
						strStuID, strBookDate, strClassID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}

}


void CDlgMyBooks::OnBnClickedMoney()
{
	if (m_nSumM == 0)
	{
		MessageBox("��ǰѧԱû����ɷ���Ŀ", "����");
		return;
	}

	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	CString strMsg, strSQL;
	BOOL isOneTime = FALSE;
	if (pRow == NULL) //û��ѡ�У�ȫ��һ���Խ���
	{
		strMsg = "��û��ѡ��ɷѶ����Ƿ�ѡ��һ���Խ���ȫ�����ã�";
		if (MessageBox(strMsg, "����", MB_YESNOCANCEL) != IDYES) return;
		isOneTime = TRUE;
	}
	if (isOneTime == TRUE)
	{
		strMsg.Format("��ǰѧԱ��Ƿ��%dԪ����ȷ�Ͻ������ȷ�ϣ�", m_nSumM);
		if (MessageBox(strMsg, "����", MB_YESNO) != IDYES) return;

		strSQL.Format("UPDATE bookings SET FEE_DONE='-1' WHERE FILE_NAME='%s' AND BOOK_TYPE!='0'", m_strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
		Refresh();
		return;
	}

	int n = m_datas.size();
	int sumM = 0;
	std::vector<int> indexes;
	for (int i = 0; i < n; i++)
	{
		if (!m_wndGrid.IsRowSelected(i)) continue;

		int type = atoi(m_datas[i][9]);
		int fee = atoi(m_datas[i][10]);
		int feeD = atoi(m_datas[i][11]);
		if (type != 0)
		{
			indexes.push_back(i);
			if (type == 1) //ѧ���ɷ�
			{
				sumM += fee - feeD;
			}
			else if (type == 2) //��У�ɷ�
			{
				sumM -= fee - feeD;
			}
		}
	}
	strMsg.Format("��ǰѡ�з�Χ�У���ѧԱ��ɷ�%dԪ����ȷ�Ͻ������ȷ�ϣ�", sumM);
	if (MessageBox(strMsg, "����", MB_YESNO) != IDYES) return;

	//�������ݿ�
	n = indexes.size();
	for (int i = 0; i < n; i++)
	{
		int nRow = indexes[i];
		strSQL.Format("UPDATE bookings SET FEE_DONE='-1' WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'", m_strStuID, m_datas[nRow][0], m_datas[nRow][1]);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);
	}
	Refresh();
}
