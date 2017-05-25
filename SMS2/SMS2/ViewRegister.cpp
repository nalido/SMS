// ViewRegister.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewRegister.h"
#include "xPublic\MySQLEx.h"
#include "MainFrm.h"


using namespace cv;
// CViewRegister

IMPLEMENT_DYNCREATE(CViewRegister, CBCGPFormView)

CViewRegister::CViewRegister()
: CBCGPFormView(CViewRegister::IDD)
, m_isCaptured(FALSE)
, m_canCap(FALSE)
, m_nDataType(DATA_NEW)
{
	EnableVisualManagerStyle();


	m_strNumber = ""; //档案号
}

CViewRegister::~CViewRegister()
{
}

void CViewRegister::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHOTO, m_SPhoto);
	DDX_Control(pDX, IDC_NUMBER, m_Sta_Num);
	DDX_Control(pDX, IDC_DATE_SIGNIN, m_Date_Sign);
	DDX_Control(pDX, IDC_CARTYPE, m_Comb_CarType);
	DDX_Control(pDX, IDC_GENDER, m_Comb_Gender);
	DDX_Control(pDX, IDC_NAME, m_Ed_Name);
	DDX_Control(pDX, IDC_BIRTH, m_Date_Birth);
	DDX_Control(pDX, IDC_TEL, m_Ed_Tel);
	DDX_Control(pDX, IDC_ID, m_Ed_ID);
	DDX_Control(pDX, IDC_HOME, m_Ed_Home);
	DDX_Control(pDX, IDC_FEE, m_Ed_Fee);


	//背景设置
	DDX_Control(pDX, IDC_STATIC0, m_staticText[0]);
	DDX_Control(pDX, IDC_STATIC1, m_staticText[1]);
	DDX_Control(pDX, IDC_STATIC2, m_staticText[2]);
	DDX_Control(pDX, IDC_STATIC3, m_staticText[3]);
	DDX_Control(pDX, IDC_STATIC4, m_staticText[4]);
	DDX_Control(pDX, IDC_STATIC5, m_staticText[5]);
	DDX_Control(pDX, IDC_STATIC6, m_staticText[6]);
	DDX_Control(pDX, IDC_STATIC7, m_staticText[7]);
	DDX_Control(pDX, IDC_STATIC8, m_staticText[8]);
	DDX_Control(pDX, IDC_STATIC9, m_staticText[9]);


	DDX_Control(pDX, IDC_BKBMP, m_BKpos1);
	DDX_Control(pDX, IDC_BKBMP1, m_BKpos2);
	DDX_Control(pDX, IDC_CONTENT, m_Content);
}

BEGIN_MESSAGE_MAP(CViewRegister, CBCGPFormView)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CAMERA, &CViewRegister::OnBnClickedCamera)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_WM_TIMER()
//	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SIGN, &CViewRegister::OnBnClickedBtnSign)
	ON_BN_CLICKED(IDC_NEWFILE, &CViewRegister::OnBnClickedNewfile)
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CLEAR, &CViewRegister::OnBnClickedClear)
END_MESSAGE_MAP()


// CViewRegister 诊断

#ifdef _DEBUG
void CViewRegister::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewRegister::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewRegister 消息处理程序


void CViewRegister::OnBnClickedCamera()
{
	CString str;
	GetDlgItem(IDC_CAMERA)->GetWindowText(str);
	if (str == "打开摄像头" || str == "重新采集照片")
	{

		if (!m_videoCap.isOpened()) m_videoCap.open(0);
		if (!m_videoCap.isOpened())
		{
			MessageBox("摄像头打开失败");
		}

		//init the timer
		SetTimer(0, 50, NULL);
		GetDlgItem(IDC_CAMERA)->SetWindowTextA("采集照片");
	}
	else if (str == "采集照片")
	{
		KillTimer(0);
		m_isCaptured = TRUE;
		ShowMsg2Output1("拍照成功");
		GetDlgItem(IDC_CAMERA)->SetWindowTextA("重新采集照片");
	}
}


