#pragma once


// CDlgProxy �Ի���

class CDlgProxy : public CDialog
{
	DECLARE_DYNAMIC(CDlgProxy)

public:
	CDlgProxy(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProxy();

// �Ի�������
	enum { IDD = IDD_PROXY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
