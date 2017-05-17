// ViewAllStudents.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewAllStudents.h"


// CViewAllStudents

IMPLEMENT_DYNCREATE(CViewAllStudents, CBCGPFormView)

CViewAllStudents::CViewAllStudents()
	: CBCGPFormView(CViewAllStudents::IDD)
{
	EnableVisualManagerStyle();
}

CViewAllStudents::~CViewAllStudents()
{
}

void CViewAllStudents::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_GRID, m_wndGridLocation);

	DDX_Control(pDX, IDC_PIE, m_Pie);
	DDX_Control(pDX, IDC_LABEL1, m_Label[0]);
	DDX_Control(pDX, IDC_LABEL2, m_Label[1]);


	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);

	DDX_Control(pDX, IDC_INFO, m_Info);

	DDX_Text(pDX, IDC_S1, m_strLabel[0]);
	DDX_Text(pDX, IDC_S2, m_strLabel[1]);


	DDX_Control(pDX, IDC_DATE1, m_DateS);
	DDX_Control(pDX, IDC_DATE2, m_DateE);
	DDX_Text(pDX, IDC_DATE1, m_strDateS);
	DDX_Text(pDX, IDC_DATE2, m_strDateE);
}

BEGIN_MESSAGE_MAP(CViewAllStudents, CBCGPFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CViewAllStudents::OnTcnSelchangeTab1)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_QUERY, &CViewAllStudents::OnBnClickedQuery)
	ON_BN_CLICKED(IDC_QUIT, &CViewAllStudents::OnBnClickedQuit)
END_MESSAGE_MAP()


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
			if (nCol == 7)
			{
				int step = atoi(pThis->m_datas1[nRow][nCol]);
				if (step < 1000)
					pdi->item.varValue = "����";
				else if (step < 1002)
					pdi->item.varValue = "����ͨ��";
				else
					pdi->item.varValue = "��;��ѧ";
			}
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
			if (nCol == 8)
			{
				if (pThis->m_datas5[nRow][nCol] != "0")
					pdi->item.varValue = "�Ѵ���";
				else
					pdi->item.varValue = "";
			}
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

	m_TabCtrl.InsertItem(0, "����ͳ��");
	m_TabCtrl.InsertItem(1, "��Ŀһͳ��");
	m_TabCtrl.InsertItem(2, "��Ŀ��ͳ��");
	m_TabCtrl.InsertItem(3, "��Ŀ��ͳ��");
	m_TabCtrl.InsertItem(4, "��ѧͳ��");


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
	m_wndGrid[1].EnableWindow(FALSE);

	m_arrColumns3 = m_arrColumns2;
	InitGrid(&m_wndGrid[2], rect, m_arrColumns3);
	m_wndGrid[2].EnableVirtualMode(Grid3Callback, (LPARAM)this);
	m_wndGrid[2].EnableWindow(FALSE);

	m_arrColumns4 = m_arrColumns2;
	InitGrid(&m_wndGrid[3], rect, m_arrColumns4);
	m_wndGrid[3].EnableVirtualMode(Grid4Callback, (LPARAM)this);
	m_wndGrid[3].EnableWindow(FALSE);

	m_arrColumns5.push_back("��ѧ����");
	m_arrColumns5.push_back("����");
	m_arrColumns5.push_back("�Ա�");
	m_arrColumns5.push_back("�ֻ�");
	m_arrColumns5.push_back("����");
	m_arrColumns5.push_back("������");
	m_arrColumns5.push_back("��ѧԭ��");
	m_arrColumns5.push_back("��ѧ��ʱ��");
	m_arrColumns5.push_back("�Ƿ����˿�");
	InitGrid(&m_wndGrid[4], rect, m_arrColumns5);
	m_wndGrid[4].EnableVirtualMode(Grid5Callback, (LPARAM)this);
	m_wndGrid[4].EnableWindow(FALSE);

	m_nPieTotal = 100;
	m_nPiePart.clear();
	m_nPiePart.push_back(10);
	m_nPiePart.push_back(20);
	m_nPiePart.push_back(30);
	m_nPiePart.push_back(40);
	m_Pie.GetClientRect(&m_rctPie);
	m_Pie.MapWindowPoints(this, &m_rctPie);
	m_Label[0].GetClientRect(&m_rctLabel[0]);
	m_Label[0].MapWindowPoints(this, &m_rctLabel[0]);
	m_Label[1].GetClientRect(&m_rctLabel[1]);
	m_Label[1].MapWindowPoints(this, &m_rctLabel[1]);

	m_Info.GetClientRect(&m_rctInfo);
	m_Info.MapWindowPoints(this, &m_rctInfo);
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
							WHERE REGIST_DATE>'%s' AND REGIST_DATE<='%s'",
							strDateS, strDateE);
			  g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg);
			  ShowMsg2Output1(strMsg);
			  size = m_datas1.size();
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
	}
		break;
	case 4: //��ѧͳ��
	{
				m_datas5.clear();
				strSQL.Format("SELECT stuQuits.QUIT_DATE, students.SNAME, students.GENDER, students.TEL, \
							  students.CAR_TYPE, students.FILE_NAME, stuQuits.QUIT_REASON, students.CLASS_NUM, \
							  stuQuits.RETURN_MONEY FROM stuQuits \
							  inner join students ON students.FILE_NAME=stuQuits.STU_ID\
							  WHERE QUIT_DATE>'%s' AND QUIT_DATE<='%s'",
							  strDateS, strDateE);
				g_mysqlCon.ExecuteQuery(strSQL, m_datas5, strMsg);
				ShowMsg2Output1(strMsg);
				size = m_datas5.size();
	}
		break;
	}

	m_wndGrid[nID].GridRefresh(size);
}

