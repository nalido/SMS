#pragma once


// CDlgDateItem �Ի���

class CDlgDateItem : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDateItem)

public:
	CDlgDateItem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDateItem();

// �Ի�������
	enum { IDD = IDD_DATE_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMcnSelectMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strDate;
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnInitDialog();
};
