// ViewScan.cpp : 实现文件
//

#include "stdafx.h"
#include "SMS2.h"
#include "ViewScan.h"
#include "MainFrm.h"


// CViewScan

IMPLEMENT_DYNCREATE(CViewScan, CBCGPFormView)

CViewScan::CViewScan()
	: CBCGPFormView(CViewScan::IDD)
{
	EnableVisualManagerStyle();

	m_canEraseBkgnd = FALSE;
	m_isPublic = TRUE;
	m_strMaxClass = "全部有00课时"; //全部课时
	m_strBookedClass = "已预约00课时"; //已预约课时数
	m_strUsedClass = "已使用00课时"; //已使用课时数
}

CViewScan::~CViewScan()
{
}

void CViewScan::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POS1, m_BKpos1);
	DDX_Control(pDX, IDC_POS2, m_BKpos2);
	DDX_Control(pDX, IDC_CONTENT, m_Content);
	DDX_Control(pDX, IDC_PIE, m_Pie);
	DDX_Control(pDX, IDC_PIC, m_SPhoto);

	DDX_Control(pDX, IDC_GRID1, m_wndGridLocation1);
	DDX_Control(pDX, IDC_GRID2, m_wndGridLocation2);
	DDX_Control(pDX, IDC_GRID3, m_wndGridLocation3);

	DDX_Control(pDX, IDC_BASICINFO, m_BasicInfo);
	DDX_Control(pDX, IDC_BASICINFO2, m_BasicInfo2);
	DDX_Control(pDX, IDC_BASICINFO3, m_BasicInfo3);
	DDX_Control(pDX, IDC_BASICINFO4, m_BasicInfo4);
	DDX_Control(pDX, IDC_BASICINFO5, m_BasicInfo5);


	DDX_Control(pDX, IDC_S1, m_S[0]);
	DDX_Control(pDX, IDC_S2, m_S[1]);
	DDX_Control(pDX, IDC_S3, m_S[2]);
	DDX_Control(pDX, IDC_S4, m_S[3]);
	DDX_Control(pDX, IDC_S5, m_S[4]);
	DDX_Control(pDX, IDC_S6, m_S[5]);
	DDX_Control(pDX, IDC_S7, m_S[6]);
	DDX_Control(pDX, IDC_S8, m_S[7]);
	DDX_Control(pDX, IDC_S9, m_S[8]);
	DDX_Control(pDX, IDC_S10, m_S[9]);

	DDX_Control(pDX, IDC_T1, m_T[0]);
	DDX_Control(pDX, IDC_T2, m_T[1]);
	DDX_Control(pDX, IDC_T3, m_T[2]);
	DDX_Control(pDX, IDC_T4, m_T[3]);
	DDX_Control(pDX, IDC_T5, m_T[4]);

	DDX_Control(pDX, IDC_D1, m_D[0]); //姓名
	DDX_Control(pDX, IDC_D2, m_D[1]); //出生日期
	DDX_Control(pDX, IDC_D3, m_D[2]); //电话
	DDX_Control(pDX, IDC_D4, m_D[3]); //车型
	DDX_Control(pDX, IDC_D5, m_D[4]); //身份证
	DDX_Control(pDX, IDC_D6, m_D[5]); //登记住所
	DDX_Control(pDX, IDC_D7, m_D[6]); //报名日期
	DDX_Control(pDX, IDC_D8, m_D[7]); //档案号
	DDX_Control(pDX, IDC_D9, m_D[8]); //费用
	DDX_Control(pDX, IDC_D10, m_D[9]); //正在进行的课程类别
	DDX_Control(pDX, IDC_D11, m_D[10]); //全部课时
	DDX_Control(pDX, IDC_D12, m_D[11]); //已预约课时
	DDX_Control(pDX, IDC_D13, m_D[12]); //已使用课时
	DDX_Text(pDX, IDC_D1, m_student.strName); //姓名
	DDX_Text(pDX, IDC_D2, m_student.strBirthDay); //出生日期
	DDX_Text(pDX, IDC_D3, m_student.strTEL); //电话
	DDX_Text(pDX, IDC_D4, m_student.strCarType); //车型
	DDX_Text(pDX, IDC_D5, m_student.strIDCard); //身份证
	DDX_Text(pDX, IDC_D6, m_student.strHome); //登记住所
	DDX_Text(pDX, IDC_D7, m_student.strSignDate); //报名日期
	DDX_Text(pDX, IDC_D8, m_student.strFileName); //档案号
	DDX_Text(pDX, IDC_D9, m_student.strFee); //费用
	DDX_Text(pDX, IDC_D10, m_strClassType); //正在进行的课程类别
	DDX_Text(pDX, IDC_D11, m_strMaxClass); //全部课时
	DDX_Text(pDX, IDC_D12, m_strBookedClass); //已预约课时
	DDX_Text(pDX, IDC_D13, m_strUsedClass); //已使用课时
}

