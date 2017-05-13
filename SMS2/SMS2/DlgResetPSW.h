#pragma once


// CDlgResetPSW 对话框

class CDlgResetPSW : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgResetPSW)

public:
	CDlgResetPSW(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResetPSW();

// 对话框数据
	enum { IDD = IDD_RESETPSW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSms();
	afx_msg void OnBnClickedConfirm();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

	CStatic m_S[6]; //静态文本框
	CString m_strTel;
	CString m_strSMS; //填写的验证码
	CString m_strPSW1;
	CString m_strPSW2;

	CString m_strSMS0; //发送的验证码
	int m_nCount; //验证码发送倒计时
	afx_msg void OnBnClickedReset();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeE3();
	afx_msg void OnEnChangeE4();
};
