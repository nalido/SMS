#pragma once


// CMSGINFO 对话框

class CMSGINFO : public CBCGPDialog
{
	DECLARE_DYNAMIC(CMSGINFO)

public:
	CMSGINFO(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMSGINFO();

// 对话框数据
	enum { IDD = IDD_SMSINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_nFlag; //短信类型标志
	CString m_strSMS;
	virtual BOOL OnInitDialog();
	void Init(int flag);
};
