// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SMS_SERVER.h"

#include "MainFrm.h"
#include "xPublic\Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////global functions and values///////////////////////
CString g_strFilePath = "E:\\Server_Photos\\"; 
CString g_sMySQLIP = "127.0.0.1";
xPublic::CMySQLEx g_mysqlCon;
CDStrs g_strMsgLog;
CString g_strCurrentTime; 
BOOL g_isSMSSended;


CTime GetServerTime()
{
	int pos0 = g_strCurrentTime.Find('-');
	CString strHour = g_strCurrentTime.Mid(pos0 + 1);
	CString strDate = g_strCurrentTime.Left(pos0);

	//解析日期
	int pos1, pos2;
	pos1 = strDate.Find('/');
	pos2 = strDate.ReverseFind('/');

	int nYear = atoi(strDate.Left(pos1));
	int nMonth = atoi(strDate.Mid(pos1 + 1, pos2));
	int nDay = atoi(strDate.Mid(pos2 + 1));

	//解析时间
	pos1 = strHour.Find(':');
	pos2 = strHour.ReverseFind(':');
	int nHour = atoi(strHour.Left(pos1));
	int nMin = atoi(strHour.Mid(pos1 + 1, pos2));
	int nSec = atoi(strHour.Mid(pos2 + 1));

	CTime tmp(nYear, nMonth, nDay, nHour, nMin, nSec);

	return tmp;
}


void LOG(CString sFileName, CString str_log, int flag) // 程序运行日志：记录系统运行状态 
{
	//12.6
	CFile f;
	if (f.Open(sFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		LONG filesize = f.GetLength();
		if (filesize > 40960) f.SetLength(0); //大于40M时清空

		f.SeekToEnd();
		if (flag)
		{
			CTime localtime;
			localtime = CTime::GetCurrentTime();
			str_log = localtime.Format("\r\n\r\n%Y-%m-%d\t%X\r\n") + str_log;
		}
		else
		{
			str_log.Format(_T("%s\r\n"), str_log);
		}
		f.Write(str_log, strlen(str_log));
		f.Close();
	}
}
void ShowMsg2Output1(CString strMsg)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndOutput.AddItem2List1(strMsg);
}

