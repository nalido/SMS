//Common.cpp
#include "stdafx.h"
#include "Common.h"
#include <afxglobals.h>

namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
static void App_SetFont(void)
{
	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.fontRegular.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("����"));
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);

	//��������Ӵ�
	afxGlobalData.fontBold.GetLogFont(&lf);
	afxGlobalData.fontBold.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("����"));
	afxGlobalData.fontBold.CreateFontIndirect(&lf);

	//������ʾ����
	afxGlobalData.fontTooltip.GetLogFont(&lf);
	afxGlobalData.fontTooltip.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("����"));
	afxGlobalData.fontTooltip.CreateFontIndirect(&lf);
}

static CString	s_sAppIni;	//���������ļ�ȫ·��
static CString	s_sAppPath;	//������·��
/********************************************************************
��������: ��ȡ����Ĺ���·��
�������: LPCTSTR lpszIniName�������ini�ļ�·��
�������: ��
�� �� ֵ: ��
********************************************************************/
CString GetWorkPath(LPCTSTR lpszAppName)
{
	//App_SetFont();
	TCHAR szWorkPath[MAX_PATH] = {};
	::GetModuleFileName(NULL,szWorkPath,MAX_PATH);
	CString strTmp = szWorkPath;
	s_sAppPath = strTmp.Left(strTmp.ReverseFind(_T('\\')));
	s_sAppIni.Format(_T("%s\\%s.ini"), s_sAppPath,lpszAppName);
	return s_sAppPath;
}
/********************************************************************
��������: ���س���Ĺ���·��
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
CString GetAppPath(void)
{
	return s_sAppPath;
}

/********************************************************************
��������: ��Ini�����ļ��ж�ȡ�ַ���
�������: LPCTSTR lpAppName��������
		  LPCTSTR lpKeyName���ؼ���
		  LPCTSTR lpDefault��Ĭ��ֵ
�������: ��
�� �� ֵ: ��Ini��ȡ��ʵ��ֵ�ַ��������ini�����ڻ���iniû�и�����Ĭ��ֵ����
********************************************************************/
CString GETSTR2(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault)
{
	CString strReturn;
	::GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, strReturn.GetBuffer(MAX_PATH), MAX_PATH, s_sAppIni);
	strReturn.ReleaseBuffer();
	return strReturn;
}


/********************************************************************
��������: ��Ini�����ļ��ж�ȡ��ֵ
�������: LPCTSTR lpAppName��������
		  LPCTSTR lpKeyName���ؼ���
		  LPCTSTR lpDefault��Ĭ��ֵ
�������: ��
�� �� ֵ: ��Ini��ȡ��ʵ����ֵ(UINT)�����ini�����ڻ���iniû�и�����Ĭ��ֵ����
********************************************************************/
UINT GETINT2(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault)
{
	return ::GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, s_sAppIni);
}

/********************************************************************
��������: д�ַ�����Ini�����ļ�
�������: LPCTSTR lpAppName��������
		  LPCTSTR lpKeyName���ؼ���
		  LPCTSTR lpString ��ֵ
�������: ��
�� �� ֵ: ��
********************************************************************/
void WRISTR2(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	::WritePrivateProfileString(lpAppName, lpKeyName, lpString, s_sAppIni);
}

/********************************************************************
��������: д����ֵ��Ini�����ļ�
�������: LPCTSTR lpAppName��������
		  LPCTSTR lpKeyName���ؼ���
		  UINT nValue      ��ֵ
�������: ��
�� �� ֵ: ��
********************************************************************/
void WRIINT2(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT nValue)
{
	CString sValue;
	sValue.Format(_T("%d"), nValue);
	WRISTR2(lpAppName, lpKeyName, sValue);
}
//================================================================================================
//================================================================================================

/********************************************************************
��������: �ַ���16�������Ϊ�ַ���
�������: BYTE *inBuffer���ַ�����
		  int inLength  ���ַ����鳤��
�������: ��
�� �� ֵ: �ַ���
********************************************************************/
CStringA Hex2STR(const BYTE *inBuffer, int inLength)
{
	CStringA strMsg,strTmp;
	for ( int i=0; i<inLength; i++ )
	{
		strTmp.Format("%02X ", inBuffer[i]);
		strMsg += strTmp;
	}
	return strMsg;
}
//================================================================================================
//================================================================================================

/********************************************************************
��������: д��Ϣ��¼�ļ����ļ��������������֣�ÿ����Ϣ����ʱ�������
		  ����ļ����������Զ������������뱣֤���ļ�Ŀ¼����
�������: LPCTSTR lpFileName���ļ���
		  char *pszMsg      ����Ϣ
�������: ��
�� �� ֵ: ��
********************************************************************/
void WriteFileDebug(LPCTSTR lpFileName, const char *pszMsg)
{
	CStringA strMsg;
	SYSTEMTIME t; 
	::GetLocalTime(&t);
	strMsg.Format("%02d:%02d:%02d.%03d %s\r\n", t.wHour,t.wMinute,t.wSecond,t.wMilliseconds,pszMsg);

	CString sFileName;
	sFileName.Format(_T("%s\\logs(Debug)\\%s(%04d-%02d-%02d).txt"), GetAppPath(),lpFileName,t.wYear,t.wMonth,t.wDay);
	CFile cf;
	if ( cf.Open(sFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyNone|CFile::typeBinary) )
	{
		cf.SeekToEnd();
		cf.Write(strMsg, strMsg.GetLength());
		cf.Close();
	}
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace
