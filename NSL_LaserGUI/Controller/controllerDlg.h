
// controllerDlg.h : header file
//

#pragma once


// CcontrollerDlg dialog
class CcontrollerDlg : public CDialogEx
{
// Construction
public:
	CcontrollerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONTROLLER_DIALOG };

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
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDelAll();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedUp2();
	afx_msg void OnBnClickedZUp();
	afx_msg void OnBnClickedZDown();
	afx_msg void OnBnClickedZap();
	afx_msg void OnBnClickedFire();
};
