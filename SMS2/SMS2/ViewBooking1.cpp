// ViewBooking1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewBooking1.h"
#include "MainFrm.h"


// CViewBooking1

IMPLEMENT_DYNCREATE(CViewBooking1, CBCGPFormView)

CViewBooking1::CViewBooking1()
	: CBCGPFormView(CViewBooking1::IDD)
{
	EnableVisualManagerStyle();
}

CViewBooking1::~CViewBooking1()
{
}

void CViewBooking1::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIST, m_wndGridLocation);
	DDX_Control(pDX, IDC_WEEK, m_SWeek);
	DDX_Control(pDX, IDC_SPHOTO, m_SPhoto);
}

BEGIN_MESSAGE_MAP(CViewBooking1, CBCGPFormView)
//	ON_BN_CLICKED(IDC_BUTTON1, &CViewBooking1::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
//	ON_BN_CLICKED(IDC_BUTTON1, &CViewBooking1::OnBnClickedButton1)
END_MESSAGE_MAP()


// CViewBooking1 ���

#ifdef _DEBUG
void CViewBooking1::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewBooking1::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewBooking1* pThis = (CViewBooking1*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
	}

	return TRUE;
}
// CViewBooking1 ��Ϣ�������

void CALLBACK CViewBooking1::OnCalendarClick(LPVOID lParam, BOOL lParam2)
{
	CViewBooking1* pThis = (CViewBooking1*)lParam;


}

void CViewBooking1::UpdateCalendar()
{
	CBCGPGridRow* pRow = m_wndCalendar.GetRow(1);
	CBCGPGridItem* pItem = pRow->GetItem(1);
	pItem->SetBackgroundColor(RGB(0, 117, 194));
	pItem->SetTextColor(RGB(255, 255, 255));
}

void CALLBACK CViewBooking1::OnGridClick(LPVOID lParam)
{
	CViewBooking1* pThis = (CViewBooking1*)lParam;

	CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CString strFileName = pThis->m_datas[nRow][0]; //������
		CString strName = pThis->m_datas[nRow][1]; //����

		pThis->GetDlgItem(IDC_NAME)->SetWindowText(strName);

		//���ش���Ƭ���������ޣ����ѯ����������
		ShowMsg2Output1("ѡ��ԤԼ���󣺵���" + strFileName);
		CString strFile;
		strFile.Format("%s\\%s.bmp", g_strFilePath, strFileName);
		char* file = strFile.GetBuffer();
		pThis->m_img = cv::imread(file);
		strFile.ReleaseBuffer();
		if (pThis->m_img.empty()) //��������Ƭ���ӷ���������
		{
			//���ݴ��
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame->m_pSendBuf != NULL)
			{
				pThis->MessageBox("��һ����Ϣ��δ������ϣ����Ե����ԡ�");
			}
			else
			{
				int len = 11; //
				pFrame->m_isSendReady = FALSE;
				pFrame->m_pSendBuf = new BYTE[len];//������ɾ��
				pFrame->m_nSendLen = len;
				pFrame->m_pSendBuf[0] = 3; //����ͼ������
				char* cID = strFileName.GetBuffer();
				memcpy(pFrame->m_pSendBuf + 1, cID, 10); //������
				strFileName.ReleaseBuffer();
				pFrame->m_isSendReady = TRUE;
			}
		}

		pThis->m_SPhoto.Invalidate();
	}
}

