#pragma once


// CTmpOrder �Ի���

class CTmpOrder : public CBCGPDialog
{
	DECLARE_DYNAMIC(CTmpOrder)

public:
	CTmpOrder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTmpOrder();

// �Ի�������
	enum { IDD = IDD_TMPORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
