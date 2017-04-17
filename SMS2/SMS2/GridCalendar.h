#pragma once
#include "BCGPGridCtrl.h"
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

};

