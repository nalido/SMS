// School.cpp : ʵ���ļ�
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
}

BEGIN_MESSAGE_MAP(CSchool, CBCGPFormView)
	ON_BN_CLICKED(IDC_SAVE, &CSchool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SAVE2, &CSchool::OnBnClickedSave2)
	ON_BN_CLICKED(IDC_SAVE3, &CSchool::OnBnClickedSave3)
END_MESSAGE_MAP()


// CSchool ���

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


// CSchool ��Ϣ�������


void CSchool::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	m_strSubForLeave.Format("%d", g_nSubForLeave);
	m_strMinWorkTime.Format("%d", g_nMinWorkTime);
	m_strK1Place = g_strK1Address;
	UpdateData(FALSE);
}


void CSchool::OnBnClickedSave()
{
	UpdateData();
	int tmp = atoi(m_strSubForLeave);
	if (tmp == 0 || m_strSubForLeave.IsEmpty())
	{
		MessageBox("���벻�Ϸ���"); 
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
		MessageBox("���벻�Ϸ���");
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
		MessageBox("���벻�Ϸ���");
		m_strK1Place = g_strK1Address;
		UpdateData(FALSE);
		return;
	}

	g_strK1Address = m_strK1Place;
	xPublic::WRISTR2("SMS", "K1Address", g_strK1Address);
}