BEGIN_MESSAGE_MAP(CViewScan, CBCGPFormView)
	ON_MESSAGE(WM_USER_MESSAGE, OnUserMessage)
	ON_MESSAGE(WM_USER_UPDATE_VIEW, OnUserUpdate)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BOOK, &CViewScan::OnBnClickedBook)
END_MESSAGE_MAP()


HBRUSH CViewScan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_S1:
	case IDC_S2:
	case IDC_S3:
	case IDC_S4:
	case IDC_S5:
	case IDC_S6:
	case IDC_S7:
	case IDC_S8:
	case IDC_S9:
	case IDC_S10:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(51, 103, 155));
				   CFont font1;
				   font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //返回一个空画刷
				   break;
	}
	case IDC_D1:
	case IDC_D2:
	case IDC_D3:
	case IDC_D4:
	case IDC_D5:
	case IDC_D6:
	case IDC_D7:
	case IDC_D8:
	case IDC_D9:
	case IDC_D10:
	case IDC_D11:
	case IDC_D12:
	case IDC_D13:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(144, 146, 148));
				   CFont font1;
				   font1.CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //返回一个空画刷
				   break;
	}
	case IDC_T1:
	case IDC_T2:
	case IDC_T3:
	case IDC_T4:
	case IDC_T5:
	{
				   pDC->SetBkMode(TRANSPARENT);
				   pDC->SetTextColor(RGB(46, 100, 153));
				   CFont font1;
				   font1.CreateFontA(25, 0, 0, 0, FW_BOLD, 0, 0, 0, 0,
					   0, 0, 0, VARIABLE_PITCH | FF_SWISS, "微软雅黑");
				   pDC->SelectObject(&font1);
				   return HBRUSH(GetStockObject(NULL_BRUSH)); //返回一个空画刷
				   break;
	}
	}

	return hbr;
}


// CViewScan 诊断

#ifdef _DEBUG
void CViewScan::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewScan::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewScan 消息处理程序

static BOOL CALLBACK Grid1Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewScan* pThis = (CViewScan*)lp;

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
		}
	}

	return TRUE;
}

static BOOL CALLBACK Grid2Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewScan* pThis = (CViewScan*)lp;

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
		}
	}

	return TRUE;
}

static BOOL CALLBACK Grid3Callback(BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT(pdi != NULL);

	CViewScan* pThis = (CViewScan*)lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	int ndata = pThis->m_datas3.size(); //number of data exist
	if (nCol >= 0 && nRow >= 0 && ndata > 0 && nRow < ndata)
	{
		std::vector<CStrs>::iterator it = pThis->m_datas3.begin() + nRow;
		if (!it->empty())
		{
			pdi->item.varValue = pThis->m_datas3[nRow][nCol];
		}
		else
		{
			pdi->item.varValue = "访问内存出错";
		}
	}

	return TRUE;
}


