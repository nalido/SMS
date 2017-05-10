// ViewDevices.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewDevices.h"
#include "MainFrm.h"
#include "DlgDevice.h"
#include "DlgDevice2.h"


// CViewDevices

IMPLEMENT_DYNCREATE(CViewDevices, CBCGPFormView)

CViewDevices::CViewDevices()
	: CBCGPFormView(CViewDevices::IDD)
{
	EnableVisualManagerStyle();

	m_nSelected = -1;
	m_canEraseBkgnd = FALSE;
}

CViewDevices::~CViewDevices()
{
}

void CViewDevices::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICE, m_SDevice[0]);
	DDX_Control(pDX, IDC_DEVICE2, m_SDevice[1]);
	DDX_Control(pDX, IDC_DEVICE3, m_SDevice[2]);
	DDX_Control(pDX, IDC_DEVICE4, m_SDevice[3]);
	DDX_Control(pDX, IDC_DEVICE5, m_SDevice[4]);
	DDX_Control(pDX, IDC_DEVICE6, m_SDevice[5]);

	DDX_Control(pDX, IDC_TEXT1, m_SText[0]);
	DDX_Control(pDX, IDC_TEXT2, m_SText[1]);
	DDX_Control(pDX, IDC_TEXT3, m_SText[2]);
	DDX_Control(pDX, IDC_TEXT4, m_SText[3]);
	DDX_Control(pDX, IDC_TEXT5, m_SText[4]);
	DDX_Control(pDX, IDC_TEXT6, m_SText[5]);
}

BEGIN_MESSAGE_MAP(CViewDevices, CBCGPFormView)
	ON_WM_PAINT()
//	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
//	ON_STN_CLICKED(IDC_DEVICE, &CViewDevices::OnStnClickedDevice)
ON_WM_LBUTTONDOWN()
//ON_WM_MOUSEMOVE()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CViewDevices 诊断

#ifdef _DEBUG
void CViewDevices::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewDevices::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewDevices 消息处理程序


CString pth[] = { "res\\设备1.png", "res\\保险2.png", "res\\汽油1.png",
"res\\保养1.png", "res\\年检1.png", "res\\理赔1.png" };
CString name[] = { "设备台账表", "保险明细", "油料管理",
"保养记录", "年检记录", "理赔记录" };

void CViewDevices::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	CRect rect;
	GetClientRect(&rect);
	MapWindowPoints(this, &rect);

	CDC     MemDC;
	//HBITMAP hbitmap;
	CBitmap bitmp;
	MemDC.CreateCompatibleDC(&dc);

	bitmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	MemDC.SelectObject(&bitmp);

	//if (m_nSelected != -1)
	//{
	//	CRect r;
	//	m_SDevice[m_nSelected].GetClientRect(&r);
	//	m_SDevice[m_nSelected].MapWindowPoints(this, &r);

	//	CBrush brush;
	//	brush.CreateSolidBrush(RGB(120, 240, 100));
	//	//dc.FillRect(r, &brush);
	//}

	//if (m_imgDevice.IsNull())
	//{
	//	m_imgDevice.Create(rect.Width(), rect.Height(), 32, m_imgDevice.createAlphaChannel);
	//	for (int i = 0; i < 6; i++)
	//		DrawIcon(m_imgDevice, i);
	//}

	//m_imgDevice.Draw(dc.m_hDC, rect);

	///GDI+
	Graphics graph(MemDC.m_hDC); 

	//填充背景色
	if (m_canEraseBkgnd == TRUE)
	{
		m_ThemeColor = dc.GetPixel(5, 5);
		m_canEraseBkgnd = FALSE;
	}
	CBrush brush;
	brush.CreateSolidBrush(m_ThemeColor);
	MemDC.FillRect(rect, &brush);


	for (int i = 0; i < 6; i++)
	{
		CRect recti;
		m_SDevice[i].GetClientRect(&recti);
		m_SDevice[i].MapWindowPoints(this, &recti);

		if (m_nSelected == i)
		{
			Point p1(recti.left, recti.top);
			Point p2(recti.left, recti.bottom);
			LinearGradientBrush lgb(p1, p2, Color(249, 238, 224), Color::BurlyWood);
			graph.FillRectangle(&lgb, recti.left, recti.top, recti.Width(), recti.Height());
			Pen pen(Color(188, 157, 115));
			graph.DrawRectangle(&pen, recti.left, recti.top, recti.Width(), recti.Height());
		}
		CStringW strW = CT2CW(pth[i]);
		Image img(strW.GetBuffer(0));
		graph.DrawImage(&img, recti.left, recti.top, recti.Width(), recti.Height());

		strW.ReleaseBuffer();
		//delete pDC;
	}


	CFont font1;
	font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
		0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SelectObject(&font1);
	MemDC.SetTextAlign(TA_CENTER);
	for (int i = 0; i < 6; i++)
	{
		CRect rectT;
		m_SText[i].GetClientRect(&rectT);
		m_SText[i].MapWindowPoints(this, &rectT);

		MemDC.TextOutA(rectT.CenterPoint().x, rectT.CenterPoint().y, name[i]);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	//、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、

	//dc.TransparentBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, rect.Width(), rect.Height(), RGB(0,0,0));
	//
	bitmp.DeleteObject();
	MemDC.DeleteDC();
}

