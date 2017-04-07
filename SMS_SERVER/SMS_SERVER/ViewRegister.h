#pragma once

#include "SMS_SERVERDoc.h"
#include "SMS_SERVERView.h"
// CViewRegister 窗体视图


#include "xPublic\CvvImage.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>

class CViewRegister : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewRegister)

protected:
	CViewRegister();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCamera();


	CStatic m_SPhoto;
	CButton m_Btn_Cap;

	CStatic m_Sta_Num;
	CString m_strNumber; //档案号
	CEdit m_Ed_Name; 
	CEdit m_Ed_ID;
	CEdit m_Ed_Tel;
	CEdit m_Ed_Home;
	CEdit m_Ed_Fee;
	CComboBox m_Comb_CarType;
	CDateTimeCtrl m_Date_Sign;
	CDateTimeCtrl m_Date_Birth;

	cv::VideoCapture m_videoCap;//镜头捕捉
	cv::Mat m_cap;//捕捉到的帧
	BOOL m_canCap; //是否已打开摄像头
	BOOL m_isCaptured; //完成拍照后由Onpaint()函数来负责刷新照片
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnSign();
	afx_msg void OnBnClickedNewfile();
};


