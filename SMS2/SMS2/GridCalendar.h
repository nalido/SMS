#pragma once
#include "BCGPGridCtrl.h"
#include "xPublic\MySQLEx.h"
class CGridCalendar :
	public CBCGPGridCtrl
{
	typedef void (CALLBACK *ONWORK)(LPVOID lParam, BOOL lParam2); //回调函数
	//typedef void (CALLBACK *ONDRAW)(CDC*pDC, CBCGPGridRow*pItem); //回调函数
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
	unsigned int m_nStatus[15][7]; // 课时预约状态
	CTime m_tToday; //当天日期
	CPoint m_PointToday; //当天日期所在位置

	void InitGrid(int wndWid); //初始化表格样式
	void DrawItems(); //自定义表格背景， 显示当天预约情况
	void UpdateGrid(); //刷新表格
	CTime Str2Time(CString str); //将数据库得到的日期字符串解析出具体日期
	void GetClassStatus(CDStrs &data); //解析数据库数据得到每天课时预约状态
	CPoint GetDay0Pos(); //获取字符串表示日期在表中的位置
	CPoint GetDayPos(CString strDay); //获取字符串表示日期在表中的位置
	CTime GetSelectedDay(int r, int c); //获取选择位置的日期
	void DrawSelectedItem(CPoint& pos); //设置当前学员的已选课时
	void DrawSelectedItem(int r, int c);
};

