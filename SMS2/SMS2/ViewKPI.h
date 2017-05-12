#pragma once



// CViewKPI 窗体视图

class CViewKPI : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewKPI)

protected:
	CViewKPI();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewKPI();

public:
	enum { IDD = IDD_KPI };
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


