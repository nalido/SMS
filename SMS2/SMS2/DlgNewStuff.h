#pragma once


// CDlgNewStuff �Ի���

class CDlgNewStuff : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNewStuff)

public:
	CDlgNewStuff(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewStuff();

// �Ի�������
	enum { IDD = IDD_NEWSTUFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRegister();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();


	CStatic m_S[6]; //��̬�ı���
	

	CString m_strName;
	CString m_strID;
	CString m_strTel;
	CString m_strPSW1;
	CString m_strPSW2;
};
