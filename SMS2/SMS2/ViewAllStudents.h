#pragma once

#include "MainFrm.h"

// CViewAllStudents ������ͼ

class CViewAllStudents : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewAllStudents)

protected:
	CViewAllStudents();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewAllStudents();

public:
	enum { IDD = IDD_ALLSTUDENTS };
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
	CTabCtrl m_TabCtrl;
	CTabCtrl m_TabShowType; //������·���ʾ
	CDateTimeCtrl m_DateS;
	CDateTimeCtrl m_DateE;
	CString m_strDateS;
	CString m_strDateE;

	//��״ͼ��Ϣ
	CStatic m_Pie;
	CRect m_rctPie;
	CStatic m_Label;
	CRect m_rctLabel;
	CStatic m_S[5];
	int m_nPieTotal;
	std::vector<int> m_nPiePart;
	std::vector<CString> m_strPiePart;

	CStatic m_Info;
	CRect m_rctInfo;

	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid[5];
	std::vector<CString> m_arrColumns1;
	std::vector<CString> m_arrColumns2;
	std::vector<CString> m_arrColumns3;
	std::vector<CString> m_arrColumns4;
	std::vector<CString> m_arrColumns5;
	CDStrs m_datas1;
	CDStrs m_datas2;
	CDStrs m_datas3;
	CDStrs m_datas4;
	CDStrs m_datas5;
	int m_dataSize[5];
	void InitGrid(CVirtualGridCtrl* pGrid, CRect& rect, std::vector<CString>& arrColumns);
	void Refresh(int nID, CString strDateS, CString strDateE);


	//���ݴ������߳�
	CVirtualGridCtrl m_wndGridM;
	CVirtualGridCtrl m_wndGridY;
	CDStrs m_datasM;
	CDStrs m_datasY;
	int m_nGridWidth;
	void InitGridMY(CVirtualGridCtrl* pGrid, std::vector<CString>& arrColumns);
	xPublic::CThreadBase m_threadProcess;
	static void CALLBACK ThreadProcessCallback(LPVOID pParam, HANDLE hCloseEvent);
	BOOL m_canAnal;
	void Analysis(int nID, int YMD, CDStrs& datas, int cFlag); //nID ������ YMD ͳ������, cFlag�����׼���к�

	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedQuit();
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
};


