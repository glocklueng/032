// PickobearDlg.cpp : implementation file
//
// charliex - null space labs 032.la
//
// Todo list :-
//   add head down check to home!!! (done, grbl)
//   remove all the utf8 buffers, choose CString or std::string ? (nearly all done)
//   fully implement new feeder classes (mostly done)
//   update component classes to make it simpler
//   too much relies on the index in the CListCtrl's (all have been replaced)
//   make busy status reflect in GUI (done)
//   add a status print somewhere in the GUI (added)
//   more error checking (a lot more added)
//   figure out the problem with CheckX / CheckAck and see if it can be handled better (added longer delay on timeout, needs proper ACK) (seems to be ok now)
//   add last feeder XY to Grbl and add new GCODE parameter to set feeder (partially added, sets feeder )
//   more help documentation
//   document GCODE (done)
//   add 'Append' option to feeder load/import
//   plain text for all load/save files ? XML i guess since everyones going with that.
//   finish adding machine plot area to GUI (simulate)
//   double check rotation (build eagle test board)
//   add picker for serial port, add registry key, add more error checking for serial port
//   more camera controls, fine tune the slow/fast modes
//   tidy up headers
//   add localisation to strings?
//   reflect limit switches in GUI (partially working)
//   handle flipped pcbs!! (Most important) (done)
//   move database saves to the postncdestroy or earlier (done)
//   make sure feeder names are unique
//   changing over GCODE command feeder to threaded with callbacks (lots of work) (seems to work) (offline tests are good)
//   change camera offset to user definable
//   add x,y update to feeder/components (d0ne)
//   remove/fix all the MFC child thread updates (lots of changes) (seems to be ok)
//   Feeder SetCount isn't properly updated in some cases. (fixed)
//   Cache m_Side since user can change it during a GO process (done)
//   Move commands should check Target XY coordinates, not current
//   Why is head up/down using AddGCODE ?

// Recently added :-
//   added multiple PCB offsets  (not tested!)
//	 implemented delete function in feeder list
//   temporarily turn off redraw in rebuild of feeder list
//   added component/feeder save to exit, add logic for changes (Added for edit dialogs)
//   hopefully a better EmptySerial routine
//   crash bug in quit routines (waits for thread exit)
//   added new gCODE buffer, mostly, untested
//   fixed that wierd bug on the redraw /resize sending commands, rotate was using a draw event to fire a change...
//   MFC isn't threadsafe outside classes so instead i'm switching to WM_APP PostMessages so all the GUI elements 
//      are updated in the main thread only. Added a WHOLE bunch of stuff for this
//   Thread priorities changed
//   More error checking information adding to feeder out dialog
//   New GCODE CPU uses different model to existing (not tested) old gcode does work properly now in GUI thread, testing in its own Multi:: with new priorities
//   Aded more controls to disable/enable state, GUI switches off buttons when machine is busy etc
//   Move slow and rel check current position
//   Added gfx head move to gui
//   Added more agressive error fail out conditions, bad serial writes will abort run and unhome machine
//   Added more defines for GCODE and MS delays

// Recently fixed :-
//   bug in component search, C2 would match C20
//   bug that InternalWriteSerial was reading data back, CheckAck is now inside InternalWriteSerial
//   EmptySerial wasn't flushing the buffer correctly
//	 Some of the cursor functions were incorrectly calculating m_stepSize

#include "stdafx.h"
#include "PickobearDlg.h"

// camera offset in X, move to registry
#define CAMERA_X_OFFSET				( 73900-540+120 )
#define CAMERA_Y_OFFSET				( 0 )

// interval in ms for GUI update of limit switches ( and some other bits )
#define LIMIT_SWITCHES_UPDATE_MS	( 2000 )

// How long to delay waiting for gcode to execute
#define GCODE_WAIT_MS				( 50 )

// how long to delay after a pickup, this shouldn't be necessary
#define GCODE_PICKUP_WAIT_MS		( 1000 ) 

#define GCODE_PUTDOWN_WAIT_MS		( 1000 ) 

// how long to wait for the head to rotate
#define GCODE_WAIT_HEAD_ROTATE_MS	( 500 )

// if nothing is read from the serial port when expected, wait this long in ms for data to appear, this is generally in a timeout loop so its usually time*loopcount
#define SERIAL_WAIT_MS				( 50 )

//  how long to wait after a part is place in the GO thread
#define SLEEP_AFTER_PART_PLACE_MS	( 100 )


// starting to document GCODE, if there is a trailing \n it is an InternalSerialWrite, if not it is AddGCODE*
// home function, controlled by AddGCODE
#define GCODE_HOME			("G28")

// Park machine no park yet in grbl)
//#define GCODE_PARK			("")

// Query Limit Switches
#define GCODE_LIMIT_QUERY	("M29\n")

/// ATC's
#define GCODE_TOOL1			("M24\n")
#define GCODE_TOOL2			("M24\n")
#define GCODE_TOOL3			("M24\n")
#define GCODE_TOOL4			("M24\n")
#define GCODE_TOOL5			("M24\n")
#define GCODE_TOOL6			("M24\n")

// head control (this uses AddGCODE?)
#define GCODE_HEAD_UP		("M11")
#define GCODE_HEAD_DOWN		("M10")

// vacuum control
#define GCODE_VACUUM_ON		("M19\n")
#define GCODE_VACUUM_OFF	("M20\n")

// pick up from feeder
#define GCODE_PICK_UP		("M26\n")

// drop off at PCB
#define GCODE_PUT_DOWN		("M27\n")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation5
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CPickobearDlg dialog

// move this
double m_Thresh1=0,m_Thresh2=10;

CPickobearDlg::CPickobearDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPickobearDlg::IDD, pParent)
	, m_headXPosUM(-1)
	, m_headYPosUM(-1)
	, m_Threshold1(m_Thresh1)
	, m_Threshold2(m_Thresh2)
	, m_Head(0)
	, m_Vacuum(0)
	, m_GOX(0)
	, m_GOY(0)
	, m_FeedersModified(false)
	, m_ComponentsModified(false)
	, m_pFeederDlg( NULL )
	, m_Quit(0)
	, m_Speed(28000/2)
	, m_Homed(false)
	, m_CameraUpdateRate(CAMERA_DEFAULT_UPDATE_RATE_MS)
	, bCameraHead( false )
	, m_Side(0)
	, bFlip(false)
	, bBusy(false)
	, m_Simulate( false )
	, m_PCBIndex(0)
	, m_PCBCount(0)
	, threadHandleCamera(NULL)
	, m_MachineState( MS_IDLE )
	, m_LimitState(0)
	, updateThreadHandle(0)
	, m_StepSizeUM(1)
	, threadProcessGCODE(0)
	, GCODE_CPU_Thread(0)
	, m_TargetXum(0)
	, m_TargetYum(0)
	, m_LastXum(0)
	, m_LastYum(0)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// clear command buffer
	m_GCODECMDBuffer.erase();

	m_PCBs.clear();
}

void CPickobearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GO, GO);
	DDX_Text(pDX, IDC_X_POS, m_headXPosUM);
	DDX_Text(pDX, IDC_Y_POS, m_headYPosUM);
	DDX_Control(pDX, IDC_ROTATE, m_Rotation);
	DDX_Control(pDX, IDC_COMPONENT_LIST, m_ComponentList);
	DDX_Text(pDX, IDC_GOX, m_GOX);
	DDV_MinMaxLong(pDX,IDC_GOX,0,364550);
	DDX_Text(pDX, IDC_GOY, m_GOY);
	DDX_Control(pDX, IDC_FEEDER_LIST, m_FeederList);
	DDX_Control(pDX, IDC_UP_CAMERA, m_UpCamera);
	DDX_Control(pDX, IDC_DOWN_CAMERA, m_DownCamera);
	DDX_Control(pDX, IDC_STEPSIZE, m_StepSize);
	DDX_CBIndex(pDX, IDC_SIDE, m_Side);
	DDX_Control(pDX, IDC_G_SPEED, m_SpeedSelect);
	DDX_Control(pDX, IDC_PCB_LIST, m_PCBList);
	DDX_Text(pDX, IDC_PCB_INDEX, m_PCBIndex);
	DDX_Control(pDX, IDC_STATUS_BAR, m_StatusBar);
}

BEGIN_MESSAGE_MAP(CPickobearDlg, CDialog) 
	ON_WM_SERIAL(OnSerialMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HOME, &CPickobearDlg::OnBnClickedHome)
	ON_BN_CLICKED(IDC_RIGHT, &CPickobearDlg::OnBnClickedRight)
	ON_BN_CLICKED(IDC_PARK, &CPickobearDlg::OnBnClickedPark)
	ON_BN_CLICKED(IDC_TOOL1, &CPickobearDlg::OnBnClickedTool1)
	ON_BN_CLICKED(IDC_TOOL2, &CPickobearDlg::OnBnClickedTool2)
	ON_BN_CLICKED(IDC_TOOL3, &CPickobearDlg::OnBnClickedTool3)
	ON_BN_CLICKED(IDC_TOOL4, &CPickobearDlg::OnBnClickedTool4)
	ON_BN_CLICKED(IDC_TOOL5, &CPickobearDlg::OnBnClickedTool5)
	ON_BN_CLICKED(IDC_TOOL6, &CPickobearDlg::OnBnClickedTool6)
	ON_BN_CLICKED(IDC_UP, &CPickobearDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CPickobearDlg::OnBnClickedDown)
	ON_BN_CLICKED(IDC_LEFT, &CPickobearDlg::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_HEAD, &CPickobearDlg::OnBnClickedHead)
	ON_NOTIFY(TB_ENDTRACK , IDC_ROTATE, &CPickobearDlg::OnNMCustomdrawRotate)
	ON_BN_CLICKED(IDC_UPLEFT, &CPickobearDlg::OnBnClickedUpleft)
	ON_BN_CLICKED(IDC_UPRIGHT, &CPickobearDlg::OnBnClickedUpright)
	ON_BN_CLICKED(IDC_LEFTDOWN, &CPickobearDlg::OnBnClickedLeftdown)
	ON_BN_CLICKED(IDC_BOTTOMLEFT, &CPickobearDlg::OnBnClickedBottomleft)
	ON_BN_CLICKED(IDC_LOAD, &CPickobearDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_SAVE, &CPickobearDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_IMPORT, &CPickobearDlg::OnBnClickedImport)
	ON_WM_NCRBUTTONDOWN()
	ON_BN_CLICKED(IDC_GO, &CPickobearDlg::OnBnClickedGo)
	ON_BN_CLICKED(IDC_FEEDER, &CPickobearDlg::OnBnClickedFeeder)
	ON_BN_CLICKED(IDC_ZERO, &CPickobearDlg::OnBnClickedZero)
	ON_BN_CLICKED(IDC_OFFSET, &CPickobearDlg::OnBnClickedOffset)
	ON_BN_CLICKED(IDC_GOFF, &CPickobearDlg::OnBnClickedGoff)
	ON_BN_CLICKED(IDC_GOXY, &CPickobearDlg::OnBnClickedGoxy)
	ON_EN_KILLFOCUS(IDC_GOX, &CPickobearDlg::OnEnChangeGox)
	ON_EN_KILLFOCUS(IDC_GOY, &CPickobearDlg::OnEnChangeGoy)
	ON_BN_CLICKED(IDC_ADD_FEEDER, &CPickobearDlg::OnBnClickedAddFeeder)
	ON_BN_CLICKED(IDC_UPDATE, &CPickobearDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_LOAD_FEEDER, &CPickobearDlg::OnBnClickedLoadFeeder)
	ON_BN_CLICKED(IDC_SAVE_FEEDER, &CPickobearDlg::OnBnClickedSaveFeeder)
	ON_CBN_SELCHANGE(IDC_UP_CAMERA, &CPickobearDlg::OnCbnSelchangeUpCamera)
	ON_CBN_SELCHANGE(IDC_DOWN_CAMERA, &CPickobearDlg::OnCbnSelchangeDownCamera)
	ON_BN_CLICKED(IDC_ADD_LOWERRIGHT, &CPickobearDlg::OnBnClickedAddLowerright)
	ON_CBN_SELCHANGE(IDC_STEPSIZE, &CPickobearDlg::OnCbnSelchangeStepsize)
	ON_BN_CLICKED(IDC_EDITFEEDER, &CPickobearDlg::OnBnClickedEditfeeder)
	ON_BN_CLICKED(IDC_ASSIGNFEEDER, &CPickobearDlg::OnBnClickedAssignfeeder)
	ON_BN_CLICKED(IDC_CONSOLE, &CPickobearDlg::OnBnClickedConsole)
	ON_BN_CLICKED(IDC_GO2, &CPickobearDlg::OnBnClickedGo2)
	ON_BN_CLICKED(IDC_EDIT_COMPONENT, &CPickobearDlg::OnBnClickedEditComponent)
	ON_BN_CLICKED(IDC_SWAP_HEAD_CAMERA, &CPickobearDlg::OnBnClickedSwapHeadCamera)
	ON_BN_CLICKED(IDC_DELETE_FEEDER, &CPickobearDlg::OnBnClickedDeleteFeeder)
	ON_BN_CLICKED(IDC_PCB_FLIP, &CPickobearDlg::OnBnClickedPcbFlip)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
	ON_BN_CLICKED(IDC_VACUUM_TOGGLE, &CPickobearDlg::OnBnClickedVacuumToggle)
	ON_CBN_SELCHANGE(IDC_G_SPEED, &CPickobearDlg::OnCbnSelchangeGSpeed)
	ON_STN_CLICKED(IDC_SIMULATION, &CPickobearDlg::OnStnClickedSimulation)
	ON_BN_CLICKED(IDC_ADD_PCB, &CPickobearDlg::OnBnClickedAddPcb)
	ON_BN_CLICKED(IDC_DELETE_PCB, &CPickobearDlg::OnBnClickedDeletePcb)
	ON_BN_CLICKED(IDC_TEST_MODE, &CPickobearDlg::OnBnClickedTestMode)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ESTOP, &CPickobearDlg::OnBnClickedEstop)
	ON_WM_TIMER()
	ON_MESSAGE (PB_UPDATE_ALERT, UpdateAlertMessage)
	ON_MESSAGE (PB_UPDATE_XY, UpdateXY)
	ON_MESSAGE (PB_TEXT_OUT, UpdateTextOut)
	ON_BN_CLICKED(IDC_TRANSFER_XY, &CPickobearDlg::OnBnClickedTransferXy)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CListCtrl_Components, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnHdnItemdblclickList2) 
	ON_NOTIFY_REFLECT(NM_RCLICK ,  &CListCtrl_Components::OnNMRClickList2)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CListCtrl_FeederList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnHdnItemdblclickList2) 
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/**
 * PreSubclassWindow - set extended style to use full row select
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_Components::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	// switch to report style/fullrow
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT| LVS_REPORT);
}

/**
 * PreSubclassWindow - set extended style to use full row select
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_FeederList::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	// switch to report style/fullrow
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT| LVS_REPORT);
}

// CPickobearDlg message handlers
// registry names
static const WCHAR *pszName = L"cameras";
static const WCHAR *pszKey = L"PickoBear";

/**
 * GetRegistryDWORD - Get a DWORD from the registry
 *
 * @param name , item to fetch, defaultValue, default in case not found
 * @return DWORD value of item or defaultValue if not found
 *        
 */
DWORD GetRegistryDWORD( CString name , DWORD defaultValue )
{
	CRegKey regKey;
	DWORD regEntry;
	long lResult;

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{
		// Make 'regEntry' equal to zero
		regEntry = 0;

		// Read the value
		if ((lResult = regKey.QueryDWORDValue(name, regEntry)) != ERROR_SUCCESS)
		{
			// Save a default value in the registry key
			regKey.SetDWORDValue(name, defaultValue );

			regEntry = defaultValue;
		}

		// then close the registry key
		regKey.Close();
	}

	return regEntry;
}

/**
 * OnInitDialog - Does presetup of GUI etc, initialise cameras, serial ports etc.
 *
 * @param none used
 * @return 
 * @todo serial picker
 */
