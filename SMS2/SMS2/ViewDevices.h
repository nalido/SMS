#pragma once



// CViewDevices ������ͼ

class CViewDevices : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewDevices)

protected:
	CViewDevices();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewDevices();

public:
	enum { IDD = IDD_DEVICES };
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
	afx_msg void OnPaint();
	void DrawIcon(ATL::CImage& img, int nID);

	CStatic m_SDevice[6];
	CStatic m_SText[6];
	CImage m_imgDevice;
	int m_nSelected; //���ѡ�е�λ��
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
};


