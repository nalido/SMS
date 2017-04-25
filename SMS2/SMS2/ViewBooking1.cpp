// ViewBooking1.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewBooking1.h"
#include "MainFrm.h"
#include "ClassDetail.h"

CString arrClasses[5] = { "0未定义", "Am08:00-10:00", "Am10:00-12:00", "Pm01:00-03:00", "Pm03:00-05:00" };
// CViewBooking1

IMPLEMENT_DYNCREATE(CViewBooking1, CBCGPFormView)

CViewBooking1::CViewBooking1()
	: CBCGPFormView(CViewBooking1::IDD)
{
	EnableVisualManagerStyle();

	m_strFileName = "未选择";
	m_strName = "未选择";
	m_strGender = "未知";
	m_strCarType = "未知";
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


// CViewBooking1 诊断

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

			//预约日期小于今天的灰化
			CTime aday = pThis->m_wndCalendar.Str2Time(pThis->m_datas[nRow][0]);
			CTime today = pThis->m_wndCalendar.m_tToday;
			if (aday < today)
			{
				if (pThis->m_datas[nRow][2] != "0") //正常完成预约的灰化
				{
					pdi->item.clrBackground = RGB(140, 140, 140);
				}
				else //预约后未参加训练的 暗红色显示
				{
					pdi->item.clrBackground = RGB(240, 140, 140);
				}
				pdi->item.clrText = RGB(220, 220, 220);
			}

			//上课时间
			if (nCol == 1)
			{
				int n = atoi(pThis->m_datas[nRow][nCol]);
				
				pdi->item.varValue = arrClasses[n];
			}
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
		}
	}

	return TRUE;
}
// CViewBooking1 消息处理程序

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
		ShowMsg2Output1("时间在可预约范围之外！");
		return;
	}
	
	CClassDetail dlg;
	int r = item.m_nRow / 2;
	int c = item.m_nColumn;
	dlg.m_nStatus[0][0] = pThis->m_wndCalendar.m_nStatus[r][c][0];
	dlg.m_nStatus[1][0] = pThis->m_wndCalendar.m_nStatus[r][c][1];
	dlg.m_nStatus[2][0] = pThis->m_wndCalendar.m_nStatus[r][c][2];
	dlg.m_nStatus[3][0] = pThis->m_wndCalendar.m_nStatus[r][c][3];

	//传递当前学员的选择情况
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
			str.Format("您已预约全部%d节课，不能继续预约", g_nMaxBooking);
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
	strs.push_back("0"); //最后一位表示未存数据库
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

	//初始化表格
	CRect rectGrid;
	m_wndGridLocation.GetClientRect(&rectGrid);
	m_wndGridLocation.MapWindowPoints(this, &rectGrid); //转为桌面坐标
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	m_wndGrid.SetSingleSel(); //只能选一个
	m_wndGrid.EnableRowHeader(TRUE);
	m_wndGrid.EnableLineNumbers();


	int nColumn = 0;
	int hw = m_wndGrid.GetRowHeaderWidth();
	m_wndGrid.InsertColumn(nColumn++, _T("预约日期"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("训练时间"), 100);
	m_wndGrid.InsertColumn(nColumn++, _T("完成状态"), rectGrid.Width() - hw - 170);
	for (int i = 0; i < nColumn; i++)
	{
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);
	m_wndGrid.SetCallBack_Clk(OnGridClick);

	m_SPhoto.InitPicSource(&m_img); //注册图片控件数据源


	//创建日期单元格
	CRect rectWeek;
	m_SWeek.GetClientRect(&rectWeek);
	m_SWeek.MapWindowPoints(this, &rectWeek); //转为桌面坐标

	m_wndCalendar.Create(nStyle, rectWeek, this, IDC_GRID_STUPRO+1);
	m_wndCalendar.InitGrid(rectWeek.Width());
	m_wndCalendar.SetCallBack_Clk(OnCalendarClick);


	Refresh(); //刷新列表

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
		ShowMsg2Output1("查询预约信息成功");
		int n = m_datas.size();
		m_strBooked.Format("%d", n);
		CString state = "1"; //最后一列（第四列）表示已经记录在数据库，避免重复提交数据库
		for (int i = 0; i < n; i++)
		{
			m_datas[i].push_back(state);

			//日历上显示已选的天数
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

	//if (IsWindow(GetDlgItem(IDC_BUTTON1)->GetSafeHwnd())) //判断控件窗口是否已经创建
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

	////双缓存绘制
	//CRect   rect;
	//CDC     MenDC;
	//CBitmap MemMap;

	//GetClientRect(&rect);
	//MenDC.CreateCompatibleDC(&dc);
	//MemMap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	//MenDC.SelectObject(&MemMap);
	//MenDC.FillSolidRect(&rect, RGB(255, 255, 255));

	////调用默认的OnPaint(),把图形画在内存DC表上
	//DefWindowProc(WM_PAINT, (WPARAM)MenDC.m_hDC, (LPARAM)0);

	////输出   
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


		//本地打开照片，若本地无，则查询服务器下载
		ShowMsg2Output1("选择预约对象：档案" + m_strFileName);
		CString strFile;
		strFile.Format("%s\\%s.bmp", g_strFilePath, m_strFileName);
		char* file = strFile.GetBuffer();
		m_img = cv::imread(file);
		strFile.ReleaseBuffer();
		if (m_img.empty()) //本地无照片，从服务器下载
		{
			//数据打包
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame->m_pSendBuf != NULL)
			{
				MessageBox("上一个信息还未处理完毕，请稍等重试。");
			}
			else
			{
				int len = 11; //
				pFrame->m_isSendReady = FALSE;
				pFrame->m_pSendBuf = new BYTE[len];//发送完删除
				pFrame->m_nSendLen = len;
				pFrame->m_pSendBuf[0] = 3; //请求图像数据
				char* cID = m_strFileName.GetBuffer();
				memcpy(pFrame->m_pSendBuf + 1, cID, 10); //档案号
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
	//添加数据库
	if (m_strFileName == "未选择") return;

	int n = m_datas.size();
	if (n > 0)
	{
		int cols = m_datas[0].size(); //列数， 最后一列为是否上传数据库的标志位
		CString strMsg;
		for (int i = 0; i < n; i++)
		{
			if (m_datas[i][cols - 1] == "0") //未上传数据库
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
				if (m_wndCalendar.m_nStatus[pos.y][pos.x][4]) //数据库中已有此日期的预约记录，直接更新预约数据
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
				else //无预约记录，添加新记录
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
			}// if (m_datas[i][cols - 1] == "0") //未上传数据库
		} //end for loop
		g_mysqlCon.ExecuteSQL("SET AUTOCOMMIT=1", strMsg);
	}

	UpdateBookingList(); //排序
}


void CViewBooking1::OnBnClickedRemove()
{
	int n = m_datas.size(); 
	int cols = m_datas[0].size(); //列数， 最后一列为是否上传数据库的标志位
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
				MessageBox("已过去的预约不能取消");
				continue;
			}

			int nClass = atoi(m_datas[i][1]);

			if (m_datas[i][cols - 1] == "1") //已经上传数据库
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

			//执行删除吃操作，若数据库中没有此记录，删除操作不影响原有数据
			strSQL.Format("DELETE FROM bookings WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'"
				, m_strFileName, m_datas[i][0], m_datas[i][1]);
			if (!g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1(strMsg);
				g_mysqlCon.ExecuteSQL("ROLLBACK", strMsg);
				continue;
			}

			//删除本地记录
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

	//更新图表
	m_wndGrid.GridRefresh(m_datas.size());
}
