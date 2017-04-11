// ViewK1Check.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewK1Check.h"
#include "MainFrm.h"
#include "MSGINFO.h"


static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK1Check* pThis = (CViewK1Check*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		if (nCol == 0)
		{
			CString str;
			str.Format("%d", nRow);
			pdi->item.varValue = str;

		}
		else
		{
			pdi->item.varValue = pThis->m_datas[nRow][nCol-1];
		}
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
		if (nCol == 0)
		{
			CString str;
			str.Format("%d", nRow);
			pdi->item.varValue = str;

		}
		else
		{
			pdi->item.varValue = pThis->m_datas_pass[nRow][nCol - 1];
		}
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
		if (nCol == 0)
		{
			CString str;
			str.Format("%d", nRow);
			pdi->item.varValue = str;

		}
		else
		{
			pdi->item.varValue = pThis->m_datas_nopass[nRow][nCol - 1];
		}
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
	ON_BN_CLICKED(IDC_BTN_PASS, &CViewK1Check::OnBnClickedBtnPass)
	ON_BN_CLICKED(IDC_BTN_FRESH, &CViewK1Check::OnBnClickedBtnFresh)
	ON_BN_CLICKED(IDC_BTN_NOPASS, &CViewK1Check::OnBnClickedBtnNopass)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RETURN1, &CViewK1Check::OnBnClickedReturn1)
	ON_BN_CLICKED(IDC_RETURN2, &CViewK1Check::OnBnClickedReturn2)
	ON_BN_CLICKED(IDC_BTN_SMS1, &CViewK1Check::OnBnClickedBtnSms1)
	ON_BN_CLICKED(IDC_BTN_SMS2, &CViewK1Check::OnBnClickedBtnSms2)
	ON_BN_CLICKED(IDC_BTN_FRESH2, &CViewK1Check::OnBnClickedBtnFresh2)
END_MESSAGE_MAP()


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
	InitList(&m_wndGrid_nopass, rectGrid);
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
	pGrid->EnableHeader(TRUE, BCGP_GRID_HEADER_MOVE_ITEMS); //�����ͷ�ƶ����ǲ���ɾ����ͷ
	// Set grid tab order (first):
	pGrid->SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	pGrid->SetReadOnly();
	pGrid->SetWholeRowSel();
	pGrid->InsertColumn(0, _T("���"), 50);
	pGrid->InsertColumn(1, _T("����"), 70);
	pGrid->InsertColumn(2, _T("�Ա�"), 50);
	pGrid->InsertColumn(3, _T("�ֻ�"), 80);
	pGrid->InsertColumn(4, _T("����"), 80);
	pGrid->InsertColumn(5, _T("������"), rect.Width() - 330);
	pGrid->SetColumnAlign(0, HDF_CENTER);
	pGrid->SetColumnAlign(1, HDF_CENTER);
	pGrid->SetColumnAlign(2, HDF_CENTER);
	pGrid->SetColumnAlign(3, HDF_CENTER);
	pGrid->SetColumnAlign(4, HDF_CENTER);
	pGrid->SetColumnAlign(5, HDF_CENTER);
	pGrid->SetHeaderAlign(0, HDF_CENTER);
	pGrid->SetHeaderAlign(1, HDF_CENTER);
	pGrid->SetHeaderAlign(2, HDF_CENTER);
	pGrid->SetHeaderAlign(3, HDF_CENTER);
	pGrid->SetHeaderAlign(4, HDF_CENTER);
	pGrid->SetHeaderAlign(5, HDF_CENTER);
}

