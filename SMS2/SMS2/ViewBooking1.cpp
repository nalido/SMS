// ViewBooking1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewBooking1.h"
#include "MainFrm.h"
#include "ClassDetail.h"

CString arrClasses[5] = { "0δ����", "Am08:00-10:00", "Am10:00-12:00", "Pm01:00-03:00", "Pm03:00-05:00" };
// CViewBooking1

IMPLEMENT_DYNCREATE(CViewBooking1, CBCGPFormView)

CViewBooking1::CViewBooking1()
	: CBCGPFormView(CViewBooking1::IDD)
{
	EnableVisualManagerStyle();

	m_strFileName = "δѡ��";
	m_strName = "δѡ��";
	m_strGender = "δ֪";
	m_strCarType = "δ֪";
	m_strBooked = "NULL";
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

	DDX_Text(pDX, IDC_FILE, m_strFileName);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_GENDER, m_strGender);
	DDX_Text(pDX, IDC_CARTYPE, m_strCarType);
	DDX_Text(pDX, IDC_NCLASS, m_strBooked);
}

BEGIN_MESSAGE_MAP(CViewBooking1, CBCGPFormView)
//	ON_BN_CLICKED(IDC_BUTTON1, &CViewBooking1::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
//	ON_BN_CLICKED(IDC_BUTTON1, &CViewBooking1::OnBnClickedButton1)
ON_BN_CLICKED(IDC_STUDENT_SEL, &CViewBooking1::OnBnClickedStudentSel)
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
			if (nCol == 1)
			{
				int n = atoi(pThis->m_datas[nRow][nCol]);
				
				pdi->item.varValue = arrClasses[n];
			}
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
	if (!lParam2) return;

	CViewBooking1* pThis = (CViewBooking1*)lParam;
	CBCGPGridItem* pItem = pThis->m_wndCalendar.GetCurSelItem();
	CBCGPGridItemID item = pItem->GetGridItemID();
	
	CTime aDay = pThis->m_wndCalendar.GetSelectedDay(item.m_nRow, item.m_nColumn);

	CString strMsg;
	if (aDay < pThis->m_wndCalendar.m_tToday) 
	{
		pThis->MessageBox("ʱ���ڿ�ԤԼ��Χ֮�⣡");
		return;
	}
	
	CClassDetail dlg;
	int r = item.m_nRow / 2;
	int c = item.m_nColumn;
	dlg.m_nStatus[0] = pThis->m_wndCalendar.m_nStatus[r][c][0];
	dlg.m_nStatus[1] = pThis->m_wndCalendar.m_nStatus[r][c][1];
	dlg.m_nStatus[2] = pThis->m_wndCalendar.m_nStatus[r][c][2];
	dlg.m_nStatus[3] = pThis->m_wndCalendar.m_nStatus[r][c][3];
	dlg.m_strDay = aDay.Format("%Y/%m/%d");;
	if (dlg.DoModal() == IDOK)
	{
		int selected = dlg.m_nSelected;
		if (selected != 0)
		{
			pThis->AddNewBooking(dlg.m_strDay, selected / 1000);
			pThis->AddNewBooking(dlg.m_strDay, (selected % 1000 / 100) ? 2 : 0);
			pThis->AddNewBooking(dlg.m_strDay, (selected % 100 / 10) ? 3 : 0);
			pThis->AddNewBooking(dlg.m_strDay, (selected % 10) ? 4 : 0);
			pThis->m_wndCalendar.DrawSelectedItem(r, c);
		}
	}
}

void CViewBooking1::AddNewBooking(CString day, int classID)
{
	if (classID == 0) return;

	CString strtmp;
	strtmp.Format("%d", classID);
	CStrs strs;
	strs.push_back(day);
	strs.push_back(strtmp);
	strs.push_back("0");
	m_datas.push_back(strs);
	m_wndGrid.GridRefresh(m_datas.size());

	//������ݿ�
	if (m_strFileName == "δѡ��") return;
	CString strMsg;
	CString strSQL;
	strSQL.Format("INSERT INTO bookings(FILE_NAME, BOOKDATE, CLASS_ID, STEP) VALUES ('%s', '%s', '%d', '0')", m_strFileName, day, classID);
}

