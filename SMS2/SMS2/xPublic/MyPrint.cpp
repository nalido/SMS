#include "stdafx.h"
#include "MyPrint.h"
#include <math.h>

namespace xPublic{

	CString CMyPrint::GetClassTime(int n) //����ʱ�α�Ż�þ���ʱ��
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
			res = "δ����";
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
			m_dcPrinter.Detach();//�ͷ�DC
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

		//��ʼ����ӡ������ȡ��ӡ�����
		CPrintDialog dlg(FALSE, PD_ALLPAGES, NULL);//��ӡ�Ի�������壬����һ���õ����Ի���Ϊ��ӡ�Ի���,���������ô�ӡ��ΧΪȫ��ҳ�棬��������ָ����ӡ��������ȱʡ����

		/*��ӡ�Ի���ĳ�ʼֵ���ã���ʵ��Ӧ���пɸ�����Ҫ�������ã���ͨ��Ԥ�ȼ���õ�Ҫ��ӡ��ҳ�淶Χ��*/
		dlg.m_pd.nMinPage = 1;//ָ����ʼ/����ҳ��༭�ؼ���ҳ�뷶Χ����Сֵ����nMinPage=nMaxPage����ҳ�뷶Χ����ѡť�Ϳ�ʼ/����ҳ��༭�ؼ����һ�
		dlg.m_pd.nMaxPage = 1;//ָ����ʼ/����ҳ��༭�ؼ���ҳ�뷶Χ�����ֵ
		dlg.m_pd.nCopies = 1;//ָ�����������༭�ؼ��ĳ�ʼֵ
		dlg.m_pd.nFromPage = 1;//ָ����ʼҳ��༭�ؼ��ĳ�ʼֵ
		dlg.m_pd.nToPage = 2;//ָ������ҳ��༭�ؼ��ĳ�ʼֵ

