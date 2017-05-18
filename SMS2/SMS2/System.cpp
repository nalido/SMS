// System.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "System.h"
#include "MainFrm.h"
#include "xPublic\Common.h"


// CSystem

IMPLEMENT_DYNCREATE(CSystem, CBCGPFormView)

CSystem::CSystem()
	: CBCGPFormView(CSystem::IDD)
{
	EnableVisualManagerStyle();
	m_strFilePath = "";
	m_strOutPath = "";
}

CSystem::~CSystem()
{
}

void CSystem::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT_FILEPATH2, m_strOutPath);
}

BEGIN_MESSAGE_MAP(CSystem, CBCGPFormView)
	ON_BN_CLICKED(IDC_FILEPATH, &CSystem::OnBnClickedFilepath)
	ON_BN_CLICKED(IDC_CHANGEPATH, &CSystem::OnBnClickedChangepath)
	ON_BN_CLICKED(IDC_CHANGEBACK, &CSystem::OnBnClickedChangeback)
	ON_BN_CLICKED(IDC_FILEPATH2, &CSystem::OnBnClickedFilepath2)
	ON_BN_CLICKED(IDC_CHANGEPATH2, &CSystem::OnBnClickedChangepath2)
	ON_BN_CLICKED(IDC_CHANGEBACK2, &CSystem::OnBnClickedChangeback2)
END_MESSAGE_MAP()


// CSystem 诊断

#ifdef _DEBUG
void CSystem::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSystem::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSystem 消息处理程序


void CSystem::OnBnClickedFilepath()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择一个文件夹：");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			m_strFilePath = szFolderPath;
			UpdateData(FALSE);
			if (m_strFilePath != g_strFilePath)
			{
				GetDlgItem(IDC_CHANGEPATH)->EnableWindow(TRUE);
				GetDlgItem(IDC_CHANGEBACK)->EnableWindow(TRUE);
			}
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
}


void CSystem::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	GetDlgItem(IDC_CHANGEPATH)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGEBACK)->EnableWindow(FALSE);
	m_strFilePath = g_strFilePath;

	GetDlgItem(IDC_CHANGEPATH2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGEBACK2)->EnableWindow(FALSE);
	m_strOutPath = g_strOutPath;

	UpdateData(FALSE);
}


void CSystem::OnBnClickedChangepath()
{
	g_strFilePath = m_strFilePath;
	xPublic::WRISTR2("Option", "PicSavePath", g_strFilePath);
	GetDlgItem(IDC_CHANGEPATH)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGEBACK)->EnableWindow(FALSE);
}


void CSystem::OnBnClickedChangeback()
{
	m_strFilePath = g_strFilePath;
	UpdateData(FALSE);
}



void CSystem::OnBnClickedFilepath2()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择一个文件夹：");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			m_strOutPath = szFolderPath;
			UpdateData(FALSE);
			if (m_strOutPath != g_strOutPath)
			{
				GetDlgItem(IDC_CHANGEPATH2)->EnableWindow(TRUE);
				GetDlgItem(IDC_CHANGEBACK2)->EnableWindow(TRUE);
			}
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
}


void CSystem::OnBnClickedChangepath2()
{
	g_strOutPath = m_strOutPath;
	xPublic::WRISTR2("Option", "FileSavePath", g_strOutPath);
	GetDlgItem(IDC_CHANGEPATH2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGEBACK2)->EnableWindow(FALSE);
}


void CSystem::OnBnClickedChangeback2()
{
	m_strOutPath = g_strOutPath;
	UpdateData(FALSE);
}