BOOL CPickobearDlg::OnInitDialog()
{
	CRegKey regKey;
	DWORD regEntry;
	long lResult;

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	EnableToolTips();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon		

	CRect rect,rect1;

	m_ComponentList.GetClientRect(&rect);
	int nInterval =( rect.Width() /8);

	m_ComponentList.InsertColumn(0, _T("Name"),LVCFMT_CENTER,(int)(nInterval / 1.32 ));
	m_ComponentList.InsertColumn(1, _T("Value"),LVCFMT_CENTER,(int)(nInterval/1.5));
	m_ComponentList.InsertColumn(2, _T("Type"),		LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(3, _T("X"),		LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(4, _T("Y"),		LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(5, _T("R"),		LVCFMT_CENTER,nInterval/2);
	m_ComponentList.InsertColumn(6, _T("Feeder"),	LVCFMT_CENTER,(int)(nInterval*1.8));
	m_ComponentList.InsertColumn(7, _T("L"),		LVCFMT_CENTER,nInterval);

	m_Rotation.SetRange(0,360);

	m_FeederList.GetClientRect(&rect);
	nInterval =( rect.Width() / 10 );

	m_FeederList.InsertColumn(0, _T("Name"),LVCFMT_CENTER,(int)(nInterval+(nInterval/1.3)));
	m_FeederList.InsertColumn(1, _T("X"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(2, _T("Y"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(3, _T("Rot"),LVCFMT_CENTER,(int)(nInterval/1.4));

	m_FeederList.InsertColumn(4, _T("LX"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(5, _T("LY"),LVCFMT_CENTER,nInterval);

	m_FeederList.InsertColumn(6, _T("CX"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(7, _T("CY"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(8, _T("T"),LVCFMT_CENTER,(int)(nInterval/1.5));
	m_FeederList.InsertColumn(9, _T("Index"),LVCFMT_CENTER,(int)(nInterval));

	m_PCBList.GetClientRect(&rect);
	nInterval =( rect.Width() / 2 );

	m_PCBList.InsertColumn(0, _T("Top"),LVCFMT_CENTER,(int)nInterval);
	m_PCBList.InsertColumn(1, _T("Bottom"),LVCFMT_CENTER,(int)nInterval);

	m_UpCamera.ResetContent();
	m_DownCamera.ResetContent();

	DWORD numDevices = VI.listDevices();

	for( DWORD i = 0 ; i < numDevices ; i++ ) {

		_RPT2(_CRT_WARN,"%d) Camera name is [%s]\n",i, VI.getDeviceName( i  ));

		m_UpCamera.AddString( CString(VI.getDeviceName( i  )) );
		m_DownCamera.AddString( CString( VI.getDeviceName( i  ) ));

	}

	// get saved entry, or set to default 0
	regEntry = 0;
	regEntry = GetRegistryDWORD(L"up",regEntry);

	// clamp
	if ( regEntry > numDevices-1 ) {
		regEntry = 0;
	}

	if( CB_ERR  == m_UpCamera.SetCurSel( regEntry ) ) {
		_RPT0(_CRT_WARN,"Error\n");
	}

	regEntry = 0;
	regEntry = GetRegistryDWORD(L"down",regEntry);

	if ( regEntry > numDevices-1 ) regEntry = 0;

	if( CB_ERR  == m_DownCamera.SetCurSel( regEntry ) ) {
		_RPT0(_CRT_WARN,"Error\n");
	}

	// strings

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{
		// read the string
		TCHAR filename[MAX_PATH];
		ULONG length= sizeof(filename)/sizeof(TCHAR);

		if ((lResult = regKey.QueryStringValue(_T("feederDatabase"), filename,&length)) == ERROR_SUCCESS)
		{
			m_FeederList.LoadDatabase( filename );
			m_FeedersModified = false;
		}
		// read the string

		length= sizeof(filename)/sizeof(TCHAR);

		if ((lResult = regKey.QueryStringValue(_T("componentDatabase"), filename,&length)) == ERROR_SUCCESS)
		{
			m_ComponentList.LoadDatabase( filename );
			m_ComponentsModified = false;
		}

		// then close the registry key
		regKey.Close();
	}

	UpdateData(FALSE);

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_CAM1)->GetWindowRect(rect);
	GetDlgItem(IDC_CAM1)->GetWindowRect(rect1);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_UpCameraWindow.oglCreate( rect, rect1, this, m_UpCamera.GetCurSel());

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_CAM2)->GetWindowRect(rect);
	GetDlgItem(IDC_CAM2)->GetWindowRect(rect1);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window	
	m_DownCameraWindow.oglCreate( rect, rect1, this,m_DownCamera.GetCurSel() );

	// convert to a picker
	CString m_ComPort = _T("\\\\.\\COM17");
	m_Serial.Open(m_ComPort, this );

	m_Serial.Setup(CSerial::EBaud38400 );
	EmptySerial();

	// create console
	m_TextEdit = new CTextDump();
	m_TextEdit->Create(IDD_TEXT_DIALOG, this);
	m_TextEdit->ShowWindow(SW_HIDE);
	m_TextEdit->pDlg = this;

	m_AlertBox = new CAlertBox();
	m_AlertBox->Create(IDD_ALERT_BOX, this);
	m_AlertBox->CenterDialog();

	m_AlertBox->m_AlertString.SetWindowText(L"This is a warning");
	m_AlertBox->ShowWindow(SW_HIDE);
	m_AlertBox->pDlg = this;

	// Start thread for 'goCamera'
	Multi::Thread::Create<void>(&CPickobearDlg::goCamera, this, NULL); 

#if 0	
	COGLThread* oglThread = new COGLThread() ;
	oglThread->CreateThread(CREATE_SUSPENDED) ;
	oglThread->m_bAutoDelete = true ; // Let me delete it.
	oglThread->sName = _T("OGL Thread") ; // Initialize something
	oglThread->p_OGLCamera1 = &m_oglWindow;
	oglThread->p_OGLCamera2 = &m_oglWindow1;
	oglThread->ResumeThread() ;
#endif

	// Setup the OpenGL Window's timer to render
	// threaded
	//	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 100, 0);
	if( m_DownCameraWindow.m_camera != -1 ){
		m_DownCameraWindow.m_unpTimer = m_DownCameraWindow.SetTimer(1, CAMERA_DEFAULT_UPDATE_RATE_MS, 0);
	}
	// reset the step size drop down
	m_StepSize.ResetContent();

	for(double i = 0; i < 10 ; i+=.1 ) {

		CString num;

		num.Format(L"%g",i);
		// no 0
		if( i == 0 ) 
			m_StepSize.AddString( L".01" );
		else
			m_StepSize.AddString( num );
	}

	regEntry = 0;
	regEntry = GetRegistryDWORD(L"stepSize",regEntry);

	double value = (regEntry / 1000.0);

	// convert to string
	CString num;
	num.Format(L"%g",value);

	int i = m_StepSize.FindStringExact(-1,  num );

	m_StepSize.SetCurSel( i );

	// convert mm to um
	m_StepSizeUM = atol( CStringA( num ) ) * 1000 ;

	m_SpeedSelect.SetCurSel( ( m_Speed / 2154 )-1 );

	// updating limit switches thread.
	SetTimer( 1, LIMIT_SWITCHES_UPDATE_MS , NULL) ;

	// Start thread that processes GCODE commands
	Multi::Thread::Create<void>(&CPickobearDlg::StartGCODEThread, this, NULL); 

	SetControls( FALSE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPickobearDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		CRect rect;
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);

		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
#if 1
		CRect rect;

		CDC *myDC ;
		CWnd *pWnd = (CWnd*)GetDlgItem(IDC_SIMULATION_DRAW);
		ASSERT( pWnd );
		myDC = pWnd->GetDC();

		if( myDC ) { 

			pWnd->GetClientRect(rect);

			// limits 364000,517000

			long x,y,lx=0,ly=0;
			double dx,dy;

			dx =  rect.right / 364000.0 ;
			dy =  rect.bottom / 517000.0 ;

			x = (long)(m_TargetXum * dx);
			y = (long)(m_TargetYum * dy);
			
			lx = (long)(m_LastXum * dx);
			ly = (long)(m_LastYum * dy);
			
			CPen pen( PS_SOLID, 0, RGB( 255, 0, 0 ) );
			CPen* pOldPen = myDC->SelectObject( &pen );

			_RPT4(_CRT_WARN,"(%d,%d) to (%d,%d)\n",lx,ly,x,y);

			myDC->MoveTo( lx, ly);
			myDC->LineTo( x, y);
			
			myDC->SelectObject( &pOldPen );
			

		}
#endif

	}
}

/**
 * The system calls this function to obtain the cursor to display while the user drags
 * the minimized window.
 *
 * @param none used
 * @return  HCURSOR
 *        
 */
HCURSOR CPickobearDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**
 * InternalWriteSerial - data to send, noConsole don't echo to console( for automatic commands )
 *
 * @param data - UTF8 ASCIZ to send, noConsole false to echo to console
 * @return  pass/fail, machine did or did not ack the command
 *        
 */
bool CPickobearDlg::InternalWriteSerial( const char *data,bool noConsole=false)
{
	ASSERT( data );
	CString out( data  );
	unsigned char buffer[256];
	unsigned long timeout = 100 ;
	DWORD lengthRead = 0;

	// automated commands don't go to console
	if( noConsole == false ) {

		m_TextOut = out; 
		PostMessage (PB_TEXT_OUT, 0 ,0 );

	}

	EmptySerial();

	if( m_Simulate == false ) {

		// Send command to serial port
		if (m_Serial.IsOpen() ) {
			
			// M29 doesn't ACK
			if( strncmp(data,GCODE_LIMIT_QUERY,3) == 0  )  {
				m_Serial.Write( data );
			} else {

				do {
					// if not homed
					if( m_Homed == false ) {
						if( strncmp(data,GCODE_HOME,3) != 0  ) {
							// not homed, not home command
							return false;
						}
					}

					m_Serial.Write( data );

					// wait for machine to acknowledge receipt of data. This relies on no data being in the Serial buffer
			
					memset(buffer,0,sizeof( buffer ) );

					if(ReadSerial(&buffer[0],sizeof(buffer),lengthRead ) == false ){ 
						
						CString Error;
						Error.Format(L"Error in Serial.Read %d",m_Serial.GetLastError() );
						
						int ret = AfxMessageBox( Error ,MB_RETRYCANCEL);
						
						if( ret == IDCANCEL ) {
							return false;
						}

					}
				} while( strncmp((char*)buffer,"ok",2) != 0 );
			}
		} else {
			_RPT0(_CRT_WARN,"InternalWriteSerial: Port not open\n");
			return false;
		}
	}

	return true;
}

/**
 * SetControls - Switches on and off controls that can move the machine
 *
 * @param state - true to enable controls, false to disable
 * @return 
 *        
 */
void CPickobearDlg::SetControls( boolean state )
{
	// dis/enable the GO button
	GO.EnableWindow( state ) ;

	GetDlgItem( IDC_SWAP_HEAD_CAMERA )->EnableWindow( state );

	// have to disable these elsewhere
//	GetDlgItem( 12644620 )->EnableWindow( state );
//	GetDlgItem( 12644844 )->EnableWindow( state );
	
	// test mode
	GetDlgItem( IDC_TEST_MODE )->EnableWindow( state );
	
	// vacuum on/off
	GetDlgItem( IDC_VACUUM_TOGGLE )->EnableWindow( state );

	GetDlgItem( IDC_UPDATE )->EnableWindow( state );

	// enable tool changers
	GetDlgItem( IDC_TOOL1 )->EnableWindow( state );
	GetDlgItem( IDC_TOOL2 )->EnableWindow( state );
	GetDlgItem( IDC_TOOL3 )->EnableWindow( state );
	GetDlgItem( IDC_TOOL4 )->EnableWindow( state );
	GetDlgItem( IDC_TOOL5 )->EnableWindow( state );
	GetDlgItem( IDC_TOOL6 )->EnableWindow( state );

	// Park enable
	GetDlgItem( IDC_PARK )->EnableWindow( state );

	// Cursor pad
	GetDlgItem( IDC_UPLEFT )->EnableWindow( state );
	GetDlgItem( IDC_UPRIGHT )->EnableWindow( state );
	GetDlgItem( IDC_LEFTDOWN )->EnableWindow( state );
	GetDlgItem( IDC_BOTTOMLEFT )->EnableWindow( state );
	GetDlgItem( IDC_UP )->EnableWindow( state );
	GetDlgItem( IDC_DOWN )->EnableWindow( state );
	GetDlgItem( IDC_LEFT )->EnableWindow( state );
	GetDlgItem( IDC_RIGHT )->EnableWindow( state );

	// head up/down
	GetDlgItem( IDC_HEAD )->EnableWindow( state );

	//zero
	GetDlgItem( IDC_ZERO )->EnableWindow( state );

	GetDlgItem( IDC_GOXY )->EnableWindow( state );
	GetDlgItem( IDC_GOFF )->EnableWindow( state );

	GetDlgItem( IDC_GO2 )->EnableWindow( state );
}


/**
 * OnBnClickedHome - Does HOME procedure
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedHome()
{	
	// do we want to do this?
	command_buffer.clear();
	
	m_MachineState = MS_IDLE;

	// if not this will be at the bottom of the list.
	// should we adjust it so its always at the top of the list?
	AddGCODECommand(GCODE_HOME,"Home Failed",Home_callback);
}

// this is the home callback

/**
 * Home_callback - called after home operation (suitable for using in thread)
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::Home_callback( void *pThis, void *userdata ) 
{
	return ((CPickobearDlg*)pThis)->Home_cb2(userdata);
}

/**
 * Home_cb2 - actual home callback
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::Home_cb2(void *userdata ) 
{

	// passed
	if( userdata == (void*)1 )  {

			m_headXPosUM = 0 ; m_headYPosUM =0;
			
			SetControls( TRUE ) ;

			PostMessage (PB_UPDATE_ALERT, 0 ,0 );

			m_Homed = true ;

	} else {

		// switch controls off
		SetControls( FALSE ) ;

		// invalid head position
		m_headXPosUM = -1 ; m_headYPosUM = -1;

		// get user message
		CString message( (const char *)userdata);

		int ret = AfxMessageBox( message, MB_RETRYCANCEL );
		if( ret == IDCANCEL ) {
			return false;
		}
	}

	PostMessage (PB_UPDATE_XY, m_headXPosUM ,m_headYPosUM );

	return true;
}


/**
 * BuildGCodeMove - make the actual GCODE G command from coords and speed
 *
 * @param output buffer to put in, length of buffer, mode 0/1 , xy position, F speed
 * @return 
 *        
 */
bool BuildGCodeMove( char *output, int length, int mode , long x, long y, long speed )
{
	long double tx,ty;

	ASSERT( output) ;
	
	if( output == NULL ) 
		return false;

	// limit the move (move to config )
	if( x > 364550 ) {

		int ret = AfxMessageBox(L"X out of limits, press OK to change to stay within limits, or cancel to stop move",MB_OKCANCEL|MB_ICONHAND);
		if( ret == IDCANCEL ) {
			sprintf_s(output,length,"");
			return false;
		}

		x = 364550 ;
	}

	if( y > 517000 ) {

		int ret = AfxMessageBox(L"Y out of limits, press OK to change to stay within limits, or cancel to stop move",MB_OKCANCEL|MB_ICONHAND);
		if( ret == IDCANCEL ) {
			sprintf_s(output,length,"");
			return false;
		}

		y = 517000 ;
	}

	memset(output,0,length);

	// clamp (we shouldn't do this, it should be an error thats reported to the user)
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;

	// tx,ty this is in mm, x y and microns
	tx = (long double) x / 1000.0 ;
	ty = (long double) y / 1000.0;

	_RPT4(_CRT_WARN,"BuildGCodeMove: (%d, %d) - (%g,%g)\n",x,y,tx,ty);

	sprintf_s(output, length,"G%dX%gY%gF%d",mode,tx,ty,speed);

	_RPT1(_CRT_WARN,"BuildGCodeMove:  %s\n",output);

	return true;
}

/**
 * HomeTest -are we at home position? ( upedate/limit function does this already )
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::HomeTest( void ) 
{
	if( m_Homed == false ) {
		int ret = AfxMessageBox(L"Not Homed",MB_OK|MB_ICONEXCLAMATION);
		if( ret == IDOK ) 
			return false;
	}

	// ping machine ?

	return true;

}

// move head can only go to (int)(x/40)*40,same for y
/**
 * MoveHeadSlow - request to move the head at a fixed lower speed
 *
 * @param XY coords, wait is blocked/non blocked move
 * @return 
 *        
 */
bool CPickobearDlg::MoveHeadSlow(  long x, long y, bool wait=false ) 
{
	if( false == HomeTest( ) ) {
		return false;
	}

	char buffer[ 256 ];

	if ( x < 0 ) x = 0;
	if ( y < 0 ) x = 0;

	// already at position
	if( m_headXPosUM == x && m_headYPosUM == y ) {
		_RPT0(_CRT_WARN,"MoveHeadSlow: ignoring move request, as already at location\n");
		return false;
	}


	if ( false == BuildGCodeMove(buffer,sizeof(buffer),1,x,y,2000) ) {
		return false;
	}

	//	_RPT5(_CRT_WARN,"current pos = %dum,%dum\nGoing to %dum,%dum\n%s\n",m_headXPosUM,m_headYPosUM,x,y,buffer);
	
	// working on simplification
	
	m_LastXum = m_headXPosUM;
	m_LastYum = m_headYPosUM;

	m_TargetXum = x;
	m_TargetYum = y;

	AddGCODECommand(buffer,"MoveHeadSlow: Move Failed",UpdatePosition_callback);
		
	if( wait == true ) {
		// wait for command to process, this locks the gui....
		while( QueueEmpty() == false ) {
			Sleep( GCODE_WAIT_MS );
		}
	}


	return true;
}
// move head can only go to (int)(x/40)*40,same for y
/**
 * MoveHead - request to move the head
 *
 * @param XY coords, wait is blocked/non blocked move
 * @return 
 *        
 */
bool CPickobearDlg::MoveHead(  long x, long y, bool wait=false ) 
{
	if( false == HomeTest( ) ) {
		_RPT0(_CRT_WARN,"MoveHead: Not homed\n");
		return false;
	}

	// already at position
	if( m_headXPosUM == x && m_headYPosUM == y ) {
		_RPT0(_CRT_WARN,"MoveHead: ignoring move request, as already at location\n");
		return false;
	}

	char buffer[ 256 ];

	// this shouldn't really be here, its a failure condition
	if ( x < 0 ) x = 0;
	if ( y < 0 ) x = 0;

	if( false == BuildGCodeMove(buffer,sizeof(buffer),1,x,y,m_Speed) ) {
		_RPT0(_CRT_WARN,"BuildGCodeMove: failed\n");
		return false;
	}

	//	_RPT5(_CRT_WARN,"current pos = %dum,%dum\nGoing to %dum,%dum\n%s\n",m_headXPosUM,m_headYPosUM,x,y,buffer)
	
	m_LastXum = m_headXPosUM;
	m_LastYum = m_headYPosUM;

	// where we want to go
	m_TargetXum = x;
	m_TargetYum = y;


	AddGCODECommand(buffer,"MoveHead failed",UpdatePosition_callback );

	// if gui asked for a move that waits on the head to finish moving
	if( wait == true ) {
		_RPT0(_CRT_WARN,"MoveHead: Waiting\n");
		// wait for command to process, this locks most of the gui....
		while( QueueEmpty() == false ) {
			Sleep( GCODE_WAIT_MS );
		}
	}
	return true;
}
// this is the GCODE callback
/**
 * GCODE_CommandAck_callback - called after GCODE command executes
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::GCODE_CommandAck_callback( void *pThis, void *userdata ) 
{
	_RPT1(_CRT_WARN,"GCODE_CommandAck_callback(%d)\n",userdata);

	return ((CPickobearDlg*)pThis)->GCODE_CommandAck_cb2(userdata);
}

/**
 * GCODE_CommandAck_cb2 - actual GCODE done callback
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::GCODE_CommandAck_cb2(void *userdata ) 
{
	
	if( m_MachineState == MS_ESTOP  ){
		return false ;
	}

	// passed, this means GCODE was accepted.
	if( userdata == (void*)1 )  {

		// do nothing so far, turn off ack msg
		PostMessage (PB_UPDATE_ALERT, 0,0 );

	} else {

		CString message( (const char *)userdata);

		// something went awry
		PostMessage (PB_UPDATE_ALERT,COMMAND_FAILED ,1 );

		int ret = AfxMessageBox( message, MB_RETRYCANCEL );
		if( ret == IDCANCEL ) {
			return false;
		}
	}
	return true;
}


// this is the default callback
/**
 * UpdatePosition_callback - called after gcode execution that needs to update the head position
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::UpdatePosition_callback( void *pThis, void *userdata ) 
{
	_RPT1(_CRT_WARN,"UpdatePosition_callback(%d)\n",userdata);

	return ((CPickobearDlg*)pThis)->UpdatePosition_cb2(userdata);
}

/**
 * UpdatePosition_cb2 - actual update xy callback
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::UpdatePosition_cb2(void *userdata ) 
{
	if( m_MachineState == MS_ESTOP  ){
		return false ;
	}

	// passed, this means GCODE was accepted.
	if( userdata == (void*)1 )  {

		// update the global position
		PostMessage (PB_UPDATE_XY, m_TargetXum,m_TargetYum);

		((CWnd*)GetDlgItem(IDC_SIMULATION_DRAW))->Invalidate();
		
		Invalidate();

	} else {

		CString message( (const char *)userdata);

		int ret = AfxMessageBox( message, MB_RETRYCANCEL );
		if( ret == IDCANCEL ) {
			return false;
		}
	}
	return true;
}

// move head can only go to (int)(x/40)*40,same for y
/**
 * MoveHeadRel - request move head using relative instead of absolute coordinates
 *
 * @param XY position, wait block or non blocking
 * @return 
 *        
 */
bool CPickobearDlg::MoveHeadRel(  long x, long y, bool wait=false ) 
{
	if( false == HomeTest( ) ) {
		return false;
	}

		// already at position
	if( m_headXPosUM == x && m_headYPosUM == y ) {
		_RPT0(_CRT_WARN,"MoveHeadSlow: ignoring move request, as already at location\n");
		return false;
	}

	char buffer[ 256 ];

	if(false == BuildGCodeMove(buffer,sizeof(buffer),0,m_headXPosUM+x,m_headYPosUM+y,m_Speed) ) {
		return false;
	}

	// remember current position
	m_LastXum = m_headXPosUM;
	m_LastYum = m_headYPosUM;

	// Target x/y position
	m_TargetXum = x;
	m_TargetYum = y;

	// Add to queue
	AddGCODECommand(buffer,"MoveHeadRel failed",UpdatePosition_callback );

	PostMessage (PB_UPDATE_XY, m_headXPosUM +x,m_headYPosUM+y );
		
	// wait to complete if finish
	if( wait == true ) {
		
		// wait for command to process, this locks the gui....
		while( QueueEmpty() == false ) {
			Sleep( GCODE_WAIT_MS );
		}
	}

	return true;
}

/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::EmptySerial ( void ) 
{
	char scratchBuffer[100];
	
	// not open
	if( !m_Serial.IsOpen() ) 
		return;

	// attempt to empty serial buffer
	if( ERROR_SUCCESS != m_Serial.Purge() )  {
		_RPT0(_CRT_WARN,"EmptySerial: Purge failed\n");
		return;
	}
	
	int timeout = 5;
	DWORD lengthRead;

	// really empty serial data
	{
		lengthRead = 0;
		
		do { 
			// abort on stop
			if( m_MachineState == MS_ESTOP  || m_MachineState == MS_STOP ) {
				return ;
			}

			m_Serial.Read( &scratchBuffer,sizeof( scratchBuffer) ,&lengthRead,0,500);
			
			if( lengthRead == 0 ) {
				Sleep( SERIAL_WAIT_MS );
				timeout --;
			} 

			if (timeout == 0 )
				break;

		} while( lengthRead == 0 ) ;
	}
}

//this is banannas
/**
 *  CheckX - Waits for the movement ACK
 *
 * X - move went ok
 * L - hit a limit switch
 * H - not homed
 *
 * @param none used
 * @return returns true if acked ok, false if hit limit or other problem
 *        
 */
bool CPickobearDlg::CheckX( void )
{
	DWORD lengthRead = 0;
	unsigned char ch;
	int ret;

	int timeOut = 500;

	if( m_Simulate == true ) {
		return true;
	}
	
	PostMessage (PB_UPDATE_ALERT, WAITING_FOR_CMD_MOVE_ACK ,1 );

	do { 

		if( m_MachineState == MS_ESTOP ) {

			PostMessage (PB_UPDATE_ALERT, MACHINE_ESTOP,0 );
		
			return false;
		}

		timeOut --;
		
		ch = 0;

		ret = m_Serial.Read( &ch, sizeof(ch), &lengthRead );

		// failed to read anything
		if( lengthRead == 0 ) {
			Sleep( SERIAL_WAIT_MS );
		}

		switch ( ret ) {

			case ERROR_SUCCESS:
				if( lengthRead ) {
					_RPT1(_CRT_WARN,"CheckX: received data (%c) \n",isprint(ch)?ch:'?');
				}
				break;
			default:
				break;
		}

	} while( ch!='X' && (timeOut ) );

	PostMessage (PB_UPDATE_ALERT, 0,0 );
	
	if( timeOut == 0 ) 
		_RPT0(_CRT_WARN,"\nCheckX: Timed out\n");
	else
		_RPT1(_CRT_WARN,"\nCheckX: CheckAck('%c')\n", ch);

	if( ch == 'X' && ( timeOut > 0 ) ) {

		return true;
	}

	if( ch == 'L' && ( timeOut > 0 ) ) {

		AfxMessageBox( L"Machine hit a limit switch, going to emergency stop");
		
		m_MachineState = MS_ESTOP;
		
		PostMessage (PB_UPDATE_ALERT, MACHINE_ESTOP,0 );

		return false;
	}

	// should do something
	if( timeOut )
		PostMessage (PB_UPDATE_ALERT, MACHINE_ESTOP,0 );

	// this is either a bad read, or not homed or serial timed out
	return false;
}

/**
 * ReadSerial - Reads one line of input from serial port, with timeout and check for stop/estop
 *
 * @param buffer - point to buffer to receive data
 * @param output_buffer_length -  length of target buffer
 * @param bytes_read - optional , length of data read
 * @return true/false pass or fail, timeout or machine stopped
 *        
 */
bool CPickobearDlg::ReadSerial( unsigned char *buffer, size_t output_buffer_length, DWORD &bytes_read )
{
	DWORD bytesRead;
	int ret;
	size_t index;
	int timeout = 200;
	char ch;

	// not good
	ASSERT( buffer) ;
	ASSERT( output_buffer_length ) ;

	// no buffer
	if (buffer == NULL )
		return false;

	// no length
	if (output_buffer_length == 0 )
		return false;

	// serial port is closed
	if( m_Serial.IsOpen() == false) {
		return false;
	}

	// start of buffer
	index = 0;

	// reset read length
	if (bytes_read)
		bytes_read = 0;

	// read line from serial, with timeout and check for machine state
	do { 

		// count down
		timeout -- ;
		
		// stop if machine fails or user requests
		if( m_MachineState == MS_ESTOP || m_MachineState == MS_STOP ) {

			PostMessage (PB_UPDATE_ALERT, MACHINE_ESTOP,0 );
			return false;
		}
		
		// ran out of time
		if ( timeout == 0 ) {
			break;
		}

		// set to NUL
		ch = 0;

		// read one byte from serial port
		ret = m_Serial.Read( &ch, sizeof(ch) ,&bytesRead );
		if( ret != ERROR_SUCCESS ) {
			
			CString Error;
			Error.Format(L"Error in Serial.Read %d",m_Serial.GetLastError() );

			AfxMessageBox( Error  );
			return false;
		}

		// did serial command execute ok, and did we read anything? 
		if( bytesRead ) {

			//we're not at the end of the buffer?
			if (index < sizeof( buffer ) ) {

				// pass back length to calling func
				if( bytes_read ) {
					bytes_read += bytesRead;
				}

				// don't add end of line to buffer
				if( !( ch == 13 || ch == 10 ) ) {
					buffer[ index++ ] = ch;
				}
			}else {
				_RPT0(_CRT_WARN,"ReadSerial: Data overflow\n");
				
				// since data is valid, even though there is more available
				return true;
			}
		}else {
			// pause if nothing read.
			Sleep( SERIAL_WAIT_MS );
		}

	// wait to end of line 9, \0 terminate?
	} while(!( ch == 13 || ch == 10 ));

	return true;
}

/**
 * CheckAck - Waits for machine to ack our GCODE command
 *
 * @param ack_string - string expecting to reply with
 * @return 
 *        
 */
char CPickobearDlg::CheckAck(const char *ack_string )
{
	DWORD bytesRead = 0;

	ASSERT( ack_string );

	_RPT1(_CRT_WARN,"CheckACK(%s):\n",ack_string);

	unsigned char buffer[ 256 ];

	if( ack_string == NULL ){
		return 'p';
	}

	if( m_Simulate == true ) 
		return 'p';

	memset(buffer,0,sizeof(buffer));

	// Let the GUI status bar know what we're doing
	PostMessage (PB_UPDATE_ALERT, WAITING_FOR_CMD_ACK ,1 );

	// Read one line from serial port
	if( ReadSerial( &buffer[0],sizeof(buffer),bytesRead ) == false ){
		
		// timed out, or machine stopped
		return 'f';
	}

	// reflect status in UI
	PostMessage (PB_UPDATE_ALERT, 0,0 );

	// this should never be zero
	if( bytesRead ) {
		_RPT1(_CRT_WARN,"CheckAck(%s)\n", buffer);
	}
	
	// check to see if its the same ACK string
	if(0==_strnicmp((const char *)ack_string,(const char *)&buffer[0], strlen( ack_string )   )  ) {

		// pass
		return 'p';
	}

	// fail
	return 'f';
}


/**
 * OnBnClickedPark - park head, moves it out of the way
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedPark()
{
#ifndef GCODE_PARK
	MoveHead( MAX_X_TABLE-40 , MAX_Y_TABLE-40 );
#else
	AddGCODECommand(GCODE_PARK,"Park Failed",UpdatePosition_callback);
#endif

}


/**
 * GotoTestPad - moves head to vacuum test pad
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::GotoTestPad( void )
{
	MoveHeadSlow(pulsestoum(12308),pulsestoum(5002));
}

/**
 * OnBnClickedTool1 - change to tool 1
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedTool1()
{
	if( false == HomeTest( ) ) {
		return ;
	}

	AddGCODECommand("M24","Tool 1 failed",UpdatePosition_callback);
}


/**
 * OnBnClickedTool2 - change to tool 2
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedTool2()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	AddGCODECommand("M24","Tool 2 failed",UpdatePosition_callback);
}

/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedTool3()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	AddGCODECommand("M24","Tool 3 failed",UpdatePosition_callback);
}


/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedTool4()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	AddGCODECommand("M24","Tool 4 failed",UpdatePosition_callback);
}


/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedTool5()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	AddGCODECommand("M24","Tool 5 failed",UpdatePosition_callback);
}


/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedTool6()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	AddGCODECommand("M24","Tool 6 failed",UpdatePosition_callback);
}

/**
 * OnBnClickedHead - toggle head up and down
 *
 * @param none used
 * @return m_Head stores state
 *        
 */
void CPickobearDlg::OnBnClickedHead()
{
	if( false == HomeTest( ) ) {
		return ;
	}	

	// wait for command to process, this locks the gui....
	while( QueueEmpty() == false ) {
		Sleep( GCODE_WAIT_MS );
	}

	// @todo : this isn't right at all..
	if( m_Head ) {
		// up
		AddGCODECommand(GCODE_HEAD_UP,"Head up failed",UpdatePosition_callback);
		m_Head = 0;
	} else { 
		//down
		AddGCODECommand(GCODE_HEAD_DOWN,"Head down failed",UpdatePosition_callback);
		m_Head = 1;
	}

	// this should be using immediate mode
}


/**
 * OnNMCustomdrawRotate - sends out a rotate to the head (pretty useless except testing)
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnNMCustomdrawRotate(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( false == HomeTest( ) ) {
		return ;
	}

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	static int rot = 0 ;

	if( m_Serial.IsOpen() && m_Rotation.GetPos() > 0 ) {
		AddGCODECommand("G0H10","Rotate failed",UpdatePosition_callback);	
	}

	_RPT1(_CRT_WARN,"rotation = %d\n",m_Rotation.GetPos() ) ;
}

/**
 * OnBnClickedUp move head up by step size
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedUp()
{	 
	_RPT1(_CRT_WARN,"StepSize %d\n",m_StepSizeUM);
	
	if( m_StepSizeUM == 0 ) 
		m_StepSizeUM = 1;

	MoveHead( m_headXPosUM, m_headYPosUM + m_StepSizeUM) ;
}

/**
 *  OnBnClickedDown move head down  by step size
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedDown()
{
	_RPT1(_CRT_WARN,"StepSize %d\n",m_StepSizeUM);
	
	if( m_StepSizeUM == 0 ) 
		m_StepSizeUM = 1;

	MoveHead( m_headXPosUM, m_headYPosUM - m_StepSizeUM) ;
}

/**
 * OnBnClickedLeft move head left by step size
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedLeft()
{
	_RPT1(_CRT_WARN,"StepSize %d\n",m_StepSizeUM);
	
	if( m_StepSizeUM == 0 ) 
		m_StepSizeUM = 1;

	MoveHead( m_headXPosUM - m_StepSizeUM, m_headYPosUM) ;
}

/**
 * OnBnClickedRight move head right by stepsize
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedRight()
{
	_RPT1(_CRT_WARN,"StepSize %d\n",m_StepSizeUM);
	
	if( m_StepSizeUM == 0 ) 
		m_StepSizeUM = 1;


	MoveHead( m_headXPosUM + m_StepSizeUM, m_headYPosUM) ;
}

/**
 * OnBnClickedUpleft move head left by step size
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedUpleft()
{
	_RPT1(_CRT_WARN,"StepSize %d\n",m_StepSizeUM);
	
	if( m_StepSizeUM == 0 ) 
		m_StepSizeUM = 1;

	MoveHead( m_headXPosUM -= m_StepSizeUM, m_headYPosUM += m_StepSizeUM) ;
}

/**
 * OnBnClickedUpright move head up and right by stepsize
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedUpright()
{
	_RPT1(_CRT_WARN,"StepSize %d\n",m_StepSizeUM);

	if( m_StepSizeUM == 0 ) 
		m_StepSizeUM = 1;

	MoveHead( m_headXPosUM + m_StepSizeUM, m_headYPosUM + m_StepSizeUM) ;
}

/**
 *  OnBnClickedLeftdown move head left and down by stepsize
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedLeftdown()
{
	_RPT1(_CRT_WARN,"StepSize %d\n",m_StepSizeUM);

	if( m_StepSizeUM == 0 ) 
		m_StepSizeUM = 1;


	MoveHead( m_headXPosUM - m_StepSizeUM, m_headYPosUM + m_StepSizeUM) ;
}

/**
 * OnBnClickedBottomleft - move head towards bottom left by step size
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedBottomleft()
{
	_RPT1(_CRT_WARN,"StepSize %d\n",m_StepSizeUM);

	if( m_StepSizeUM == 0 ) 
		m_StepSizeUM = 1;

	MoveHead( m_headXPosUM -= m_StepSizeUM, m_headYPosUM -= m_StepSizeUM) ;
}

/**
 * OnBnClickedLoad - load component database
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedLoad()
{
	if( m_ComponentList.LoadDatabase( L"" ) == true ){ 
		m_ComponentsModified = false;
	}
}

/**
 * OnBnClickedSave - save component database
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedSave()
{
	if( m_ComponentList.SaveDatabase() == true  ){
		m_ComponentsModified = false;
	}
}

/**
 * OnBnClickedImport - import CSV into component database
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedImport()
{
	CStringA m_LoadFile(::GetLoadFile( 
		_T("Supported Files Types(*.csv)\0*.csv\0\0"),
		_T("Pick board to load"),
		NULL
		));

	m_ComponentList.DeleteAllItems();
	m_ComponentList.m_ComponentDatabase.clear();

	const char field_terminator = ',';
	const char line_terminator  = '\n';
	const char enclosure_char   = '\"';

	csv_parser file_parser;
	/* Define how many records we're gonna skip. This could be used to skip the column definitions. */
	file_parser.set_skip_lines(1);

	const char *buffer ;

	buffer = m_LoadFile.GetString();
	file_parser.init( buffer );

	m_LoadFile.ReleaseBuffer();

	/* Here we tell the parser how to parse the file */
	file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_REQUIRED);

	file_parser.set_field_term_char(field_terminator);

	file_parser.set_line_term_char(line_terminator);

	unsigned int row_count = 1U;

	while(file_parser.has_more_rows()) {

		csv_row row = file_parser.get_row();

		m_ComponentList.AddItem(
			row[0].c_str(), 
			row[1].c_str(), 
			row[2].c_str(), 
			row[3].c_str(), 
			row[4].c_str(), 
			row[5].c_str(), 
			row[6].c_str()
			);

	}	;

}

/**
 * GetLoadFile - pops up a requestor for a filename for loading
 *
 * @param none used
 * @return 
 *        
 */
CString GetLoadFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir)
{
	CString szFile;
	TCHAR tmpFile[MAX_PATH];

	OPENFILENAME ofn;

	szFile.Empty();

	ZeroMemory( tmpFile, MAX_PATH );

	if( pStartDir != NULL && _tcslen( pStartDir ) ) {

		_tcscpy_s(
			tmpFile, 
			MAX_PATH,
			pStartDir
			);
	}

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);

	ofn.Flags		=  OFN_FILEMUSTEXIST | OFN_EXPLORER| OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; 
	ofn.hwndOwner	= NULL; //Hwnd

	ofn.lpstrFilter	= ptypes;
	ofn.lpstrInitialDir = pStartDir;

	ofn.lpstrTitle	= caption;

	ofn.lpstrFile	= &tmpFile[0];
	ofn.nMaxFile	= MAX_PATH;

	if (IDOK != GetOpenFileName(&ofn)) {

		return _T("");
	}

	szFile = tmpFile;

	return szFile;
}

/**
 * GetSaveFile get a suitable save file name
 *
 * @param none used
 * @return 
 *        
 */
CString GetSaveFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir )
{

	TCHAR szFile[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(szFile, MAX_PATH);

	if(pStartDir != NULL && _tcslen( pStartDir ) ) {

		_tcscpy_s(szFile, sizeof(szFile)-1,pStartDir);
	}

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);

	ofn.Flags		=  OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; 
	ofn.hwndOwner	= NULL; //Hwnd

	ofn.lpstrFilter	= ptypes;
	ofn.lpstrInitialDir = pStartDir;

	ofn.lpstrTitle	= caption;

	ofn.lpstrFile	= szFile;
	ofn.nMaxFile	= MAX_PATH;

	if (IDOK != GetSaveFileName(&ofn)) {

		return _T("");
	}

	return szFile;
}

/**
 * goCamera camera thread
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::goCamera(LPVOID pThis)
{
	// grab a duplicate handle for the thread so we can suspend resume it when needed.
	if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &threadHandleCamera,
		DUPLICATE_SAME_ACCESS, FALSE, DUPLICATE_SAME_ACCESS)) { 
			return; 
	} 
		
	SetThreadPriority( threadProcessGCODE, THREAD_PRIORITY_BELOW_NORMAL );


	while( m_Quit == 0 ) {

		m_UpCameraWindow.UpdateCamera( 1 ) ;

		// wastes CPU time
		Sleep( m_CameraUpdateRate );
	}

	return;
}

/**
 * goSetup do a full PNP  run
 *
 * @param none used
 * @return 
 * @todo move stuff into pre run check
 */
void CPickobearDlg::goSetup(LPVOID pThis)
{
	unsigned int local_side;

	// user can change side during pick, so cache it.
	local_side = m_Side;

	// These are also problematic for the GCODE buffer
	// Since it is multiple GCODE commands.

	Feeder CurrentFeeder;

	// wait til empty
	while(QueueEmpty() == false )  {
		Sleep( GCODE_WAIT_MS );
	}

	unsigned int i ;

	CListCtrl_Components::CompDatabase entry; 

	if ( bBusy || 	m_MachineState != MS_IDLE ) {
		AfxMessageBox(L"Machine is already running a task",MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

	_RPT0(_CRT_WARN,"goThread\n");

	bBusy = true;

	// Change machine state to busy
	m_MachineState = MS_GO;

	SetControls( FALSE );

	do {

		// check for more than one PCB
		if ( !m_PCBs.empty() ) {

			_RPT2(_CRT_WARN,"goThread: Multiple PCBs placing board ( %d ) of ( %d )\n", m_PCBIndex , m_PCBCount);

			// multiple PCB's to place, is this more complicated becase of sides?
			if( bFlip == false ) {
				m_ComponentList.m_OffsetX = m_PCBs.at( m_PCBIndex ).x_top ;
				m_ComponentList.m_OffsetY = m_PCBs.at( m_PCBIndex ).y_top ;
			} else { 
				m_ComponentList.m_OffsetX = m_PCBs.at( m_PCBIndex ).x_bottom ;
				m_ComponentList.m_OffsetY = m_PCBs.at( m_PCBIndex ).y_bottom ;
			}

			// go to next entry
			m_PCBIndex ++;
		}


		for (i = 0 ; i < m_ComponentList.GetCount(); i ++ ) {

			int in = (m_ComponentList.GetCount()-1)-i;

			m_ComponentList.SetItemState(in, LVIS_SELECTED, LVIS_SELECTED);
			m_ComponentList.EnsureVisible( in ,TRUE );

			entry = m_ComponentList.at(i);

			if( entry.side !=  local_side ) {

				_RPT1(_CRT_WARN,"goThread: skipping %s, wrong side selected\n",entry.label);
				goto skip_part;
			} else {
				_RPT1(_CRT_WARN,"goThread: this side %s selected\n",entry.label);
			}

			// this should be in the precheck
			if ( strlen( entry.feeder ) == 0 ) {

				int ret = AfxMessageBox(L"Feeder not defined", MB_OK|MB_ICONHAND);

				// move back
				if( m_PCBIndex > 0 ) 
					m_PCBIndex--;
				
				// thread not busy
				bBusy = false;

				// Machine is IDLE
				m_MachineState = MS_IDLE;

				SetControls( TRUE );

				// failed
				return;
			}

			CListCtrl_FeederList::FeederDatabase feeder;

			int feederEntry = m_FeederList.Search( entry.feeder  );
			feeder = m_FeederList.at ( feederEntry );

			// duplication as we change over to the new class
			CurrentFeeder = m_FeederList.m_Feeders.at( feederEntry );

			if (feeder.tool < 1 || feeder.tool > 6 ) {

				int ret = AfxMessageBox(L"Tool not defined", MB_OK|MB_ICONHAND);
				
				goto skip_part;
			}

			_RPT1(_CRT_WARN,"goThread: Going to tool %d\n", feeder.tool );

			// can't tool change at the moment...
#if 0
			switch ( feeder.tool ) {
			case 1:
				WriteSerial("M24\n");
				break;
			case 2:
				WriteSerial("M24\n");
				break;
			case 3:
				WriteSerial("M24\n");
				break;
			case 4:
				WriteSerial("M24\n");
				break;
			case 5:
				WriteSerial("M24\n");
				break;
			case 6:
				WriteSerial("M24\n");
				break;
			}
#endif


			/// slow the camera down
			m_CameraUpdateRate = CAMERA_SLOW_UPDATE_RATE_MS ;

			_RPT1(_CRT_WARN,"goThread: Going to feeder %s\n", feeder.label );

			unsigned long feederX,feederY;

			if( CurrentFeeder.GetNextPartPosition( feederX, feederY ) ) {

				/// wait for move
				MoveHead(feederX + CAMERA_X_OFFSET , feederY - CAMERA_Y_OFFSET ,true);
			

				// next part, fail if none available
				if( CurrentFeeder.AdvancePart() == false ) {

					CString Error ;			
					CurrentFeeder.GetLabel( Error ) ;

					Error += L"\n"+CurrentFeeder.ErrorMessage();

					int ret = AfxMessageBox(Error ,MB_OK|MB_ICONEXCLAMATION );

					m_FeederList.RebuildList();

					// move back
					if( m_PCBIndex > 0 ) {
						m_PCBIndex--;
					}

					// Machine is IDLE
					m_MachineState = MS_IDLE;

					// thread is no longer busy
					bBusy = false;

					SetControls( TRUE );

					return ;
				}

				// reflect quantities
				m_FeederList.RebuildList();

			} else {

				CString Error ;			
				CurrentFeeder.GetLabel( Error ) ;
				Error += L"\n"+CurrentFeeder.ErrorMessage();
				int ret = AfxMessageBox(Error ,MB_OK|MB_ICONEXCLAMATION );

				// move back
				if( m_PCBIndex > 0 ) 
					m_PCBIndex--;

				// Machine is IDLE
				m_MachineState = MS_IDLE;
				
				// thread is no longer busy
				bBusy = false;

				SetControls( TRUE );

				return ;
			}

			_RPT0(_CRT_WARN,"goThread: Picking up part\n");
			bool  ret = InternalWriteSerial( GCODE_PICK_UP ); 
			if (ret == false ) {
				// something failed

				// Machine is IDLE
				m_MachineState = MS_ESTOP;
				
				// thread is no longer busy
				bBusy = false;

				SetControls( FALSE );

				return;

			}

			// @todo : tune this
			Sleep( 1000 );

			_RPT1(_CRT_WARN,"goThread: Going to component %s\n", entry.label );

			// wait for move
			if( bFlip == false ) {
				MoveHead(entry.x+m_ComponentList.m_OffsetX + CAMERA_X_OFFSET, entry.y+m_ComponentList.m_OffsetY - CAMERA_Y_OFFSET,true);
			} else {
				MoveHead(entry.x+m_ComponentList.m_OffsetX + CAMERA_X_OFFSET , (0-entry.y)+m_ComponentList.m_OffsetY - CAMERA_Y_OFFSET,true);
			}

			if( entry.rot  || feeder.rot) {

				_RPT1(_CRT_WARN,"goThread: Rotating part %d degrees \n", entry.rot );

				double angle = ( entry.rot + feeder.rot );
			
				_RPT1(_CRT_WARN,"goThread: Adding feeder rotation %d degrees \n",feeder.rot );

				// calculate pulses
				angle = ( 1000.0 / 360.0  ) * angle ;

				char buffer[256];
				int pulses;

				// calculate pulses
				pulses = ( int) ( angle );
				
				if( pulses > 1000 ) {
					pulses -= 1000;
				}

				/// 1000 is 360 degrees
				if( pulses != 1000 ) {
					sprintf_s(buffer,sizeof(buffer),"G0H%d\n", pulses );


					_RPT1(_CRT_WARN,"Executing GCODE %s\n",buffer);

					// do the rotate
					bool ret = InternalWriteSerial( buffer );
					if (ret == false ) {
						// something failed

						// Machine is IDLE
						m_MachineState = MS_ESTOP;

						// thread is no longer busy
						bBusy = false;

						SetControls( FALSE );

						return;

					}
					// @todo : tune this
					Sleep( 500 );
				}
			}

			_RPT0(_CRT_WARN,"goThread: dropping off part\n");

			// head down/air off/up
			// Put Part down
			if (InternalWriteSerial(GCODE_PUT_DOWN) == false ) {
				// something failed

				// Machine is IDLE
				m_MachineState = MS_ESTOP;
				
				// thread is no longer busy
				bBusy = false;

				SetControls( FALSE );

				return;

			}	
			
			//@todo : tune this ( why isn't it using the delay?
			Sleep( 100 );

			// comes here if the part is skipped for some reason
skip_part:;

			// GUI asked machine to stop?
			if( m_MachineState == MS_STOP ) {

				m_MachineState = MS_IDLE;

				goto stop;
			}

			// Emergency stop
			if( m_MachineState == MS_ESTOP ) {

				m_CameraUpdateRate = CAMERA_DEFAULT_UPDATE_RATE_MS ;

				PostMessage (PB_UPDATE_ALERT, MACHINE_ESTOP,0 );

				// ESTOP so all off
				SetControls( FALSE );

				return;
			}

			UpdateWindow();

		}

	// looop for all PCB's
	} while(m_PCBCount, m_PCBIndex < m_PCBCount ) ;

	// Park machine
	OnBnClickedPark();

stop:;
	
	while(QueueEmpty() == false )  {
		Sleep( GCODE_WAIT_MS );
	}

	/// reset the camera speed 
	m_CameraUpdateRate = CAMERA_DEFAULT_UPDATE_RATE_MS ;

	// switch state to idle
	m_MachineState = MS_IDLE ;
	
	bBusy = false;

	SetControls( TRUE );

	return;
}


/**
 * PreRunCheck - make sure all the data makes sense
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::PreRunCheck( bool all_parts=false )
{
	CListCtrl_Components::CompDatabase CompDatabase_Entry; 

	// things to check before starting a placing run (has to know if its single/grouped part or all parts 

	// homed
	if( m_Homed == false ){
		AfxMessageBox(L"PreRunCheck: Machine isn't homed, clear table, check head is up, compressor etc and use HOME button",MB_OK|MB_ICONEXCLAMATION ) ;
		return false;
	}

	// all feeders defined
	// all feeders used have parts
	// all feeders are in table bounds
	// all parts to be placed are in table bounds

	// there are parts on the side choosen	
	bool flag = false;

	if ( all_parts ) {
		for ( unsigned int i = 0 ; i < m_ComponentList.GetCount(); i ++ ) {

			int componentItem = (m_ComponentList.GetCount()-1) - i;
			
			CStringA text ( m_ComponentList.GetItemText( componentItem, 0 )) ;

			_RPT1(_CRT_WARN,"PreRunCheck part %s\n",text);

			componentItem = m_ComponentList.Search( text.GetBuffer() ) ;
			text.ReleaseBuffer();

			if( i == -1 ) {
				AfxMessageBox(L"PreRunCheck: Internal error part not found!",MB_OK|MB_ICONEXCLAMATION);
				return false;
			}

			CompDatabase_Entry = m_ComponentList.at( componentItem );

			if( CompDatabase_Entry.side ==  m_Side ) {
				flag = true;
			}
		}
	} else {

		// single or multiple select mode

		// number of items selected in components list
		int numberComponentsSelected = m_ComponentList.GetSelectedCount();

		if( numberComponentsSelected == 0 ){
			AfxMessageBox(L"PreRunCheck: Nothing will be place\n\nNo components are selected in the component list",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}

		// First entry
		int componentItem = -1;

		for (int itemSelected = 0 ; itemSelected < numberComponentsSelected ; itemSelected ++ ) {

			// get next item
			componentItem = m_ComponentList.GetNextItem(componentItem, LVNI_SELECTED);

			// no more found
			if ( componentItem == -1 ) 
				break;	

			CStringA text ( m_ComponentList.GetItemText( componentItem, 0 )) ;

			_RPT1(_CRT_WARN,"PreRunCheck part %s\n",text);

			int i = m_ComponentList.Search( text.GetBuffer() ) ;
			text.ReleaseBuffer();

			if( i == -1 ) {
				AfxMessageBox(L"PreRunCheck: Internal error part not found!",MB_OK|MB_ICONEXCLAMATION);
				return false;
			}

			CompDatabase_Entry = m_ComponentList.at( i );

			_RPT1(_CRT_WARN,"PreRunCheck: Checking component %s\n",CompDatabase_Entry.label );

			if( CompDatabase_Entry.side ==  m_Side ) {
				flag = true;
			}
		}
	}

	if( flag == false ){

		AfxMessageBox(L"PreRunCheck: No parts will be placed as there are none on the Side you selected\n\nCheck that you have selected the correct parts/side", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	// offset is defined
	// I can't see how this would fly, i suppose if a board is perfectly matched
	if (m_ComponentList.m_OffsetX == 0 && m_ComponentList.m_OffsetY == 0  ) {

		AfxMessageBox(L"PreRunCheck: Offsets not defined\n\nDouble click a valid component from the component list\nMove camera to correct centroid location of part on PCB\nPress OFFSET button to register correct offset\nCheck location of other components by double clicking in component list (OFFSET button can be reused to make changes)",MB_OK|MB_ICONEXCLAMATION ) ;
		return false;
	}

	// should be ready to run
	return true;
}

/**
 * OnBnClickedGo - service GO button, start thread to place
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedGo()
{
	static HANDLE h=0;

	UpdateData();

	// machine is doing something already
	if (bBusy == true ) {

		AfxMessageBox(L"Machine is busy",MB_OK|MB_ICONEXCLAMATION);

		return;
	}

	// stop if we're already going ?
	if ( m_MachineState == MS_GO  ) { 
		
		m_MachineState = MS_STOP;
		
		// hmm
		bBusy = false ; 
		return;
	}

	// do a check on feeders, tools etc here

	if( PreRunCheck( true ) == false ) {
		return;
	}

	// Start thread for 'GO', thread handles busy and go states
//	h = CreateThread(NULL, 0, &CPickobearDlg::goSetup, (LPVOID)this, 0, NULL);

	Multi::Thread::Create<void>(&CPickobearDlg::goSetup, this, NULL);

}

/**
 * OnBnClickedFeeder - run FeederSetup dialog
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedFeeder()
{
	static bool active = false;

	if (active == false ) {

		active = true; 
		m_pFeederDlg = new CFeederSetup(this);

		// need to add deleting to modeless dialog
		if(!m_pFeederDlg->Create(CFeederSetup::IDD,GetDesktopWindow()) ) {
			delete m_pFeederDlg;
			return;
		}
		
//		m_FeederList.RebuildList();
		m_pFeederDlg->ShowWindow(SW_SHOW);

	} else {

		m_pFeederDlg->RebuildList();

		if( !m_pFeederDlg->IsWindowVisible() )
			m_pFeederDlg->ShowWindow(SW_SHOW);

		m_pFeederDlg->BringWindowToTop();
	}

}

/**
 * OnHdnItemdblclickList2 - Move to component
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_Components::OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	// Wont move if machine is doing something
	if ( pDlg->bBusy || pDlg->m_MachineState != CPickobearDlg::MS_IDLE  ) {
		
		_RPT2(_CRT_WARN,"CListCtrl_Components::OnHdnItemdblclickList2: Machine is busy (busy %d, state=%d)\n", (int)pDlg->bBusy, (unsigned int)pDlg->m_MachineState) ;

		return;
	}

	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if( phdr->iItem == -1 ) 
		return;

	*pResult = 0;

	if( false == pDlg->HomeTest( ) ) {
		return ;
	}

	// iItem is item number, list is backwards
	int componentItem = phdr->iItem;

	CStringA text ( GetItemText( componentItem, 0 )) ;

	_RPT1(_CRT_WARN,"CListCtrl_Components::OnHdnItemdblclickList2: part %s\n",text);

	componentItem = Search( text.GetBuffer() ) ;

	text.ReleaseBuffer();

	if( componentItem == -1 ) {
		AfxMessageBox(L"CListCtrl_Components::OnHdnItemdblclickList2: Internal error part not found!",MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

	entry = &m_ComponentDatabase.at( componentItem ) ;

	// entry is item.
	ASSERT( entry );

	_RPT1(_CRT_WARN,"Part = %s\n",entry->label);
	_RPT2(_CRT_WARN,"XY (%d,%d)\n",entry->x,entry->y);
	_RPT2(_CRT_WARN,"O  (%d,%d)\n",pDlg->m_ComponentList.m_OffsetX,pDlg->m_ComponentList.m_OffsetY);

	if( m_OffsetX == 0 && m_OffsetY == 0 ) {

		pDlg->MoveHead(35480,222890);

	}else {

		// GotoXY in micrometers

		// if board is flipped then invert y
		if( pDlg->bFlip == false ) {
			pDlg->MoveHead(
				(entry->x+pDlg->m_ComponentList.m_OffsetX),
				(entry->y)+(pDlg->m_ComponentList.m_OffsetY)
			);
		} else { 
			pDlg->MoveHead(
				(entry->x+pDlg->m_ComponentList.m_OffsetX),
				(0-entry->y)+(pDlg->m_ComponentList.m_OffsetY)
			);
		}
	}

	pDlg->GetDlgItem( IDC_OFFSET )->EnableWindow( TRUE );
}

/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_FeederList::OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	if( phdr->iItem == -1 ) 
		return;

	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	// Wont move if machine is doing something
	if ( pDlg->bBusy || pDlg->m_MachineState != CPickobearDlg::MS_IDLE  ) {
		
		_RPT2(_CRT_WARN,"CListCtrl_FeederList::OnHdnItemdblclickList2: Machine is busy (busy %d, state=%d)\n", (int)pDlg->bBusy, (unsigned int)pDlg->m_MachineState) ;

		return;
	}

	// iItem is item number, list is backwards,.. @todo , is this supposed to use GetCount ?
	int item = (GetCount()-1) - phdr->iItem;

	entry = &mFeederDatabase.at( item ) ;
	// entry is item.
	ASSERT( entry );

	Feeder CurrentFeeder( m_Feeders.at( item ) );

	_RPT2(_CRT_WARN,"CListCtrl_FeederList::OnHdnItemdblclickList2 Going to feeder %s, %s\n", entry->label,CurrentFeeder.entry->label);

	if( GetKeyState ( VK_SHIFT ) & 0x80 ){
		// GotoXY in micrometers
		pDlg->MoveHead(entry->lx,entry->ly);

	} else if( GetKeyState ( VK_CONTROL ) & 0x80 ){

		unsigned long feederX,feederY;

		if( CurrentFeeder.GetNextPartPosition( feederX, feederY ) ) {
			pDlg->MoveHead(feederX , feederY );
		}

	} else {

		// GotoXY in micrometers
		pDlg->MoveHead(entry->x,entry->y);
	}

	*pResult = 0;
}

/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_FeederList::RebuildList ( void ) 
{		
	CString temp;
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	SetRedraw(FALSE);

	// remove any and all items from List
	DeleteAllItems();
	m_Feeders.clear();

	// for all items loaded
	for ( unsigned int i = 0 ; i < m_Count ; i ++ ) {

		// fetch entry 
		entry = &mFeederDatabase.at( i  ) ;

		// add to new feeder entry
		m_Feeders.push_back( *entry );

		temp = entry->label;

		// add first item
		int Index = InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);

		// convert to string
		temp.Format(L"%d",entry->x);
		SetItemText(Index,1,temp);

		temp.Format(L"%d",entry->y);
		SetItemText(Index,2,temp);

		temp.Format(L"%d",entry->rot);
		SetItemText(Index,3,temp);

		temp.Format(L"%d",entry->lx);
		SetItemText(Index,4,temp);

		temp.Format(L"%d",entry->ly);
		SetItemText(Index,5,temp);

		temp.Format(L"%d",entry->countx);
		SetItemText(Index,6,temp);

		temp.Format(L"%d",entry->county);
		SetItemText(Index,7,temp);

		temp.Format(L"%d",entry->tool);
		SetItemText(Index,8,temp);

		temp.Format(L"%d",entry->componentIndex);
		SetItemText(Index,9,temp);
	}	

	SetRedraw( TRUE );
}

/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
bool SetCurrentPosition( long x,long y)
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;

	ASSERT( pDlg );

	// these should be reported as errors.
	if ( x < 0 ) {
		x = 0;
	}

	if ( y < 0 ) {
		y = 0;
	}

	unsigned int linePtr = 0;

	CString xstring;
	CString ystring;

	xstring.Format(L"%d",x);
	ystring.Format(L"%d",y);

	pDlg->m_GOX = x;
	pDlg->m_GOY = y;

	pDlg->m_headXPosUM = x;
	pDlg->m_headYPosUM = y;

	( ( CWnd* ) pDlg->GetDlgItem( IDC_X_POS ) )->SetWindowText( xstring ) ;
	( ( CWnd* ) pDlg->GetDlgItem( IDC_Y_POS ) )->SetWindowText( ystring ) ;

	return true;
}

/**
 * OnBnClickedZero - goto to 0,0
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedZero()
{
	MoveHead(0,0);
	return;
}

/**
 * OnBnClickedOffset set the offset. Tells the PNP where the selected component is) 
 *
 * @param none used
 * @return none
 *        
 */

//  81775
// 236275

void CPickobearDlg::OnBnClickedOffset()
{
	ASSERT( m_ComponentList.entry );

	if( m_ComponentList.entry  == NULL ) {
		return;
	}

	// set the offset.
	m_ComponentList.m_OffsetX = m_headXPosUM - m_ComponentList.entry->x; 

	// board is natural side up (top)
	if( bFlip == false ) {

		m_ComponentList.m_OffsetY = m_headYPosUM - (m_ComponentList.entry->y);

		m_ComponentList.m_OffsetX_top = m_ComponentList.m_OffsetX;
		m_ComponentList.m_OffsetY_top = m_ComponentList.m_OffsetY;

	} else {

		// board is upside down
		m_ComponentList.m_OffsetY = m_headYPosUM - (0-m_ComponentList.entry->y);

		m_ComponentList.m_OffsetX_bottom = m_ComponentList.m_OffsetX;
		m_ComponentList.m_OffsetY_bottom = m_ComponentList.m_OffsetY;
	}

	// turn it off
	GetDlgItem( IDC_OFFSET )->EnableWindow( FALSE );
}

/**
 * OnBnClickedGoff - goto wherever offset is, pointless command
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedGoff()
{
	MoveHead(m_ComponentList.m_OffsetX ,m_ComponentList.m_OffsetY );
}

/**
 * OnBnClickedGoxy - Go to positions specifed in XY boxes
 *
 * @param none used
 * @return  none
 *        
 */
void CPickobearDlg::OnBnClickedGoxy()
{
	MoveHead(m_GOX,m_GOY);
}

/**
 * OnEnChangeGox - internal gui stuff, get rid of
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnEnChangeGox()
{
	UpdateData(TRUE);
}

/**
 * OnEnChangeGoy- internal gui stuff, get rid of
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnEnChangeGoy()
{
	UpdateData(TRUE);
}

/**
 * UTF16toUTF8 convert UTF16 to UTF8
 *
 * @param none used
 * @return 
 *        
 */
CStringA UTF16toUTF8(const CStringW& utf16)
{
	CStringA utf8;
	int len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0);
	if (len>1)
	{ 
		char *ptr = utf8.GetBuffer(len-1);
		if (ptr) WideCharToMultiByte(CP_UTF8, 0, utf16, -1, ptr, len, 0, 0);
		utf8.ReleaseBuffer();
	}

	return utf8;
}

/**
 * OnBnClickedAddFeeder - Add feeder at current XY
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedAddFeeder()
{
	CTextEditDialog TextDialog;

	int ret = TextDialog.DoModal();
	if (ret == IDOK  ) {
		CStringA userInput8( UTF16toUTF8( TextDialog.m_Text ) );

		m_FeederList.AddItem(userInput8.GetString(),m_GOX, m_GOY, 0);
		userInput8.ReleaseBuffer();
	}
}

/**
 * threadUpdateXY This updates the XY position in the GUI
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::threadUpdateXY(LPVOID data)
{
	// thread will have some serious issue if this fails.
	// grab a duplicate handle for the thread so we can suspend resume it when needed.
	if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &updateThreadHandle,
		DUPLICATE_SAME_ACCESS, FALSE, DUPLICATE_SAME_ACCESS)) { 
			return; 
	} 
	
	// low priority thread
	SetThreadPriority( updateThreadHandle, THREAD_PRIORITY_BELOW_NORMAL);

	// run until main thread shuts down
	while( m_Quit == 0  ) {

		// not allowed to process while GCODE still exists in buffer, machine isn't idle or is otherwise busy
		if ( !command_buffer.empty() || bBusy || m_MachineState != MS_IDLE  ) {

			_RPT2(_CRT_WARN,"Machine is busy (busy %d, state=%d), thread going to sleep\n", (int)bBusy, (unsigned int)m_MachineState) ;

			goto suspend;
		}


		// too noisy
		//_RPT0(_CRT_WARN,"threadUpdateXY\n");

		// mark as busy
		m_MachineState = MS_GO;

		// this will generate a delay
		EmptySerial();

		// M29 is a special command, its called often to update the limit switch state, and it doesn't currently properly ACK, it only replies with the limit switches
		// we can handle this in the gcode execution buffer
		if (InternalWriteSerial(GCODE_LIMIT_QUERY,true) == false ) {
			// something failed

			// Machine is IDLE
			m_MachineState = MS_ESTOP;

			SetControls( FALSE );

			return;

		}	
		// should be enough time for a reply	
		Sleep( SERIAL_WAIT_MS );

		DWORD lengthRead=0;

		// read 1 char
		m_Serial.Read( &m_LimitState,sizeof( m_LimitState ) ,&lengthRead,0,100);

		// machine can go back to IDLE ( should be previous mode ) ?
		m_MachineState = MS_IDLE ;

		// no reply, skip til next time
		if (lengthRead == 0 ) {
			goto suspend;
		}

		// bad response
		if( !(m_LimitState & ( 1 << 7 ) ) ){

			_RPT0(_CRT_WARN,"OnBnClickedUpdate: invalid response\n");

			goto suspend;
		}

		// mask off top bit
		m_LimitState &= 0x7f;

		// update GUI (should this move back into OnTimer ) ?

		if( m_LimitState & (1 << 1 ) ) { 
			((CButton*)GetDlgItem(IDC_XL1))->SetCheck(1);
			m_MachineState = MS_ESTOP;
			_RPT0(_CRT_WARN,"X1 Limit\n");
		} else 
			((CButton*)GetDlgItem(IDC_XL1))->SetCheck(0);

		if( m_LimitState & (1 << 2 ) ) {
			((CButton*)GetDlgItem(IDC_XL2))->SetCheck(1);
			m_MachineState = MS_ESTOP;
			_RPT0(_CRT_WARN,"X2 Limit\n");
		} else 
			((CButton*)GetDlgItem(IDC_XL2))->SetCheck(0);

		if( m_LimitState & (1 << 3 ) ) {
			((CButton*)GetDlgItem(IDC_YL1))->SetCheck(1);
			m_MachineState = MS_ESTOP;
			_RPT0(_CRT_WARN,"Y1 Limit\n");
		} else 
			((CButton*)GetDlgItem(IDC_YL1))->SetCheck(0);

		if( m_LimitState & (1 << 4 ) ) {
			((CButton*)GetDlgItem(IDC_YL2))->SetCheck(1);
			m_MachineState = MS_ESTOP;
			_RPT0(_CRT_WARN,"Y2 Limit\n");
		} else 
			((CButton*)GetDlgItem(IDC_YL2))->SetCheck(0);

		if( m_LimitState & (1 << 5 ) ) {
			((CButton*)GetDlgItem(IDC_XHOME))->SetCheck(1);
			//_RPT0(_CRT_WARN,"X Home\n");
		} else 
			((CButton*)GetDlgItem(IDC_XHOME))->SetCheck(0);

		if( m_LimitState & (1 << 6 ) ) {
			((CButton*)GetDlgItem(IDC_YHOME))->SetCheck(1);
			//_RPT0(_CRT_WARN,"Y Home\n");
		} else 
			((CButton*)GetDlgItem(IDC_YHOME))->SetCheck(0);

suspend:;
		if( m_MachineState == MS_ESTOP ) {

			PostMessage (PB_UPDATE_ALERT, MACHINE_ESTOP,0 );

		}

		// suspend this thread
		SuspendThread( updateThreadHandle );
	}
}



// this function has the problem of having a race condition, if anything is added to the GCODE buffer while its working, it'll cause problems.
// needs some sort of interlock.

/**
 * OnBnClickedUpdate - user clicked update button
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedUpdate()
{
	if( m_Simulate ) {
		return;
	}

	// no serial, no nothing
	if (m_Serial.IsOpen() == false )
		return;

	// not allowed to process while GCODE still exists in buffer
	if ( !command_buffer.empty() )
		return;

	if ( bBusy || m_MachineState != MS_IDLE  ) {
		
		_RPT2(_CRT_WARN,"Machine is busy (busy %d, state=%d)\n", (int)bBusy, (unsigned int)m_MachineState) ;

		return;
	}

	// mark as busy
	m_MachineState = MS_GO;

	EmptySerial();

	// M29 is a special command, its called often to update the limit switch state, and it doesn't currently properly ACK, it only replies with the limit switches
	// we can handle this in the gcode execution buffer
	if (InternalWriteSerial( GCODE_LIMIT_QUERY, true ) == false ) {
		// something failed

		// Machine is IDLE
		m_MachineState = MS_ESTOP;
				
		SetControls( FALSE );

		return;

	}	
	
	Sleep( SERIAL_WAIT_MS );
	DWORD lengthRead=0;
	m_Serial.Read( &m_LimitState,1,&lengthRead,0,100);
	
	// machine can go back to IDLE ( should be previous mode ) ?
	m_MachineState = MS_IDLE ;

	// no reply
	if (lengthRead == 0 ) {
		return;
	}

	// bad response
	if( !(m_LimitState & ( 1 << 7 ) ) ){

		_RPT0(_CRT_WARN,"OnBnClickedUpdate: invalid response\n");
		m_MachineState = MS_IDLE ;

		return;
	}

	// mask off top bit
	m_LimitState &= 0x7f;

	// update GUI

	if( m_LimitState & (1 << 1 ) ) { 
		((CButton*)GetDlgItem(IDC_XL1))->SetCheck(1);_RPT0(_CRT_WARN,"X1 Limit\n");} 
	else 
		((CButton*)GetDlgItem(IDC_XL1))->SetCheck(0);

	if( m_LimitState & (1 << 2 ) ) {
		((CButton*)GetDlgItem(IDC_XL2))->SetCheck(1);_RPT0(_CRT_WARN,"X2 Limit\n");} 
	else 
		((CButton*)GetDlgItem(IDC_XL2))->SetCheck(0);

	if( m_LimitState & (1 << 3 ) ) {
		((CButton*)GetDlgItem(IDC_YL1))->SetCheck(1);_RPT0(_CRT_WARN,"Y1 Limit\n");} 
	else 
		((CButton*)GetDlgItem(IDC_YL1))->SetCheck(0);

	if( m_LimitState & (1 << 4 ) ) {
		((CButton*)GetDlgItem(IDC_YL2))->SetCheck(1);_RPT0(_CRT_WARN,"Y2 Limit\n");} 
	else 
		((CButton*)GetDlgItem(IDC_YL2))->SetCheck(0);

	if( m_LimitState & (1 << 5 ) ) {
		((CButton*)GetDlgItem(IDC_XHOME))->SetCheck(1);_RPT0(_CRT_WARN,"X Home\n");} 
	else 
		((CButton*)GetDlgItem(IDC_XHOME))->SetCheck(0);

	if( m_LimitState & (1 << 6 ) ) {
		((CButton*)GetDlgItem(IDC_YHOME))->SetCheck(1);_RPT0(_CRT_WARN,"Y Home\n");} 
	else 
		((CButton*)GetDlgItem(IDC_YHOME))->SetCheck(0);
}

enum {
	PNP_NULL,

	// goto xy
	PNP_GOTO,

	// read position
	PNP_CURRENTPOS,

	// tool changer
	PNP_TOOL0,
	PNP_TOOL1,
	PNP_TOOL2,
	PNP_TOOL3,
	PNP_TOOL4,
	PNP_TOOL5,
	PNP_TOOL6,
};

/**
 * OnNMRClickList2  - Assign feeder to component
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_Components::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;

	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	if( pDlg->m_FeederList.GetSelectedCount() == 0 )
		return;

	// number of items selected in components list
	int numberComponentsSelected = pDlg->m_ComponentList.GetSelectedCount();

	if( numberComponentsSelected == 0 ) {
		return ;
	}

	int nItem = -1;

	// get first item in feeder list ( only one can be selected
	int feederItem = (pDlg->m_FeederList.GetCount()-1) - pDlg->m_FeederList.GetNextItem(-1, LVNI_SELECTED);

	for( int i = 0 ; i < numberComponentsSelected ; i++ ) {

		// nItem is component index selected, -1 first time
		nItem = pDlg->m_ComponentList.GetNextItem(nItem, LVNI_SELECTED);
		if( nItem == -1 ) {
			break;
		}


		int componentEntry ;

		componentEntry = (GetCount()-1 )-nItem;

		// component entry
		entry = &m_ComponentDatabase.at( componentEntry ) ;
			
		_RPT2(_CRT_WARN,"OnNMRClickList2: assigning feeder %s to %s\n",pDlg->m_FeederList.mFeederDatabase.at( feederItem ).label, entry->label );

		pDlg->m_ComponentList.AssignFeeder( (GetCount()-1)-nItem, pDlg->m_FeederList.mFeederDatabase.at (feederItem ).label ) ;

	}

	// rebuild the list
	pDlg->m_ComponentList.RebuildList();
}

/**
 * AddItem - ADd entry to feeder database
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_FeederList::AddItem( const char *label,const long x,long y,short rot )
{
	ASSERT( label ) ;

	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	FeederDatabase entry;

	// zero it
	memset(&entry, 0, sizeof( FeederDatabase ) );

	// copy the label
	strcpy_s(entry.label,label );

	entry.x = x;
	entry.y = y;

	// rotation of the part
	entry.rot = ( rot ) ;

	// conver the string from utf8
	CString temp( label );

	int Index = InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);
	temp.Format(L"%d",x);
	SetItemText(Index,1,temp);
	temp.Format(L"%d",y);
	SetItemText(Index,2,temp);
	temp.Format(L"%d",rot);
	SetItemText(Index,3,temp);

	// add to database
	mFeederDatabase.push_back (entry );
	m_Feeders.push_back ( entry );

	// add one
	m_Count ++;
}

/**
 * OnBnClickedLoadFeeder - Load feeder database from disk
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedLoadFeeder()
{
	if( m_FeederList.LoadDatabase(L"") == true ){
		m_FeedersModified = false;
	}

}

/**
 * OnBnClickedSaveFeeder - Save feeder database to disk
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedSaveFeeder()
{
	if( m_FeederList.SaveDatabase() == true ) {
		m_FeedersModified = false;
	}
}

/**
 * OnContextMenu - not used.
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_FeederList::OnContextMenu(CWnd*, CPoint point)
{
	if((point.x == -1) && (point.y == -1)) {
		// Keystroke invocation
		CRect rect;

		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}
	cMenu = new CMenu();

	cMenu->CreatePopupMenu();

	cMenu->AppendMenu(MF_STRING, ID_FILE_CLOSE, _T("&Close"));
	cMenu->AppendMenu(MF_STRING, ID_FILE_SAVE, _T("&Save\tCtrl+S"));
	cMenu->AppendMenu(MF_STRING, ID_FILE_SAVE_AS, _T("Save &As..."));
	cMenu->AppendMenu(MF_SEPARATOR);
	cMenu->AppendMenu(MF_STRING, ID_FILE_PRINT, _T("&Print\tCtrl+P"));
	cMenu->AppendMenu(MF_STRING, ID_FILE_PRINT_PREVIEW, _T("Print Pre&view"));
	cMenu->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
		point.x, 
		point.y,
		NULL);

	//cMenu->DestroyMenu();
}

// this seems flakey

/**
 * OnSerialMsg - event driven serial, doesn't work
 *
 * @param none used
 * @return 
 *        
 */
afx_msg LRESULT CPickobearDlg::OnSerialMsg (WPARAM wParam, LPARAM lParam)
{
	const CSerialMFC::EEvent eEvent = CSerialMFC::EEvent(LOWORD(wParam));
	const CSerialMFC::EError eError = CSerialMFC::EError(HIWORD(wParam));


	switch (eEvent)
	{
	case CSerialMFC::EEventRecv:

		break;
	}

	// Return successful

	return 0;
}

//bottom camera

/**
 * OnCbnSelchangeDownCamera - change camera, this needs a lot more logic
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnCbnSelchangeDownCamera()
{
	CRegKey regKey;
	DWORD regEntry = 100;
	long lResult;

	UpdateData(FALSE);

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{
		// Make 'regEntry' equal to zero
		regEntry = m_DownCamera.GetCurSel();

		// Save a value in the registry key
		regKey.SetDWORDValue(_T("down"), regEntry);

		// then close the registry key
		regKey.Close();
	}

	m_DownCameraWindow.SetCamera( regEntry );
}

//top camera

/**
 * OnCbnSelchangeUpCamera - change camera, this needs a lot more logic
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnCbnSelchangeUpCamera()
{
	UpdateData();
	CString str;
	CRegKey regKey;
	DWORD regEntry = 100;
	long lResult;

	UpdateData(FALSE);

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{
		// Make 'regEntry' equal to zero
		regEntry = m_UpCamera.GetCurSel();

		// Save a value in the registry key
		regKey.SetDWORDValue(_T("up"), regEntry);

		// then close the registry key
		regKey.Close();
	}

	m_UpCameraWindow.SetCamera( regEntry );
}

/**
 * OnBnClickedAddLowerright - assign lower right position to feeder
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedAddLowerright()
{
	UpdateData();
	CString str;
	int item = m_FeederList.GetSelectedCount();

	if( item == 0 ) {
		return ;
	}

	item = (m_FeederList.GetCount()-1)-m_FeederList.GetNextItem(-1, LVNI_SELECTED);

	// set bottom/left to where head is.
	// this updates the database, but not the list
	m_FeederList.at( item ).lx = m_headXPosUM;
	m_FeederList.at( item ).ly = m_headXPosUM;

	CCounts CountDialog;

	int ret = CountDialog.DoModal();
	if( ret == IDOK ) {
		// set counts
		// this updates the database, but not the list
		m_FeederList.at( item ).countx = CountDialog.m_CountX;
		m_FeederList.at( item ).county = CountDialog.m_CountY;

		// rebuild the list
		m_FeederList.RebuildList();
	}
}

/**
 * OnCbnSelchangeStepsize - change step size for moving around with cursors
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnCbnSelchangeStepsize()
{
	CRegKey regKey;
	DWORD regEntry = 100;
	long lResult;

	CString strText;

	int i = m_StepSize.GetCurSel();

	// GetWindowsText hasn't updated by now
	m_StepSize.GetLBText(i,strText);

	CStringA c(strText);
	_RPT1(_CRT_WARN,"Selected %s mm \n",c);

	double value = atof(c);

	m_StepSizeUM = (unsigned long)(value * 1000);

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{

		// Save a value in the registry key
		regKey.SetDWORDValue(_T("stepSize"), m_StepSizeUM);

		// then close the registry key
		regKey.Close();
	}
}


/**
 * OnBnClickedEditfeeder - pop up feeder edit dialog
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedEditfeeder()
{
	FeederEdit FeederDialog;

	int item = m_FeederList.GetSelectedCount();

	if( item == 0 ) {
		return ;
	}

	item = m_FeederList.GetNextItem(-1, LVNI_SELECTED);

	// iItem is item number, database is backwards
	int clist = (m_FeederList.GetCount()-1) - m_FeederList.GetNextItem(-1, LVNI_SELECTED);

	FeederDialog.entry = m_FeederList.mFeederDatabase.at( clist ) ;

	FeederDialog.m_FeederName = FeederDialog.entry.label;

	int ret = FeederDialog.DoModal();

	// if user didn't cancel dialog
	if (ret == IDOK ) {
		CStringA userInput8( UTF16toUTF8( FeederDialog.m_FeederName ) );

		// Since CString doesn't like being in a vector for load/save then we're using a char array, which makes this bit ugly
		strcpy_s( FeederDialog.entry.label, userInput8 );
		userInput8.ReleaseBuffer();

		// any actual changes?
		if( memcmp( &m_FeederList.mFeederDatabase.at( clist ), &FeederDialog.entry ,sizeof( FeederDialog.entry ) ) ) {

			m_FeederList.mFeederDatabase.at( clist ) = FeederDialog.entry;

			// mark to save later
			m_FeedersModified = true;

			//ugly
			m_FeederList.RebuildList();
		}
	}
}


/**
 * OnBnClickedAssignfeeder - assign a feeder to a component, not finished
 *
 * @param none used
 * @return 
 * @todo implement
 *
 */
void CPickobearDlg::OnBnClickedAssignfeeder()
{
	int componentItem = m_ComponentList.GetSelectedCount();
	if( componentItem == 0 ) {
		return ;
	}
	int feederItem = m_FeederList.GetSelectedCount();

	if( feederItem == 0 ) {
		return ;
	}

	feederItem = m_FeederList.GetNextItem(-1, LVNI_SELECTED);
	componentItem = m_ComponentList.GetNextItem(-1, LVNI_SELECTED);

	_RPT2(_CRT_WARN,"feeder = %d, component = %d\n",feederItem, componentItem );
}

/**
 * RebuildList - rebuild the list in the gui of the feeders.
 *
 * @param none used
 * @return 
 *        
 */
void CListCtrl_Components::RebuildList( void )
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );
	CString temp;
	
	int selected;

	selected = GetNextItem(-1, LVNI_SELECTED);

	DeleteAllItems();

	for ( unsigned int i = 0 ; i < m_Count ; i ++ ) {

		entry = &m_ComponentDatabase.at( i  ) ;

		temp = entry->label;

		int Index = InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);

		temp.Format(L"%s",CString( entry->value ));
		SetItemText(Index,1,temp);

		temp.Format(L"%s",CString( entry->type ) );
		SetItemText(Index,2,temp);

		temp.Format(L"%d",entry->x);
		SetItemText(Index,3,temp);

		temp.Format(L"%d",entry->y);
		SetItemText(Index,4,temp);

		temp.Format(L"%d",entry->rot);
		SetItemText(Index,5,temp);

		temp.Format(L"%d",entry->side);
		SetItemText(Index,7,temp);

		if( strlen( entry->feeder ) == 0 ) { 

			temp.Format(L"NA");

		} else {

			//todo: fix this, search on name of feeder, not the id in
			//this will break as soon as feeders get reordered
			//charlie: changed, and testing, tested working

			// iItem is item number, list is backwards

			int feederIndex ;
			feederIndex = pDlg->m_FeederList.Search( entry->feeder );
			
			_RPT1(_CRT_WARN,"Looking for %s\n",entry->feeder);

			if( feederIndex == -1 ) {

				int ret = AfxMessageBox(L"Feeder not found ", MB_OK|MB_ICONEXCLAMATION);
				return;
			}

			int item = (pDlg->m_FeederList.GetCount()-1) - feederIndex;

			CListCtrl_FeederList::FeederDatabase feederEntry = pDlg->m_FeederList.at( feederIndex ) ;

			_RPT2(_CRT_WARN,"Found for %s at %d\n",feederEntry.label, feederIndex );

			temp.Format(L"%s",CString( feederEntry.label ) );
		}

		SetItemText(Index,6,temp);
	}

	SetItemState(-1, 0, LVIS_SELECTED);

	if (selected != -1 ) {

		SetItemState(selected, LVIS_SELECTED, LVIS_SELECTED);
		EnsureVisible( selected ,TRUE );

	}
}


/**
 * OnBnClickedConsole - hide or show the GCODE console
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedConsole()
{
	static int nconsole = 0;

	if( nconsole == 0 )
		m_TextEdit->ShowWindow( SW_SHOW );
	else
		m_TextEdit->ShowWindow( SW_HIDE );

	nconsole = 1 - nconsole;
}


/**
 * OnBnClickedGo2 - place one part
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedGo2()
{
	static HANDLE h=0;

	UpdateData();

	if ( bBusy ) {
		AfxMessageBox(L"Machine is already running a task",MB_OK|MB_ICONEXCLAMATION);
		return;
	}


	if ( m_MachineState == MS_GO  ) { 
		m_MachineState = MS_STOP;
		return;
	}

	if( PreRunCheck() == false ) {
		return;
	}

	UpdateData(FALSE);

	// Start thread for 'GO', thread handles busy and states
	h = CreateThread(NULL, 0, &CPickobearDlg::goSingleSetup, (LPVOID)this, 0, NULL);
}

/**
 * goSingleSetup - single place thread
 *
 * @param none used
 * @return 
 *        
 */
DWORD WINAPI CPickobearDlg::goSingleSetup(LPVOID pThis)
{
	return ((CPickobearDlg*)pThis)->goSingleThread();
}


/**
 * OnBnClickedSwapHeadCamera - swap position of camera and head, blocks GUI
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedSwapHeadCamera()
{
	if( bCameraHead == false ) {
		if( true == MoveHeadRel( CAMERA_X_OFFSET, -CAMERA_Y_OFFSET, true)  )
			bCameraHead = true;
	} else { 
		if( true == MoveHeadRel( -CAMERA_X_OFFSET, CAMERA_Y_OFFSET, true)  )
			bCameraHead = false;
	}
}

/**
 * OnBnClickedEditComponent - edit component in popup dialog
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedEditComponent()
{
	EditComponent EditDialog;

	int item = m_ComponentList.GetSelectedCount();

	if( item == 0 ) {
		return ;
	}

	item = m_ComponentList.GetNextItem( -1, LVNI_SELECTED );

	// iItem is item number, database is backwards
	int componentListIndex = (m_ComponentList.GetCount()-1) - m_ComponentList.GetNextItem(-1, LVNI_SELECTED);

	if( componentListIndex == -1 ) {
	
		return;
	}

	EditDialog.entry = m_ComponentList.m_ComponentDatabase.at( componentListIndex ) ;

	// push the names out
	EditDialog.m_Name    = EditDialog.entry.label;
	EditDialog.m_Value   = EditDialog.entry.value;
	EditDialog.m_Type    = EditDialog.entry.type;
	EditDialog.m_Feeder  = EditDialog.entry.feeder;

	int ret	 = EditDialog.DoModal();

	if (ret == IDOK ) {
		// grab them back
		// Since CString doesn't like being in a vector for load/save then we're using a char array, which makes this bit ugly
		CStringA userInput8( UTF16toUTF8( EditDialog.m_Name ) );
		strcpy_s( EditDialog.entry.label, userInput8 );
		userInput8.ReleaseBuffer();

		userInput8 =  UTF16toUTF8( EditDialog.m_Value ) ;
		strcpy_s( EditDialog.entry.value, userInput8 );
		userInput8.ReleaseBuffer();

		userInput8 = UTF16toUTF8( EditDialog.m_Type );
		strcpy_s( EditDialog.entry.type, userInput8 );
		userInput8.ReleaseBuffer();

		userInput8 = UTF16toUTF8( EditDialog.m_Feeder );
		strcpy_s( EditDialog.entry.feeder, userInput8 );
		userInput8.ReleaseBuffer();


		//check if any changes were made
		if( memcmp( &m_ComponentList.m_ComponentDatabase.at( componentListIndex ) , &EditDialog.entry, sizeof( EditDialog.entry ) ) ) {

			_RPT0(_CRT_WARN,"Changes were made\n");

			m_ComponentList.m_ComponentDatabase.at( componentListIndex ) = EditDialog.entry;

			m_ComponentsModified = true;
		
			// ugly
			m_ComponentList.RebuildList();
		};

	}
}


/**
 * goSingleThread -  place one part
 *
 * @param none used
 * @return 
 *        
 */
DWORD CPickobearDlg::goSingleThread(void )
{
	Feeder CurrentFeeder;
		
	while( QueueEmpty() == false ) {
		Sleep( GCODE_WAIT_MS );
	}

	unsigned int i ;
	CListCtrl_Components::CompDatabase entry; 

	// find number of parts selected
	int numberComponentsSelected = m_ComponentList.GetSelectedCount();

	// nothing selected
	if( numberComponentsSelected == 0 ) {
		
		bBusy = false;
		
		m_MachineState = MS_IDLE;

		return false;
	}

	SetControls( FALSE );

	// First entry
	int componentItem = -1;

	for (int itemSelected = 0 ; itemSelected < numberComponentsSelected ; itemSelected ++ ) {


		// get next item
		componentItem = m_ComponentList.GetNextItem(componentItem, LVNI_SELECTED);

		// no more found
		if ( componentItem == -1 ) 
			break;


		i = (m_ComponentList.GetCount()-1) - componentItem;

		entry = m_ComponentList.at(i);

		if( entry.side !=  m_Side ) {

			_RPT1(_CRT_WARN,"goSingleThread: skipping %s, wrong side selected\n",entry.label);
			goto skip;
		}

		_RPT1(_CRT_WARN,"goSingleThread: Placing %s\n",entry.label);

		if (strlen( entry.feeder) == 0) {
			int ret = AfxMessageBox(L"Feeder not defined", MB_OK|MB_ICONEXCLAMATION);
			
			bBusy = false;

			SetControls( TRUE );
			
			return 0;
		}

		CListCtrl_FeederList::FeederDatabase feeder ;

		int feederEntry = m_FeederList.Search( entry.feeder  );

		if( feederEntry == -1 ) {

			int ret = AfxMessageBox(L"Feeder not found ", MB_OK | MB_ICONEXCLAMATION);

			bBusy = false;

			SetControls( TRUE );

			return 0;
		}

		int feederIndex ;
		feederIndex = feederEntry;

		feeder = m_FeederList.at ( feederIndex );

		// duplication as we change over to the new class
		CurrentFeeder = m_FeederList.m_Feeders.at( feederIndex );


		if (feeder.tool < 1 || feeder.tool > 6 ) {
			int ret = AfxMessageBox(L"Tool not defined", MB_OK|MB_ICONHAND);

			SetControls( TRUE );
			bBusy = false;

			return 0;
		}

		int in = (m_ComponentList.GetCount()-1)-i;
		m_ComponentList.SetItemState(in, LVIS_SELECTED, LVIS_SELECTED);
		m_ComponentList.EnsureVisible( in ,TRUE );
				
		
		// Machine is moving
		m_MachineState = MS_GO;

		_RPT1(_CRT_WARN,"goSingleThread: Going to tool %d\n", feeder.tool );

		// can't tool change at the moment...
#if 0
		switch ( feeder.tool ) {
		case 1:
			WriteSerial("M24\n");
			break;
		case 2:
			WriteSerial("M24\n");
			break;
		case 3:
			WriteSerial("M24\n");
			break;
		case 4:
			WriteSerial("M24\n");
			break;
		case 5:
			WriteSerial("M24\n");
			break;
		case 6:
			WriteSerial("M24\n");
			break;
		}

#endif

		/// slow the camera down
		m_CameraUpdateRate = CAMERA_SLOW_UPDATE_RATE_MS ;

		_RPT1(_CRT_WARN,"goSingleThread: Going to feeder %s\n", feeder.label );

		unsigned long feederX,feederY;

		if( CurrentFeeder.GetNextPartPosition( feederX, feederY ) ) {

			MoveHead(feederX + CAMERA_X_OFFSET , feederY - CAMERA_Y_OFFSET ,true);
			
			// next part
			CurrentFeeder.AdvancePart();

			// reflect quantities
			m_FeederList.RebuildList();

		} else {

			CString Error ;			
			CurrentFeeder.GetLabel( Error ) ;
			Error += L"\n"+CurrentFeeder.ErrorMessage();
			int ret = AfxMessageBox(Error ,MB_OK|MB_ICONEXCLAMATION );
					
			// Machine is idle
			m_MachineState = MS_IDLE;

			// thread isn't busy
			bBusy = false;

			SetControls( TRUE );

			return 0;
		}

		_RPT0(_CRT_WARN,"Picking up part\n");

		// Pickup
		if (InternalWriteSerial( GCODE_PICK_UP ) == false ) {
			// something failed

			// Machine is IDLE
			m_MachineState = MS_ESTOP;
				
			// thread is no longer busy
			bBusy = false;

			SetControls( FALSE );

			return false;

		}	

		// @todo :tune this
		Sleep( GCODE_PICKUP_WAIT_MS );

		_RPT1(_CRT_WARN,"goSingleThread: Going to component %s\n", entry.label );

	
		if( bFlip == false ) {
			MoveHead(entry.x+m_ComponentList.m_OffsetX + CAMERA_X_OFFSET , entry.y+m_ComponentList.m_OffsetY - CAMERA_Y_OFFSET,true);
		} else {
			MoveHead(entry.x+m_ComponentList.m_OffsetX + CAMERA_X_OFFSET , (0-entry.y)+m_ComponentList.m_OffsetY - CAMERA_Y_OFFSET,true);
		}

		if( entry.rot  || feeder.rot) {

			_RPT1(_CRT_WARN,"goSingleThread: Rotating part %d degrees \n", entry.rot );

			double angle = ( entry.rot + feeder.rot );
			
			_RPT1(_CRT_WARN,"goSingleThread: Adding feeder rotation %d degrees \n",feeder.rot );


			// calculate pulses
			angle = ( 1000.0 / 360.0  ) * angle ;

			int pulses;

			// calculate pulses
			pulses = ( int) ( angle );
				
			if( pulses > 1000 ) {
				pulses -= 1000;
			}

			if ( pulses != 1000 ) {
				CStringA pulses_gcode(L"G0H%d\n", pulses );

				_RPT1(_CRT_WARN,"goSingleThread: Executing GCODE %s\n",pulses_gcode);

				// do the rotate
				if (InternalWriteSerial(pulses_gcode) == false ) {
					// something failed

					// Machine is IDLE
					m_MachineState = MS_ESTOP;
				
					// thread is no longer busy
					bBusy = false;

					SetControls( FALSE );

					return false;

				}

				Sleep( GCODE_WAIT_HEAD_ROTATE_MS );
			}
		}

		// head down/air off/up
		_RPT0(_CRT_WARN,"goSingleThread: dropping off part\n");

		// Put Part down
		if( InternalWriteSerial( GCODE_PUT_DOWN ) == false ) {
				// something failed

				// Machine is IDLE
				m_MachineState = MS_ESTOP;
				
				// thread is no longer busy
				bBusy = false;

				SetControls( FALSE );

				return false;

		}

		//wait
		Sleep( GCODE_PUTDOWN_WAIT_MS );

skip:;

		if( m_MachineState == MS_STOP ) {

			m_MachineState = MS_IDLE;

			/// reset the camera update rate
			m_CameraUpdateRate = CAMERA_DEFAULT_UPDATE_RATE_MS ;

			bBusy = false;

			SetControls( TRUE );

			return true ;
		}

		UpdateWindow();

		Sleep( SLEEP_AFTER_PART_PLACE_MS );

		// Move Camera to part
		if( bFlip == false ) {
			MoveHead(entry.x+m_ComponentList.m_OffsetX, entry.y+m_ComponentList.m_OffsetY,true );
		} else {
			MoveHead(entry.x+m_ComponentList.m_OffsetX, (0-entry.y)+m_ComponentList.m_OffsetY,true );
		}
	}

	// switch state to idle
	m_MachineState = MS_IDLE ;

	// no longer busy
	bBusy  =  false;

	/// reset  the camera speed
	m_CameraUpdateRate = CAMERA_DEFAULT_UPDATE_RATE_MS ;

	SetControls( TRUE );

	return true;
}


/**
 * OnBnClickedDeleteFeeder -  Delete a feeder
 *
 * @param none used
 * @return none
 *        
 */
void CPickobearDlg::OnBnClickedDeleteFeeder()
{
	int feederIndex = -1;

	// find selected items
	feederIndex	= m_FeederList.GetNextItem(feederIndex, LVNI_SELECTED);

	// any selected?
	if( feederIndex != -1 ) {

		feederIndex = m_FeederList.GetItemCount()-1 - feederIndex;
	
		// erase from vector
		m_FeederList.mFeederDatabase.erase( m_FeederList.mFeederDatabase.begin() + feederIndex );
		m_FeederList.m_Count--;

		m_FeedersModified = true;

		m_FeederList.RebuildList();
	}
}

/**
 * flip the pcb, this is not layer but front/back, it mirrors the coordinates
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedPcbFlip()
{
	if ( bFlip == true ) {
		bFlip = false;
		m_ComponentList.m_OffsetX = m_ComponentList.m_OffsetX_top;
		m_ComponentList.m_OffsetY = m_ComponentList.m_OffsetY_top;
	} else {
		bFlip = true;
		m_ComponentList.m_OffsetX = m_ComponentList.m_OffsetX_bottom;
		m_ComponentList.m_OffsetY = m_ComponentList.m_OffsetY_bottom;
	}
}

/**
 * OnToolTipNotify - Show a tool tip for a ID
 *
 * @param id - id of dialog ite,. pNMHDR controls id handle pResult set to 0 if tooltip provided
 * @return  passed in via pResult param, return FALSE if no tooltip set
 *        
 */
BOOL CPickobearDlg::OnToolTipNotify( UINT id,
                                  NMHDR * pNMHDR,
                                  LRESULT * pResult )
{
    // Get the tooltip structure.
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

    // Actually the idFrom holds Control's handle.
    UINT CtrlHandle = pNMHDR->idFrom;

    // Check once again that the idFrom holds handle itself.
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // Get the control's ID.
        UINT nID = ::GetDlgCtrlID( HWND( CtrlHandle ));

        // Now you have the ID. depends on control,
        // set your tooltip message.
        switch( nID )
        {
        case IDC_GO:
            // Set the tooltip text.
            pTTT->lpszText = _T("This starts the pick and place operation after everything has been setup, runs through one side");
            break;

        case IDC_HEAD:
            // Set the tooltip text.
            pTTT->lpszText = _T("Toggles head up and down, be careful with this one. It is useful for teaching location but typically better to use the camera");
            break;

		case IDC_LEFT:
            pTTT->lpszText = _T("Move head to the Left by <steps>");
            break;
		case IDC_RIGHT:
            pTTT->lpszText = _T("Move head to the Right by <steps>");
            break;
		case IDC_UP:
            pTTT->lpszText = _T("Move head to the Up by <steps>");
            break;
		case IDC_DOWN:
            pTTT->lpszText = _T("Move head to the Down by <steps>");
            break;
			
		case IDC_LEFTDOWN:
            pTTT->lpszText = _T("Move head to the Left Down by <steps>");
            break;
			
		case IDC_TOOL1:
            pTTT->lpszText = _T("Loads tool A");
            break;

		case IDC_TOOL2:
            pTTT->lpszText = _T("Loads tool B");
            break;

		case IDC_TOOL3:
            pTTT->lpszText = _T("Loads tool C");
            break;

		case IDC_TOOL4:
            pTTT->lpszText = _T("Loads tool D");
            break;

		case IDC_TOOL5:
            pTTT->lpszText = _T("Loads tool E");
            break;

		case IDC_TOOL6:
            pTTT->lpszText = _T("Loads tool F");
            break;

		case IDC_ADD_LOWERRIGHT:
            pTTT->lpszText = _T("Add lower right coordinates to currently selected feeder");
            break;

		case IDC_FEEDER_GRID_EDIT:
            pTTT->lpszText = _T("Pops up the feeder editing grid");
            break;
		
		case IDC_GOFF:
            pTTT->lpszText = _T("Move head to current X,Y offset");
            break;
		
		case IDC_EDIT_COMPONENT:
            pTTT->lpszText = _T("Brings up edit component dialog");
            break;
		
		case IDC_ZERO:
            pTTT->lpszText = _T("Move head to 0,0 position (same as home, but doesn't home, useful for testing for any accuracy loses)");
            break;
		
		case IDC_UPCAM:
            pTTT->lpszText = _T("Up Camera");
            break;

		case IDC_GOXY:
            pTTT->lpszText = _T("Move head to XY position specified");
            break;

		case IDC_IMPORT:
            pTTT->lpszText = _T("Import CSV file into components grid");
            break;

		case IDC_SAVE:
            pTTT->lpszText = _T("Save components to file");
            break;

		case IDC_LOAD:
            pTTT->lpszText = _T("Load components from file");
            break;

		case IDC_OFFSET:
            pTTT->lpszText = _T("Set offset of component selected in components list");
            break;

		case IDC_GO2:
            pTTT->lpszText = _T("Places single or multiple parts that are selected in the component list");
            break;

		case IDC_DOWNCAM:
            pTTT->lpszText = _T("Down Camera");
            break;

		case IDC_PCB_FLIP:
            pTTT->lpszText = _T("Set when the pcb is flipped to the back side.Also toggles offets so you can have different offsets for each side of PCB");
            break;

		case IDC_XL1:
            pTTT->lpszText = _T("X limit 1 ( left side)");
            break;

		case IDC_XL2:
            pTTT->lpszText = _T("X limit 2 ( right side)");
            break;

		case IDC_YL1:
            pTTT->lpszText = _T("Y limit 1 ( near side)");
            break;

		case IDC_YL2:
            pTTT->lpszText = _T("Y limit 2 ( far side)");
            break;

		case IDC_HOME:
            pTTT->lpszText = _T("Home the machine (bug: fails on long travels sometimes, just reclick it)");
            break;

		case IDC_PCB_LIST:
            pTTT->lpszText = _T("List of PCB XY's on table. Select component from component list. Do offset procedure. Click Add. Repeat for each board (always use same part!)");
            break;

		case IDC_ESTOP:
            pTTT->lpszText = _T("Stops the machine as quickly as possible");
            break;

		case IDC_ADD_PCB:
            pTTT->lpszText = _T("Add PCB at current offset");
            break;

		case IDC_DELETE_PCB:
            pTTT->lpszText = _T("Delete selected PCB");
            break;

		case IDC_VACUUM_TOGGLE:
            pTTT->lpszText = _T("Toggle vacuum on and off");
            break;

		case IDC_FEEDER:
            pTTT->lpszText = _T("Edit feeders in grid");
            break;

		case IDC_PARK:
            pTTT->lpszText = _T("Edit feeders in grid");
            break;

		case IDC_SIDE:
            pTTT->lpszText = _T("Pick the side/layer to place, this is not the same as flipping the PCB, its grouping of the components");
            break;

		case IDC_TEST_MODE:
            pTTT->lpszText = _T("Run the test mode");
            break;

		case IDC_SWAP_HEAD_CAMERA:
            pTTT->lpszText = _T("Swap head and camera position");
            break;
			
		default:
			
			_RPT1(_CRT_WARN, "Not yet documented %d\n", nID );

            pTTT->lpszText = _T("Not yet documented");
            break;
        }

        return TRUE;
    }

    // Not handled.
    return FALSE;
}

/**
 * switch vacuum on and off, if homed
 *
 * @param none used
 * @return none
 *        
 */
void CPickobearDlg::OnBnClickedVacuumToggle()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	
	// wait for command to process, this locks the gui....
	while( QueueEmpty() == false ) {
		Sleep( GCODE_WAIT_MS );
	}


	if( m_Vacuum ) {
		// off
		if( InternalWriteSerial( GCODE_VACUUM_OFF ) == false ) {
			// something failed

			// Machine is IDLE
			m_MachineState = MS_ESTOP;

			SetControls( FALSE );

			return ;

		}
		m_Vacuum = 0;
	} else { 
		//on
		if( InternalWriteSerial( GCODE_VACUUM_ON )  == false ) {
			// something failed

			// Machine is IDLE
			m_MachineState = MS_ESTOP;

			SetControls( FALSE );

			return ;

		}

		m_Vacuum = 1;
	}
}


/**
 * OnCbnSelchangeGSpeed - update machine moving speed  F parameter in GCODE
 *
 * @param none used
 * @return none
 *
 * @todo to proper conversions to speed/mm
 */
void CPickobearDlg::OnCbnSelchangeGSpeed()
{
	UpdateData(FALSE);

	m_Speed = ((m_SpeedSelect.GetCurSel()+1) * 2154 );
	_RPT1(_CRT_WARN,"m_Speed %d\n",m_Speed);
}


/**
 * OnStnClickedSimulation - not yet implemented, supposed to visualise position
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnStnClickedSimulation()
{
}

/**
 * OnBnClickedAddPcb - Add PCB offset at current XY head poisition
 *
 * @param none used
 * @return none
 *        
 */
void CPickobearDlg::OnBnClickedAddPcb()
{
	CString temp;
	PCBEntry entry;
	PCBEntry check;
	
	if( m_ComponentList.m_OffsetX_top == 0  && m_ComponentList.m_OffsetY_top == 0  ) {
		AfxMessageBox(L"Error:Can't add PCB at ( 0, 0 )",MB_OK|MB_ICONHAND);
		return;
	}

	// store both sides
	entry.x_top = m_ComponentList.m_OffsetX_top;
	entry.y_top = m_ComponentList.m_OffsetY_top;
	entry.x_bottom = m_ComponentList.m_OffsetX_bottom;
	entry.y_bottom = m_ComponentList.m_OffsetY_bottom;

	// check if setup already exists
	if (m_PCBCount, m_PCBs.size() ) { 
		for( unsigned int i = 0 ; i < m_PCBs.size() ; i ++ ) {
			check = m_PCBs.at( i );

			if ( memcmp( &entry, &check,sizeof( PCBEntry ) ) == 0  )  {
				AfxMessageBox(L"Already exists!!",MB_OK|MB_ICONEXCLAMATION);
				return;
			}
		}
	}

	temp.Format(L"(%d,%d)",m_ComponentList.m_OffsetX_top,m_ComponentList.m_OffsetY_top );
	int Index = m_PCBList.InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);

	temp.Format(L"(%d,%d)",m_ComponentList.m_OffsetX_bottom,m_ComponentList.m_OffsetY_bottom );
	m_PCBList.SetItemText(Index,1,temp);

	m_PCBs.push_back( entry );

	m_PCBCount ++;
}


