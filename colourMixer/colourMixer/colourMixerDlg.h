
// colourMixerDlg.h : header file
//

#pragma once
#include "afxcolorbutton.h"
#include "afxcmn.h"


// CcolourMixerDlg dialog
class CcolourMixerDlg : public CDialogEx
{
// Construction
public:
	CcolourMixerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COLOURMIXER_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	CMFCColorButton m_CButtonB;
	CSliderCtrl m_MixAmount_A;
};
