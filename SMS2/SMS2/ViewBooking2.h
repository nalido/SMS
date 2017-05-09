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
	static void CALLBACK OnGrid1DbClick(LPVOID lParam);
	CStatic m_wndGridLocation2;
	CVirtualGridCtrl m_wndGrid2;
	CDStrs m_datas2;
	static void CALLBACK OnGrid2Click(LPVOID lParam);

	CStatic m_wndGridLocation3;
	CVirtualGridCtrl m_wndGrid3;
	CDStrs m_datas3; //车辆编号信息
	Indexes m_order; //记录当前派工预览的数据索引
	Indextable m_orderIndexes; //派工单总览的信息索引
	static void CALLBACK OnOrdersClick(LPVOID lParam);
	BOOL m_canChangeOrder; //是否可以修改派工
	int m_nChangingOrderIndex; //正在修改的派工序号
	afx_msg void OnBnClickedOrderChange();

	//type 0 普通模式下的重置，清空所有状态 
	//type 1 点击确定派工后的重置，保留学员选中状态
	void RestOrder(int type = 0);

	//选中第一个学员后对剩下学员进行刷选，以颜色区分, 以授教内容分类
	std::vector<BYTE> m_arrClassIndex;

	//判断选择的学生是否可以加入派工单
	BOOL CanBeSelected(int nRow); 

	//获取列表中所有学员的授教内容序号
	void GetClassIndex();

	void Refresh(int nID = 0);

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
	afx_msg void OnBnClickedOrderQuery();
	afx_msg void OnBnClickedAutoOrder();
	afx_msg void OnBnClickedTmpOrder();
};


