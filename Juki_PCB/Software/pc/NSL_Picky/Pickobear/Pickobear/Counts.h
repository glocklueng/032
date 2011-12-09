#pragma once


// CCounts dialog

class CCounts : public CDialogEx
{
	DECLARE_DYNAMIC(CCounts)

public:
	CCounts(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCounts();

// Dialog Data
	enum { IDD = IDD_XYCOUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// count of components in tray x wise
	long m_CountX;
	long m_CountY;
};