		if (dlg.DoModal() == IDOK)
		{
			m_hdcPrinter = dlg.GetPrinterDC();
			if (m_hdcPrinter == NULL)//��ȡ�豸�����ľ��,�������������Ѳ��˳�
			{
				m_isInit = FALSE;
			}
			else
			{
				m_isInit = TRUE; //��ô�ӡ������ɹ������Դ�ӡ

				/*����ͼ���豸���������ӡ���豸�������������ͼ���൱������ӡ��Ҫ�����ֽ�ϻ�ͼ*/
				m_dcPrinter.Attach(m_hdcPrinter);

				/*
				*�ڴ�ӡ���ù�������Щ��������ϣ���û���ѡ������ʹ���˶Ի������ʽ��������Щ
				*��������ֽ�Ŵ�С�ʹ�ӡ���������ϣ�����ǹ̶��ģ��������û�������������鷳
				*��ˣ����������Ѿ��õ��Ĵ�ӡ��ͼ���豸������ͨ���޸�����ʵ�̶ֹ����õĹ���
				*/
				LPDEVMODE pDevMode;
				pDevMode = (LPDEVMODE)GlobalLock(dlg.m_pd.hDevMode);
				pDevMode->dmOrientation = DMORIENT_PORTRAIT;//���ô�ӡ����Ϊ����
				pDevMode->dmPaperSize = DMPAPER_A4;//����ֽ�Ŵ�СΪA4
				m_dcPrinter.ResetDC(pDevMode);
				
				/*��ӡ��ֽ�Ŵ�С��������Ҫ֪����,�������ǻ���Ҫ�豸���ص��ʵ�ʳ��ȵ�λ�Ļ����ϵ��DPI*/
				int xDPI = GetDeviceCaps(m_hdcPrinter, LOGPIXELSX);//����X������ÿӢ����豸��������DPI
				float mmdpix = xDPI / 25.4;//ÿ������ռ���豸����
				int yDPI = GetDeviceCaps(m_hdcPrinter, LOGPIXELSY);//����Y������ÿӢ����豸��������DPI
				float mmdpiy = yDPI / 25.4;//ÿ������ռ���豸����

				float printerscreenx, printerscreeny;//����õ���ӡ����Ļ�Ŀ�ߣ�Ҳ����ֽ�ŵĴ�С�����ʵ��СԼ6mm
				printerscreenx = GetDeviceCaps(m_hdcPrinter, HORZSIZE);
				printerscreeny = GetDeviceCaps(m_hdcPrinter, VERTSIZE);

				/*��ΪҪ��ӡ��ҳ�뷶Χ�ͷ������û�ѡ��ģ������ڴ˶��û�ѡ�����������*/
				int pagebegin, pageend;//��ӡ��Χ
				if (dlg.PrintAll())//���û��Ƿ�ѡ����ȫ����ӡ��һ��ǵĻ���ȫ����ӡ
				{
					pagebegin = dlg.m_pd.nMinPage;
					pageend = dlg.m_pd.nMaxPage;
				}
				else if (dlg.PrintRange())//�û�ѡ����ѡ��ҳ�뷶Χ��
				{
					pagebegin = dlg.m_pd.nFromPage;
					pageend = dlg.m_pd.nToPage;
				}
				else//�û�ѡ����ѡ����Χ��ӡ����Ϊ����������û���ṩѡ����Χ�Ĺ��ܣ������û�һ��ѡ�����ﵱ���ǲ�����������
				{
					//MessageBox("Could not choise this one");
					m_dcPrinter.AbortDoc();//�����˳�������EndDoc
				}
				int  ncopy = dlg.m_pd.nCopies;//�û�ѡ���ӡ����

				
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
			strMsg = "ȡ����ӡ";
			return;
		}

		//֪ͨ��ӡ������������մ�ӡ�ĵ�����ʼ��ӡ
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("CDC::StartDoc() Code Fragment");

		//�����ʼ��ʧ�������Ѳ��˳�
		if (m_dcPrinter.StartDoc(&docinfo) < 0)
		{
			strMsg = _T("Printer wouldn't initalize");
			return;
		}

		//��ʼ��ӡ����
		while (m_printerInfo.nCopy-->0)
		{
			for (int page = m_printerInfo.nPagebegin; page <= m_printerInfo.nPageend; page++)//��ѡ����Χ��ʼ��ӡ
			{
				if (m_dcPrinter.StartPage() < 0)
				{
					//MessageBox(_T("Could not start page"));
					m_dcPrinter.AbortDoc();//�����˳�������EndDoc
				}
				else//���������������Ҫ��ӡ���ݲ�������һҳ�Ĵ�ӡ
				{
					if (type==1)
						PrintTrainSheet(page);
					if (type == 2)
						PrintTrainSheet2(page);
					m_dcPrinter.EndPage();
				}
			}
		}
		m_dcPrinter.EndDoc();//��ӡ����˳�
		strMsg = "��ӡ�ɹ�";
	}

