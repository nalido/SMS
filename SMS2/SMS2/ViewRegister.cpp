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
}

BEGIN_MESSAGE_MAP(CViewRegister, CBCGPFormView)
	ON_BN_CLICKED(IDC_CAMERA, &CViewRegister::OnBnClickedCamera)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SIGN, &CViewRegister::OnBnClickedBtnSign)
	ON_BN_CLICKED(IDC_NEWFILE, &CViewRegister::OnBnClickedNewfile)
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
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
	GetDlgItem(IDC_CAMERA)->GetWindowTextA(str);
	if (str == "������ͷ" || str == "���²ɼ���Ƭ")
	{

		if (!m_videoCap.isOpened()) m_videoCap.open(0);
		if (!m_videoCap.isOpened())
		{
			MessageBox("Failed to open camera");
		}

		//init the timer
		SetTimer(0, 50, NULL);
		GetDlgItem(IDC_CAMERA)->SetWindowTextA("�ɼ���Ƭ");
	}
	else if (str == "�ɼ���Ƭ")
	{
		KillTimer(0);
		m_isCaptured = TRUE;
		ShowMsg2Output1("���ճɹ�");
		GetDlgItem(IDC_CAMERA)->SetWindowTextA("���²ɼ���Ƭ");
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
		m_cap = cap(Rect(150, 2, 340, 476)).clone(); //����5:7��һ����Ƭ������ȡ
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

	m_Comb_Gender.AddString("��");
	m_Comb_Gender.AddString("Ů");

	m_Date_Sign.SetFormat("yyyy/MM/dd");
	m_Date_Birth.SetFormat("yyyy/MM/dd");

	GetDlgItem(IDC_BTN_SIGN)->EnableWindow(FALSE); //�������е�����
	GetDlgItem(IDC_NEWFILE)->EnableWindow(TRUE);
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

	if (!m_isCaptured){
		MessageBox("�������ͷ������Ƭ�ɼ���");
		return;
	}

	//m_strNumber = ""; //������
	CString name, type, tel, fee, id, home, birth, date, gender;
	m_Ed_Name.GetWindowTextA(name);
	m_Ed_Tel.GetWindowTextA(tel);
	m_Ed_Fee.GetWindowTextA(fee);
	m_Ed_ID.GetWindowTextA(id);
	m_Ed_Home.GetWindowTextA(home);
	m_Comb_CarType.GetWindowTextA(type);
	m_Comb_Gender.GetWindowTextA(gender);
	m_Date_Sign.GetWindowTextA(date); 
	m_Date_Birth.GetWindowTextA(birth);
	if (name.IsEmpty() || type.IsEmpty() || tel.IsEmpty()
		|| fee.IsEmpty() || id.IsEmpty() || home.IsEmpty()
		|| birth.IsEmpty() || date.IsEmpty() || gender.IsEmpty())
	{
		MessageBox("����ÿһ������ٵ��ע�ᣡ");
		ShowMsg2Output1("û�������ݾ͵����ע�ᰴť");
	}
	else
	{
		CString strMsg("");
		CString strSQL("");
		strSQL.Format("INSERT INTO STUDENTS \
			(ID, SNAME, BIRTHDAY, FILE_NUMBER, FILE_NAME, REGIST_DATE, CAR_TYPE, TEL, HOME, FEE, GENDER, STEP) \
					  			VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '0000')",
								id, name, birth, m_strNumber.Right(8), m_strNumber, date, type, tel, home, fee, gender);
		if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
		{
			MessageBox("ע��ɹ���");
			ShowMsg2Output1(name + "ע��ɹ���");
			GetDlgItem(IDC_BTN_SIGN)->EnableWindow(FALSE); //��ֹ�ظ�ע��ͬһ����Ϣ
			GetDlgItem(IDC_NEWFILE)->EnableWindow(TRUE);

			//������Ƭ
			CString sFileName("");
			sFileName.Format("%s\\", g_strFilePath);
			::SHCreateDirectory(NULL, CA2W(sFileName));
			sFileName.Format("%s\\%s.bmp", g_strFilePath, m_strNumber);
			cv::String s = sFileName.GetBuffer();
			imwrite(s, m_cap);
			sFileName.ReleaseBuffer();

			//���ݴ��
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			IplImage ipl_img = m_cap;
			int len = ipl_img.imageSize + 23;
			if (pFrame->m_pSendBuf != NULL)
			{ 
				MessageBox("��һ����Ϣ��δ������ϣ����Ե����ԡ�");
			}
			else
			{
				pFrame->m_isSendReady = FALSE;
				pFrame->m_pSendBuf = new BYTE[len];//������ɾ��
				pFrame->m_nSendLen = len;
				pFrame->m_pSendBuf[0] = 1; //����ͼ������
				char* cID = m_strNumber.GetBuffer();
				memcpy(pFrame->m_pSendBuf + 1, cID, 10); //������
				m_strNumber.ReleaseBuffer();
				memcpy(pFrame->m_pSendBuf + 11, &m_cap.cols, 4); //ͼ����
				memcpy(pFrame->m_pSendBuf + 15, &m_cap.rows, 4); //ͼ��߶�
				memcpy(pFrame->m_pSendBuf + 19, &ipl_img.imageSize, 4); //ͼ��ߴ�
				memcpy(pFrame->m_pSendBuf + 23, ipl_img.imageData, ipl_img.imageSize); //ͼ������

				pFrame->m_isSendReady = TRUE;
			}
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

LRESULT CViewRegister::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
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

	CString strMonth;
	strMonth = (localtime.Format("%Y%m")).Right(4);
	CString strSQL("");
	strSQL.Format("SELECT MAX(FILE_NUMBER) FROM (SELECT FILE_NUMBER FROM students WHERE FILE_NUMBER LIKE '%s%%' ) as file_num", strMonth);
	if (g_mysqlCon.ExecuteQuery(strSQL, data, strMsg))
	{
		ShowMsg2Output1("��ѯ�����������ݳɹ���");
		int nfiles = 1;
		if (!data[0][0].IsEmpty())
		{
			if (atoi(data[0][0]) == 0) //������¼��ĵ�һ��
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
		m_Sta_Num.SetWindowTextA(m_strNumber);
		GetDlgItem(IDC_BTN_SIGN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CAMERA)->SetWindowTextA("������ͷ");
		OnBnClickedCamera();
	}
	else
	{
		ShowMsg2Output1(_T("��ѯ�����������ݲ���ʧ��!\r\n") + strMsg);
	}

	UpdateData(FALSE); //������ʾ
}
