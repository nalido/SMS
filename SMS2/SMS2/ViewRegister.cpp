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
	DDX_Control(pDX, IDC_CAMERA, m_Btn_Cap);
	DDX_Control(pDX, IDC_NUMBER, m_Sta_Num);
	DDX_Control(pDX, IDC_DATE_SIGNIN, m_Date_Sign);
	DDX_Control(pDX, IDC_CARTYPE, m_Comb_CarType);
	DDX_Control(pDX, IDC_NAME, m_Ed_Name);
	DDX_Control(pDX, IDC_BIRTH, m_Date_Birth);
	DDX_Control(pDX, IDC_TEL, m_Ed_Tel);
	DDX_Control(pDX, IDC_ID, m_Ed_ID);
	DDX_Control(pDX, IDC_HOME, m_Ed_Home);
	DDX_Control(pDX, IDC_FEE, m_Ed_Fee);
}

BEGIN_MESSAGE_MAP(CViewRegister, CBCGPFormView)
	ON_BN_CLICKED(IDC_CAMERA, &CViewRegister::OnBnClickedCamera)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SIGN, &CViewRegister::OnBnClickedBtnSign)
	ON_BN_CLICKED(IDC_NEWFILE, &CViewRegister::OnBnClickedNewfile)
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
	m_Btn_Cap.GetWindowTextA(str);
	if (str == "打开摄像头")
	{
		//init the timer
		SetTimer(0, 50, NULL);
		m_Btn_Cap.SetWindowTextA("采集照片");
	}
	else if (str == "采集照片")
	{
		KillTimer(0);
		m_isCaptured = TRUE;
		ShowMsg2Output1("拍照成功");
		m_Btn_Cap.SetWindowTextA("打开摄像头");
	}
}


void CViewRegister::OnTimer(UINT_PTR nIDEvent)
{

	CRect rect;
	m_SPhoto.GetClientRect(&rect);
	CDC* pDc = m_SPhoto.GetDC();
	HDC hdc = pDc->GetSafeHdc();

	m_videoCap >> m_cap;
	if (!m_cap.empty())
	{
		m_cap = m_cap(Rect(150, 2, 340, 476)); //按照5:7的一寸照片比例截取
		IplImage* frame;
		frame = &IplImage(m_cap);
		CvvImage cvvImage;
		cvvImage.CopyOf(frame);
		cvvImage.DrawToHDC(hdc, rect);
	}

	CBCGPFormView::OnTimer(nIDEvent);
}


void CViewRegister::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	/******初始化一个摄像头捕捉器******/

	if (!m_videoCap.isOpened()) m_videoCap.open(0);
	if (!m_videoCap.isOpened())
	{
		MessageBox("Failed to open camera");
	}

	m_Comb_CarType.AddString("C1手动挡");
	m_Comb_CarType.AddString("C2自动挡");
	m_Comb_CarType.AddString("B2大货车");
	m_Comb_CarType.SetCurSel(0);
}


void CViewRegister::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_isCaptured)
	{
		CRect rect;
		m_SPhoto.GetClientRect(&rect);
		CDC* pDc = m_SPhoto.GetDC();
		HDC hdc = pDc->GetSafeHdc();
		if (!m_cap.empty())
		{
			IplImage* frame;
			frame = &IplImage(m_cap);
			CvvImage cvvImage;
			cvvImage.CopyOf(frame);
			cvvImage.DrawToHDC(hdc, rect);
		}
	}
}


void CViewRegister::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	//int margin = 200; //控件外边距
	//if (m_SPhoto) //控制控件位置
	//{
	//	CRect rct;
	//	m_SPhoto.GetClientRect(&rct);
	//	m_SPhoto.MapWindowPoints(this, &rct);
	//	rct.MoveToX(cx - rct.Width() - margin);

	//	m_SPhoto.MoveWindow(rct);
	//}
}


void CViewRegister::OnBnClickedBtnSign()
{
	UpdateData(TRUE); //获取输入


	//m_strNumber = ""; //档案号
	CString name, type, tel, fee, id, home, birth, date;
	m_Ed_Name.GetWindowTextA(name);
	m_Ed_Tel.GetWindowTextA(tel);
	m_Ed_Fee.GetWindowTextA(fee);
	m_Ed_ID.GetWindowTextA(id);
	m_Ed_Home.GetWindowTextA(home);
	m_Comb_CarType.GetWindowTextA(type);
	m_Date_Sign.GetWindowTextA(date);
	m_Date_Birth.GetWindowTextA(birth);
	if (name.IsEmpty() || type.IsEmpty() || tel.IsEmpty()
		|| fee.IsEmpty() || id.IsEmpty() || home.IsEmpty()
		|| birth.IsEmpty() || date.IsEmpty())
	{
		MessageBox("必须每一项都填完再点击注册！");
		ShowMsg2Output1("没填完数据就点击了注册按钮");
	}
	else
	{
		CString strMsg("");
		CString strSQL("");
		strSQL.Format("INSERT INTO STUDENTS(ID, SNAME, BIRTHDAY, FILE_NUMBER, REGIST_DATE, CAR_TYPE, TEL, HOME, FEE) \
					  			VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
								id, name, birth, m_strNumber.Right(4), date, type, tel, home, fee);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		{
			MessageBox("注册成功！");
			ShowMsg2Output1(name+"注册成功！");
			GetDlgItem(IDC_BTN_SIGN)->EnableWindow(FALSE); //防止重复注册同一个信息
			GetDlgItem(IDC_NEWFILE)->EnableWindow(TRUE);

			//保存照片
			CString sFileName("");
			sFileName.Format("%s%s.bmp", g_FilePath, id);
			::SHCreateDirectory(NULL, CA2W(g_FilePath));
			cv::String s = sFileName.GetBuffer();
			imwrite(s, m_cap);
			sFileName.ReleaseBuffer();

		}
		else
		{
			MessageBox("注册错误！");
			ShowMsg2Output1("注册操作错误：" + strSQL);
			LOG("SQLERROR.log", strSQL);
			LOG("SQLERROR.log", g_mysqlCon.GetError());
		}
	}
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
	localtime = CTime::GetCurrentTime();

	//获取已有档案数量
	CDStrs data;
	CString strMsg("");
	CString strSQL("SELECT MAX(FILE_NUMBER) FROM students");
	if (g_mysqlCon.ExecuteQuery(strSQL, data, strMsg))
	{
		ShowMsg2Output1("查询档案数量数据成功！");
		int nfiles = 1;
		if (!data[0][0].IsEmpty())
		{
			nfiles = atoi(data[0][0]) + 1;
		}
		CString strTmp;
		strTmp = (localtime.Format("%Y%m")).Right(4);
		m_strNumber.Format("DJ%s%04d", strTmp, nfiles);
		m_Sta_Num.SetWindowTextA(m_strNumber);
		GetDlgItem(IDC_BTN_SIGN)->EnableWindow(TRUE);
	}
	else
	{
		ShowMsg2Output1(_T("查询档案数量数据操作失败!\r\n") + strMsg);
	}

	UpdateData(FALSE); //更新显示
}
