#pragma once


// CDlgNextClass 对话框

class CDlgNextClass : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNextClass)

public:
	CDlgNextClass(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNextClass();

// 对话框数据
	enum { IDD = IDD_NEXTCLASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CStatic m_S[11]; //静态文本框
	CComboBox m_Comb;
	CString m_strSelectedClass; //选中的课时
	std::vector<CString> m_arrClassText;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
};
