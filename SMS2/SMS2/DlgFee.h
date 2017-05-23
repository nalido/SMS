#pragma once


// CDlgFee 对话框

class CDlgFee : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgFee)

public:
	CDlgFee(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFee();

// 对话框数据
	enum { IDD = IDD_FEE_UP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
