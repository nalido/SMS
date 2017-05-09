#pragma once
#include "SheetCtrl.h"

// COrderDetail 对话框

class COrderDetail : public CBCGPDialog
{
	DECLARE_DYNAMIC(COrderDetail)

public:
	COrderDetail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrderDetail();

// 对话框数据
	enum { IDD = IDD_ORDERDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strStudent;		//学员档案号
	CString m_strDate;			//日期
	CString m_strClassID;		//课程时段

	CSheetCtrl m_wndPrint;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnPaint();
};
