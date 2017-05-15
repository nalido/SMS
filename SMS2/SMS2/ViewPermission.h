#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"


// CViewPermission 窗体视图

class CViewPermission : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewPermission)

protected:
	CViewPermission();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewPermission();

public:
	enum { IDD = IDD_PERMISSION };
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

	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	int m_nColumns;
	virtual void OnInitialUpdate();
	void Refresh();
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedDelstuff();
	afx_msg void OnBnClickedSelfdown();
};


