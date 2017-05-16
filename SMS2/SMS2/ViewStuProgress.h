#pragma once


#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"

// CViewStuProgress 窗体视图

class CViewStuProgress : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewStuProgress)

protected:
	CViewStuProgress();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewStuProgress();

public:
	enum { IDD = IDD_STUPROGRESS };
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

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	void Refresh();
	void ListFresh();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedStufresh();
	afx_msg void OnBnClickedSendbookmsg();
	static void CALLBACK OnGridDbClick(LPVOID lParam);
	afx_msg void OnBnClickedTobook();
	afx_msg void OnBnClickedScan();
	afx_msg void OnBnClickedSettype();
};


