#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"



// CViewOrderRsp ������ͼ

class CViewOrderRsp : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewOrderRsp)

protected:
	CViewOrderRsp();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewOrderRsp();

public:
	enum { IDD = IDD_ORDERS_RSP };
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

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	virtual void OnInitialUpdate(); 
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	static void CALLBACK OnGridDbClick(LPVOID lParam);

	CString m_strToday;

	void Refresh();

	//���½���Ա�������, 
	//flag=0 �������ڣ����ڱ����һ����¼�� ��Ч��һ, ��ʱ��2Сʱ
	//flag=1 ȱ�ڣ�ȱ�ڱ����һ����¼��ȱ�ڼ�����һ�� ��Ч��8
	void UpdateCoach(int nRow, int flag = 0); 

	//����ѧԱ����
	//flag=0 �������ѵ���� bookings��FLAG�ֶθ���Ϊ��2���� ���Ͽ�ʱ����һ
	//flag=1 ȱ�ڣ�FLAG�ֶθ���Ϊ��-1���� ���Ͽ�ʱ������
	void UpdateStudent(int nRow, int flag = 0);
	afx_msg void OnBnClickedRespon();
};


