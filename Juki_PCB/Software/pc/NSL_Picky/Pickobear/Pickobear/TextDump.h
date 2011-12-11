#pragma once
#include "afxwin.h"

class CPickobearDlg;

// CTextDump dialog

class CTextDump : public CDialogEx
{
	DECLARE_DYNAMIC(CTextDump)

public:
	CTextDump(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTextDump();

	void Print( CString text ) {

		int in = m_TextOutput.AddString( text );
		
		int nCount = m_TextOutput.GetCount();
		
		if (nCount > 0) {
		   m_TextOutput.SetCurSel(nCount-1);
		}
	};

// Dialog Data
	enum { IDD = IDD_TEXT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_InputBuffer;
	CListBox m_TextOutput;
	afx_msg void OnBnClickedSend();

	CPickobearDlg *pDlg;
	afx_msg void OnBnClickedSaveText();
	afx_msg void OnBnClickedClear();
};
