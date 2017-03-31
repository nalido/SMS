// SMS2.h : main header file for the SMS2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CSMS2App:
// See SMS2.cpp for the implementation of this class
//

class CSMS2App : public CBCGPWinApp
{
public:
	CSMS2App();

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

extern CSMS2App theApp;