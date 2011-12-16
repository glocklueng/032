#pragma once

#include "PickobearDlg.h"

// CFeederSetup dialog

class CFeederSetup : public CDialog
{
	DECLARE_DYNAMIC(CFeederSetup)

public:
	CFeederSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFeederSetup();

// Dialog Data
	enum { IDD = IDD_FEEDERS };
	
	void ResetFeederGrid( void );
	void AddRow( CListCtrl_FeederList::FeederDatabase *entry );
	void SetValue(int row, int col, double value );
	void RebuildList ( void );
	void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_FeederGrid;
};
