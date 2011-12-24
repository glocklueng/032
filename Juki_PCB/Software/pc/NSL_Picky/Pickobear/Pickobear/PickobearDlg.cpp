// PickobearDlg.cpp : implementation file
//
// charliex - null space labs 032.la
//
// Todo list :-
//   add items to todo list
//   remove all the utf8 buffers, choose CString or std::string ?
//   fully implement new feeder classes
//   update component classes to make it simpler
//   too much relies on the index in the CListCtrl's
//   make busy status reflect in GUI
//   add a status print somewhere in the GUI
//   more error checking
//   figure out the problem with CheckX / CheckAck and see if it can be handled better (added longer delay on timeout)
//   add last feeder XY to Grbl and add new GCODE parameter to set feeder (partially added, sets feeder )
//   more help documentation
//   document GCODE
//   add 'Append' option to feeder load/import
//   plain text for all load/save files ? XML i guess since everyones going with that.
//   finish adding machine plot area to GUI (simulate)
//   double check rotation
//   add picker for serial port, add registry key, add more error checking for serial port
//   more camera controls, fine tune the slow/fast modes
//   tidy up headers
//   add localisation to strings?
//   reflect limit switches in GUI
//   handle flipped pcbs!! (Most important) (done)
//   move database saves to the postncdestroy or earlier

// Recently added :-
//   added multiple PCB offsets  (not tested!)
//	 implemented delete function in feeder list
//   temporarily turn off redraw in rebuild of feeder list

#include "stdafx.h"
#include "Pickobear.h"
#include "PickobearDlg.h"

#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

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
	, m_headXPos(-1)
	, m_headYPos(-1)
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
	, m_Speed(700)
	, m_CameraMode(1)
	, m_Homed(false)
	, m_CameraUpdateRate(CAMERA_DEFAULT_UPDATE_RATE_MS)
	, bSetWaitDone(false)
	, bCameraHead( false )
	, m_Side(0)
	, bFlip(false)
	, bBusy(false)
	, m_Simulate(true)
	, m_PCBIndex(0)
	, m_PCBCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_PCBs.clear();
}

void CPickobearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GO, GO);
	DDX_Text(pDX, IDC_X_POS, m_headXPos);
	DDX_Text(pDX, IDC_Y_POS, m_headYPos);
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
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ROTATE, &CPickobearDlg::OnNMCustomdrawRotate)
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
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CListCtrl_Components, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnHdnItemdblclickList2) 
	ON_NOTIFY_REFLECT(NM_RCLICK ,  &CListCtrl_Components::OnNMRClickList2)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CListCtrl_FeederList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnHdnItemdblclickList2) 
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

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
static const WCHAR *pszName = L"cameras";
static const WCHAR *pszKey = L"PickoBear";

