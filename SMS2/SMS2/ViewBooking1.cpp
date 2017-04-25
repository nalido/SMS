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
ON_BN_CLICKED(IDC_CONFIRM, &CViewBooking1::OnBnClickedConfirm)
ON_BN_CLICKED(IDC_REMOVE, &CViewBooking1::OnBnClickedRemove)
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

			//ԤԼ����С�ڽ���Ļһ�
			CTime aday = pThis->m_wndCalendar.Str2Time(pThis->m_datas[nRow][0]);
			CTime today = pThis->m_wndCalendar.m_tToday;
			if (aday < today)
			{
				if (pThis->m_datas[nRow][2] != "0") //�������ԤԼ�Ļһ�
				{
					pdi->item.clrBackground = RGB(140, 140, 140);
				}
				else //ԤԼ��δ�μ�ѵ���� ����ɫ��ʾ
				{
					pdi->item.clrBackground = RGB(240, 140, 140);
				}
				pdi->item.clrText = RGB(220, 220, 220);
			}

			//�Ͽ�ʱ��
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
		ShowMsg2Output1("ʱ���ڿ�ԤԼ��Χ֮�⣡");
		return;
	}
	
	CClassDetail dlg;
	int r = item.m_nRow / 2;
	int c = item.m_nColumn;
	dlg.m_nStatus[0][0] = pThis->m_wndCalendar.m_nStatus[r][c][0];
	dlg.m_nStatus[1][0] = pThis->m_wndCalendar.m_nStatus[r][c][1];
	dlg.m_nStatus[2][0] = pThis->m_wndCalendar.m_nStatus[r][c][2];
	dlg.m_nStatus[3][0] = pThis->m_wndCalendar.m_nStatus[r][c][3];

	//���ݵ�ǰѧԱ��ѡ�����
	int ss = pThis->m_wndCalendar.m_nStatus[r][c][5];
	if (ss / 1000) dlg.m_nStatus[0][1] = 1;
	if (ss % 1000 / 100) dlg.m_nStatus[1][1] = 1;
	if (ss % 100 / 10) dlg.m_nStatus[2][1] = 1;
	if (ss % 10) dlg.m_nStatus[3][1] = 1;
	CString strLog;
	strLog.Format("OnClick:\r\ndata:%s\r\nnclass:%d", aDay.Format("%Y/%m/%d"), pThis->m_wndCalendar.m_nStatus[r][c][5]);
	LOG("booking1Log.log", strLog);

	dlg.m_strDay = aDay.Format("%Y/%m/%d");;
	if (dlg.DoModal() == IDOK)
	{
		int selected = dlg.m_nSelected;
		//pThis->m_wndCalendar.m_nStatus[r][c][5] += selected;
		if (selected != 0)
		{
			pThis->JudgeSelected(selected, 1, dlg.m_strDay, r, c);
			pThis->JudgeSelected(selected, 2, dlg.m_strDay, r, c);
			pThis->JudgeSelected(selected, 3, dlg.m_strDay, r, c);
			pThis->JudgeSelected(selected, 4, dlg.m_strDay, r, c);
		}
	}
}

