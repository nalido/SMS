#include "stdafx.h"
#include "GridCalendar.h"


CGridCalendar::CGridCalendar()
{
	m_OnWork_Click = NULL;
	//m_OnDrawItem = NULL;
}


CGridCalendar::~CGridCalendar()
{
}


void CGridCalendar::SetCallBack_Clk(ONWORK onWork)
{
	m_OnWork_Click = onWork;
}

void CGridCalendar::OnSelChanged(const CBCGPGridRange&range, BOOL bSelect)
{
	if (m_OnWork_Click != NULL)
	{
		m_OnWork_Click(this->GetParent(), bSelect);
	}
}
BEGIN_MESSAGE_MAP(CGridCalendar, CBCGPGridCtrl)
END_MESSAGE_MAP()


//void CGridCalendar::SetCustomDrawItem(ONDRAW onDraw)
//{
//	m_OnDrawItem = onDraw;
//}
//
//BOOL CGridCalendar::OnDrawItem(CDC*pDC, CBCGPGridRow*pItem)
//{
//	if (m_OnDrawItem != NULL)
//	{
//		m_OnDrawItem(pDC, pItem);
//	}
//
//	return 0;
//}

