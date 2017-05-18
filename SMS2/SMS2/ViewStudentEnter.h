#pragma once



// CViewStudentEnter ������ͼ

class CViewStudentEnter : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewStudentEnter)

protected:
	CViewStudentEnter();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewStudentEnter();

public:
	enum { IDD = IDD_STUDENT_ENTER };
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

	//������λ
	CStatic m_BKpos1, m_BKpos2;
	CRect m_rctBK1, m_rctBK2;

	int m_nEraseBkgnd; //�����ػ����(���ݿؼ���������)
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CStatic m_Content;
	CRect m_rctContent;

	CStatic m_S[4]; //��̬�ı���
	CString m_strName;
	CString m_strIDCard;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedConfirm();
	afx_msg void OnEnChangeE1();
	afx_msg void OnEnChangeE2();
};


