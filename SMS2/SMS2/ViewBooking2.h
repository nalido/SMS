#pragma once



#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "SheetCtrl.h"

// CViewBooking2 窗体视图
typedef std::vector<int> Indexes;
typedef std::vector<Indexes> Indextable;

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
	CDStrs m_datas3; //车辆编号信息
	Indexes m_order; //记录当前派工预览的数据索引
	Indextable m_orderIndex; //派工单总览的信息索引
	static void CALLBACK OnOrdersClick(LPVOID lParam);

	void Refresh();

	//派工日期选择 设置为只能选择今天或者明天
	CTime m_tToday, m_tTomorrow;
	BOOL m_isToday;
	CStatic m_wndDate;
	CRect m_rectDate;
	afx_msg void OnBnClickedSelDay();

	//派工单
	CSheetCtrl m_wndPrint;
	afx_msg void OnBnClickedDoPrint();
//	afx_msg void OnStnClickedGrid1();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOrder();
	afx_msg void OnBnClickedResetPrint();

	CComboBox m_Combo_Cars;
	afx_msg void OnCbnSelchangeCars();
};


