#pragma once



// CSystem 窗体视图

class CSystem : public CBCGPFormView
{
	DECLARE_DYNCREATE(CSystem)

protected:
	CSystem();           // 动态创建所使用的受保护的构造函数
	virtual ~CSystem();

public:
	enum { IDD = IDD_SYSTEM };
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
	afx_msg void OnBnClickedFilepath();
	virtual void OnInitialUpdate();

	CString m_strFilePath;
	afx_msg void OnBnClickedChangepath();
	afx_msg void OnBnClickedChangeback();
	afx_msg void OnBnClickedButton1();
};


