// ViewViewK1Exam.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewK23Exam.h"
#include "MSGINFO.h"
#include "DlgDateItem.h"


// CViewK23Exam

IMPLEMENT_DYNCREATE(CViewK23Exam, CBCGPFormView)

CViewK23Exam::CViewK23Exam()
: CBCGPFormView(CViewK23Exam::IDD)
{
	EnableVisualManagerStyle();
}

CViewK23Exam::~CViewK23Exam()
{
}

void CViewK23Exam::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation1);
	DDX_Control(pDX, IDC_GRID2, m_wndGridLocation2);

	DDX_Control(pDX, IDC_COLOR1, m_C[0]);
	DDX_Control(pDX, IDC_COLOR2, m_C[1]);
	DDX_Control(pDX, IDC_COLOR3, m_C[2]);
	DDX_Control(pDX, IDC_COLOR4, m_C[3]);
	DDX_Control(pDX, IDC_COLOR5, m_C[4]);
	DDX_Control(pDX, IDC_COLOR6, m_C[5]);
}

BEGIN_MESSAGE_MAP(CViewK23Exam, CBCGPFormView)
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_BN_CLICKED(IDC_PASS, &CViewK23Exam::OnBnClickedPass)
	ON_BN_CLICKED(IDC_RETURN, &CViewK23Exam::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_SENDSMS, &CViewK23Exam::OnBnClickedSendsms)
	ON_BN_CLICKED(IDC_K1PASS, &CViewK23Exam::OnBnClickedK23pass)
	ON_BN_CLICKED(IDC_UPDATE, &CViewK23Exam::OnBnClickedUpdate)
	ON_WM_PAINT()
END_MESSAGE_MAP()


LRESULT CViewK23Exam::OnUserMessage(WPARAM wp, LPARAM lp)
{
	int flag = (int)lp;
	if (flag == 5)  //数据发送成功
	{
		Refresh();
	}
	return 0;
}

LRESULT CViewK23Exam::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;

	switch (type)
	{
	case 1:
		Refresh();
		break;
	case 3:
		m_wndGrid2.GridRefresh(m_datas2.size());
	case 2:
		m_wndGrid1.GridRefresh(m_datas1.size());
		break;
	}

	return 0;
}

// CViewK23Exam 诊断

#ifdef _DEBUG
void CViewK23Exam::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewK23Exam::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewK23Exam 消息处理程序

static BOOL CALLBACK Grid1Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK23Exam* pThis = (CViewK23Exam*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas1.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas1.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas1[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
			return TRUE;
		}

		//颜色状态控制 已考过一科的黄色， 满足报考条件的红色， 已经报考的浅绿色， 已经通知的深绿色
		CString strType = pThis->m_datas1[nRow][4];
		int nClass = atoi(pThis->m_datas1[nRow][5]);
		int minClass = g_nMinK2Class;
		int statIndex = 6;
		int statIndex1 = 7;
		if (strType == "科目三")
		{
			statIndex = 7;
			statIndex1 = 6;
			minClass = g_nMinK3Class;
		}

		int state = atoi(pThis->m_datas1[nRow][statIndex]);
		switch (state)
		{
		case 0:
			if (nClass >= minClass) //满足报考条件
				pdi->item.clrBackground = COLOR_LITTLE;
			else //不满足报考条件的 考察另一个科目的考试情况
			{
				if (pThis->m_datas1[nRow][statIndex1] == "3")
					pdi->item.clrBackground = COLOR_HALF;
			}
			break;
		case 1:
			pdi->item.clrBackground = COLOR_DOING;
			break;
		case 2:
			pdi->item.clrBackground = COLOR_DONE;
			break;
		case 3:
			pdi->item.clrBackground = COLOR_HALF;
			break;
		}
		
		if (nCol == 4)
		{
			if (strType == "科目二")
				pdi->item.clrBackground = COLOR_K2;
			else if (strType == "科目三")
				pdi->item.clrBackground = COLOR_K3;
		}
	}

	return TRUE;
}

