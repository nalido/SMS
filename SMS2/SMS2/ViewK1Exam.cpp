// ViewViewK1Exam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewK1Exam.h"
#include "MSGINFO.h"


// CViewK1Exam

IMPLEMENT_DYNCREATE(CViewK1Exam, CBCGPFormView)

CViewK1Exam::CViewK1Exam()
	: CBCGPFormView(CViewK1Exam::IDD)
{
	EnableVisualManagerStyle();
}

CViewK1Exam::~CViewK1Exam()
{
}

void CViewK1Exam::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation1);
	DDX_Control(pDX, IDC_GRID2, m_wndGridLocation2);
}

BEGIN_MESSAGE_MAP(CViewK1Exam, CBCGPFormView)
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_PASS, &CViewK1Exam::OnBnClickedPass)
	ON_BN_CLICKED(IDC_RETURN, &CViewK1Exam::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_SENDSMS, &CViewK1Exam::OnBnClickedSendsms)
	ON_BN_CLICKED(IDC_K1PASS, &CViewK1Exam::OnBnClickedK1pass)
	ON_BN_CLICKED(IDC_UPDATE, &CViewK1Exam::OnBnClickedUpdate)
END_MESSAGE_MAP()


LRESULT CViewK1Exam::OnUserMessage(WPARAM wp, LPARAM lp)
{
	int flag = (int)lp;
	if (flag == 5)  //���ݷ��ͳɹ�
	{
		Refresh();
	}
	return 0;
}

LRESULT CViewK1Exam::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		Refresh();
		break;
	case 3:
		m_wndGrid2.GridRefresh(m_datas2.size());
	case 2:
		m_wndGrid1.GridRefresh(m_datas1.size());
		break;
	}

	return 0;
}

// CViewK1Exam ���

#ifdef _DEBUG
void CViewK1Exam::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewK1Exam::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewK1Exam ��Ϣ�������

static BOOL CALLBACK Grid1Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK1Exam* pThis = (CViewK1Exam*)lp;

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

	CViewK1Exam* pThis = (CViewK1Exam*)lp;

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

		if (pThis->m_datas2[nRow][6] == "4")
		{
			pdi->item.clrBackground = COLOR_MANY;
		}
	}

	return TRUE;
}


void CViewK1Exam::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


	CRect rect;
	m_wndGridLocation1.GetClientRect(&rect);
	m_wndGridLocation1.MapWindowPoints(this, &rect);

	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid1.Create(nStyle, rect, this, IDC_GRID_STUPRO);
	m_wndGrid1.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndGrid1.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndGrid1.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid1.SetSingleSel(); //ֻ��ѡһ��
	m_wndGrid1.EnableRowHeader(TRUE);
	m_wndGrid1.EnableLineNumbers();

	int nColumn = 0;
	int hw = m_wndGrid1.GetRowHeaderWidth();
	std::vector<CString> arrColumns;
	arrColumns.push_back("����");
	arrColumns.push_back("�Ա�");
	arrColumns.push_back("�ֻ�");
	arrColumns.push_back("������");
	arrColumns.push_back("��ʱ");
		

	int nColumns = arrColumns.size();
	m_nColumns1 = nColumns;
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGrid1.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndGrid1.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid1.SetHeaderAlign(i, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid1.EnableVirtualMode(Grid1Callback, (LPARAM)this);


	m_wndGridLocation2.GetClientRect(&rect);
	m_wndGridLocation2.MapWindowPoints(this, &rect);
	m_wndGrid2.Create(nStyle, rect, this, IDC_GRID_STUPRO);
	m_wndGrid2.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndGrid2.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndGrid2.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid2.SetReadOnly();
	m_wndGrid2.SetWholeRowSel();
	//m_wndGrid2.SetSingleSel(); //ֻ��ѡһ��
	m_wndGrid2.EnableRowHeader(TRUE);
	m_wndGrid2.EnableLineNumbers();

	nColumn = 0;
	hw = m_wndGrid2.GetRowHeaderWidth();
	arrColumns.clear();
	arrColumns.push_back("����");
	arrColumns.push_back("�Ա�");
	arrColumns.push_back("�ֻ�");
	arrColumns.push_back("����");
	arrColumns.push_back("�Ŀ�����");
	arrColumns.push_back("������");


	nColumns = arrColumns.size();
	//m_nColumns = nColumns;
	w = rect.Width() - hw;
	nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGrid2.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndGrid2.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid2.SetHeaderAlign(i, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid2.EnableVirtualMode(Grid2Callback, (LPARAM)this);
}

void CViewK1Exam::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT students.SNAME, students.GENDER, students.TEL, students.FILE_NAME, \
				  stuDates.CLASS_TIME FROM students\
				  left join stuDates ON students.FILE_NAME=stuDates.STU_ID \
				  WHERE students.STEP='2'");
	m_datas1.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg))
	{
		ShowMsg2Output1("��ѯ��Ŀһѧ����Ϣ�ɹ�");
	}
	ShowMsg2Output1(strMsg);

	m_wndGrid1.GridRefresh(m_datas1.size());

	strSQL.Format("SELECT students.SNAME, students.GENDER, students.TEL, students.CAR_TYPE,\
				  stuDates.K1_DATE, students.FILE_NAME, students.STEP FROM students \
				  inner join stuDates ON students.FILE_NAME=stuDates.STU_ID\
				  WHERE students.STEP<'6'");
	m_datas2.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas2, strMsg))
	{
		ShowMsg2Output1("��ѯ��Ŀһ����ѧ����Ϣ�ɹ�");
	}
	ShowMsg2Output1(strMsg);
	m_wndGrid2.GridRefresh(m_datas2.size());
}

