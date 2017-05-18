// SMS2View.h : interface of the CSMS2View class
//


#pragma once


class CSMS2View : public CBCGPFormView
{
protected: // create from serialization only
	CSMS2View();
	DECLARE_DYNCREATE(CSMS2View)

public:
	enum{ IDD = IDD_SMS2_FORM };

// Attributes
public:
	CSMS2Doc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CSMS2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);

//	afx_msg void OnPaint();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int m_nEraseBkgnd; //窗口重绘次数(根据控件个数决定)
//	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
};

#ifndef _DEBUG  // debug version in SMS2View.cpp
inline CSMS2Doc* CSMS2View::GetDocument() const
   { return reinterpret_cast<CSMS2Doc*>(m_pDocument); }
#endif

