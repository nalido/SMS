#pragma once



// CViewK23Exam ������ͼ

class CViewK23Exam : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewK23Exam)

protected:
	CViewK23Exam();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewK23Exam();

public:
	enum { IDD = IDD_K23EXAM };
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


