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

	CString m_strStu;
	CString m_strClassIssue;//�ں�
	CString m_strSMSTmp; //ԭʼģ��
	CString m_strSMS;
	CString m_strCoach;
	CString m_strCar;
	CString m_strClassIndex;
	CString m_strReason;

	CStatic m_S[7];
	CStatic m_SMS;
	CEdit m_E[4];
	CString m_strHints[6];
	CString m_strE[4];

	CComboBox m_Comb;
	std::vector<CString> m_arrComb;

	CBCGPDateTimeCtrl	m_wndDatePicker;
	CBCGPDateTimeCtrl	m_wndTimePicker;
	CString m_strDate;
	CString m_strTime;


	virtual BOOL OnInitDialog();
	void Init(int flag);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedDate();
	afx_msg void OnBnClickedTime();
	void UpdateSMS();
	afx_msg void OnEnChangeE1();
	afx_msg void OnEnChangeE2();
	afx_msg void OnEnChangeE3();
	afx_msg void OnEnChangeE4();
	afx_msg void OnCbnSelchangeCombo1();
};
