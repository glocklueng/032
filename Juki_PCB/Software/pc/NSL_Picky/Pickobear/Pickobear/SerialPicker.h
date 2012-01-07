#pragma once
#include "afxwin.h"

#include "enumser.h"

// CSerialPicker dialog

class CSerialPicker : public CDialogEx
{
	DECLARE_DYNAMIC(CSerialPicker)

public:
	CSerialPicker(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSerialPicker();

// Dialog Data
	enum { IDD = IDD_SERIAL_PICKER };

private:
		virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCbnSelchangeStepsize();

	DECLARE_MESSAGE_MAP()
public:
		
	CString m_Port;
	int iIndex;
	CComboBox m_SerialList;
	afx_msg void OnBnClickedCancel();
};
