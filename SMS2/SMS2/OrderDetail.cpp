// OrderDetail.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "OrderDetail.h"
#include "afxdialogex.h"


// COrderDetail �Ի���

IMPLEMENT_DYNAMIC(COrderDetail, CBCGPDialog)

COrderDetail::COrderDetail(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(COrderDetail::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_strClassID = "";
	m_strDate = "";
	m_strStudent = "";
}

COrderDetail::~COrderDetail()
{
}

void COrderDetail::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_wndPrint);
}


BEGIN_MESSAGE_MAP(COrderDetail, CBCGPDialog)
	ON_BN_CLICKED(IDC_PRINT, &COrderDetail::OnBnClickedPrint)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// COrderDetail ��Ϣ�������


BOOL COrderDetail::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void COrderDetail::OnBnClickedPrint()
{
	m_wndPrint.Invalidate();
}


void COrderDetail::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
}
