#pragma once

// CDlgNextBookDate �Ի���

class CDlgNextBookDate : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgNextBookDate)

public:
	CDlgNextBookDate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNextBookDate();

// �Ի�������
	enum { IDD = IDD_NEXT_BOOKDAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	CString m_strStuID;
	CEdit m_E;
	CString m_strOldDate;
	CDateTimeCtrl m_Date;

	afx_msg void OnBnClickedOk();
	void Refresh();
};
