//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2014 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
#if !defined(AFX_VIRTUALGRIDCTRL_H__4ACA55A9_2297_4484_A5FD_4227754ABE8D__INCLUDED_)
#define AFX_VIRTUALGRIDCTRL_H__4ACA55A9_2297_4484_A5FD_4227754ABE8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualGridCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVirtualGridCtrl window
#include "BCGPGridCtrl.h"
#include "..\xPublic\MySQLEx.h"

class CVirtualGridCtrl : public CBCGPGridCtrl
{
	typedef void (CALLBACK *ONWORK)(LPVOID lParam); //回调函数
// Construction
public:
	CVirtualGridCtrl();

// Attributes
protected:
	CBCGPGridRange	m_rangePrint;

// Operations
public:
	int CalcRowHeaderWidth (int nRowCount);

	void SetPrintRange (int nFirstRow, int nLastRow);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualGridCtrl)
	//}}AFX_VIRTUAL

	virtual void OnItemChanged (CBCGPGridItem* pItem, int nRow, int nColumn);
	virtual void OnPreparePrintPages (CPrintInfo* pInfo, int nFirstItem, int nLastItem);

// Implementation
public:
	virtual ~CVirtualGridCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVirtualGridCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point); 
	ONWORK m_OnWork_DbClick;
	ONWORK m_OnWork_Click;
	void SetCallBack_DblClk(ONWORK noWork); //注册双击鼠标回调函数
	void SetCallBack_Clk(ONWORK noWork);
	virtual void OnSelChanged(const CBCGPGridRange&range, BOOL bSelect);

	void GridRefresh(int nCount);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALGRIDCTRL_H__4ACA55A9_2297_4484_A5FD_4227754ABE8D__INCLUDED_)
