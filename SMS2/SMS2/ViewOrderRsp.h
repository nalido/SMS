#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"



// CViewOrderRsp 窗体视图

class CViewOrderRsp : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewOrderRsp)

protected:
	CViewOrderRsp();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewOrderRsp();

public:
	enum { IDD = IDD_ORDERS_RSP };
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
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	virtual void OnInitialUpdate(); 
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	static void CALLBACK OnGridDbClick(LPVOID lParam);

	CString m_strToday;

	void Refresh();

	//更新教练员出勤情况, 
	//flag=0 正常出勤，出勤表添加一条记录， 绩效加一, 工时加2小时
	//flag=1 缺勤，缺勤表添加一条记录，缺勤计数加一， 绩效减8
	void UpdateCoach(int nRow, int flag = 0); 

	//更新学员进度
	//flag=0 正常完成训练， bookings表FLAG字段更新为“2”， 已上课时数加一
	//flag=1 缺勤，FLAG字段更新为“-1”， 已上课时数不变
	void UpdateStudent(int nRow, int flag = 0);
	afx_msg void OnBnClickedRespon();
};


