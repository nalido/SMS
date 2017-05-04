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

		//int xDPI = GetDeviceCaps(hdc, LOGPIXELSX);//����X������ÿӢ����豸��������DPI
		//float mmdpix = xDPI / 25.4;//ÿ������ռ���豸����
		//int yDPI = GetDeviceCaps(hdc, LOGPIXELSY);//����Y������ÿӢ����豸��������DPI
		//float mmdpiy = yDPI / 25.4;//ÿ������ռ���豸����
		//float nx = mmdpix * 210 / rect.Width(); //��A4ֽ�ߴ�ӳ�䵽�ؼ��ߴ�ı���
		//float ny = mmdpiy * 297 / rect.Height();
		//mmdpix = mmdpix / nx;
		//mmdpiy = mmdpiy / ny;
		//float mmdpix = rect.Width() * 1.0 / 210;
		//float mmdpiy = rect.Height() * 1.0 / 297;
		float mmdpix = rect.Width() * 1.0 / 160; //ֻ��ʵ�ʴ�ӡ�ı�񲿷�ӳ�䵽�ؼ�
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
