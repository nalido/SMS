#pragma once


#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
enum QUERY_TYPE2
{
	QUERY_OIL,  //油料管理
	QUERY_MAINTENANCE, //维保记录
	QUERY_MOT //年检记录
};
// CDlgDevice2 对话框

class CDlgDevice2 : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDevice2)

public:
	CDlgDevice2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDevice2();

// 对话框数据
	enum { IDD = IDD_DEVICE_DETAIL2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog();

	//virtualGrid
	//按天显示
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	int m_nOldRows; //数据库中的记录总数
	CDStrs m_newDatas; //新添加的记录
	int m_nColumns; //表的列数
	static void CALLBACK OnGridClick(LPVOID lParam);

	//按月显示
	CVirtualGridCtrl m_wndGridM;
	CDStrs m_datasM;

	//按年显示
	CVirtualGridCtrl m_wndGridY;
	CDStrs m_datasY;

	int m_nShowType; //显示方式 0 按天， 1 按月， 2 按年

	//车辆信息
	CStatic m_wndGridLocationC;
	CVirtualGridCtrl m_wndGridC;
	CDStrs m_datasC;
	CString m_strCarID; //选中的汽车编号
	CString m_strPlateNum; //车牌号
	static void CALLBACK OnGridCClick(LPVOID lParam);
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	int m_nQueryType;
	void RefreshCar();
	void Refresh();
	void CountData(); //统计年月记录
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedNewitem();
	afx_msg void OnBnClickedSave();
	void AddNewRowToDB(CStrs strs);
	void DelRowFromDB(CStrs strs);
	void GetNewData(); //获取用户输入的新数据
	afx_msg void OnBnClickedDelitem();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedProxy();
};
