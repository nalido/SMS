// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "OutputBar.h"
#include "xPublic\ThreadBase.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\TCPClient.h"


#define COLOR_SELECTED RGB(0, 117, 194)
#define COLOR_TEXTSEL RGB(255, 255, 255)
#define COLOR_TEXTNONE RGB(220, 220, 220)
#define COLOR_LITTLE RGB(250, 132, 132)
#define COLOR_NONE RGB(190, 190, 190)
#define COLOR_MANY RGB(147, 235, 149)

enum VIEW_TYPE{
	VIEW_REGISTER = 0,
	VIEW_K1CHECK,
	VIEW_BOOKING1,
	VIEW_BOOKING2,
	VIEW_K1EXAM,
	VIEW_STUPROGRESS,
	VIEW_COACHES,
	VIEW_SCHOOL,
	VIEW_SYSTEM,
	VIEW_NUM
};

class CMainFrame : public CBCGPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPRibbonStatusBar	m_wndStatusBar;
	CBCGPRibbonBar			m_wndRibbonBar;
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;
public:
	COutputBar				m_wndOutput;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnRibbonCustomize (WPARAM wp, LPARAM lp);
	afx_msg void OnToolsOptions();
	afx_msg void OnViewOutput();
	afx_msg BOOL OnViewSelected(UINT nID);
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg LRESULT OnRedraw(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	BOOL CreateRibbonBar ();
	void ShowOptions(int nPage);

	CView* GetView(int nID);
	CArray<CView*, CView*>	m_arViews;
public:
	void SelectView(int nID);
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);
	afx_msg void OnClose();


	//SQL
	xPublic::CThreadBase m_threadMySQL;
	static void CALLBACK ThreadMySQLCallback(LPVOID pParam, HANDLE hCloseEvent);

	//SOCKET
	xPublic::CTCPClient m_tcpClient;
	xPublic::CThreadBase m_threadSocket;
	BYTE* m_pSendBuf; //�������ݻ���
	int m_nSendLen; //�������ݳ���
	BOOL m_isSendReady; //�Ƿ�׼���÷�������
	static void CALLBACK ThreadSocketCallback(LPVOID pParam, HANDLE hCloseEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void SaveBmp(char* FileNum, BYTE* picBuf, int wid, int hei, int imgSize);//����ͼ��
};

extern CString g_strFilePath;
extern xPublic::CMySQLEx g_mysqlCon;
extern void LOG(CString sFileName, CString str_log, int flag = 1);
extern void ShowMsg2Output1(CString strMsg); //�����Ӵ�����ʾ��Ϣ��output1�С� û��ʹ�������б�����ֻ������ʾ��ǰ���ڵ���Ϣ
extern CString g_sServerIP; //������IP
extern char* EncodeToUTF8(const char* mbcsStr);

enum enum_StudentProgress{
	SP_NEWONE = 0,			//������¼
	SP_K0PASS,				//����ͨ��	
	SP_K0SMS,				//����ͨ������
	SP_K1WAIT,				//��һ��������
	SP_K1SMS,				//
	SP_K1EXAM,				//��һ����
	SP_K1PASS,				//
	SP_K2K3BOOKING,			//·ѵԤԼ	
	SP_K2K3BOOKED			//ԤԼ���
};