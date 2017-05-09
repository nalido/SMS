#pragma once
#include "afxwin.h"
#include "xPublic\MyPrint.h"
class CSheetCtrl :
	public CStatic
{
public:
	CSheetCtrl();
	~CSheetCtrl();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	xPublic::CMyPrint m_printData;
	xPublic::CLASSINFO m_classInfo;
	xPublic::SHEETINFO m_sheetInfo;
};

