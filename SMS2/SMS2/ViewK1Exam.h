#pragma once



// CViewK1Exam 窗体视图

class CViewK1Exam : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewK1Exam)

protected:
	CViewK1Exam();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewK1Exam();

public:
	enum { IDD = IDD_K1EXAM };
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


