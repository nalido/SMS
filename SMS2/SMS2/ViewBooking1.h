#pragma once


#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\CvvImage.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include "MyPicCtrl.h"
#include "WorkDay.h"
#include "GridCalendar.h"
#include "xPublic\MySQLEx.h"
// CViewBooking1 窗体视图


class CViewBooking1 : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewBooking1)

protected:
	CViewBooking1();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewBooking1();

public:
	enum { IDD = IDD_BOOKING1 };
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
//	afx_msg void OnBnClickedButton1();
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	static void CALLBACK OnGridClick(LPVOID lParam);
	void Refresh();

	CMyPicCtrl m_SPhoto;
	cv::Mat m_img;
	afx_msg void OnPaint();
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);

	//日历视图
	//CView* m_pView[91]; // 7*13
	CDStrs m_WDStat; //课时信息 work day state
	void UpdateDB(); //查询当前最大可预约时间，如果小于90天，则新建记录
	CStatic m_SWeek;
	CGridCalendar m_wndCalendar;
	static void CALLBACK OnCalendarClick(LPVOID lParam, BOOL lParam2);
	void UpdateCalendar();

};


