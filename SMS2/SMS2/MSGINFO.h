#pragma once


// CMSGINFO �Ի���

class CMSGINFO : public CBCGPDialog
{
	DECLARE_DYNAMIC(CMSGINFO)

public:
	CMSGINFO(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMSGINFO();

// �Ի�������
	enum { IDD = IDD_SMSINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int m_nFlag; //�������ͱ�־
	CString m_strSMS;
	virtual BOOL OnInitDialog();
	void Init(int flag);
};
