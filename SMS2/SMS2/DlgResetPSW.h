#pragma once


// CDlgResetPSW �Ի���

class CDlgResetPSW : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgResetPSW)

public:
	CDlgResetPSW(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgResetPSW();

// �Ի�������
	enum { IDD = IDD_RESETPSW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSms();
	afx_msg void OnBnClickedConfirm();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

	CStatic m_S[6]; //��̬�ı���
	CString m_strTel;
	CString m_strSMS; //��д����֤��
	CString m_strPSW1;
	CString m_strPSW2;

	CString m_strSMS0; //���͵���֤��
	int m_nCount; //��֤�뷢�͵���ʱ
	afx_msg void OnBnClickedReset();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeE3();
	afx_msg void OnEnChangeE4();
};
