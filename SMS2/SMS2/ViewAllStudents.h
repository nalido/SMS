#pragma once

#include "MainFrm.h"

// CViewAllStudents 窗体视图

class CViewAllStudents : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewAllStudents)

protected:
	CViewAllStudents();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewAllStudents();

public:
	enum { IDD = IDD_ALLSTUDENTS };
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
	CTabCtrl m_TabCtrl;
	CDateTimeCtrl m_DateS;
	CDateTimeCtrl m_DateE;
	CString m_strDateS;
	CString m_strDateE;

	//饼状图信息
	CStatic m_Pie;
	CRect m_rctPie;
	CStatic m_Label[2];
	CRect m_rctLabel[2];
	CStatic m_S[5];
	CString m_strLabel[2];
	int m_nPieTotal;
	std::vector<int> m_nPiePart;

	CStatic m_Info;
	CRect m_rctInfo;

	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid[5];
	std::vector<CString> m_arrColumns1;
	std::vector<CString> m_arrColumns2;
	std::vector<CString> m_arrColumns3;
	std::vector<CString> m_arrColumns4;
	std::vector<CString> m_arrColumns5;
	CDStrs m_datas1;
	CDStrs m_datas2;
	CDStrs m_datas3;
	CDStrs m_datas4;
	CDStrs m_datas5;
	void InitGrid(CVirtualGridCtrl* pGrid, CRect& rect, std::vector<CString>& arrColumns);
	void Refresh(int nID, CString strDateS, CString strDateE);
	void Analysis(int nID);

	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedQuit();
};


