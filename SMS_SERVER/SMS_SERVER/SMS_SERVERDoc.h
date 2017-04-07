// SMS_SERVERDoc.h : interface of the CSMS_SERVERDoc class
//


#pragma once


class CSMS_SERVERDoc : public CDocument
{
protected: // create from serialization only
	CSMS_SERVERDoc();
	DECLARE_DYNCREATE(CSMS_SERVERDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSMS_SERVERDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


