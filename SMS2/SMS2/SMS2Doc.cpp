// SMS2Doc.cpp : implementation of the CSMS2Doc class
//

#include "stdafx.h"
#include "SMS2.h"

#include "SMS2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMS2Doc

IMPLEMENT_DYNCREATE(CSMS2Doc, CDocument)

BEGIN_MESSAGE_MAP(CSMS2Doc, CDocument)
END_MESSAGE_MAP()


// CSMS2Doc construction/destruction

CSMS2Doc::CSMS2Doc()
{
	// TODO: add one-time construction code here

}

CSMS2Doc::~CSMS2Doc()
{
}

BOOL CSMS2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSMS2Doc serialization

void CSMS2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSMS2Doc diagnostics

#ifdef _DEBUG
void CSMS2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSMS2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSMS2Doc commands
