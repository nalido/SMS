#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
enum QUERY_TYPE
{
	QUERY_DEVICES,  //设备台账表
	QUERY_INSURANCES, //保险记录表
	QUERY_CLAIMS //理赔记录表
};

// CDlgDevice 对话框

class CDlgDevice : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDevice)

public:
	CDlgDevice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDevice();

// 对话框数据
	enum { IDD = IDD_DEVICE_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	int m_nOldRows; //数据库中的记录总数
	CDStrs m_newDatas; //新添加的记录
	int m_nColumns; //表的列数
	virtual BOOL OnInitDialog();
	static void CALLBACK OnGridClick(LPVOID lParam);

	int m_nQueryType;
	void Refresh();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedNewitem();
	afx_msg void OnBnClickedSave();
	void AddNewRowToDB(CStrs strs);
	void DelRowFromDB(CStrs strs);
	void GetNewData(); //获取用户输入的新数据
	afx_msg void OnBnClickedDelitem();
};