void CViewScan::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	//获取背景定位点位置
	m_BKpos1.GetClientRect(&m_rctBK1);
	m_BKpos1.MapWindowPoints(this, &m_rctBK1);
	m_BKpos2.GetClientRect(&m_rctBK2);
	m_BKpos2.MapWindowPoints(this, &m_rctBK2);

	m_Content.GetClientRect(&m_rctContent);
	m_Content.MapWindowPoints(this, &m_rctContent);

	m_BasicInfo.GetClientRect(&m_rctBasicInfo);
	m_BasicInfo.MapWindowPoints(this, &m_rctBasicInfo);
	m_BasicInfo2.GetClientRect(&m_rctBasicInfo2);
	m_BasicInfo2.MapWindowPoints(this, &m_rctBasicInfo2);
	m_BasicInfo3.GetClientRect(&m_rctBasicInfo3);
	m_BasicInfo3.MapWindowPoints(this, &m_rctBasicInfo3);
	m_BasicInfo4.GetClientRect(&m_rctBasicInfo4);
	m_BasicInfo4.MapWindowPoints(this, &m_rctBasicInfo4);
	m_BasicInfo5.GetClientRect(&m_rctBasicInfo5);
	m_BasicInfo5.MapWindowPoints(this, &m_rctBasicInfo5);

	m_Pie.GetClientRect(&m_rctPie);
	m_Pie.MapWindowPoints(this, &m_rctPie);

	//列表
	CRect rctGrid;
	m_wndGridLocation1.GetClientRect(&rctGrid);
	m_wndGridLocation1.MapWindowPoints(this, &rctGrid);
	std::vector<CString> arrColumns;
	arrColumns.push_back("政治审核");
	arrColumns.push_back("科一上课");
	arrColumns.push_back("科一报考");
	arrColumns.push_back("科一考试");
	//arrColumns.push_back("");
	GridInit(arrColumns, rctGrid, &m_wndGrid1);
	//注册虚拟列表回调函数
	m_wndGrid1.EnableVirtualMode(Grid1Callback, (LPARAM)this);

	m_wndGridLocation2.GetClientRect(&rctGrid);
	m_wndGridLocation2.MapWindowPoints(this, &rctGrid);
	arrColumns.clear();
	arrColumns.push_back("科二培训");
	arrColumns.push_back("科二报考");
	arrColumns.push_back("科二考试");
	//arrColumns.push_back("");
	GridInit(arrColumns, rctGrid, &m_wndGrid2);
	//注册虚拟列表回调函数
	m_wndGrid2.EnableVirtualMode(Grid2Callback, (LPARAM)this);

	m_wndGridLocation3.GetClientRect(&rctGrid);
	m_wndGridLocation3.MapWindowPoints(this, &rctGrid);
	arrColumns.clear();
	arrColumns.push_back("科三培训");
	arrColumns.push_back("科三报考");
	arrColumns.push_back("科三考试");
	//arrColumns.push_back("");
	GridInit(arrColumns, rctGrid, &m_wndGrid3);
	//注册虚拟列表回调函数
	m_wndGrid3.EnableVirtualMode(Grid3Callback, (LPARAM)this);


	m_SPhoto.InitPicSource(&m_img); //注册图片控件数据源

	m_strMaxClass.Format("全部有%02d课时", g_nMaxBooking);
}

void CViewScan::GridInit(std::vector<CString>& arrColumns, CRect& rect, CVirtualGridCtrl* pGrid)
{
	DWORD nStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;
	pGrid->Create(nStyle, rect, this, (UINT)-3);
	pGrid->SetCustomColors(-1, -1, -1, -1, -1, RGB(213, 213, 213)); //设置边框
	pGrid->EnableHeader(TRUE, 0); //不允许表头移动
	// Set grid tab order (first):
	pGrid->SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	pGrid->SetReadOnly();
	pGrid->SetWholeRowSel();
	pGrid->SetSingleSel(); //只能选一个
	pGrid->EnableRowHeader(TRUE);
	pGrid->EnableLineNumbers();

	int nColumn = 0;
	int hw = pGrid->GetRowHeaderWidth();
	int nColumns = arrColumns.size();
	int w = rect.Width() - hw;
	int nColumnWidth = w / nColumns;
	for (int i = 0; i < nColumns; i++)
	{
		pGrid->InsertColumn(i, arrColumns[i], nColumnWidth);
		pGrid->SetColumnAlign(i, HDF_CENTER);
		pGrid->SetHeaderAlign(i, HDF_CENTER);
	}
}

