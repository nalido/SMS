#pragma once


// CDlgQuitSchool 对话框

class CDlgQuitSchool : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgQuitSchool)

public:
	CDlgQuitSchool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuitSchool();

// 对话框数据
	enum { IDD = IDD_QUIT_SCHOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	void Refrsh();

	CString m_strInfo[10];

	CStatic m_S[10];
	CEdit m_E[10];
};
