// SMS2View.cpp : implementation of the CSMS2View class
//

#include "stdafx.h"
#include "SMS2.h"

#include "SMS2Doc.h"
#include "SMS2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMS2View

IMPLEMENT_DYNCREATE(CSMS2View, CBCGPFormView)

BEGIN_MESSAGE_MAP(CSMS2View, CBCGPFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CSMS2View construction/destruction

CSMS2View::CSMS2View()
	: CBCGPFormView(CSMS2View::IDD)
{

	EnableVisualManagerStyle();

	// TODO: add construction code here

}

CSMS2View::~CSMS2View()
{
}

void CSMS2View::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BOOL CSMS2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CBCGPFormView::PreCreateWindow(cs);
}

void CSMS2View::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();

}


// CSMS2View diagnostics

#ifdef _DEBUG
void CSMS2View::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CSMS2View::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CSMS2Doc* CSMS2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMS2Doc)));
	return (CSMS2Doc*)m_pDocument;
}
#endif //_DEBUG


// CSMS2View message handlers


void CSMS2View::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
}