BOOL CPickobearDlg::OnInitDialog()
{
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

	CRegKey regKey;
	DWORD regEntry = 100;
	long lResult;

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{
		// Make 'regEntry' equal to zero
		regEntry = 0;

		// read the value back again
		if ((lResult = regKey.QueryDWORDValue(_T("up"), regEntry)) != ERROR_SUCCESS)
		{
			// Save a value in the registry key
			regKey.SetDWORDValue(_T("up"), regEntry);

		} else {

		}

		// then close the registry key
		regKey.Close();
	}

	if ( regEntry > numDevices-1 ) {
		regEntry = 0;
	}

	if( CB_ERR  == m_UpCamera.SetCurSel( regEntry ) ) {
		_RPT0(_CRT_WARN,"Error\n");
	}

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{
		regEntry = 1;

		// read the value back again
		if ((lResult = regKey.QueryDWORDValue(_T("down"), regEntry)) != ERROR_SUCCESS)
		{
			// Save a value in the registry key
			regKey.SetDWORDValue(_T("down"), regEntry);

		} else {

		}

		// read the string
		TCHAR filename[MAX_PATH];
		ULONG length= sizeof(filename)/sizeof(TCHAR);

		if ((lResult = regKey.QueryStringValue(_T("feederDatabase"), filename,&length)) == ERROR_SUCCESS)
		{
			m_FeederList.LoadDatabase( filename );
		}
		// read the string

		length= sizeof(filename)/sizeof(TCHAR);

		if ((lResult = regKey.QueryStringValue(_T("componentDatabase"), filename,&length)) == ERROR_SUCCESS)
		{
			m_ComponentList.LoadDatabase( filename );
		}



		// then close the registry key
		regKey.Close();
	}


	if ( regEntry > numDevices-1 ) regEntry = 0;

	if( CB_ERR  == m_DownCamera.SetCurSel( regEntry ) ) {
		_RPT0(_CRT_WARN,"Error\n");
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
	CString m_ComPort = _T("\\\\.\\COM14");
	m_Serial.Open(m_ComPort, this );

	m_Serial.Setup(CSerial::EBaud38400 );
	EmptySerial();

	m_TextEdit = new CTextDump();
	m_TextEdit->Create(IDD_TEXT_DIALOG, this);
	m_TextEdit->ShowWindow(SW_HIDE);
	m_TextEdit->pDlg = this;

	// Start thread for 'goCamera'
	HANDLE h = CreateThread(NULL, 0, &CPickobearDlg::goCamera, (LPVOID)this, 0, NULL);

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
	if( m_DownCameraWindow.m_camera != -1 )
		m_DownCameraWindow.m_unpTimer = m_DownCameraWindow.SetTimer(1, 300, 0);


	m_StepSize.ResetContent();


	for( int i = 0; i < 1010 ; i+=10 ) {

		CString num;

		num.Format(L"%d",i);
		if( i == 0 ) 
			m_StepSize.AddString( L"1" );
		else
			m_StepSize.AddString( num );
	}

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{
		// Make 'regEntry' equal to zero
		regEntry = 0;

		// read the value back again
		if ((lResult = regKey.QueryDWORDValue(_T("stepSize"), regEntry)) != ERROR_SUCCESS)
		{
			// Save a value in the registry key
			regKey.SetDWORDValue(_T("stepSize"), regEntry);

		} else {

		}

		// then close the registry key
		regKey.Close();
	}

	if ( regEntry > (DWORD)m_StepSize.GetCount()-1 ) 
		regEntry = 0;

	if( CB_ERR  == m_StepSize.SetCurSel( regEntry ) ) {
		_RPT0(_CRT_WARN,"Error\n");
	}

	m_SpeedSelect.SetCurSel( (m_Speed/100)-1 );


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
#if 0
		CRect rect;

		CWnd *wnd = GetDlgItem(IDC_SIMULATION);
		wnd->GetClientRect( rect) ;


		CPaintDC dc(this); // device context for painting
		Graphics graphics(dc);

		Pen      pen(Color(255, 0, 0, 255));
		graphics.DrawLine(&pen, rect.top,rect.left,rect.bottom,rect.right);
#endif
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPickobearDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPickobearDlg::WriteSerial( const char *text)
{
	ASSERT( text );
	CString out( text  );
	m_TextEdit->Print( out ) ;

	//	m_Simulate = true;

	if( m_Simulate == false ) 
		m_Serial.Write( text );
}

void CPickobearDlg::OnBnClickedHome()
{	
	bool pass = false;

	EmptySerial();

	do { 
		WriteSerial("G28\r\n");

		Sleep( 100 );

		switch( CheckAck("ok") ) {
		case 'f':

			m_headXPos = -1 ; m_headYPos = -1;

			if( MessageBox(_T("Home failed!!"),_T("Error"),IDOK|IDRETRY) == IDRETRY)
				pass = false;
			else
				pass = true;

			break;
		case 'p':

			m_headXPos = 0 ; m_headYPos =0;

			// enable the GO button
			GO.EnableWindow( TRUE ) ;

			GetDlgItem( IDC_SWAP_HEAD_CAMERA )->EnableWindow( TRUE );


			// enable tool changers
			GetDlgItem( IDC_TOOL1 )->EnableWindow( TRUE );
			GetDlgItem( IDC_TOOL2 )->EnableWindow( TRUE );
			GetDlgItem( IDC_TOOL3 )->EnableWindow( TRUE );
			GetDlgItem( IDC_TOOL4 )->EnableWindow( TRUE );
			GetDlgItem( IDC_TOOL5 )->EnableWindow( TRUE );
			GetDlgItem( IDC_TOOL6 )->EnableWindow( TRUE );

			// Park enable
			GetDlgItem( IDC_PARK )->EnableWindow( TRUE );

			// Cursor pad
			GetDlgItem( IDC_UPLEFT )->EnableWindow( TRUE );
			GetDlgItem( IDC_UPRIGHT )->EnableWindow( TRUE );
			GetDlgItem( IDC_LEFTDOWN )->EnableWindow( TRUE );
			GetDlgItem( IDC_BOTTOMLEFT )->EnableWindow( TRUE );
			GetDlgItem( IDC_UP )->EnableWindow( TRUE );
			GetDlgItem( IDC_DOWN )->EnableWindow( TRUE );
			GetDlgItem( IDC_LEFT )->EnableWindow( TRUE );
			GetDlgItem( IDC_RIGHT )->EnableWindow( TRUE );

			// head up/down
			GetDlgItem( IDC_HEAD )->EnableWindow( TRUE );

			//zero
			GetDlgItem( IDC_ZERO )->EnableWindow( TRUE );

			GetDlgItem( IDC_GOXY )->EnableWindow( TRUE );
			GetDlgItem( IDC_GOFF )->EnableWindow( TRUE );

			GetDlgItem( IDC_GO2 )->EnableWindow( TRUE );

			m_Homed = true ;

			pass = true;
			break;
		default:
			break;
		}
	} while( pass == false );

	SetCurrentPosition(m_headXPos,m_headYPos);		
}

bool BuildGCodeMove( char *output, int length, int mode , long x, long y, long speed )
{
	long double tx,ty;

	ASSERT( output) ;

	// limit the move
	if( x > 364550 ) {

		int ret = AfxMessageBox(L"X out of limits, press OK to change to stay within limits, or cancel to stop move",MB_OKCANCEL|MB_ICONHAND);
		if( ret == IDCANCEL ) {
			sprintf_s(output,length,"");
			return false;
		}

		x = 364550 ;
	}

	memset(output,0,length);

	tx = x * 0.0000393700787;
	ty = y * 0.0000393700787;

	if ( tx < 0 ) tx = 0;
	if ( ty < 0 ) ty = 0;

	sprintf_s(output, length,"G%dX%gY%gF%d\r\n",mode,tx,ty,speed);

	return true;
}

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
bool CPickobearDlg::MoveHeadSlow(  long x, long y ) 
{
	if( false == HomeTest( ) ) {
		return false;
	}

	char buffer[ 256 ];

	if ( x < 0 ) x = 0;
	if ( y < 0 ) x = 0;

	m_headXPos = x ; 
	m_headYPos = y ;

	if ( false == BuildGCodeMove(buffer,sizeof(buffer),1,x,y,100) ) {
		return false;
	}
again:;

	//	_RPT5(_CRT_WARN,"current pos = %dum,%dum\nGoing to %dum,%dum\n%s\n",m_headXPos,m_headYPos,x,y,buffer);
	WriteSerial(buffer);

	if( CheckX() == false ) {
		int ret = AfxMessageBox(L"MoveHeadSlow: Move Failed",MB_RETRYCANCEL|MB_ICONEXCLAMATION);
		if(ret == IDRETRY ) 
			goto again;
		if( ret == IDCANCEL ) 
			return false;
	}

	SetCurrentPosition(m_headXPos,m_headYPos);

	return true;
}
// move head can only go to (int)(x/40)*40,same for y
bool CPickobearDlg::MoveHead(  long x, long y ) 
{
	if( false == HomeTest( ) ) {
		return false;
	}

	char buffer[ 256 ];

	if ( x < 0 ) x = 0;
	if ( y < 0 ) x = 0;

	m_headXPos = x ; 
	m_headYPos = y;

#if 0
	if( m_CameraMode == 1 ) {
		m_headYPos += ( CAMERA_OFFSET );
	} else {
		m_headYPos -= ( CAMERA_OFFSET );
	}
#endif

	if ( m_headXPos < 0 ) m_headXPos = 0;
	if ( m_headXPos < 0 ) m_headYPos = 0;

	if( false == BuildGCodeMove(buffer,sizeof(buffer),1,x,y,m_Speed) ) 
		return false;

again:;

	//	_RPT5(_CRT_WARN,"current pos = %dum,%dum\nGoing to %dum,%dum\n%s\n",m_headXPos,m_headYPos,x,y,buffer);
	WriteSerial(buffer);


	if( CheckX() == false ) {
		int ret = AfxMessageBox(L"MoveHead: Move Failed",MB_RETRYCANCEL|MB_ICONEXCLAMATION);
		if(ret == IDRETRY ) 
			goto again;
		if( ret == IDCANCEL ) 
			return false;
	}

	SetCurrentPosition(m_headXPos,m_headYPos);

	// waiting for serial X
	bSetWaitDone = true;

	return true;
}

// move head can only go to (int)(x/40)*40,same for y
bool CPickobearDlg::MoveHeadRel(  long x, long y ) 
{
	if( false == HomeTest( ) ) {
		return false;
	}

	char buffer[ 256 ];

	if(false == BuildGCodeMove(buffer,sizeof(buffer),0,m_headXPos+x,m_headYPos+y,200) ) 
		return false;

again:;

	WriteSerial( buffer );

	if( CheckX() == false ) {
		int ret = AfxMessageBox(L"MoveHeadRel: Move Failed",MB_RETRYCANCEL|MB_ICONEXCLAMATION);
		if(ret == IDRETRY ) 
			goto again;
		if( ret == IDCANCEL ) 
			return false;
	}

	SetCurrentPosition( m_headXPos + x, m_headYPos+y);

	return true;
}
void CPickobearDlg::EmptySerial ( void ) 
{
	char bufferx[100];

	return ;
	m_Serial.Purge();
	{
		DWORD length1;
		do { 
			length1 = 1;
			m_Serial.Read( &bufferx,100,&length1,0,100);
		} while( length1 ) ;
	}
}
char CPickobearDlg::CheckX( void )
{
	DWORD length = 0;
	unsigned char ch;
	int ret;

	// wait for ack. needs a timeout

	int counter = 500;

	if( m_Simulate == true ) {
		return true;
	}

	do { 

		counter --;

		ret = m_Serial.Read( &ch, 1, &length );

		if( length == 0 ) 
			Sleep( 100 );

		switch ( ret ) {

		case ERROR_SUCCESS:
			_RPT1(_CRT_WARN,"%c ", ch);
			break;
		default:
			break;
		}

	} while( ch!='X' && (counter ) );

	if( counter == 0 ) 
		_RPT1(_CRT_WARN,"\r\nCheckX: Timed out\n", ch);
	else
		_RPT1(_CRT_WARN,"\r\nCheckX: CheckAck(%c)\n", ch);

	if( ch == 'X' && ( counter > 0 ) ) {

		return true;
	}

	return false;

}

char CPickobearDlg::CheckAck( char *ack1 )
{
	unsigned int length = 1;

	unsigned int index ;
	unsigned char ch;
	int ret;

	unsigned char buffer[ 10 ];

	if( m_Simulate == true ) 
		return 'p';

	memset(buffer,0,sizeof(buffer));

	ASSERT( ack1 );
	if( ack1 == NULL ){
		return 'p';
	}
	index = 0;

	int counter = 1000;

	// wait for ack. needs a timeout
	do { 
		counter -- ;
		ret = m_Serial.Read( &ch, length );
		if ( index < sizeof( buffer ) ) {

			if( !( ch == 13 || ch == 10 ) ) 
				buffer[ index++ ] = ch;

		}

	} while(ch!='\n' && (counter != 0 ));

	if (counter == 0 ) {
		_RPT1(_CRT_WARN,"Timed out\n", buffer);
		return 'f';
	}else
		_RPT1(_CRT_WARN,"CheckAck(%s)\n", buffer);

	length = strlen( ack1 ) ;

	if(0==_strnicmp((const char *)ack1,(const char *)&buffer[0], length   )  ) {

		return 'p';
	}

	return 'f';

}


void CPickobearDlg::OnBnClickedPark()
{
	MoveHead( 364000,517000 );
}


void CPickobearDlg::GotoTestPad( void )
{
	MoveHeadSlow(pulsestoum(12308),pulsestoum(5002));
}

unsigned char CPickobearDlg::VacuumTest( void )
{	
	EmptySerial();

	do { 

		WriteSerial("M22\r\n");

	}while( 'p' != CheckAck("ok"));

	// wait for reply
	if( CheckAck("n") == 'p'  ) {
		return 0;
	} 

	return 1;
}

void CPickobearDlg::OnBnClickedTool1()
{
	if( false == HomeTest( ) ) {
		return ;
	}

	WriteSerial("M24\r\n");
}


void CPickobearDlg::OnBnClickedTool2()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	WriteSerial("M24\r\n");
}

void CPickobearDlg::OnBnClickedTool3()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	WriteSerial("M24\r\n");
}


