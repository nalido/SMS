#pragma once

#include "MainFrm.h"

// CViewK1Exam 窗体视图

class CViewK1Exam : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewK1Exam)

protected:
	CViewK1Exam();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewK1Exam();

public:
	enum { IDD = IDD_K1EXAM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);

	//virtualGrid
	CStatic m_wndGridLocation1;
	CVirtualGridCtrl m_wndGrid1;
	CDStrs m_datas1;
	int m_nColumns1; 
	void Refresh();

	CStatic m_wndGridLocation2;
	CVirtualGridCtrl m_wndGrid2;
	CDStrs m_datas2;
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedPass();
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedSendsms();
	afx_msg void OnBnClickedK1pass();
	afx_msg void OnBnClickedUpdate();
};