/**
 * OnBnClickedDeletePcb - Delete selected PCB offset from list
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedDeletePcb()
{

	// are there are in the list ?
	if( m_PCBCount )  {
			
		// -1 means find first
		int pcbItem = -1;
		
		// find selected items
		pcbItem	= m_PCBList.GetNextItem(pcbItem, LVNI_SELECTED);
		
		// any selected?
		if( pcbItem != -1 ) {

			pcbItem = m_PCBList.GetItemCount()-1 - pcbItem;
			// erase from vector
			m_PCBs.erase( m_PCBs.begin() + pcbItem );
			
			// one less PCB
			m_PCBCount--;

			// Remove all entries
			m_PCBList.DeleteAllItems();
			
			// Rebuild list
			for( unsigned int i = 0 ; i < m_PCBs.size() ; i ++ ) {

				PCBEntry entry ;

				// fetch entry
				entry = m_PCBs.at( i );
				CString temp;

				// re-add
				temp.Format(L"(%d,%d)", entry.x_top, entry.y_top );
				int Index = m_PCBList.InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);

				temp.Format(L"(%d,%d)", entry.x_bottom, entry.y_bottom );
				m_PCBList.SetItemText(Index,1,temp);
			} 

		}
		else {
			// nothing selected!
			AfxMessageBox(L"Nothing selected!",MB_OK|MB_ICONEXCLAMATION);
		}
	}
}

// move this
static bool testMode = false;

/**
 *  test movement and repeatability
 *  checks 0,0 is home does 50 moves, checks 0,0 is home, moves to all feeders, takes pictures, checks 0,0 homed, repeats 10 times
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedTestMode()
{
	if (testMode == true )
		return;

	if( false == HomeTest( ) ) {

		int ret = AfxMessageBox(L"Machine must be homed first",MB_OK|MB_ICONEXCLAMATION);
		
		return ;
	}

	int ret = AfxMessageBox(L"Test Mode can take a long time (10+ minutes)\n\nAre you sure you want to run it ?",MB_YESNO|MB_ICONQUESTION);
	if( ret == IDNO )
		return;


	// interestingly, fails when threaded
	ret = AfxMessageBox(L"Multithreaded test ?",MB_YESNO|MB_ICONQUESTION);
	if( ret == IDYES )
		Multi::Thread::Create<void>(&CPickobearDlg::TestMode, this, NULL);
	else
		TestMode((void*)1);

}
/**
 *  test movement and repeatability (worker thread), this can also run as just a function and not as a thread.
 *  checks 0,0 is home does 50 moves, checks 0,0 is home, moves to all feeders, takes pictures, checks 0,0 homed, repeats 10 times
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::TestMode(LPVOID data)
{
	unsigned long x, y;
	HANDLE TestModeThreadHandle;

	testMode = true;

	// single threaded call
	if (data != (void*)1 ) {
		if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &TestModeThreadHandle,
			DUPLICATE_SAME_ACCESS, FALSE, DUPLICATE_SAME_ACCESS)) { 
				AfxMessageBox(L"Fatal error, failed to duplicate TestMode thread handle");
				return; 
		} 

		SetThreadPriority(TestModeThreadHandle,THREAD_PRIORITY_TIME_CRITICAL);
	}

	SetControls( FALSE );

	for( int testRun = 0 ; testRun < 50 ; testRun ++ ) {

		_RPT1(_CRT_WARN,"TestMode: Test run %d\n",testRun);

		MoveHead( 0, 0 ,true );
		
		// Read limit switches, if not XHOME,YHOME there is a fault

		OnBnClickedUpdate();

		if( !(m_LimitState & (1 << 5 ) ) ) {
			AfxMessageBox(L"X is not homed!",MB_OK|MB_ICONEXCLAMATION);
			testMode = false;
			SetControls( TRUE );
			return;
		}

		if( !(m_LimitState & (1 << 6 ) ) ) {
			AfxMessageBox(L"Y is not homed!",MB_OK|MB_ICONEXCLAMATION);
			testMode = false;
			SetControls( TRUE );
			return;
		}

		for( unsigned int i = 0 ; i < 10 ; i ++ ) {

			do {

				x = rand() * 10;

			} while( x > MAX_X_TABLE ) ;


			do {

				y = rand() * 10;

			} while( y > MAX_Y_TABLE ) ;

			MoveHead( x, y,true  );

			/// little delay (though we should really punch it for testing, the firmware ought to deal with it.
			//Sleep( 100 );
		}

		// Zero
		MoveHead(0,0,true );

		// Read limit switches, if not XHOME,YHOME there is a fault

		if( !(m_LimitState & (1 << 5 ) ) ) {
			AfxMessageBox(L"X is not homed, test failed!",MB_OK|MB_ICONEXCLAMATION);
			testMode = false;
			SetControls( TRUE );
			return;
		}

		if( !(m_LimitState & (1 << 6 ) ) ) {
			AfxMessageBox(L"Y is not homed, test failed!",MB_OK|MB_ICONEXCLAMATION);
			testMode = false;
			SetControls( TRUE );
			return;
		}

		//Sleep(100);

		// stage ii test, run to all defined feeders

		for( unsigned int item = 0 ; item < m_FeederList.GetCount(); item++) {

			m_FeederList.entry = &m_FeederList.mFeederDatabase.at( item ) ;

			// entry is item.
			ASSERT( m_FeederList.entry );

			Feeder CurrentFeeder( m_FeederList.m_Feeders.at( item ) );

			_RPT2(_CRT_WARN,"Going to feeder %s, %s\n", m_FeederList.entry->label,CurrentFeeder.entry->label);

			MoveHead(m_FeederList.entry->x,m_FeederList.entry->y,true );

			//Sleep( 500 );

			// take a picture
			CStringA filename;
			int offset = 0;
try_again:;
			filename.Format("grab\\%s_%d.jpg",m_FeederList.entry->label,offset+testRun);

			if (_access (filename, 0) == 0) {			
				offset++;
				goto try_again;

			}

			m_UpCameraWindow.SaveImage( filename );

		}

		// Zero
		MoveHead(0,0,true );

		// Read limit switches, if not XHOME,YHOME there is a fault

		if( !(m_LimitState & (1 << 5 ) ) ) {
			AfxMessageBox(L"X is not homed, test pass failed!",MB_OK|MB_ICONEXCLAMATION);
			testMode = false;
			SetControls( TRUE );
			return;
		}

		if( !(m_LimitState & (1 << 6 ) ) ) {
			AfxMessageBox(L"Y is not homed, test failed!",MB_OK|MB_ICONEXCLAMATION);
			testMode = false;
			SetControls( TRUE );
			return;
		}
	}

	AfxMessageBox(L"Tests passed",MB_OK);

	testMode = false;

	SetControls( TRUE );

}
/**
 * OnClose - Shutdown threads.
 *
 * @param none used
 * @return 
 *        
 */

