#pragma once
#include "MainFrm.h"

// CDlgMontPlate 对话框

class CDlgMontPlate : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgMontPlate)

public:
	CDlgMontPlate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMontPlate();

// 对话框数据
	enum { IDD = IDD_MONTH_PLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCoachID;
	CString m_strMonth; 
	CTime m_tToday;

	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;

	CStatic	m_calendarLocation;
	CBCGPCalendar  m_wndCalendarCtrl; 
	CArray<DATE, DATE&> m_arMarkedDates;

	virtual BOOL OnInitDialog();
	void Refresh();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDel();
};
