// ViewViewK1Exam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewK23Exam.h"
#include "MSGINFO.h"


// CViewK23Exam

IMPLEMENT_DYNCREATE(CViewK23Exam, CBCGPFormView)

CViewK23Exam::CViewK23Exam()
: CBCGPFormView(CViewK23Exam::IDD)
{
	EnableVisualManagerStyle();
}

CViewK23Exam::~CViewK23Exam()
{
}

void CViewK23Exam::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation1);
	DDX_Control(pDX, IDC_GRID2, m_wndGridLocation2);
}

BEGIN_MESSAGE_MAP(CViewK23Exam, CBCGPFormView)
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_PASS, &CViewK23Exam::OnBnClickedPass)
	ON_BN_CLICKED(IDC_RETURN, &CViewK23Exam::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_SENDSMS, &CViewK23Exam::OnBnClickedSendsms)
	ON_BN_CLICKED(IDC_K1PASS, &CViewK23Exam::OnBnClickedK23pass)
	ON_BN_CLICKED(IDC_UPDATE, &CViewK23Exam::OnBnClickedUpdate)
END_MESSAGE_MAP()


LRESULT CViewK23Exam::OnUserMessage(WPARAM wp, LPARAM lp)
{
	int flag = (int)lp;
	if (flag == 5)  //���ݷ��ͳɹ�
	{
		Refresh();
	}
	return 0;
}

LRESULT CViewK23Exam::OnUserUpdate(WPARAM wParam, LPARAM lParam)
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

// CViewK23Exam ���

#ifdef _DEBUG
void CViewK23Exam::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewK23Exam::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewK23Exam ��Ϣ�������

static BOOL CALLBACK Grid1Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK23Exam* pThis = (CViewK23Exam*)lp;

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

	CViewK23Exam* pThis = (CViewK23Exam*)lp;

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


void CViewK23Exam::OnInitialUpdate()
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
	m_wndGrid1.SetReadOnly();
	m_wndGrid1.SetWholeRowSel();
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
	arrColumns.push_back("��ѵ����");
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
	arrColumns.push_back("������");
	arrColumns.push_back("��������");
	arrColumns.push_back("���Կ�Ŀ");


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

void CViewK23Exam::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT students.SNAME, students.GENDER, students.TEL, students.FILE_NAME, \
					students.CLASS_TYPE, students.CLASS_NUM FROM students \
					inner join stuDates ON students.FILE_NAME=stuDates.STU_ID\
					WHERE students.CLASS_TYPE!='0'");
	m_datas1.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg))
	{
		ShowMsg2Output1("��ѯ·ѵѧ����Ϣ�ɹ�");
	}
	ShowMsg2Output1(strMsg);

	m_wndGrid1.GridRefresh(m_datas1.size());



	strSQL.Format("SELECT students.SNAME, students.GENDER, students.TEL, students.CAR_TYPE,\
				  	students.FILE_NAME, stuDates.K2_DATE, students.CLASS_TYPE, \
					stuDates.K2_STAT, stuDates.K3_STAT FROM students \
					inner join stuDates ON students.FILE_NAME=stuDates.STU_ID\
					WHERE stuDates.K2_STAT='1'");
	m_datas2.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas2, strMsg);
	ShowMsg2Output1(strMsg);
	strSQL.Format("SELECT students.SNAME, students.GENDER, students.TEL, students.CAR_TYPE,\
				  	students.FILE_NAME, stuDates.K3_DATE, students.CLASS_TYPE FROM students \
					inner join stuDates ON students.FILE_NAME=stuDates.STU_ID\
					WHERE stuDates.K3_STAT='1'");
	g_mysqlCon.ExecuteQuery(strSQL, m_datas2, strMsg);
	ShowMsg2Output1(strMsg);
	m_wndGrid2.GridRefresh(m_datas2.size());
}

void CViewK23Exam::OnBnClickedPass()
{
	CBCGPGridRow* pRow = m_wndGrid1.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strClassNum = m_datas1[nRow][5];
		int nclass = atoi(strClassNum);
		CString strClassType = m_datas1[nRow][4];
		CString strTypeName;
		if (strClassType == "��Ŀ��")
		{
			strTypeName = "K2_STAT";
			if (nclass < 8)
			{
				if (MessageBox("��ѧԱ��ʱ��δ�ﵽ����Ҫ��ȷ�ϱ�����", "����", MB_YESNO) != IDYES) return;
			}
		}
		else if (strClassType == "��Ŀ��")
		{
			strTypeName = "K3_STAT";
			if (nclass < 10)
			{
				if (MessageBox("��ѧԱ��ʱ��δ�ﵽ����Ҫ��ȷ�ϱ�����", "����", MB_YESNO) != IDYES) return;
			}
		}

		CString strMsg, strSQL;
		CString strStuID = m_datas1[nRow][3];
		strSQL.Format("UPDATE stuDates SET %s='1' WHERE STU_ID='%s'", strTypeName, strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		Refresh();
	}
}


void CViewK23Exam::OnBnClickedReturn()
{
	CBCGPGridRow* pRow = m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		if (m_datas2[nRow][5] != "0") return;

		CString strMsg, strSQL;
		CString strStuID = m_datas2[nRow][4];
		CString strClassType = m_datas2[nRow][6];
		CString strTypeName;
		if (strClassType == "��Ŀ��")
		{
			strTypeName = "K2_STAT";
		}
		else if (strClassType == "��Ŀ��")
		{
			strTypeName = "K3_STAT";
		}

		strSQL.Format("UPDATE stuDates SET %s='0' WHERE STU_ID='%s'", strTypeName, strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		Refresh();
	}

}


void CViewK23Exam::OnBnClickedSendsms()
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

			WaitForSingleObject(pFrame->m_hSocketEvent, 2000); //�ȴ���Ϣ����
		}

	}
}


void CViewK23Exam::OnBnClickedK23pass()
{
	CBCGPGridRow* pRow = m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strSQL, strMsg;
		strMsg.Format("�ò������ɳ�������ȷ��%sѧԱ�Ƿ��Ѿ�ͨ����Ŀһ���ԣ�", m_datas2[nRow][0]);
		if (MessageBox(strMsg, "����", MB_YESNOCANCEL) != IDYES) return;

		strSQL.Format("UPDATE students SET STEP='6' WHERE FILE_NAME='%s'", m_datas2[nRow][5]);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		Refresh();
	}
}


void CViewK23Exam::OnBnClickedUpdate()
{
	Refresh();
}
