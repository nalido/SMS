// ViewK1Check.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewK1Check.h"
#include "MainFrm.h"
#include "MSGINFO.h"
#include "DlgNoPass.h"


static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK1Check* pThis = (CViewK1Check*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		pdi->item.varValue = pThis->m_datas[nRow][nCol];
	}

	return TRUE;
}


static BOOL CALLBACK GridCallback2(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK1Check* pThis = (CViewK1Check*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas_pass.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		pdi->item.varValue = pThis->m_datas_pass[nRow][nCol];
	}

	return TRUE;
}

static BOOL CALLBACK GridCallback3(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK1Check* pThis = (CViewK1Check*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas_nopass.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		pdi->item.varValue = pThis->m_datas_nopass[nRow][nCol];
	}

	return TRUE;
}
// CViewK1Check

IMPLEMENT_DYNCREATE(CViewK1Check, CBCGPFormView)

CViewK1Check::CViewK1Check()
	: CBCGPFormView(CViewK1Check::IDD)
{
	EnableVisualManagerStyle();
}

CViewK1Check::~CViewK1Check()
{
}

void CViewK1Check::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_wndGridLocation);
	DDX_Control(pDX, IDC_LIST2, m_wndGridLocation_pass);
	DDX_Control(pDX, IDC_LIST3, m_wndGridLocation_nopass);
}

BEGIN_MESSAGE_MAP(CViewK1Check, CBCGPFormView)
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_BTN_PASS, &CViewK1Check::OnBnClickedBtnPass)
	ON_BN_CLICKED(IDC_BTN_FRESH, &CViewK1Check::OnBnClickedBtnFresh)
	ON_BN_CLICKED(IDC_BTN_NOPASS, &CViewK1Check::OnBnClickedBtnNopass)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RETURN1, &CViewK1Check::OnBnClickedReturn1)
	ON_BN_CLICKED(IDC_RETURN2, &CViewK1Check::OnBnClickedReturn2)
	ON_BN_CLICKED(IDC_BTN_SMS1, &CViewK1Check::OnBnClickedBtnSms1)
	ON_BN_CLICKED(IDC_BTN_SMS2, &CViewK1Check::OnBnClickedBtnSms2)
	ON_BN_CLICKED(IDC_BTN_FRESH2, &CViewK1Check::OnBnClickedBtnFresh2)
	ON_BN_CLICKED(IDC_NEW_STUDENT, &CViewK1Check::OnBnClickedNewStudent)
END_MESSAGE_MAP()


LRESULT CViewK1Check::OnUserMessage(WPARAM wp, LPARAM lp)
{
	int flag = (int)lp;
	if (flag == 5)  //���ݷ��ͳɹ�
	{
		Refresh(1);
		ShowMsg2Output1("���Ͷ��ųɹ�");
	}
	return 0;
}

LRESULT CViewK1Check::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;

	if (flag == 1) //update data from database
	{
		//���ݳ�ʼ��
		Refresh();
	}

	return 0;
}
// CViewK1Check ���

#ifdef _DEBUG
void CViewK1Check::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewK1Check::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewK1Check ��Ϣ�������

int CViewK1Check::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;



	return 0;
}


