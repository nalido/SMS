#pragma once
#include "..\\stdafx.h"
#include <vector>
#include "MySQLEx.h"

namespace xPublic{
	typedef struct struct_PRINTERINFO //打印机属性信息，纸张信息
	{
		float fmmDpix; //打印机每毫米所占的设备点数
		float fmmDpiy;
		float fPrinterScreenx;//这里得到打印机屏幕的宽高，也就是纸张的大小，会比实际小约6mm
		float fPrinterScreeny;

		int nPagebegin;//打印范围
		int nPageend;
		int nCopy; //打印份数

		struct_PRINTERINFO()
		{
			fmmDpix = 0;
			fmmDpiy = 0;
			fPrinterScreenx = 0;
			fPrinterScreeny = 0;

			nPagebegin = 1;
			nPageend = 1;
			nCopy = 1;
		}
	}PRINTERINFO;

	typedef struct struct_STUDENTINFO //学员信息
	{
		CString strName; //学员姓名
		CString strDate; //约定训练日期

		int nClassTime; //上课时段 0为am8:00-10:00 一节课两小时,以此类推
		int nClassStep; //课程进度,已用课时
		int nClassTotal; //总课时

		struct_STUDENTINFO()
		{
			strName = "未定义";
			strDate = "未定义";
			nClassTime = 0;
			nClassStep = 0;
			nClassTotal = 15;
		}
		struct_STUDENTINFO(CString name, CString date, int time, int step, int total)
		{
			strName = name;
			strDate = date;
			nClassTime = time;
			nClassStep = step;
			nClassTotal = total;
		}
	}STUDENTINFO;

	typedef struct struct_CLASSINFO //课程内容
	{
		int nClassID; //课时编号
		CStrs arrClassText; //课时授课内容

		struct_CLASSINFO()
		{
			nClassID = 0;
			arrClassText.clear();
		}
	}CLASSINFO;

	typedef struct struct_SHEETINFO //表单信息
	{
		CString strTitle; //表单名称
		CString strClassType; //课程类别 科一、科二
		CString strCoach; //教练员名字
		CString strCarID; //车辆编号 固定3位数长度
		CString	strCoachID; //教练工号
		CString strMsg1; //表单固定提示内容
		CString strMsg2;
		CString strMsg3;
		CString strData; //表单时间


		struct_SHEETINFO()
		{
			strTitle = "未定义";
			strClassType = "科一";
			strCoach = "未定义";
			strCarID = "---";
			strCoachID = "未定义";
			strMsg1 = "杜绝陋习\"吃拿卡要\"，维护权益从我做起！";
			strMsg2 = "   特别提示：根据规定在有效期内，凡授训超出30个课时的，每1课时需补缴50元！";
			strMsg3 = "辛苦了！ 谢谢配合！";
			strData = "2017年03月27日制";
		}
	}SHEETINFO;

	class CMyPrint
	{
	public:
		CMyPrint();
		~CMyPrint();

		HDC m_hdcPrinter; //打印机设备句柄
		CDC m_dcPrinter; //程序控件的设备环境
		BOOL m_isInit; //设备是否初始化完成
		int m_nMaxPage; //最大打印页码范围
		PRINTERINFO m_printerInfo;
		SHEETINFO* m_sheetInfo;
		//STUDENTINFO* m_studentInfo;
		std::vector<STUDENTINFO> m_students;
		CLASSINFO* m_classInfo;

		//添加学员信息
		void AddStudent(STUDENTINFO student);
		//移除学员信息
		void RemoveStudentAt(int index);
		CString GetClassTime(int n);
		//重置表单，清空学员信息
		void Reset();

		//初始化设备，设置纸张格式等
		void PrinterInit(SHEETINFO* SheetInfo, CLASSINFO* ClassInfo); //只打印一张单子
		//打印接口，根据type打印不同格式的表格
		void Printx(int type, CString& strMsg);
		//打印单张表单内容
		void PrintTrainSheet(int page);
		void PrintTrainSheet2(int page);
		void DrawTrainSheet();

		//自定义绘图 单位为mm
		void RectangleEx(int x1, int y1, int x2, int y2);
		void LineEx(int x1, int y1, int x2, int y2, int arrayFlag = 0, int arraySpace = 0, int arrayNum = 1); //阵列标志arrayFlag： 0无阵列，1竖直向下阵列，2水平向右阵列。 
		void TextEx(int x, int y, CString str);
		void DrawTextExx(CString str, CRect& rect); //自动分行的文字输出
		void FillPieEx(COLORREF color, CRect& rect, STUDENTINFO* student); //填充扇形，扇形面积为nUsed/nTotal

	};
}//xPublic