void CPickobearDlg::OnBnClickedTool4()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	WriteSerial("M24\r\n");
}


void CPickobearDlg::OnBnClickedTool5()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	WriteSerial("M24\r\n");
}


void CPickobearDlg::OnBnClickedTool6()
{
	if( false == HomeTest( ) ) {
		return ;
	}
	WriteSerial("M24\r\n");
}

void CPickobearDlg::OnBnClickedHead()
{
	if( false == HomeTest( ) ) {
		return ;
	}

	if( m_Head ) {
		// up
		WriteSerial("M11\r\n");
		m_Head = 0;
	} else { 
		//down
		WriteSerial("M10\r\n");
		m_Head = 1;
	}
}


void CPickobearDlg::OnNMCustomdrawRotate(NMHDR *pNMHDR, LRESULT *pResult)
{
	static int firstTime = 1;

	if (firstTime == 1 ) {
		firstTime = 0 ;
		return;
	}

	if( false == HomeTest( ) ) {
		return ;
	}

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	static int rot = 0 ;

	if( m_Serial.IsOpen() && m_Rotation.GetPos() > 0 ) {
		WriteSerial("G0H10\r\n");
	}

	_RPT1(_CRT_WARN,"rotation = %d\n",m_Rotation.GetPos() ) ;


}

