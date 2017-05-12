#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
enum CHECK_TYPE
{
	CHECK_CHUQIN,
	CHECK_QUEQIN,
	CHECK_WORKTIME,
	CHECK_JIXIAO,
	CHECK_KPI
};
// CDlgCoachCheck �Ի���

class CDlgCoachCheck : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgCoachCheck)

public:
	CDlgCoachCheck(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCoachCheck();

// �Ի�������
	enum { IDD = IDD_COACH_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int m_nCheckType; //������Ŀ
	CString m_strCoach;
	CString m_strCoachID; 
	int m_nLeave; //ȱ�ڴ���
	int m_nStudents; //��������ѵѧԱ��


	virtual BOOL OnInitDialog();


	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;

	CComboBox m_Comb_YearS; //��ʼ���
	CComboBox m_Comb_MonthS;
	CComboBox m_Comb_DayS;
	CComboBox m_Comb_YearE; //��ֹ���
	CComboBox m_Comb_MonthE;
	CComboBox m_Comb_DayE;
	CString m_strYearS; //��ʼ���
	CString m_strMonthS;
	CString m_strDayS;
	CString m_strYearE; //��ֹ���
	CString m_strMonthE;
	CString m_strDayE;
	int m_nQueryType; //��ѯ��ʽ
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo5();

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();

	void Refresh();
	void GetJiXiao(); //��ȡ���¼�Ч
	void GetKPI(); //��ȡ����KPI
	CString GetSelDate(int nID); //��ȡѡ���ʱ��� nID=0 ��ʼʱ��
	void RemadeData(CDStrs& datas); //���ݲ�ͬ���������������ѯ��������
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedSettoday();

	//ͳ����Ϣ
	CStatic m_Static_Msg1;
	CStatic m_Static_Msg2;
	CStatic m_Static_Msg3;
	CRect m_rectMsg1;
	CRect m_rectMsg2;
	CRect m_rectMsg3;
	CString m_strMsg1;
	CString m_strMsg2;
	CString m_strMsg20;
	CString m_strMsg3;
	void UpdateCStatic();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