void CViewRegister::OnTimer(UINT_PTR nIDEvent)
{

	CRect rect;
	m_SPhoto.GetClientRect(&rect);
	CDC* pDc = m_SPhoto.GetDC();
	HDC hdc = pDc->GetSafeHdc();

	cv::Mat cap;
	m_videoCap >> cap;
	if (!cap.empty())
	{
		m_cap = cap(cv::Rect(150, 2, 340, 476)).clone(); //按照5:7的一寸照片比例截取
		m_SPhoto.Invalidate();
		//IplImage* frame;
		//frame = &IplImage(m_cap);
		//CvvImage cvvImage;
		//cvvImage.CopyOf(frame);
		//cvvImage.DrawToHDC(hdc, rect);
	}

	CBCGPFormView::OnTimer(nIDEvent);
}

LRESULT CViewRegister::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;
	if (flag == 1)
	{
		m_nLastView = VIEW_REGISTER;
		m_nDataType = DATA_NEW;
		GetDlgItem(IDC_BTN_SIGN)->SetWindowTextA("注册");
		GetDlgItem(IDC_BTN_SIGN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLEAR)->ShowWindow(SW_HIDE);
		OnBnClickedClear();
		KillTimer(0);
		m_isCaptured = FALSE;
		GetDlgItem(IDC_CAMERA)->SetWindowTextA("打开摄像头");
		m_cap.setTo(255);
	}
	else if (flag == 2)
	{
		m_nLastView = VIEW_K1CHECK;
		m_nDataType = DATA_EDIT;
		GetDlgItem(IDC_BTN_SIGN)->SetWindowTextA("确认修改");
		GetDlgItem(IDC_BTN_SIGN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLEAR)->ShowWindow(SW_SHOW);
		KillTimer(0);
		m_isCaptured = FALSE;
		GetDlgItem(IDC_CAMERA)->SetWindowTextA("重新采集照片");


		CString strStuID = (char*)lParam;
		CString strMsg, strSQL;

		strSQL.Format("SELECT REGIST_DATE, CAR_TYPE, SNAME, GENDER, BIRTHDAY, TEL, FEE, ID, HOME FROM students WHERE FILE_NAME='%s'", strStuID);
		CDStrs datas;
		if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg) && datas.size()>0)
		{
			m_strNumber = strStuID;
			CString date = datas[0][0];
			CString type = datas[0][1];
			CString name = datas[0][2];
			CString gender = datas[0][3];
			CString birth = datas[0][4];
			CString tel = datas[0][5];
			CString fee = datas[0][6];
			CString id = datas[0][7];
			CString home = datas[0][8];

			m_Sta_Num.SetWindowTextA(m_strNumber);
			m_Ed_Name.SetWindowTextA(name);
			m_Ed_Tel.SetWindowTextA(tel);
			m_Ed_Fee.SetWindowTextA(fee);
			m_Ed_ID.SetWindowTextA(id);
			m_Ed_Home.SetWindowTextA(home);
			m_Comb_CarType.SetWindowTextA(type);
			m_Comb_Gender.SetWindowTextA(gender);
			m_Date_Sign.SetWindowTextA(date);
			m_Date_Birth.SetWindowTextA(birth);
			CRect rect;
			m_Sta_Num.GetClientRect(&rect);
			m_Sta_Num.MapWindowPoints(this, &rect);
			InvalidateRect(&rect, TRUE); //重绘背景 消除重影


			//本地打开照片，若本地无，则查询服务器下载
			ShowMsg2Output1("选择修改对象：档案" + m_strNumber);
			CString strFile;
			strFile.Format("%s\\%s.bmp", g_strFilePath, m_strNumber);
			char* file = strFile.GetBuffer();
			m_cap = cv::imread(file);
			strFile.ReleaseBuffer();
			if (m_cap.empty()) //本地无照片，从服务器下载
			{
				//数据打包
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				if (pFrame->m_pSendBuf != NULL)
				{
					MessageBox("上一个信息还未处理完毕，请稍等重试。");
				}
				else
				{
					int len = 11; //
					pFrame->m_isSendReady = FALSE;
					pFrame->m_pSendBuf = new BYTE[len];//发送完删除
					pFrame->m_nSendLen = len;
					pFrame->m_pSendBuf[0] = 3; //请求图像数据
					char* cID = m_strNumber.GetBuffer();
					memcpy(pFrame->m_pSendBuf + 1, cID, 10); //档案号
					m_strNumber.ReleaseBuffer();
					pFrame->m_isSendReady = TRUE;
				}
			}
			else
			{
				m_isCaptured = TRUE;
				m_SPhoto.Invalidate();
			}
		}
		else
		{
			MessageBox("查询学员信息失败！");
			return 0;
		}


	}

	return 0;
}

