
// NSL_BT_BlinkyBall.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNSL_BlinkyBallApp:
// See NSL_BT_BlinkyBall.cpp for the implementation of this class
//

class CNSL_BlinkyBallApp : public CWinApp
{
public:
	CNSL_BlinkyBallApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CNSL_BlinkyBallApp theApp;