#pragma once



// CViewDevices 窗体视图

class CViewDevices : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewDevices)

protected:
	CViewDevices();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewDevices();

public:
	enum { IDD = IDD_DEVICES };
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
	afx_msg void OnPaint();
	void DrawIcon(ATL::CImage& img, int nID);

	CStatic m_SDevice[6];
	CStatic m_SText[6];
	CImage m_imgDevice;
	int m_nSelected; //鼠标选中的位置
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
};


