#pragma once
#include "BCGPGridCtrl.h"
#include "xPublic\MySQLEx.h"
class CGridCalendar :
	public CBCGPGridCtrl
{
	typedef void (CALLBACK *ONWORK)(LPVOID lParam, BOOL lParam2); //�ص�����
	//typedef void (CALLBACK *ONDRAW)(CDC*pDC, CBCGPGridRow*pItem); //�ص�����
public:
	CGridCalendar();
	~CGridCalendar();


	ONWORK m_OnWork_Click;
	void SetCallBack_Clk(ONWORK onWork);
	virtual void OnSelChanged(const CBCGPGridRange&range, BOOL bSelect);
	DECLARE_MESSAGE_MAP()

	//ONDRAW m_OnDrawItem;
	//void SetCustomDrawItem(ONDRAW onDraw);
	//virtual BOOL OnDrawItem(CDC*pDC, CBCGPGridRow*pItem);
public:
	unsigned int m_nStatus[15][7]; // ��ʱԤԼ״̬
	CTime m_tToday; //��������
	CPoint m_PointToday; //������������λ��

	void InitGrid(int wndWid); //��ʼ�������ʽ
	void DrawItems(); //�Զ����񱳾��� ��ʾ����ԤԼ���
	void UpdateGrid(); //ˢ�±��
	CTime Str2Time(CString str); //�����ݿ�õ��������ַ�����������������
	void GetClassStatus(CDStrs &data); //�������ݿ����ݵõ�ÿ���ʱԤԼ״̬
	CPoint GetDay0Pos(); //��ȡ�ַ�����ʾ�����ڱ��е�λ��
	CPoint GetDayPos(CString strDay); //��ȡ�ַ�����ʾ�����ڱ��е�λ��
	CTime GetSelectedDay(int r, int c); //��ȡѡ��λ�õ�����
	void DrawSelectedItem(CPoint& pos); //���õ�ǰѧԱ����ѡ��ʱ
	void DrawSelectedItem(int r, int c);
};

