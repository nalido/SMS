#pragma once

#include "PNGButton.h"

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
	BOOL m_canEraseBkgnd; //是否允许窗口重绘
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	CStatic m_SDevice[6];
	CStatic m_SText[6];
	ATL::CImage m_imgDevice;
	int m_nSelected; //鼠标选中的位置
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	COLORREF m_ThemeColor;
};


