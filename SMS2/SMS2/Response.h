#pragma once


// CResponse 对话框

class CResponse : public CBCGPDialog
{
	DECLARE_DYNAMIC(CResponse)

public:
	CResponse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResponse();

// 对话框数据
	enum { IDD = IDD_RESPONSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_nRspType; //反馈类型 0完成 1未完成
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();

	CString m_strServiceScore;
	CString m_strSelfScore;
};
