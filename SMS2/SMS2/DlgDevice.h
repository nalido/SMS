#pragma once


// CDlgDevice 对话框

class CDlgDevice : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDevice)

public:
	CDlgDevice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDevice();

// 对话框数据
	enum { IDD = IDD_DEVICE_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
