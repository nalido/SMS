#pragma once


// CAddCoach �Ի���

class CAddCoach : public CBCGPDialog
{
	DECLARE_DYNAMIC(CAddCoach)

public:
	CAddCoach(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddCoach();

// �Ի�������
	enum { IDD = IDD_ADDCOACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CStatic m_SName;
	CStatic m_SFilenum;
	CStatic m_SParty;
	CStatic m_STel;
	CStatic m_SHome;
	CComboBox m_Comb_Gen;
	CDateTimeCtrl m_Date_Sign;
	CDateTimeCtrl m_Date_Birth;
	
	afx_msg void OnBnClickedAdd();
	virtual BOOL OnInitDialog();
};
