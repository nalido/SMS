#pragma once



// CViewKPI ������ͼ

class CViewKPI : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewKPI)

protected:
	CViewKPI();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewKPI();

public:
	enum { IDD = IDD_KPI };
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


