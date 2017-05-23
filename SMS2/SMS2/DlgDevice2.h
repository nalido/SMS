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

	virtual BOOL OnInitDialog();

	//virtualGrid
	//������ʾ
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	int m_nOldRows; //���ݿ��еļ�¼����
	CDStrs m_newDatas; //����ӵļ�¼
	int m_nColumns; //�������
	static void CALLBACK OnGridClick(LPVOID lParam);

	//������ʾ
	CVirtualGridCtrl m_wndGridM;
	CDStrs m_datasM;

	//������ʾ
	CVirtualGridCtrl m_wndGridY;
	CDStrs m_datasY;

	int m_nShowType; //��ʾ��ʽ 0 ���죬 1 ���£� 2 ����

	//������Ϣ
	CStatic m_wndGridLocationC;
	CVirtualGridCtrl m_wndGridC;
	CDStrs m_datasC;
	CString m_strCarID; //ѡ�е��������
	CString m_strPlateNum; //���ƺ�
	static void CALLBACK OnGridCClick(LPVOID lParam);
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);

	int m_nQueryType;
	void RefreshCar();
	void Refresh();
	void CountData(); //ͳ�����¼�¼
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedNewitem();
	afx_msg void OnBnClickedSave();
	void AddNewRowToDB(CStrs strs);
	void DelRowFromDB(CStrs strs);
	void GetNewData(); //��ȡ�û������������
	afx_msg void OnBnClickedDelitem();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedProxy();
};
