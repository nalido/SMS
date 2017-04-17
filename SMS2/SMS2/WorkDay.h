#pragma once



// CWorkDay 窗体视图

class CWorkDay : public CBCGPFormView
{
	DECLARE_DYNCREATE(CWorkDay)

protected:
	CWorkDay();           // 动态创建所使用的受保护的构造函数
	virtual ~CWorkDay();

public:
	enum { IDD = IDD_WORKDAY };
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
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();

	CStatic m_SHeader;
	CStatic m_SDate;
	CStatic m_SDay;
	CStatic m_SDay4;
	int m_nDayWid;
	int m_nDayHei;
	int m_nHeadHei;
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void OnClick(int row, int col);
	void GetDayRect(int row, int col, LPRECT lpRect);
};