void CPickobearDlg::OnClose()
{
	// tell thread to close
	m_Quit = 1;
	SetControls( FALSE );

	// Wake threads up
	
	if( updateThreadHandle) {
		while( ResumeThread(updateThreadHandle)>0);
	}
	if( threadProcessGCODE){
		while(ResumeThread(threadProcessGCODE)>0);
	}

	int timeout;
	

	// wait for all the threads to shutdown
	timeout = 100;

	if(threadHandleCamera)
		while (WAIT_OBJECT_0 != WaitForSingleObject(threadHandleCamera, 200)) {
			_RPT0(_CRT_WARN,"waiting on camera thread\n");
			timeout--;
			if( timeout == 0) break;
		}
	threadHandleCamera = 0;

	timeout = 100;

	if(updateThreadHandle) 
		while (WAIT_OBJECT_0 != WaitForSingleObject(updateThreadHandle, 200)){
			_RPT0(_CRT_WARN,"waiting on update thread\n");
			timeout--;
			if( timeout == 0) break;
		}
	updateThreadHandle = 0;

	timeout = 100;

	if(threadProcessGCODE) 
		while (WAIT_OBJECT_0 != WaitForSingleObject(threadProcessGCODE, 200)){
			_RPT0(_CRT_WARN,"waiting on gcode thread\n");
			timeout--;
			if( timeout == 0) break;
		}
	threadProcessGCODE = 0;

	timeout = 100;

	if(GCODE_CPU_Thread) 
		while (WAIT_OBJECT_0 != WaitForSingleObject(GCODE_CPU_Thread, 200)){
			_RPT0(_CRT_WARN,"waiting on gcode cpu thread\n");
			timeout--;
			if( timeout == 0) break;
		}

	GCODE_CPU_Thread = 0;

	// save databases if they've been changed.
	if ( m_ComponentsModified ) {
		
		int ret = AfxMessageBox(L"Component database has been modified, would you to save it? ",MB_YESNO|MB_ICONQUESTION);
		if( ret == IDYES ) 
			m_ComponentList.SaveDatabase();
	}

	if ( m_FeedersModified ) {
	
		int ret = AfxMessageBox(L"Feeder database has been modified, would you like to save it? ",MB_YESNO|MB_ICONQUESTION);
		if( ret == IDYES ) 
			m_FeederList.SaveDatabase();
	}
	// call base class
	CDialog::OnClose();
}

