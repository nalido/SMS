#pragma once
#include "SheetCtrl.h"

// COrderDetail �Ի���

class COrderDetail : public CBCGPDialog
{
	DECLARE_DYNAMIC(COrderDetail)

public:
	COrderDetail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrderDetail();

// �Ի�������
	enum { IDD = IDD_ORDERDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString m_strStudent;		//ѧԱ������
	CString m_strDate;			//����
	CString m_strClassID;		//�γ�ʱ��

	CSheetCtrl m_wndPrint;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnPaint();
};
