// ViewAllStudents.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewAllStudents.h"
#include "DlgQuitSchool.h"


// CViewAllStudents

IMPLEMENT_DYNCREATE(CViewAllStudents, CBCGPFormView)

CViewAllStudents::CViewAllStudents()
	: CBCGPFormView(CViewAllStudents::IDD)
	, m_threadProcess(this, ThreadProcessCallback)
{
	EnableVisualManagerStyle();
}

CViewAllStudents::~CViewAllStudents()
{
	m_threadProcess.StopThread();
}

void CViewAllStudents::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX); 
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_TAB2, m_TabShowType);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);

	DDX_Control(pDX, IDC_PIE, m_Pie);
	DDX_Control(pDX, IDC_LABEL1, m_Label);


	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);

	DDX_Control(pDX, IDC_INFO, m_Info);



	DDX_Control(pDX, IDC_DATE1, m_DateS);
	DDX_Control(pDX, IDC_DATE2, m_DateE);
	DDX_Text(pDX, IDC_DATE1, m_strDateS);
	DDX_Text(pDX, IDC_DATE2, m_strDateE);
}

BEGIN_MESSAGE_MAP(CViewAllStudents, CBCGPFormView)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CViewAllStudents::OnTcnSelchangeTab1)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_QUERY, &CViewAllStudents::OnBnClickedQuery)
	ON_BN_CLICKED(IDC_QUIT, &CViewAllStudents::OnBnClickedQuit)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CViewAllStudents::OnTcnSelchangeTab2)
	ON_BN_CLICKED(IDC_EXPORT2, &CViewAllStudents::OnBnClickedExport2)
END_MESSAGE_MAP()


LRESULT CViewAllStudents::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		break;
	case 2: //����ͳ�����
	{
				int pos1 = m_TabShowType.GetCurSel();
				int pos0 = m_TabCtrl.GetCurSel();
				if (pos1==0)
					InvalidateRect(&m_rctInfo);
				if (pos1 == 1) //����ͳ��
				{
					std::vector<CString> arrColumns;
					m_wndGridM.DeleteAllColumns();
					arrColumns.push_back("ͳ���·�");
					arrColumns.push_back("ͳ������");
					int nColumns = m_nPiePart.size();
					for (int i = 0; i < nColumns; i++)
						arrColumns.push_back(m_strPiePart[i]);

					InitGridMY(&m_wndGridM, arrColumns);
					m_wndGridM.GridRefresh(m_datasM.size());
				}
				if (pos1 == 2) //����ͳ��
				{
					std::vector<CString> arrColumns;
					m_wndGridY.DeleteAllColumns();
					arrColumns.push_back("ͳ�����");
					arrColumns.push_back("ͳ������");
					int nColumns = m_nPiePart.size();
					for (int i = 0; i < nColumns; i++)
						arrColumns.push_back(m_strPiePart[i]);

					InitGridMY(&m_wndGridY, arrColumns);
					m_wndGridY.GridRefresh(m_datasY.size());
				}
	}
		break;
	}
	return TRUE;
}
// CViewAllStudents ���

#ifdef _DEBUG
void CViewAllStudents::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewAllStudents::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewAllStudents ��Ϣ�������

static BOOL CALLBACK Grid1Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewAllStudents* pThis = (CViewAllStudents*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas1.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas1.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas1[nRow][nCol];
			if (nCol == 8)
			{
				if (!pThis->m_datas1[nRow][nCol].IsEmpty())
					pdi->item.varValue = "�Ѵ���";
			}
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

	CViewAllStudents* pThis = (CViewAllStudents*)lp;

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
	}

	return TRUE;
}

static BOOL CALLBACK Grid3Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewAllStudents* pThis = (CViewAllStudents*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas3.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas3.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas3[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
	}

	return TRUE;
}

static BOOL CALLBACK Grid4Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewAllStudents* pThis = (CViewAllStudents*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas4.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas4.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas4[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
	}

	return TRUE;
}

static BOOL CALLBACK Grid5Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewAllStudents* pThis = (CViewAllStudents*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas5.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas5.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas5[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
	}

	return TRUE;
}

static BOOL CALLBACK GridMCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewAllStudents* pThis = (CViewAllStudents*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datasM.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datasM.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datasM[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
	}

	return TRUE;
}

