
// NSL_BT_BlinkyBallDlg.h : header file
//

#pragma once


// CNSL_BT_BlinkyBallDlg dialog
class CNSL_BT_BlinkyBallDlg : public CDialogEx
{
// Construction
public:
	CNSL_BT_BlinkyBallDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NSL_BT_BLINKYBALL_DIALOG };

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
	afx_msg void OnBnClickedSend();
};
