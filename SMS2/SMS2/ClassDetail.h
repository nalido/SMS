#pragma once

#include "ClassItem.h"

// CClassDetail 对话框

class CClassDetail : public CBCGPDialog
{
	DECLARE_DYNAMIC(CClassDetail)

public:
	CClassDetail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClassDetail();

// 对话框数据
	enum { IDD = IDD_CLASSDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strDay;
	virtual BOOL OnInitDialog();

	CClassItem m_ClassItem[4];
	afx_msg void OnBnClickedConfirm();

	int m_nSelected; //选择结果 每天4节课 1111表示四节课全选 1000表示选中第一节课
};
