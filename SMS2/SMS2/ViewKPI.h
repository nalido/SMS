#pragma once

#include "MainFrm.h"
#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"


// CViewKPI 窗体视图

class CViewKPI : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewKPI)

protected:
	CViewKPI();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewKPI();

public:
	enum { IDD = IDD_KPI };
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
	std::map<CString, int> m_arrAdmonish; //first:coach_ID, second:是否处理，0=未打印， 1=已打印未处理， 2=已反馈可激活
	std::vector<CString> m_arrColumns;
	virtual void OnInitialUpdate();

	CTime m_tToday;
	void Refresh();
	afx_msg void OnBnClickedExport();

	//查询子线程
	xPublic::CThreadBase m_threadMySQL;
	static void CALLBACK ThreadMySQLCallback(LPVOID pParam, HANDLE hCloseEvent);
	afx_msg void OnBnClickedTalk();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedUpdate();
};


