#pragma once


// CDlgQuitSchool �Ի���

class CDlgQuitSchool : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgQuitSchool)

public:
	CDlgQuitSchool(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgQuitSchool();

// �Ի�������
	enum { IDD = IDD_QUIT_SCHOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	void Refrsh();

	CString m_strInfo[10];

	CStatic m_S[10];
	CEdit m_E[10];
};
