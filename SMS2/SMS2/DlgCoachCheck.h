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
	CVirtualGridCtrl m_wndGridM;
	CDStrs m_datasM;
	CVirtualGridCtrl m_wndGridY;
	CDStrs m_datasY;
	std::vector<CString> m_arrColumns;
	std::vector<CString> m_arrColumnsM;

	CDateTimeCtrl m_DateS;
	CDateTimeCtrl m_DateE;


	int m_nQueryType; //��ѯ��ʽ

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();

	void Refresh();
	void GetJiXiao(); //��ȡ���¼�Ч
	void GetKPI(); //��ȡ����KPI
	void RemadeData(CDStrs& datas); //���ݲ�ͬ���������������ѯ��������
	afx_msg void OnBnClickedQuery();
//	afx_msg void OnBnClickedSettoday();

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
	afx_msg void OnBnClickedExport();
};