void CALLBACK CViewBooking1::OnGridClick(LPVOID lParam)
{
	CViewBooking1* pThis = (CViewBooking1*)lParam;

	CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
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
	m_wndGrid.InsertColumn(nColumn++, _T("ԤԼ����"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("ѵ��ʱ��"), 100);
	m_wndGrid.InsertColumn(nColumn++, _T("���״̬"), rectGrid.Width() - hw - 170);
	for (int i = 0; i < nColumn; i++)
	{
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);
	m_wndGrid.SetCallBack_Clk(OnGridClick);

	m_SPhoto.InitPicSource(&m_img); //ע��ͼƬ�ؼ�����Դ


	//�������ڵ�Ԫ��
	CRect rectWeek;
	m_SWeek.GetClientRect(&rectWeek);
	m_SWeek.MapWindowPoints(this, &rectWeek); //תΪ��������

	m_wndCalendar.Create(nStyle, rectWeek, this, IDC_GRID_STUPRO+1);
	m_wndCalendar.InitGrid(rectWeek.Width());
	m_wndCalendar.SetCallBack_Clk(OnCalendarClick);


	Refresh(); //ˢ���б�

}

void CViewBooking1::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT BOOK_DATE, CLASS_ID, STEP FROM bookings WHERE FILE_NAME='%s'", m_strFileName);
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("��ѯԤԼ��Ϣ�ɹ�");
		m_strBooked.Format("%d", m_datas.size());
	}
	else ShowMsg2Output1(strMsg);


	m_wndGrid.GridRefresh(m_datas.size());
	m_wndCalendar.UpdateGrid();
	//m_wndCalendar.DrawSelectedItem(3, 3);
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
	int flag = (int)lp;
	if (flag == 2)
	{
		cv::Mat* pImg = (cv::Mat*)wp;
		m_img = pImg->clone();
		m_SPhoto.Invalidate();
	}
	else if (flag == 1)
	{
		STUDENTINFO* pInfo = (STUDENTINFO*)wp;
		m_strName = pInfo->strName;
		m_strGender = pInfo->strGender;
		m_strCarType = pInfo->strCarType;
		m_strFileName = pInfo->strFileName;
		UpdateData(FALSE);


		//���ش���Ƭ���������ޣ����ѯ����������
		ShowMsg2Output1("ѡ��ԤԼ���󣺵���" + m_strFileName);
		CString strFile;
		strFile.Format("%s\\%s.bmp", g_strFilePath, m_strFileName);
		char* file = strFile.GetBuffer();
		m_img = cv::imread(file);
		strFile.ReleaseBuffer();
		if (m_img.empty()) //��������Ƭ���ӷ���������
		{
			//���ݴ��
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame->m_pSendBuf != NULL)
			{
				MessageBox("��һ����Ϣ��δ������ϣ����Ե����ԡ�");
			}
			else
			{
				int len = 11; //
				pFrame->m_isSendReady = FALSE;
				pFrame->m_pSendBuf = new BYTE[len];//������ɾ��
				pFrame->m_nSendLen = len;
				pFrame->m_pSendBuf[0] = 3; //����ͼ������
				char* cID = m_strFileName.GetBuffer();
				memcpy(pFrame->m_pSendBuf + 1, cID, 10); //������
				m_strFileName.ReleaseBuffer();
				pFrame->m_isSendReady = TRUE;
			}
		}
		else
			m_SPhoto.Invalidate();
	}

	return 0;
}


void CViewBooking1::OnBnClickedStudentSel()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SelectView(VIEW_STUPROGRESS);
}
