#pragma once

#include "MainFrm.h"

// CCoaches 窗体视图

class CCoaches : public CBCGPFormView
{
	DECLARE_DYNCREATE(CCoaches)

protected:
	CCoaches();           // 动态创建所使用的受保护的构造函数
	virtual ~CCoaches();

public:
	enum { IDD = IDD_COACHES };
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
	CDStrs m_arCoaches;
	CStatic m_wndCoachesLoc;
	CVirtualGridCtrl m_wndCoaches;
	virtual void OnInitialUpdate();
	static void CALLBACK OnGridClick(LPVOID lParam);
	void Refresh();
	afx_msg void OnBnClickedAddcoach();
};


