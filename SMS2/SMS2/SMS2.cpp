// SMS2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SMS2.h"
#include "MainFrm.h"

#include "SMS2Doc.h"
#include "SMS2View.h"

#include "ViewRegister.h"
#include "xPublic\Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMS2App

BEGIN_MESSAGE_MAP(CSMS2App, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CBCGPWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CBCGPWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CSMS2App construction

CSMS2App::CSMS2App()
{
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2003, ID_VIEW_APPLOOK_2003);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLUE, ID_VIEW_APPLOOK_2007);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLACK, ID_VIEW_APPLOOK_2007_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_SILVER, ID_VIEW_APPLOOK_2007_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_AQUA, ID_VIEW_APPLOOK_2007_3);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_BLUE, ID_VIEW_APPLOOK_2010_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_BLACK, ID_VIEW_APPLOOK_2010_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_SILVER, ID_VIEW_APPLOOK_2010_3);
	AddVisualTheme(BCGP_VISUAL_THEME_SCENIC, ID_VIEW_APPLOOK_WIN7);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2013_WHITE, ID_VIEW_APPLOOK_2013_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2013_GRAY, ID_VIEW_APPLOOK_2013_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2013_DARK_GRAY, ID_VIEW_APPLOOK_2013_3);
	AddVisualTheme(BCGP_VISUAL_THEME_VS_2013_LIGHT, ID_VIEW_APPLOOK_VS2012_LIGHT);
	AddVisualTheme(BCGP_VISUAL_THEME_VS_2013_DARK, ID_VIEW_APPLOOK_VS2012_DARK);
	AddVisualTheme(BCGP_VISUAL_THEME_VS_2013_BLUE, ID_VIEW_APPLOOK_VS2012_BLUE);



	CBCGPVisualManager2013::SetAccentColor(CBCGPVisualManager2013::VS2012_Green);
	//CBCGPVisualManager2013::SetAccentColor(CBCGPVisualManager2013::VS2012_Orange);
	//CBCGPVisualManager2013::SetAccentColor(CBCGPVisualManager2013::VS2012_Blue);
	//CBCGPVisualManagerVS2012::SetAccentColor(CBCGPVisualManagerVS2012::VS2012_Blue);
	SetVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_BLUE);

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSMS2App object

CSMS2App theApp;


// CSMS2App initialization

BOOL CSMS2App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBCGPWinApp::InitInstance();


	//系统配置初始化
	xPublic::GetWorkPath("StudentsManagementSystem");
	g_strFilePath = xPublic::GETSTR2("Option", "PicSavePath", _T("E:\\Photos\\"));
	g_nSubForLeave = xPublic::GETINT2("Coach", "SubForLeave", 8);
	g_nMinWorkTime = xPublic::GETINT2("Coach", "MinWorkTime", 8);

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("BCGP AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	SetRegistryBase(_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSMS2Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSMS2View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SelectView(VIEW_STUFFENTER);
	pFrame->PostMessage(WM_USER_MESSAGE, (WPARAM)0);
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// CSMS2App message handlers

int CSMS2App::ExitInstance() 
{
	GdiplusShutdown(m_gdiplusToken);

	return CBCGPWinApp::ExitInstance();
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	//CBCGPURLLinkButton m_btnURL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCompanyUrl();
};

CAboutDlg::CAboutDlg() : CBCGPDialog(CAboutDlg::IDD)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
	ON_BN_CLICKED(IDC_COMPANY_URL, &CAboutDlg::OnBnClickedCompanyUrl)
END_MESSAGE_MAP()

// App command to run the dialog
void CSMS2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CSMS2App message handlers


void CSMS2App::PreLoadState()
{
}


void CAboutDlg::OnBnClickedCompanyUrl()
{
	CString strMsg;
	strMsg.Format("开发商：黄剑冰\r\n邮箱：nalido@yeah.net\r\n");
	MessageBox(strMsg);
}

//
//void CSMS2App::OnBeforeChangeVisualTheme(CBCGPAppOptions& appOptions, CWnd* pMainWnd)
//{
//	CBCGPWinApp::OnBeforeChangeVisualTheme(appOptions, pMainWnd);
//
//	switch (m_ActiveTheme)
//	{
//	case BCGP_VISUAL_THEME_OFFICE_2013_WHITE:
//		CBCGPVisualManager2013::SetAccentColor(CBCGPVisualManager2013::VS2012_Green);
//		break;
//
//	case BCGP_VISUAL_THEME_OFFICE_2013_GRAY:
//		CBCGPVisualManager2013::SetAccentColor(CBCGPVisualManager2013::VS2012_Orange);
//		break;
//
//	case BCGP_VISUAL_THEME_OFFICE_2013_DARK_GRAY:
//		CBCGPVisualManager2013::SetAccentColor(CBCGPVisualManager2013::VS2012_Blue);
//		break;
//
//	case BCGP_VISUAL_THEME_VS_2012_LIGHT:
//	case BCGP_VISUAL_THEME_VS_2012_DARK:
//	case BCGP_VISUAL_THEME_VS_2012_BLUE:
//		CBCGPVisualManagerVS2012::SetAccentColor(CBCGPVisualManagerVS2012::VS2012_Blue);	// Default
//	}
//}
//
//void CSMS2App::OnAfterChangeVisualTheme(CWnd* pMainWnd)
//{
//	CBCGPWinApp::OnAfterChangeVisualTheme(pMainWnd);
//
//	CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
//
//	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, pMainWnd);
//	if (pFrame->GetSafeHwnd() == NULL)
//	{
//		return;
//	}
//
//	//CSMS2View* pView = DYNAMIC_DOWNCAST(CSMS2View, pFrame->GetActiveView());
//	//if (pView->GetSafeHwnd())
//	//{
//	//	pView->GetManagerCtrl().AdjustLayout(TRUE);
//	//}
//}
