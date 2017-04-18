// ViewStuProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewStuProgress.h"
#include "MainFrm.h"
#include <afxmsg_.h>


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
				}
				else //后边是进度
				{
					CString strStep = pThis->m_datas[nRow][5];
					int step = atoi(strStep);
					int rCol = (nCol - 4); //进度的列号从1开始 1=状态1(step=1~2)
					int tmp = rCol * 2 - 1;
					if (tmp < step)
					{
						pdi->item.clrBackground = RGB(149, 200, 146);
						pdi->item.varValue = "完成";
					}
					if (tmp == step)
					{
						pdi->item.clrBackground = RGB(195, 218, 195);
						pdi->item.varValue = "进行中";
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
	ON_BN_CLICKED(IDC_STUFRESH, &CViewStuProgress::OnBnClickedStufresh)
	ON_BN_CLICKED(IDC_SENDBOOKMSG, &CViewStuProgress::OnBnClickedSendbookmsg)
	ON_BN_CLICKED(IDC_TOBOOK, &CViewStuProgress::OnBnClickedTobook)
END_MESSAGE_MAP()


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
	strSQL.Format("SELECT FILE_NAME, SNAME, GENDER, TEL, CAR_TYPE, STEP FROM students WHERE STEP<'1000'");
	m_datas.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas, strMsg))
	{
		ShowMsg2Output1("查询学生信息成功");
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
	CString strMsg, strTmp;
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_wndGrid.IsRowSelected(i))
		{
			nSel++;
			int step = atoi(m_datas[i][5]);
			if (step < SP_K1PASS) //科目一未结束
			{
				strTmp.Format("学员（%s)尚未完成科目一课程，不能发送预约短信", m_datas[i][0]);
				MessageBox(strTmp);
				continue;
				ShowMsg2Output1("发预约短信，选择了尚未完成科目一的学生档案");
			}
			else if (step == SP_K2K3BOOKING)
			{
				strTmp.Format("学员（%s)已经发送过短信，是否仍然选择发送短信？", m_datas[i][0]);
				ShowMsg2Output1("发预约短信，选择了已发过短信的学生档案");
				if(MessageBox(strTmp, "", MB_YESNOCANCEL) != IDOK) continue;
			}
			else if (step > SP_K2K3BOOKING) //已经预约完成
			{
				strTmp.Format("学员（%s)已经完成预约，无需发送预约短信", m_datas[i][0]);
				MessageBox(strTmp);
				continue;
				ShowMsg2Output1("发预约短信，选择了已完成预约的学生档案");
			}

			/*未完成*/
			//发送预约短信  
			//int len = 14; //一次发送一个短信
			//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			//pFrame->m_isSendReady = FALSE;
			//pFrame->m_pSendBuf = new BYTE[len];//发送完删除
			//pFrame->m_nSendLen = len;
			//pFrame->m_pSendBuf[0] = 2; //发送短信平台数据
			//pFrame->m_pSendBuf[1] = 4; //培训预约短信
			//int n = 1;
			//memcpy(pFrame->m_pSendBuf + 2, &n, 4); //档案数量

			CString fileNum = m_datas[i][0];
			//CString strFileNum = fileNum.Right(8);
			//char* data = strFileNum.GetBuffer();
			//memcpy(pFrame->m_pSendBuf + 6 + 8 * i, data, 8);
			//strFileNum.ReleaseBuffer();
			//pFrame->m_isSendReady = TRUE;

			CString strSQL;
			strSQL.Format("UPDATE students SET STEP='7' WHERE FILE_NAME='%s'", fileNum);
			if (g_mysqlCon.ExecuteSQL(strSQL, strMsg))
			{
				ShowMsg2Output1("更新新生信息成功");
				m_datas[i][5].Format("%d", SP_K2K3BOOKING);
			}
			else ShowMsg2Output1(strMsg);
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
		int nRow = pRow->GetRowId();
		CString strFileName = pThis->m_datas[nRow][0];
		pThis->MessageBox(strFileName);
	}
}

void CViewStuProgress::OnBnClickedTobook()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SelectView(VIEW_BOOKING1);
}
