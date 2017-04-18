#pragma once
#include "afxwin.h"


class CClassItem :
	public CStatic
{
public:
	CClassItem();
	~CClassItem();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	int m_nClass; //课程标号 1=>8:00-10:00 依次类推
	int m_nTotal; //可选总数
	int m_nUsed;  //已选数量
	BOOL m_nSelected; //是否选中当前控件
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