void CPickobearDlg::OnBnClickedUp()
{
	long stepsize = (m_StepSize.GetCurSel() * 10);
	_RPT1(_CRT_WARN,"StepSize %d\n",stepsize);
	if( stepsize == 0 ) stepsize = 1;

	m_headYPos += stepsize;

	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedDown()
{
	long stepsize = (m_StepSize.GetCurSel() * 10);
	_RPT1(_CRT_WARN,"StepSize %d\n",stepsize);
	if( stepsize == 0 ) stepsize = 1;

	m_headYPos -= stepsize;

	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedLeft()
{
	long stepsize = (m_StepSize.GetCurSel() * 10);
	_RPT1(_CRT_WARN,"StepSize %d\n",stepsize);
	if( stepsize == 0 ) stepsize = 1;

	m_headXPos -= stepsize;

	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedRight()
{
	long stepsize = (m_StepSize.GetCurSel() * 10);
	_RPT1(_CRT_WARN,"StepSize %d\n",stepsize);
	if( stepsize == 0 ) stepsize = 1;

	m_headXPos += stepsize;

	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedUpleft()
{
	long stepsize = (m_StepSize.GetCurSel() * 10);
	_RPT1(_CRT_WARN,"StepSize %d\n",stepsize);
	if( stepsize == 0 ) stepsize = 1;

	m_headXPos -= stepsize;
	m_headYPos += stepsize;

	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedUpright()
{
	long stepsize = (m_StepSize.GetCurSel() * 10);

	_RPT1(_CRT_WARN,"StepSize %d\n",stepsize);

	if( stepsize == 0 ) 
		stepsize = 1;

	m_headXPos += stepsize;
	m_headYPos += stepsize;

	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedLeftdown()
{
	long stepsize = (m_StepSize.GetCurSel() * 10);
	_RPT1(_CRT_WARN,"StepSize %d\n",stepsize);
	if( stepsize == 0 ) 
		stepsize = 1;


	m_headXPos -= stepsize;
	m_headYPos += stepsize;

	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedBottomleft()
{
	long stepsize = (m_StepSize.GetCurSel() * 10);
	_RPT1(_CRT_WARN,"StepSize %d\n",stepsize);
	if( stepsize == 0 ) 
		stepsize = 1;

	m_headXPos -= stepsize;
	m_headYPos -= stepsize;

	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedLoad()
{
	m_ComponentList.LoadDatabase( L"" );
}

void CPickobearDlg::OnBnClickedSave()
{
	m_ComponentList.SaveDatabase();
}

void CPickobearDlg::OnBnClickedImport()
{
	CString m_LoadFile = ::GetLoadFile( 
		_T("Supported Files Types(*.csv)\0*.csv\0\0"),
		_T("Pick board to load"),
		NULL
		);

	m_ComponentList.DeleteAllItems();

	m_ComponentList.m_ComponentDatabase.clear();

	const char field_terminator = ',';
	const char line_terminator  = '\n';
	const char enclosure_char   = '\"';

	csv_parser file_parser;
	/* Define how many records we're gonna skip. This could be used to skip the column definitions. */
	file_parser.set_skip_lines(1);

	/* Specify the file to parse */
	file_parser.init( 
		CStringA(m_LoadFile.GetString()) 
		);

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

DWORD WINAPI CPickobearDlg::goCamera(LPVOID pThis)
{
	return ((CPickobearDlg*)pThis)->cameraThread();
}

DWORD CPickobearDlg::cameraThread(void )
{
	while( m_Quit == 0 ) {
		m_UpCameraWindow.UpdateCamera( 1 ) ;

		Sleep( m_CameraUpdateRate );

		//m_DownCameraWindow.UpdateCamera( 1 ) ;
		//		Sleep(1);

	}
	return true;
}

DWORD WINAPI CPickobearDlg::goSetup(LPVOID pThis)
{
	return ((CPickobearDlg*)pThis)->goThread();
}

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

void CPickobearDlg::OnBnClickedGo()
{
	static HANDLE h=0;

	UpdateData();

	if ( m_MachineState == MS_GO  ) { 
		m_MachineState = MS_STOP;
		return;
	}

	// do a check on feeders, tools etc here

	if( PreRunCheck( true ) == false ) {
		return;
	}


	// switch state to GO
	m_MachineState = MS_GO ;

	// Start thread for 'GO'
	h = CreateThread(NULL, 0, &CPickobearDlg::goSetup, (LPVOID)this, 0, NULL);
}

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

void CListCtrl_Components::OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	*pResult = 0;

	if( false == pDlg->HomeTest( ) ) {
		return ;
	}

	// iItem is item number, list is backwards
	int componentItem = (GetCount()-1)-phdr->iItem;

	CStringA text ( GetItemText( componentItem, 0 )) ;

	_RPT1(_CRT_WARN,"OnHdnItemdblclickList2: part %s\n",text);

	componentItem = Search( text.GetBuffer() ) ;

	text.ReleaseBuffer();

	if( componentItem == -1 ) {
		AfxMessageBox(L"OnHdnItemdblclickList2: Internal error part not found!",MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

	entry = &m_ComponentDatabase.at( componentItem ) ;

	// entry is item.
	ASSERT( entry );

	if( m_OffsetX == 0 && m_OffsetY == 0 ) {

		pDlg->MoveHead(36240,222890);

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

void CListCtrl_FeederList::OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	*pResult = 0;
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	// iItem is item number, list is backwards
	int item = (GetCount()-1) - phdr->iItem;

	entry = &mFeederDatabase.at( item ) ;
	// entry is item.
	ASSERT( entry );

	Feeder CurrentFeeder( m_Feeders.at( item ) );

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
}

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

bool SetCurrentPosition( long x,long y)
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;

	ASSERT( pDlg );

	if ( x < 0 ) 
		x = 0;

	if ( y < 0 )
		y = 0;

	unsigned int linePtr = 0;
	char xString[200];
	char yString[200];

	sprintf_s(xString,sizeof(xString),"%d",x);
	sprintf_s(yString,sizeof(xString),"%d",y);

	pDlg->m_GOX = x;
	pDlg->m_GOY = y;

	pDlg->m_headXPos = x;
	pDlg->m_headYPos = y;

	( ( CWnd* ) pDlg->GetDlgItem( IDC_X_POS ) )->SetWindowText( CString(xString) ) ;
	( ( CWnd* ) pDlg->GetDlgItem( IDC_Y_POS ) )->SetWindowText( CString(yString) ) ;

	return true;
}

void CPickobearDlg::OnBnClickedZero()
{
	MoveHead(0,0);

	return;
}

//  81775
// 236275

void CPickobearDlg::OnBnClickedOffset()
{
	ASSERT( m_ComponentList.entry );

	SetCurrentPosition(m_headXPos,m_headYPos);

	// set the offset.
	m_ComponentList.m_OffsetX = m_headXPos - m_ComponentList.entry->x; 

	// board is natural side up (top)
	if( bFlip == false ) {
		m_ComponentList.m_OffsetY = m_headYPos - (m_ComponentList.entry->y);

		m_ComponentList.m_OffsetX_top = m_ComponentList.m_OffsetX;
		m_ComponentList.m_OffsetY_top = m_ComponentList.m_OffsetY;

	} else {

		// board is upside down
		m_ComponentList.m_OffsetY = m_headYPos - (0-m_ComponentList.entry->y);

		m_ComponentList.m_OffsetX_bottom = m_ComponentList.m_OffsetX;
		m_ComponentList.m_OffsetY_bottom = m_ComponentList.m_OffsetY;
	}
	GetDlgItem( IDC_OFFSET )->EnableWindow( FALSE );
}

void CPickobearDlg::OnBnClickedGoff()
{
	MoveHead(m_ComponentList.m_OffsetX ,m_ComponentList.m_OffsetY );
}


void CPickobearDlg::OnBnClickedGoxy()
{
	MoveHead(m_GOX,m_GOY);
}


void CPickobearDlg::OnEnChangeGox()
{
	UpdateData(TRUE);
}


void CPickobearDlg::OnEnChangeGoy()
{
	UpdateData(TRUE);
}

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


void CPickobearDlg::OnBnClickedAddFeeder()
{
	CTextEditDialog TextDialog;

	TextDialog.DoModal();

	CStringA userInput8( UTF16toUTF8( TextDialog.m_Text ) );

	m_FeederList.AddItem(userInput8.GetString(),m_GOX, m_GOY, 0);
	userInput8.ReleaseBuffer();

}

void CPickobearDlg::OnBnClickedUpdate()
{
	SetCurrentPosition(m_headXPos,m_headYPos);
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

int CPickobearDlg::SendCommand( int command ) 
{
	switch( command  ) {

	case PNP_GOTO:
		break;

	case PNP_CURRENTPOS:

		SetCurrentPosition(m_headXPos,m_headXPos);
		break;

	default:
		break;
	}

	return 0;
}

void CListCtrl_Components::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
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
			
		_RPT2(_CRT_WARN,"OnNMRClickList2: assigning feeder %s to %s\r\n",pDlg->m_FeederList.mFeederDatabase.at( feederItem ).label, entry->label );

		pDlg->m_ComponentList.AssignFeeder( (GetCount()-1)-nItem, pDlg->m_FeederList.mFeederDatabase.at (feederItem ).label ) ;

	}

	// rebuild the list
	pDlg->m_ComponentList.RebuildList();
}

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

	// convert to level of accuracy pnp can handle
	entry.x = (( x )/40)*40 ;
	entry.y = (( y )/40)*40 ;

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

void CPickobearDlg::OnBnClickedLoadFeeder()
{
	m_FeederList.LoadDatabase(L"");
}

void CPickobearDlg::OnBnClickedSaveFeeder()
{
	m_FeederList.SaveDatabase();
}

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
	m_FeederList.at( item ).lx = m_headXPos;
	m_FeederList.at( item ).ly = m_headXPos;

	CCounts CountDialog;

	CountDialog.DoModal();

	// set counts
	// this updates the database, but not the list
	m_FeederList.at( item ).countx = CountDialog.m_CountX;
	m_FeederList.at( item ).county = CountDialog.m_CountY;

	// rebuild the list
	m_FeederList.RebuildList();

}

void CPickobearDlg::OnCbnSelchangeStepsize()
{
	CRegKey regKey;
	DWORD regEntry = 100;
	long lResult;

	UpdateData();

	if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

	if (ERROR_SUCCESS == lResult)
	{
		// Make 'regEntry' equal to zero
		regEntry = m_StepSize.GetCurSel();

		// Save a value in the registry key
		regKey.SetDWORDValue(_T("stepSize"), regEntry);

		// then close the registry key
		regKey.Close();
	}
}


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

	FeederDialog.DoModal();

	CStringA userInput8( UTF16toUTF8( FeederDialog.m_FeederName ) );

	// Since CString doesn't like being in a vector for load/save then we're using a char array, which makes this bit ugly
	strcpy_s( FeederDialog.entry.label, userInput8 );
	userInput8.ReleaseBuffer();

	m_FeederList.mFeederDatabase.at( clist ) = FeederDialog.entry;

	//ugly
	m_FeederList.RebuildList();
}


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


void CPickobearDlg::OnBnClickedConsole()
{
	static int nconsole = 0;

	if( nconsole == 0 )
		m_TextEdit->ShowWindow( SW_SHOW );
	else
		m_TextEdit->ShowWindow( SW_HIDE );

	nconsole = 1 - nconsole;
}


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

	// switch state to GO
	m_MachineState = MS_GO ;

	bBusy = true;

	UpdateData(FALSE);

	// Start thread for 'GO'
	h = CreateThread(NULL, 0, &CPickobearDlg::goSingleSetup, (LPVOID)this, 0, NULL);
}

DWORD WINAPI CPickobearDlg::goSingleSetup(LPVOID pThis)
{
	return ((CPickobearDlg*)pThis)->goSingleThread();
}


void CPickobearDlg::OnBnClickedSwapHeadCamera()
{
	if( bCameraHead == false ) {
		MoveHeadRel( 0, -CAMERA_OFFSET) ;
		bCameraHead = true;
	} else { 
		MoveHeadRel( 0, CAMERA_OFFSET) ;
		bCameraHead = false;
	}
}



void CPickobearDlg::OnBnClickedEditComponent()
{
	EditComponent EditDialog;

	int item = m_ComponentList.GetSelectedCount();

	if( item == 0 ) {
		return ;
	}

	item = m_ComponentList.GetNextItem( -1, LVNI_SELECTED );

	// iItem is item number, database is backwards
	int clist = (m_ComponentList.GetCount()-1) - m_ComponentList.GetNextItem(-1, LVNI_SELECTED);

	EditDialog.entry = m_ComponentList.m_ComponentDatabase.at( clist ) ;

	// push the names out
	EditDialog.m_Name    = EditDialog.entry.label;
	EditDialog.m_Value   = EditDialog.entry.value;
	EditDialog.m_Type    = EditDialog.entry.type;
	EditDialog.m_Feeder  = EditDialog.entry.feeder;

	EditDialog.DoModal();

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


	m_ComponentList.m_ComponentDatabase.at( clist ) = EditDialog.entry;

	//ugly
	m_ComponentList.RebuildList();
}



DWORD CPickobearDlg::goThread(void )
{
	Feeder CurrentFeeder;

	unsigned int i ;

	CListCtrl_Components::CompDatabase entry; 

	_RPT0(_CRT_WARN,"goThread\r\n");

	if ( bBusy ) {
		AfxMessageBox(L"Machine is already running a task",MB_OK|MB_ICONEXCLAMATION);
		return true;
	}

	bBusy = true;

	EmptySerial();


	do {

		// check fo more than one PCB (these should be equal!)
		if (m_PCBs.size() ) {

			_RPT2(_CRT_WARN,"Multiple PCBs placing board ( %d ) of ( %d )\n", m_PCBIndex , m_PCBCount);

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

			if( entry.side !=  m_Side ) {

				_RPT1(_CRT_WARN,"skipping %s, wrong side selected\n",entry.label);
				goto skip_part;
			} else {
				_RPT1(_CRT_WARN,"this side %s selected\n",entry.label);
			}

			if ( strlen( entry.feeder ) == 0 ) {

				int ret = AfxMessageBox(L"Feeder not defined", MB_OK|MB_ICONHAND);

				// move back
				if( m_PCBIndex > 0 ) 
					m_PCBIndex--;
				
				bBusy = false;
				return false;

				goto skip_part;
			}

			CListCtrl_FeederList::FeederDatabase feeder;

			int feederEntry = m_FeederList.Search( entry.feeder  );

			int feederIndex ;
			feederIndex = (m_FeederList.GetCount()-1) - feederEntry;

			feeder = m_FeederList.at ( feederIndex );
			// duplication as we change over to the new class
			CurrentFeeder = m_FeederList.m_Feeders.at( feederIndex );

			if (feeder.tool < 1 || feeder.tool > 6 ) {
				int ret = AfxMessageBox(L"Tool not defined", MB_OK|MB_ICONHAND);
				

				goto skip_part;
			}

			_RPT1(_CRT_WARN,"Going to tool %d\n", feeder.tool );

			// can't tool change at the moment...
#if 0
			switch ( feeder.tool ) {
			case 1:
				WriteSerial("M24\r\n");
				break;
			case 2:
				WriteSerial("M24\r\n");
				break;
			case 3:
				WriteSerial("M24\r\n");
				break;
			case 4:
				WriteSerial("M24\r\n");
				break;
			case 5:
				WriteSerial("M24\r\n");
				break;
			case 6:
				WriteSerial("M24\r\n");
				break;
			}

			return true;
#endif


			/// slow the camera down
			m_CameraUpdateRate = CAMERA_SLOW_UPDATE_RATE_MS ;

			_RPT1(_CRT_WARN,"Going to feeder %s\n", feeder.label );

			unsigned long feederX,feederY;

			if( CurrentFeeder.GetNextPartPosition( feederX, feederY ) ) {

				MoveHead(feederX , feederY - CAMERA_OFFSET );

				// next part
				if( CurrentFeeder.AdvancePart() == false ) {

					CString Error ;			
					Error = CurrentFeeder.ErrorMessage();
					int ret = AfxMessageBox(Error ,MB_OK|MB_ICONEXCLAMATION );

					bBusy = false;

					m_FeederList.RebuildList();

				// move back
				if( m_PCBIndex > 0 ) 
					m_PCBIndex--;

					return 0;
				}
				// reflect quantities
				m_FeederList.RebuildList();


			} else {

				CString Error ;			
				Error = CurrentFeeder.ErrorMessage();
				int ret = AfxMessageBox(Error ,MB_OK|MB_ICONEXCLAMATION );

				bBusy = false;

				// move back
				if( m_PCBIndex > 0 ) 
					m_PCBIndex--;

				return 0;
			}

			_RPT0(_CRT_WARN,"Picking up part\n");
			WriteSerial("M26\r\n");

			Sleep( 1000 );

			_RPT1(_CRT_WARN,"Going to component %s\n", entry.label );

			if( bFlip == false ) {
				MoveHead(entry.x+m_ComponentList.m_OffsetX, entry.y+m_ComponentList.m_OffsetY - CAMERA_OFFSET);
			} else {
				MoveHead(entry.x+m_ComponentList.m_OffsetX, (0-entry.y)+m_ComponentList.m_OffsetY - CAMERA_OFFSET);
			}

			if( entry.rot ) {

				_RPT1(_CRT_WARN,"Rotating part %d degrees \n", entry.rot );

				double angle = entry.rot;

				// calculate pulses
				angle = ( 1000.0 / 360.0  ) * angle ;

				char buffer[256];
				int pulses;

				// calculate pulses
				pulses = ( int) ( angle );
				sprintf_s(buffer,sizeof(buffer),"G0H%d\r\n", pulses );

				_RPT1(_CRT_WARN,"Executing GCODE %s\r\n",buffer);

				// do the rotate
				WriteSerial( buffer );
				Sleep( 500 );
			}

			_RPT0(_CRT_WARN,"dropping off part\n");

			// head down/air off/up
			// Put Part down
			WriteSerial("M27\r\n");

			//wait
			Sleep( 100 );


			// comes here if the part is skipped for some reason
skip_part:;

			if( m_MachineState == MS_STOP ) {

				m_MachineState = MS_IDLE;
			
				// its an emergency stop, what else do you want
				goto stop;
			}

			UpdateWindow();

		}

	// looop for all PCB's
	} while(m_PCBCount, m_PCBIndex < m_PCBCount ) ;

	// Park machine
	WriteSerial("G1X14Y15F200\r\n");


stop:;

	/// reset the camera speed 
	m_CameraUpdateRate = CAMERA_DEFAULT_UPDATE_RATE_MS ;

	// switch state to idle
	m_MachineState = MS_IDLE ;
	
	bBusy = false;

	return true;
}

/*


// get first item in feeder list ( only one can be selected
int feederItem = pDlg->m_FeederList.GetNextItem(-1, LVNI_SELECTED);

for( int i = 0 ; i < numberComponentsSelected ; i++ ) {
// nItem is component index selected, -1 first time
nItem = pDlg->m_ComponentList.GetNextItem(nItem, LVNI_SELECTED);
if( nItem == -1 ) {
break;
}


*/


DWORD CPickobearDlg::goSingleThread(void )
{
	Feeder CurrentFeeder;

	unsigned int i ;
	CListCtrl_Components::CompDatabase entry; 


	EmptySerial();

	Sleep( 500 ) ;

	// find number of parts selected
	int numberComponentsSelected = m_ComponentList.GetSelectedCount();

	// nothing selected
	if( numberComponentsSelected == 0 ) {
		
		bBusy = false;
		
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


		i = (m_ComponentList.GetCount()-1) - componentItem;

		entry = m_ComponentList.at(i);

		if( entry.side !=  m_Side ) {

			_RPT1(_CRT_WARN,"goSingleThread: skipping %s, wrong side selected\n",entry.label);
			goto skip;
		}

		_RPT1(_CRT_WARN,"goSingleThread: Placing %s\r\n",entry.label);

		if (strlen( entry.feeder) == 0) {
			int ret = AfxMessageBox(L"Feeder not defined", MB_OK|MB_ICONEXCLAMATION);
			
			bBusy = false;
			
			return 0;
		}

		CListCtrl_FeederList::FeederDatabase feeder ;

		int feederEntry = m_FeederList.Search( entry.feeder  );
		if( feederEntry == -1 ) {

			int ret = AfxMessageBox(L"Feeder not found ", MB_OK | MB_ICONEXCLAMATION);

			bBusy = false;

			return 0;
		}

		int feederIndex ;
		feederIndex = (m_FeederList.GetCount()-1) - feederEntry;

		feeder = m_FeederList.at ( feederIndex );

		// duplication as we change over to the new class
		CurrentFeeder = m_FeederList.m_Feeders.at( feederIndex );


		if (feeder.tool < 1 || feeder.tool > 6 ) {
			int ret = AfxMessageBox(L"Tool not defined", MB_OK|MB_ICONHAND);
		}

		int in = (m_ComponentList.GetCount()-1)-i;
		m_ComponentList.SetItemState(in, LVIS_SELECTED, LVIS_SELECTED);
		m_ComponentList.EnsureVisible( in ,TRUE );

		_RPT1(_CRT_WARN,"goSingleThread: Going to tool %d\n", feeder.tool );

		// can't tool change at the moment...
#if 0
		switch ( feeder.tool ) {
		case 1:
			WriteSerial("M24\r\n");
			break;
		case 2:
			WriteSerial("M24\r\n");
			break;
		case 3:
			WriteSerial("M24\r\n");
			break;
		case 4:
			WriteSerial("M24\r\n");
			break;
		case 5:
			WriteSerial("M24\r\n");
			break;
		case 6:
			WriteSerial("M24\r\n");
			break;
		}

		return true;
#endif

		/// slow the camera down
		m_CameraUpdateRate = CAMERA_SLOW_UPDATE_RATE_MS ;

		_RPT1(_CRT_WARN,"goSingleThread: Going to feeder %s\n", feeder.label );

		unsigned long feederX,feederY;

		if( CurrentFeeder.GetNextPartPosition( feederX, feederY ) ) {

			MoveHead(feederX , feederY - CAMERA_OFFSET );
			
			// next part
			CurrentFeeder.AdvancePart();

			// reflect quantities
			m_FeederList.RebuildList();

		} else {

			CString Error ;			
			Error = CurrentFeeder.ErrorMessage();
			int ret = AfxMessageBox(Error ,MB_OK | MB_ICONHAND);

			bBusy = false;

			return 0;
		}

		_RPT0(_CRT_WARN,"vPicking up part\n");

		// Pickup
		WriteSerial("M26\r\n");

		Sleep( 1000 );

		_RPT1(_CRT_WARN,"goSingleThread: Going to component %s\n", entry.label );

	
		if( bFlip == false ) {
			MoveHead(entry.x+m_ComponentList.m_OffsetX, entry.y+m_ComponentList.m_OffsetY - CAMERA_OFFSET);
		} else {
			MoveHead(entry.x+m_ComponentList.m_OffsetX, (0-entry.y)+m_ComponentList.m_OffsetY - CAMERA_OFFSET);
		}

		if( entry.rot ) {

			_RPT1(_CRT_WARN,"goSingleThread: Rotating part %d degrees \n", entry.rot );

			double angle = entry.rot;

			// calculate pulses
			angle = ( 1000.0 / 360.0  ) * angle ;

			char buffer[256];
			int pulses;

			// calculate pulses
			pulses = ( int) ( angle );
			sprintf_s(buffer,sizeof(buffer),"G0H%d\r\n", pulses );

			_RPT1(_CRT_WARN,"goSingleThread: Executing GCODE %s\r\n",buffer);

			// do the rotate
			WriteSerial( buffer );
			Sleep( 500 );


		}
		// head down/air off/up
		_RPT0(_CRT_WARN,"goSingleThread: dropping off part\n");

		// Put Part down
		WriteSerial("M27\r\n");


		//wait
		Sleep( 100 );

skip:;

		if( m_MachineState == MS_STOP ) {

			m_MachineState = MS_IDLE;

			/// reset the camera update rate
			m_CameraUpdateRate = CAMERA_DEFAULT_UPDATE_RATE_MS ;

			bBusy = false;

			return true ;
		}

		UpdateWindow();

		Sleep( 500 );

		// Move Camera to part
		if( bFlip == false ) {
			MoveHead(entry.x+m_ComponentList.m_OffsetX, entry.y+m_ComponentList.m_OffsetY );
		} else {
			MoveHead(entry.x+m_ComponentList.m_OffsetX, (0-entry.y)+m_ComponentList.m_OffsetY );
		}
	}

	// switch state to idle
	m_MachineState = MS_IDLE ;

	// no longer busy
	bBusy  =  false;

	/// reset  the camera speed
	m_CameraUpdateRate = CAMERA_DEFAULT_UPDATE_RATE_MS ;


	return true;
}


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


void CPickobearDlg::OnBnClickedVacuumToggle()
{
	if( false == HomeTest( ) ) {
		return ;
	}

	if( m_Vacuum ) {
		// off
		WriteSerial("M20\r\n");
		m_Vacuum = 0;
	} else { 
		//down
		WriteSerial("M19\r\n");
		m_Vacuum = 1;
	}
}


void CPickobearDlg::OnCbnSelchangeGSpeed()
{
	UpdateData(FALSE);

	m_Speed = ((m_SpeedSelect.GetCurSel()+1) * 100);
	_RPT1(_CRT_WARN,"m_Speed %d\n",m_Speed);
}


void CPickobearDlg::OnStnClickedSimulation()
{
}


void CPickobearDlg::OnBnClickedAddPcb()
{
	CString temp;
	PCBEntry entry;
	PCBEntry check;
	
	if( m_ComponentList.m_OffsetX_top == 0  && m_ComponentList.m_OffsetY_top == 0  ) {
		AfxMessageBox(L"Error:Can't add a PCB at (0,0)",MB_OK|MB_ICONHAND);
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

			if ( memcmp( &entry, &check,sizeof( PCBEntry ) )== 0  )  {
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