void CViewBooking1::JudgeSelected(int selected, int nclass, CString aday, int r, int c)
{
	int n = 5 - nclass;
	n = exp(n);
	int sel = selected / n % 10; 
	if (sel == 1)
	{
		if (m_datas.size() == g_nMaxBooking)
		{
			CString str;
			str.Format("����ԤԼȫ��%d�ڿΣ����ܼ���ԤԼ", g_nMaxBooking);
			MessageBox(str);
		}
		else
		{
			m_wndCalendar.m_nStatus[r][c][nclass - 1] += 1;
			m_wndCalendar.m_nStatus[r][c][5] += n;
			AddNewBooking(aday, nclass);
			m_wndCalendar.DrawSelectedItem(r, c);
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
	strs.push_back("0"); //���һλ��ʾδ�����ݿ�
	m_datas.push_back(strs);
	m_wndGrid.GridRefresh(m_datas.size());
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
	m_wndCalendar.UpdateGrid();
	UpdateBookingList();
}

void CViewBooking1::UpdateBookingList()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT BOOK_DATE, CLASS_ID, FLAG FROM bookings WHERE FILE_NAME='%s' ORDER BY BOOK_DATE", m_strFileName);
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("��ѯԤԼ��Ϣ�ɹ�");
		int n = m_datas.size();
		m_strBooked.Format("%d", n);
		CString state = "1"; //���һ�У������У���ʾ�Ѿ���¼�����ݿ⣬�����ظ��ύ���ݿ�
		for (int i = 0; i < n; i++)
		{
			m_datas[i].push_back(state);

			//��������ʾ��ѡ������
			CTime aday = m_wndCalendar.Str2Time(m_datas[i][0]);
			CTime today = m_wndCalendar.m_tToday;
			if (aday >= today)
			{
				CPoint pos = m_wndCalendar.GetDayPos(m_datas[i][0]);
				int nclass = 5 - atoi(m_datas[i][1]);
				nclass = exp(nclass);
				m_wndCalendar.m_nStatus[pos.y][pos.x][5] += nclass;
				m_wndCalendar.m_nStatus[pos.y][pos.x][4] = 1;
				//m_wndCalendar.DrawSelectedItem(pos);

				//CString str;
				//str.Format("\r\ndate:%s\r\nclassID:%s\r\npos:%d,%d\r\nnclass:%d\r\n",
				//	m_datas[i][0], m_datas[i][1], pos.x, pos.y, m_wndCalendar.m_nStatus[pos.y][pos.x][5]);
				//LOG("booking1Log.log", str);
				//TRACE0(str);
			}
		} //end for
		m_wndCalendar.DrawSelectedItems();
	}
	else ShowMsg2Output1(strMsg);


	m_wndGrid.GridRefresh(m_datas.size());
}

