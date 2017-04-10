#pragma once



// CViewBooking1 窗体视图

class CViewBooking1 : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewBooking1)

protected:
	CViewBooking1();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewBooking1();

public:
	enum { IDD = IDD_BOOKING1 };
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
	afx_msg void OnBnClickedButton1();
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


