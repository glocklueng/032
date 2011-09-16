
// PickobearDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "Serial.h"

// CPickobearDlg dialog
class CPickobearDlg : public CDialog
{
private:
	// each of the camera windows
	COpenGLControl m_oglWindow;
	COpenGLControl m_oglWindow1;
	
	CSerial m_Serial;

// Construction
public:
	CPickobearDlg(CWnd* pParent = NULL);	// standard constructor

	~CPickobearDlg(){

		m_Serial.Close();
	}

	void SendCommand(const char *cmd)
	{
		DWORD written;

		m_Serial.Write(cmd,&written,0,INFINITE);
	};

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
	double m_Threshold1;
	double m_Threshold2;
	afx_msg void OnEnChangeThreshold();
	afx_msg void OnEnChangeThreshold2();
	afx_msg void OnBnClickedHome();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedPark();
	afx_msg void OnBnClickedTool1();
	afx_msg void OnBnClickedTool2();
	afx_msg void OnBnClickedTool3();
	afx_msg void OnBnClickedTool4();
	afx_msg void OnBnClickedTool5();
	afx_msg void OnBnClickedTool6();
};