void CViewK1Check::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();



	//��ʼ�����
	CRect rectGrid;
	m_wndGridLocation.GetClientRect(&rectGrid);
	m_wndGridLocation.MapWindowPoints(this, &rectGrid); //תΪ��������
	InitList(&m_wndGrid, rectGrid);
	//ע�������б�ص�����
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);

	m_wndGridLocation_pass.GetClientRect(&rectGrid);
	m_wndGridLocation_pass.MapWindowPoints(this, &rectGrid); //תΪ��������
	InitList(&m_wndGrid_pass, rectGrid);
	//ע�������б�ص�����
	m_wndGrid_pass.EnableVirtualMode(GridCallback2, (LPARAM)this);


	m_wndGridLocation_nopass.GetClientRect(&rectGrid);
	m_wndGridLocation_nopass.MapWindowPoints(this, &rectGrid); //תΪ��������
	//InitList(&m_wndGrid_nopass, rectGrid);
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid_nopass.Create(nStyle, rectGrid, this, (UINT)-3);
	m_wndGrid_nopass.SetCustomColors(-1, -1, -1, -1, -1, RGB(0, 0, 0)); //��ɫ�߿�
	m_wndGrid_nopass.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndGrid_nopass.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid_nopass.SetReadOnly();
	m_wndGrid_nopass.SetWholeRowSel();
	m_wndGrid_nopass.EnableRowHeader(TRUE);
	m_wndGrid_nopass.EnableLineNumbers();

	int hw = m_wndGrid_nopass.GetRowHeaderWidth();
	std::vector<CString> arrColumns;
	arrColumns.push_back("����");
	arrColumns.push_back("�Ա�");
	arrColumns.push_back("�ֻ�");
	arrColumns.push_back("ԭ��");
	arrColumns.push_back("������");
	const int nColumns = arrColumns.size();
	int w = rectGrid.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int nColumn = 0; nColumn < nColumns; nColumn++)
	{
		m_wndGrid_nopass.InsertColumn(nColumn, arrColumns[nColumn], nColumnWidth);
		m_wndGrid_nopass.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid_nopass.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid_nopass.EnableVirtualMode(GridCallback3, (LPARAM)this);
	
	//��ѯ������Ϣ
	Refresh(TRUE); //��һ��ˢ��Ϊ�������ˢ��
}

void CViewK1Check::InitList(CVirtualGridCtrl* pGrid, CRect& rect)
{
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	pGrid->Create(nStyle, rect, this, (UINT)-3);
	pGrid->SetCustomColors(-1, -1, -1, -1, -1, RGB(0, 0, 0)); //��ɫ�߿�
	pGrid->EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	pGrid->SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	pGrid->SetReadOnly();
	pGrid->SetWholeRowSel();
	pGrid->EnableRowHeader(TRUE);
	pGrid->EnableLineNumbers();

	int hw = pGrid->GetRowHeaderWidth();
	LPCTSTR arrColumns[] = { _T("����"), _T("�Ա�"), _T("�ֻ�"), _T("����"), _T("������")};
	int w[5] = { 70, 50, 80, 80, 0};
	w[4] = rect.Width() - hw - 280;
	const int nColumns = sizeof (arrColumns) / sizeof (LPCTSTR);
	for (int nColumn = 0; nColumn < nColumns; nColumn++)
	{
		pGrid->InsertColumn(nColumn, arrColumns[nColumn], w[nColumn]);
		pGrid->SetColumnAlign(nColumn, HDF_CENTER);
		pGrid->SetHeaderAlign(nColumn, HDF_CENTER);
	}
}

void CViewK1Check::Refresh(BOOL isInit)
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT SNAME, GENDER, TEL, CAR_TYPE, FILE_NAME FROM students WHERE STEP='0000'");
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("��ѯ������Ϣ�ɹ�");
	}
	else ShowMsg2Output1(strMsg);

	if (isInit)
	{
		strSQL.Format("SELECT SNAME, GENDER, TEL, CAR_TYPE, FILE_NAME FROM students WHERE STEP='1'");
		m_datas_pass.clear();
		if (g_mysqlCon.ExecuteQuery(strSQL, m_datas_pass, strMsg))
		{
			ShowMsg2Output1("��ѯͨ��������Ϣ�ɹ�");
		}
		else ShowMsg2Output1(strMsg);

		strSQL.Format("SELECT SNAME, GENDER, TEL, NOPASS_REASON, FILE_NAME, CAR_TYPE FROM students WHERE STEP='1000'");
		m_datas_nopass.clear();
		if (g_mysqlCon.ExecuteQuery(strSQL, m_datas_nopass, strMsg))
		{
			ShowMsg2Output1("��ѯδͨ��������Ϣ�ɹ�");
		}
		else ShowMsg2Output1(strMsg);
	}
	ListFresh();
}