	void CMyPrint::PrintTrainSheet(int page)
	{
		CPen pen, *pOldPen;// ����ʶ����ָ��  
		pen.CreatePen(PS_SOLID, m_printerInfo.fmmDpiy*0.5, RGB(0, 0, 0));
		pOldPen = m_dcPrinter.SelectObject(&pen); 

		CFont font_3mm, font_4mm, font_6mm, font_7mm, font_8mm, font_13mm; //��С����
		font_3mm.CreateFontA(m_printerInfo.fmmDpiy * 3, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�3mm
		font_4mm.CreateFontA(m_printerInfo.fmmDpiy * 4, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�4mm
		font_6mm.CreateFontA(m_printerInfo.fmmDpiy * 6, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�6mm
		font_7mm.CreateFontA(m_printerInfo.fmmDpiy * 7, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�7mm
		font_8mm.CreateFontA(m_printerInfo.fmmDpiy * 8, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�8mm
		font_13mm.CreateFontA(m_printerInfo.fmmDpiy * 13, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�13mm
		m_dcPrinter.SetBkMode(TRANSPARENT); //���ֱ���͸��

		//��ͼ
		float W = m_printerInfo.fPrinterScreenx;
		float H = m_printerInfo.fPrinterScreeny;
		int tableR = W - 22; //��ӡ���ڱ����Һ�����
		RectangleEx(20, 45, W-20, H-25);// ��������


		CFont *font = m_dcPrinter.SelectObject(&font_8mm);
		TextEx(tableR-60, 60, "������ţ�"+m_sheetInfo->strCarID);

		m_dcPrinter.SelectObject(&font_6mm);
		TextEx(tableR-100, 77, m_sheetInfo->strClassType+"����Ա��" + m_sheetInfo->strCoach
			+ "  ���ţ�" + m_sheetInfo->strCoachID);


		m_dcPrinter.SelectObject(&font_7mm);
		CRect rect(CPoint(22, 138), CPoint(tableR,153));
		DrawTextExx(m_sheetInfo->strMsg2, rect); //drawText֮����ֱ�ӽ���TextOut����֪��Ϊʲô��


		//ѧԱ��Ϣ���
		RectangleEx(22, 85, tableR, 135);
		LineEx(22, 99, tableR, 99, 1, 9, 4); //��4������
		float tableW = (tableR - 22) / 7; //����п�
		LineEx(22 + tableW, 99, 22 + tableW, 135, 2, tableW, 6); //��6������

		TextEx(32, 89, m_sheetInfo->strMsg1);
		m_dcPrinter.SelectObject(&font_4mm);
		//col 1
		int yc1 = 22 + 5;
		TextEx(27, 101, "�� ��");

		//col 2
		int yc2 = 22 + tableW + 3;
		TextEx(yc2, 101, "Լ������");

		//col 3
		int yc3 = 22 + 2*tableW + 6;
		TextEx(yc3, 101, "ʱ ��");

		//col 4
		int yc4 = 22 + 3 * tableW + 1;
		TextEx(yc4, 101, "��ʱ������");


		//col 5
		int yc5 = 22 + 4 * tableW + 4;
		TextEx(yc5, 101, "��������");
		LineEx(yc5+1, 115, yc5+11, 115, 1, 9, 3); //��3������
		TextEx(yc5 + 11, 111, "��");
		TextEx(yc5 + 11, 120, "��");
		TextEx(yc5 + 11, 129, "��");

		//col 6
		int yc6 = 22 + 5 * tableW + 4;
		TextEx(yc6, 101, "��������");
		LineEx(yc6+1, 115, yc6 + 15, 115, 1, 9, 3); //��3������

		//�γ����ݱ��
		RectangleEx(22, 155, tableR, 241);
		LineEx(22, 169, tableR, 169, 1, 9, 8); //��8������

		m_dcPrinter.SelectObject(&font_7mm);
		TextEx(24, 159, "�ڽ����ݣ�");
		TextEx(tableR - 57, H - 34, m_sheetInfo->strDate);
		m_dcPrinter.SelectObject(&font_8mm);
		TextEx(32, 250, "�����ˣ� лл��ϣ�");


		//�����̬��Ϣ��ѧԱ��Ϣ���ڽ�����
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
				//����
				TextEx(26, 110 + 9 * i, m_students[i].strName);
				//Լ������
				TextEx(yc2, 110 + 9 * i, m_students[i].strDate);
				//ʱ��
				m_dcPrinter.SelectObject(&font_3mm);
				TextEx(yc3 - 4, 111 + 9 * i, GetClassTime(m_students[i].nClassTime));
				m_dcPrinter.SelectObject(&font_4mm);
				//��ʱ����
				FillPieEx(RGB(0, 0, 0), CRect(yc4, 109 + 9 * i, yc4 + 7, 116 + 9 * i), &m_students[i]);
			}

			//�ڿ�����
			int nText = m_classInfo->arrClassText.size();
			if (nText == 0) //��ʼ�� �������ļ���ȡ�ڿ�����
			{
				if (m_classInfo->nClassID > 0)
				{
					CString strClassID;
					strClassID.Format("CLASS%d", m_classInfo->nClassID);
					int nItem = xPublic::GETINT2(strClassID, "nItem", 0);
					m_sheetInfo->strTitle = xPublic::GETSTR2(strClassID, "title", "δ����");
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

		//��ͷ��Ϣ
		m_dcPrinter.SelectObject(&font_13mm);
		TextEx(44, 52, m_sheetInfo->strTitle);

		// �ָ���ǰ�Ļ���
		m_dcPrinter.SelectObject(font);
		m_dcPrinter.SelectObject(pOldPen);
	}


	void CMyPrint::DrawTrainSheet()
	{
		CPen pen, *pOldPen;// ����ʶ����ָ��  
		pen.CreatePen(PS_SOLID, m_printerInfo.fmmDpiy*0.5, RGB(0, 0, 0));
		pOldPen = m_dcPrinter.SelectObject(&pen);

		CFont font_3mm, font_4mm, font_6mm, font_7mm, font_8mm, font_13mm; //��С����
		font_3mm.CreateFontA(m_printerInfo.fmmDpiy * 3, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�3mm
		font_4mm.CreateFontA(m_printerInfo.fmmDpiy * 4, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�4mm
		font_6mm.CreateFontA(m_printerInfo.fmmDpiy * 6, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�6mm
		font_7mm.CreateFontA(m_printerInfo.fmmDpiy * 7, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�7mm
		font_8mm.CreateFontA(m_printerInfo.fmmDpiy * 8, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�8mm
		font_13mm.CreateFontA(m_printerInfo.fmmDpiy * 13, 0, 0, 0, FW_BLACK, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�13mm
		m_dcPrinter.SetBkMode(TRANSPARENT); //���ֱ���͸��

		//��ͼ
		float W = m_printerInfo.fPrinterScreenx;
		float H = m_printerInfo.fPrinterScreeny;
		int tableR = W - 2; //��ӡ���ڱ����Һ�����
		//RectangleEx(20, 45, W - 20, H - 25);// ��������
		RectangleEx(0, 0, W, H);


		CFont *font = m_dcPrinter.SelectObject(&font_8mm);
		TextEx(tableR - 55, 15, "������ţ�" + m_sheetInfo->strCarID);

		m_dcPrinter.SelectObject(&font_6mm);
		TextEx(tableR - 95, 32, m_sheetInfo->strClassType + "����Ա��" + m_sheetInfo->strCoach
			+ "  ���ţ�" + m_sheetInfo->strCoachID);


		m_dcPrinter.SelectObject(&font_7mm);
		CRect rect(CPoint(2, 93), CPoint(tableR, 108));
		DrawTextExx(m_sheetInfo->strMsg2, rect); //drawText֮����ֱ�ӽ���TextOut����֪��Ϊʲô��


		//ѧԱ��Ϣ���
		RectangleEx(2, 40, tableR, 90);
		LineEx(2, 54, tableR, 54, 1, 9, 4); //��4������
		float tableW = (tableR - 2) / 7; //����п�
		LineEx(2 + tableW, 54, 2 + tableW, 90, 2, tableW, 6); //��6������

		TextEx(4, 44, m_sheetInfo->strMsg1);
		m_dcPrinter.SelectObject(&font_4mm);
		//col 1
		int yc1 = 2 + 5;
		TextEx(7, 56, "�� ��");

		//col 2
		int yc2 = 2 + tableW + 3;
		TextEx(yc2, 56, "Լ������");

		//col 3
		int yc3 = 2 + 2 * tableW + 6;
		TextEx(yc3, 56, "ʱ ��");

		//col 4
		int yc4 = 2 + 3 * tableW + 1;
		TextEx(yc4, 56, "��ʱ������");


		//col 5
		int yc5 = 2 + 4 * tableW + 4;
		TextEx(yc5, 56, "��������");
		LineEx(yc5 + 1, 70, yc5 + 11, 70, 1, 9, 3); //��3������
		TextEx(yc5 + 11, 66, "��");
		TextEx(yc5 + 11, 75, "��");
		TextEx(yc5 + 11, 84, "��");

		//col 6
		int yc6 = 2 + 5 * tableW + 4;
		TextEx(yc6, 56, "��������");
		LineEx(yc6 + 1, 70, yc6 + 15, 70, 1, 9, 3); //��3������

		//�γ����ݱ��
		RectangleEx(2, 110, tableR, 196);
		LineEx(2, 124, tableR, 124, 1, 9, 8); //��8������

		m_dcPrinter.SelectObject(&font_7mm);
		TextEx(4, 114, "�ڽ����ݣ�");
		TextEx(tableR - 58, H - 9, m_sheetInfo->strDate);
		m_dcPrinter.SelectObject(&font_8mm);
		TextEx(2, 205, "�����ˣ� лл��ϣ�");

		//�����̬��Ϣ��ѧԱ��Ϣ���ڽ�����
		m_dcPrinter.SelectObject(&font_4mm);
		if (m_students.size() == 0)
		{
			m_classInfo->nClassID = 0;
			m_classInfo->arrClassText.clear();
		}
		else if (m_students.size() > 0)
		{
			//ѧ����Ϣ
			for (int i = 0; i < m_students.size(); i++)
			{
				//����
				TextEx(6, 65 + 9 * i, m_students[i].strName);
				//Լ������
				TextEx(yc2-1, 65 + 9 * i, m_students[i].strDate);
				//ʱ��
				m_dcPrinter.SelectObject(&font_3mm);
				TextEx(yc3 - 5, 66 + 9 * i, GetClassTime(m_students[i].nClassTime));
				m_dcPrinter.SelectObject(&font_4mm);
				//��ʱ����
				FillPieEx(RGB(0, 0, 0), CRect(yc4, 64 + 9 * i, yc4 + 7, 71 + 9 * i), &m_students[i]);
			}

			//�ڿ�����
			int nText = m_classInfo->arrClassText.size();
			if (nText == 0) //��ʼ�� �������ļ���ȡ�ڿ�����
			{
				if (m_classInfo->nClassID > 0)
				{
					CString strClassID;
					strClassID.Format("CLASS%d", m_classInfo->nClassID);
					int nItem = xPublic::GETINT2(strClassID, "nItem", 0);
					m_sheetInfo->strTitle = xPublic::GETSTR2(strClassID, "title", "δ����");
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

		//��ͷ��Ϣ
		m_dcPrinter.SelectObject(&font_13mm);
		TextEx(24, 7, m_sheetInfo->strTitle);

		// �ָ���ǰ�Ļ���
		m_dcPrinter.SelectObject(font);
		m_dcPrinter.SelectObject(pOldPen);
	}

	void CMyPrint::PrintTrainSheet2(int page)
	{
		CPen pen, *pOldPen;// ����ʶ����ָ��  
		pen.CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
		pOldPen = m_dcPrinter.SelectObject(&pen);// ѡ����ɫ��  
		RectangleEx(0, 0, m_printerInfo.fPrinterScreenx, m_printerInfo.fPrinterScreeny);// ������ 

		m_dcPrinter.SelectObject(pOldPen);// ѡ����ɫ��  
		pen.DeleteObject();// ɾ����ɫ��
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


		//���������Ϣ //��ΪҪ�õ�TextEx������������任ǰ���
		CFont font_2mm;
		font_2mm.CreateFontA(m_printerInfo.fmmDpiy * 2, 0, 0, 0, FW_ULTRALIGHT, 0, 0, 0, 0,
			0, 0, 0, VARIABLE_PITCH | FF_SWISS, "����"); //����߶�2mm
		CString str1, str2, str3;
		str1.Format("������ȥ%02d��ʱ", nUsed);
		str2.Format("��%02d��ʱ��", nTotal - nUsed);
		str3.Format("��Ч����%03d�죡", 230);
		CFont* pOldFont = m_dcPrinter.SelectObject(&font_2mm);
		TextEx(rect.right, rect.top, str1);
		TextEx(rect.right + 5, rect.top + 2, str2);
		TextEx(rect.right, rect.top + 5, str3);

		//mm�������ص�λ
		rect.top *= m_printerInfo.fmmDpiy;
		rect.bottom *= m_printerInfo.fmmDpiy;
		rect.left *= m_printerInfo.fmmDpix;
		rect.right *= m_printerInfo.fmmDpix;

		//���ʻ�ˢ׼��
		CBrush brush(color);
		CPen pen(PS_SOLID, 1, color);

		//��ͼ
		CPen* pOldPen = m_dcPrinter.SelectObject(&pen);
		m_dcPrinter.Ellipse(rect);//�Ȼ�һ������Բ
		//��Բ��ΪnTotal��		
		CPoint o = rect.CenterPoint();//Բ������
		CPoint p(o.x, rect.top); //�߶ε��յ���ʼλ��, �߶����ΪԲ��
		CPoint t(o.x, rect.top); //����nUsedλ�õ��߶��յ�����
		float A = 2 * M_PI / nTotal; //ÿ�����εĽǶ�
		for (int i = 0; i <= nTotal; i++)
		{
			//������ת�������
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
		//�������ɵ�����
		CBrush* pOldBrush = m_dcPrinter.SelectObject(&brush);
		if (nUsed!=0)
			m_dcPrinter.Pie(rect, t, p);


		//���ʻ�ˢ�ָ�
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
			m_sheetInfo->strTitle = "δ����";
			m_sheetInfo->strCarID = "---";
			m_sheetInfo->strCoach = "---";
			m_sheetInfo->strCoachID = "---";
		}
	}
}//xPublic