void CViewAllStudents::Analysis(int nID)
{
	m_nPieTotal = 0;
	switch (nID)
	{
	case 0:
	{
			  m_nPieTotal = m_datas1.size();
	}
		break;
	default:
		break;
	}
}

void CViewAllStudents::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int pos = m_TabCtrl.GetCurSel();

	for (int i = 0; i < 5; i++)
		m_wndGrid[i].EnableWindow(FALSE);

	m_wndGrid[pos].EnableWindow(TRUE);
	m_wndGrid[pos].AdjustLayout();

	if (pos == 0)
	{
		GetDlgItem(IDC_QUIT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_QUIT)->EnableWindow(FALSE);
	}

	*pResult = 0;
}


HBRUSH CViewAllStudents::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S3:
	case IDC_S4:
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

	Graphics graph(dc.m_hDC);

	//�����ȱ�ͼ
	SolidBrush brush2(Color(156, 187, 232));
	SolidBrush brush3(Color(191, 211, 242));
	Rect rctPie(m_rctPie.left, m_rctPie.top, m_rctPie.Height(), m_rctPie.Height());
	graph.FillEllipse(&brush3, rctPie);

	int n = m_nPiePart.size();
	double sweepA, A0 = -90;
	for (int i = 0; i < n; i++)
	{
		int p = m_nPiePart[i];
		double sweepA = p * 360.0 / m_nPieTotal;
		SolidBrush brushI(colors[i]);
		graph.FillPie(&brushI, rctPie, A0, sweepA);
		A0 += sweepA;
	}

	graph.FillRectangle(&brush3, m_rctLabel[0].left, m_rctLabel[0].top, m_rctLabel[0].Width(), m_rctLabel[0].Height());
	graph.FillRectangle(&brush2, m_rctLabel[1].left, m_rctLabel[1].top, m_rctLabel[1].Width(), m_rctLabel[1].Height());

}


void CViewAllStudents::OnBnClickedQuery()
{
	UpdateData();

	int type = m_TabCtrl.GetCurSel();
	Refresh(type, m_strDateS, m_strDateE);
}


void CViewAllStudents::OnBnClickedQuit()
{
	CBCGPGridRow* pRow = m_wndGrid[0].GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
	}
}
