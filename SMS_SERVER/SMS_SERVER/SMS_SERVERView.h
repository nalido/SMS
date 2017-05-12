// SMS_SERVERView.h : interface of the CSMS_SERVERView class
//


#pragma once

#include "xPublic\netSocket.h"
#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\CObjectBase.h"

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


	xPublic::CTCPServer		m_tcpServer;
	UINT					m_nHostPort;
	CImageList				m_TreeImages;
	xPublic::CTCPTree		m_cTreeCtrl;
	static void CALLBACK OnTCPAccept(LPVOID lParam, xPublic::CTCPClient *pSocket, BOOL bNew);
	static void CALLBACK OnTCPRecive(LPVOID lParam, xPublic::CTCPClient *pSocket);
	static void CALLBACK OnTCPClosed(LPVOID lParam, xPublic::CTCPClient *pSocket);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	xPublic::CObjectBase m_arMsg; 
	void ListFresh();

	//保存图片
	void SaveBmp(char* FileNum, BYTE* picBuf, int wid, int hei, int imgSize);

	//短信平台
	void SendSMS(BYTE flag, vector<CString>& vFiles); //flag: 1-开班短信 2-退款短信
};

#ifndef _DEBUG  // debug version in SMS_SERVERView.cpp
inline CSMS_SERVERDoc* CSMS_SERVERView::GetDocument() const
   { return reinterpret_cast<CSMS_SERVERDoc*>(m_pDocument); }
#endif

