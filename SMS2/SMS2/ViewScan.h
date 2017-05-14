#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\CvvImage.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include "MyPicCtrl.h"
#include "MainFrm.h"

// CViewScan 窗体视图

class CViewScan : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewScan)

protected:
	CViewScan();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewScan();

public:
	enum { IDD = IDD_SCAN };
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

	BOOL m_canEraseBkgnd;
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	//背景定位
	CStatic m_BKpos1, m_BKpos2;
	CRect m_rctBK1, m_rctBK2;

	CStatic m_Content;
	CRect m_rctContent;


	CStatic m_BasicInfo;
	CRect m_rctBasicInfo;
	CStatic m_BasicInfo2;
	CRect m_rctBasicInfo2;
	CStatic m_BasicInfo3;
	CRect m_rctBasicInfo3;
	CStatic m_BasicInfo4;
	CRect m_rctBasicInfo4;
	CStatic m_BasicInfo5;
	CRect m_rctBasicInfo5;

	CStatic m_Pie; //进度饼图
	CRect m_rctPie;

	//字体控制
	CStatic m_S[11]; //静态文本框
	CStatic m_T[5]; //栏目标题
	CStatic m_D[13]; //信息文本框

	//列表
	CStatic m_wndGridLocation1;
	CStatic m_wndGridLocation2;
	CStatic m_wndGridLocation3;
	CVirtualGridCtrl m_wndGrid1;
	CVirtualGridCtrl m_wndGrid2;
	CVirtualGridCtrl m_wndGrid3;
	CDStrs m_datas1;
	CDStrs m_datas2;
	CDStrs m_datas3;
	void GridInit(std::vector<CString>& arrColumns, CRect& rect, CVirtualGridCtrl* pGrid);


	//学生信息
	BOOL m_isPublic; //1:不显示隐私信息 0:驾校员工查看，显示全部信息
	STUDENTINFO m_student;
	CMyPicCtrl m_SPhoto;
	cv::Mat m_img;
	CString m_strMaxClass; //全部课时
	CString m_strBookedClass; //已预约课时数
	CString m_strUsedClass; //已使用课时数
	CString m_strClassType; //正在进行的课程类别

	//打开此视图的上一个视图
	UINT m_LAST_VIEW;

	afx_msg void OnBnClickedBook();
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void Refresh();
	afx_msg void OnBnClickedExit();
};


