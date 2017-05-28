#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
enum CHECK_TYPE
{
	CHECK_CHUQIN,
	CHECK_QUEQIN,
	CHECK_WORKTIME,
	CHECK_JIXIAO,
	CHECK_KPI
};
// CDlgCoachCheck 对话框

class CDlgCoachCheck : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgCoachCheck)

public:
	CDlgCoachCheck(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCoachCheck();

// 对话框数据
	enum { IDD = IDD_COACH_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_nCheckType; //考核项目
	CString m_strCoach;
	CString m_strCoachID; 
	int m_nLeave; //缺勤次数
	int m_nStudents; //本月已培训学员数


	virtual BOOL OnInitDialog();


	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	CVirtualGridCtrl m_wndGridM;
	CDStrs m_datasM;
	CVirtualGridCtrl m_wndGridY;
	CDStrs m_datasY;
	std::vector<CString> m_arrColumns;
	std::vector<CString> m_arrColumnsM;

	CDateTimeCtrl m_DateS;
	CDateTimeCtrl m_DateE;


	int m_nQueryType; //查询方式

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();

	void Refresh();
	void GetJiXiao(); //获取本月绩效
	void GetKPI(); //获取本月KPI
	void RemadeData(CDStrs& datas); //根据不同的类型重新整理查询到的数据
	afx_msg void OnBnClickedQuery();
//	afx_msg void OnBnClickedSettoday();

	//统计信息
	CStatic m_Static_Msg1;
	CStatic m_Static_Msg2;
	CStatic m_Static_Msg3;
	CRect m_rectMsg1;
	CRect m_rectMsg2;
	CRect m_rectMsg3;
	CString m_strMsg1;
	CString m_strMsg2;
	CString m_strMsg20;
	CString m_strMsg3;
	void UpdateCStatic();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedExport();
};
