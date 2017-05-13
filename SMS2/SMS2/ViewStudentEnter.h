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
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
};


