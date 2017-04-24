#pragma once


#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\CvvImage.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include "MyPicCtrl.h"
#include "GridCalendar.h"
#include "xPublic\MySQLEx.h"
// CViewBooking1 ������ͼ


class CViewBooking1 : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewBooking1)

protected:
	CViewBooking1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewBooking1();

public:
	enum { IDD = IDD_BOOKING1 };
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
//	afx_msg void OnBnClickedButton1();
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	static void CALLBACK OnGridClick(LPVOID lParam);
	void AddNewBooking(CString day, int classID);
	void Refresh();

	CMyPicCtrl m_SPhoto;
	cv::Mat m_img;
	afx_msg void OnPaint();
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);

	//������ͼ
	//CView* m_pView[91]; // 7*13
	//CDStrs m_WDStat; //��ʱ��Ϣ work day state/
	CStatic m_SWeek;
	CGridCalendar m_wndCalendar;
	static void CALLBACK OnCalendarClick(LPVOID lParam, BOOL lParam2);

	//��ǰԤԼ��ѧԱ��Ϣ
	CString m_strFileName;
	CString m_strName;
	CString m_strGender;
	CString m_strCarType;
	CString m_strBooked; //��ԤԼ��ʱ��
	afx_msg void OnBnClickedStudentSel();
	afx_msg void OnBnClickedConfirm(); //ȷ��ԤԼ���ϴ����ݿ�
	int exp(int n); //����10��n�η�
	void UpdateBookingList(); //����ԤԼ�б�ʵ������
	void JudgeSelected(int selected, int nclass, CString aday, int r, int c); //�жϵ�nclass��ʱ�Ƿ�ѡ���Լ��Ƿ���Ч
	afx_msg void OnBnClickedRemove();
};


