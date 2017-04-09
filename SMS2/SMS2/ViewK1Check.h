#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"


// CViewK1Check ������ͼ

class CViewK1Check : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewK1Check)

protected:
	CViewK1Check();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewK1Check();

public:
	enum { IDD = IDD_K1CHECK };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	void ListFresh();
	void Refresh();
	void RemoveData(int nRow);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedBtnPass();

	afx_msg void OnBnClickedBtnFresh();
	afx_msg void OnBnClickedBtnNopass();
};


