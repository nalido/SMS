#pragma once
#include "afxwin.h"
class CPNGButton :
	public CStatic
{
public:
	CPNGButton();
	~CPNGButton();

	ATL::CImage m_PNG;
	CString m_strFilePath;
	BOOL m_isSelected;
	void SetPNGSource(CString & path);
	void SetState(BOOL isSelected);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

