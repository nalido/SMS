// ViewStuProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewStuProgress.h"
#include "ViewBooking1.h"
#include "MainFrm.h"
#include <afxmsg_.h>
#include "MSGINFO.h"
#include "DlgDateItem.h"


static BOOL CALLBACK GridCallback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewStuProgress* pThis = (CViewStuProgress*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
			std::vector<CStrs>::iterator it = pThis->m_datas.begin() + nRow;
			if (!it->empty())
			{
				if (nCol < 5)
				{
					pdi->item.varValue = pThis->m_datas[nRow][nCol];

					//判断明天是不是预约时间
					CTime t = GetServerTime();//CTime::GetCurrentTime();
					CTime tomo = t + CTimeSpan(1, 0, 0, 0);
					CString strTomo = tomo.Format("%Y/%m/%d");
					if (strTomo == pThis->m_datas[nRow][9] && pThis->m_datas[nRow][10] == "0")
						pdi->item.clrBackground = COLOR_LITTLE;
				}
				else if(nCol < 9) //后边是进度
				{
					CString strStep = pThis->m_datas[nRow][5];
					int step = atoi(strStep);
					int rCol = (nCol - 4); //进度的列号从1开始 1=状态1(step=1~2)
					int tmp = rCol * 2 - 1;
					if (tmp < step)
					{
						pdi->item.clrBackground = COLOR_COMPLETE;
						pdi->item.varValue = "完成";
					}
					if (tmp == step)
					{
						pdi->item.clrBackground = COLOR_DOING;
						pdi->item.varValue = "进行中";
					}
				}
				else // 9列开始是培训类型的进度
				{
					CString strClassType = pThis->m_datas[nRow][6];
					int stat = atoi(pThis->m_datas[nRow][(nCol+5) / 2]);
					if (nCol == 9)
					{
						if (stat < 3 && strClassType == "科目二")
						{
							pdi->item.clrBackground = COLOR_DOING;
							pdi->item.varValue = "进行中";
						}
						if (stat == 3)
						{
							pdi->item.clrBackground = COLOR_DONE;
							pdi->item.varValue = "完成";
						}
					}
					if (nCol == 10)
					{
						if (stat > 0 && stat < 3)
						{
							pdi->item.clrBackground = COLOR_DOING;
							pdi->item.varValue = "进行中";
						}
						if (stat == 3)
						{
							pdi->item.clrBackground = COLOR_DONE;
							pdi->item.varValue = "完成";
						}
					}

					if (nCol == 11)
					{
						if (stat < 3 && strClassType == "科目三")
						{
							pdi->item.clrBackground = COLOR_DOING;
							pdi->item.varValue = "进行中";
						}
						if (stat == 3)
						{
							pdi->item.clrBackground = COLOR_DONE;
							pdi->item.varValue = "完成";
						}
					}
					if (nCol == 12)
					{
						if (stat > 0 && stat < 3)
						{
							pdi->item.clrBackground = COLOR_DOING;
							pdi->item.varValue = "进行中";
						}
						if (stat == 3)
						{
							pdi->item.clrBackground = COLOR_DONE;
							pdi->item.varValue = "完成";
						}
					}
				}
			}
			else
			{
				pdi->item.varValue = "访问内存出错";
			}
	}

	return TRUE;
}

// CViewStuProgress

IMPLEMENT_DYNCREATE(CViewStuProgress, CBCGPFormView)


CViewStuProgress::CViewStuProgress()
	: CBCGPFormView(CViewStuProgress::IDD)
{
	EnableVisualManagerStyle();
}

CViewStuProgress::~CViewStuProgress()
{
}

void CViewStuProgress::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STUPROGRESS, m_wndGridLocation);
}

BEGIN_MESSAGE_MAP(CViewStuProgress, CBCGPFormView)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_STUFRESH, &CViewStuProgress::OnBnClickedStufresh)
	ON_BN_CLICKED(IDC_SENDBOOKMSG, &CViewStuProgress::OnBnClickedSendbookmsg)
	ON_BN_CLICKED(IDC_TOBOOK, &CViewStuProgress::OnBnClickedTobook)
	ON_BN_CLICKED(IDC_SCAN, &CViewStuProgress::OnBnClickedScan)
	ON_BN_CLICKED(IDC_SETTYPE, &CViewStuProgress::OnBnClickedSettype)
	ON_BN_CLICKED(IDC_NEXTBOOK, &CViewStuProgress::OnBnClickedNextbook)
END_MESSAGE_MAP()


LRESULT CViewStuProgress::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		Refresh();
		break;
	}

	return 0;
}

// CViewStuProgress 诊断

#ifdef _DEBUG
void CViewStuProgress::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewStuProgress::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewStuProgress 消息处理程序

