// SMS_SERVERView.cpp : implementation of the CSMS_SERVERView class
//

#include "stdafx.h"
#include "SMS_SERVER.h"

#include "SMS_SERVERDoc.h"
#include "SMS_SERVERView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMS_SERVERView

IMPLEMENT_DYNCREATE(CSMS_SERVERView, CBCGPFormView)

BEGIN_MESSAGE_MAP(CSMS_SERVERView, CBCGPFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CSMS_SERVERView construction/destruction

CSMS_SERVERView::CSMS_SERVERView()
	: CBCGPFormView(CSMS_SERVERView::IDD)
{

	EnableVisualManagerStyle();

	// TODO: add construction code here

}

CSMS_SERVERView::~CSMS_SERVERView()
{
}

void CSMS_SERVERView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BOOL CSMS_SERVERView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CBCGPFormView::PreCreateWindow(cs);
}

void CSMS_SERVERView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CSMS_SERVERView diagnostics

#ifdef _DEBUG
void CSMS_SERVERView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CSMS_SERVERView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CSMS_SERVERDoc* CSMS_SERVERView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMS_SERVERDoc)));
	return (CSMS_SERVERDoc*)m_pDocument;
}
#endif //_DEBUG


// CSMS_SERVERView message handlers


void CSMS_SERVERView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
}
