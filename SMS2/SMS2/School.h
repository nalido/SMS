#pragma once



// CSchool ������ͼ

class CSchool : public CBCGPFormView
{
	DECLARE_DYNCREATE(CSchool)

protected:
	CSchool();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSchool();

public:
	enum { IDD = IDD_SCHOOL };
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
	CString m_strSubForLeave; //����ȱ��һ�μ�ȥ�ļ�Ч
	CString m_strMinWorkTime; //����һ�������ٹ�ʱ
	CString m_strK1Place; //��Ŀһ�Ͽε�ַ
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSave2();
	afx_msg void OnBnClickedSave3();
};


