#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"

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

	//virtualGrid
	CStatic m_wndGridLocation1;
	CVirtualGridCtrl m_wndGrid1;
	CDStrs m_datas1;
	std::vector<int> m_dataPrint; //��¼�Ƿ��Ѿ���ӡ��

	//
	CDateTimeCtrl m_Date_Start;
	CDateTimeCtrl m_Date_End;
	CString m_strToday;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedPrint();
};
