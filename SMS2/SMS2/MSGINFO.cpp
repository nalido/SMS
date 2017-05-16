// MSGINFO.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMS2.h"
#include "MSGINFO.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CMSGINFO �Ի���

IMPLEMENT_DYNAMIC(CMSGINFO, CBCGPDialog)

CMSGINFO::CMSGINFO(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CMSGINFO::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_nFlag = 0;
	m_strSMS = "";
}

CMSGINFO::~CMSGINFO()
{
}

void CMSGINFO::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_S1, m_S[6]);
	DDX_Control(pDX, IDC_S2, m_S[0]);
	DDX_Control(pDX, IDC_S3, m_S[1]);
	DDX_Control(pDX, IDC_S4, m_S[2]);
	DDX_Control(pDX, IDC_S5, m_S[3]);
	DDX_Control(pDX, IDC_S6, m_S[4]);
	DDX_Control(pDX, IDC_S7, m_S[5]);
	DDX_Control(pDX, IDC_DATE, m_wndDatePicker);
	DDX_Control(pDX, IDC_TIME, m_wndTimePicker);


	DDX_Control(pDX, IDC_E1, m_E[0]);
	DDX_Control(pDX, IDC_E2, m_E[1]);
	DDX_Control(pDX, IDC_E3, m_E[2]);
	DDX_Control(pDX, IDC_E4, m_E[3]);

	DDX_Control(pDX, IDC_SMSINFO, m_SMS);
	DDX_Control(pDX, IDC_COMBO1, m_Comb);

	DDX_Text(pDX, IDC_E1, m_strE[0]);
	DDX_Text(pDX, IDC_E2, m_strE[1]);
	DDX_Text(pDX, IDC_E3, m_strE[2]);
	DDX_Text(pDX, IDC_E4, m_strE[3]);

	DDX_Text(pDX, IDC_S2, m_strHints[0]);
	DDX_Text(pDX, IDC_S3, m_strHints[1]);
	DDX_Text(pDX, IDC_S4, m_strHints[2]);
	DDX_Text(pDX, IDC_S5, m_strHints[3]);
	DDX_Text(pDX, IDC_S6, m_strHints[4]);
	DDX_Text(pDX, IDC_S7, m_strHints[5]);

	DDX_Text(pDX, IDC_SMSINFO, m_strSMS);
}


