// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "OutputBar.h"
#include "xPublic\ThreadBase.h"
#include "xPublic\MySQLEx.h"
#include "xPublic\TCPClient.h"



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

	CArray<CView*, CView*>	m_arViews;
public:
	CView* GetView(int nID);
	void SelectView(int nID);
	afx_msg LRESULT OnUserMessage(WPARAM wp, LPARAM lp);
	afx_msg void OnClose();


	//SQL
	xPublic::CThreadBase m_threadMySQL;
	static void CALLBACK ThreadMySQLCallback(LPVOID pParam, HANDLE hCloseEvent);

	//SOCKET
	xPublic::CTCPClient m_tcpClient;
	xPublic::CThreadBase m_threadSocket;
	BYTE* m_pSendBuf; //发送数据缓存
	int m_nSendLen; //发送数据长度
	BOOL m_isSendReady; //是否准备好发送数据
	static void CALLBACK ThreadSocketCallback(LPVOID pParam, HANDLE hCloseEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void SaveBmp(char* FileNum, BYTE* picBuf, int wid, int hei, int imgSize);//保存图像
};

extern CString g_strFilePath;
extern xPublic::CMySQLEx g_mysqlCon;
extern void LOG(CString sFileName, CString str_log, int flag = 1);
extern void ShowMsg2Output1(CString strMsg); //用于子窗口显示信息到output1中。 没有使用虚拟列表技术，只用于显示当前窗口的信息
extern CString g_sServerIP; //服务器IP
extern char* EncodeToUTF8(const char* mbcsStr);
extern int g_nClassTotal; //每个课时每天可以预约的总数
extern int g_nMaxBooking; //每个学员最多预约课数
extern int g_nSubForLeave; //教练缺勤一次减去的学员数量
extern int g_nMinWorkTime; //教练一个月最少工时

extern CString g_strUserID; //当前用户工号
extern int g_nPermissions[6]; //当前用户的权限

enum enum_StudentProgress{
	SP_NEWONE = 0,			//新生记录
	SP_K0PASS,				//政审通过	
	SP_K0SMS,				//政审通过短信
	SP_K1WAIT,				//科一报考申请
	SP_K1SMS,				//
	SP_K1EXAM,				//科一考试
	SP_K1PASS,				//
	SP_K2K3BOOKING,			//路训预约	
	SP_K2K3BOOKED			//预约完成
};


#define COLOR_SELECTED RGB(0, 117, 194)
#define COLOR_TEXTSEL RGB(255, 255, 255)
#define COLOR_TEXTNONE RGB(220, 220, 220)
#define COLOR_TEXTNORMAL RGB(0, 0, 0)
#define COLOR_LITTLE RGB(250, 132, 132)
#define COLOR_NONE RGB(190, 190, 190)
#define COLOR_MANY RGB(147, 235, 149)
#define COLOR_COMPLETE RGB(149, 200, 146)
#define COLOR_DOING RGB(195, 218, 195)
#define COLOR_DONE RGB(149, 200, 146)

enum VIEW_TYPE{
	VIEW_MAIN = 0,
	VIEW_HOME,
	VIEW_REGISTER,
	VIEW_K1CHECK,
	VIEW_BOOKING1,
	VIEW_BOOKING2,
	VIEW_ORDER_RSP,
	VIEW_K1EXAM,
	VIEW_STUPROGRESS,
	VIEW_COACHES,
	VIEW_KPI,
	VIEW_DEVICES,
	VIEW_SCHOOL,
	VIEW_SYSTEM,
	VIEW_SCAN,
	VIEW_STUFFENTER,
	VIEW_STUDENTENTER,
	VIEW_PERMISSION,
	VIEW_NUM
};

typedef struct struct_STUDENTINFO
{
	CString strName; //学员名字
	CString strFileName; //学员档案名
	CString strGender; //学员性别
	CString strCarType; //申领车型
	CString strSignDate; //报名日期
	CString strBirthDay; //出生日期
	CString strTEL; //电话
	CString strIDCard; //身份证
	CString strHome; //住址
	CString strFee; //费用


	int nClassBooked; //已预约课时
	int nClassStep; //课程进度,已用课时

	struct_STUDENTINFO()
	{
		strName = "";
		strFileName = "";
		strGender = "";
		strCarType = "";
		strSignDate = "";
		strBirthDay = ""; //出生日期
		strTEL = ""; //电话
		strIDCard = ""; //身份证
		strHome = ""; //住址
		strFee = ""; //费用

		nClassStep = 0;
		nClassBooked = 0;
	}

	struct_STUDENTINFO(CString name, CString filename, CString gender, CString cartype)
	{
		strName = name;
		strFileName = filename;
		strGender = gender;
		strCarType = cartype;
	}
}STUDENTINFO;


extern CString GetClassTime(int n); //根据时段编号获得具体时间
extern CString GetLastMonth(CTime& thisMonth); //获取上个月的月份
extern CTime Str2Time(CString str); //解析字符串得到时间
extern void ExportExcel(std::vector<CString>& titles, CDStrs &datas); //数据导出到excel
