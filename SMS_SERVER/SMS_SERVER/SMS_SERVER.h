// SMS_SERVER.h : main header file for the SMS_SERVER application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CSMS_SERVERApp:
// See SMS_SERVER.cpp for the implementation of this class
//

class CSMS_SERVERApp : public CBCGPWinApp
{
public:
	CSMS_SERVERApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSMS_SERVERApp theApp;