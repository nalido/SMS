#pragma once



#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\MyPrint.h"

// CViewBooking2 窗体视图

class CViewBooking2 : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewBooking2)

protected:
	CViewBooking2();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewBooking2();

public:
	enum { IDD = IDD_BOOKING2 };
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
	virtual void OnInitialUpdate();
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);


	//virtualGrid
	CStatic m_wndGridLocation1;
	CVirtualGridCtrl m_wndGrid1;
	CDStrs m_datas1;
	static void CALLBACK OnGrid1Click(LPVOID lParam);
	CStatic m_wndGridLocation2;
	CVirtualGridCtrl m_wndGrid2;
	CDStrs m_datas2;
	static void CALLBACK OnGrid2Click(LPVOID lParam);
	CStatic m_wndGridLocation3;
	CVirtualGridCtrl m_wndGrid3;
	CDStrs m_datas3;
	static void CALLBACK OnGrid3DbClick(LPVOID lParam);
	void Refresh();

	//派工日期选择 设置为只能选择今天或者明天
	CTime m_tToday, m_tTomorrow;
	BOOL m_isToday;
	afx_msg void OnBnClickedSelDay();

	//派工单
	xPublic::CMyPrint m_wndPrint;
};


