#pragma once

// CDlgNextBookDate 对话框

class CDlgNextBookDate : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNextBookDate)

public:
	CDlgNextBookDate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNextBookDate();

// 对话框数据
	enum { IDD = IDD_NEXT_BOOKDAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	CString m_strStuID;
	CEdit m_E;
	CString m_strOldDate;
	CDateTimeCtrl m_Date;

	afx_msg void OnBnClickedOk();
	void Refresh();
};
