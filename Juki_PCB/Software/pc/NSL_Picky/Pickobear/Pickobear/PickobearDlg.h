
// PickobearDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CPickobearDlg dialog
class CPickobearDlg : public CDialog
{
private:
		COpenGLControl m_oglWindow;
		COpenGLControl m_oglWindow1;

// Construction
public:
	CPickobearDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PICKOBEAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_Components;
	CButton GO;
	afx_msg void OnLbnSelchangeList1();
	long m_headXPos;
	long m_headYpos;
};
