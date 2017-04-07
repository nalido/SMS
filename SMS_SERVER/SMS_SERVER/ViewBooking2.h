#pragma once



// CViewBooking2 窗体视图

class CViewBooking2 : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewBooking2)

protected:
	CViewBooking2();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewBooking2();

public:
	enum { IDD = IDD_BOOKING2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


