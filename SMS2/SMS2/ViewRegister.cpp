// ViewRegister.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewRegister.h"


using namespace cv;
// CViewRegister

IMPLEMENT_DYNCREATE(CViewRegister, CBCGPFormView)

CViewRegister::CViewRegister()
: CBCGPFormView(CViewRegister::IDD)
, m_isCaptured(FALSE)
, m_canCap(FALSE)
{
	EnableVisualManagerStyle();
}

CViewRegister::~CViewRegister()
{
}

void CViewRegister::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHOTO, m_SPhoto);
	DDX_Control(pDX, IDC_CAMERA, m_Btn_Cap);
}

BEGIN_MESSAGE_MAP(CViewRegister, CBCGPFormView)
	ON_BN_CLICKED(IDC_CAMERA, &CViewRegister::OnBnClickedCamera)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SIZE()
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
		imwrite("123.bmp", m_cap);
		MessageBox("���ճɹ�");
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

	int margin = 200; //�ؼ���߾�
	if (m_SPhoto) //���ƿؼ�λ��
	{
		CRect rct;
		m_SPhoto.GetClientRect(&rct);
		m_SPhoto.MapWindowPoints(this, &rct);
		rct.MoveToX(cx - rct.Width() - margin);

		m_SPhoto.MoveWindow(rct);
	}
}