static BOOL CALLBACK Grid2Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewK23Exam* pThis = (CViewK23Exam*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas2.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas2.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas2[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
			return TRUE;
		}

		//颜色状态控制 已经发过短信的深绿色显示
		CString strType = pThis->m_datas2[nRow][6];
		int statIndex = 7; //默认科目二
		if (strType == "科目三")
		{
			statIndex = 8;
		}
		int stat = atoi(pThis->m_datas2[nRow][statIndex]);
		if (stat == 2)
		{
			pdi->item.clrBackground = COLOR_DONE;
		}
		else if (stat == 1)
		{
			pdi->item.clrBackground = COLOR_DOING;
		}

		if (nCol == 6)
		{
			if (strType == "科目二")
				pdi->item.clrBackground = COLOR_K2;
			else if (strType == "科目三")
				pdi->item.clrBackground = COLOR_K3;
		}
	}

	return TRUE;
}


void CViewK23Exam::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();


	CRect rect;
	m_wndGridLocation1.GetClientRect(&rect);
	m_wndGridLocation1.MapWindowPoints(this, &rect);

	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	m_wndGrid1.Create(nStyle, rect, this, IDC_GRID_STUPRO);
	m_wndGrid1.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid1.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid1.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid1.SetReadOnly();
	m_wndGrid1.SetWholeRowSel();
	m_wndGrid1.SetSingleSel(); //只能选一个
	m_wndGrid1.EnableRowHeader(TRUE);
	m_wndGrid1.EnableLineNumbers();

	int nColumn = 0;
	int hw = m_wndGrid1.GetRowHeaderWidth();
	std::vector<CString> arrColumns;
	arrColumns.push_back("姓名");
	arrColumns.push_back("性别");
	arrColumns.push_back("手机");
	arrColumns.push_back("档案号");
	arrColumns.push_back("培训类型");
	arrColumns.push_back("课时");


	int nColumns = arrColumns.size();
	m_nColumns1 = nColumns;
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGrid1.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndGrid1.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid1.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid1.EnableVirtualMode(Grid1Callback, (LPARAM)this);


	m_wndGridLocation2.GetClientRect(&rect);
	m_wndGridLocation2.MapWindowPoints(this, &rect);
	m_wndGrid2.Create(nStyle, rect, this, IDC_GRID_STUPRO);
	m_wndGrid2.SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	m_wndGrid2.EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	m_wndGrid2.SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_wndGrid2.SetReadOnly();
	m_wndGrid2.SetWholeRowSel();
	//m_wndGrid2.SetSingleSel(); //只能选一个
	m_wndGrid2.EnableRowHeader(TRUE);
	m_wndGrid2.EnableLineNumbers();

	nColumn = 0;
	hw = m_wndGrid2.GetRowHeaderWidth();
	arrColumns.clear();
	arrColumns.push_back("姓名");
	arrColumns.push_back("性别");
	arrColumns.push_back("手机");
	arrColumns.push_back("申领");
	arrColumns.push_back("档案号");
	arrColumns.push_back("考试日期");
	arrColumns.push_back("考试科目");


	nColumns = arrColumns.size();
	//m_nColumns = nColumns;
	w = rect.Width() - hw;
	nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		m_wndGrid2.InsertColumn(i, arrColumns[i], nColumnWidth);
		m_wndGrid2.SetColumnAlign(i, HDF_CENTER);
		m_wndGrid2.SetHeaderAlign(i, HDF_CENTER);
	}
	//注册虚拟列表回调函数
	m_wndGrid2.EnableVirtualMode(Grid2Callback, (LPARAM)this);
}

