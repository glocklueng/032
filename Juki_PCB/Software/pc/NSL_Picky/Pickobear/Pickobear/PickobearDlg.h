
// PickobearDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "Serial.h"
#include "afxcmn.h"

class CListCtrl_Components : public CListCtrl
{
	DECLARE_MESSAGE_MAP();

	void PreSubclassWindow();

public:
	CListCtrl_Components()
	{}


	void AddItem( const char *label,const char *type,const char *x,const char *y,const char *rot)
	{
		CString mLabel(label);
		CString mType(type);
		CString mX(x);
		CString mY(y);
		CString mRot(rot);

		int Index = InsertItem(LVIF_TEXT, 0,mLabel, 0, 0, 0, NULL);
		SetItemText(Index,1,mType);
		SetItemText(Index,2,mX);
		SetItemText(Index,3,mY);
		SetItemText(Index,4,mRot);
	}
};


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
//	CListCtrl m_Components;
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
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedHead();
	afx_msg void OnNMCustomdrawRotate(NMHDR *pNMHDR, LRESULT *pResult);
	// rotation for head
	CSliderCtrl m_Rotation;
	CListCtrl_Components m_ComponentList;
	afx_msg void OnBnClickedUpleft();
	afx_msg void OnBnClickedUpright();
	afx_msg void OnBnClickedLeftdown();
	afx_msg void OnBnClickedBottomleft();
};