void CViewRegister::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	//获取背景定位点位置
	m_BKpos1.GetClientRect(&m_rctBK1);
	m_BKpos1.MapWindowPoints(this, &m_rctBK1);
	m_BKpos2.GetClientRect(&m_rctBK2);
	m_BKpos2.MapWindowPoints(this, &m_rctBK2);

	m_Content.GetClientRect(&m_rctContent);
	m_Content.MapWindowPoints(this, &m_rctContent);

	///******初始化一个摄像头捕捉器******/

	//if (!m_videoCap.isOpened()) m_videoCap.open(0);
	//if (!m_videoCap.isOpened())
	//{
	//	MessageBox("Failed to open camera");
	//}

	m_Comb_CarType.AddString("C1手动挡");
	m_Comb_CarType.AddString("C2自动挡");
	m_Comb_CarType.AddString("B2大货车");
	m_Comb_CarType.SetCurSel(0);

	m_Comb_Gender.AddString("男");
	m_Comb_Gender.AddString("女");

	m_Date_Sign.SetFormat("yyyy/MM/dd");
	m_Date_Birth.SetFormat("yyyy/MM/dd");

	GetDlgItem(IDC_BTN_SIGN)->EnableWindow(FALSE); //必须先有档案号
	GetDlgItem(IDC_NEWFILE)->EnableWindow(TRUE);

	m_SPhoto.InitPicSource(&m_cap);
}


void CViewRegister::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//双缓存绘制
	CRect   rect;
	GetClientRect(&rect);
	MapWindowPoints(this, &rect);
	CDC     MemDC;
	CBitmap MemMap;


	Gdiplus::Point p1(m_rctBK1.left, m_rctBK1.top);
	Gdiplus::Point p2(m_rctBK2.right, m_rctBK2.bottom);
	int width = m_rctBK2.right - m_rctBK1.left;
	int height = m_rctBK2.bottom - m_rctBK1.top;
	int  W = GetSystemMetrics(SM_CXSCREEN);  //得到屏幕宽度 
	int  H = GetSystemMetrics(SM_CYSCREEN);
	width = max(W, width);
	height = max(H, height);

	MemDC.CreateCompatibleDC(&dc); 
	MemMap.CreateCompatibleBitmap(&dc, width, height);
	MemDC.SelectObject(&MemMap);

	Graphics graph(MemDC.m_hDC);

	Image img(L"res//r1.jpg");
	graph.DrawImage(&img, Gdiplus::Rect(p1.X, p1.Y, width, height));

	SolidBrush brush(Color(150, 230, 230, 230));
	graph.FillRectangle(&brush, m_rctContent.left, m_rctContent.top, m_rctContent.Width(), m_rctContent.Height());

	//复制内存DC到屏幕上
	CPoint pos = GetScrollPosition();
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);
	MemDC.DeleteDC();
	MemMap.DeleteObject();

	//if (m_isCaptured)
	//{
	//	CRect rect;
	//	m_SPhoto.GetClientRect(&rect);
	//	CDC* pDc = m_SPhoto.GetDC();
	//	HDC hdc = pDc->GetSafeHdc();
	//	if (!m_cap.empty())
	//	{
	//		IplImage* frame;
	//		frame = &IplImage(m_cap);
	//		CvvImage cvvImage;
	//		cvvImage.CopyOf(frame);
	//		cvvImage.DrawToHDC(hdc, rect);
	//	}
	//}
}


