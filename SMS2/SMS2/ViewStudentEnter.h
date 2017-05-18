#pragma once



// CViewStudentEnter 窗体视图

class CViewStudentEnter : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewStudentEnter)

protected:
	CViewStudentEnter();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewStudentEnter();

public:
	enum { IDD = IDD_STUDENT_ENTER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	//背景定位
	CStatic m_BKpos1, m_BKpos2;
	CRect m_rctBK1, m_rctBK2;

	int m_nEraseBkgnd; //窗口重绘次数(根据控件个数决定)
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CStatic m_Content;
	CRect m_rctContent;

	CStatic m_S[4]; //静态文本框
	CString m_strName;
	CString m_strIDCard;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedConfirm();
	afx_msg void OnEnChangeE1();
	afx_msg void OnEnChangeE2();
};


