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
	DDX_Control(pDX, IDC_SPHOTO, m_SPhoto);
}

BEGIN_MESSAGE_MAP(CViewBooking1, CBCGPFormView)
//	ON_BN_CLICKED(IDC_BUTTON1, &CViewBooking1::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
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
	m_wndGrid.EnableHeader(TRUE, BCGP_GRID_HEADER_MOVE_ITEMS); //�����ͷ�ƶ����ǲ���ɾ����ͷ
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	m_wndGrid.SetSingleSel(); //ֻ��ѡһ��

	int nColumn = 0;
	m_wndGrid.InsertColumn(nColumn++, _T("������"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("����"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("�Ա�"), 50);
	m_wndGrid.InsertColumn(nColumn++, _T("��������"), 80);
	for (int i = 0; i < nColumn; i++)
	{
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//ע�������б�ص�����
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);
	m_wndGrid.SetCallBack_Clk(OnGridClick);
	Refresh();

	m_SPhoto.InitPicSource(&m_img);
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

	//CRect rect;
	//m_SPhoto.GetClientRect(&rect);
	//if (!m_img.empty())
	//{
	//	CDC* pDc = m_SPhoto.GetDC();
	//	HDC hdc = pDc->GetSafeHdc();

	//	IplImage* frame;
	//	frame = &IplImage(m_img);
	//	CvvImage cvvImage;
	//	cvvImage.CopyOf(frame);
	//	cvvImage.DrawToHDC(hdc, rect);
	//}
	//else
	//{
	//	dc.TextOutA(rect.CenterPoint().x, rect.CenterPoint().y, "����Ƭ");
	//}
}

LRESULT CViewBooking1::OnUserMessage(WPARAM wp, LPARAM lp)
{
	cv::Mat* pImg = (cv::Mat*)wp;
	m_img = pImg->clone();
	m_SPhoto.Invalidate();
	return 0;
}