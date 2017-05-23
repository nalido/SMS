#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"


enum DLG_TYPE
{
	DLG_RESP,  //课程评分
	DLG_MONEY //缴费明细
};
// CDlgMyBooks 对话框

class CDlgMyBooks : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgMyBooks)

public:
	CDlgMyBooks(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMyBooks();

// 对话框数据
	enum { IDD = IDD_MYBOOKS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_nDlgType; 
	BOOL m_isPublic; //1:不显示缴费按钮 0:显示缴费按钮

	CString m_strMsg;
	int m_nSumM; //总未缴费金额

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	static void CALLBACK OnGridDbClick(LPVOID lParam);
	void Refresh();
	void UpdateCoach(int nRow, int flag = 0);
	void UpdateStudent(int nRow, int flag = 0);

	CString m_strToday;
	CString m_strStuID;

	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	afx_msg void OnBnClickedMoney();
};