static BOOL CALLBACK GridYCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewAllStudents* pThis = (CViewAllStudents*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datasY.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datasY.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datasY[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
	}

	return TRUE;
}


void CViewAllStudents::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	m_DateS.SetFormat("yyyy/MM/dd");
	m_DateE.SetFormat("yyyy/MM/dd");

	CRect rect;
	m_wndGridLocation.GetClientRect(&rect);
	m_wndGridLocation.MapWindowPoints(this, &rect);
	m_nGridWidth = rect.Width();

	m_TabCtrl.InsertItem(0, "����ͳ��");
	m_TabCtrl.InsertItem(1, "��Ŀһͳ��");
	m_TabCtrl.InsertItem(2, "��Ŀ��ͳ��");
	m_TabCtrl.InsertItem(3, "��Ŀ��ͳ��");
	m_TabCtrl.InsertItem(4, "��ѧͳ��");

	m_TabShowType.InsertItem(0, "����ͳ��");
	m_TabShowType.InsertItem(1, "����ͳ��");
	m_TabShowType.InsertItem(2, "����ͳ��");

	m_arrColumns1.push_back("��������");
	m_arrColumns1.push_back("����");
	m_arrColumns1.push_back("�Ա�");
	m_arrColumns1.push_back("�ֻ�");
	m_arrColumns1.push_back("����");
	m_arrColumns1.push_back("������");
	m_arrColumns1.push_back("��ѧ��ʱ��");
	m_arrColumns1.push_back("״̬");
	m_arrColumns1.push_back("����");
	InitGrid(&m_wndGrid[0], rect, m_arrColumns1);
	m_wndGrid[0].EnableVirtualMode(Grid1Callback, (LPARAM)this);

	m_arrColumns2.push_back("��������");
	m_arrColumns2.push_back("����");
	m_arrColumns2.push_back("������");
	m_arrColumns2.push_back("��������");
	m_arrColumns2.push_back("���Խ��");
	InitGrid(&m_wndGrid[1], rect, m_arrColumns2);
	m_wndGrid[1].EnableVirtualMode(Grid2Callback, (LPARAM)this);
	m_wndGrid[1].ShowWindow(FALSE);

	m_arrColumns3 = m_arrColumns2;
	InitGrid(&m_wndGrid[2], rect, m_arrColumns3);
	m_wndGrid[2].EnableVirtualMode(Grid3Callback, (LPARAM)this);
	m_wndGrid[2].ShowWindow(FALSE);

	m_arrColumns4 = m_arrColumns2;
	InitGrid(&m_wndGrid[3], rect, m_arrColumns4);
	m_wndGrid[3].EnableVirtualMode(Grid4Callback, (LPARAM)this);
	m_wndGrid[3].ShowWindow(FALSE);

	m_arrColumns5.push_back("��ѧ����");
	m_arrColumns5.push_back("����");
	m_arrColumns5.push_back("�Ա�");
	m_arrColumns5.push_back("�ֻ�");
	m_arrColumns5.push_back("����");
	m_arrColumns5.push_back("������");
	m_arrColumns5.push_back("��ѧԭ��");
	m_arrColumns5.push_back("��ѧ��ʱ��");
	m_arrColumns5.push_back("Ӧ�˽��");
	m_arrColumns5.push_back("ʵ�˽��");
	InitGrid(&m_wndGrid[4], rect, m_arrColumns5);
	m_wndGrid[4].EnableVirtualMode(Grid5Callback, (LPARAM)this);
	m_wndGrid[4].ShowWindow(FALSE);

	std::vector<CString> arrColumns;
	arrColumns.push_back("ͳ��ʱ��");
	InitGrid(&m_wndGridM, rect, arrColumns);
	InitGrid(&m_wndGridY, rect, arrColumns);
	m_wndGridM.EnableVirtualMode(GridMCallback, (LPARAM)this);
	m_wndGridY.EnableVirtualMode(GridYCallback, (LPARAM)this);
	m_wndGridM.ShowWindow(FALSE);
	m_wndGridY.ShowWindow(FALSE);

	m_nPieTotal = 0;
	m_nPiePart.clear();
	m_strPiePart.clear();
	m_Pie.GetClientRect(&m_rctPie);
	m_Pie.MapWindowPoints(this, &m_rctPie);
	m_Label.GetClientRect(&m_rctLabel);
	m_Label.MapWindowPoints(this, &m_rctLabel);

	m_Info.GetClientRect(&m_rctInfo);
	m_Info.MapWindowPoints(this, &m_rctInfo);

	m_canAnal = FALSE;
	m_threadProcess.StartThread();
}

