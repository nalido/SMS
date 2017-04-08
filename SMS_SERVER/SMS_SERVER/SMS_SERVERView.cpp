// SMS_SERVERView.cpp : implementation of the CSMS_SERVERView class
//

#include "stdafx.h"
#include "SMS_SERVER.h"

#include "SMS_SERVERDoc.h"
#include "SMS_SERVERView.h"

#include "MainFrm.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include "xPublic\CvvImage.h"

using namespace xPublic;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMS_SERVERView

IMPLEMENT_DYNCREATE(CSMS_SERVERView, CBCGPFormView)

BEGIN_MESSAGE_MAP(CSMS_SERVERView, CBCGPFormView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CSMS_SERVERView construction/destruction

static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CSMS_SERVERView* pThis = (CSMS_SERVERView*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = g_strMsgLog.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		pdi->item.varValue = g_strMsgLog[nRow][nCol];
		//if (pThis->m_datas[nRow][nCol] == "nalido")
		//{
		//	pdi->item.clrText = COLORREF(RGB(255, 0, 0));
		//}
		//if (pThis->m_datas[nRow][nCol] == "snow")
		//{
		//	pdi->item.clrBackground = COLORREF(RGB(0, 110, 0));
		//}
	}

	return TRUE;
}

CSMS_SERVERView::CSMS_SERVERView()
: CBCGPFormView(CSMS_SERVERView::IDD)
, m_tcpServer(this, OnTCPAccept, OnTCPRecive, OnTCPClosed)
{

	EnableVisualManagerStyle();

	// TODO: add construction code here

}

CSMS_SERVERView::~CSMS_SERVERView()
{
}

void CSMS_SERVERView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_cTreeCtrl);
	DDX_Control(pDX, IDC_STATIC_VGRID, m_wndGridLocation);
}

BOOL CSMS_SERVERView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CBCGPFormView::PreCreateWindow(cs);
}

void CSMS_SERVERView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();


	m_cTreeCtrl.SetImageList(&m_TreeImages, TVSIL_NORMAL);
	//初始化表格
	CRect rectGrid;
	m_wndGridLocation.GetClientRect(&rectGrid);
	m_wndGridLocation.MapWindowPoints(this, &rectGrid); //转为桌面坐标

	m_wndGrid.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	//m_wndGrid.SetCustomColors(-1, -1, -1, -1, -1, RGB(0, 0, 0)); //黑色边框
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.InsertColumn(0, _T("信息"), rectGrid.Width());
	//注册虚拟列表回调函数
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);
}


// CSMS_SERVERView diagnostics

#ifdef _DEBUG
void CSMS_SERVERView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CSMS_SERVERView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CSMS_SERVERDoc* CSMS_SERVERView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMS_SERVERDoc)));
	return (CSMS_SERVERDoc*)m_pDocument;
}
#endif //_DEBUG


// CSMS_SERVERView message handlers


void CSMS_SERVERView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
}


void CSMS_SERVERView::OnDestroy()
{
	CBCGPFormView::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	m_tcpServer.ShutDown();
}


int CSMS_SERVERView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_nHostPort = 39200;
	m_TreeImages.Create(IDB_TREE_IMG, 16, 4, RGB(255, 0, 255));
	m_tcpServer.Create(m_nHostPort);

	return 0;
}

