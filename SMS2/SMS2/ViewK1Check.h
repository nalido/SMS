#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"


// CViewK1Check 窗体视图

class CViewK1Check : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewK1Check)

protected:
	CViewK1Check();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewK1Check();

public:
	enum { IDD = IDD_K1CHECK };
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
	//virtualGrid
	CStatic m_wndGridLocation;
	CStatic m_wndGridLocation_pass;
	CStatic m_wndGridLocation_nopass;
	CVirtualGridCtrl m_wndGrid;
	CVirtualGridCtrl m_wndGrid_pass;
	CVirtualGridCtrl m_wndGrid_nopass;
	CDStrs m_datas;
	CDStrs m_datas_pass;
	CDStrs m_datas_nopass;
	void InitList(CVirtualGridCtrl* pGrid, CRect& rect);
	void ListFresh();
	void Refresh(BOOL isInit = FALSE);
	void RemoveData(int type, int nRow);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedBtnPass();

	afx_msg void OnBnClickedBtnFresh();
	afx_msg void OnBnClickedBtnNopass();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReturn1();
	afx_msg void OnBnClickedReturn2();
	afx_msg void OnBnClickedBtnSms1();
	afx_msg void OnBnClickedBtnSms2();
	afx_msg void OnBnClickedBtnFresh2();
	afx_msg void OnBnClickedNewStudent();

	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);
	afx_msg void OnBnClickedDelStudent();
};


