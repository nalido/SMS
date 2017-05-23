#pragma once

#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"


enum DLG_TYPE
{
	DLG_RESP,  //�γ�����
	DLG_MONEY //�ɷ���ϸ
};
// CDlgMyBooks �Ի���

class CDlgMyBooks : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgMyBooks)

public:
	CDlgMyBooks(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMyBooks();

// �Ի�������
	enum { IDD = IDD_MYBOOKS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int m_nDlgType; 
	BOOL m_isPublic; //1:����ʾ�ɷѰ�ť 0:��ʾ�ɷѰ�ť

	CString m_strMsg;
	int m_nSumM; //��δ�ɷѽ��

	//virtualGrid
	CStatic m_wndGridLocation;
	CVirtualGridCtrl m_wndGrid;
	CDStrs m_datas;
	static void CALLBACK OnGridDbClick(LPVOID lParam);
	void Refresh();
	void UpdateCoach(int nRow, int flag = 0);
	void UpdateStudent(int nRow, int flag = 0);

	CString m_strToday;
	CString m_strStuID;

	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);
	afx_msg void OnBnClickedMoney();
};
