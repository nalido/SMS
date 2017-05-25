#pragma once


// CDlgState 对话框

class CDlgState : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgState)

public:
	CDlgState(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgState();

// 对话框数据
	enum { IDD = IDD_CARSTAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	int m_nStat;
	afx_msg void OnBnClickedOk();
};
