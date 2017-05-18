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
	int ndata = pThis->m_arMsg.GetCount(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		SENDFILEPARAM * pParam= pThis->m_arMsg.GetAt(nRow);
		if (pParam != NULL && nCol == 0)
			pdi->item.varValue = pParam->strMsg;
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
		pThis->m_arMsg.AddTail(strMsg);
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
				pThis->m_arMsg.AddTail(strMsg);
				pThis->ListFresh();
			}
			else
			{
				strMsg.Format("�յ��ͻ���ͼ����Ϣ��%s %dX%d ����ͼ������ʧ��", FileNum, wid, hei);
				//pThis->SaveBmp(FileNum, picBuf, wid, hei, imgSize);
				pThis->m_arMsg.AddTail(strMsg);
				pThis->ListFresh();
			}

			delete[] picBuf; picBuf = NULL;
		}//end MsgType 1
		else if (MsgType == 2) //����ƽ̨
		{
			BYTE Flag = 0;
			pClient->Receive(&Flag, 1);
			int SMSsize = 0;
			pClient->Receive(&SMSsize, 4);
			BYTE* SMSBuf = new BYTE[SMSsize + 1];

			if (pClient->Receive(SMSBuf, SMSsize))
			{
				CString strSMS; 
				SMSBuf[SMSsize] = '\0';
				strSMS.Format("%s", SMSBuf);
				pThis->SendSMS(Flag, strSMS);
			}
		}// end MsgType 2
		else if (MsgType == 3) //ѧ��ͼƬѰ��
		{
			char FileNum[11] = { 0 };
			pClient->Receive(&FileNum, 10);
			CString strFile;
			strFile.Format("%s\\%s.bmp", g_strFilePath, FileNum);
			char* file = strFile.GetBuffer();
			cv::Mat img = cv::imread(file);
			strFile.ReleaseBuffer();
			if (!img.empty())
			{
				IplImage ipl_img = img;
				int len = ipl_img.imageSize + 23;
				int w = img.cols;
				int h = img.rows;
				BYTE* buf = new BYTE[len];
				buf[0] = 1; //��־λ
				memcpy(buf + 1, FileNum, 10); //������
				memcpy(buf + 11, &img.cols, 4);
				memcpy(buf + 15, &img.rows, 4);
				memcpy(buf + 19, &ipl_img.imageSize, 4);
				memcpy(buf + 23, ipl_img.imageData, ipl_img.imageSize);
				if (pClient->Send(buf, len))
				{
					pThis->m_arMsg.AddTail("ͼ���ͳɹ�");
					pThis->ListFresh();
				}
				else
				{
					pThis->m_arMsg.AddTail("ͼ����ʧ��");
					pThis->ListFresh();
				}
				if (buf != NULL)
				{
					delete[] buf; buf = NULL;
				}
			}
			else
			{
				BYTE buf = 0; //��־λ
				if (pClient->Send(&buf, 1))
				{
					pThis->m_arMsg.AddTail("δ�ҵ�ͼ�����ݷ��ͳɹ�");
					pThis->ListFresh();
				}
				else
				{
					pThis->m_arMsg.AddTail("δ�ҵ�ͼ�����ݷ���ʧ��");
					pThis->ListFresh();
				}
			}
		}// end MsgType 3
		else if (MsgType == 'P') //POST ���� //�ֻ��˽ӿ�
		{
			BYTE data[1024] = { 0 };
			pClient->Receive(data, 1023);
			CString strLog;
			strLog.Format("%s", data);
			LOG("receive.log", strLog);

			int pos = strLog.Find("HTTP");
			CString strHttpVersion = strLog.Mid(pos, 8);
			CString strBuf("");
			CString strResponse("response=received,200\nkkk");
			strBuf.Format("%s 200 OK\r\n\
							Content-Type: application/x-www-form-urlencoded\r\n\
							Content-Length: %d\r\n\
							\r\n\
							%s",
							strHttpVersion, strlen(strResponse), strResponse);
			strBuf.Remove('\t'); //ȥ����ʽ�е��Ʊ��
			pClient->Send(strBuf.GetBuffer(), strlen(strBuf));
			strBuf.ReleaseBuffer();
		}//end MsgType POST
	}
}

