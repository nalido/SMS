// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "OutputBar.h"
#include "xPublic\ThreadBase.h"
#include "xPublic\MySQLEx.h"

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


	//SQL
	xPublic::CThreadBase m_threadMySQL;
	static void CALLBACK ThreadMySQLCallback(LPVOID pParam, HANDLE hCloseEvent);
	afx_msg void OnClose();

	//��ʱ�������߳�
	xPublic::CThreadBase m_threadClock;
	static void CALLBACK ThreadClockCallback(LPVOID pParam, HANDLE hCloseEvent);

	//��ʱˢ�������Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

extern CString g_strFilePath;
extern xPublic::CMySQLEx g_mysqlCon;
extern void LOG(CString sFileName, CString str_log, int flag = 1);
extern void ShowMsg2Output1(CString strMsg); //�����Ӵ�����ʾ��Ϣ��output1�С�
extern CDStrs g_strMsgLog; //ȫ����Ϣ
extern CString g_strCurrentTime; //��ȡ���ݿ������ʱ��
extern CTime GetServerTime(); //����g_strCurrentTime�õ�ʱ��
extern BOOL g_isSMSSended; //�����Ƿ��Ѿ������Ͽ����Ѷ���
extern CString GetClassTime(int n); //��0��ʼ�Ŀ�ʱʱ���