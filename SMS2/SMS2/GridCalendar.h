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

	// ��ʱԤԼ״̬, 
	//[r][c][0-3]��ʾÿ���γ̵���ԤԼ����
	//[r][c][4]��ʾ���ݿ����Ƿ��е�ǰ���ڵļ�¼// ʡȥ����ʱ��
	//[r][c][5]��ʾ��ǰѧԱ��ǰ����ԤԼ��ʱʱ��� 1000��ʾѡ��һ�ڿ� 1100Ϊǰ����
	unsigned int m_nStatus[15][7][6]; 
	
	//��������
	CTime m_tToday; 
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
	void DrawSelectedItems();

	/*����posλ�õĵ�Ԫ��״̬ flag 1:ѡ��״̬�� 0��default δѡ��״̬*/
	void SetItemState(CPoint pos, int flag = 0); 
};