/**
 * 
 *
 * @param none used
 * @return 
 *        
 */
CPickobearDlg::~CPickobearDlg()
{
	// Wait for camera thread to shut down ( otherwise updates will crash )
	// we can reduce the quit tests in the thread now.

	if(threadHandleCamera)while (WAIT_OBJECT_0 != WaitForSingleObject(threadHandleCamera, 200));

	if( m_AlertBox ) {
		delete m_AlertBox;
		m_AlertBox = NULL;
	}
	if (m_pFeederDlg ) {
		delete m_pFeederDlg;
		m_pFeederDlg = NULL;
	}

	if( m_Serial.IsOpen() ) {
		// close out serial
		m_Serial.Close();
	}
}


/**
 *  If machine sees this , everything will stop and unhome
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnBnClickedEstop()
{
	if( m_Serial.IsOpen() ) {

		// shut it down
		m_Serial.Write("\xFF");
		m_Serial.Write("\xFF");
		m_Serial.Write("\xFF");
	}

	// turn off
	m_MachineState = MS_ESTOP;
	m_Homed= 0;

	SetControls(FALSE);

}


/**
 * StartGCODEThread - Process the GCODE 
 *
 * Should this be timer based ? timer will block the message pump, so it'd have to be a timer in the second thread
 * this should be the only function writing to the serial port.
 * in practise it doesnt work well though, most of the bugs are ironed out now
 *
 * THREAD_PRIORITY_TIME_CRITICAL thread
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::StartGCODEThread(LPVOID pThis)
{
	if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &threadProcessGCODE,
		DUPLICATE_SAME_ACCESS, FALSE, DUPLICATE_SAME_ACCESS)) { 
			AfxMessageBox(L"Fatal error, failed to duplicate GCODE thread handle");
			return; 
	} 
	
	SetThreadPriority( threadProcessGCODE, THREAD_PRIORITY_TIME_CRITICAL );

	// run while not quitting
	while ( m_Quit == 0 ) {

		// pointer to gcode statement callback
		gcode_callback func_ptr;

		// wastes cpu time, message?
		if ( !command_buffer.empty() ) {

			_RPT0(_CRT_WARN,"ProcessGCODECommandsThread: command to process (");

			// copy into buffer, std::vector front returns the oldest entry.
			m_GCODECMDBuffer = command_buffer.front().gcode;
			func_ptr = command_buffer.front().func_ptr_completed;
			_RPT1(_CRT_WARN,"%s)\n",m_GCODECMDBuffer.c_str());

			// i know i know..
retry:;

			// wait if machine isn't in suitable state.
			while (m_MachineState == MS_STOP ||  m_MachineState == MS_ESTOP) {
	
				Sleep( 100 );
			}

			// update state
			m_MachineState = MS_GO;

			// send to machine
			if( m_GCODECMDBuffer.find("\n") == string::npos ) {
				m_GCODECMDBuffer.append("\n");
			}
			
			int ret;

			if( InternalWriteSerial( m_GCODECMDBuffer.c_str())  == false ){

				if(m_GCODECMDBuffer.find(GCODE_HOME) == string::npos )
					ret = GCODE_CommandAck_callback( this, (void*) command_buffer.front().error_message.c_str() ) ;
				else
					ret = func_ptr( this, (void*) command_buffer.front().error_message.c_str() ) ;

				if( ret == false ){
					m_MachineState = MS_ESTOP;
					// command is still in buffer
					goto retry;

				}

			} else {

				// command was accepted
				if(m_GCODECMDBuffer.find(GCODE_HOME) == string::npos )
					ret = GCODE_CommandAck_callback( this , (void*)1 ) ;
				else
					ret = func_ptr( this, (void*) 1 ) ;
		
			}

			// if its a move command we should also get an X (needs a better way)
			// HOME(G28) doesn't do a X acknowledge
			if ((m_GCODECMDBuffer.find(GCODE_HOME) == string::npos ) && m_GCODECMDBuffer.at(0) == 'G' ) {

				bool ret = CheckX();

				// failed to get a proper acknowledge
				if( ret == false ) {

					if( func_ptr ) {

						// failed, callback with message
						bool ret = func_ptr( this, (void*) command_buffer.front().error_message.c_str() ) ;

						if(ret == true ) {
							// retry
							
							// update state
							m_MachineState = MS_IDLE;

							goto retry;
						}

					} else {

						AfxMessageBox(L"Command failed, no callback defined",MB_OK);
						m_MachineState = MS_ESTOP;
						break;// break out of if	

					}

				} else if( func_ptr ) {

					// passed, callback with 1
					func_ptr( this,(void*)1 ) ;
				}
			}
			
			// clear command
			m_GCODECMDBuffer.erase();
			
			// If ESTOP then clear out buffer
			if (m_MachineState != MS_ESTOP ) {
				// remove gcode command from front of list.
				command_buffer.erase(command_buffer.begin());

				// update state
				m_MachineState = MS_IDLE;
			}

		} else {
			
			// uses less CPU time, if can't suspend then Sleep
			if( threadProcessGCODE != 0 )
				if( SuspendThread(threadProcessGCODE) == -1 ) {
					Sleep(10);
				}
			else  
				Sleep(10);
			
		}
	}

	return;
}

// Process queue of GCODE commands
// This is tricky we don't want to stall the MFC message pump since we want to be able to process ESTOP and update graphics properly etc
// But we aso don't want to have the user send commands when the machine is busy
// The function that issued the command needs to know if it passed or failed before going to the next one, or reporting back to the user.
//
// flow
//     (repeat for ever ) user/gui adds a command to the stack
//
// seperate thread
//     (1) process loop is list not empty, wait ?
//       list isn't empty. copy command from bottom of stack
//       process command, 
//          did it fail ? abort everything or retry
//       passed? remove from bottom of stack
//       go back to 1

// this needs to add the GCODE command, a descriptive string and a callback with pass/fail/userdata
// only this command should add to the GCODE buffer

/**
 * AddGCODECommand - ADD GCODE command to buffer
 *
 * @param none used
 * @return 
 *        
 */
