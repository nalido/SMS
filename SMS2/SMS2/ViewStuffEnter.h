#pragma once



// CViewStuffEnter ������ͼ

class CViewStuffEnter : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewStuffEnter)

protected:
	CViewStuffEnter();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewStuffEnter();

public:
	enum { IDD = IDD_STUFF_ENTER };
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
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();

	//������λ
	CStatic m_BKpos1, m_BKpos2;
	CRect m_rctBK1, m_rctBK2;

	CStatic m_Content;
	CRect m_rctContent;

	CStatic m_S[11]; //��̬�ı���
	CString m_strName;
	CString m_strPSWD;
	BOOL m_isLoged; //�Ƿ��Ѿ���¼
	int m_nEraseBkgnd; //�����ػ����(���ݿؼ���������)
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedLogin();
	afx_msg void OnEnChangeE1();
	afx_msg void OnEnChangeE2();
	afx_msg void OnBnClickedNewstuff();
	afx_msg void OnBnClickedForget();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


