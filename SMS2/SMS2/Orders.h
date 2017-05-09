#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\MyPrint.h"

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
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	//virtualGrid
	CStatic m_wndGridLocation1;
	CVirtualGridCtrl m_wndGrid1;
	CDStrs m_datas1;
	static void CALLBACK OnGrid1Click(LPVOID lParam);

	BOOL m_enableMultiSel; //是否允许多选
	int m_index[3]; //标记同一个订单的学员
	std::vector<int> m_dataPrint; //记录是否已经打印过

	//
	CDateTimeCtrl m_Date_Start;
	CDateTimeCtrl m_Date_End;
	CString m_strToday;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedReset();
	void ResetStudent(int nRow);
	afx_msg void OnBnClickedPrint();


	xPublic::CMyPrint m_printx;
	xPublic::CLASSINFO m_classInfo;
	xPublic::SHEETINFO m_sheetInfo;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedPreprint();
};