bool CPickobearDlg::AddGCODECommand( std::string gcode ,std::string error_message, gcode_callback func_ptr_completed  )
{
	gcode_command command;
	
	// copy in data
	command.gcode = gcode;
	command.error_message = error_message;
	command.func_ptr_completed = func_ptr_completed;
	
	// add to buffer
	command_buffer.push_back( command );
	
	// Wake thread up
	if( threadProcessGCODE){
		if( ResumeThread(threadProcessGCODE) == -1 ){
			_RPT0(_CRT_WARN,"error resuming thread threadProcessGCODE\n");
		}
	}
	return true;
}

/**
 * OnTimer - handles the estop and the gui XY position
 *
 * @param none used
 * @return 
 *        
 */
void CPickobearDlg::OnTimer(UINT_PTR nIDEvent)
{	
	static bool firstRun = true;
	void *Param=0;
	
	if ( m_MachineState == MS_ESTOP ){
		
		SetControls( FALSE ) ;

		PostMessage (PB_UPDATE_ALERT, MACHINE_ESTOP ,1 );
		
		command_buffer.clear();

		return;

	}

	if(!m_Simulate &&   m_Serial.IsOpen() &&  m_Homed == true ) {
		if (firstRun ) {

			_RPT0(_CRT_WARN,"first run, creating update limit switches in GUI thread\n");
			Multi::Thread::Create<void>(&CPickobearDlg::threadUpdateXY, this, NULL);
			firstRun = false;

		} else {

			// update status of limit switches and home etc, but only if the machine is in the right state
			if ( command_buffer.empty() == true && bBusy == false && m_MachineState == MS_IDLE  ) {
			
				if( updateThreadHandle) {
					//_RPT0(_CRT_WARN,"Resuming update thread\n");
					if( ResumeThread(updateThreadHandle) == -1 ){
						_RPT0(_CRT_WARN,"error resuming thread updateThreadHandle\n");
					}
				}
			}
		}
	}

	// should we check GCODE command buffer if its not empty and the thread is suspended here ??
#if 0
	if( threadProcessGCODE, command_buffer.empty() == false ){
		if( ResumeThread(threadProcessGCODE) == -1 ){
			_RPT0(_CRT_WARN,"error resuming thread threadProcessGCODE\n");
		}
	}
#endif
	CDialog::OnTimer(nIDEvent);
}