void CViewK23Exam::Refresh()
{
	CString strMsg("");
	CString strSQL("");
	strSQL.Format("SELECT students.SNAME, students.GENDER, students.TEL, students.FILE_NAME, \
					students.CLASS_TYPE, students.CLASS_NUM, \
					stuDates.K2_STAT, stuDates.K3_STAT FROM students \
					inner join stuDates ON students.FILE_NAME=stuDates.STU_ID\
					WHERE students.CLASS_TYPE!='0' AND stuDates.ENDED='0'");
	m_datas1.clear();
	if (g_mysqlCon.ExecuteQuery(strSQL, m_datas1, strMsg))
	{
		ShowMsg2Output1("查询路训学生信息成功");
	}
	ShowMsg2Output1(strMsg);

	m_wndGrid1.GridRefresh(m_datas1.size());



	strSQL.Format("SELECT students.SNAME, students.GENDER, students.TEL, students.CAR_TYPE,\
				  	students.FILE_NAME, stuDates.K2_DATE, students.CLASS_TYPE, \
					stuDates.K2_STAT, stuDates.K3_STAT FROM students \
					inner join stuDates ON students.FILE_NAME=stuDates.STU_ID\
					WHERE stuDates.K2_STAT>'0' AND stuDates.K2_STAT<'3'");
	m_datas2.clear();
	g_mysqlCon.ExecuteQuery(strSQL, m_datas2, strMsg);
	ShowMsg2Output1(strMsg);
	strSQL.Format("SELECT students.SNAME, students.GENDER, students.TEL, students.CAR_TYPE,\
				  	students.FILE_NAME, stuDates.K3_DATE, students.CLASS_TYPE, \
					stuDates.K2_STAT, stuDates.K3_STAT FROM students \
					inner join stuDates ON students.FILE_NAME=stuDates.STU_ID\
					WHERE stuDates.K3_STAT>'0' AND stuDates.K3_STAT<'3'");
	g_mysqlCon.ExecuteQuery(strSQL, m_datas2, strMsg);
	ShowMsg2Output1(strMsg);
	m_wndGrid2.GridRefresh(m_datas2.size());
}

void CViewK23Exam::OnBnClickedPass()
{
	CBCGPGridRow* pRow = m_wndGrid1.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strClassNum = m_datas1[nRow][5];
		int nclass = atoi(strClassNum);
		CString strClassType = m_datas1[nRow][4];
		CString strTypeName;
		if (strClassType == "科目二")
		{
			strTypeName = "K2_STAT";
			if (nclass < 8)
			{
				if (MessageBox("该学员课时尚未达到报考要求，确认报考吗？", "警告", MB_YESNO) != IDYES) return;
			}
		}
		else if (strClassType == "科目三")
		{
			strTypeName = "K3_STAT";
			if (nclass < 10)
			{
				if (MessageBox("该学员课时尚未达到报考要求，确认报考吗？", "警告", MB_YESNO) != IDYES) return;
			}
		}

		CString strMsg, strSQL;
		CString strStuID = m_datas1[nRow][3];
		strSQL.Format("UPDATE stuDates SET %s='1' WHERE STU_ID='%s'", strTypeName, strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		Refresh();
	}
}


void CViewK23Exam::OnBnClickedReturn()
{
	CBCGPGridRow* pRow = m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		if (m_datas2[nRow][5] != "0") return;

		CString strMsg, strSQL;
		CString strStuID = m_datas2[nRow][4];
		CString strClassType = m_datas2[nRow][6];
		CString strTypeName;
		if (strClassType == "科目二")
		{
			strTypeName = "K2_STAT";
		}
		else if (strClassType == "科目三")
		{
			strTypeName = "K3_STAT";
		}

		strSQL.Format("UPDATE stuDates SET %s='0' WHERE STU_ID='%s'", strTypeName, strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		ShowMsg2Output1(strMsg);

		Refresh();
	}

}


