#pragma once


// CDlgNextClass �Ի���

class CDlgNextClass : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNextClass)

public:
	CDlgNextClass(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNextClass();

// �Ի�������
	enum { IDD = IDD_NEXTCLASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	int m_nFlag; //flag=1 ��ʾ�޸�����

	CStatic m_S[12]; //��̬�ı���
	CEdit m_E[8];
	CFont m_fontE;
	CComboBox m_Comb;
	CString m_strSelectedClass; //ѡ�еĿ�ʱ
	std::vector<CString> m_arrClassText;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
};
