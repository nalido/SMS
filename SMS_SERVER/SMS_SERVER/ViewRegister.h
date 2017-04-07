#pragma once

#include "SMS_SERVERDoc.h"
#include "SMS_SERVERView.h"
// CViewRegister ������ͼ


#include "xPublic\CvvImage.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>

class CViewRegister : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewRegister)

protected:
	CViewRegister();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewRegister();

public:
	enum { IDD = IDD_REGISTER };
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
	afx_msg void OnBnClickedCamera();


	CStatic m_SPhoto;
	CButton m_Btn_Cap;

	CStatic m_Sta_Num;
	CString m_strNumber; //������
	CEdit m_Ed_Name; 
	CEdit m_Ed_ID;
	CEdit m_Ed_Tel;
	CEdit m_Ed_Home;
	CEdit m_Ed_Fee;
	CComboBox m_Comb_CarType;
	CDateTimeCtrl m_Date_Sign;
	CDateTimeCtrl m_Date_Birth;

	cv::VideoCapture m_videoCap;//��ͷ��׽
	cv::Mat m_cap;//��׽����֡
	BOOL m_canCap; //�Ƿ��Ѵ�����ͷ
	BOOL m_isCaptured; //������պ���Onpaint()����������ˢ����Ƭ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnSign();
	afx_msg void OnBnClickedNewfile();
};