void CViewK23Exam::OnBnClickedSendsms()
{
	CBCGPGridRow* pRow = m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();


		BOOL isResend = FALSE; //是否当前考试不通过，报考下一场。
		CString strOldDate;

		CString strType = m_datas2[nRow][6];
		int statIndex = 7; //默认科目二的状态
		CString strTypeName = "K2_DATE";
		if (strType == "科目三")
		{
			statIndex = 8;
			strTypeName = "K3_DATE";
		}
		if (m_datas2[nRow][statIndex] == "2")
		{
			isResend = TRUE;
			strOldDate = m_datas2[nRow][5];
			strOldDate.Replace("年", "/");
			strOldDate.Replace("月", "/");
			strOldDate.Replace("日", "");
			CString strM;
			strM.Format("%s已经设置过考试日期，是否上一次考试未通过？", m_datas2[nRow][0]);
			if (MessageBox(strM, "警告", MB_YESNO) != IDYES) return;
		}
		CString strTypeStat = strTypeName.Left(3) + "STAT";

		CString strStuID = m_datas2[nRow][4];


		CDlgDateItem dlg;
		if(dlg.DoModal() != IDOK) return;

		CString strDate = dlg.m_strDate; //考试日期
		CString strSQL, strMsg;
		strSQL.Format("UPDATE stuDates SET %s='%s', %s='2' WHERE STU_ID='%s'", strTypeName, strDate, strTypeStat, strStuID);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		if (isResend == TRUE)
		{
			strSQL.Format("UPDATE stuDateHistory SET EXAM_RESULT='2' WHERE EXAM_DATE='%s' AND STU_ID='%s' AND EXAM_TYPE='%s'", strOldDate, strStuID, strType);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		}

		strDate.Replace("年", "/");
		strDate.Replace("月", "/");
		strDate.Replace("日", "");
		//删除重复数据
		strSQL.Format("DELETE FROM stuDateHistory WHERE EXAM_DATE='%s' AND STU_ID='%s' AND EXAM_TYPE='%s'", strDate, strStuID, strType);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		strSQL.Format("INSERT INTO stuDateHistory (EXAM_DATE, STU_ID, EXAM_TYPE) VALUES('%s', '%s', '%s')", strDate, strStuID, strType);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		Refresh();
	}
}


void CViewK23Exam::OnBnClickedK23pass()
{
	CBCGPGridRow* pRow = m_wndGrid2.GetCurSel();
	if (pRow != NULL)
	{
		int nRow = pRow->GetRowId();

		CString strSQL, strMsg;

		CString strDate = m_datas2[nRow][5];
		if (strDate == "0" || strDate.IsEmpty())
		{
			return;
		}

		CString strType = m_datas2[nRow][6];
		int statIndex = 8; //默认科目二通过, 通过考试后查询科目三的状态
		CString strTypeName = "K2_STAT";
		CString strNextClass = "科目三";
		if (strType == "科目三")
		{
			statIndex = 7;
			strTypeName = "K3_STAT";
			strNextClass = "科目二";
		}

		strMsg.Format("该操作不可撤销，请确认%s学员是否已经通过%s考试？", m_datas2[nRow][0], strType);
		if (MessageBox(strMsg, "警告", MB_YESNOCANCEL) != IDYES) return;


		strSQL.Format("UPDATE stuDates SET %s='3' WHERE STU_ID='%s'", strTypeName, m_datas2[nRow][4]);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		strDate.Replace("年", "/");
		strDate.Replace("月", "/");
		strDate.Replace("日", "");
		strSQL.Format("UPDATE stuDateHistory SET EXAM_RESULT='1' WHERE EXAM_DATE='%s' AND STU_ID='%s' AND EXAM_TYPE='%s'", strDate, m_datas2[nRow][4], strType);
		g_mysqlCon.ExecuteSQL(strSQL, strMsg);

		//检查该学员是否通过全部考试
		int anotherStat = atoi(m_datas2[nRow][statIndex]);
		if (anotherStat < 3) //另一个科目没有过
		{
			strSQL.Format("UPDATE students SET CLASS_TYPE='%s' WHERE FILE_NAME='%s'", strNextClass, m_datas2[nRow][4]);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		}
		else //全部通过，完成驾校学习，设置标志位
		{
			strSQL.Format("UPDATE stuDates SET ENDED='1' WHERE STU_ID='%s'", m_datas2[nRow][4]);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
			strSQL.Format("UPDATE students SET STEP='1010' WHERE FILE_NAME='%s'", m_datas2[nRow][4]);
			g_mysqlCon.ExecuteSQL(strSQL, strMsg);
		}

		Refresh();
	}
}


void CViewK23Exam::OnBnClickedUpdate()
{
	Refresh();
}


void CViewK23Exam::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	COLORREF col[6] = { COLOR_LITTLE, COLOR_DOING, COLOR_DONE, COLOR_HALF, COLOR_K2, COLOR_K3 };
	for (int i = 0; i < 6; i++)
	{
		CRect rect;
		m_C[i].GetClientRect(&rect);
		m_C[i].MapWindowPoints(this, &rect);
		COLORREF color = col[i];
		CBrush brush(color);
		dc.FillRect(rect, &brush);
	}
}
