#pragma once


// CDlgState �Ի���

class CDlgState : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgState)

public:
	CDlgState(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgState();

// �Ի�������
	enum { IDD = IDD_CARSTAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	int m_nStat;
	afx_msg void OnBnClickedOk();
};
