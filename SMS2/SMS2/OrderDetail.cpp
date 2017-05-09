// OrderDetail.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "OrderDetail.h"
#include "afxdialogex.h"


// COrderDetail 对话框

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


// COrderDetail 消息处理程序


BOOL COrderDetail::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void COrderDetail::OnBnClickedPrint()
{
	xPublic::CMyPrint printx;
	printx.PrinterInit(&m_wndPrint.m_sheetInfo, &m_wndPrint.m_classInfo);
	printx.m_students = m_wndPrint.m_printData.m_students;
	CString strMsg;
	printx.Printx(1, strMsg);
	OnOK(); //打印完退出
}


void COrderDetail::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
}
