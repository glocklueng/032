#pragma once


// CFeederSetup dialog

class CFeederSetup : public CDialog
{
	DECLARE_DYNAMIC(CFeederSetup)

public:
	CFeederSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFeederSetup();

// Dialog Data
	enum { IDD = IDD_FEEDERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
