#pragma once

#include "ClassItem.h"

// CClassDetail �Ի���

class CClassDetail : public CBCGPDialog
{
	DECLARE_DYNAMIC(CClassDetail)

public:
	CClassDetail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClassDetail();

// �Ի�������
	enum { IDD = IDD_CLASSDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString m_strDay;
	virtual BOOL OnInitDialog();

	CClassItem m_ClassItem[4];
	afx_msg void OnBnClickedConfirm();

	int m_nSelected; //ѡ���� ÿ��4�ڿ� 1111��ʾ�Ľڿ�ȫѡ 1000��ʾѡ�е�һ�ڿ�
};