void CViewStuProgress::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


	//初始化表格
	CRect rectGrid;
	m_wndGridLocation.GetClientRect(&rectGrid);
	m_wndGridLocation.MapWindowPoints(this, &rectGrid); //转为桌面坐标
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid.SetReadOnly();
	m_wndGrid.SetWholeRowSel();
	m_wndGrid.EnableRowHeader(TRUE);
	m_wndGrid.EnableLineNumbers();


	int nColumn = 0;
	m_wndGrid.InsertColumn(nColumn++, _T("档案号"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("姓名"), 70);
	m_wndGrid.InsertColumn(nColumn++, _T("性别"), 50);
	m_wndGrid.InsertColumn(nColumn++, _T("手机号"), 80);
	m_wndGrid.InsertColumn(nColumn++, _T("申领类别"), 80);
	int wid = (rectGrid.Width() - 350 - 20) / 8;
	m_wndGrid.InsertColumn(nColumn++, _T("政治审核"), wid); //col = 5; step = 1~2
	m_wndGrid.InsertColumn(nColumn++, _T("科一报考"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("科一考试"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("培训预约"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("科二培训"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("科二考试"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("科三培训"), wid);
	m_wndGrid.InsertColumn(nColumn++, _T("科三考试"), wid);
	for (int i = 0; i < nColumn; i++)
	{
		m_wndGrid.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid.EnableVirtualMode(GridCallback, (LPARAM)this);
	m_wndGrid.SetCallBack_DblClk(OnGridDbClick);

	Refresh();
}

void CViewStuProgress::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT students.FILE_NAME, students.SNAME, students.GENDER, students.TEL, \
					students.CAR_TYPE, students.STEP, students.CLASS_TYPE, \
					stuDates.K2_STAT, stuDates.K3_STAT, stuDates.BOOK_DATE, stuDates.BOOK_SMS FROM students \
					left join stuDates ON students.FILE_NAME=stuDates.STU_ID\
					WHERE STEP<'1000'");
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("查询学生信息成功");

		int n = m_datas.size();
		for (int i = 0; i < n; i++)
		{
			m_datas[i][9].Replace("年", "/");
			m_datas[i][9].Replace("月", "/");
			m_datas[i][9].Replace("日", "");
		}
	}
	else ShowMsg2Output1(strMsg);

	ListFresh();
}

void CViewStuProgress::OnBnClickedStufresh()
{
	Refresh();
}

void CViewStuProgress::ListFresh()
{
	m_wndGrid.RemoveAll();
	m_wndGrid.SetVirtualRows(m_datas.size());
	m_wndGrid.AdjustLayout();
}

void CViewStuProgress::OnBnClickedSendbookmsg()
{
	int nCount = m_datas.size();
	int nSel = 0;
	CString strMsg, strTmp, strSQL;
	CMSGINFO dlgMsg;
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_wndGrid.IsRowSelected(i))
		{
			nSel++;
			int step = atoi(m_datas[i][5]);
			int nSMSFlag = 4; //默认为第一次发送预约短信
			if (step < SP_K1PASS) //科目一未结束
			{
				strTmp.Format("学员（%s)尚未完成科目一课程，不能发送预约短信", m_datas[i][0]);
				MessageBox(strTmp);
				continue;
				ShowMsg2Output1("发预约短信，选择了尚未完成科目一的学生档案");
			}
			else if (step == SP_K2K3BOOKING)
			{
				strSQL.Format("SELECT BOOK_DATE FROM stuDates WHERE STU_ID='%s'", m_datas[i][0]);
				CDStrs dates;
				g_mysqlCon.ExecuteQuery(strSQL, dates, strMsg);
				if (dates.size() > 0)
				{
					dlgMsg.m_strDate = dates[0][0];
					nSMSFlag = 45;
				}
				else
				{
					nSMSFlag = 44;
				}
				//strTmp.Format("学员（%s)已经发送过短信，是否仍然选择发送短信？", m_datas[i][0]);
				//ShowMsg2Output1("发预约短信，选择了已发过短信的学生档案");
				//if(MessageBox(strTmp, "", MB_YESNOCANCEL) != IDYES) continue;
			}
			else if (step > SP_K2K3BOOKING) //已经预约完成
			{
				strTmp.Format("学员（%s)已经完成预约，无需发送预约短信", m_datas[i][0]);
				MessageBox(strTmp);
				continue;
				ShowMsg2Output1("发预约短信，选择了已完成预约的学生档案");
			}

			
			//发送预约短信 
			dlgMsg.m_nFlag = nSMSFlag;
			dlgMsg.m_strStu = m_datas[i][1];
			if (dlgMsg.DoModal() != IDOK) continue;
			CString strSMS0 = dlgMsg.m_strSMS;

			//数据打包发送
			CString strStuID = m_datas[i][0];
			CString strTel = m_datas[i][3];
			CString strSMS; 
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			strSMS.Format("%s:%s>%s", strTel, strStuID, strSMS0);
			int SMSlen = strlen(strSMS);
			int len = 6 + SMSlen;
			pFrame->m_isSendReady = FALSE;
			pFrame->m_pSendBuf = new BYTE[len];//发送完删除
			pFrame->m_nSendLen = len;
			pFrame->m_pSendBuf[0] = 2; //发送短信平台数据
			pFrame->m_pSendBuf[1] = dlgMsg.m_nFlag; //短信类型
			memcpy(pFrame->m_pSendBuf + 2, &SMSlen, 4); //档案数量

			char* data = strSMS.GetBuffer();
			memcpy(pFrame->m_pSendBuf + 6, data, SMSlen);
			strSMS.ReleaseBuffer();
			pFrame->m_isSendReady = TRUE;


			strSQL.Format("UPDATE students SET STEP='7' WHERE FILE_NAME='%s'", strStuID);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("更新新生信息成功");
				m_datas[i][5].Format("%d", SP_K2K3BOOKING);
			}
			else ShowMsg2Output1(strMsg);

			CTime t = GetServerTime();//CTime::GetCurrentTime();
			CTime tomo = t + CTimeSpan(1, 0, 0, 0);
			CString strTomo = tomo.Format("%Y/%m/%d");
			strSQL.Format("UPDATE stuDates SET BOOK_SMS='1' WHERE STU_ID='%s' AND BOOK_DATE='%s'", strStuID, strTomo);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
			m_datas[i][10] = "1";

			WaitForSingleObject(pFrame->m_hSocketEvent, 2000); //等待信息发送
		}
	}
	ListFresh();
	strMsg.Format("共选中%d行", nSel);
	ShowMsg2Output1(strMsg);
}

void CALLBACK CViewStuProgress::OnGridDbClick(LPVOID lParam)
{
	CViewStuProgress* pThis = (CViewStuProgress*)lParam;
	
	CBCGPGridRow* pRow = pThis->m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		pThis->OnBnClickedTobook();
	}
}