void CViewAllStudents::InitGridMY(CVirtualGridCtrl* pGrid, std::vector<CString>& arrColumns)
{
	int nColumn = 0;
	int hw = pGrid->GetRowHeaderWidth();

	int nColumns = arrColumns.size();
	int w = m_nGridWidth - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		pGrid->InsertColumn(i, arrColumns[i], nColumnWidth);
		pGrid->SetColumnAlign(i, HDF_CENTER);
		pGrid->SetHeaderAlign(i, HDF_CENTER);
	}
}

void CViewAllStudents::InitGrid(CVirtualGridCtrl* pGrid, CRect& rect, std::vector<CString>& arrColumns)
{
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	pGrid->Create(nStyle, rect, this, IDC_GRID_STUPRO);
	pGrid->SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	pGrid->EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	pGrid->SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	pGrid->SetReadOnly();
	pGrid->SetWholeRowSel();
	pGrid->SetSingleSel(); //ֻ��ѡһ��
	pGrid->EnableRowHeader(TRUE);
	pGrid->EnableLineNumbers();

	int nColumn = 0;
	int hw = pGrid->GetRowHeaderWidth();

	int nColumns = arrColumns.size();
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		pGrid->InsertColumn(i, arrColumns[i], nColumnWidth);
		pGrid->SetColumnAlign(i, HDF_CENTER);
		pGrid->SetHeaderAlign(i, HDF_CENTER);
	}
}

