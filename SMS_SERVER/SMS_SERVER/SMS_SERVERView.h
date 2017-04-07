// SMS_SERVERView.h : interface of the CSMS_SERVERView class
//


#pragma once


class CSMS_SERVERView : public CBCGPFormView
{
protected: // create from serialization only
	CSMS_SERVERView();
	DECLARE_DYNCREATE(CSMS_SERVERView)

public:
	enum{ IDD = IDD_SMS_SERVER_FORM };

// Attributes
public:
	CSMS_SERVERDoc* GetDocument() const;

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
	virtual ~CSMS_SERVERView();
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in SMS_SERVERView.cpp
inline CSMS_SERVERDoc* CSMS_SERVERView::GetDocument() const
   { return reinterpret_cast<CSMS_SERVERDoc*>(m_pDocument); }
#endif

