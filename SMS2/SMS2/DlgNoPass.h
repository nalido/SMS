#pragma once


// CDlgNoPass �Ի���

class CDlgNoPass : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNoPass)

public:
	CDlgNoPass(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNoPass();

// �Ի�������
	enum { IDD = IDD_NOPASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString m_strStuName;
	CString m_strReason;

	CStatic m_S1;
	

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