void CViewAllStudents::Refresh(int nID, CString strDateS, CString strDateE)
{
	CString strSQL, strMsg;
	int size = 0;
	switch (nID)
	{
	case 0: //��������ͳ��
	{
				m_datas1.clear();
			  strSQL.Format("SELECT students.REGIST_DATE, students.SNAME, students.GENDER, \
							students.TEL, students.CAR_TYPE, students.FILE_NAME, students.CLASS_NUM, \
							students.STEP,  stuQuits.QUIT_DATE FROM students \
							left join stuQuits ON stuQuits.STU_ID=students.FILE_NAME \
							WHERE REGIST_DATE>'%s' AND REGIST_DATE<='%s' ORDER BY students.REGIST_DATE",
							strDateS, strDateE);
			  g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg);
			  ShowMsg2Output1(strMsg);
			  size = m_datas1.size();
			  m_dataSize[0] = size;

			  //����Ԥ����
			  for (int nRow = 0; nRow < size; nRow++)
			  {
				  int step = atoi(m_datas1[nRow][7]);
				  if (step < 1000)
					  m_datas1[nRow][7] = "����";
				  else if (step < 1002)
					  m_datas1[nRow][7] = "����ͨ��";
				  else
					  m_datas1[nRow][7] = "��;��ѧ";
			  }
	}
		break;
	case 1: //��Ŀһ����ͳ��
	{
				m_datas2.clear();
				strSQL.Format("SELECT stuDateHistory.EXAM_DATE, students.SNAME, students.FILE_NAME, \
							  stuDateHistory.EXAM_TYPE, stuDateHistory.EXAM_RESULT FROM stuDateHistory \
							  left join students ON stuDateHistory.STU_ID=students.FILE_NAME \
							  WHERE REGIST_DATE>'%s' AND REGIST_DATE<='%s' \
							  AND stuDateHistory.EXAM_TYPE='��Ŀһ'"
							  , strDateS, strDateE);
				g_mysqlCon.ExecuteQuery(strSQL, m_datas2, strMsg);
				ShowMsg2Output1(strMsg);
				size = m_datas2.size();
				m_dataSize[1] = size;


				//����Ԥ����
				for (int nRow = 0; nRow < size; nRow++)
				{
					int step = atoi(m_datas2[nRow][4]);
					if (step == 0)
						m_datas2[nRow][4] = "������";
					else if (step == 1)
						m_datas2[nRow][4] = "ͨ��";
					else
						m_datas2[nRow][4] = "δͨ��";
				}
	}
		break;
	case 2: //��Ŀ������ͳ��
	{
				m_datas3.clear();
				strSQL.Format("SELECT stuDateHistory.EXAM_DATE, students.SNAME, students.FILE_NAME, \
							  	stuDateHistory.EXAM_TYPE, stuDateHistory.EXAM_RESULT FROM stuDateHistory \
								left join students ON stuDateHistory.STU_ID=students.FILE_NAME \
								WHERE REGIST_DATE>'%s' AND REGIST_DATE<='%s' \
								AND stuDateHistory.EXAM_TYPE='��Ŀ��'"
								, strDateS, strDateE);
				g_mysqlCon.ExecuteQuery(strSQL, m_datas3, strMsg);
				ShowMsg2Output1(strMsg);
				size = m_datas3.size();
				m_dataSize[2] = size;

				//����Ԥ����
				for (int nRow = 0; nRow < size; nRow++)
				{
					int step = atoi(m_datas3[nRow][4]);
					if (step == 0)
						m_datas3[nRow][4] = "������";
					else if (step == 1)
						m_datas3[nRow][4] = "ͨ��";
					else
						m_datas3[nRow][4] = "δͨ��";
				}
	}
		break;
	case 3: //��Ŀ������ͳ��
	{
				m_datas4.clear();
				strSQL.Format("SELECT stuDateHistory.EXAM_DATE, students.SNAME, students.FILE_NAME, \
							  	stuDateHistory.EXAM_TYPE, stuDateHistory.EXAM_RESULT FROM stuDateHistory \
								left join students ON stuDateHistory.STU_ID=students.FILE_NAME \
								WHERE REGIST_DATE>'%s' AND REGIST_DATE<='%s' \
								AND stuDateHistory.EXAM_TYPE='��Ŀ��'"
								, strDateS, strDateE);
				g_mysqlCon.ExecuteQuery(strSQL, m_datas4, strMsg);
				ShowMsg2Output1(strMsg);
				size = m_datas4.size();
				m_dataSize[3] = size;

				//����Ԥ����
				for (int nRow = 0; nRow < size; nRow++)
				{
					int step = atoi(m_datas4[nRow][4]);
					if (step == 0)
						m_datas4[nRow][4] = "������";
					else if (step == 1)
						m_datas4[nRow][4] = "ͨ��";
					else
						m_datas4[nRow][4] = "δͨ��";
				}
	}
		break;
	case 4: //��ѧͳ��
	{
				m_datas5.clear();
				strSQL.Format("SELECT stuQuits.QUIT_DATE, students.SNAME, students.GENDER, students.TEL, \
							  students.CAR_TYPE, students.FILE_NAME, stuQuits.QUIT_REASON, students.CLASS_NUM, \
							  stuQuits.SHOULD_MONEY, stuQuits.RETURN_MONEY FROM stuQuits \
							  inner join students ON students.FILE_NAME=stuQuits.STU_ID\
							  WHERE QUIT_DATE>'%s' AND QUIT_DATE<='%s'",
							  strDateS, strDateE);
				g_mysqlCon.ExecuteQuery(strSQL, m_datas5, strMsg);
				ShowMsg2Output1(strMsg);
				size = m_datas5.size();
				m_dataSize[4] = size;
	}
		break;
	}

	m_wndGrid[nID].GridRefresh(size);
}

void CALLBACK CViewAllStudents::ThreadProcessCallback(LPVOID pParam, HANDLE hCloseEvent)
{
	CViewAllStudents* pThis = (CViewAllStudents*)pParam;

	while (WAIT_TIMEOUT == ::WaitForSingleObject(hCloseEvent, 1000))
	{
		if (!pThis->m_canAnal) continue;
		pThis->m_canAnal = FALSE;

		int pos0 = pThis->m_TabCtrl.GetCurSel();
		int pos1 = pThis->m_TabShowType.GetCurSel();

		//if (pos1==0) //����ͳ��
			switch (pos0)
			{
			case 0:
				pThis->Analysis(pos0, pos1, pThis->m_datas1, 7);
				break;
			case 1:
				pThis->Analysis(pos0, pos1, pThis->m_datas2, 4);
				break;
			case 2:
				pThis->Analysis(pos0, pos1, pThis->m_datas3, 4);
				break;
			case 3:
				pThis->Analysis(pos0, pos1, pThis->m_datas4, 4);
				break;
			}
		//else if (pos1 == 1) //����ͳ��
		//{

		//}
	}
}