void CViewK1Check::Refresh(BOOL isInit)
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT SNAME, GENDER, TEL, CAR_TYPE, FILE_NUMBER FROM students WHERE STEP='0'");
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("��ѯ������Ϣ�ɹ�");
	}
	else ShowMsg2Output1(strMsg);

	if (isInit)
	{
		strSQL.Format("SELECT SNAME, GENDER, TEL, CAR_TYPE, FILE_NUMBER FROM students WHERE STEP='1'");
		m_datas_pass.clear();
		if (g_mysqlCon.ExecuteQuery(strSQL, m_datas_pass, strMsg))
		{
			ShowMsg2Output1("��ѯͨ��������Ϣ�ɹ�");
		}
		else ShowMsg2Output1(strMsg);

		strSQL.Format("SELECT SNAME, GENDER, TEL, CAR_TYPE, FILE_NUMBER FROM students WHERE STEP='1000'");
		m_datas_nopass.clear();
		if (g_mysqlCon.ExecuteQuery(strSQL, m_datas_nopass, strMsg))
		{
			ShowMsg2Output1("��ѯͨ��������Ϣ�ɹ�");
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
			strSQL.Format("UPDATE students SET STEP='1' WHERE FILE_NUMBER='%s'", fileNum);
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
			nSel++;
			CString fileNum = m_datas[i][4];
			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='1000' WHERE FILE_NUMBER='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("����������Ϣ�ɹ�");
				CStrs strs = m_datas[i];
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
			strSQL.Format("UPDATE students SET STEP='0' WHERE FILE_NUMBER='%s'", fileNum);
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
			strSQL.Format("UPDATE students SET STEP='0' WHERE FILE_NUMBER='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("����������Ϣ�ɹ�");
				CStrs strs = m_datas_nopass[i];
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
	CMSGINFO dlgMsg;
	dlgMsg.m_nFlag = 1;
	dlgMsg.m_strSMS.Format("������%d���������Ϳ�Ŀһ����֪ͨ���ţ���ȷ�������������ȷ�ϰ�ť", nCount);
	if(dlgMsg.DoModal() != IDOK) return;

	//���ݴ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if (pFrame->m_pSendBuf != NULL)
	{
		MessageBox("��һ����Ϣ��δ������ϣ����Ե����ԡ�");
	}
	else
	{
		int len = 6 + nCount * 8; //Type(1) Flag(1) Number(4) FileNums(Number*8)
		pFrame->m_isSendReady = FALSE;
		pFrame->m_pSendBuf = new BYTE[len];//������ɾ��
		pFrame->m_nSendLen = len;
		pFrame->m_pSendBuf[0] = 2; //���Ͷ���ƽ̨����
		pFrame->m_pSendBuf[1] = 1; //����֪ͨ����
		memcpy(pFrame->m_pSendBuf + 2, &nCount, 4); //��������

		CString strFileNum;
		for (int i = 0; i < nCount; i++)
		{
			strFileNum = m_datas_pass[i][4].Right(8);
			char* data = strFileNum.GetBuffer();
			memcpy(pFrame->m_pSendBuf + 6 + 8*i, data, 8);
			strFileNum.ReleaseBuffer();
		}

		pFrame->m_isSendReady = TRUE;
		m_datas_pass.clear();
		ListFresh();
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
		int len = 6 + nCount * 8; //Type(1) Flag(1) Number(4) FileNums(Number*8)
		pFrame->m_isSendReady = FALSE;
		pFrame->m_pSendBuf = new BYTE[len];//������ɾ��
		pFrame->m_nSendLen = len;
		pFrame->m_pSendBuf[0] = 2; //���Ͷ���ƽ̨����
		pFrame->m_pSendBuf[1] = 2; //�˿�֪ͨ����
		memcpy(pFrame->m_pSendBuf + 2, &nCount, 4); //��������

		CString strFileNum;
		for (int i = 0; i < nCount; i++)
		{
			strFileNum = m_datas_nopass[i][4].Right(8);
			char* data = strFileNum.GetBuffer();
			memcpy(pFrame->m_pSendBuf + 6 + 8 * i, data, 8);
			strFileNum.ReleaseBuffer();
		}
		pFrame->m_isSendReady = TRUE;

		m_datas_nopass.clear();
		ListFresh();
	}
}


void CViewK1Check::OnBnClickedBtnFresh2()
{
	Refresh(TRUE);
}
