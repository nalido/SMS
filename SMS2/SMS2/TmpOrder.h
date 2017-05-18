#pragma once

#include "xPublic\MyPrint.h"
#include "xPublic\MySQLEx.h"

// CTmpOrder �Ի���

class CTmpOrder : public CBCGPDialog
{
	DECLARE_DYNAMIC(CTmpOrder)

public:
	CTmpOrder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTmpOrder();

// �Ի�������
	enum { IDD = IDD_TMPORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_Comb_Type;
	CComboBox m_Comb_Car;
	CComboBox m_Comb_Coach;
	CComboBox m_Comb_Stu;
	CComboBox m_Comb_Time;
	CComboBox m_Comb_Class;
	CStatic m_SCoach;
	CStatic m_SStu;
	CStatic m_SNClass;
	CDateTimeCtrl m_Date;

	CString m_strType;
	CString m_strCar;
	CString m_strCoach;
	CString m_strStu;
	CString m_strTime;
	CString m_strCoachID;
	CString m_strStuID;
	CString m_strTEL;
	CString m_strNClass;
	CString m_strNextClass;
	CString m_strBookDate;
	CString m_strClassID;
	CString m_strOrderDate;
	BOOL m_isDataReady; //�Ƿ��Ѿ���ȡ������

	CDStrs m_datasCar;
	CDStrs m_datasCoach;
	CDStrs m_datasStu;

	virtual BOOL OnInitDialog();

	void Refresh();
	afx_msg void OnCbnSelchangeComboCoach();
	afx_msg void OnCbnEditchangeComboCoach();
	afx_msg void OnCbnEditchangeComboStu();
	afx_msg void OnCbnSelchangeComboStu();
	afx_msg void OnBnClickedPrint();

	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboType();
};