void CViewAllStudents::Analysis(int nID, int YMD, CDStrs& datas, int cFlag)
{
	switch (YMD)
	{
	case 0: //����ͳ��
	{
				m_nPieTotal = 0;
				m_nPiePart.clear();
				m_strPiePart.clear();
			  m_nPieTotal = datas.size();
			  if (m_nPieTotal == 0) return;
			  std::map<CString, int> mapLabels;
			  for (int r = 0; r < m_nPieTotal; r++)
			  {
				  mapLabels[datas[r][cFlag]]++;
			  }
			  std::map<CString, int>::iterator it = mapLabels.begin();
			  for (; it != mapLabels.end(); it++)
			  {
				  m_nPiePart.push_back(it->second);
				  m_strPiePart.push_back(it->first);
			  }
	}
		break;
	case 1: //����ͳ��
	{
				m_datasM.clear();
				if (m_nPieTotal == 0)
				{
					PostMessage(WM_USER_UPDATE_VIEW, (WPARAM)2);
					return;
				}

				std::map<CString, int> mapLabels;
				int nColumns = m_nPiePart.size();
				//for (int c = 0; c < nColumns; c++)
				//	mapLabels[m_strPiePart[c]] = 0; //�����

				CString strM = datas[0][0].Left(7);
				int sumM = 0;
				for (int r = 0; r < m_nPieTotal; r++)
				{
					if (strM == datas[r][0].Left(7)) //ͬһ���µ�
					{
						mapLabels[datas[r][cFlag]] ++;
						sumM++;
					}
					else //��һ����
					{
						CStrs strs;
						strs.push_back(strM); //�·�
						strs.push_back("");
						strs[1].Format("%d", sumM); //����


						for (int c = 0; c < nColumns; c++) //����������
						{
							CString str;
							str.Format("%d", mapLabels[m_strPiePart[c]]);
							strs.push_back(str);
						}
						m_datasM.push_back(strs);

						strM = datas[r][0].Left(7);
						mapLabels.clear();
						mapLabels[datas[r][cFlag]]  = 1;
						sumM = 1;
					}
				}

				CStrs strs;
				strs.push_back(strM); //�·�
				strs.push_back("");
				strs[1].Format("%d", sumM); //����
				for (int c = 0; c < nColumns; c++) //����������
				{
					CString str;
					str.Format("%d", mapLabels[m_strPiePart[c]]);
					strs.push_back(str);
				}
				m_datasM.push_back(strs);

	}
		break;
	case 2: //����ͳ��
	{
				m_datasY.clear();
				if (m_nPieTotal == 0)
				{
					PostMessage(WM_USER_UPDATE_VIEW, (WPARAM)2);
					return;
				}

				std::map<CString, int> mapLabels;
				int nColumns = m_nPiePart.size();
				//for (int c = 0; c < nColumns; c++)
				//	mapLabels[m_strPiePart[c]] = 0; //�����

				CString strY = datas[0][0].Left(4);
				int sumY = 0;
				for (int r = 0; r < m_nPieTotal; r++)
				{
					if (strY == datas[r][0].Left(4)) //ͬһ���µ�
					{
						mapLabels[datas[r][cFlag]] ++;
						sumY++;
					}
					else //��һ����
					{
						CStrs strs;
						strs.push_back(strY); //���
						strs.push_back("");
						strs[1].Format("%d", sumY); //����


						for (int c = 0; c < nColumns; c++) //����������
						{
							CString str;
							str.Format("%d", mapLabels[m_strPiePart[c]]);
							strs.push_back(str);
						}
						m_datasY.push_back(strs);

						strY = datas[r][0].Left(7);
						mapLabels.clear();
						mapLabels[datas[r][cFlag]] = 1;
						sumY = 1;
					}
				}

				CStrs strs;
				strs.push_back(strY); //���
				strs.push_back("");
				strs[1].Format("%d", sumY); //����
				for (int c = 0; c < nColumns; c++) //����������
				{
					CString str;
					str.Format("%d", mapLabels[m_strPiePart[c]]);
					strs.push_back(str);
				}
				m_datasY.push_back(strs);

	}
		break;
	default:
		break;
	}

	PostMessage(WM_USER_UPDATE_VIEW, (WPARAM)2);
}


