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

	CString m_strInfo[11];
	CString m_strStuID;
	int m_nQuitFlag; //��ѧ��־�� 1002Ϊ��;��ѧ�� 1000~1001Ϊ����ͨ����

	CStatic m_S[11];
	CEdit m_E[11];
	afx_msg void OnBnClickedOk();
};
