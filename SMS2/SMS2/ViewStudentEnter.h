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
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
};


