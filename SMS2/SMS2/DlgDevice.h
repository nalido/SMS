#pragma once


// CDlgDevice �Ի���

class CDlgDevice : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDevice)

public:
	CDlgDevice(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDevice();

// �Ի�������
	enum { IDD = IDD_DEVICE_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
