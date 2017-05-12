#pragma once


// CDlgDateItem 对话框

class CDlgDateItem : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDateItem)

public:
	CDlgDateItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDateItem();

// 对话框数据
	enum { IDD = IDD_DATE_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMcnSelectMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strDate;
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnInitDialog();
};