//void CViewRegister::OnSize(UINT nType, int cx, int cy)
//{
//	CBCGPFormView::OnSize(nType, cx, cy);
//
//	//int margin = 200; //控件外边距
//	//if (m_SPhoto) //控制控件位置
//	//{
//	//	CRect rct;
//	//	m_SPhoto.GetClientRect(&rct);
//	//	m_SPhoto.MapWindowPoints(this, &rct);
//	//	rct.MoveToX(cx - rct.Width() - margin);
//
//	//	m_SPhoto.MoveWindow(rct);
//	//}
//}


void CViewRegister::OnBnClickedBtnSign()
{
	UpdateData(TRUE); //获取输入

	if (!m_isCaptured){
		MessageBox("请打开摄像头进行照片采集！");
		return;
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	//m_strNumber = ""; //档案号
	CString name, type, tel, fee, id, home, birth, date, gender;
	m_Ed_Name.GetWindowText(name);
	m_Ed_Tel.GetWindowText(tel);
	m_Ed_Fee.GetWindowText(fee);
	m_Ed_ID.GetWindowText(id);
	m_Ed_Home.GetWindowText(home);
	m_Comb_CarType.GetWindowText(type);
	m_Comb_Gender.GetWindowText(gender);
	m_Date_Sign.GetWindowText(date); 
	m_Date_Birth.GetWindowText(birth);
	if (name.IsEmpty() || type.IsEmpty() || tel.IsEmpty()
		|| fee.IsEmpty() || id.IsEmpty() || home.IsEmpty()
		|| birth.IsEmpty() || date.IsEmpty() || gender.IsEmpty())
	{
		MessageBox("数据不完整！");
		ShowMsg2Output1("没填完数据就点击了注册按钮");
	}
	else
	{
		CString strMsg("");
		CString strSQL("");

		if (m_nDataType == DATA_EDIT)
		{
			strSQL.Format("DELETE FROM students WHERE FILE_NAME='%s'", m_strNumber);
			if (!g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				MessageBox("操作失败，请检查网络连接！");
				pFrame->SelectView(m_nLastView);
				return;
			}
		}

		strSQL.Format("INSERT INTO STUDENTS \
			(ID, SNAME, BIRTHDAY, FILE_NUMBER, FILE_NAME, REGIST_DATE, CAR_TYPE, TEL, HOME, FEE, GENDER, STEP) \
			VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '0')",
			id, name, birth, m_strNumber.Right(8), m_strNumber, date, type, tel, home, fee, gender);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		{
			MessageBox("操作成功！");
			ShowMsg2Output1(name + "操作成功！");
			GetDlgItem(IDC_BTN_SIGN)->EnableWindow(FALSE); //防止重复注册同一个信息
			GetDlgItem(IDC_NEWFILE)->EnableWindow(TRUE);

			//保存照片
			CString sFileName("");
			sFileName.Format("%s\\", g_strFilePath);
			::SHCreateDirectory(NULL, CA2W(sFileName));
			sFileName.Format("%s\\%s.bmp", g_strFilePath, m_strNumber);
			cv::String s = sFileName.GetBuffer();
			imwrite(s, m_cap);
			sFileName.ReleaseBuffer();

			//数据打包
			//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			IplImage ipl_img = m_cap;
			int len = ipl_img.imageSize + 23;
			if (pFrame->m_pSendBuf != NULL)
			{ 
				MessageBox("上一个信息还未处理完毕，请稍等重试。");
			}
			else
			{
				pFrame->m_isSendReady = FALSE;
				pFrame->m_pSendBuf = new BYTE[len];//发送完删除
				pFrame->m_nSendLen = len;
				pFrame->m_pSendBuf[0] = 1; //发送图像数据
				char* cID = m_strNumber.GetBuffer();
				memcpy(pFrame->m_pSendBuf + 1, cID, 10); //档案号
				m_strNumber.ReleaseBuffer();
				memcpy(pFrame->m_pSendBuf + 11, &m_cap.cols, 4); //图像宽度
				memcpy(pFrame->m_pSendBuf + 15, &m_cap.rows, 4); //图像高度
				memcpy(pFrame->m_pSendBuf + 19, &ipl_img.imageSize, 4); //图像尺寸
				memcpy(pFrame->m_pSendBuf + 23, ipl_img.imageData, ipl_img.imageSize); //图像数据

				pFrame->m_isSendReady = TRUE;
			}
		}
		else
		{
			MessageBox("注册错误！");
			ShowMsg2Output1("注册操作错误：" + strSQL);
			//LOG("SQLERROR.log", strSQL);
			//LOG("SQLERROR.log", g_mysqlCon.GetError());
		}

		if (m_nLastView == VIEW_K1CHECK)
			pFrame->SelectView(m_nLastView);
	}
}

LRESULT CViewRegister::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)lParam;
	if (flag == 2)
	{
		cv::Mat* pImg = (cv::Mat*)wParam;
		m_cap = pImg->clone();
		m_SPhoto.Invalidate();
	}
	return 0;
}