void CViewK1Check::ListFresh()
{
	m_wndGrid.RemoveAll();
	m_wndGrid.SetVirtualRows(m_datas.size());
	m_wndGrid.AdjustLayout();
	m_wndGrid_pass.RemoveAll();
	m_wndGrid_pass.SetVirtualRows(m_datas_pass.size());
	m_wndGrid_pass.AdjustLayout();
	m_wndGrid_nopass.RemoveAll();
	m_wndGrid_nopass.SetVirtualRows(m_datas_nopass.size());
	m_wndGrid_nopass.AdjustLayout();
}

void CViewK1Check::OnBnClickedBtnPass()
{
	int nCount = m_datas.size();
	int nSel = 0;
	CString strMsg;
	for (int i = nCount-1; i >=0; i--)
	{
		if (m_wndGrid.IsRowSelected(i))
		{
			nSel++;
			CString fileNum = m_datas[i][4];
			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='0001' WHERE FILE_NAME='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("����������Ϣ�ɹ�");
				CStrs strs = m_datas[i];
				m_datas_pass.push_back(strs);
				RemoveData(0, i);
			}
			else ShowMsg2Output1(strMsg);
		}
	}
	ListFresh();// Refresh();
	strMsg.Format("��ѡ��%d��", nSel);
	ShowMsg2Output1(strMsg);
}

void CViewK1Check::RemoveData(int type, int nRow)
{
	std::vector<CStrs>::iterator it;
	switch (type)
	{
	case 0: 
		it = m_datas.begin() + nRow;
		m_datas.erase(it);
		break;
	case 1:
		it = m_datas_pass.begin() + nRow;
		m_datas_pass.erase(it);
		break;
	case 2:
		it = m_datas_nopass.begin() + nRow;
		m_datas_nopass.erase(it);
		break;
	}
}

void CViewK1Check::OnBnClickedBtnFresh()
{
	Refresh();
}


void CViewK1Check::OnBnClickedBtnNopass()
{
	int nCount = m_datas.size();
	int nSel = 0;
	CString strMsg;
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_wndGrid.IsRowSelected(i))
		{
			CDlgNoPass dlg;
			dlg.m_strStuName = m_datas[i][0];
			if (dlg.DoModal() != IDOK) continue;
			CString strReason = "ԭ��:" + dlg.m_strReason;

			nSel++;
			CString fileNum = m_datas[i][4];
			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='1000', NOPASS_REASON='%s' WHERE FILE_NAME='%s'", strReason, fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("����������Ϣ�ɹ�");
				CStrs strs = m_datas[i];
				CString strCarType = strs[3];
				strs[3] = strReason;
				strs.push_back(strCarType);
				m_datas_nopass.push_back(strs);
				RemoveData(0, i);
			}
			else ShowMsg2Output1(strMsg);
		}
	}
	ListFresh();// Refresh();
	strMsg.Format("��ѡ��%d��", nSel);
	ShowMsg2Output1(strMsg);
}


void CViewK1Check::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);


	//if (IsWindow(m_wndGridLocation.GetSafeHwnd())) //�жϿؼ������Ƿ��Ѿ�����
	//{
	//	CRect rect, rect_btn;
	//	GetClientRect(&rect);
	//	//GetDlgItem(IDC_BUTTON1)->GetClientRect(&rect_btn);��
	//	//GetDlgItem(IDC_BUTTON1)->MapWindowPoints(this, &rect_btn);

	//	//GetDlgItem(IDC_BUTTON1)->MoveWindow(rect.right / 2, rect_btn.top, rect_btn.Width(), rect_btn.Height());
	//}
}


void CViewK1Check::OnBnClickedReturn1()
{
	int nCount = m_datas_pass.size();
	int nSel = 0;
	CString strMsg;
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_wndGrid_pass.IsRowSelected(i))
		{
			nSel++;
			CString fileNum = m_datas_pass[i][4];
			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='0000' WHERE FILE_NAME='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("����������Ϣ�ɹ�");
				CStrs strs = m_datas_pass[i];
				m_datas.push_back(strs);
				RemoveData(1, i);
			}
			else ShowMsg2Output1(strMsg);
		}
	}
	ListFresh();// Refresh();
	strMsg.Format("��ѡ��%d��", nSel);
	ShowMsg2Output1(strMsg);
}


