#pragma once


#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
enum QUERY_TYPE2
{
	QUERY_OIL,  //���Ϲ���
	QUERY_MAINTENANCE, //ά����¼
	QUERY_MOT //����¼
};
// CDlgDevice2 �Ի���

class CDlgDevice2 : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDevice2)

public:
	CDlgDevice2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDevice2();

// �Ի�������
	enum { IDD = IDD_DEVICE_DETAIL2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	int m_nOldRows; //���ݿ��еļ�¼����
	CDStrs m_newDatas; //����ӵļ�¼
	int m_nColumns; //�������
	virtual BOOL OnInitDialog();

	int m_nQueryType;
	void Refresh();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedNewitem();
	afx_msg void OnBnClickedSave();
	void AddNewRowToDB(CStrs strs);
	void DelRowFromDB(CStrs strs);
	void GetNewData(); //��ȡ�û������������
	afx_msg void OnBnClickedDelitem();
};
