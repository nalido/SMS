#pragma once


// CDlgNoPass 对话框

class CDlgNoPass : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNoPass)

public:
	CDlgNoPass(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNoPass();

// 对话框数据
	enum { IDD = IDD_NOPASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strStuName;
	CString m_strReason;

	CStatic m_S1;
	

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