void CViewK1Check::OnBnClickedReturn2()
{
	int nCount = m_datas_nopass.size();
	int nSel = 0;
	CString strMsg;
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_wndGrid_nopass.IsRowSelected(i))
		{
			nSel++;
			CString fileNum = m_datas_nopass[i][4];
			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='0000' WHERE FILE_NAME='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("����������Ϣ�ɹ�");
				CStrs strs = m_datas_nopass[i];
				strs[3] = strs[5];
				strs.pop_back();
				m_datas.push_back(strs);
				RemoveData(2, i);
			}
			else ShowMsg2Output1(strMsg);
		}
	}
	ListFresh();// Refresh();
	strMsg.Format("��ѡ��%d��", nSel);
	ShowMsg2Output1(strMsg);
}


void CViewK1Check::OnBnClickedBtnSms1()
{
	int nCount = m_datas_pass.size();

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
			if (!m_wndGrid_pass.IsRowSelected(i)) continue;

			CMSGINFO dlgMsg;
			dlgMsg.m_nFlag = 1;
			dlgMsg.m_strStu = m_datas_pass[i][0];
			dlgMsg.m_strClassIssue = strClassIssue;
			if (dlgMsg.DoModal() != IDOK) continue;
			strClassIssue = dlgMsg.m_strClassIssue;
			CString strSMS0 = dlgMsg.m_strSMS;

			//���ݴ������
			CString strStuID = m_datas_pass[i][4];
			CString strTel = m_datas_pass[i][2];
			strSMS.Format("%s:%s>%s", strTel, strStuID, strSMS0);
			int SMSlen = strlen(strSMS);
			int len = 6 + SMSlen;
			pFrame->m_isSendReady = FALSE;
			pFrame->m_pSendBuf = new BYTE[len];//������ɾ��
			pFrame->m_nSendLen = len;
			pFrame->m_pSendBuf[0] = 2; //���Ͷ���ƽ̨����
			pFrame->m_pSendBuf[1] = 1; //����֪ͨ����
			memcpy(pFrame->m_pSendBuf + 2, &SMSlen, 4); //��������

			char* data = strSMS.GetBuffer();
			memcpy(pFrame->m_pSendBuf + 6, data, SMSlen);
			strSMS.ReleaseBuffer();
			pFrame->m_isSendReady = TRUE;

			m_datas_pass[i][2] = "�ѷ���";

			WaitForSingleObject(pFrame->m_hSocketEvent, 2000); //�ȴ���Ϣ����
		}

	}
}


void CViewK1Check::OnBnClickedBtnSms2()
{
	//���ݴ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if (pFrame->m_pSendBuf != NULL)
	{
		MessageBox("��һ����Ϣ��δ������ϣ����Ե����ԡ�");
	}
	else
	{
		int nCount = m_datas_nopass.size();

		CString strFileNum;
		CString strClassIssue("");
		CString strSMS("");
		for (int i = 0; i < nCount; i++)
		{
			if (!m_wndGrid_nopass.IsRowSelected(i)) continue;

			CMSGINFO dlgMsg;
			dlgMsg.m_nFlag = 2;
			dlgMsg.m_strStu = m_datas_nopass[i][0];
			dlgMsg.m_strClassIssue = strClassIssue;
			dlgMsg.m_strReason = m_datas_nopass[i][3];
			if (dlgMsg.DoModal() != IDOK) continue;
			strClassIssue = dlgMsg.m_strClassIssue;
			CString strSMS0 = dlgMsg.m_strSMS;


			//���ݴ������
			CString strStuID = m_datas_nopass[i][4];
			CString strTel = m_datas_nopass[i][2];
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

			m_datas_nopass[i][2] = "�ѷ���";

			WaitForSingleObject(pFrame->m_hSocketEvent, 2000); //�ȴ���Ϣ����
		}
	}
}


void CViewK1Check::OnBnClickedBtnFresh2()
{
	Refresh(TRUE);
}


void CViewK1Check::OnBnClickedNewStudent()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SelectView(VIEW_REGISTER);
}
