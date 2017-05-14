#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\CvvImage.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include "MyPicCtrl.h"
#include "MainFrm.h"

// CViewScan ������ͼ

class CViewScan : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewScan)

protected:
	CViewScan();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewScan();

public:
	enum { IDD = IDD_SCAN };
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

	BOOL m_canEraseBkgnd;
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	//������λ
	CStatic m_BKpos1, m_BKpos2;
	CRect m_rctBK1, m_rctBK2;

	CStatic m_Content;
	CRect m_rctContent;


	CStatic m_BasicInfo;
	CRect m_rctBasicInfo;
	CStatic m_BasicInfo2;
	CRect m_rctBasicInfo2;
	CStatic m_BasicInfo3;
	CRect m_rctBasicInfo3;
	CStatic m_BasicInfo4;
	CRect m_rctBasicInfo4;
	CStatic m_BasicInfo5;
	CRect m_rctBasicInfo5;

	CStatic m_Pie; //���ȱ�ͼ
	CRect m_rctPie;

	//�������
	CStatic m_S[11]; //��̬�ı���
	CStatic m_T[5]; //��Ŀ����
	CStatic m_D[13]; //��Ϣ�ı���

	//�б�
	CStatic m_wndGridLocation1;
	CStatic m_wndGridLocation2;
	CStatic m_wndGridLocation3;
	CVirtualGridCtrl m_wndGrid1;
	CVirtualGridCtrl m_wndGrid2;
	CVirtualGridCtrl m_wndGrid3;
	CDStrs m_datas1;
	CDStrs m_datas2;
	CDStrs m_datas3;
	void GridInit(std::vector<CString>& arrColumns, CRect& rect, CVirtualGridCtrl* pGrid);


	//ѧ����Ϣ
	BOOL m_isPublic; //1:����ʾ��˽��Ϣ 0:��УԱ���鿴����ʾȫ����Ϣ
	STUDENTINFO m_student;
	CMyPicCtrl m_SPhoto;
	cv::Mat m_img;
	CString m_strMaxClass; //ȫ����ʱ
	CString m_strBookedClass; //��ԤԼ��ʱ��
	CString m_strUsedClass; //��ʹ�ÿ�ʱ��
	CString m_strClassType; //���ڽ��еĿγ����

	//�򿪴���ͼ����һ����ͼ
	UINT m_LAST_VIEW;

	afx_msg void OnBnClickedBook();
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void Refresh();
	afx_msg void OnBnClickedExit();
};


