#pragma once



// CViewHome ������ͼ

class CViewHome : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewHome)

protected:
	CViewHome();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewHome();

public:
	enum { IDD = IDD_HOME };
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	int m_nEraseBkgnd; //�����ػ����(���ݿؼ���������)
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	//������λ
	CStatic m_BKpos1, m_BKpos2;
	CRect m_rctBK1, m_rctBK2;
	virtual void OnInitialUpdate();
};


