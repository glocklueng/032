
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif
#define WIN32_LEAN_AND_MEAN   

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

// C++ headers
#include <vector>

// for _access 
#include <io.h>

// for gdi
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include <gl/gl.h>
#include <gl/glu.h>

#include "Thread.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))


// Pickobear specific headers
#include "resource.h"
#include "Feeder.h"
#include "csv_parser.h"

#include "SerialPicker.h"

// http://muonics.net/school/spring05/videoInput/
#include "VideoInput/include/videoInput.h"
#include "opencv2/imgproc/imgproc.hpp"

#include <cv.h>
#include <highgui.h>

#include "Grid/GridCtrl.h"

#include "OpenGLControl.h"
#include "TextDump.h"
#include "AlertBox.h"

#include "GCODE_Processor.h"

#include "SerialMFC.h"
#include "FeederSetup.h"
#include "PickobearDlg.h"

#include "opencv.h"
#include "Grid\GridCellNumeric.h"

#include "FeederEdit.h"

#include "TextEditDialog.h"

#include "Counts.h"

#include "EditComponent.h"


#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"opencv_highgui231.lib")
#pragma comment(lib,"opencv_imgproc231.lib")
#pragma comment(lib,"opencv_core231.lib")
//#pragma comment(lib,"videoInput\\videoInput.lib")

// these shouldn't be here.
extern 	videoInput VI;
extern CStringA UTF16toUTF8(const CStringW& utf16);



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


