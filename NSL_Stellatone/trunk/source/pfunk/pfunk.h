
// pfunk.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CpfunkDlg;

// CpfunkApp:
// See pfunk.cpp for the implementation of this class
//

class CpfunkApp : public CWinApp
{
private:
	DECLARE_GDI_PLUS;

public:

	CpfunkApp();

	CpfunkDlg *pDlg;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CpfunkApp theApp;