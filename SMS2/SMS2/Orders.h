#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"

// COrders 对话框

class COrders : public CBCGPDialog
{
	DECLARE_DYNAMIC(COrders)

public:
	COrders(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrders();

// 对话框数据
	enum { IDD = IDD_ORDERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	//virtualGrid
	CStatic m_wndGridLocation1;
	CVirtualGridCtrl m_wndGrid1;
	CDStrs m_datas1;
	std::vector<int> m_dataPrint; //记录是否已经打印过

	//
	CDateTimeCtrl m_Date_Start;
	CDateTimeCtrl m_Date_End;
	CString m_strToday;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedPrint();
};
