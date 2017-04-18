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
	int m_nClass; //�γ̱�� 1=>8:00-10:00 ��������
	int m_nTotal; //��ѡ����
	int m_nUsed;  //��ѡ����
	BOOL m_nSelected; //�Ƿ�ѡ�е�ǰ�ؼ�
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

