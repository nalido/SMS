#pragma once

#include "SMS2Doc.h"
#include "SMS2View.h"
// CViewRegister ������ͼ

class CViewRegister : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewRegister)

protected:
	CViewRegister();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewRegister();

public:
	enum { IDD = IDD_REGISTER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