void CViewK1Exam::OnBnClickedPass()
{
	CBCGPGridRow* pRow = m_wndGrid1.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strClassTime = pRow->GetItem(4)->GetValue();
		if (strClassTime.IsEmpty() || strClassTime=="0")
		{
			MessageBox("����дѧ����ʱ��Ϣ��");
			return;
		}

		CString strMsg, strSQL;
		CString strStuID = m_datas1[nRow][3];
		strSQL.Format("INSERT INTO stuDates (STU_ID, CLASS_TIME) VALUES('%s', '%s')", strStuID, strClassTime);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		{
			ShowMsg2Output1(strMsg);
			strSQL.Format("UPDATE students SET STEP='3' WHERE FILE_NAME='%s'", strStuID);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		}
		ShowMsg2Output1(strMsg);

		Refresh();
	}
}


void CViewK1Exam::OnBnClickedReturn()
{
	CBCGPGridRow* pRow = m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		if (m_datas2[nRow][6] == "4") return;

		CString strMsg, strSQL;
		CString strStuID = m_datas2[nRow][5];
		strSQL.Format("DELETE FROM stuDates WHERE STU_ID='%s'", strStuID);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		{
			ShowMsg2Output1(strMsg);
			strSQL.Format("UPDATE students SET STEP='2' WHERE FILE_NAME='%s'", strStuID);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		}
		ShowMsg2Output1(strMsg);

		Refresh();
	}

}


void CViewK1Exam::OnBnClickedSendsms()
{
	int nCount = m_datas2.size();

	//���ݴ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if (pFrame->m_pSendBuf != NULL)
	{
		MessageBox("��һ����Ϣ��δ������ϣ����Ե����ԡ�");
	}
	else
	{
		CString strClassIssue("");
		CString strSMS("");
		for (int i = 0; i < nCount; i++)
		{
			if (!m_wndGrid2.IsRowSelected(i)) continue;

			if (m_datas2[i][6] == "4")
			{
				CString strM;
				strM.Format("%s�Ѿ����͹�֪ͨ���ţ��Ƿ�������ͣ�", m_datas2[i][0]);
				if (MessageBox(strM, "����", MB_YESNO) != IDYES) continue;
			}

			CMSGINFO dlgMsg;
			dlgMsg.m_nFlag = 3;
			dlgMsg.m_strStu = m_datas2[i][0];
			dlgMsg.m_strClassIssue = strClassIssue;
			if (dlgMsg.DoModal() != IDOK) continue;
			strClassIssue = dlgMsg.m_strClassIssue;
			CString strSMS0 = dlgMsg.m_strSMS;

			//���ݴ������
			CString strStuID = m_datas2[i][5];
			CString strTel = m_datas2[i][2];
			strSMS.Format("%s:%s>%s", strTel, strStuID, strSMS0);
			int SMSlen = strlen(strSMS);
			int len = 6 + SMSlen;
			pFrame->m_isSendReady = FALSE;
			pFrame->m_pSendBuf = new BYTE[len];//������ɾ��
			pFrame->m_nSendLen = len;
			pFrame->m_pSendBuf[0] = 2; //���Ͷ���ƽ̨����
			pFrame->m_pSendBuf[1] = dlgMsg.m_nFlag; //��������
			memcpy(pFrame->m_pSendBuf + 2, &SMSlen, 4); //��������

			char* data = strSMS.GetBuffer();
			memcpy(pFrame->m_pSendBuf + 6, data, SMSlen);
			strSMS.ReleaseBuffer();
			pFrame->m_isSendReady = TRUE;

			CString strDate = dlgMsg.m_strDate; //��������
			CString strSQL, strMsg;
			strSQL.Format("UPDATE stuDates SET K1_DATE='%s' WHERE STU_ID='%s'", strDate, strStuID);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);

			strSQL.Format("INSERT INTO stuDateHistory (EXAM_DATE, STU_ID, EXAM_TYPE) VALUES('%s', '%s', '��Ŀһ')", strDate, strStuID);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);

			WaitForSingleObject(pFrame->m_hSocketEvent, 2000); //�ȴ���Ϣ����
		}

	}
}


void CViewK1Exam::OnBnClickedK1pass()
{
	CBCGPGridRow* pRow = m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strDate = m_datas2[nRow][4];
		if (strDate == "0") return;

		CString strSQL, strMsg;
		strMsg.Format("�ò������ɳ�������ȷ��%sѧԱ�Ƿ��Ѿ�ͨ����Ŀһ���ԣ�", m_datas2[nRow][0]);
		if (MessageBox(strMsg, "����", MB_YESNOCANCEL) != IDYES) return;

		strSQL.Format("UPDATE students SET STEP='6' WHERE FILE_NAME='%s'", m_datas2[nRow][5]);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		strSQL.Format("UPDATE stuDates SET K1_STAT='1' WHERE STU_ID='%s'", m_datas2[nRow][5]);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		Refresh();
	}
}


void CViewK1Exam::OnBnClickedUpdate()
{
	Refresh();
}
