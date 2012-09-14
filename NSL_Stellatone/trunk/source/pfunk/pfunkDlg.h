
// pfunkDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "vmrotaryfaderctrl.h"


// CpfunkDlg dialog
class CpfunkDlg : public CDialogEx
{
// Construction
public:
	CpfunkDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PFUNK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	CIsiLcdDisplay m_LCDPanelA;
	CIsiLcdDisplay m_LCDPanelB;
	CIsiLcdDisplay m_LCDPanelC;
	CIsiLcdDisplay m_LCDPanelD;
	CVMRotaryFaderCtrl  m_Encoder1;
	CButton m_Eng;
	CButton m_Osc;
	CButton m_Env;
	CButton m_Mtx;
	CButton m_LFo;
	CButton m_Back;
	CButton m_Menu;
};
