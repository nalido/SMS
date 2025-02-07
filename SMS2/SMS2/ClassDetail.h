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

	//课时选择状况, [][1]为是否选择， [][0]为课程预约数量
	int m_nStatus[4][2]; 

	CClassItem m_ClassItem[4];
	afx_msg void OnBnClickedConfirm();

	int m_nSelected; //选择结果 每天4节课 1111表示四节课全选 1000表示选中第一节课
};
