#pragma once



#include "BCGClass\VirtualGridCtrl.h"
#include "xPublic\MySQLEx.h"
#include "SheetCtrl.h"

// CViewBooking2 ������ͼ
typedef std::vector<int> Indexes;
typedef std::vector<Indexes> Indextable;

class CViewBooking2 : public CBCGPFormView
{
	DECLARE_DYNCREATE(CViewBooking2)

protected:
	CViewBooking2();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewBooking2();

public:
	enum { IDD = IDD_BOOKING2 };
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
	virtual void OnInitialUpdate();
	afx_msg LRESULT OnUserUpdate(WPARAM wp, LPARAM lp);


	//virtualGrid
	CStatic m_wndGridLocation1;
	CVirtualGridCtrl m_wndGrid1;
	CDStrs m_datas1;
	static void CALLBACK OnGrid1Click(LPVOID lParam);
	static void CALLBACK OnGrid1DbClick(LPVOID lParam);
	CStatic m_wndGridLocation2;
	CVirtualGridCtrl m_wndGrid2;
	CDStrs m_datas2;
	static void CALLBACK OnGrid2Click(LPVOID lParam);

	CStatic m_wndGridLocation3;
	CVirtualGridCtrl m_wndGrid3;
	CDStrs m_datas3; //���������Ϣ
	Indexes m_order; //��¼��ǰ�ɹ�Ԥ������������
	Indextable m_orderIndexes; //�ɹ�����������Ϣ����
	static void CALLBACK OnOrdersClick(LPVOID lParam);
	BOOL m_canChangeOrder; //�Ƿ�����޸��ɹ�
	int m_nChangingOrderIndex; //�����޸ĵ��ɹ����
	afx_msg void OnBnClickedOrderChange();

	//type 0 ��ͨģʽ�µ����ã��������״̬ 
	//type 1 ���ȷ���ɹ�������ã�����ѧԱѡ��״̬
	void RestOrder(int type = 0);

	//ѡ�е�һ��ѧԱ���ʣ��ѧԱ����ˢѡ������ɫ����, ���ڽ����ݷ���
	std::vector<BYTE> m_arrClassIndex;

	//�ж�ѡ���ѧ���Ƿ���Լ����ɹ���
	BOOL CanBeSelected(int nRow); 

	//��ȡ�б�������ѧԱ���ڽ��������
	void GetClassIndex();

	void Refresh(int nID = 0);

	//�ɹ�����ѡ�� ����Ϊֻ��ѡ������������
	CTime m_tToday, m_tTomorrow;
	BOOL m_isToday;
	CStatic m_wndDate;
	CRect m_rectDate;
	afx_msg void OnBnClickedSelDay();

	//�ɹ���
	CSheetCtrl m_wndPrint;
	afx_msg void OnBnClickedDoPrint();
//	afx_msg void OnStnClickedGrid1();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOrder();

	afx_msg void OnBnClickedResetPrint(); 

	CComboBox m_Combo_Cars;
	afx_msg void OnCbnSelchangeCars();
	afx_msg void OnBnClickedOrderQuery();
	afx_msg void OnBnClickedAutoOrder();
	afx_msg void OnBnClickedTmpOrder();
};