CString GetClassTime(int n) //根据时段编号获得具体时间
{
	CString res("");
	switch (n)
	{
	case 0:
		res = "A8:00-10:00";
		break;
	case 1:
		res = "A10:00-12:00";
		break;
	case 2:
		res = "P2:00 - 4:00";
		break;
	case 3:
		res = "P4:00 - 6:00";
		break;
	case 4:
		res = "P6:00 - 8:00";
		break;
	default:
		res = "未定义";
		break;
	}
	return res;
}
///////////////////////////////end of global functions//////////////

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_REGISTERED_MESSAGE(BCGM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_MESSAGE(UM_REDRAW, OnRedraw)
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMainFrame construction/destruction
enum VIEW_TYPE{
	VIEW_REGISTER = 0,
	VIEW_BOOKING1,
	VIEW_BOOKING2
};

CMainFrame::CMainFrame()
: m_threadMySQL(this, ThreadMySQLCallback)
, m_threadClock(this, ThreadClockCallback)
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if (!CreateRibbonBar ())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: add your status bar panes here:
	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_PANE1, _T("Pane 1"), TRUE), _T("Pane 1"));
	m_wndStatusBar.AddExtendedElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_PANE2, _T("Pane 2"), TRUE), _T("Pane 2"));

	// Load control bar icons:
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.SetTransparentColor (RGB (255, 0, 255));
	imagesWorkspace.Load (IDB_WORKSPACE);
	imagesWorkspace.SmoothResize(globalData.GetRibbonImageScale());

	if (!m_wndOutput.Create (_T("Output"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
	m_wndOutput.SetIcon (imagesWorkspace.ExtractIcon (2), FALSE);

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndOutput);

	//连接数据
	CString strMsg("");
	if (!g_mysqlCon.Connect(g_sMySQLIP.GetBuffer(0), 3306, "snow", "snow", "snow123", "gbk", strMsg))
	{
		m_wndOutput.AddItem2List1(_T("连接数据库失败!\r\n") + strMsg);
	}
	else
	{
		m_wndOutput.AddItem2List1("连接数据库成功！");
	}
	g_sMySQLIP.ReleaseBuffer();
	//开始子线程
	m_threadMySQL.StartThread();
	m_threadClock.StartThread();

	//开启定时器定时刷新输出信息
	SetTimer(0, 100, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//cs.cx = GetSystemMetrics(SM_CXSCREEN);
	//cs.cy = GetSystemMetrics(SM_CYSCREEN);

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




BOOL CMainFrame::CreateRibbonBar ()
{
	m_wndRibbonBar.EnableCustomization();

	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}

	if (!m_wndRibbonBar.LoadFromXML (_T("IDR_BCGP_RIBBON_XML")))
	{
		return FALSE;
	}
 // USE_RIBBON_DESIGNER

	m_wndRibbonBar.EnablePrintPreview(FALSE);

	return TRUE;
}

LRESULT CMainFrame::OnRibbonCustomize (WPARAM wp, LPARAM /*lp*/)
{
	ShowOptions (wp == 0 ? 1 : 0);
	return 1;
}

void CMainFrame::OnToolsOptions()
{
	ShowOptions (0);
}

void CMainFrame::ShowOptions (int nPage)
{
	// Create custom categories:
	
	// "Popular" items:
	
	CList<UINT, UINT> lstPopular;
	
	lstPopular.AddTail (ID_FILE_NEW);
	lstPopular.AddTail (ID_FILE_OPEN);
	lstPopular.AddTail (ID_FILE_SAVE);
	lstPopular.AddTail (ID_EDIT_UNDO);
	
	// Hidden commands:
	CList<UINT,UINT> lstHidden;
	m_wndRibbonBar.GetItemIDsList (lstHidden, TRUE);
	
	// All commands:
	CList<UINT,UINT> lstAll;
	m_wndRibbonBar.GetItemIDsList (lstAll);

	// Create "Customize QAT" page:
	CBCGPRibbonCustomizeQATPage pageCustomizeQAT(&m_wndRibbonBar);
	
	pageCustomizeQAT.AddCustomCategory (_T("Popular Commands"), lstPopular);
	pageCustomizeQAT.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeQAT.AddCustomCategory (_T("All Commands"), lstAll);
	
	// Create "Customize Ribbon" page:
	CBCGPRibbonCustomizeRibbonPage pageCustomizeRibbon(&m_wndRibbonBar);
	
	pageCustomizeRibbon.AddCustomCategory (_T("Popular Commands"), lstPopular);
	pageCustomizeRibbon.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeRibbon.AddCustomCategory (_T("All Commands"), lstAll);
	
	// Create property sheet:
	CBCGPPropertySheet propSheet (_T("Options"), this, nPage);
	propSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	propSheet.EnableLayout();
	
	propSheet.SetLook(CBCGPPropertySheet::PropSheetLook_List, 150 /* List width */);
	
	propSheet.AddPage (&pageCustomizeRibbon);
	
	propSheet.AddPage (&pageCustomizeQAT);

	// TODO: add your option pages here:
	// COptionsPage1 pageOptions1;
	// propSheet.AddPage (&pageOptions1);
	//
	// COptionsPage1 pageOptions2;
	// propSheet.AddPage (&pageOptions2);
	
	if (propSheet.DoModal() != IDOK)
	{
		return;
	}
}
 // WORKSPACEBAR
void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!(m_wndOutput.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}
 // OUTPUTBAR

 // UI_TYPE_RIBBON


CView* CMainFrame::GetView(int nID)
{
	if (m_arViews.GetSize() == 0)
	{
		const int nCount = 3; //子窗口总数
		for (int i = 0; i < nCount; i++)
		{
			m_arViews.Add(NULL);
		}
	}

	int nIndex = nID;

	if (nIndex < 0 || nIndex >= m_arViews.GetSize())
	{
		ASSERT(FALSE);
		return NULL;
	}

	CView* pView = m_arViews[nIndex];
	if (pView != NULL)
	{
		return pView;
	}
	CRuntimeClass* pClass;
	switch (nIndex)
	{
	case VIEW_REGISTER:
		//pClass = RUNTIME_CLASS(CViewRegister);
		break;
	case VIEW_BOOKING1:
		break;
	case VIEW_BOOKING2:
		break;
	}
	if (pClass == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	ASSERT(pClass->IsDerivedFrom(RUNTIME_CLASS(CView)));

	pView = DYNAMIC_DOWNCAST(CView, pClass->CreateObject());

	if (pView == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}
	ASSERT_VALID(pView);

	CDocument* pCurrentDoc = GetActiveDocument();

	CCreateContext newContext;
	newContext.m_pNewViewClass = NULL;
	newContext.m_pNewDocTemplate = NULL;
	newContext.m_pLastView = NULL;
	newContext.m_pCurrentFrame = NULL;
	newContext.m_pCurrentDoc = pCurrentDoc;

	if (!pView->Create(NULL, _T(""), (AFX_WS_DEFAULT_VIEW & ~WS_VISIBLE),
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST + nIndex, &newContext))
	{
		TRACE1("Failed to create view '%s'\n", pClass->m_lpszClassName);
		delete pView;
		return NULL;
	}

	pView->OnInitialUpdate(); //initial

	m_arViews.SetAt(nIndex, pView);
	return pView;
}

void CMainFrame::SelectView(int nID)
{
	CWaitCursor wait;

	CView* pNewView = GetView(nID);
	if (pNewView == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	CView* pActiveView = GetActiveView();

	if (pActiveView == NULL)
	{
		return;
	}

	//m_nCurrType = nID;

	if (pNewView != pActiveView)
	{
		UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);

		::SetWindowLong(pActiveView->m_hWnd, GWL_ID,
			::GetWindowLong(pNewView->m_hWnd, GWL_ID));

		::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);

		pActiveView->ShowWindow(SW_HIDE);
		pNewView->ShowWindow(SW_SHOW);

		SetActiveView(pNewView);
	}

	//theApp.WriteInt(_T("ViewType"), m_nCurrType);
	PostMessage(UM_REDRAW);
}

LRESULT CMainFrame::OnRedraw(WPARAM, LPARAM)
{
	RecalcLayout(TRUE);
	GetActiveView()->UpdateWindow();
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN | RDW_FRAME);
	return 0;
}


void CALLBACK CMainFrame::ThreadClockCallback(LPVOID pParam, HANDLE hCloseEvent)
{
	CMainFrame* pThis = (CMainFrame*)pParam;
	DWORD dwWaitTime = 600000; //十分钟刷新一次

	//已连接时等待5s，未连接时等待2s
	while (WAIT_TIMEOUT == ::WaitForSingleObject(hCloseEvent, 10000))
	{
		CString strMsg, strSQL;
		CTime t = GetServerTime();// CTime::GetCurrentTime();
		//事务四： 提前一天发送提醒短信 每天晚上7点 //在服务器端完成

		int nHour = t.GetHour();
		if (nHour < 19)
		{
			g_isSMSSended = FALSE;
		}
		else if (g_isSMSSended == FALSE)
		{
			CTime tomo = t + CTimeSpan(1, 0, 0, 0);
			CString strTomo = tomo.Format("%Y/%m/%d");
			strSQL.Format("SELECT students.SNAME, students.TEL, bookings.CLASS_NUM, students.CLASS_TYPE, \
						  bookings.CLASS_ID, coachinfo.SName, bookings.ORDER_CAR FROM bookings \
						  left join students ON students.FILE_NAME=bookings.FILE_NAME \
						  left join coachinfo ON coachinfo.FILE_NUM=bookings.ORDER_COACH \
						  WHERE bookings.BOOK_DATE='%s' AND bookings.FLAG='1'", strTomo);
			CDStrs orders;
			g_mysqlCon.ExecuteQuery(strSQL, orders, strMsg);

			int n = orders.size();
			if (n > 0)
			{
				//获取短信模板

				CString strKeyName = "NextClass";
				CString strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
				if (strSMSTmp == "failed")
				{
					LOG("serverLog.log", "获取短信模板失败");
					continue;
				}

				for (int i = 0; i < n; i++)
				{
					CString strName = orders[i][0];
					CString strTEL = orders[i][1];
					CString strCoach = orders[i][5];
					CString strCar = orders[i][6];

					CString strClassType = orders[i][3];
					int classStep = atoi(orders[i][2]) + 1;
					CString strClassi;
					strClassi.Format("%d", classStep);
					int classID = atoi(orders[i][4]) - 1;
					CString strClassTime = GetClassTime(classID);
					CString strDate = strTomo + strClassTime;

					CString strSMS = strTEL + ":" + strName + ">" + strSMSTmp;
					strSMS.Replace("N", strName);
					strSMS.Replace("L", strCoach);
					strSMS.Replace("M", strCar);
					strSMS.Replace("K", strClassi);
					strDate.Replace("A", "上午");
					strDate.Replace("P", "下午");
					strSMS.Replace("T", strDate);

					
					CView* pView = pThis->GetActiveView();
					pView->SendMessage(WM_USER_MESSAGE, (WPARAM)strSMS.GetBuffer(0), (LPARAM)2);
					strSMS.ReleaseBuffer();

					CString strLog;
					strLog.Format("发送上课提醒短信给【%s】", strName);
					LOG("serverLog.log", strLog);
				}

			}

			g_isSMSSended = TRUE;
		}

		////事务一：每月添加新的KPI记录
		//CString thisMonth = t.Format("%Y/%m");
		//strSQL.Format("SELECT * FROM kpis WHERE KMONTH='%s'", thisMonth);
		//CDStrs datas;
		//if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg))
		//{
		//	if (datas.size() == 0) //没有本月的记录
		//	{
		//		g_mysqlCon.ExecuteSQL("BEGIN;\r\nSET AUTOCOMMIT=0\r\n", strMsg);
		//		strSQL.Format("INSERT INTO KPIS (COACH, COACH_ID) SELECT coachinfo.SNAME, coachinfo.FILE_NUM FROM coachinfo INNER JOIN coachstat ON coachinfo.FILE_NUM=coachstat.FILE_NUM WHERE coachstat.BLACK_NAME='0'");
		//		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		//		{
		//			strSQL.Format("UPDATE KPIS SET KMONTH='%s' WHERE KMONTH='0'", thisMonth);
		//			if (!g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		//			{
		//				g_mysqlCon.ExecuteSQL("ROLLBACK", strMsg);
		//			}
		//		}
		//		else
		//			g_mysqlCon.ExecuteSQL("ROLLBACK", strMsg);
		//	}
		//}

		////事务二：更新coachstat的KPI
		//CString lastMonth = GetLastMonth(t);
		//strSQL.Format("UPDATE coachstat, kpis SET coachstat.PERFORMANCE=kpis.SCORE WHERE coachstat.FILE_NUM=kpis.COACH_ID AND kpis.KMONTH='%s'", lastMonth);
		//g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		//事务三：代办事务提醒

	}
}



void CALLBACK CMainFrame::ThreadMySQLCallback(LPVOID pParam, HANDLE hCloseEvent)
{
	CMainFrame* pThis = (CMainFrame*)pParam;
	DWORD dwWaitTime = 0;

	//已连接时等待5s，未连接时等待2s
	while (WAIT_TIMEOUT == ::WaitForSingleObject(hCloseEvent, dwWaitTime))
	{
		CString strMsg;
		if (g_mysqlCon.IsConnected())
		{
			strMsg.Format("%s(数据库已连接)", APP_TITLE);
			pThis->SetWindowText(strMsg);
			dwWaitTime = 5000;
		}
		else
		{
			g_mysqlCon.Close();
			strMsg.Format("%s(数据库未连接)", APP_TITLE);
			pThis->SetWindowText(strMsg);
			g_mysqlCon.Reconnect(strMsg);
			dwWaitTime = 2000;
		}
	}
}

void CMainFrame::OnClose()
{
	//关闭子线程
	m_threadMySQL.StopThread();
	m_threadClock.StopThread();
	CBCGPFrameWnd::OnClose();
}


LRESULT CMainFrame::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 0: //定时刷新输出信息
		break;
	}
	CBCGPFrameWnd::OnTimer(nIDEvent);
}
