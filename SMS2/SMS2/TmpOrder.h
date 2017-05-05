#pragma once


// CTmpOrder 对话框

class CTmpOrder : public CBCGPDialog
{
	DECLARE_DYNAMIC(CTmpOrder)

public:
	CTmpOrder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTmpOrder();

// 对话框数据
	enum { IDD = IDD_TMPORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