void CViewBooking1::OnInitialUpdate()
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
	m_wndGrid.SetSingleSel(); //ֻ��ѡһ��
	m_wndGrid.EnableRowHeader(TRUE);
	m_wndGrid.EnableLineNumbers();


	int nColumn = 0;
	int hw = m_wndGrid.GetRowHeaderWidth();
	m_wndGrid.InsertColumn(nColumn++, _T("������"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("����"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("�Ա�"), 50);
	m_wndGrid.InsertColumn(nColumn++, _T("��������"), rectGrid.Width() - hw - 190);
	for (int i = 0; i < nColumn; i++)
	{
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);
	m_wndGrid.SetCallBack_Clk(OnGridClick);
	Refresh();

	m_SPhoto.InitPicSource(&m_img); //ע��ͼƬ�ؼ�����Դ


	//�������ڵ�Ԫ��
	CRect rectWeek;
	m_SWeek.GetClientRect(&rectWeek);
	m_SWeek.MapWindowPoints(this, &rectWeek); //תΪ��������

	m_wndCalendar.Create(nStyle, rectWeek, this, IDC_GRID_STUPRO+1);
	//m_wndCalendar.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //���ñ߿�
	m_wndCalendar.EnableHeader(TRUE, 0); //�������ͷ�ƶ�
	// Set grid tab order (first):
	m_wndCalendar.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndCalendar.SetReadOnly();
	//m_wndCalendar.SetWholeRowSel();
	m_wndCalendar.SetSingleSel(); //ֻ��ѡһ��
	//m_wndCalendar.EnableRowHeader(TRUE);
	//m_wndCalendar.EnableLineNumbers();
	m_wndCalendar.SetCallBack_Clk(OnCalendarClick);
	CFont font;
	VERIFY(font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_LIGHT,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename
	//m_wndCalendar.SetFont(&font);

	LPCTSTR arrColumns[] = { _T("������"), _T("����һ"), _T("���ڶ�"), _T("������"), _T("������"), _T("������"), _T("������") };
	const int nColumns = sizeof (arrColumns) / sizeof (LPCTSTR);
	int rowheaderW = m_wndCalendar.GetRowHeaderWidth();
	int nColumnWidth = (rectWeek.Width() - rowheaderW - 35) / 7;
	for (nColumn = 0; nColumn < nColumns; nColumn++)
	{
		m_wndCalendar.InsertColumn(nColumn, arrColumns[nColumn], nColumnWidth);
		m_wndCalendar.SetHeaderAlign(nColumn, HDF_CENTER);
		m_wndCalendar.SetColumnAlign(nColumn, HDF_CENTER); 
	}

	for (int nRow = 0; nRow < 15; nRow++)
	{
		CBCGPGridRow* pRow = m_wndCalendar.CreateMultiLineRow(2);
		pRow->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
		CString strText;
		strText.Format("2017/04/17");
		pRow->GetItem(1)->SetValue((LPCTSTR)strText);
		m_wndCalendar.AddRow(pRow, FALSE);

		CBCGPGridRow* pRow1 = m_wndCalendar.CreateMultiLineRow(5);
		pRow1->SetVertAlign(BCGP_GRID_ITEM_VCENTER);
		strText.Format("��ԤԼ��\n\n5");
		pRow1->GetItem(1)->SetValue((LPCTSTR)strText);
		m_wndCalendar.AddRow(pRow1, FALSE);
	}
	m_wndCalendar.AdjustLayout();
	UpdateCalendar();
	//CRuntimeClass* pClass = RUNTIME_CLASS(CWorkDay);
	//if (pClass != NULL)
	//{
	//	m_pWndWorkDay = DYNAMIC_DOWNCAST(CView, pClass->CreateObject());
	//	if (m_pWndWorkDay != NULL)
	//	{
	//		m_pWndWorkDay->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rectWeek, this, 0, NULL);
	//	}
	//}
	//m_pWndWorkDay->OnInitialUpdate();
}

void CViewBooking1::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT FILE_NAME, SNAME, GENDER, CAR_TYPE FROM students WHERE STEP='%d'", SP_K2K3BOOKING);
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("��ѯѧ����Ϣ�ɹ�");
	}
	else ShowMsg2Output1(strMsg);


	m_wndGrid.GridRefresh(m_datas.size());
}

void CViewBooking1::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	//if (IsWindow(GetDlgItem(IDC_BUTTON1)->GetSafeHwnd())) //�жϿؼ������Ƿ��Ѿ�����
	//{
	//	CRect rect, rect_btn;
	//	GetClientRect(&rect);
	//	GetDlgItem(IDC_BUTTON1)->GetClientRect(&rect_btn);
	//	GetDlgItem(IDC_BUTTON1)->MapWindowPoints(this, &rect_btn);

	//	GetDlgItem(IDC_BUTTON1)->MoveWindow(rect.right / 2, rect_btn.top, rect_btn.Width(), rect_btn.Height());
	//}
}


void CViewBooking1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
}

LRESULT CViewBooking1::OnUserMessage(WPARAM wp, LPARAM lp)
{
	cv::Mat* pImg = (cv::Mat*)wp;
	m_img = pImg->clone();
	m_SPhoto.Invalidate();
	return 0;
}

void CViewBooking1::UpdateDB() //��ѯ��ǰ����ԤԼʱ�䣬���С��90�죬���½���¼
{
	CTime t = CTime::GetCurrentTime();
	t += CTimeSpan(90, 0, 0, 0);    //�ú�����һ������Ϊ��������  
	int nYear = t.GetYear();
	int nMonth = t.GetMonth();
	int nDay = t.GetDay();
}