BEGIN_MESSAGE_MAP(CMSGINFO, CBCGPDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_DATE, &CMSGINFO::OnBnClickedDate)
	ON_BN_CLICKED(IDC_TIME, &CMSGINFO::OnBnClickedTime)
	ON_EN_CHANGE(IDC_E1, &CMSGINFO::OnEnChangeE1)
	ON_EN_CHANGE(IDC_E2, &CMSGINFO::OnEnChangeE2)
	ON_EN_CHANGE(IDC_E3, &CMSGINFO::OnEnChangeE3)
	ON_EN_CHANGE(IDC_E4, &CMSGINFO::OnEnChangeE4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMSGINFO::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CMSGINFO ��Ϣ�������


BOOL CMSGINFO::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	UINT stateFlags = 0;
	stateFlags |= (CBCGPDateTimeCtrl::DTM_SPIN | CBCGPDateTimeCtrl::DTM_TIME);
	const UINT stateMask =
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR |
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME |
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE |
		CBCGPDateTimeCtrl::DTM_SECONDS;
	m_wndTimePicker.SetState(stateFlags, stateMask);
	m_wndTimePicker.SizeToContent();

	stateFlags = 0;
	stateFlags |= (
		CBCGPDateTimeCtrl::DTM_SPIN | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR);
	m_wndDatePicker.SetState(stateFlags, stateMask);
	m_wndDatePicker.SizeToContent();
	m_wndDatePicker.m_monthFormat = 2;

	CString strKeyName("");
	switch (m_nFlag)
	{
	case 1: //�������
	{
				m_strHints[0] = "ѧԱ����";
				m_strHints[1] = "______";
				m_strHints[2] = "______";
				m_strHints[3] = "______";
				m_strHints[4] = "______";
				m_strHints[5] = "�Ͽ�ʱ��";

				m_S[1].EnableWindow(FALSE);
				m_S[2].EnableWindow(FALSE);
				m_S[3].EnableWindow(FALSE);
				m_S[4].EnableWindow(FALSE);
				m_E[1].EnableWindow(FALSE);
				m_E[2].EnableWindow(FALSE);
				m_E[3].EnableWindow(FALSE);

				m_arrComb.push_back("��Ŀһ");
				m_arrComb.push_back("��Ŀ��");
				m_arrComb.push_back("��Ŀ��");
				m_Comb.AddString("��Ŀһ");
				m_Comb.AddString("��Ŀ��");
				m_Comb.AddString("��Ŀ��");
				m_Comb.EnableWindow(FALSE);


				strKeyName = "K1PrePass";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");

				strKeyName = "K1Address";
				CString str = xPublic::GETSTR2("SMS", strKeyName, "failed");
				m_strSMSTmp.Replace("A", str);

				m_strSMSTmp.Replace("N", m_strStu);

				m_strE[0] = m_strClassIssue;
				m_strSMS = m_strSMSTmp;
	}
		break;
	case 2: //�˿����
	{
				m_strHints[0] = "______";
				m_strHints[1] = "______";
				m_strHints[2] = "______";
				m_strHints[3] = "______";
				m_strHints[4] = "______";
				m_strHints[5] = "�˿�ʱ��";

				m_S[0].EnableWindow(FALSE);
				m_S[1].EnableWindow(FALSE);
				m_S[2].EnableWindow(FALSE);
				m_S[3].EnableWindow(FALSE);
				m_S[4].EnableWindow(FALSE);
				m_S[5].EnableWindow(TRUE);

				m_E[0].EnableWindow(FALSE);
				m_E[1].EnableWindow(FALSE);
				m_E[2].EnableWindow(FALSE);
				m_E[3].EnableWindow(FALSE);

				m_Comb.EnableWindow(FALSE);


				strKeyName = "K1NOPass";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");

				m_strSMSTmp.Replace("N", m_strStu);

				m_strSMS = m_strSMSTmp;
	}
		break;
	case 3: //��һ���Զ���
	{
				m_strHints[0] = "______";
				m_strHints[1] = "______";
				m_strHints[2] = "______";
				m_strHints[3] = "______";
				m_strHints[4] = "______";
				m_strHints[5] = "����ʱ��";

				m_S[0].EnableWindow(FALSE);
				m_S[1].EnableWindow(FALSE);
				m_S[2].EnableWindow(FALSE);
				m_S[3].EnableWindow(FALSE);
				m_S[4].EnableWindow(FALSE);
				m_S[5].EnableWindow(TRUE);

				m_E[0].EnableWindow(FALSE);
				m_E[1].EnableWindow(FALSE);
				m_E[2].EnableWindow(FALSE);
				m_E[3].EnableWindow(FALSE);

				m_Comb.EnableWindow(FALSE);

				strKeyName = "K1Exame";
				m_strSMSTmp = xPublic::GETSTR2("SMS", strKeyName, "failed");
				m_strSMSTmp.Replace("N", m_strStu);
				m_strSMS = m_strSMSTmp;
	}
		break;
	case 4: //��ѵԤԼ����
		strKeyName = "K1PrePass";
		break;
	case 5: //�Ͽ����Ѷ���
		strKeyName = "K1PrePass";
		break;
	}

	//char* strSMS = EncodeToUTF8(m_strSMS);

	UpdateData(FALSE); //��ʾԤ����Ϣ
	UpdateSMS();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CMSGINFO::Init(int flag)
{
	m_nFlag = flag;
}

HBRUSH CMSGINFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S3:
	case IDC_S4:
	case IDC_S5:
	case IDC_S6:
	case IDC_S7:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //����һ���ջ�ˢ
				   break;
	}
	case IDC_SMSINFO:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(25, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "΢���ź�");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //����һ���ջ�ˢ
				   break;
	}
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CMSGINFO::OnBnClickedDate()
{
	UpdateSMS();
}


void CMSGINFO::OnBnClickedTime()
{
	UpdateSMS();
}

void CMSGINFO::UpdateSMS()
{
	UpdateData(); //�������
	m_strSMS = m_strSMSTmp;

	COleDateTime dt = m_wndTimePicker.GetDate();

	CString AP = dt.Format("%p");
	if (AP == "AM")
	{
		AP = "����";
	}
	else
		AP = "����";
	CString strTime = dt.Format("%I��%M��");
	m_strTime = AP + strTime;

	COleDateTime dd = m_wndDatePicker.GetDate();
	m_strDate = dd.Format("%Y��%m��%d��");

	switch (m_nFlag)
	{
	case 1: //�������
	{
			  CString str = m_strDate + m_strTime;
			  m_strSMS.Replace("T", str);
			  m_strSMS.Replace("Q", m_strE[0]);
			  m_strClassIssue = m_strE[0];
	}
		break;
	case 2: //�˿����
	{
				m_strSMS.Replace("T", m_strDate);
	}
		break;
	case 3: //��һ���Զ���
	{
				CString str = m_strDate + m_strTime;
				m_strSMS.Replace("T", str);
	}
		break;
	case 4: //��ѵԤԼ����
		
		break;
	case 5: //�Ͽ����Ѷ���
		
		break;
	}

	//ˢ��
	CRect rect;
	m_SMS.GetClientRect(&rect);
	m_SMS.MapWindowPoints(this, &rect);

	UpdateData(FALSE);
	InvalidateRect(&rect);
}



void CMSGINFO::OnEnChangeE1()
{
	UpdateData();

	UpdateSMS();
}


void CMSGINFO::OnEnChangeE2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CBCGPDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMSGINFO::OnEnChangeE3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CBCGPDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMSGINFO::OnEnChangeE4()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CBCGPDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMSGINFO::OnCbnSelchangeCombo1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
