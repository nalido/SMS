// SMS2Doc.h : interface of the CSMS2Doc class
//


#pragma once


class CSMS2Doc : public CDocument
{
protected: // create from serialization only
	CSMS2Doc();
	DECLARE_DYNCREATE(CSMS2Doc)

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
	virtual ~CSMS2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:

};


