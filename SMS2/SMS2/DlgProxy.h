#pragma once


// CDlgProxy 对话框

class CDlgProxy : public CDialog
{
	DECLARE_DYNAMIC(CDlgProxy)

public:
	CDlgProxy(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProxy();

// 对话框数据
	enum { IDD = IDD_PROXY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