void CViewScan::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	MapWindowPoints(this, &rect);

	CDC     MemDC;
	//HBITMAP hbitmap;
	CBitmap bitmp;
	MemDC.CreateCompatibleDC(&dc);

	Point p1(m_rctBK1.left, m_rctBK1.top);
	Point p2(m_rctBK2.right, m_rctBK2.bottom);
	int width = m_rctBK2.right - m_rctBK1.left;
	int height = m_rctBK2.bottom - m_rctBK1.top;

	bitmp.CreateCompatibleBitmap(&dc,  width, height);
	MemDC.SelectObject(&bitmp);

	Graphics graph(MemDC.m_hDC);
	//LinearGradientBrush lgb(p1, p2, Color(145, 168, 201), Color(213, 202, 181));
	LinearGradientBrush lgb(p1, p2, Color(156, 187, 232), Color(213, 202, 181));
	graph.FillRectangle(&lgb, m_rctBK1.left, m_rctBK1.top, width, height);


	SolidBrush brush(Color(230, 230, 230));
	graph.FillRectangle(&brush, m_rctContent.left, m_rctContent.top, m_rctContent.Width(), m_rctContent.Height());

	SolidBrush brush1(Color(240, 243, 244));
	Pen pen(Color(46, 100, 153));
	Pen pen1(Color(173, 202, 223));
	graph.FillRectangle(&brush1, m_rctBasicInfo.left, m_rctBasicInfo.top, m_rctBasicInfo.Width(), m_rctBasicInfo.Height());
	graph.DrawRectangle(&pen1, m_rctBasicInfo.left, m_rctBasicInfo.top, m_rctBasicInfo.Width(), m_rctBasicInfo.Height());
	graph.FillRectangle(&brush1, m_rctBasicInfo2.left, m_rctBasicInfo2.top, m_rctBasicInfo2.Width(), m_rctBasicInfo2.Height());
	graph.DrawRectangle(&pen1, m_rctBasicInfo2.left, m_rctBasicInfo2.top, m_rctBasicInfo2.Width(), m_rctBasicInfo2.Height());
	graph.FillRectangle(&brush1, m_rctBasicInfo3.left, m_rctBasicInfo3.top, m_rctBasicInfo3.Width(), m_rctBasicInfo3.Height());
	graph.DrawRectangle(&pen1, m_rctBasicInfo3.left, m_rctBasicInfo3.top, m_rctBasicInfo3.Width(), m_rctBasicInfo3.Height());
	graph.FillRectangle(&brush1, m_rctBasicInfo4.left, m_rctBasicInfo4.top, m_rctBasicInfo4.Width(), m_rctBasicInfo4.Height());
	graph.DrawRectangle(&pen1, m_rctBasicInfo4.left, m_rctBasicInfo4.top, m_rctBasicInfo4.Width(), m_rctBasicInfo4.Height());
	graph.FillRectangle(&brush1, m_rctBasicInfo5.left, m_rctBasicInfo5.top, m_rctBasicInfo5.Width(), m_rctBasicInfo5.Height());
	graph.DrawRectangle(&pen1, m_rctBasicInfo5.left, m_rctBasicInfo5.top, m_rctBasicInfo5.Width(), m_rctBasicInfo5.Height());

	//画进度饼图
	int nTotal = g_nMaxBooking;
	SolidBrush brush2(Color(156, 187, 232));
	SolidBrush brush3(Color(191, 211, 242));
	SolidBrush brush4(Color(241, 191, 191));
	Rect rctPie(m_rctPie.left, m_rctPie.top, m_rctPie.Height(), m_rctPie.Height());
	graph.FillEllipse(&brush3, rctPie);
	int nClass = m_student.nClassStep > 15 ? 15 : m_student.nClassStep;
	double sweepA = nClass * 360.0 / nTotal;
	if (sweepA>300)
		graph.FillPie(&brush4, rctPie, -90, sweepA);
	else
		graph.FillPie(&brush2, rctPie, -90, sweepA);
	////将圆分为nTotal份		
	//CPoint o = m_rctPie.CenterPoint();//圆心坐标
	//Point oo(m_rctPie.left + m_rctPie.Height()/2, o.y);
	//Point p(oo.X, m_rctPie.top); //线段的终点起始位置, 线段起点为圆心
	//Point t(oo.X, m_rctPie.top); //保存nUsed位置的线段终点坐标
	//for (int i = 0; i <= nTotal; i++)
	//{
	//	//计算旋转后的坐标
	//	float xx = cos(A*i) * (p.X - oo.X) - sin(A*i) * (p.Y - oo.Y) + oo.X;
	//	float yy = sin(A*i) * (p.X - oo.X) + cos(A*i) * (p.Y - oo.Y) + oo.Y;

	//	graph.DrawLine(&pen, oo, Point(xx, yy));

	//	if (i == m_student.nClassStep)
	//	{
	//		t.X = xx;
	//		t.Y = yy;
	//	}
	//}

	//复制内存DC到屏幕上
	//MFC在滚动视图时只刷新原来看不见的部分，之前能看见的部分不会刷新，而dc获得的是viewport的环境，graph画的坐标是逻辑视图里的内容，
	//所以需要判断滚动条的位置来决定bitblt贴图的起点。
	CPoint pos = GetScrollPosition();
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, pos.x, pos.y, SRCCOPY);


	bitmp.DeleteObject();
	MemDC.DeleteDC();
}