int CViewBooking1::exp(int n)
{
	int res = 1;
	for (int i = 0; i < n-1; i++)
	{
		res *= 10;
	}
	return res;
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

	////˫�������
	//CRect   rect;
	//CDC     MenDC;
	//CBitmap MemMap;

	//GetClientRect(&rect);
	//MenDC.CreateCompatibleDC(&dc);
	//MemMap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	//MenDC.SelectObject(&MemMap);
	//MenDC.FillSolidRect(&rect, RGB(255, 255, 255));

	////����Ĭ�ϵ�OnPaint(),��ͼ�λ����ڴ�DC����
	//DefWindowProc(WM_PAINT, (WPARAM)MenDC.m_hDC, (LPARAM)0);

	////���   
	//dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MenDC, 0, 0, SRCCOPY);
	//MenDC.DeleteDC();
	//MemMap.DeleteObject();
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


CString arrClassID[5] = { "", "AM1", "AM2", "PM1", "PM2" };
void CViewBooking1::OnBnClickedConfirm()
{
	//������ݿ�
	if (m_strFileName == "δѡ��") return;

	int n = m_datas.size();
	if (n > 0)
	{
		int cols = m_datas[0].size(); //������ ���һ��Ϊ�Ƿ��ϴ����ݿ�ı�־λ
		CString strMsg;
		for (int i = 0; i < n; i++)
		{
			if (m_datas[i][cols - 1] == "0") //δ�ϴ����ݿ�
			{
				CString strSQL;
				BOOL isOK = TRUE;
				g_mysqlCon.ExecuteSQL("BEGIN;\r\nSET AUTOCOMMIT=0\r\n", strMsg);
				strSQL.Format("INSERT INTO bookings(FILE_NAME, BOOK_DATE, CLASS_ID, FLAG) \
					VALUES ('%s', '%s', '%s', '0')", m_strFileName, m_datas[i][0], m_datas[i][1]);
				if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
				{
					m_datas[i][cols - 1] = "1";
				}
				else
				{
					ShowMsg2Output1(strMsg);
					g_mysqlCon.ExecuteSQL("ROLLBACK", strMsg);
					isOK = FALSE;
					continue;
				}

				CPoint pos = m_wndCalendar.GetDayPos(m_datas[i][0]);
				int nClass = atoi(m_datas[i][1]);
				if (m_wndCalendar.m_nStatus[pos.y][pos.x][4]) //���ݿ������д����ڵ�ԤԼ��¼��ֱ�Ӹ���ԤԼ����
				{
					strSQL.Format("UPDATE workdaystat set %s = %s + 1 WHERE WORKDAY = '%s'"
						, arrClassID[nClass], arrClassID[nClass], m_datas[i][0]);
					if (!g_mysqlCon.ExecuteSQL(strSQL, strMsg))
					{
						ShowMsg2Output1(strMsg);
						g_mysqlCon.ExecuteSQL("ROLLBACK", strMsg);
						isOK = FALSE;
						continue;
					}
				}
				else //��ԤԼ��¼������¼�¼
				{
					strSQL.Format("INSERT INTO workdaystat(WORKDAY, %s) VALUES ('%s', '1')"
						, arrClassID[nClass], m_datas[i][0]);
					if (!g_mysqlCon.ExecuteSQL(strSQL, strMsg))
					{
						ShowMsg2Output1(strMsg);
						g_mysqlCon.ExecuteSQL("ROLLBACK", strMsg);
						isOK = FALSE;
						continue;
					}
					m_wndCalendar.m_nStatus[pos.y][pos.x][4] = 1;
				}
				g_mysqlCon.ExecuteSQL("COMMIT", strMsg);
			}// if (m_datas[i][cols - 1] == "0") //δ�ϴ����ݿ�
		} //end for loop
		g_mysqlCon.ExecuteSQL("SET AUTOCOMMIT=1", strMsg);
	}

	UpdateBookingList(); //����
}


void CViewBooking1::OnBnClickedRemove()
{
	int n = m_datas.size(); 
	int cols = m_datas[0].size(); //������ ���һ��Ϊ�Ƿ��ϴ����ݿ�ı�־λ
	CString strMsg;
	CString strSQL;
	for (int i = n - 1; i >= 0; i--)
	{
		if (m_wndGrid.IsRowSelected(i))
		{
			CTime aday = m_wndCalendar.Str2Time(m_datas[i][0]);
			CTime today = m_wndCalendar.m_tToday;
			if (aday < today)
			{
				MessageBox("�ѹ�ȥ��ԤԼ����ȡ��");
				continue;
			}

			int nClass = atoi(m_datas[i][1]);

			if (m_datas[i][cols - 1] == "1") //�Ѿ��ϴ����ݿ�
			{
				g_mysqlCon.ExecuteSQL("BEGIN;\r\nSET AUTOCOMMIT=0\r\n", strMsg);
				strSQL.Format("UPDATE workdaystat set %s = %s - 1 WHERE WORKDAY = '%s'"
					, arrClassID[nClass], arrClassID[nClass], m_datas[i][0]);
				if (!g_mysqlCon.ExecuteSQL(strSQL, strMsg))
				{
					ShowMsg2Output1(strMsg);
					g_mysqlCon.ExecuteSQL("ROLLBACK", strMsg);
					continue;
				}
			}

			//ִ��ɾ���Բ����������ݿ���û�д˼�¼��ɾ��������Ӱ��ԭ������
			strSQL.Format("DELETE FROM bookings WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'"
				, m_strFileName, m_datas[i][0], m_datas[i][1]);
			if (!g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1(strMsg);
				g_mysqlCon.ExecuteSQL("ROLLBACK", strMsg);
				continue;
			}

			//ɾ�����ؼ�¼
			CPoint pos = m_wndCalendar.GetDayPos(m_datas[i][0]);
			m_wndCalendar.m_nStatus[pos.y][pos.x][nClass - 1] --;
			int nn = 5 - nClass;
			nn = exp(nn);
			m_wndCalendar.m_nStatus[pos.y][pos.x][5] -= nn;
			m_wndCalendar.SetItemState(pos, 0);

			std::vector<CStrs>::iterator it = m_datas.begin() + i;
			m_datas.erase(it);
		}
	}

	//����ͼ��
	m_wndGrid.GridRefresh(m_datas.size());
}
