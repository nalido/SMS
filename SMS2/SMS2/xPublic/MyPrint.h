#pragma once
#include "..\\stdafx.h"
#include <vector>
#include "MySQLEx.h"

namespace xPublic{
	typedef struct struct_PRINTERINFO //��ӡ��������Ϣ��ֽ����Ϣ
	{
		float fmmDpix; //��ӡ��ÿ������ռ���豸����
		float fmmDpiy;
		float fPrinterScreenx;//����õ���ӡ����Ļ�Ŀ�ߣ�Ҳ����ֽ�ŵĴ�С�����ʵ��СԼ6mm
		float fPrinterScreeny;

		int nPagebegin;//��ӡ��Χ
		int nPageend;
		int nCopy; //��ӡ����

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

	typedef struct struct_STUDENTINFO //ѧԱ��Ϣ
	{
		CString strName; //ѧԱ����
		CString strDate; //Լ��ѵ������

		int nClassTime; //�Ͽ�ʱ�� 0Ϊam8:00-10:00 һ�ڿ���Сʱ,�Դ�����
		int nClassStep; //�γ̽���,���ÿ�ʱ
		int nClassTotal; //�ܿ�ʱ

		struct_STUDENTINFO()
		{
			strName = "δ����";
			strDate = "δ����";
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

	typedef struct struct_CLASSINFO //�γ�����
	{
		int nClassID; //��ʱ���
		CStrs arrClassText; //��ʱ�ڿ�����

		struct_CLASSINFO()
		{
			nClassID = 0;
			arrClassText.clear();
		}
	}CLASSINFO;

	typedef struct struct_SHEETINFO //����Ϣ
	{
		CString strTitle; //������
		CString strClassType; //�γ���� ��һ���ƶ�
		CString strCoach; //����Ա����
		CString strCarID; //������� �̶�3λ������
		CString	strCoachID; //��������
		CString strMsg1; //���̶���ʾ����
		CString strMsg2;
		CString strMsg3;
		CString strData; //��ʱ��


		struct_SHEETINFO()
		{
			strTitle = "δ����";
			strClassType = "��һ";
			strCoach = "δ����";
			strCarID = "---";
			strCoachID = "δ����";
			strMsg1 = "�ž�ªϰ\"���ÿ�Ҫ\"��ά��Ȩ���������";
			strMsg2 = "   �ر���ʾ�����ݹ涨����Ч���ڣ�����ѵ����30����ʱ�ģ�ÿ1��ʱ�貹��50Ԫ��";
			strMsg3 = "�����ˣ� лл��ϣ�";
			strData = "2017��03��27����";
		}
	}SHEETINFO;

	class CMyPrint
	{
	public:
		CMyPrint();
		~CMyPrint();

		HDC m_hdcPrinter; //��ӡ���豸���
		CDC m_dcPrinter; //����ؼ����豸����
		BOOL m_isInit; //�豸�Ƿ��ʼ�����
		int m_nMaxPage; //����ӡҳ�뷶Χ
		PRINTERINFO m_printerInfo;
		SHEETINFO* m_sheetInfo;
		//STUDENTINFO* m_studentInfo;
		std::vector<STUDENTINFO> m_students;
		CLASSINFO* m_classInfo;

		//���ѧԱ��Ϣ
		void AddStudent(STUDENTINFO student);
		//�Ƴ�ѧԱ��Ϣ
		void RemoveStudentAt(int index);
		CString GetClassTime(int n);
		//���ñ������ѧԱ��Ϣ
		void Reset();

		//��ʼ���豸������ֽ�Ÿ�ʽ��
		void PrinterInit(SHEETINFO* SheetInfo, CLASSINFO* ClassInfo); //ֻ��ӡһ�ŵ���
		//��ӡ�ӿڣ�����type��ӡ��ͬ��ʽ�ı��
		void Printx(int type, CString& strMsg);
		//��ӡ���ű�����
		void PrintTrainSheet(int page);
		void PrintTrainSheet2(int page);
		void DrawTrainSheet();

		//�Զ����ͼ ��λΪmm
		void RectangleEx(int x1, int y1, int x2, int y2);
		void LineEx(int x1, int y1, int x2, int y2, int arrayFlag = 0, int arraySpace = 0, int arrayNum = 1); //���б�־arrayFlag�� 0�����У�1��ֱ�������У�2ˮƽ�������С� 
		void TextEx(int x, int y, CString str);
		void DrawTextExx(CString str, CRect& rect); //�Զ����е��������
		void FillPieEx(COLORREF color, CRect& rect, STUDENTINFO* student); //������Σ��������ΪnUsed/nTotal

	};
}//xPublic