void CViewRegister::OnBnClickedNewfile()
{
	//清空输入框
	m_Ed_Name.SetWindowTextA("");
	m_Ed_ID.SetWindowTextA("");
	m_Ed_Tel.SetWindowTextA("");
	m_Ed_Home.SetWindowTextA("");
	m_Ed_Fee.SetWindowTextA("");
	m_Comb_CarType.SetCurSel(0);


	CTime localtime;
	localtime = GetServerTime();//CTime::GetCurrentTime();

	//获取已有档案数量
	CDStrs data;
	CString strMsg("");

	CString strMonth;
	strMonth = (localtime.Format("%Y%m")).Right(4);
	CString strSQL("");
	strSQL.Format("SELECT MAX(FILE_NUMBER) FROM (SELECT FILE_NUMBER FROM students WHERE FILE_NUMBER LIKE '%s%%' ) as file_num", strMonth);
	if (g_mysqlCon.ExecuteQuery(strSQL, data, strMsg))
	{
		ShowMsg2Output1("查询档案数量数据成功！");
		int nfiles = 1;
		if (!data[0][0].IsEmpty())
		{
			if (atoi(data[0][0]) == 0) //整个记录表的第一条
			{
				nfiles = 1;
				m_strNumber.Format("DJ%s%04d", strMonth, nfiles);
			}
			else
			{
				nfiles = atoi(data[0][0]) + 1;
				m_strNumber.Format("DJ%d", nfiles);
			}
		}
		else
		{
			nfiles = 1;
			m_strNumber.Format("DJ%s%04d", strMonth, nfiles);
		}
		m_Sta_Num.SetWindowTextA(m_strNumber);
		m_nDataType = DATA_NEW;
		GetDlgItem(IDC_BTN_SIGN)->SetWindowTextA("注册");
		GetDlgItem(IDC_BTN_SIGN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CAMERA)->SetWindowTextA("打开摄像头");
		OnBnClickedCamera();
	}
	else
	{
		ShowMsg2Output1(_T("查询档案数量数据操作失败!\r\n") + strMsg);
	}

	CRect rect;
	m_Sta_Num.GetClientRect(&rect);
	m_Sta_Num.MapWindowPoints(this, &rect);
	InvalidateRect(&rect, TRUE); //重绘背景 消除重影

	UpdateData(FALSE); //更新显示
}


HBRUSH CViewRegister::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(NULL_BRUSH)); //返回一个空画刷
	}


	HBRUSH hbr = CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}


void CViewRegister::OnBnClickedClear()
{
	//清空输入框
	m_Sta_Num.SetWindowTextA("");
	m_Ed_Name.SetWindowTextA("");
	m_Ed_ID.SetWindowTextA("");
	m_Ed_Tel.SetWindowTextA("");
	m_Ed_Home.SetWindowTextA("");
	m_Ed_Fee.SetWindowTextA("");
	m_Comb_CarType.SetCurSel(0);
	m_cap.setTo(255);

	if (m_nLastView == VIEW_K1CHECK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->SelectView(m_nLastView);
	}
}