HBRUSH CViewAllStudents::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	//case IDC_S3:
	//case IDC_S4:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //����һ���ջ�ˢ
	}
		break;
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

Color colors[9] = {
	Color(96, 157, 202),
	Color(255, 150, 65),
	Color(56, 194, 193),
	Color(231, 138, 195),
	Color(184, 135, 195),
	Color(182, 115, 101),
	Color(254, 144, 194),
	Color(164, 160, 155),
	Color(210, 204, 90)
};
void CViewAllStudents::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	MapWindowPoints(this, &rect);

	m_Pie.GetClientRect(&m_rctPie);
	m_Pie.MapWindowPoints(this, &m_rctPie);
	m_Label.GetClientRect(&m_rctLabel);
	m_Label.MapWindowPoints(this, &m_rctLabel);

	m_Info.GetClientRect(&m_rctInfo);
	m_Info.MapWindowPoints(this, &m_rctInfo);

	//CDC     MemDC;
	////HBITMAP hbitmap;
	//CBitmap bitmp;
	//MemDC.CreateCompatibleDC(&dc);

	//int  W = GetSystemMetrics(SM_CXSCREEN);  //�õ���Ļ��� 
	//int  H = GetSystemMetrics(SM_CYSCREEN);
	//bitmp.CreateCompatibleBitmap(&dc, W, H);
	//MemDC.SelectObject(&bitmp);

	//Graphics graph(MemDC.m_hDC);
	Graphics graph(dc.m_hDC);

	//�����ȱ�ͼ
	SolidBrush brush3(Color(191, 211, 242));
	//SolidBrush brush2(Color(240, 243, 244));
	//Rect rctInfo(m_rctInfo.left, m_rctInfo.top, m_rctInfo.Height(), m_rctInfo.Height());
	//graph.FillRectangle(&brush2, rctInfo);
	Rect rctPie(m_rctPie.left, m_rctPie.top, m_rctPie.Height(), m_rctPie.Height());
	graph.FillEllipse(&brush3, rctPie);

	int n = m_nPiePart.size();
	double A0 = -90; 
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(51, 103, 155));
	CFont font1;
	font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
	dc.SelectObject(&font1);
	Gdiplus::Rect rctLabel(m_rctLabel.left, m_rctLabel.top, m_rctLabel.Width(), m_rctLabel.Height());
	int TextX = m_rctLabel.right + 3;
	for (int i = 0; i < n; i++)
	{
		if (m_nPieTotal == 0) break;

		int p = m_nPiePart[i];
		double sweepA = p * 100.0 / m_nPieTotal;
		CString strPercent;
		strPercent.Format("%s %.2f%%", m_strPiePart[i], sweepA);
		sweepA *= 3.600;
		SolidBrush brushI(colors[i]);
		graph.FillPie(&brushI, rctPie, A0, sweepA);
		A0 += sweepA;

		//ͼ��
		rctLabel.Offset(0, -m_rctLabel.Height()*1.5*i);
		dc.TextOutA(TextX, rctLabel.Y, strPercent);
		graph.FillRectangle(&brushI, rctLabel);
	}


	//CPoint pos = GetScrollPosition();
	////dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);
	//dc.TransparentBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, rect.Width(), rect.Height(), RGB(0, 0, 0));
	//bitmp.DeleteObject();
	//MemDC.DeleteDC();
}


void CViewAllStudents::OnBnClickedQuery()
{
	UpdateData();

	int type = m_TabCtrl.GetCurSel();
	Refresh(type, m_strDateS, m_strDateE);
	m_canAnal = TRUE;

	m_TabShowType.SetCurSel(0);
	m_wndGridM.ShowWindow(FALSE);
	m_wndGridY.ShowWindow(FALSE);

	InvalidateRect(&m_rctInfo);
}


void CViewAllStudents::OnBnClickedQuit()
{
	CBCGPGridRow* pRow = m_wndGrid[0].GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CDlgQuitSchool dlg;
		dlg.m_strInfo[1] = m_datas1[nRow][5];
		if (dlg.DoModal() == IDOK)
		{
			OnBnClickedQuery();
		}
	}
}