void CViewStuProgress::OnBnClickedTobook()
{


	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	//pFrame->GetView(VIEW_BOOKING1); //若预约视图未创建，则创建新视图

	CBCGPGridRow* pRow = m_wndGrid.GetCurSel(); 
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strStep = m_datas[nRow][5];
		int step = atoi(strStep);
		if (step < 6)
		{
			MessageBox("该学员不能进行预约操作！");
			return;
		}
		CString strType = m_datas[nRow][6];
		if (strType == "0")
		{
			MessageBox("该学员还没选定培训模式，请前往选择！");
			return;
		}

		CString strFileName = m_datas[nRow][0];
		STUDENTINFO stuInfo(m_datas[nRow][1], m_datas[nRow][0], m_datas[nRow][2], m_datas[nRow][4]);

		pFrame->SelectView(VIEW_BOOKING1);
		CViewBooking1* pView = (CViewBooking1*)pFrame->GetActiveView();
		pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)VIEW_STUPROGRESS, (LPARAM)3);
		pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)&stuInfo, (LPARAM)1);
	}

}


void CViewStuProgress::OnBnClickedScan()
{
	CMainFrame* pFrame = (CMainFrame*)::AfxGetMainWnd();


	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strFileName = m_datas[nRow][0];
		STUDENTINFO stuInfo(m_datas[nRow][1], m_datas[nRow][0], m_datas[nRow][2], m_datas[nRow][4]);

		pFrame->SelectView(VIEW_SCAN);
		CView* pView = (CView*)pFrame->GetActiveView();
		pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)VIEW_STUPROGRESS, (LPARAM)3);
		pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)&stuInfo, (LPARAM)1);
	}
}


void CViewStuProgress::OnBnClickedSettype()
{
	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strType = m_datas[nRow][6];
		CString strID = m_datas[nRow][0];

		if (strType != "0")
		{
			MessageBox("已设置！");
			return;
		}

		CString str = "选择“是”为先报考科目二\r\n选择“否”为先报考科目三";
		int n = MessageBoxA(str, "培训模式选择", MB_YESNOCANCEL);
		if (n == IDYES)
		{
			strType = "科目二";
		}
		else if (n == IDNO)
		{
			strType = "科目三";
		}
		else return;


		CString strMsg, strSQL;

		strSQL.Format("UPDATE students SET CLASS_TYPE='%s', STEP='7' WHERE FILE_NAME='%s'", strType, strID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		Refresh();
	}
}


void CViewStuProgress::OnBnClickedNextbook()
{
	CBCGPGridRow* pRow = m_wndGrid.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();
		CString strStuID = m_datas[nRow][0];

		CDlgDateItem dlg;
		if (dlg.DoModal() == IDOK)
		{
			CString strMsg, strSQL;
			strSQL.Format("UPDATE stuDates SET BOOK_DATE='%s' WHERE STU_ID='%s'", dlg.m_strDate, strStuID);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				MessageBox("设置成功!");
			}
		}
	}
}
