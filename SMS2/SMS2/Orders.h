#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\MyPrint.h"

// COrders �Ի���

class COrders : public CBCGPDialog
{
	DECLARE_DYNAMIC(COrders)

public:
	COrders(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrders();

// �Ի�������
	enum { IDD = IDD_ORDERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	//virtualGrid
	CStatic m_wndGridLocation1;
	CVirtualGridCtrl m_wndGrid1;
	CDStrs m_datas1;
	static void CALLBACK OnGrid1Click(LPVOID lParam);

	BOOL m_enableMultiSel; //�Ƿ������ѡ
	int m_index[3]; //���ͬһ��������ѧԱ
	std::vector<int> m_dataPrint; //��¼�Ƿ��Ѿ���ӡ��

	//
	CDateTimeCtrl m_Date_Start;
	CDateTimeCtrl m_Date_End;
	CString m_strToday;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedReset();
	void ResetStudent(int nRow);
	afx_msg void OnBnClickedPrint();


	xPublic::CMyPrint m_printx;
	xPublic::CLASSINFO m_classInfo;
	xPublic::SHEETINFO m_sheetInfo;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedPreprint();
};