BOOL CViewScan::OnEraseBkgnd(CDC* pDC)
{
	if (!m_canEraseBkgnd)
		return TRUE;
	else
	{
		//m_canEraseBkgnd = FALSE;
		return CBCGPFormView::OnEraseBkgnd(pDC);
	}

	return CBCGPFormView::OnEraseBkgnd(pDC);
}

LRESULT CViewScan::OnUserUpdate(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)wParam;
	if (flag == 1)
		m_canEraseBkgnd = TRUE;

	return 0;
}

void CViewScan::OnBnClickedBook()
{
	CMainFrame* pFrame = (CMainFrame*)::AfxGetMainWnd();

	pFrame->SelectView(VIEW_BOOKING1);
	CView* pView = (CView*)pFrame->GetActiveView();
	pView->SendMessageA(WM_USER_MESSAGE, (WPARAM)&m_student, (LPARAM)1);
}

void CViewScan::Refresh()
{

}

LRESULT CViewScan::OnUserMessage(WPARAM wp, LPARAM lp)
{
	int flag = (int)lp;
	if (flag == 2)
	{
		cv::Mat* pImg = (cv::Mat*)wp;
		m_img = pImg->clone();
		m_SPhoto.Invalidate();
	}
	else if (flag == 1)
	{
		STUDENTINFO* pInfo = (STUDENTINFO*)wp;
		m_student.strName = pInfo->strName;
		m_student.strGender = pInfo->strGender;
		m_student.strCarType = pInfo->strCarType;
		m_student.strFileName = pInfo->strFileName;
		UpdateData(FALSE);

		//查询信息
		CString strMsg, strSQL;
		CDStrs datas;
		strSQL.Format("SELECT BIRTHDAY, REGIST_DATE, TEL, ID, HOME, FEE, CLASS_TYPE, CLASS_NUM, BOOK_NUM \
					  FROM students WHERE FILE_NAME='%s'", m_student.strFileName);
		if (g_mysqlCon.ExecuteQuery(strSQL, datas, strMsg))
		{
			m_student.strBirthDay = datas[0][0];
			m_student.strSignDate = datas[0][1];
			m_student.strTEL = datas[0][2];
			m_student.strIDCard = datas[0][3];
			m_student.strHome = datas[0][4];
			m_student.strFee = datas[0][5];
			m_strClassType = datas[0][6];
			m_student.nClassStep = atoi(datas[0][7]);
			m_student.nClassBooked = atoi(datas[0][8]);
			m_strBookedClass.Format("已预约%02d课时", m_student.nClassBooked);
			m_strUsedClass.Format("已使用%02d课时", m_student.nClassStep);
		}
		ShowMsg2Output1(strMsg);



		//本地打开照片，若本地无，则查询服务器下载
		ShowMsg2Output1("选择预约对象：档案" + m_student.strFileName);
		CString strFile;
		strFile.Format("%s\\%s.bmp", g_strFilePath, m_student.strFileName);
		char* file = strFile.GetBuffer();
		m_img = cv::imread(file);
		strFile.ReleaseBuffer();
		if (m_img.empty()) //本地无照片，从服务器下载
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
				char* cID = m_student.strFileName.GetBuffer();
				memcpy(pFrame->m_pSendBuf + 1, cID, 10); //档案号
				m_student.strFileName.ReleaseBuffer();
				pFrame->m_isSendReady = TRUE;
			}
		}
		else
			m_SPhoto.Invalidate();

		Refresh();


		UpdateData(FALSE);
	}

	return 0;
}
