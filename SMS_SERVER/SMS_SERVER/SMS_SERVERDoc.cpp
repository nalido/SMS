// SMS_SERVERDoc.cpp : implementation of the CSMS_SERVERDoc class
//

#include "stdafx.h"
#include "SMS_SERVER.h"

#include "SMS_SERVERDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMS_SERVERDoc

IMPLEMENT_DYNCREATE(CSMS_SERVERDoc, CDocument)

BEGIN_MESSAGE_MAP(CSMS_SERVERDoc, CDocument)
END_MESSAGE_MAP()


// CSMS_SERVERDoc construction/destruction

CSMS_SERVERDoc::CSMS_SERVERDoc()
{
	// TODO: add one-time construction code here

}

CSMS_SERVERDoc::~CSMS_SERVERDoc()
{
}

BOOL CSMS_SERVERDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSMS_SERVERDoc serialization

void CSMS_SERVERDoc::Serialize(CArchive& ar)
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


// CSMS_SERVERDoc diagnostics

#ifdef _DEBUG
void CSMS_SERVERDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSMS_SERVERDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSMS_SERVERDoc commands
