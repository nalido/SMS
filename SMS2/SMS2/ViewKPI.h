#pragma once

#include "MainFrm.h"
#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"


// CViewKPI ������ͼ

class CViewKPI : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewKPI)

protected:
	CViewKPI();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewKPI();

public:
	enum { IDD = IDD_KPI };
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
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	std::map<CString, int> m_arrAdmonish; //first:coach_ID, second:�Ƿ���0=δ��ӡ�� 1=�Ѵ�ӡδ���� 2=�ѷ����ɼ���
	std::vector<CString> m_arrColumns;
	virtual void OnInitialUpdate();

	CTime m_tToday;
	void Refresh();
	afx_msg void OnBnClickedExport();

	//��ѯ���߳�
	xPublic::CThreadBase m_threadMySQL;
	static void CALLBACK ThreadMySQLCallback(LPVOID pParam, HANDLE hCloseEvent);
	afx_msg void OnBnClickedTalk();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedUpdate();
};


