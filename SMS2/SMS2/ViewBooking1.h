#pragma once



// CViewBooking1 ������ͼ

class CViewBooking1 : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewBooking1)

protected:
	CViewBooking1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewBooking1();

public:
	enum { IDD = IDD_BOOKING1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


