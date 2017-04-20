// ClassDetail.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ClassDetail.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CClassDetail 对话框

IMPLEMENT_DYNAMIC(CClassDetail, CBCGPDialog)

CClassDetail::CClassDetail(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CClassDetail::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_strDay = "";
	m_nSelected = 0;

	for (int i = 0; i < 4; i++)
		m_nStatus[i] = 0;
}

CClassDetail::~CClassDetail()
{
}

void CClassDetail::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLASS1, m_ClassItem[0]);
	DDX_Control(pDX, IDC_CLASS2, m_ClassItem[1]);
	DDX_Control(pDX, IDC_CLASS3, m_ClassItem[2]);
	DDX_Control(pDX, IDC_CLASS4, m_ClassItem[3]);
}


BEGIN_MESSAGE_MAP(CClassDetail, CBCGPDialog)
	ON_BN_CLICKED(IDC_CONFIRM, &CClassDetail::OnBnClickedConfirm)
END_MESSAGE_MAP()


// CClassDetail 消息处理程序


BOOL CClassDetail::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	SetWindowText(m_strDay);

	for (int i = 0; i < 4; i++)
	{
		m_ClassItem[i].m_nClass = i + 1;
		m_ClassItem[i].m_nTotal = g_nClassTotal;
		m_ClassItem[i].m_nUsed = m_nStatus[i];
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}



void CClassDetail::OnBnClickedConfirm()
{
	m_nSelected = m_ClassItem[0].m_nSelected * 1000 + m_ClassItem[1].m_nSelected * 100
		+ m_ClassItem[2].m_nSelected * 10 + m_ClassItem[3].m_nSelected;
	OnOK();
}