void CViewDevices::DrawIcon(CImage& img, int nID)
{
	CRect rect;
	m_SDevice[nID].GetClientRect(&rect);
	m_SDevice[nID].MapWindowPoints(this, &rect);
	CImage img_Device;
	img_Device.Load(pth[nID]);
	if (img_Device.IsNull())
	{
		TRACE("加载图片资源失败");
		return;
	}

	if (img_Device.GetBPP() == 32) //确认图片包含alpha通道
	{
		for (int i = 0; i < img_Device.GetWidth(); i++)
		for (int j = 0; j < img_Device.GetHeight(); j++)
		{
			BYTE* pByte = (BYTE*)img_Device.GetPixelAddress(i, j);
			pByte[0] = pByte[0] * pByte[3] / 255;
			pByte[1] = pByte[1] * pByte[3] / 255;
			pByte[2] = pByte[2] * pByte[3] / 255;
		}
	}

	img_Device.Draw(img.GetDC(), rect.left, rect.top, rect.Width(), rect.Height());
	img.ReleaseDC();
	img_Device.Destroy();
}

//HBRUSH CViewDevices::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN)
//	{
//		CFont font1;
//		font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
//			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
//		pDC->SelectObject(&font1);
//		font1.DeleteObject();
//		pDC->SetBkMode(TRANSPARENT);
//	}
//
//	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	return hbr;
//}



void CViewDevices::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL isSelected = FALSE;
	int lastSel = m_nSelected;
	for (int nID = 0; nID < 6; nID++)
	{
		CRect rect;
		m_SDevice[nID].GetClientRect(&rect);
		m_SDevice[nID].MapWindowPoints(this, &rect);
		if (rect.PtInRect(point))
		{
			//CString str;
			//str.Format("%d is clicked\r\n", nID);
			//TRACE(str);

			isSelected = TRUE;
			m_nSelected = nID;
			break;
		}
	}

	if (!isSelected)
	{
		m_nSelected = -1;
	}

	Invalidate();

	if (isSelected)
	{
		switch (m_nSelected)
		{
		case 0:
		{
				  CDlgDevice dlg;
				  dlg.m_nQueryType = QUERY_DEVICES;
				  dlg.DoModal();
				  break;
		}
		case 1:
		{
				  CDlgDevice dlg;
				  dlg.m_nQueryType = QUERY_INSURANCES;
				  dlg.DoModal();
				  break;
		}
		case 5:
		{
				  CDlgDevice dlg;
				  dlg.m_nQueryType = QUERY_CLAIMS;
				  dlg.DoModal();
				  break;
		}

		case 2:
		{
				  CDlgDevice2 dlg;
				  dlg.m_nQueryType = QUERY_OIL;
				  dlg.DoModal();
				  break;
		}
		case 3:
		{
				  CDlgDevice2 dlg;
				  dlg.m_nQueryType = QUERY_MAINTENANCE;
				  dlg.DoModal();
				  break;
		}
		case 4:
		{
				  CDlgDevice2 dlg;
				  dlg.m_nQueryType = QUERY_MOT;
				  dlg.DoModal();
				  break;
		}
		}
	}

	CBCGPFormView::OnLButtonDown(nFlags, point);
}


void CViewDevices::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();



	//CString pth[] = { "res\\设备1.png", "res\\保险2.png", "res\\汽油.png",
	//	"res\\保养1.png", "res\\年检1.png", "res\\理赔1.png" };
	//for (int i = 0; i < 6; i++)
	//{
	//	m_SDevice[i].ShowWindow(1);
	//	m_SDevice[i].SetPNGSource(pth[i]);
	//}
}


void CViewDevices::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_nSelected == -1)
	for (int nID = 0; nID < 6; nID++)
	{
		m_SDevice[nID].ShowWindow(0);
		//m_SDevice[nID].SetState(FALSE);
		CRect rect;
		m_SDevice[nID].GetClientRect(&rect);
		m_SDevice[nID].MapWindowPoints(this, &rect);
		if (rect.PtInRect(point))
		{
			m_SDevice[nID].ShowWindow(1);
			//m_SDevice[nID].SetState(TRUE);
		}
	}



	CBCGPFormView::OnMouseMove(nFlags, point);
}


BOOL CViewDevices::OnEraseBkgnd(CDC* pDC)
{
	if (!m_canEraseBkgnd)
		return TRUE;
	else
	{
		return CBCGPFormView::OnEraseBkgnd(pDC);
	}
}


LRESULT CViewDevices::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;
	if(flag==1)
		m_canEraseBkgnd = TRUE;

	return 0;
}