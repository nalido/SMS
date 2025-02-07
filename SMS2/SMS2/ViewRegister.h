#pragma once

#include "SMS2Doc.h"
#include "SMS2View.h"
// CViewRegister 窗体视图


#include "xPublic\CvvImage.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include "MainFrm.h"
#include "MyPicCtrl.h"

enum DATA_TYPE
{
	DATA_NEW, //新添信息
	DATA_EDIT //修改信息
};

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
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	int m_nDataType; //当前信息类型
	int m_nLastView; //上一个视图,只在修改信息时有效

	//背景定位
	CStatic m_BKpos1, m_BKpos2;
	CRect m_rctBK1, m_rctBK2;
	CStatic m_Content;
	CRect m_rctContent;

	CMyPicCtrl m_SPhoto;

	CStatic m_Sta_Num;
	CString m_strNumber; //档案号
	CEdit m_Ed_Name; 
	CEdit m_Ed_ID;
	CEdit m_Ed_Tel;
	CEdit m_Ed_Home;
	CEdit m_Ed_Fee;
	CComboBox m_Comb_CarType;
	CComboBox m_Comb_Gender;
	CDateTimeCtrl m_Date_Sign;
	CDateTimeCtrl m_Date_Birth;

	cv::VideoCapture m_videoCap;//镜头捕捉
	cv::Mat m_cap;//捕捉到的帧
	BOOL m_canCap; //是否已打开摄像头
	BOOL m_isCaptured; //完成拍照后由Onpaint()函数来负责刷新照片
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnSign();
	afx_msg void OnBnClickedNewfile();
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//背景设置
	CStatic m_staticText[10]; //10个静态文本框
	afx_msg void OnBnClickedClear();
};


