#pragma once



// CSchool 窗体视图

class CSchool : public CBCGPFormView
{
	DECLARE_DYNCREATE(CSchool)

protected:
	CSchool();           // 动态创建所使用的受保护的构造函数
	virtual ~CSchool();

public:
	enum { IDD = IDD_SCHOOL };
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
	CString m_strSubForLeave; //教练缺勤一次减去的绩效
	CString m_strMinWorkTime; //教练一个月最少工时
	CString m_strK1Place; //科目一上课地址
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSave2();
	afx_msg void OnBnClickedSave3();
};


