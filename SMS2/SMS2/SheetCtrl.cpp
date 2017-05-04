#include "stdafx.h"
#include "SheetCtrl.h"


CSheetCtrl::CSheetCtrl()
{
}


CSheetCtrl::~CSheetCtrl()
{
}
BEGIN_MESSAGE_MAP(CSheetCtrl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CSheetCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (!m_printData.m_isInit)
	{
		CRect rect;
		GetClientRect(&rect);

		HDC hdc = GetDC()->GetSafeHdc();

		//int xDPI = GetDeviceCaps(hdc, LOGPIXELSX);//返回X方向上每英寸的设备点数，即DPI
		//float mmdpix = xDPI / 25.4;//每毫米所占的设备点数
		//int yDPI = GetDeviceCaps(hdc, LOGPIXELSY);//返回Y方向上每英寸的设备点数，即DPI
		//float mmdpiy = yDPI / 25.4;//每毫米所占的设备点数
		//float nx = mmdpix * 210 / rect.Width(); //将A4纸尺寸映射到控件尺寸的倍数
		//float ny = mmdpiy * 297 / rect.Height();
		//mmdpix = mmdpix / nx;
		//mmdpiy = mmdpiy / ny;
		//float mmdpix = rect.Width() * 1.0 / 210;
		//float mmdpiy = rect.Height() * 1.0 / 297;
		float mmdpix = rect.Width() * 1.0 / 160; //只将实际打印的表格部分映射到控件
		float mmdpiy = rect.Height() * 1.0 / 220;

		m_printData.m_printerInfo.fPrinterScreenx = rect.Width() / mmdpix;
		m_printData.m_printerInfo.fPrinterScreeny = rect.Height() / mmdpiy;
		m_printData.m_printerInfo.fmmDpix = mmdpix;
		m_printData.m_printerInfo.fmmDpiy = mmdpiy;
		m_printData.m_dcPrinter.Attach(hdc);


		m_printData.m_classInfo = &m_classInfo;
		m_printData.m_sheetInfo = &m_sheetInfo;

		m_printData.m_isInit = TRUE;
	}
	m_printData.DrawTrainSheet();
}