void CViewAllStudents::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int pos = m_TabCtrl.GetCurSel();

	for (int i = 0; i < 5; i++)
		m_wndGrid[i].ShowWindow(FALSE);

	m_TabShowType.SetCurSel(0);
	m_wndGridM.ShowWindow(FALSE);
	m_wndGridY.ShowWindow(FALSE);

	m_wndGrid[pos].ShowWindow(TRUE);
	m_wndGrid[pos].AdjustLayout();

	if (pos == 0)
	{
		GetDlgItem(IDC_QUIT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_QUIT)->EnableWindow(FALSE);
	}


	m_canAnal = TRUE;
	*pResult = 0;
}


void CViewAllStudents::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	int pos = m_TabShowType.GetCurSel();
	int pos0 = m_TabCtrl.GetCurSel();
	m_canAnal = TRUE;
	switch (pos)
	{
	case 0: //����
		m_wndGrid[pos0].ShowWindow(TRUE);
		m_wndGridM.ShowWindow(FALSE);
		m_wndGridY.ShowWindow(FALSE);
		m_wndGrid[pos0].AdjustLayout();
		if (pos0 == 0)
			GetDlgItem(IDC_QUIT)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_QUIT)->EnableWindow(FALSE);
		break;
	case 1: //����
		m_wndGrid[pos0].ShowWindow(FALSE);
		m_wndGridM.ShowWindow(TRUE);
		m_wndGridY.ShowWindow(FALSE);
		GetDlgItem(IDC_QUIT)->EnableWindow(FALSE);
		//m_wndGridM.DeleteAllColumns();
		//arrColumns.push_back("ͳ���·�");
		//InitGridMY(&m_wndGridM, arrColumns);
		//m_wndGridM.AdjustLayout();
		break;
	case 2: //����
		m_wndGrid[pos0].ShowWindow(FALSE);
		m_wndGridM.ShowWindow(FALSE);
		m_wndGridY.ShowWindow(TRUE);
		GetDlgItem(IDC_QUIT)->EnableWindow(FALSE);
		//m_wndGridY.DeleteAllColumns();
		//arrColumns.push_back("ͳ�����");
		//InitGridMY(&m_wndGridY, arrColumns);
		//m_wndGridY.AdjustLayout();
		break;
	}

	*pResult = 0;
}


void CViewAllStudents::OnBnClickedExport2()
{
	int pos1 = m_TabShowType.GetCurSel();
	int pos0 = m_TabCtrl.GetCurSel();

	CString strT1[] = { "����ͳ��", "��Ŀһͳ��", "��Ŀ��ͳ��", "��Ŀ��ͳ��", "��ѧͳ��" };
	CString strT2[] = { "����", "����", "����" };
	CString strFileName = strT1[pos0] + strT2[pos1] + m_strDateS + "-" + m_strDateE + ".csv"; //g_strOutPath + "\\" + 
	strFileName.Replace('/', '_');

	if (pos1==0)
		switch (pos0)
	{
		case 0:
			ExportExcel(strFileName, m_arrColumns1, m_datas1);
			break;
		case 1:
			ExportExcel(strFileName, m_arrColumns2, m_datas2);
			break;
		case 2:
			ExportExcel(strFileName, m_arrColumns3, m_datas3);
			break;
		case 3:
			ExportExcel(strFileName, m_arrColumns4, m_datas4);
			break;
		case 4:
			ExportExcel(strFileName, m_arrColumns5, m_datas5);
			break;
	}
	else if (pos1 == 1)
	{
		std::vector<CString> arrColumns;
		arrColumns.push_back("ͳ���·�");
		arrColumns.push_back("ͳ������");
		int n = m_nPiePart.size();
		for (int i = 0; i < n; i++)
			arrColumns.push_back(m_strPiePart[i]);
		ExportExcel(strFileName, arrColumns, m_datasM);
	}
	else if (pos1 == 2)
	{
		std::vector<CString> arrColumns;
		arrColumns.push_back("ͳ�����");
		arrColumns.push_back("ͳ������");
		int n = m_nPiePart.size();
		for (int i = 0; i < n; i++)
			arrColumns.push_back(m_strPiePart[i]);
		ExportExcel(strFileName, arrColumns, m_datasY);
	}
}
