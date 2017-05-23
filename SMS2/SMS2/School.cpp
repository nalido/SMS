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
	DDX_Text(pDX, IDC_K1PLACE, m_strK1Place);
	DDX_Text(pDX, IDC_MIN_K2CLASS, m_strMinK2Class);
	DDX_Text(pDX, IDC_MIN_K3CLASS, m_strMinK3Class);
	DDX_Text(pDX, IDC_FEEFORONECLASS, m_strFeeForOneClass);
}

BEGIN_MESSAGE_MAP(CSchool, CBCGPFormView)
	ON_BN_CLICKED(IDC_SAVE, &CSchool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SAVE2, &CSchool::OnBnClickedSave2)
	ON_BN_CLICKED(IDC_SAVE3, &CSchool::OnBnClickedSave3)
	ON_BN_CLICKED(IDC_SAVE4, &CSchool::OnBnClickedSave4)
	ON_BN_CLICKED(IDC_SAVE5, &CSchool::OnBnClickedSave5)
	ON_BN_CLICKED(IDC_SAVE6, &CSchool::OnBnClickedSave6)
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
	m_strK1Place = g_strK1Address;
	m_strMinK2Class.Format("%d", g_nMinK2Class);
	m_strMinK3Class.Format("%d", g_nMinK3Class);
	m_strFeeForOneClass.Format("%d", g_nFeeForOneClass);
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


void CSchool::OnBnClickedSave3()
{
	UpdateData();
	if (m_strK1Place.IsEmpty())
	{
		MessageBox("输入不合法！");
		m_strK1Place = g_strK1Address;
		UpdateData(FALSE);
		return;
	}

	g_strK1Address = m_strK1Place;
	xPublic::WRISTR2("SMS", "K1Address", g_strK1Address);
}


void CSchool::OnBnClickedSave4()
{
	UpdateData();
	if (m_strMinK2Class.IsEmpty())
	{
		MessageBox("输入不合法！");
		m_strMinK2Class.Format("%d", g_nMinK2Class);
		UpdateData(FALSE);
		return;
	}

	g_nMinK2Class = atoi(m_strMinK2Class);
	xPublic::WRIINT2("Students", "MinK2Class", g_nMinK2Class);
}


void CSchool::OnBnClickedSave5()
{
	UpdateData();
	if (m_strMinK3Class.IsEmpty())
	{
		MessageBox("输入不合法！");
		m_strMinK3Class.Format("%d", g_nMinK3Class);
		UpdateData(FALSE);
		return;
	}

	g_nMinK3Class = atoi(m_strMinK3Class);
	xPublic::WRIINT2("Students", "MinK3Class", g_nMinK3Class);
}


void CSchool::OnBnClickedSave6()
{
	UpdateData();
	if (m_strFeeForOneClass.IsEmpty())
	{
		MessageBox("输入不合法！");
		m_strFeeForOneClass.Format("%d", g_nFeeForOneClass);
		UpdateData(FALSE);
		return;
	}

	g_nFeeForOneClass = atoi(m_strFeeForOneClass);
	xPublic::WRIINT2("Students", "FeeForOneClass", g_nFeeForOneClass);
}
