#pragma once


// CResponse �Ի���

class CResponse : public CBCGPDialog
{
	DECLARE_DYNAMIC(CResponse)

public:
	CResponse(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResponse();

// �Ի�������
	enum { IDD = IDD_RESPONSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int m_nRspType; //�������� 0��� 1δ���
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();

	CString m_strServiceScore;
	CString m_strSelfScore;
};