void CSMS_SERVERView::OnTCPAccept(LPVOID lParam, xPublic::CTCPClient *pClient, BOOL bNew)
{
	CSMS_SERVERView*  pThis = (CSMS_SERVERView *)lParam;

	if (NULL != pClient)
	{
		if (bNew)
		{
			//创建树结点结构
			TV_INSERTSTRUCT tvis;

			//设定树结构参数
			tvis.hParent = TVI_ROOT;				// Handle to parent item
			tvis.hInsertAfter = TVI_LAST;				// TVI_SORT = Sort items in tree
			tvis.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_TEXT;// item props we will fill	
			tvis.item.lParam = (LPARAM)pClient;		// item data: pointer to object
			tvis.item.pszText = LPSTR_TEXTCALLBACK;	// item text: defer to OnGetDispInfo ()
			tvis.item.iImage = I_IMAGECALLBACK;		// item image: defer to OnGetDispInfo ()
			tvis.item.iSelectedImage = I_IMAGECALLBACK;	// item selimage: defer to OnGetDispInfo ()

			//插入树结点
			pThis->m_cTreeCtrl.InsertItem(&tvis);
		}

		CString strMsg;
		strMsg.Format("监听到客户端【%s】的连接", pClient->m_sHostIPPort);
		CStrs strs;
		strs.push_back(strMsg);
		g_strMsgLog.push_back(strs);
		pThis->ListFresh();
		pThis->m_cTreeCtrl.Invalidate();
	}

}

void CSMS_SERVERView::OnTCPRecive(LPVOID lParam, xPublic::CTCPClient *pClient)
{
	CSMS_SERVERView*  pThis = (CSMS_SERVERView *)lParam;
	if (NULL != pClient)
	{
		BYTE MsgType = 0;
		pClient->Receive(&MsgType, 1);

		CString strMsg;
		if (MsgType == 1)
		{
			char FileNum[11] = { 0 };
			pClient->Receive(&FileNum, 10);
			int wid, hei, imgSize;
			pClient->Receive(&wid, 4);
			pClient->Receive(&hei, 4);
			pClient->Receive(&imgSize, 4);
			BYTE* picBuf = new BYTE[imgSize + 1];
			if (pClient->Receive(picBuf, imgSize))
			{
				strMsg.Format("收到客户端图像信息：%s %dX%d 接收成功", FileNum, wid, hei);
				pThis->SaveBmp(FileNum, picBuf, wid, hei, imgSize);
				CStrs strs;
				strs.push_back(strMsg);
				g_strMsgLog.push_back(strs);
				pThis->ListFresh();
			}
			else
			{
				strMsg.Format("收到客户端图像信息：%s %dX%d 接收图像数据失败", FileNum, wid, hei);
				pThis->SaveBmp(FileNum, picBuf, wid, hei, imgSize);
				CStrs strs;
				strs.push_back(strMsg);
				g_strMsgLog.push_back(strs);
				pThis->ListFresh();
			}

			delete[] picBuf; picBuf = NULL;
		}
	}
}


void CSMS_SERVERView::SaveBmp(char* FileNum, BYTE* picBuf, int wid, int hei, int imgSize)
{
	IplImage* pImg = cvCreateImageHeader(cvSize(wid, hei), 8, 3);
	int lineByte = 1920;// (wid + 3) / 4 * 4;
	cvSetData(pImg, picBuf, lineByte);

	cv::Mat img = cv::cvarrToMatND(pImg);
	CString sFileName("");
	sFileName.Format("%s%s.bmp", g_FilePath, FileNum);
	::SHCreateDirectory(NULL, CA2W(g_FilePath));
	cv::String s = sFileName.GetBuffer();
	imwrite(s, img);
	sFileName.ReleaseBuffer();
	pImg = NULL;
}

void CSMS_SERVERView::OnTCPClosed(LPVOID lParam, xPublic::CTCPClient *pClient)
{
	CSMS_SERVERView*  pThis = (CSMS_SERVERView *)lParam;
	if (NULL != pClient)
	{
		CString strMsg;
		strMsg.Format("客户端【%s】断开连接", pClient->m_sHostIPPort);
		CStrs strs;
		strs.push_back(strMsg);
		g_strMsgLog.push_back(strs);
		pThis->ListFresh();
		pThis->m_cTreeCtrl.Invalidate();
	}
}



void CSMS_SERVERView::ListFresh()
{
	m_wndGrid.RemoveAll();
	m_wndGrid.SetVirtualRows(1000000);// m_datas.size());
	m_wndGrid.AdjustLayout();

	if (g_strMsgLog.size() > 5000)
	{
		std::vector<CStrs>::iterator it = g_strMsgLog.begin();
		g_strMsgLog.erase(it);
	}
}