/**
 * UpdateXY - set new position
 *
 * @param none used
 * @return 
 *        
 */
afx_msg LRESULT CPickobearDlg::UpdateXY (WPARAM wParam, LPARAM lParam)
{
	SetCurrentPosition( wParam, lParam);

	return true;
}
/**
 * Called by the PB_UPDATE_ALERT WM message, changes the status bar message to one of a list of preset messages
 * WAITING_FOR_CMD_ACK
 * WAITING_FOR_CMD_MOVE_ACK
 * COMMAND_FAILED
 * MACHINE_ESTOP
 * MACHINE_IDLING
 * @param wParam which message to display
 *        
 */
afx_msg LRESULT CPickobearDlg::UpdateAlertMessage (WPARAM wParam, LPARAM lParam)
{

	_RPT1(_CRT_WARN,"Updating alert messsage (%d)\n",wParam);

	CString message =L"Idle";

	switch( wParam ) {
		
		case WAITING_FOR_CMD_ACK:
			message = L"Waiting for machine to ACK GCODE";
			break;
		case WAITING_FOR_CMD_MOVE_ACK:
			message = L"Waiting for machine to finish moving";
			break;

		case COMMAND_FAILED:
			message = L"Command failed";
			break;

		case MACHINE_ESTOP:
			
			m_Homed = false ;

			message = L"Emergency Stop : rerun Home procedure";
			break;

		case MACHINE_IDLING:
			break;
	}
			
	// append number of commands left to process
	if( !command_buffer.empty()) {
		message.Format(L"%s : GCODE commands left in buffer ( %d )",CString(message),command_buffer.size());
	}


	if(lParam) {
		_RPT1(_CRT_WARN,"%s\n",CStringA(message));
		m_StatusBar.SetWindowText(message);
	} else
		m_StatusBar.SetWindowText(message);

	return true;

}
/**
 * Called by the PB_UPDATE_TEXT WM message, adds m_TextOut to console output box
 *
 * @param none used
 * @return always true if text added, false if console not available or text length is zero
 *        
 */
afx_msg LRESULT CPickobearDlg::UpdateTextOut (WPARAM wParam, LPARAM lParam)
{		
	ASSERT( m_TextEdit );
	
	if( m_TextEdit, m_TextOut.GetLength() )
		m_TextEdit->Print( m_TextOut ) ;
	else 
		return false;
	
	return true;

}

/**
 * Clicking this updates the currently selected feeders base XY (upper left)
 *
 * @param none used
 * @return none
 *        
 */
void CPickobearDlg::OnBnClickedTransferXy()
{
	if( m_FeederList.GetSelectedCount() == 0 ) {
		AfxMessageBox(L"Select a feeder first");
		return;
	}

	// get first item in feeder list ( only one can be selected
	int feederItem = (m_FeederList.GetCount()-1) - m_FeederList.GetNextItem(-1, LVNI_SELECTED);

	if (feederItem >= 0 && ((unsigned int)feederItem  < m_FeederList.GetCount() ) ){
		m_FeederList.mFeederDatabase.at( feederItem ).x = m_headXPosUM;
		m_FeederList.mFeederDatabase.at( feederItem ).y = m_headYPosUM;
		
		// force redraw of list
		m_FeederList.RebuildList();
	} 
}
