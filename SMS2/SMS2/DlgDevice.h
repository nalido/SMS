#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
enum QUERY_TYPE
{
	QUERY_DEVICES,  //�豸̨�˱�
	QUERY_INSURANCES, //���ռ�¼��
	QUERY_CLAIMS //�����¼��
};

// CDlgDevice �Ի���

class CDlgDevice : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgDevice)

public:
	CDlgDevice(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDevice();

// �Ի�������
	enum { IDD = IDD_DEVICE_DETAIL };

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
	static void CALLBACK OnGridClick(LPVOID lParam);

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
