#pragma once


// CDlgFee �Ի���

class CDlgFee : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgFee)

public:
	CDlgFee(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFee();

// �Ի�������
	enum { IDD = IDD_FEE_UP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
