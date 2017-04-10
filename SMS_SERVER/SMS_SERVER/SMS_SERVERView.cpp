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

#include "xPublic\HTTPClient.h"

using namespace xPublic;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* EncodeToUTF8(const char* mbcsStr)
{
	wchar_t*  wideStr;
	char*   utf8Str;
	int   charLen;

	charLen = MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, NULL, 0);
	wideStr = (wchar_t*)malloc(sizeof(wchar_t)*charLen);
	MultiByteToWideChar(CP_ACP, 0, mbcsStr, -1, wideStr, charLen);

	charLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);

	utf8Str = (char*)malloc(charLen);

	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, charLen, NULL, NULL);

	free(wideStr);
	return utf8Str;

}
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
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		vector<CStrs>::iterator it = pThis->m_datas.begin() + nRow;
		if (!it->empty())
		{
			CString str = pThis->m_datas[nRow][nCol];
			pdi->item.varValue = str;
		}
		else
		{
			pdi->item.varValue = "�����ڴ����";
		}
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
	//ResizeParentToFit();


	m_cTreeCtrl.SetImageList(&m_TreeImages, TVSIL_NORMAL);
	//��ʼ�����
	CRect rectGrid;
	m_wndGridLocation.GetClientRect(&rectGrid);
	m_wndGridLocation.MapWindowPoints(this, &rectGrid); //תΪ��������

	m_wndGrid.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	//m_wndGrid.SetCustomColors(-1, -1, -1, -1, -1, RGB(0, 0, 0)); //��ɫ�߿�
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.InsertColumn(0, _T("��Ϣ"), rectGrid.Width());
	//ע�������б�ص�����
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

	// TODO:  �ڴ˴������Ϣ����������
	m_tcpServer.ShutDown();
}


int CSMS_SERVERView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
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
			//���������ṹ
			TV_INSERTSTRUCT tvis;

			//�趨���ṹ����
			tvis.hParent = TVI_ROOT;				// Handle to parent item
			tvis.hInsertAfter = TVI_LAST;				// TVI_SORT = Sort items in tree
			tvis.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_TEXT;// item props we will fill	
			tvis.item.lParam = (LPARAM)pClient;		// item data: pointer to object
			tvis.item.pszText = LPSTR_TEXTCALLBACK;	// item text: defer to OnGetDispInfo ()
			tvis.item.iImage = I_IMAGECALLBACK;		// item image: defer to OnGetDispInfo ()
			tvis.item.iSelectedImage = I_IMAGECALLBACK;	// item selimage: defer to OnGetDispInfo ()

			//���������
			pThis->m_cTreeCtrl.InsertItem(&tvis);
		}

		CString strMsg;
		strMsg.Format("�������ͻ��ˡ�%s��������", pClient->m_sHostIPPort);
		CStrs strs;
		strs.push_back(strMsg);
		pThis->m_datas.push_back(strs);
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
				strMsg.Format("�յ��ͻ���ͼ����Ϣ��%s %dX%d ���ճɹ�", FileNum, wid, hei);
				pThis->SaveBmp(FileNum, picBuf, wid, hei, imgSize);
				CStrs strs;
				strs.push_back(strMsg);
				pThis->m_datas.push_back(strs);
				pThis->ListFresh();
			}
			else
			{
				strMsg.Format("�յ��ͻ���ͼ����Ϣ��%s %dX%d ����ͼ������ʧ��", FileNum, wid, hei);
				pThis->SaveBmp(FileNum, picBuf, wid, hei, imgSize);
				CStrs strs;
				strs.push_back(strMsg);
				pThis->m_datas.push_back(strs);
				pThis->ListFresh();
			}

			delete[] picBuf; picBuf = NULL;
		}//end MsgType 1
		else if (MsgType == 2) //����ƽ̨
		{
			BYTE Flag = 0;
			pClient->Receive(&Flag, 1);
			int number = 0;
			pClient->Receive(&number, 4);
			vector<CString> vFiles;
			for (int i = 0; i < number; i++)
			{
				char FileNum[9] = { 0 };
				pClient->Receive(&FileNum, 8); //8λ�����ֵ�����
				vFiles.push_back(FileNum);
			}
			pThis->SendSMS(Flag, vFiles);
		}// end MsgType 2
	}
}

void CSMS_SERVERView::SendSMS(BYTE flag, vector<CString>& vFiles)
{
	//��ѯ�ֻ��ź�����
	CDStrs datas;
	CString strMsg;
	CString strSQL;
	int nCount = vFiles.size();
	for (int i = 0; i < nCount; i++)
	{
		if (vFiles[i].IsEmpty()) continue;

		CDStrs tmp;
		strSQL.Format("SELECT SNAME, TEL FROM students WHERE FILE_NUMBER='%s'", vFiles[i]);
		if (g_mysqlCon.ExecuteQuery(strSQL, tmp, strMsg))
		{
			datas.push_back(tmp[0]);
			m_datas.push_back(tmp[0]);
		}
		else
		{
			CStrs strs;
			strs.push_back(strMsg);
			m_datas.push_back(strs);
			ListFresh();
		}
	}
	CStrs strs;
	strs.push_back("��ѯͨѶ��Ϣ�ɹ�");
	m_datas.push_back(strs);
	ListFresh();

	//���Ͷ���
	if (flag == 1)
	{
		CHttpClient hPost;
		CString strUrl("https://sms.253.com/msg/send");
		CString strPosData;
		string strResponse("");
		strMsg.Format("��253��ͨѶ���𾴵�%s�����������˵����������˳��ͨ����ˣ�\
			����2017��6��23�գ����ڶ�������8�㣺30��֮ǰ����У�μӿ�Ŀһ��105����\
			��ѧϰ����ַ���Ͼ��к�ɽ·90�ŵڶ����ң���лл������ϣ�", m_datas[0][0]);
		//strMsg.Format("��253��ͨѶ��������֤���ǻƽ���");
		char* cstring = EncodeToUTF8(strMsg);
		strPosData.Format("un=N5676872&pw=clERDUIcs@17&phone=%s&msg=%s&rd=1", m_datas[0][1], cstring);
		hPost.HttpPost(strUrl, strPosData, strResponse);
		strMsg = strResponse.c_str();
	}//end flag 1
	else if (flag == 2) //�˿�֪ͨ
	{

	}// end flag 2


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
		strMsg.Format("�ͻ��ˡ�%s���Ͽ�����", pClient->m_sHostIPPort);
		CStrs strs;
		strs.push_back(strMsg);
		pThis->m_datas.push_back(strs);
		pThis->ListFresh();
		pThis->m_cTreeCtrl.Invalidate();
	}
}



void CSMS_SERVERView::ListFresh()
{
	if (m_datas.size() > 5000)
	{
		//m_cs.Lock();
		std::vector<CStrs>::iterator it = m_datas.begin();
		m_datas.erase(it);
		//m_cs.Unlock();
	}

	m_wndGrid.RemoveAll();
	m_wndGrid.SetVirtualRows(m_datas.size());// 10000000);
	m_wndGrid.AdjustLayout();

}
