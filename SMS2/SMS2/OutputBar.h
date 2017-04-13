// outputbar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\CObjectBase.h"

class COutputBar : public CBCGPDockingControlBar
{
public:
	COutputBar();

// Attributes
protected:
	CBCGPTabWnd	m_wndTabs;
	
	CBCGPListCtrl	m_wndList1;
	CBCGPListCtrl	m_wndList2;
	CBCGPListCtrl	m_wndList3;

public:
	CVirtualGridCtrl m_wndGrid;
	xPublic::CObjectBase m_datas;
	void ListFresh();
	void AddItem2List4(CString str);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputBar();

// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void AddItem2List1(CString strMsg);
};

/////////////////////////////////////////////////////////////////////////////
