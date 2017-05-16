// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SMS2.h"

#include "MainFrm.h"
#include "ViewRegister.h"
#include "ViewBooking1.h"
#include "ViewBooking2.h"
#include "ViewK1Check.h"
#include "ViewK1Exam.h"
#include "ViewStuProgress.h"
#include "School.h"
#include "System.h"
#include "Coaches.h"
#include "ViewOrderRsp.h"
#include "ViewDevices.h"
#include "ViewKPI.h"
#include "ViewScan.h"
#include "ViewStuffEnter.h"
#include "ViewStudentEnter.h"
#include "ViewHome.h"
#include "ViewPermission.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////global functions and values///////////////////////
CString g_strFilePath = "E:\\Photos\\";
CString g_strK1Address;
xPublic::CMySQLEx g_mysqlCon;
CString g_sServerIP = "127.0.0.1";
int g_nClassTotal = 9;
int g_nMaxBooking = 15;
int g_nSubForLeave = 8;
int g_nMinWorkTime = 176;
int g_nPermissions[6] = { 0 };
CString g_strUserID = "";
void LOG(CString sFileName, CString str_log, int flag) // 程序运行日志：记录系统运行状态 
{
	//12.6
	CFile f;
	if (f.Open(sFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
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

char* EncodeToUTF8(const char* mbcsStr)
{
	wchar_t*  wideStr;
	char*   utf8Str;
	int   charLen;

	charLen = MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, NULL, 0);
	wideStr = (wchar_t*)malloc(sizeof(wchar_t)*charLen);
	MultiByteToWideChar(CP_ACP, 0, mbcsStr, -1, wideStr, charLen);

	charLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);

	utf8Str = (char*)malloc(charLen);

	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, charLen, NULL, NULL);

	free(wideStr);
	return utf8Str;

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

CString GetLastMonth(CTime& thisMonth)
{
	int tyear = thisMonth.GetYear();
	int tmonth = thisMonth.GetMonth();
	int tday = thisMonth.GetDay();

	if (tmonth == 1)
	{
		tyear -= 1;
		tmonth = 12;
	}
	else tmonth -= 1;
	CString lastMonth;
	lastMonth.Format("%d/%02d", tyear, tmonth);

	return lastMonth;
}

CTime Str2Time(CString str)
{
	int pos1, pos2;
	pos1 = str.Find('/');
	pos2 = str.ReverseFind('/');

	int nYear = atoi(str.Left(pos1));
	int nMonth = atoi(str.Mid(pos1 + 1, pos2));
	int nDay = atoi(str.Mid(pos2 + 1));

	CTime tmp(nYear, nMonth, nDay, 0, 0, 0);

	return tmp;
}

void ExportExcel(std::vector<CString>& titles, CDStrs &datas)
{
	CFileDialog fileDlg(FALSE, "*.csv", NULL, 6UL, "通用表格(*.csv)|*.csv|");
	CString filename("");
	if (fileDlg.DoModal() == IDOK)
	{
		filename = fileDlg.GetPathName();
	}
	else return;

	CFile f;
	if (f.Open(filename, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		CString strLine("");

		//列名
		int cols = titles.size();
		for (int i = 0; i < cols; i++)
		{
			strLine = strLine + titles[i] + ",";
		}
		strLine = strLine + "\r\n";
		f.Write(strLine, strlen(strLine));

		//数据
		int rows = datas.size();
		for (int r = 0; r < rows; r++)
		{
			strLine = "";
			for (int c = 0; c < cols; c++)
			{
				strLine = strLine + datas[r][c] + ",";
			}
			strLine = strLine + "\r\n";
			f.Write(strLine, strlen(strLine));
		}

		f.Close();
	}
	
}
///////////////////////////////end of global functions//////////////

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_COMMAND_EX(ID_VIEW_REGISTER, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_K1CHECK, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_K1EXAM, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_STUPROGRESS, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_BOOKING1, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_BOOKING2, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_COACH, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_KPI, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_SCAN, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_4STUDENT, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_STUFF, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_DEVICE, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_SYSTEMSETTING, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_SCHOOLSETTING, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_ORDER_RSP, OnViewSelected)
	ON_COMMAND_EX(ID_VIEW_PERMISSION, OnViewSelected)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_REGISTERED_MESSAGE(BCGM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_MESSAGE(UM_REDRAW, OnRedraw)
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
: m_threadMySQL(this, ThreadMySQLCallback)
, m_threadSocket(this, ThreadSocketCallback)
{
	m_pSendBuf = NULL;
	m_nSendLen = 0;
	m_isSendReady = FALSE;
	m_hSocketEvent = ::CreateEventA(NULL, TRUE, FALSE, NULL);
}

CMainFrame::~CMainFrame()
{

	if (NULL != m_hSocketEvent)
	{
		::CloseHandle(m_hSocketEvent); m_hSocketEvent = NULL;
	}
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
	if (!g_mysqlCon.Connect("localhost", 3306, "snow", "snow", "snow123", "gbk", strMsg))
	{
		m_wndOutput.AddItem2List1(_T("连接数据库失败!\r\n") + strMsg);
	}
	else
	{
		m_wndOutput.AddItem2List1("连接数据库成功！");
	}
	//开始子线程
	m_threadMySQL.StartThread();
	m_threadSocket.StartThread();

	//timer
	SetTimer(0, 100, NULL); //定时刷新子界面

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~FWS_ADDTOTITLE; //使用固定标题
	cs.lpszName = "东华驾校学员管理系统";
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


BOOL CMainFrame::OnViewSelected(UINT nID)
{
	switch (nID)
	{
	case ID_VIEW_BOOKING1:
		SelectView(VIEW_BOOKING1);
		break;
	case ID_VIEW_BOOKING2:
		SelectView(VIEW_BOOKING2); 
			break;
	case ID_VIEW_ORDER_RSP:
		SelectView(VIEW_ORDER_RSP);
			break;
	case ID_VIEW_REGISTER:
		SelectView(VIEW_REGISTER);
		break;
	case ID_VIEW_K1CHECK:
		SelectView(VIEW_K1CHECK);
		break;
	case ID_VIEW_K1EXAM:
		SelectView(VIEW_K1EXAM);
		break;
	case ID_VIEW_STUPROGRESS:
		SelectView(VIEW_STUPROGRESS);
		break;
	case ID_VIEW_SYSTEMSETTING:
		SelectView(VIEW_SYSTEM);
		break;
	case ID_VIEW_SCHOOLSETTING:
		SelectView(VIEW_SCHOOL);
		break;
	case ID_VIEW_COACH:
		SelectView(VIEW_COACHES);
		break;
	case ID_VIEW_KPI:
		SelectView(VIEW_KPI);
		break;
	case ID_VIEW_SCAN:
		SelectView(VIEW_SCAN);
		break;
	case ID_VIEW_DEVICE:
		SelectView(VIEW_DEVICES);
		break;
	case ID_VIEW_STUFF:
		SelectView(VIEW_STUFFENTER);
		break;
	case ID_VIEW_4STUDENT:
		SelectView(VIEW_STUDENTENTER);
		break;
	case ID_VIEW_PERMISSION:
		SelectView(VIEW_PERMISSION);
		break;
	}
	return TRUE;
}

 // UI_TYPE_RIBBON


CView* CMainFrame::GetView(int nID)
{
	if (m_arViews.GetSize() == 0)
	{
		const int nCount = VIEW_NUM; //子窗口总数
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
	case VIEW_HOME:
		pClass = RUNTIME_CLASS(CViewHome);
		break;
	case VIEW_REGISTER:
		pClass = RUNTIME_CLASS(CViewRegister);
		break;
	case VIEW_BOOKING1:
		pClass = RUNTIME_CLASS(CViewBooking1);
		break;
	case VIEW_BOOKING2:
		pClass = RUNTIME_CLASS(CViewBooking2);
		break;
	case VIEW_ORDER_RSP:
		pClass = RUNTIME_CLASS(CViewOrderRsp);
		break;
	case VIEW_K1CHECK:
		pClass = RUNTIME_CLASS(CViewK1Check);
		break;
	case VIEW_K1EXAM:
		pClass = RUNTIME_CLASS(CViewK1Exam); 
		break;
	case VIEW_COACHES:
		pClass = RUNTIME_CLASS(CCoaches);
		break;
	case VIEW_KPI:
		pClass = RUNTIME_CLASS(CViewKPI);
		break;
	case VIEW_DEVICES:
		pClass = RUNTIME_CLASS(CViewDevices);
		break;
	case VIEW_SCAN:
		pClass = RUNTIME_CLASS(CViewScan);
		break;
	case VIEW_STUFFENTER:
		pClass = RUNTIME_CLASS(CViewStuffEnter);
		break;
	case VIEW_STUDENTENTER:
		pClass = RUNTIME_CLASS(CViewStudentEnter);
		break;
	case VIEW_STUPROGRESS:
		pClass = RUNTIME_CLASS(CViewStuProgress); 
			break;
	case VIEW_SCHOOL:
		pClass = RUNTIME_CLASS(CSchool);
		break;
	case VIEW_SYSTEM:
		pClass = RUNTIME_CLASS(CSystem);
		break;
	case VIEW_PERMISSION:
		pClass = RUNTIME_CLASS(CViewPermission);
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

	if (!pView->Create(NULL, _T(""), (AFX_WS_DEFAULT_VIEW),
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

	//窗口的数据初始化刷新
	pNewView->SendMessage(WM_USER_UPDATE_VIEW, (WPARAM)1);

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
	m_threadSocket.StopThread();
	CBCGPFrameWnd::OnClose();
}


LRESULT CMainFrame::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	int permission = (int)wParam; //当前用户的权限
	switch (permission)
	{
	case 0: //初始化，隐藏全部
	{
				//默认全部隐藏
				//int nCount = m_wndRibbonBar.GetCategoryCount();
				//for (int i = 1; i < nCount; i++)
				//{
				//	m_wndRibbonBar.ShowCategory(i, FALSE);
				//}
				//CBCGPRibbonCategory* pCate = m_wndRibbonBar.GetCategory(0);
				//m_wndRibbonBar.SetActiveCategory(pCate);
				//m_wndRibbonBar.RecalcLayout();
				//for (int i = 0; i < 5; i++)
				//{
				//	g_nPermissions[i] = 0;
				//}
				break;
	}
	case 1: //最高权限，显示全部
	{
				int nCount = m_wndRibbonBar.GetCategoryCount();
				for (int i = 1; i < nCount; i++)
				{
					m_wndRibbonBar.ShowCategory(i, TRUE);
				}
				m_wndRibbonBar.RecalcLayout();
				break;
	}
	default: //非最高权限，按权限设置显示
	{
				 int nCount = m_wndRibbonBar.GetCategoryCount();
				for (int i = 1; i < nCount; i++)
				{
					m_wndRibbonBar.ShowCategory(i, TRUE);
				}
				
				if (g_nPermissions[0] == 0) //新生管理
					m_wndRibbonBar.ShowCategory(1, FALSE);

				if (g_nPermissions[1] == 0) //学员管理
					m_wndRibbonBar.ShowCategory(2, FALSE);

				if (g_nPermissions[2] == 0) //设备管理
					m_wndRibbonBar.ShowCategory(3, FALSE);

				if (g_nPermissions[3] == 0) //员工管理
					m_wndRibbonBar.ShowCategory(4, FALSE);

				if (g_nPermissions[4] == 0) //驾校管理
					m_wndRibbonBar.ShowCategory(5, FALSE);

				m_wndRibbonBar.RecalcLayout();
				break;
	}
	}

	return 0;
}


void CALLBACK CMainFrame::ThreadSocketCallback(LPVOID pParam, HANDLE hCloseEvent)
{
	CMainFrame* pThis = (CMainFrame*)pParam;
	xPublic::CTCPClient *pTcpClient = &pThis->m_tcpClient;
	DWORD dwWaitTime = 10;
	BOOL bNotify = TRUE;
	CString strMsg;
	BYTE senttime = 0;

	while (WAIT_TIMEOUT == ::WaitForSingleObject(hCloseEvent, dwWaitTime))
	{
		if (pThis->m_pSendBuf == NULL)
		{
			if (pTcpClient->Close())
			{
				strMsg.Format("没有图像发送，自动断开与服务器的连接");
				pThis->m_wndOutput.AddItem2List4(strMsg);
			}
			continue;
		}

		if (pThis->m_isSendReady == FALSE) continue; //数据还没有准备好

		//检测和创建TCP连接
		if (!pTcpClient->IsConnected())
		{
			if (!pTcpClient->Connect(g_sServerIP, 39200, NULL))
			{
				if (bNotify)
				{
					bNotify = FALSE;
					strMsg.Format("连接服务器(%s:39200)失败,直接删除数据", g_sServerIP);
					pThis->m_wndOutput.AddItem2List4(strMsg);
					if (pThis->m_pSendBuf != NULL) //服务器无连接则直接删除数据
					{
						delete[] pThis->m_pSendBuf;
						pThis->m_pSendBuf = NULL;
						bNotify = TRUE;
					}
				}
				::WaitForSingleObject(hCloseEvent, 2000);
				continue;
			}
			bNotify = TRUE;
			strMsg.Format("连接服务器(%s:39200)成功", g_sServerIP);
			pThis->m_wndOutput.AddItem2List4(strMsg);
		}//检测和创建连接

		//发送数据
		::ResetEvent(pThis->m_hSocketEvent);
		strMsg.Format("第%d次发送数据", senttime);
		pThis->m_wndOutput.AddItem2List4(strMsg);
		BOOL bSendOK = FALSE;
		int nPicLen = pThis->m_nSendLen;
		if (pTcpClient->IsConnected() && pTcpClient->Send(pThis->m_pSendBuf, nPicLen) & !bSendOK)
		{
			bSendOK = TRUE;
			strMsg.Format("数据发送成功");
			pThis->m_wndOutput.AddItem2List4(strMsg);

			if (pThis->m_pSendBuf[0] == 3)
			{
				BYTE flag = 0;
				pTcpClient->Receive(&flag, 1);
				if (flag == 1) //后有图像数据
				{
					int wid, hei, imgSize;
					char FileNum[11] = { 0 };
					pTcpClient->Receive(&FileNum, 10);
					pTcpClient->Receive(&wid, 4);
					pTcpClient->Receive(&hei, 4);
					pTcpClient->Receive(&imgSize, 4);
					BYTE* picBuf = new BYTE[imgSize + 1];
					if (pTcpClient->Receive(picBuf, imgSize))
					{
						strMsg.Format("收到图像信息：%dX%d 接收成功", wid, hei);

						pThis->SaveBmp(FileNum, picBuf, wid, hei, imgSize); //保存图片，第二次点击时无需再下载
						IplImage* pImg = cvCreateImageHeader(cvSize(wid, hei), 8, 3);
						int lineByte = (wid * 3 + 3) / 4 * 4;
						cvSetData(pImg, picBuf, lineByte);
						cv::Mat img = cv::cvarrToMatND(pImg);
						pThis->GetActiveView()->SendMessageA(WM_USER_MESSAGE, (WPARAM)&img, (LPARAM)2);

						pThis->m_wndOutput.AddItem2List4(strMsg);
					}
					else
					{
						strMsg.Format("收到图像信息：%dX%d 接收图像数据失败", wid, hei);
						pThis->m_wndOutput.AddItem2List4(strMsg);
					}

					delete[] picBuf; picBuf = NULL;
				}
			} //if (pThis->m_pSendBuf[0] == 3)

			delete[]pThis->m_pSendBuf; //删除数据
			pThis->m_pSendBuf = NULL;
		}

		//判断是否发送成功，删除图像缓存
		if (!bSendOK)
		{
			if (++senttime >= 3) //试发三次后删除图像
			{
				if (pThis->m_pSendBuf != NULL)
				{
					delete[] pThis->m_pSendBuf;
					pThis->m_pSendBuf = NULL;
				}
				senttime = 0;
				strMsg.Format("数据发送三次失败");
				pThis->m_wndOutput.AddItem2List4(strMsg);
			}
		}
		::SetEvent(pThis->m_hSocketEvent);
	}//end while


	if (pThis->m_pSendBuf != NULL)
	{
		delete[] pThis->m_pSendBuf;
		pThis->m_pSendBuf = NULL;
	}
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	m_wndOutput.ListFresh();

	CBCGPFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::SaveBmp(char* FileNum, BYTE* picBuf, int wid, int hei, int imgSize)
{
	IplImage* pImg = cvCreateImageHeader(cvSize(wid, hei), 8, 3);
	int lineByte = (wid*3 + 3) / 4 * 4;
	cvSetData(pImg, picBuf, lineByte);

	cv::Mat img = cv::cvarrToMatND(pImg);
	CString sFileName("");
	sFileName.Format("%s\\%s.bmp", g_strFilePath, FileNum);
	::SHCreateDirectory(NULL, CA2W(g_strFilePath));
	cv::String s = sFileName.GetBuffer();
	imwrite(s, img);
	sFileName.ReleaseBuffer();
	pImg = NULL;
}