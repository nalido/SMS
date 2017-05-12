// School.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "School.h"
#include "MainFrm.h"


// CSchool

IMPLEMENT_DYNCREATE(CSchool, CBCGPFormView)

CSchool::CSchool()
	: CBCGPFormView(CSchool::IDD)
{
	EnableVisualManagerStyle();
}

CSchool::~CSchool()
{
}

void CSchool::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NSUBFORLEAVE, m_strSubForLeave);
	DDX_Text(pDX, IDC_MIN_WORK_TIME, m_strMinWorkTime);
}

BEGIN_MESSAGE_MAP(CSchool, CBCGPFormView)
	ON_BN_CLICKED(IDC_SAVE, &CSchool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SAVE2, &CSchool::OnBnClickedSave2)
END_MESSAGE_MAP()


// CSchool 诊断

#ifdef _DEBUG
void CSchool::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSchool::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSchool 消息处理程序


void CSchool::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	m_strSubForLeave.Format("%d", g_nSubForLeave);
	m_strMinWorkTime.Format("%d", g_nMinWorkTime);
	UpdateData(FALSE);
}


void CSchool::OnBnClickedSave()
{
	UpdateData();
	int tmp = atoi(m_strSubForLeave);
	if (tmp == 0 || m_strSubForLeave.IsEmpty())
	{
		MessageBox("输入不合法！"); 
		m_strSubForLeave.Format("%d", g_nSubForLeave);
		UpdateData(FALSE);
		return;
	}

	g_nSubForLeave = atoi(m_strSubForLeave);
	xPublic::WRIINT2("Coach", "SubForLeave", g_nSubForLeave);
}


void CSchool::OnBnClickedSave2()
{
	UpdateData();
	int tmp = atoi(m_strMinWorkTime);
	if (tmp == 0 || m_strMinWorkTime.IsEmpty())
	{
		MessageBox("输入不合法！");
		m_strMinWorkTime.Format("%d", g_nMinWorkTime);
		UpdateData(FALSE);
		return;
	}

	g_nMinWorkTime = atoi(m_strMinWorkTime);
	xPublic::WRIINT2("Coach", "SubForLeave", g_nSubForLeave);
}
