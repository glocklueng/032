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
	, m_Speed(800)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPickobearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GO, GO);
	DDX_Text(pDX, IDC_X_POS, m_headXPos);
	DDX_Text(pDX, IDC_Y_POS, m_headYPos);
	DDX_Text(pDX, IDC_THRESHOLD, m_Threshold1);
	DDX_Text(pDX, IDC_THRESHOLD2, m_Threshold2);
	DDX_Control(pDX, IDC_ROTATE, m_Rotation);
	DDX_Control(pDX, IDC_LIST2, m_ComponentList);
	DDX_Text(pDX, IDC_GOX, m_GOX);
	DDX_Text(pDX, IDC_GOY, m_GOY);
	DDX_Control(pDX, IDC_LIST3, m_FeederList);
	DDX_Control(pDX, IDC_COMBO1, m_UpCamera);
	DDX_Control(pDX, IDC_COMBO2, m_DownCamera);
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
	ON_BN_CLICKED(IDC_UPDATE2, &CPickobearDlg::OnBnClickedUpdate2)
	ON_BN_CLICKED(IDC_UPDATE3, &CPickobearDlg::OnBnClickedUpdate3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPickobearDlg::OnCbnSelchangeCombo1)
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
			
	m_UpCamera.ResetContent();
	m_DownCamera.ResetContent();
		
	int numDevices = VI.listDevices();
		
	UpdateData(FALSE);

	for( int i = 0 ; i < numDevices ; i++ ) {
	
		_RPT2(_CRT_WARN,"%d) Camera name is [%s]\n",i, VI.getDeviceName( i  ));

		m_UpCamera.AddString( CString(VI.getDeviceName( i  )) );
		m_DownCamera.AddString( CString( VI.getDeviceName( i  ) ));

	}
	  
	int i = GetProfileInt(pszKey, _T("upCamera"), 0);
	if (i < 0 || i > numDevices-1 ) i = 0;

	m_UpCamera.SetCurSel( i );

	i = GetProfileInt(pszKey, _T("downCamera"), 0);
	if (i < 0 || i > numDevices-1 ) i = 0;

	m_DownCamera.SetCurSel( i );
		
	UpdateData();

	CRect rect,rect1;

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_CAM1)->GetWindowRect(rect);
	GetDlgItem(IDC_CAM1)->GetWindowRect(rect1);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	( ( CWnd* ) GetDlgItem( IDC_DOWNCAM ) )->SetWindowText( m_oglWindow.oglCreate( rect, rect1, this, m_UpCamera.GetCurSel()) );
	
	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_CAM2)->GetWindowRect(rect);
	GetDlgItem(IDC_CAM2)->GetWindowRect(rect1);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);
	
	// Create OpenGL Control window	
	( ( CWnd* ) GetDlgItem( IDC_UPCAM ) )->SetWindowText( m_oglWindow1.oglCreate( rect, rect1, this,m_DownCamera.GetCurSel() ));


	// convert to a picker
	CString m_ComPort = _T("\\\\.\\COM31");
	m_Serial.Open(m_ComPort, this );

	m_Serial.Setup(CSerial::EBaud38400 );
	EmptySerial();

	m_ComponentList.GetClientRect(&rect);
	int nInterval =( rect.Width() / 7);

	m_ComponentList.InsertColumn(0, _T("Name"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(1, _T("Value"),LVCFMT_CENTER,nInterval/2);
	m_ComponentList.InsertColumn(2, _T("Type"),LVCFMT_CENTER,nInterval+(nInterval/2));
	m_ComponentList.InsertColumn(3, _T("X"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(4, _T("Y"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(5, _T("R"),LVCFMT_CENTER,nInterval/2);
	m_ComponentList.InsertColumn(6, _T("F"),LVCFMT_CENTER,nInterval);

	m_Rotation.SetRange(0,360);
		
	m_FeederList.GetClientRect(&rect);
	nInterval =( rect.Width() / 4 );

	m_FeederList.InsertColumn(0, _T("Name"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(1, _T("X"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(2, _T("Y"),LVCFMT_CENTER,nInterval);
	m_FeederList.InsertColumn(3, _T("Rot"),LVCFMT_CENTER,nInterval);

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
	m_oglWindow1.m_unpTimer = m_oglWindow1.SetTimer(1, 300, 0);



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


void CPickobearDlg::OnBnClickedHome()
{	
	bool pass = false;

	do { 
		m_Serial.Write("G28\r\n");

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

			
			pass = true;
			break;
		default:
			break;
		}
	} while( pass == false );

	SetCurrentPosition(m_headXPos,m_headYPos);		
}

void BuildGCodeMove( char *output, int length, int mode , long x,long y,long speed )
{
	long double tx,ty;

	ASSERT( output) ;
	
	memset(output,0,length);

	tx = x * 0.0000393700787;
	ty = y * 0.0000393700787;

	sprintf_s(output, length,"G%dX%gY%gF%d\r\n",mode,tx,ty,speed);
}

// move head can only go to (int)(x/40)*40,same for y
bool CPickobearDlg::MoveHead( long x, long y ) 
{
	char buffer[ 256 ];

	m_headXPos = x ; m_headYPos = y;

	SetCurrentPosition(m_headXPos,m_headYPos);

	BuildGCodeMove(buffer,sizeof(buffer),1,x,y,m_Speed);

	_RPT5(_CRT_WARN,"current pos = %dum,%dum\nGoing to %dum,%dum\n%s\n",m_headXPos,m_headYPos,x,y,buffer);
	m_Serial.Write(buffer);

	CheckAck("ok");

	return true;
}

void CPickobearDlg::EmptySerial ( void ) 
{
	char bufferx[100];

	m_Serial.Purge();
	{
		DWORD length1;
		do { 
			length1 = 1;
			m_Serial.Read( &bufferx,100,&length1,0,100);
		} while( length1 ) ;
	}
}

char CPickobearDlg::CheckAck( char *ack1 )
{
	unsigned int length = 1;

	unsigned int index ;
	unsigned char ch;
	int ret;

	unsigned char buffer[ 10 ];

	memset(buffer,0,sizeof(buffer));

	ASSERT( ack1 );
	if( ack1 == NULL ){
		return 'p';
	}
	index = 0;

	// wait for ack. needs a timeout
	do { 

		ret = m_Serial.Read( &ch, length );
		if ( index < sizeof( buffer ) ) {

			if( !( ch == 13 || ch == 10 ) ) 
				buffer[ index++ ] = ch;

		}

	} while(ch!='\n');
	
	_RPT1(_CRT_WARN,"%s\n", buffer);

	length = strlen( ack1 ) ;

	if(0==_strnicmp((const char *)ack1,(const char *)&buffer[0], length   )  ) {

		return 'p';
	}

	return 'f';

}

void CPickobearDlg::OnBnClickedRight()
{
	m_headXPos += 1000;
	
	MoveHead( m_headXPos, m_headYPos) ;
}


void CPickobearDlg::OnBnClickedPark()
{
	MoveHead( 364000,517000 );
	SetCurrentPosition(m_headXPos,m_headYPos);
}


void CPickobearDlg::OnBnClickedTool1()
{
	bool pass = false;

	do { 
		m_Serial.Write("M12");
		
		Sleep(400);

		switch( CheckAck("ok") )
		{
		case 'f':
			if( MessageBox(_T("Tool change failed!!"),_T("Error"),IDOK|IDRETRY) == IDRETRY)
				pass = false;
			else
				pass = true;
			break;
		case 'p':
			pass = true;
			break;
		default:
			break;
		}
	} while( pass == false );

	SetCurrentPosition(m_headXPos,m_headYPos);
}


void CPickobearDlg::OnBnClickedTool2()
{
	bool pass = false;

	do { 
		m_Serial.Write("M13");
		
		Sleep(400);

		switch( CheckAck("ok") )
		{
		case 'f':
			if( MessageBox(_T("Tool change failed!!"),_T("Error"),IDOK|IDRETRY) == IDRETRY)
				pass = false;
			else
				pass = true;
			break;
		case 'p':
			pass = true;
			break;
		default:
			break;
		}
	} while( pass == false );

	SetCurrentPosition(m_headXPos,m_headYPos);
}


void CPickobearDlg::OnBnClickedTool3()
{
	m_Serial.Write("M14");
}


void CPickobearDlg::OnBnClickedTool4()
{
	m_Serial.Write("M15");
}


void CPickobearDlg::OnBnClickedTool5()
{
	m_Serial.Write("M16");
}


void CPickobearDlg::OnBnClickedTool6()
{
	m_Serial.Write("M17");
}


void CPickobearDlg::OnBnClickedUp()
{
	m_headYPos += 1000;
	
	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedDown()
{

	m_headYPos -= 1000;
	
	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedLeft()
{
	m_headXPos -= 1000;
	
	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedHead()
{
	if( m_Head ) {
		// down
		m_Serial.Write("M10");
		m_Head = 0;
	} else { 
		//up
		m_Serial.Write("M11");
		m_Head = 1;
	}
}


void CPickobearDlg::OnNMCustomdrawRotate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CPickobearDlg::OnBnClickedUpleft()
{
	m_headXPos -= 1000;
	m_headYPos += 1000;
	
	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedUpright()
{
	m_headXPos += 1000;
	m_headYPos += 1000;
	
	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedLeftdown()
{
	m_headXPos -= 1000;
	m_headYPos += 1000;
	
	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedBottomleft()
{
	m_headXPos -= 1000;
	m_headYPos -= 1000;
	
	MoveHead( m_headXPos, m_headYPos) ;
}

void CPickobearDlg::OnBnClickedLoad()
{
	CFile m_File;

	CString m_LoadFile = ::GetLoadFile( _T("Supported Files Types(*.pck)\0*.pck\0\0"),_T("Pick board to load"),_T("") );

	if( FALSE == m_File.Open(  m_LoadFile , CFile::modeRead ) ) {
		return ;
	} 

	m_File.Close();
}

void CPickobearDlg::OnBnClickedSave()
{
	CFile m_File;

	CString m_SaveFile = ::GetSaveFile( _T("Supported Files Types(*.pck)\0*.pck\0\0"),_T("Pick board to save"),_T("") );

	if( FALSE == m_File.Open(  m_SaveFile , CFile::modeWrite ) ) {
		return ;
	}

	m_File.Close();
}

void CPickobearDlg::OnBnClickedImport()
{
	int i;
	const char seps[] = ",";
	char* token,*next;
	CFile m_File;

	static char list[10][100];
	static char buffer[1024];

	CString m_LoadFile = ::GetLoadFile( 
		_T("Supported Files Types(*.csv)\0*.csv\0\0"),
		_T("Pick board to load"),
		NULL
	);

	if(  FALSE == m_File.Open( m_LoadFile  ,CFile::modeRead ) ) { 
		return;
	}

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

		token = strtok_s( buffer, seps, &next);
	
		i = 0;

		while (token != NULL) {

			if( 1 == sscanf_s(token, "%s", &list[i] ) ) {
				token = strtok_s (NULL, seps, &next);
				i++;
				
				if( i == 6 ) {
					// too many inputs
					token = NULL;
				}

			} else {
				// error in input
				token = NULL;
			}
		}

		m_ComponentList.AddItem(list[0], list[1], list[2], list[3], list[4],list[5] );

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
		m_oglWindow.UpdateCamera( 1 ) ;
		Sleep(1);
		
		//m_oglWindow1.UpdateCamera( 1 ) ;
		Sleep(1);

	}
	return true;
}

 DWORD WINAPI CPickobearDlg::goSetup(LPVOID pThis)
{
	return ((CPickobearDlg*)pThis)->goThread();
 }

DWORD CPickobearDlg::goThread(void )
{
	unsigned int i ;
	char buffer[5];
	CListCtrl_Components::CompDatabase entry; 
	
	ZeroMemory(buffer,sizeof(buffer));

	while( m_Serial.Read( &buffer,sizeof(buffer ) ) );

	for (i = 0 ; i < m_ComponentList.GetCount(); i ++ ) {
		
		 entry = m_ComponentList.at(i);
		 
		MoveHead(entry.x+m_ComponentList.m_OffsetX,
			     entry.y+m_ComponentList.m_OffsetY);
		

		int in = (m_ComponentList.GetCount()-1)-i;
		m_ComponentList.SetItemState(in, LVIS_SELECTED, LVIS_SELECTED);
		m_ComponentList.EnsureVisible( in ,TRUE );

		CListCtrl_FeederList::FeederDatabase feeder = m_FeederList.at(rand()%2);
	
		MoveHead(feeder.x,
			     feeder.y);
		

		UpdateWindow();

	}
	 
	// Park machine
	m_Serial.Write("G1X14Y15F200\r\n");

	// switch state to idle
	m_MachineState = MS_IDLE ;
	return true;
}

void CPickobearDlg::OnBnClickedGo()
{
	if ( m_MachineState == MS_GO  ) { 
		return;
	}

	// switch state to GO
	m_MachineState = MS_GO ;

	// Start thread for 'GO'
	HANDLE h = CreateThread(NULL, 0, &CPickobearDlg::goSetup, (LPVOID)this, 0, NULL);
}

void CPickobearDlg::OnBnClickedFeeder()
{
	m_pFeederDlg = new CFeederSetup(this);
	m_pFeederDlg->Create(CFeederSetup::IDD,GetDesktopWindow());
	m_pFeederDlg->ShowWindow(SW_SHOW);
}

void CListCtrl_Components::OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult)
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
	pDlg->MoveHead(entry->x+pDlg->m_ComponentList.m_OffsetX,entry->y+pDlg->m_ComponentList.m_OffsetY);

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

	unsigned int linePtr = 0;
	char xString[200];
	char yString[200];

	sprintf_s(xString,sizeof(xString),"%d",x);
	sprintf_s(yString,sizeof(xString),"%d",y);

	pDlg->m_GOX = x;
	pDlg->m_GOY = y;

	( ( CWnd* ) pDlg->GetDlgItem( IDC_X_POS ) )->SetWindowText( CString(xString) ) ;
	( ( CWnd* ) pDlg->GetDlgItem( IDC_Y_POS ) )->SetWindowText( CString(yString) ) ;

	return true;
}

void CPickobearDlg::OnBnClickedZero()
{
	int counter = 100;

	long x,y;
	do {
		x = rand()%14;
		y = rand()%16;
		MoveHead(x,y);
		Sleep( 3000 );
	}while(counter--);

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


	int item = pDlg->m_FeederList.GetSelectedCount();

	if( item == 0 ) {
		return ;
	}
	
	item = pDlg->m_FeederList.GetNextItem(-1, LVNI_SELECTED);

	// iItem is item number, database is backwards
	int clist = (GetCount()-1)-pDlg->m_ComponentList.GetNextItem(-1, LVNI_SELECTED);

	entry = &m_Database.at( clist ) ;

	pDlg->m_ComponentList.AssignFeeder( clist, item ) ;
}


void CPickobearDlg::OnBnClickedLoadFeeder()
{
	m_FeederList.LoadDatabase();
}


void CPickobearDlg::OnBnClickedSaveFeeder()
{
	m_FeederList.SaveDatabase();
}

void CPickobearDlg::OnBnClickedUpdate2()
{


	SetCurrentPosition(m_headXPos,m_headYPos);
	m_headYPos += 73050;
	MoveHead(m_headXPos,m_headYPos);
}


void CPickobearDlg::OnBnClickedUpdate3()
{


	SetCurrentPosition(m_headXPos,m_headYPos);
	m_headYPos -= 73050;
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


afx_msg LRESULT CPickobearDlg::OnSerialMsg (WPARAM wParam, LPARAM lParam)
{
    const CSerialMFC::EEvent eEvent = CSerialMFC::EEvent(LOWORD(wParam));
    const CSerialMFC::EError eError = CSerialMFC::EError(HIWORD(wParam));

    switch (eEvent)
    {
	    case CSerialMFC::EEventRecv:
			_RPT0(_CRT_WARN,"EEventRecv\n");

        break;
    }
   
    // Return successful

    return 0;
}


void CPickobearDlg::OnCbnSelchangeCombo1()
{

}
