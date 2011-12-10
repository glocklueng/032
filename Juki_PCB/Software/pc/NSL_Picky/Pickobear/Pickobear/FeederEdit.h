#pragma once
#include "stdafx.h"

// FeederEdit dialog

class FeederEdit : public CDialogEx
{
	DECLARE_DYNAMIC(FeederEdit)

public:
	FeederEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~FeederEdit();

	CListCtrl_FeederList::FeederDatabase entry;

// Dialog Data
	enum { IDD = IDD_FEEDER_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_FeederName;
};
