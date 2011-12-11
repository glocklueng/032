//
// PickobearDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "PickobearDlg.h"

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
	, m_GOX(0)
	, m_GOY(0)
	, m_pFeederDlg( NULL )
	, m_Quit(0)
	, m_Speed(700)
	, m_CameraMode(1)
	, m_Homed(false)
	, m_CameraUpdateRate(10)
	, bSetWaitDone(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPickobearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GO, GO);
	DDX_Text(pDX, IDC_X_POS, m_headXPos);
	DDX_Text(pDX, IDC_Y_POS, m_headYPos);
	DDX_Control(pDX, IDC_ROTATE, m_Rotation);
	DDX_Control(pDX, IDC_LIST2, m_ComponentList);
	DDX_Text(pDX, IDC_GOX, m_GOX);
	DDV_MinMaxLong(pDX,IDC_GOX,0,364550);
	DDX_Text(pDX, IDC_GOY, m_GOY);
	DDX_Control(pDX, IDC_LIST3, m_FeederList);
	DDX_Control(pDX, IDC_COMBO1, m_UpCamera);
	DDX_Control(pDX, IDC_COMBO2, m_DownCamera);
	DDX_Control(pDX, IDC_STEPSIZE, m_StepSize);

}

BEGIN_MESSAGE_MAP(CPickobearDlg, CDialog) 
	ON_WM_SERIAL(OnSerialMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_LIST1, &CPickobearDlg::OnLbnSelchangeList1)
	ON_EN_KILLFOCUS(IDC_THRESHOLD, &CPickobearDlg::OnEnChangeThreshold)
	ON_EN_KILLFOCUS(IDC_THRESHOLD2, &CPickobearDlg::OnEnChangeThreshold2)
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
	ON_BN_CLICKED(IDC_UPDATE2, &CPickobearDlg::OnBnClickedH2C)
	ON_BN_CLICKED(IDC_UPDATE3, &CPickobearDlg::OnBnClickedC2H)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPickobearDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CPickobearDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_ADD_LOWERRIGHT, &CPickobearDlg::OnBnClickedAddLowerright)
	ON_CBN_SELCHANGE(IDC_STEPSIZE, &CPickobearDlg::OnCbnSelchangeStepsize)
	ON_BN_CLICKED(IDC_EDITFEEDER, &CPickobearDlg::OnBnClickedEditfeeder)
	ON_BN_CLICKED(IDC_ASSIGNFEEDER, &CPickobearDlg::OnBnClickedAssignfeeder)
	ON_BN_CLICKED(IDC_CONSOLE, &CPickobearDlg::OnBnClickedConsole)
	ON_BN_CLICKED(IDC_GO2, &CPickobearDlg::OnBnClickedGo2)
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

	m_ComponentList.InsertColumn(0, _T("Name"),LVCFMT_CENTER,nInterval/1.2);
	m_ComponentList.InsertColumn(1, _T("Value"),LVCFMT_CENTER,nInterval/1.5);
	m_ComponentList.InsertColumn(2, _T("Type"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(3, _T("X"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(4, _T("Y"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(5, _T("R"),LVCFMT_CENTER,nInterval/2);
	m_ComponentList.InsertColumn(6, _T("Feeder"),LVCFMT_CENTER,nInterval*1.8);
	m_ComponentList.InsertColumn(7, _T("L"),LVCFMT_CENTER,nInterval);

	m_Rotation.SetRange(0,360);
		
	m_FeederList.GetClientRect(&rect);
	nInterval =( rect.Width() / 9 );

	m_FeederList.InsertColumn(0, _T("Name"),LVCFMT_CENTER,nInterval+(nInterval/1.3));
	m_FeederList.InsertColumn(1, _T("X"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(2, _T("Y"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(3, _T("Rot"),LVCFMT_CENTER,nInterval/1.4);

	m_FeederList.InsertColumn(4, _T("LX"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(5, _T("LY"),LVCFMT_CENTER,nInterval);

	m_FeederList.InsertColumn(6, _T("CX"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(7, _T("CY"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(8, _T("T"),LVCFMT_CENTER,nInterval/1.5);


	m_UpCamera.ResetContent();
	m_DownCamera.ResetContent();
		
	int numDevices = VI.listDevices();
		

	for( int i = 0 ; i < numDevices ; i++ ) {
	
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

	if ( regEntry > m_StepSize.GetCount()-1 ) 
		regEntry = 0;
	
	if( CB_ERR  == m_StepSize.SetCurSel( regEntry ) ) {
		_RPT0(_CRT_WARN,"Error\n");
	}
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

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPickobearDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPickobearDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

void CPickobearDlg::OnEnChangeThreshold()
{

	UpdateData(TRUE);
	m_Thresh1 = m_Threshold1;

}

void CPickobearDlg::OnEnChangeThreshold2()
{
	UpdateData(TRUE);
	m_Thresh2 = m_Threshold2;
}

void CPickobearDlg::WriteSerial( const char *text)
{
	ASSERT( text );
	CString out( text  );
	m_TextEdit->Print( out ) ;
	m_Serial.Write( text );
}

void CPickobearDlg::OnBnClickedHome()
{	
	bool pass = false;

	do { 
		WriteSerial("G28\r\n");

		Sleep( 100 );

		switch( CheckAck("ok") )
		{
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
		
		int ret = AfxMessageBox(L"X out of limits, press OK to change to stay within limits, or cancel to stop move",MB_OKCANCEL);
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
		int ret = AfxMessageBox(L"Not Homed",MB_OK);
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
	m_headYPos = y;

	if ( false == BuildGCodeMove(buffer,sizeof(buffer),1,x,y,100) ) {
		return false;
	}
again:;

//	_RPT5(_CRT_WARN,"current pos = %dum,%dum\nGoing to %dum,%dum\n%s\n",m_headXPos,m_headYPos,x,y,buffer);
	WriteSerial(buffer);

	if( CheckX() == false ) {
		int ret = AfxMessageBox(L"MoveHeadSlow: Move Failed",MB_RETRYCANCEL);
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
		m_headYPos += ( 73740 );
	} else {
		m_headYPos -= ( 73740 );
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
		int ret = AfxMessageBox(L"MoveHead: Move Failed",MB_RETRYCANCEL);
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

	if(false == BuildGCodeMove(buffer,sizeof(buffer),0,m_headXPos+x,m_headYPos+y,100) ) 
		return false;

again:;

	WriteSerial( buffer );

	if( CheckX() == false ) {
		int ret = AfxMessageBox(L"MoveHeadRel: Move Failed",MB_RETRYCANCEL);
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
	
	int counter = 100;

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
		// down
		WriteSerial("M10\r\n");
		m_Head = 0;
	} else { 
		//up
		WriteSerial("M11\r\n");
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
	int i;
	const char seps[] = ",";
	char* token,*next;
	CFile m_File;

	static char list[10][1024];
	static char buffer[1024];

	CString m_LoadFile = ::GetLoadFile( 
		_T("Supported Files Types(*.csv)\0*.csv\0\0"),
		_T("Pick board to load"),
		NULL
	);

	if(  FALSE == m_File.Open( m_LoadFile  ,CFile::modeRead ) ) { 
		return;
	}
	m_ComponentList.DeleteAllItems();

	do {
		
		ZeroMemory( buffer, sizeof( buffer ) );

		for( i = 0 ; i < sizeof( buffer) - 1 ; i++ ) {
	
			// if ended
			if( 0 == m_File.Read( &buffer[i],1) ) {
				
				m_File.Close();

				return;
			}

			// line end
			if( buffer[i] == '\n')
				break;
		}

		// Terminate last character
		buffer[i] = 0;
		next = NULL;

		token = strtok_s( buffer, seps, &next);
	
		i = 0;

		while (token != NULL) {

			if( 1 == sscanf(token, "%s", &list[i] ) ) {
				token = strtok_s (NULL, seps, &next);
				i++;
				
				if( i == 7 ) {
					// too many inputs
					token = NULL;
				}

			} else {
				// error in input
				token = NULL;
			}
		}

		m_ComponentList.AddItem(list[0], list[1], list[2], list[3], list[4], list[5],list[6] );

	}	while( 1 ) ;

	m_File.Close();	
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

DWORD CPickobearDlg::goThread(void )
{
	static int busy = 0;

	unsigned int i ;
	char buffer[5];
	CListCtrl_Components::CompDatabase entry; 
	
	ZeroMemory(buffer,sizeof(buffer));

	if ( busy ) {
		return true;
	}

	busy = 1;

	EmptySerial();

	for (i = 0 ; i < m_ComponentList.GetCount(); i ++ ) {
		
		entry = m_ComponentList.at(i);
		
		if (entry.feeder == -1) {
			int ret = AfxMessageBox(L"Feeder not defined", MB_OK);
		}

		CListCtrl_FeederList::FeederDatabase feeder = m_FeederList.at ((m_FeederList.GetCount()-1) - entry.feeder );

		if (feeder.tool < 1 || feeder.tool > 6 ) {
			int ret = AfxMessageBox(L"Tool not defined", MB_OK);
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
	
		_RPT1(_CRT_WARN,"Going to feeder %s\n", feeder.label );

		MoveHead(feeder.x, feeder.y  - 73740 );

		Sleep( 100 );
		
		// This will stall the machine till it can knock
		WriteSerial("M21\r\n");
		Sleep( 100 );

		_RPT0(_CRT_WARN,"Picking up part\n");
		// head down
		WriteSerial("M10\r\n");
		//wait
		Sleep( 500 );
		// vacuum on
		WriteSerial("M19\r\n");
		//wait
		Sleep( 500 );
		// head up
		WriteSerial("M11\r\n");
		//wait
		Sleep( 500 );

		_RPT1(_CRT_WARN,"Going to component %s\n", entry.label );
		 
		MoveHead(entry.x+m_ComponentList.m_OffsetX,
			     entry.y+m_ComponentList.m_OffsetY - 73740);
		

		int in = (m_ComponentList.GetCount()-1)-i;
		m_ComponentList.SetItemState(in, LVIS_SELECTED, LVIS_SELECTED);
		m_ComponentList.EnsureVisible( in ,TRUE );

		// head down/air off/up
		_RPT0(_CRT_WARN,"dropping off part\n");
		// head down
		WriteSerial("M10\r\n");
		//wait
		Sleep( 800 );
		// vacuum off
		WriteSerial("M20\r\n");
		//wait
		Sleep( 500 );
		// head up
		WriteSerial("M11\r\n");
		
		//wait
		Sleep( 100 );

		if( m_MachineState == MS_STOP ) {
			busy = 0;
			m_MachineState =MS_IDLE;
			break;
		}

		UpdateWindow();

	}
	 
	// Park machine
	WriteSerial("G1X14Y15F200\r\n");

	// switch state to idle
	m_MachineState = MS_IDLE ;
	busy = 0;

	return true;
}

void CPickobearDlg::OnBnClickedGo()
{
	static HANDLE h=0;

	if ( m_MachineState == MS_GO  ) { 
		m_MachineState = MS_STOP;
		return;
	}

	// switch state to GO
	m_MachineState = MS_GO ;

	// Start thread for 'GO'
	h = CreateThread(NULL, 0, &CPickobearDlg::goSetup, (LPVOID)this, 0, NULL);
}

void CPickobearDlg::OnBnClickedFeeder()
{
	m_pFeederDlg = new CFeederSetup(this);
	m_pFeederDlg->Create(CFeederSetup::IDD,GetDesktopWindow());
	m_pFeederDlg->ShowWindow(SW_SHOW);
}

void CListCtrl_Components::OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if( false == pDlg->HomeTest( ) ) {
		return ;
	}

	// iItem is item number, list is backwards
	int item = (GetCount()-1)-phdr->iItem;

	entry = &m_Database.at( item ) ;
	// entry is item.
	ASSERT( entry );

	if( m_OffsetX == 0 && m_OffsetY == 0 ) {
		
		pDlg->MoveHead(36960,222570);
	}else {
		// GotoXY in micrometers
		pDlg->MoveHead(entry->x+pDlg->m_ComponentList.m_OffsetX,entry->y+pDlg->m_ComponentList.m_OffsetY);
	}

	pDlg->GetDlgItem( IDC_OFFSET )->EnableWindow( TRUE );
}

void CListCtrl_FeederList::OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	// iItem is item number, list is backwards
	int item = (GetCount()-1)-phdr->iItem;

	entry = &m_Database.at( item ) ;
	// entry is item.
	ASSERT( entry );

	// GotoXY in micrometers
	pDlg->MoveHead(entry->x,entry->y);

}

bool SetCurrentPosition( long x,long y)
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;

	ASSERT( pDlg );
		
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;

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
	m_ComponentList.m_OffsetY = m_headYPos - m_ComponentList.entry->y;

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
	int feederItem = pDlg->m_FeederList.GetNextItem(-1, LVNI_SELECTED);

	for( int i = 0 ; i < numberComponentsSelected ; i++ ) {
		// nItem is component index slected, -1 first time
		nItem = pDlg->m_ComponentList.GetNextItem(nItem, LVNI_SELECTED);
		 if( nItem == -1 ) {
			 break;
		}
		entry = &m_Database.at( (GetCount()-1)-nItem ) ;

		pDlg->m_ComponentList.AssignFeeder( (GetCount()-1)-nItem, feederItem ) ;

	}

	// rebuild the list
	pDlg->m_ComponentList.RebuildList();
}


void CPickobearDlg::OnBnClickedLoadFeeder()
{
	m_FeederList.LoadDatabase(L"");
}

void CPickobearDlg::OnBnClickedSaveFeeder()
{
	m_FeederList.SaveDatabase();
}

// swap head and camera, this should be one function.
void CPickobearDlg::OnBnClickedH2C()
{
	m_CameraMode = 1 - m_CameraMode;

	MoveHead(m_headXPos,m_headYPos);
}


void CPickobearDlg::OnBnClickedC2H()
{
	MoveHead(m_headXPos,m_headYPos);
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

void CPickobearDlg::OnCbnSelchangeCombo2()
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

void CPickobearDlg::OnCbnSelchangeCombo1()
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

	FeederDialog.entry = m_FeederList.m_Database.at( clist ) ;

	FeederDialog.m_FeederName = FeederDialog.entry.label;

	FeederDialog.DoModal();

	CStringA userInput8( UTF16toUTF8( FeederDialog.m_FeederName ) );

	// Since CString doesn't like being in a vector for load/save then we're using a char array, which makes this bit ugly
	strcpy_s( FeederDialog.entry.label, userInput8 );
	userInput8.ReleaseBuffer();

	m_FeederList.m_Database.at( clist ) = FeederDialog.entry;

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

	int i;

	i = GetNextItem(-1, LVNI_SELECTED);

	DeleteAllItems();

	for ( unsigned int i = 0 ; i < m_Count ; i ++ ) {

		entry = &m_Database.at( i  ) ;

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

		if( entry->feeder == -1 ) { 
			temp.Format(L"NA");
		} else {
			// iItem is item number, list is backwards
			int item = (pDlg->m_FeederList.GetCount()-1) - entry->feeder;

			CListCtrl_FeederList::FeederDatabase entry = pDlg->m_FeederList.at( item ) ;

			temp.Format(L"%s",CString(entry.label));
		}

		SetItemText(Index,6,temp);
	
	}
	 
	
	 SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	 EnsureVisible( i ,TRUE );

}


void CPickobearDlg::OnBnClickedConsole()
{
	static int nconsole = 0;
	
	if( nconsole == 0 )
		m_TextEdit->ShowWindow(SW_SHOW);
	else
		m_TextEdit->ShowWindow(SW_HIDE);

	nconsole = 1 - nconsole;
}


void CPickobearDlg::OnBnClickedGo2()
{
	static HANDLE h=0;

	if ( m_MachineState == MS_GO  ) { 
		m_MachineState = MS_STOP;
		return;
	}

	// switch state to GO
	m_MachineState = MS_GO ;

	// Start thread for 'GO'
	h = CreateThread(NULL, 0, &CPickobearDlg::goSingleSetup, (LPVOID)this, 0, NULL);
}

 DWORD WINAPI CPickobearDlg::goSingleSetup(LPVOID pThis)
{
	return ((CPickobearDlg*)pThis)->goSingleThread();
}

 DWORD CPickobearDlg::goSingleThread(void )
 {
	 static int busy = 0;

	 unsigned int i ;
	 char buffer[5];
	 CListCtrl_Components::CompDatabase entry; 
	 
	 ZeroMemory(buffer,sizeof(buffer));

	 if ( busy ) {
		 return true ;
	 }

	 busy = 1;

	 EmptySerial();

	 Sleep( 500 ) ;

	 int componentItem = m_ComponentList.GetNextItem(-1, LVNI_SELECTED);

	 i = (m_ComponentList.GetCount()-1) - componentItem;

	 entry = m_ComponentList.at(i);

	 _RPT1(_CRT_WARN,"Placing %s\r\n",entry.label);

	 if (entry.feeder == -1) {
		 int ret = AfxMessageBox(L"Feeder not defined", MB_OK);
	 }

	 CListCtrl_FeederList::FeederDatabase feeder = m_FeederList.at ((m_FeederList.GetCount()-1) - entry.feeder );

	 if (feeder.tool < 1 || feeder.tool > 6 ) {
		 int ret = AfxMessageBox(L"Tool not defined", MB_OK);
	 }
	 
	 int in = (m_ComponentList.GetCount()-1)-i;
	 m_ComponentList.SetItemState(in, LVIS_SELECTED, LVIS_SELECTED);
	 m_ComponentList.EnsureVisible( in ,TRUE );

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
	 m_CameraUpdateRate = 20 ;

	 _RPT1(_CRT_WARN,"Going to feeder %s\n", feeder.label );

	 MoveHead(feeder.x, feeder.y  - 73740 );

	 _RPT0(_CRT_WARN,"Picking up part\n");

	 // Put Part down
	 WriteSerial("M26\r\n");

	 Sleep( 1000 );
	 
	_RPT1(_CRT_WARN,"Going to component %s\n", entry.label );
	
	 MoveHead(entry.x+m_ComponentList.m_OffsetX, entry.y+m_ComponentList.m_OffsetY - 73740);

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
	 // head down/air off/up
	 _RPT0(_CRT_WARN,"dropping off part\n");
	 
	 // Put Part down
	 WriteSerial("M27\r\n");


	 //wait
	 Sleep( 100 );

	 if( m_MachineState == MS_STOP ) {
		 busy = 0;
		 m_MachineState =MS_IDLE;
		 /// reset the camera update rate
		m_CameraUpdateRate = 10 ;
		
		 return true ;
	 }

	 UpdateWindow();

	 Sleep( 500 );

	 // Move Camera to part
	 MoveHead(entry.x+m_ComponentList.m_OffsetX,entry.y+m_ComponentList.m_OffsetY );

	 // switch state to idle
	 m_MachineState = MS_IDLE ;

	 // no longer busy
	 busy  = 0 ;

	 /// slow the camera down
	 m_CameraUpdateRate = 10 ;

	 return true;


 }
