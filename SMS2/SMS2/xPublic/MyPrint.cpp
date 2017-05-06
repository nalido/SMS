#include "stdafx.h"
#include "MyPrint.h"
#include <math.h>

namespace xPublic{

	CString CMyPrint::GetClassTime(int n) //根据时段编号获得具体时间
	{
		CString res("");
		switch (n)
		{
		case 0:
			res = "A8:00-10:00";
			break;
		case 1:
			res = "A10:00-12:00";
			break;
		case 2:
			res = "P2:00 - 4:00";
			break;
		case 3:
			res = "P4:00 - 6:00";
			break;
		case 4:
			res = "P6:00 - 8:00";
			break;
		default:
			res = "未定义";
			break;
		}
		return res;
	}

	CMyPrint::CMyPrint()
	{
		m_isInit = FALSE;
		m_nMaxPage = 1;
	}

	CMyPrint::~CMyPrint()
	{
		if (m_dcPrinter != NULL)
		{
			m_dcPrinter.Detach();//释放DC
		}
	}

	void CMyPrint::operator=(CMyPrint& print)
	{
		m_classInfo = print.m_classInfo;
		m_sheetInfo = print.m_sheetInfo;
		m_students = print.m_students;
	}

	void CMyPrint::PrinterInit(SHEETINFO* SheetInfo, CLASSINFO* ClassInfo)
	{
		m_nMaxPage = 1;
		m_sheetInfo = SheetInfo;
		m_classInfo = ClassInfo;

		//初始化打印机，获取打印机句柄
		CPrintDialog dlg(FALSE, PD_ALLPAGES, NULL);//打印对话框对象定义，参数一设置弹出对话框为打印对话框,参数二设置打印范围为全部页面，参数三可指定打印机，这里缺省即可

		/*打印对话框的初始值设置，在实际应用中可根据需要进行设置，如通过预先计算得到要打印的页面范围等*/
		dlg.m_pd.nMinPage = 1;//指定开始/结束页码编辑控件的页码范围的最小值，若nMinPage=nMaxPage，则“页码范围”单选钮和开始/结束页码编辑控件被灰化
		dlg.m_pd.nMaxPage = 1;//指定开始/结束页码编辑控件的页码范围的最大值
		dlg.m_pd.nCopies = 1;//指定拷贝份数编辑控件的初始值
		dlg.m_pd.nFromPage = 1;//指定开始页码编辑控件的初始值
		dlg.m_pd.nToPage = 2;//指定结束页码编辑控件的初始值

		if (dlg.DoModal() == IDOK)
		{
			m_hdcPrinter = dlg.GetPrinterDC();
			if (m_hdcPrinter == NULL)//检取设备环境的句柄,检索不到则提醒并退出
			{
				m_isInit = FALSE;
			}
			else
			{
				m_isInit = TRUE; //获得打印机句柄成功，可以打印

				/*创建图形设备环境并与打印机设备关联，在上面绘图就相当于往打印机要打出的纸上绘图*/
				m_dcPrinter.Attach(m_hdcPrinter);

				/*
				*在打印设置过程中有些东西我们希望用户来选，所以使用了对话框的形式，但是有些
				*东西比如纸张大小和打印方向等我们希望它是固定的，不能让用户随意设置造成麻烦
				*因此，我们利用已经得到的打印机图形设备环境，通过修改它来实现固定设置的功能
				*/
				LPDEVMODE pDevMode;
				pDevMode = (LPDEVMODE)GlobalLock(dlg.m_pd.hDevMode);
				pDevMode->dmOrientation = DMORIENT_PORTRAIT;//设置打印方向为竖向
				pDevMode->dmPaperSize = DMPAPER_A4;//设置纸张大小为A4
				m_dcPrinter.ResetDC(pDevMode);
				
				/*打印的纸张大小我们是需要知道的,此外我们还需要设备像素点和实际长度单位的换算关系即DPI*/
				int xDPI = GetDeviceCaps(m_hdcPrinter, LOGPIXELSX);//返回X方向上每英寸的设备点数，即DPI
				float mmdpix = xDPI / 25.4;//每毫米所占的设备点数
				int yDPI = GetDeviceCaps(m_hdcPrinter, LOGPIXELSY);//返回Y方向上每英寸的设备点数，即DPI
				float mmdpiy = yDPI / 25.4;//每毫米所占的设备点数

				float printerscreenx, printerscreeny;//这里得到打印机屏幕的宽高，也就是纸张的大小，会比实际小约6mm
				printerscreenx = GetDeviceCaps(m_hdcPrinter, HORZSIZE);
				printerscreeny = GetDeviceCaps(m_hdcPrinter, VERTSIZE);

				/*因为要打印的页码范围和份数是用户选择的，所以在此对用户选择的项做处理*/
				int pagebegin, pageend;//打印范围
				if (dlg.PrintAll())//看用户是否选择了全部打印这一项，是的话则全部打印
				{
					pagebegin = dlg.m_pd.nMinPage;
					pageend = dlg.m_pd.nMaxPage;
				}
				else if (dlg.PrintRange())//用户选择了选定页码范围项
				{
					pagebegin = dlg.m_pd.nFromPage;
					pageend = dlg.m_pd.nToPage;
				}
				else//用户选择了选定范围打印，因为我们在这里没有提供选定范围的功能，所以用户一旦选择这里当成是操作错误处理即可
				{
					//MessageBox("Could not choise this one");
					m_dcPrinter.AbortDoc();//错误退出区别于EndDoc
				}
				int  ncopy = dlg.m_pd.nCopies;//用户选择打印分数

				
				m_printerInfo.fmmDpix = mmdpix;
				m_printerInfo.fmmDpiy = mmdpiy;
				m_printerInfo.fPrinterScreenx = printerscreenx;
				m_printerInfo.fPrinterScreeny = printerscreeny;
				m_printerInfo.nCopy = ncopy;
				m_printerInfo.nPagebegin = pagebegin;
				m_printerInfo.nPageend = pageend;
			}
		}
	}

