// ViewRegister.cpp : ʵ���ļ�
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


	m_strNumber = ""; //������
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


// CViewRegister ���

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


// CViewRegister ��Ϣ�������


void CViewRegister::OnBnClickedCamera()
{
	CString str;
	m_Btn_Cap.GetWindowTextA(str);
	if (str == "������ͷ")
	{
		//init the timer
		SetTimer(0, 50, NULL);
		m_Btn_Cap.SetWindowTextA("�ɼ���Ƭ");
	}
	else if (str == "�ɼ���Ƭ")
	{
		KillTimer(0);
		m_isCaptured = TRUE;
		ShowMsg2Output1("���ճɹ�");
		m_Btn_Cap.SetWindowTextA("������ͷ");
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
		m_cap = m_cap(Rect(150, 2, 340, 476)); //����5:7��һ����Ƭ������ȡ
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

	/******��ʼ��һ������ͷ��׽��******/

	if (!m_videoCap.isOpened()) m_videoCap.open(0);
	if (!m_videoCap.isOpened())
	{
		MessageBox("Failed to open camera");
	}

	m_Comb_CarType.AddString("C1�ֶ���");
	m_Comb_CarType.AddString("C2�Զ���");
	m_Comb_CarType.AddString("B2�����");
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

	//int margin = 200; //�ؼ���߾�
	//if (m_SPhoto) //���ƿؼ�λ��
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
	UpdateData(TRUE); //��ȡ����


	//m_strNumber = ""; //������
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
		MessageBox("����ÿһ������ٵ��ע�ᣡ");
		ShowMsg2Output1("û�������ݾ͵����ע�ᰴť");
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
			MessageBox("ע��ɹ���");
			ShowMsg2Output1(name+"ע��ɹ���");
			GetDlgItem(IDC_BTN_SIGN)->EnableWindow(FALSE); //��ֹ�ظ�ע��ͬһ����Ϣ
			GetDlgItem(IDC_NEWFILE)->EnableWindow(TRUE);

			//������Ƭ
			CString sFileName("");
			sFileName.Format("%s%s.bmp", g_FilePath, id);
			::SHCreateDirectory(NULL, CA2W(g_FilePath));
			cv::String s = sFileName.GetBuffer();
			imwrite(s, m_cap);
			sFileName.ReleaseBuffer();

		}
		else
		{
			MessageBox("ע�����");
			ShowMsg2Output1("ע���������" + strSQL);
			LOG("SQLERROR.log", strSQL);
			LOG("SQLERROR.log", g_mysqlCon.GetError());
		}
	}
}


void CViewRegister::OnBnClickedNewfile()
{
	//��������
	m_Ed_Name.SetWindowTextA("");
	m_Ed_ID.SetWindowTextA("");
	m_Ed_Tel.SetWindowTextA("");
	m_Ed_Home.SetWindowTextA("");
	m_Ed_Fee.SetWindowTextA("");
	m_Comb_CarType.SetCurSel(0);


	CTime localtime;
	localtime = CTime::GetCurrentTime();

	//��ȡ���е�������
	CDStrs data;
	CString strMsg("");
	CString strSQL("SELECT MAX(FILE_NUMBER) FROM students");
	if (g_mysqlCon.ExecuteQuery(strSQL, data, strMsg))
	{
		ShowMsg2Output1("��ѯ�����������ݳɹ���");
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
		ShowMsg2Output1(_T("��ѯ�����������ݲ���ʧ��!\r\n") + strMsg);
	}

	UpdateData(FALSE); //������ʾ
}
