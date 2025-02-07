// Orders.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "Orders.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "OrderDetail.h"
#include "MSGINFO.h"

// COrders 对话框

IMPLEMENT_DYNAMIC(COrders, CBCGPDialog)

COrders::COrders(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(COrders::IDD, pParent)
{
	EnableVisualManagerStyle();

	m_strToday = "";
}

COrders::~COrders()
{
}

void COrders::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation1);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_Date_Start);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_Date_End); 
	DDX_Check(pDX, IDC_CHECK1, m_enableMultiSel);
}


BEGIN_MESSAGE_MAP(COrders, CBCGPDialog)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_QUERY, &COrders::OnBnClickedQuery)
	ON_BN_CLICKED(IDC_RESET, &COrders::OnBnClickedReset)
	ON_BN_CLICKED(IDC_PRINT, &COrders::OnBnClickedPrint)
	ON_BN_CLICKED(IDC_CHECK1, &COrders::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_PREPRINT, &COrders::OnBnClickedPreprint)
END_MESSAGE_MAP()


// COrders 消息处理程序

static BOOL CALLBACK Grid1Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	COrders* pThis = (COrders*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas1.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas1.begin() + nRow;
		if (!it->empty())
		{
			if (nCol == 2) //课程时间
			{
				int nClass = atoi(pThis->m_datas1[nRow][nCol]) - 1;
				pdi->item.varValue = GetClassTime(nClass);
			}
			else if (nCol == 6) //预约完成情况
			{
				if (pThis->m_datas1[nRow][nCol] == "1")
					pdi->item.varValue = "已派工，等待训练";
				else if (pThis->m_datas1[nRow][nCol] == "2")
					pdi->item.varValue = "已完成训练";
				else if (pThis->m_datas1[nRow][nCol] == "-1")
					pdi->item.varValue = "学员缺勤";
				else if (pThis->m_datas1[nRow][nCol] == "-2")
					pdi->item.varValue = "教练缺勤";
			}
			else
				pdi->item.varValue = pThis->m_datas1[nRow][nCol];

			//颜色控制
			CString strBookDate = pThis->m_datas1[nRow][1];
			if (strBookDate < pThis->m_strToday)
			{
				if (pThis->m_datas1[nRow][6] < "2") //未完成训练
				{
					pdi->item.clrBackground = COLOR_LITTLE;
					if (nCol == 6) pdi->item.varValue = "未完成训练";
				}
				else if (pThis->m_datas1[nRow][6] >= "2") //已完成训练
				{
					pdi->item.clrBackground = COLOR_MANY;
				}
			}
			else
			{
				if (pThis->m_dataPrint[nRow] == 1)
				{
					pdi->item.clrText = COLOR_MANY;
				}

				if (pThis->m_index[0] == nRow
					|| pThis->m_index[1] == nRow
					|| pThis->m_index[2] == nRow)
				{
					pdi->item.clrText = COLOR_LITTLE;
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


BOOL COrders::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	UpdateData(TRUE);

	m_index[0] = -1;
	m_index[1] = -1;
	m_index[2] = -1;

	//grid
	CRect rectGrid;
	m_wndGridLocation1.GetClientRect(&rectGrid);
	m_wndGridLocation1.MapWindowPoints(this, &rectGrid); //转为桌面坐标
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid1.Create(nStyle, rectGrid, this, IDC_GRID_STUPRO);
	m_wndGrid1.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid1.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid1.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid1.SetReadOnly();
	m_wndGrid1.SetWholeRowSel();
	//m_wndGrid1.SetSingleSel(); //只能选一个
	m_wndGrid1.EnableRowHeader(TRUE);
	m_wndGrid1.EnableLineNumbers();


	LPCTSTR arrColumns[] = { _T("派工日期"), _T("训练日期"), _T("训练时间"), _T("车辆编号"), _T("教练"), _T("学生"), _T("完成情况") };
	int hw = m_wndGrid1.GetRowHeaderWidth() * 2 / 3; //行头宽度
	int w = (rectGrid.Width()-hw) / 7;
	for (int nColumn = 0; nColumn < 7; nColumn++)
	{
		m_wndGrid1.InsertColumn(nColumn, arrColumns[nColumn], w);
		m_wndGrid1.SetColumnAlign(nColumn, HDF_CENTER);
		m_wndGrid1.SetHeaderAlign(nColumn, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid1.EnableVirtualMode(Grid1Callback, (LPARAM)this);
	m_wndGrid1.SetCallBack_Clk(OnGrid1Click);



	m_Date_Start.SetFormat("yyyy/MM/dd");
	m_Date_End.SetFormat("yyyy/MM/dd");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void COrders::OnBnClickedQuery()
{
	UpdateData();

	CString dateS, dateE;
	m_Date_Start.GetWindowText(dateS);
	m_Date_End.GetWindowText(dateE);

	CString strMsg, strSQL;
	strSQL.Format("SELECT bookings.ORDER_DATE, bookings.BOOK_DATE, bookings.CLASS_ID, bookings.ORDER_CAR, coachinfo.SName\
				  	, students.SNAME, bookings.FLAG, bookings.FILE_NAME, \
					bookings.ORDER_COACH, bookings.CLASS_NUM, bookings.CLASS_TYPE, bookings.CLASS_INDEX, students.TEL \
					FROM bookings \
					inner join students ON bookings.FILE_NAME = students.FILE_NAME \
					inner join coachinfo ON bookings.ORDER_COACH = coachinfo.FILE_NUM\
					WHERE bookings.ORDER_DATE>='%s' AND bookings.ORDER_DATE<='%s' AND students.STEP<'1000' \
					ORDER BY bookings.ORDER_DATE, bookings.BOOK_DATE, bookings.CLASS_ID, bookings.ORDER_CAR",
					dateS, dateE);
	m_datas1.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg);
	ShowMsg2Output1(strMsg);

	int size = m_datas1.size();

	m_dataPrint.clear();
	for (int i = 0; i < size; i++)
	{
		m_dataPrint.push_back(0);
	}

	m_wndGrid1.GridRefresh(size);
}


void COrders::OnBnClickedReset()
{
	if (m_enableMultiSel)
	{
		//CBCGPGridRow* pRow = m_wndGrid1.GetCurSel();
		//if (pRow != NULL)
		int rows = m_datas1.size() - 1;
		for (int nRow = rows; nRow >= 0; nRow--)
		{
			//int nRow = pRow->GetRowId();
			if (!m_wndGrid1.IsRowSelected(nRow)) continue;

			ResetStudent(nRow);
		}
	}
	else
	{
		int nRow = m_index[0];
		int nRow1 = m_index[1];
		int nRow2 = m_index[2];
		if (nRow >= 0)
		{
			ResetStudent(nRow);
		}
		if (nRow1 >= 0)
		{
			ResetStudent(nRow1);
		}
		if (nRow2 >= 0)
		{
			ResetStudent(nRow2);
		}

		m_index[0] = -1;
		m_index[1] = -1;
		m_index[2] = -1;
	}
	m_wndGrid1.GridRefresh(m_datas1.size());
}

void COrders::ResetStudent(int nRow)
{
	CString strBookDate = m_datas1[nRow][1];
	if (strBookDate < m_strToday)
	{
		MessageBox("今天之前的派工无法取消，请手动添加临时派工");
		return;
	}

	CString strStudent = m_datas1[nRow][7];
	CString strClassID = m_datas1[nRow][2];

	CString strMsg, strSQL;
	strSQL.Format("UPDATE bookings SET FLAG='0', ORDER_DATE='0', ORDER_COACH='0', ORDER_CAR='0' \
				  					    WHERE FILE_NAME='%s' AND BOOK_DATE='%s' AND CLASS_ID='%s'",
										strStudent, strBookDate, strClassID);
	g_mysqlCon.ExecuteSQL(strSQL, strMsg);
	ShowMsg2Output1(strMsg);

	CDStrs::iterator it = m_datas1.begin() + nRow;
	m_datas1.erase(it);
}

void COrders::OnBnClickedPrint()
{
	int nRow = m_index[0];
	if (nRow >= 0)
	{
		m_dataPrint[nRow] = 1;
		int row2 = m_index[1], row3 = m_index[2];
		if (row2 >= 0) m_dataPrint[row2] = 1;
		if (row3 >= 0) m_dataPrint[row3] = 1;

		m_index[0] = -1;
		m_index[1] = -1;
		m_index[2] = -1;

		m_printx.PrinterInit(&m_sheetInfo, &m_classInfo);
		CString strMsg; 
		m_printx.Printx(1, strMsg);

		//发送短信
		//int n = m_printx.m_students.size();
		//for (int nRow = 0; nRow < n; nRow++)
		//{
		//	CMSGINFO dlgMsg;
		//	dlgMsg.m_strStu = m_printx.m_students[nRow].strName;
		//	dlgMsg.m_strCoach = m_printx.m_sheetInfo->strCoach;
		//	dlgMsg.m_strCar = m_printx.m_sheetInfo->strCarID;
		//	int classi = m_printx.m_students[nRow].nClassStep + 1;
		//	dlgMsg.m_strClassIndex.Format("%d", classi);
		//	CString classTime = GetClassTime(m_printx.m_students[nRow].nClassTime);
		//	dlgMsg.m_strDate = m_printx.m_students[nRow].strDate + classTime;
		//	dlgMsg.Init(5);
		//	CString strSMS0 = dlgMsg.m_strSMS;
		//	CString strTEL = m_printx.m_students[nRow].strTEL;

		//	CString strSMS;
		//	strSMS.Format("%s:%s>%s", strTEL, dlgMsg.m_strStu, strSMS0);
		//	int SMSlen = strlen(strSMS);
		//	int len = 6 + SMSlen;
		//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		//	if (pFrame->m_pSendBuf != NULL)
		//	{
		//		//MessageBox("上一个信息还未处理完毕，请稍等重试。");
		//		WaitForSingleObject(pFrame->m_hSocketEvent, 2000); //等待信息发送
		//	}
		//	else
		//	{
		//		pFrame->m_isSendReady = FALSE;
		//		pFrame->m_pSendBuf = new BYTE[len];//发送完删除
		//		pFrame->m_nSendLen = len;
		//		pFrame->m_pSendBuf[0] = 2; //发送短信平台数据
		//		pFrame->m_pSendBuf[1] = 5; //短信类型
		//		memcpy(pFrame->m_pSendBuf + 2, &SMSlen, 4); //档案数量

		//		char* data = strSMS.GetBuffer();
		//		memcpy(pFrame->m_pSendBuf + 6, data, SMSlen);
		//		strSMS.ReleaseBuffer();
		//		pFrame->m_isSendReady = TRUE;

		//		WaitForSingleObject(pFrame->m_hSocketEvent, 2000); //等待信息发送
		//	}
		//}
	}
	
}


void CALLBACK COrders::OnGrid1Click(LPVOID lParam)
{
	COrders* pThis = (COrders*)lParam;


	pThis->m_index[0] = -1;
	pThis->m_index[1] = -1;
	pThis->m_index[2] = -1;

	CBCGPGridRow* pRow = pThis->m_wndGrid1.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		pThis->m_printx.m_students.clear();
		pThis->m_index[0] = nRow;
		//pThis->m_dataPrint[nRow] = 1;
		//xPublic::CMyPrint printx;
		//xPublic::CLASSINFO classInfo;
		//xPublic::SHEETINFO sheetInfo;


		//表单信息
		CString strCar = pThis->m_datas1[nRow][3];
		pThis->m_sheetInfo.strCarID = strCar;
		pThis->m_sheetInfo.strCoach = pThis->m_datas1[nRow][4];
		pThis->m_sheetInfo.strCoachID = pThis->m_datas1[nRow][8];

		CString str = pThis->m_datas1[nRow][0]; //派工日期
		//解析字符串得到日期
		int pos1, pos2;
		pos1 = str.Find('/');
		pos2 = str.ReverseFind('/');
		int nYear = atoi(str.Left(pos1));
		int nMonth = atoi(str.Mid(pos1 + 1, pos2));
		int nDay = atoi(str.Mid(pos2 + 1));
		CTime tOrderDay(nYear, nMonth, nDay, 0, 0, 0);

		pThis->m_sheetInfo.strDate = tOrderDay.Format("%Y年%m月%d日制");
		//课程信息
		CString classType = pThis->m_datas1[nRow][10];
		pThis->m_sheetInfo.strClassType = classType;
		int classStep = atoi(pThis->m_datas1[nRow][11]);
		pThis->m_classInfo.nClassID = classStep;
		//学员信息
		CString strBookDate = pThis->m_datas1[nRow][1];
		CString name = pThis->m_datas1[nRow][5];
		int classID = atoi(pThis->m_datas1[nRow][2]) - 1;
		int  nClass = (classID) / 2;
		xPublic::STUDENTINFO student(name, strBookDate.Right(8), classID, classStep, g_nMaxBooking);
		student.strTEL = pThis->m_datas1[nRow][12];
		pThis->m_printx.AddStudent(student);

		//寻找同个派工单的学员
		int rows = pThis->m_datas1.size();
		CString strCoachID = pThis->m_datas1[nRow][8];
		int nstu = 0;
		for (int i = 0; i < rows; i++)
		{
			CString strCar1 = pThis->m_datas1[i][3];
			CString strBookDate1 = pThis->m_datas1[i][1];
			int classID1 = atoi(pThis->m_datas1[i][2]) - 1;
			int nClass1 = (classID1) / 2;
			CString strCoachID1 = pThis->m_datas1[i][8];
			if (i != nRow && strCar == strCar1 && strBookDate == strBookDate1 
				&& nClass == nClass1 && strCoachID == strCoachID1)
			{
				CString name1 = pThis->m_datas1[i][5];
				int classStep1 = atoi(pThis->m_datas1[i][9]) + 1;
				xPublic::STUDENTINFO student1(name1, strBookDate.Right(8), classID1, classStep1, g_nMaxBooking);
				student1.strTEL = pThis->m_datas1[i][12];
				pThis->m_printx.AddStudent(student1);
				//pThis->m_dataPrint[i] = 1;
				pThis->m_index[nstu + 1] = i; nstu++;
				continue;
			}
		}

		//COrderDetail dlgDetail;
		//dlgDetail.m_wndPrint.m_classInfo = pThis->m_classInfo;
		//dlgDetail.m_wndPrint.m_sheetInfo = pThis->m_sheetInfo;
		//dlgDetail.m_wndPrint.m_printData.m_students = pThis->m_printx.m_students;
		//dlgDetail.DoModal();

		//printx.PrinterInit(&sheetInfo, &classInfo);
		//CString strMsg;
		//printx.Printx(1, strMsg);

		if(!pThis->m_enableMultiSel) pThis->PostMessageA(WM_USER_UPDATE_VIEW, (WPARAM)2);
	}
}

LRESULT COrders::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;

	m_wndGrid1.GridRefresh(m_datas1.size());
	
	return 0;
}


void COrders::OnBnClickedCheck1()
{
	UpdateData(TRUE);
}


void COrders::OnBnClickedPreprint()
{
	COrderDetail dlgDetail;
	dlgDetail.m_wndPrint.m_classInfo = m_classInfo;
	dlgDetail.m_wndPrint.m_sheetInfo = m_sheetInfo;
	dlgDetail.m_wndPrint.m_printData.m_students = m_printx.m_students;
	if (dlgDetail.DoModal() == IDOK)
	{
		int row1 = m_index[0], row2 = m_index[1], row3 = m_index[2];
		if (row1 >= 0) m_dataPrint[row1] = 1;
		if (row2 >= 0) m_dataPrint[row2] = 1;
		if (row3 >= 0) m_dataPrint[row3] = 1;

		m_index[0] = -1;
		m_index[1] = -1;
		m_index[2] = -1;

		m_wndGrid1.GridRefresh(m_datas1.size());
	}
}