void CSMS_SERVERView::SendSMS(BYTE flag, CString& vFiles)
{
	CHttpClient hPost;
	CString strUrl("http://121.40.160.86:7890/msgapiv2.aspx"); //���Ͷ���
	CString strPosData;
	string strResponse("");

	if (vFiles.IsEmpty()) return;

	//TEL:ID>MSG flg<60
	//TEL:MSG  flag=60
	int pos1 = vFiles.Find(":");
	if (pos1 == -1) return; //invalid
	CString strTel = vFiles.Left(pos1);

	CString strStuID;
	int pos2 = vFiles.Find(">");
	if (flag < 60)
	{
		if (pos2 == -1) return; //��Ч��Ϣ

		strStuID = vFiles.Mid(pos1+1, pos2-pos1-1);
	}
	else if (flag==60)
	{
		pos2 = pos1;
	}

	CString strSMS = vFiles.Mid(pos2 + 1);
	
	strPosData.Format("action=send&username=dhjx&password=c739fa3c630ca4e65ac9efdc8317df7d&apiid=13952&mobiles=%s&text=%s", strTel, strSMS);
	char* posdata = EncodeToUTF8(strPosData);
	hPost.HttpPost(strUrl, posdata, strResponse);
	strSMS = strResponse.c_str();

	if (flag < 5)
	{
		//�������ݿ�
		CString strMsg, strSQL;
		int step = 0;
		switch (flag)
		{
		case 1:
			step = 2;
			break;
		case 2:
			step = 1001;
			break;
		case 3:
			step = 4;
			break;
		case 4:
			step = 7;
			break;
		}
		strSQL.Format("UPDATE students SET STEP='%d' WHERE FILE_NAME='%s'", step, strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	}
	else if (flag==6 || flag==7) //·ѵ����
	{
		//�������ݿ�
		CString strMsg, strSQL;
		CString strTypeName = "K2_STAT";
		if (flag == 7)
			strTypeName = "K3_STAT";
		strSQL.Format("UPDATE stuDates SET %s='2' WHERE STU_ID='%s'", strTypeName, strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	}

	m_arMsg.AddTail("���ͳɹ���");
	ListFresh();
	return;
}

void CSMS_SERVERView::SaveBmp(char* FileNum, BYTE* picBuf, int wid, int hei, int imgSize)
{
	IplImage* pImg = cvCreateImageHeader(cvSize(wid, hei), 8, 3);
	int lineByte = (wid*3 + 3) / 4 * 4;
	cvSetData(pImg, picBuf, lineByte);

	cv::Mat img = cv::cvarrToMatND(pImg);
	CString sFileName("");
	sFileName.Format("%s%s.bmp", g_strFilePath, FileNum);
	::SHCreateDirectory(NULL, CA2W(g_strFilePath));
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
		pThis->m_arMsg.AddTail(strMsg);
		pThis->ListFresh();

		//�������ṹɾ���ѹرյĿͻ���
		HTREEITEM hItem = NULL;
		hItem = pThis->m_cTreeCtrl.GetRootItem();
		while (hItem != NULL)
		{
			CTCPClient* pClients = (CTCPClient*)pThis->m_cTreeCtrl.GetItemData(hItem);
			if (pClient == pClients) //�ҵ��ýڵ�
			{
				pThis->m_cTreeCtrl.DeleteItem(hItem);
				break;
			}
			else
			{
				hItem = pThis->m_cTreeCtrl.GetNextSiblingItem(hItem);
			}
		}
		pThis->m_cTreeCtrl.Invalidate();
	}
}



void CSMS_SERVERView::ListFresh()
{
	try
	{
		m_wndGrid.RemoveAll();
		m_wndGrid.SetVirtualRows(m_arMsg.GetCount());
		m_wndGrid.AdjustLayout();
	}
	catch (...)
	{
		ShowMsg2Output1("ˢ�������б����");
	}

}
