#pragma once
#include "afxwin.h"


// CTextEditDialog dialog

class CTextEditDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTextEditDialog)

public:
	CTextEditDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTextEditDialog();

// Dialog Data
	enum { IDD = IDD_TEXTEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_Text;
};