	void CMyPrint::Printx(int type, CString& strMsg)
	{
		if (!m_isInit)
		{
			strMsg = "取消打印";
			return;
		}

		//通知打印机驱动程序接收打印文档并开始打印
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("CDC::StartDoc() Code Fragment");

		//如果初始化失败则提醒并退出
		if (m_dcPrinter.StartDoc(&docinfo) < 0)
		{
			strMsg = _T("Printer wouldn't initalize");
			return;
		}

		//开始打印工作
		while (m_printerInfo.nCopy-->0)
		{
			for (int page = m_printerInfo.nPagebegin; page <= m_printerInfo.nPageend; page++)//从选定范围开始打印
			{
				if (m_dcPrinter.StartPage() < 0)
				{
					//MessageBox(_T("Could not start page"));
					m_dcPrinter.AbortDoc();//错误退出区别于EndDoc
				}
				else//如果进入这里则绘出要打印内容并结束掉一页的打印
				{
					if (type==1)
						PrintTrainSheet(page);
					if (type == 2)
						PrintTrainSheet2(page);
					m_dcPrinter.EndPage();
				}
			}
		}
		m_dcPrinter.EndDoc();//打印完成退出
		strMsg = "打印成功";
	}

	void CMyPrint::PrintTrainSheet(int page)
	{
		CPen pen, *pOldPen;// 定义笔对象和指针  
		pen.CreatePen(PS_SOLID, m_printerInfo.fmmDpiy*0.5, RGB(0, 0, 0));
		pOldPen = m_dcPrinter.SelectObject(&pen); 

		CFont font_3mm, font_4mm, font_6mm, font_7mm, font_8mm, font_13mm; //从小到大
		font_3mm.CreateFontA(m_printerInfo.fmmDpiy * 3, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度3mm
		font_4mm.CreateFontA(m_printerInfo.fmmDpiy * 4, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度4mm
		font_6mm.CreateFontA(m_printerInfo.fmmDpiy * 6, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度6mm
		font_7mm.CreateFontA(m_printerInfo.fmmDpiy * 7, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度7mm
		font_8mm.CreateFontA(m_printerInfo.fmmDpiy * 8, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度8mm
		font_13mm.CreateFontA(m_printerInfo.fmmDpiy * 13, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度13mm
		m_dcPrinter.SetBkMode(TRANSPARENT); //文字背景透明

		//绘图
		float W = m_printerInfo.fPrinterScreenx;
		float H = m_printerInfo.fPrinterScreeny;
		int tableR = W - 22; //打印单内表格的右横坐标
		RectangleEx(20, 45, W-20, H-25);// 画表格外框


		CFont *font = m_dcPrinter.SelectObject(&font_8mm);
		TextEx(tableR-60, 60, "车辆编号："+m_sheetInfo->strCarID);

		m_dcPrinter.SelectObject(&font_6mm);
		TextEx(tableR-100, 77, m_sheetInfo->strClassType+"教练员：" + m_sheetInfo->strCoach
			+ "  工号：" + m_sheetInfo->strCoachID);


		m_dcPrinter.SelectObject(&font_7mm);
		CRect rect(CPoint(22, 138), CPoint(tableR,153));
		DrawTextExx(m_sheetInfo->strMsg2, rect); //drawText之后不能直接进行TextOut，不知道为什么。


		//学员信息表格
		RectangleEx(22, 85, tableR, 135);
		LineEx(22, 99, tableR, 99, 1, 9, 4); //画4条横线
		float tableW = (tableR - 22) / 7; //表格列宽
		LineEx(22 + tableW, 99, 22 + tableW, 135, 2, tableW, 6); //画6条竖线

		TextEx(32, 89, m_sheetInfo->strMsg1);
		m_dcPrinter.SelectObject(&font_4mm);
		//col 1
		int yc1 = 22 + 5;
		TextEx(27, 101, "姓 名");

		//col 2
		int yc2 = 22 + tableW + 3;
		TextEx(yc2, 101, "约定日期");

		//col 3
		int yc3 = 22 + 2*tableW + 6;
		TextEx(yc3, 101, "时 段");

		//col 4
		int yc4 = 22 + 3 * tableW + 1;
		TextEx(yc4, 101, "用时、进度");


		//col 5
		int yc5 = 22 + 4 * tableW + 4;
		TextEx(yc5, 101, "服务评价");
		LineEx(yc5+1, 115, yc5+11, 115, 1, 9, 3); //画3条横线
		TextEx(yc5 + 11, 111, "分");
		TextEx(yc5 + 11, 120, "分");
		TextEx(yc5 + 11, 129, "分");

		//col 6
		int yc6 = 22 + 5 * tableW + 4;
		TextEx(yc6, 101, "自我评价");
		LineEx(yc6+1, 115, yc6 + 15, 115, 1, 9, 3); //画3条横线

		//课程内容表格
		RectangleEx(22, 155, tableR, 241);
		LineEx(22, 169, tableR, 169, 1, 9, 8); //画8条横线

		m_dcPrinter.SelectObject(&font_7mm);
		TextEx(24, 159, "授教内容：");
		TextEx(tableR - 57, H - 34, m_sheetInfo->strDate);
		m_dcPrinter.SelectObject(&font_8mm);
		TextEx(32, 250, "辛苦了！ 谢谢配合！");


		//输出动态信息：学员信息、授教内容
		m_dcPrinter.SelectObject(&font_4mm);
		if (m_students.size() == 0)
		{
			m_classInfo->nClassID = 0;
			m_classInfo->arrClassText.clear();
		}
		else if (m_students.size() > 0)
		{
			for (int i = 0; i < m_students.size(); i++)
			{
				//姓名
				TextEx(26, 110 + 9 * i, m_students[i].strName);
				//约定日期
				TextEx(yc2, 110 + 9 * i, m_students[i].strDate);
				//时段
				m_dcPrinter.SelectObject(&font_3mm);
				TextEx(yc3 - 4, 111 + 9 * i, GetClassTime(m_students[i].nClassTime));
				m_dcPrinter.SelectObject(&font_4mm);
				//用时进度
				FillPieEx(RGB(0, 0, 0), CRect(yc4, 109 + 9 * i, yc4 + 7, 116 + 9 * i), &m_students[i]);
			}

			//授课内容
			int nText = m_classInfo->arrClassText.size();
			if (nText == 0) //初始化 从配置文件读取授课内容
			{
				if (m_classInfo->nClassID > 0)
				{
					CString strClassID;
					strClassID.Format("CLASS%d", m_classInfo->nClassID);
					int nItem = xPublic::GETINT2(strClassID, "nItem", 0);
					m_sheetInfo->strTitle = xPublic::GETSTR2(strClassID, "title", "未定义");
					CString strItemID;
					for (int i = 1; i <= nItem; i++)
					{
						strItemID.Format("item%d", i);
						CString strClassText = xPublic::GETSTR2(strClassID, strItemID, "");
						m_classInfo->arrClassText.push_back(strClassText);
					}
					nText = nItem;
				}
			}
			if (nText > 0)
			{
				m_dcPrinter.SelectObject(&font_6mm);
				for (int i = 0; i< nText; i++)
				{
					TextEx(24, 170 + 9 * i, m_classInfo->arrClassText[i]);
				}
			}
		}

		//表头信息
		m_dcPrinter.SelectObject(&font_13mm);
		TextEx(44, 52, m_sheetInfo->strTitle);

		// 恢复以前的画笔
		m_dcPrinter.SelectObject(font);
		m_dcPrinter.SelectObject(pOldPen);
	}


	void CMyPrint::DrawTrainSheet()
	{
		CPen pen, *pOldPen;// 定义笔对象和指针  
		pen.CreatePen(PS_SOLID, m_printerInfo.fmmDpiy*0.5, RGB(0, 0, 0));
		pOldPen = m_dcPrinter.SelectObject(&pen);

		CFont font_3mm, font_4mm, font_6mm, font_7mm, font_8mm, font_13mm; //从小到大
		font_3mm.CreateFontA(m_printerInfo.fmmDpiy * 3, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度3mm
		font_4mm.CreateFontA(m_printerInfo.fmmDpiy * 4, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度4mm
		font_6mm.CreateFontA(m_printerInfo.fmmDpiy * 6, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度6mm
		font_7mm.CreateFontA(m_printerInfo.fmmDpiy * 7, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度7mm
		font_8mm.CreateFontA(m_printerInfo.fmmDpiy * 8, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度8mm
		font_13mm.CreateFontA(m_printerInfo.fmmDpiy * 13, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度13mm
		m_dcPrinter.SetBkMode(TRANSPARENT); //文字背景透明

		//绘图
		float W = m_printerInfo.fPrinterScreenx;
		float H = m_printerInfo.fPrinterScreeny;
		int tableR = W - 2; //打印单内表格的右横坐标
		//RectangleEx(20, 45, W - 20, H - 25);// 画表格外框
		RectangleEx(0, 0, W, H);


		CFont *font = m_dcPrinter.SelectObject(&font_8mm);
		TextEx(tableR - 55, 15, "车辆编号：" + m_sheetInfo->strCarID);

		m_dcPrinter.SelectObject(&font_6mm);
		TextEx(tableR - 95, 32, m_sheetInfo->strClassType + "教练员：" + m_sheetInfo->strCoach
			+ "  工号：" + m_sheetInfo->strCoachID);


		m_dcPrinter.SelectObject(&font_7mm);
		CRect rect(CPoint(2, 93), CPoint(tableR, 108));
		DrawTextExx(m_sheetInfo->strMsg2, rect); //drawText之后不能直接进行TextOut，不知道为什么。


		//学员信息表格
		RectangleEx(2, 40, tableR, 90);
		LineEx(2, 54, tableR, 54, 1, 9, 4); //画4条横线
		float tableW = (tableR - 2) / 7; //表格列宽
		LineEx(2 + tableW, 54, 2 + tableW, 90, 2, tableW, 6); //画6条竖线

		TextEx(4, 44, m_sheetInfo->strMsg1);
		m_dcPrinter.SelectObject(&font_4mm);
		//col 1
		int yc1 = 2 + 5;
		TextEx(7, 56, "姓 名");

		//col 2
		int yc2 = 2 + tableW + 3;
		TextEx(yc2, 56, "约定日期");

		//col 3
		int yc3 = 2 + 2 * tableW + 6;
		TextEx(yc3, 56, "时 段");

		//col 4
		int yc4 = 2 + 3 * tableW + 1;
		TextEx(yc4, 56, "用时、进度");


		//col 5
		int yc5 = 2 + 4 * tableW + 4;
		TextEx(yc5, 56, "服务评价");
		LineEx(yc5 + 1, 70, yc5 + 11, 70, 1, 9, 3); //画3条横线
		TextEx(yc5 + 11, 66, "分");
		TextEx(yc5 + 11, 75, "分");
		TextEx(yc5 + 11, 84, "分");

		//col 6
		int yc6 = 2 + 5 * tableW + 4;
		TextEx(yc6, 56, "自我评价");
		LineEx(yc6 + 1, 70, yc6 + 15, 70, 1, 9, 3); //画3条横线

		//课程内容表格
		RectangleEx(2, 110, tableR, 196);
		LineEx(2, 124, tableR, 124, 1, 9, 8); //画8条横线

		m_dcPrinter.SelectObject(&font_7mm);
		TextEx(4, 114, "授教内容：");
		TextEx(tableR - 58, H - 9, m_sheetInfo->strDate);
		m_dcPrinter.SelectObject(&font_8mm);
		TextEx(2, 205, "辛苦了！ 谢谢配合！");

		//输出动态信息：学员信息、授教内容
		m_dcPrinter.SelectObject(&font_4mm);
		if (m_students.size() == 0)
		{
			m_classInfo->nClassID = 0;
			m_classInfo->arrClassText.clear();
		}
		else if (m_students.size() > 0)
		{
			//学生信息
			for (int i = 0; i < m_students.size(); i++)
			{
				//姓名
				TextEx(6, 65 + 9 * i, m_students[i].strName);
				//约定日期
				TextEx(yc2-1, 65 + 9 * i, m_students[i].strDate);
				//时段
				m_dcPrinter.SelectObject(&font_3mm);
				TextEx(yc3 - 5, 66 + 9 * i, GetClassTime(m_students[i].nClassTime));
				m_dcPrinter.SelectObject(&font_4mm);
				//用时进度
				FillPieEx(RGB(0, 0, 0), CRect(yc4, 64 + 9 * i, yc4 + 7, 71 + 9 * i), &m_students[i]);
			}

			//授课内容
			int nText = m_classInfo->arrClassText.size();
			if (nText == 0) //初始化 从配置文件读取授课内容
			{
				if (m_classInfo->nClassID > 0)
				{
					CString strClassID;
					strClassID.Format("CLASS%d", m_classInfo->nClassID);
					int nItem = xPublic::GETINT2(strClassID, "nItem", 0);
					m_sheetInfo->strTitle = xPublic::GETSTR2(strClassID, "title", "未定义");
					CString strItemID;
					for (int i = 1; i <= nItem; i++)
					{
						strItemID.Format("item%d", i);
						CString strClassText = xPublic::GETSTR2(strClassID, strItemID, "");
						m_classInfo->arrClassText.push_back(strClassText);
					}
					nText = nItem;
				}
			}
			if (nText > 0)
			{
				m_dcPrinter.SelectObject(&font_7mm);
				for (int i = 0; i< nText; i++)
				{
					TextEx(4, 125 + 9 * i, m_classInfo->arrClassText[i]);
				}
			}
		}

		//表头信息
		m_dcPrinter.SelectObject(&font_13mm);
		TextEx(24, 7, m_sheetInfo->strTitle);

		// 恢复以前的画笔
		m_dcPrinter.SelectObject(font);
		m_dcPrinter.SelectObject(pOldPen);
	}

	void CMyPrint::PrintTrainSheet2(int page)
	{
		CPen pen, *pOldPen;// 定义笔对象和指针  
		pen.CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
		pOldPen = m_dcPrinter.SelectObject(&pen);// 选入绿色笔  
		RectangleEx(0, 0, m_printerInfo.fPrinterScreenx, m_printerInfo.fPrinterScreeny);// 画矩形 

		m_dcPrinter.SelectObject(pOldPen);// 选出绿色笔  
		pen.DeleteObject();// 删除绿色笔
	}

	void CMyPrint::RectangleEx(int x1, int y1, int x2, int y2)
	{
		m_dcPrinter.Rectangle(x1 * m_printerInfo.fmmDpix, y1 * m_printerInfo.fmmDpiy, 
			x2 * m_printerInfo.fmmDpix, y2 * m_printerInfo.fmmDpiy);
	}

	void CMyPrint::LineEx(int x1, int y1, int x2, int y2, int arrayFlag, int arraySpace, int arrayNum)
	{
		for (int i = 0; i < arrayNum; i++)
		{
			int x11 = (x1 + (arrayFlag&2? 1:0)*arraySpace*i) * m_printerInfo.fmmDpix;
			int y11 = (y1 + (arrayFlag&1? 1:0)*arraySpace*i) * m_printerInfo.fmmDpiy;
			int x21 = (x2 + (arrayFlag&2? 1:0)*arraySpace*i) * m_printerInfo.fmmDpix;
			int y21 = (y2 + (arrayFlag&1? 1:0)*arraySpace*i) * m_printerInfo.fmmDpiy;

			m_dcPrinter.MoveTo(x11, y11);
			m_dcPrinter.LineTo(x21, y21);
		}
	}

	void CMyPrint::TextEx(int x, int y, CString str)
	{
		m_dcPrinter.TextOutA(x * m_printerInfo.fmmDpix, y * m_printerInfo.fmmDpiy, str);
	}

	void CMyPrint::DrawTextExx(CString str, CRect& rect)
	{
		rect.top *= m_printerInfo.fmmDpiy;
		rect.bottom *= m_printerInfo.fmmDpiy;
		rect.left *= m_printerInfo.fmmDpix;
		rect.right *= m_printerInfo.fmmDpix;
		m_dcPrinter.DrawText(str, rect, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	}

	void CMyPrint::FillPieEx(COLORREF color, CRect& rect, STUDENTINFO* student)
	{
		int nUsed = student->nClassStep;
		int nTotal = student->nClassTotal;


		//输出进度信息 //因为要用到TextEx，所以在坐标变换前输出
		CFont font_2mm;
		font_2mm.CreateFontA(m_printerInfo.fmmDpiy * 2, 0, 0, 0, FW_ULTRALIGHT, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "宋体"); //字体高度2mm
		CString str1, str2, str3;
		str1.Format("您已用去%02d课时", nUsed);
		str2.Format("余%02d课时！", nTotal - nUsed);
		str3.Format("有效期限%03d天！", 230);
		CFont* pOldFont = m_dcPrinter.SelectObject(&font_2mm);
		TextEx(rect.right, rect.top, str1);
		TextEx(rect.right + 5, rect.top + 2, str2);
		TextEx(rect.right, rect.top + 5, str3);

		//mm换成像素单位
		rect.top *= m_printerInfo.fmmDpiy;
		rect.bottom *= m_printerInfo.fmmDpiy;
		rect.left *= m_printerInfo.fmmDpix;
		rect.right *= m_printerInfo.fmmDpix;

		//画笔画刷准备
		CBrush brush(color);
		CPen pen(PS_SOLID, 1, color);

		//画图
		CPen* pOldPen = m_dcPrinter.SelectObject(&pen);
		m_dcPrinter.Ellipse(rect);//先画一个空心圆
		//将圆分为nTotal份		
		CPoint o = rect.CenterPoint();//圆心坐标
		CPoint p(o.x, rect.top); //线段的终点起始位置, 线段起点为圆心
		CPoint t(o.x, rect.top); //保存nUsed位置的线段终点坐标
		float A = 2 * M_PI / nTotal; //每个扇形的角度
		for (int i = 0; i <= nTotal; i++)
		{
			//计算旋转后的坐标
			float xx = cos(A*i) * (p.x - o.x) - sin(A*i) * (p.y - o.y) + o.x;
			float yy = sin(A*i) * (p.x - o.x) + cos(A*i) * (p.y - o.y) + o.y;

			m_dcPrinter.MoveTo(o);
			m_dcPrinter.LineTo(xx, yy);

			if (i == nUsed)
			{
				t.x = xx;
				t.y = yy;
			}
		}
		//填充已完成的扇形
		CBrush* pOldBrush = m_dcPrinter.SelectObject(&brush);
		if (nUsed!=0)
			m_dcPrinter.Pie(rect, t, p);


		//画笔画刷恢复
		m_dcPrinter.SelectObject(pOldFont);
		m_dcPrinter.SelectObject(pOldBrush);
		m_dcPrinter.SelectObject(pOldPen);
	}

	void CMyPrint::AddStudent(STUDENTINFO student)
	{
		m_students.push_back(student);
	}

	void CMyPrint::RemoveStudentAt(int index)
	{
		std::vector<STUDENTINFO>::iterator it = m_students.begin() + index;
		m_students.erase(it);
	}

	void CMyPrint::Reset()
	{
		m_students.clear();

		if (m_classInfo != NULL)
		{
			m_classInfo->arrClassText.clear();
			m_classInfo->nClassID = 0;
		}

		if (m_sheetInfo != NULL)
		{
			m_sheetInfo->strTitle = "未定义";
			m_sheetInfo->strCarID = "---";
			m_sheetInfo->strCoach = "---";
			m_sheetInfo->strCoachID = "---";
		}
	}
}//xPublic