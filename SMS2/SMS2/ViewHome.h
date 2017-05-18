#pragma once



// CViewHome 窗体视图

class CViewHome : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewHome)

protected:
	CViewHome();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewHome();

public:
	enum { IDD = IDD_HOME };
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	int m_nEraseBkgnd; //窗口重绘次数(根据控件个数决定)
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	//背景定位
	CStatic m_BKpos1, m_BKpos2;
	CRect m_rctBK1, m_rctBK2;
	virtual void OnInitialUpdate();
};


