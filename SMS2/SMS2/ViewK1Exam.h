#pragma once



// CViewK1Exam ������ͼ

class CViewK1Exam : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewK1Exam)

protected:
	CViewK1Exam();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewK1Exam();

public:
	enum { IDD = IDD_K1EXAM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


