// ViewStuProgress.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewStuProgress.h"
#include "MainFrm.h"
#include <afxmsg_.h>


static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewStuProgress* pThis = (CViewStuProgress*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
			std::vector<CStrs>::iterator it = pThis->m_datas.begin() + nRow;
			if (!it->empty())
			{
				if (nCol < 5)
				{
					pdi->item.varValue = pThis->m_datas[nRow][nCol];
				}
				else //����ǽ���
				{
					CString strStep = pThis->m_datas[nRow][5];
					int step = atoi(strStep);
					int rCol = (nCol - 4); //���ȵ��кŴ�1��ʼ 1=״̬1(step=1~2)
					int tmp = rCol * 2 - 1;
					if (tmp < step)
					{
						pdi->item.clrBackground = RGB(149, 200, 146);
						pdi->item.varValue = "���";
					}
					if (tmp == step)
					{
						pdi->item.clrBackground = RGB(195, 218, 195);
						pdi->item.varValue = "������";
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

// CViewStuProgress

IMPLEMENT_DYNCREATE(CViewStuProgress, CBCGPFormView)


CViewStuProgress::CViewStuProgress()
	: CBCGPFormView(CViewStuProgress::IDD)
{
	EnableVisualManagerStyle();
}

CViewStuProgress::~CViewStuProgress()
{
}

void CViewStuProgress::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STUPROGRESS, m_wndGridLocation);
}

BEGIN_MESSAGE_MAP(CViewStuProgress, CBCGPFormView)
	ON_BN_CLICKED(IDC_STUFRESH, &CViewStuProgress::OnBnClickedStufresh)
	ON_BN_CLICKED(IDC_SENDBOOKMSG, &CViewStuProgress::OnBnClickedSendbookmsg)
	ON_BN_CLICKED(IDC_TOBOOK, &CViewStuProgress::OnBnClickedTobook)
END_MESSAGE_MAP()


// CViewStuProgress ���

#ifdef _DEBUG
void CViewStuProgress::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewStuProgress::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewStuProgress ��Ϣ�������

void CViewStuProgress::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


	//��ʼ�����
	CRect rectGrid;
	m_wndGridLocation.GetClientRect(&rectGrid);
	m_wndGridLocation.MapWindowPoints(this, &rectGrid); //תΪ��������
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndGrid.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	m_wndGrid.EnableRowHeader(TRUE);
	m_wndGrid.EnableLineNumbers();


	int nColumn = 0;
	m_wndGrid.InsertColumn(nColumn++, _T("������"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("����"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("�Ա�"), 50);
	m_wndGrid.InsertColumn(nColumn++, _T("�ֻ���"), 80);
	m_wndGrid.InsertColumn(nColumn++, _T("�������"), 80);
	int wid = (rectGrid.Width() - 350 - 20) / 8;
	m_wndGrid.InsertColumn(nColumn++, _T("�������"), wid); //col = 5; step = 1~2
	m_wndGrid.InsertColumn(nColumn++, _T("��һ����"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("��һ����"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("��ѵԤԼ"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("�ƶ���ѵ"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("�ƶ�����"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("������ѵ"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("��������"), wid);
	for (int i = 0; i < nColumn; i++)
	{
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);
	m_wndGrid.SetCallBack_DblClk(OnGridDbClick);

	Refresh();
}

void CViewStuProgress::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT FILE_NAME, SNAME, GENDER, TEL, CAR_TYPE, STEP FROM students WHERE STEP<'1000'");
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("��ѯѧ����Ϣ�ɹ�");
	}
	else ShowMsg2Output1(strMsg);

	ListFresh();
}

void CViewStuProgress::OnBnClickedStufresh()
{
	Refresh();
}

void CViewStuProgress::ListFresh()
{
	m_wndGrid.RemoveAll();
	m_wndGrid.SetVirtualRows(m_datas.size());
	m_wndGrid.AdjustLayout();
}

void CViewStuProgress::OnBnClickedSendbookmsg()
{
	int nCount = m_datas.size();
	int nSel = 0;
	CString strMsg, strTmp;
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_wndGrid.IsRowSelected(i))
		{
			nSel++;
			int step = atoi(m_datas[i][5]);
			if (step < SP_K1PASS) //��Ŀһδ����
			{
				strTmp.Format("ѧԱ��%s)��δ��ɿ�Ŀһ�γ̣����ܷ���ԤԼ����", m_datas[i][0]);
				MessageBox(strTmp);
				continue;
				ShowMsg2Output1("��ԤԼ���ţ�ѡ������δ��ɿ�Ŀһ��ѧ������");
			}
			else if (step == SP_K2K3BOOKING)
			{
				strTmp.Format("ѧԱ��%s)�Ѿ����͹����ţ��Ƿ���Ȼѡ���Ͷ��ţ�", m_datas[i][0]);
				ShowMsg2Output1("��ԤԼ���ţ�ѡ�����ѷ������ŵ�ѧ������");
				if(MessageBox(strTmp, "", MB_YESNOCANCEL) != IDOK) continue;
			}
			else if (step > SP_K2K3BOOKING) //�Ѿ�ԤԼ���
			{
				strTmp.Format("ѧԱ��%s)�Ѿ����ԤԼ�����跢��ԤԼ����", m_datas[i][0]);
				MessageBox(strTmp);
				continue;
				ShowMsg2Output1("��ԤԼ���ţ�ѡ���������ԤԼ��ѧ������");
			}

			/*δ���*/
			//����ԤԼ����  
			//int len = 14; //һ�η���һ������
			//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			//pFrame->m_isSendReady = FALSE;
			//pFrame->m_pSendBuf = new BYTE[len];//������ɾ��
			//pFrame->m_nSendLen = len;
			//pFrame->m_pSendBuf[0] = 2; //���Ͷ���ƽ̨����
			//pFrame->m_pSendBuf[1] = 4; //��ѵԤԼ����
			//int n = 1;
			//memcpy(pFrame->m_pSendBuf + 2, &n, 4); //��������

			CString fileNum = m_datas[i][0];
			//CString strFileNum = fileNum.Right(8);
			//char* data = strFileNum.GetBuffer();
			//memcpy(pFrame->m_pSendBuf + 6 + 8 * i, data, 8);
			//strFileNum.ReleaseBuffer();
			//pFrame->m_isSendReady = TRUE;

			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='7' WHERE FILE_NAME='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("����������Ϣ�ɹ�");
				m_datas[i][5].Format("%d", SP_K2K3BOOKING);
			}
			else ShowMsg2Output1(strMsg);
		}
	}
	ListFresh();
	strMsg.Format("��ѡ��%d��", nSel);
	ShowMsg2Output1(strMsg);
}

void CALLBACK CViewStuProgress::OnGridDbClick(LPVOID lParam)
{
	CViewStuProgress* pThis = (CViewStuProgress*)lParam;
	
	CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CString strFileName = pThis->m_datas[nRow][0];
		pThis->MessageBox(strFileName);
	}
}

void CViewStuProgress::OnBnClickedTobook()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SelectView(VIEW_BOOKING1);
}
