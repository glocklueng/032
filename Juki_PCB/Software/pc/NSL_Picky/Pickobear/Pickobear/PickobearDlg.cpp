
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

// Implementation
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


double m_Thresh1=0,m_Thresh2=10;


CPickobearDlg::CPickobearDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPickobearDlg::IDD, pParent)
	, m_headXPos(0)
	, m_headYpos(0)
	, m_Threshold1(m_Thresh1)
	, m_Threshold2(m_Thresh2)
	, m_Head(0)
	, m_GOX(0)
	, m_GOY(0)
{

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPickobearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GO, GO);
	DDX_Text(pDX, IDC_X_POS, m_headXPos);
	DDX_Text(pDX, IDC_Y_POS, m_headYpos);
	DDX_Text(pDX, IDC_THRESHOLD, m_Threshold1);
	DDX_Text(pDX, IDC_THRESHOLD2, m_Threshold2);
	DDX_Control(pDX, IDC_ROTATE, m_Rotation);
	DDX_Control(pDX, IDC_LIST2, m_ComponentList);
	DDX_Text(pDX, IDC_GOX, m_GOX);
	DDX_Text(pDX, IDC_GOY, m_GOY);
}

BEGIN_MESSAGE_MAP(CPickobearDlg, CDialog)
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
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CListCtrl_Components, CListCtrl)
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnHdnItemdblclickList2) 
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


// CPickobearDlg message handlers


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

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_CAM1)->GetWindowRect(rect);
	GetDlgItem(IDC_CAM1)->GetWindowRect(rect1);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	( ( CWnd* ) GetDlgItem( IDC_DOWNCAM ) )->SetWindowText( m_oglWindow.oglCreate( rect, rect1, this, 4 ) );

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);

	// Get size and position of the template textfie7ld we created before in the dialog editor
	GetDlgItem(IDC_CAM2)->GetWindowRect(rect);
	GetDlgItem(IDC_CAM2)->GetWindowRect(rect1);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	
	( ( CWnd* ) GetDlgItem( IDC_UPCAM ) )->SetWindowText( m_oglWindow1.oglCreate( rect, rect1, this, 2 ) );

	// Setup the OpenGL Window's timer to render
	m_oglWindow1.m_unpTimer = m_oglWindow1.SetTimer(1, 1, 0);

	CString m_ComPort = _T("\\\\.\\COM14");
	m_Serial.Open(m_ComPort );
	m_Serial.Setup(CSerial::EBaud9600);
	
	m_ComponentList.GetClientRect(&rect);
	int nInterval =( rect.Width() / 6);

	m_ComponentList.InsertColumn(0, _T("Name"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(1, _T("Type"),LVCFMT_CENTER,nInterval+(nInterval/2));
	m_ComponentList.InsertColumn(2, _T("X"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(3, _T("Y"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(4, _T("R"),LVCFMT_CENTER,nInterval/2);
	m_ComponentList.InsertColumn(5, _T("F"),LVCFMT_CENTER,nInterval);

	m_Rotation.SetRange(0,360);

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
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData(TRUE);
	m_Thresh1 = m_Threshold1;

}

void CPickobearDlg::OnEnChangeThreshold2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);
	m_Thresh2 = m_Threshold2;
}


void CPickobearDlg::OnBnClickedHome()
{	
	bool pass = false;

	do { 
		m_Serial.Write(" ");

		switch( CheckAck('f','p') )
		{
		case 'f':
			if( MessageBox(_T("Home failed!!"),_T("Error"),IDOK|IDRETRY) == IDRETRY)
				pass = false;
			else
				pass = true;
			break;
		case 'p':
			
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

	long cx,cy;
	GetCurrentPosition(cx,cy);
	
}

// move head can only go to (int)(x/40)*40,same for y
bool CPickobearDlg::MoveHead( long x, long y ) 
{
	char buffer[56];

	long cx,cy;
	GetCurrentPosition(cx,cy);

	x = (x/40)*40;
	y = (y/40)*40;

	sprintf(buffer,"g%d,%d\n",x,y);

	_RPT4(_CRT_WARN,"current pos = %dum,%dum\nGoing to %dum,%dum\n",cx,cy,x,y);
	m_Serial.Write(buffer);

	char ch;
	// wait for ack.
	do { 
		m_Serial.Read(&ch,1);
	} while(ch!='A');


	GetCurrentPosition(cx,cy);
	_RPT2(_CRT_WARN,"current pos = %d,%d\n",cx,cy);

	if( cx!= x &&  cy != y ) {
		
		_RPT0(_CRT_WARN,"Problem!");
	}

	return true;
}

char CPickobearDlg::CheckAck(char ack,char ack1)
{
	char ch;
	// wait for ack.
	do { 
		m_Serial.Read(&ch,1);
	} while(ch!=ack && ch!=ack1);
	
	return ch;

}

void CPickobearDlg::OnBnClickedRight()
{
	m_Serial.Write("r");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedPark()
{
	m_Serial.Write("p");	
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedTool1()
{
	bool pass = false;

	do { 
		m_Serial.Write("1");
		Sleep(400);

		switch( CheckAck('f','p') )
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

	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedTool2()
{
	m_Serial.Write("2");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedTool3()
{
	m_Serial.Write("3");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedTool4()
{
	m_Serial.Write("4");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedTool5()
{
	m_Serial.Write("5");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedTool6()
{
	m_Serial.Write("6");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedUp()
{
	m_Serial.Write("u");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedDown()
{
	m_Serial.Write("d");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedLeft()
{
	m_Serial.Write("l");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedHead()
{
	if( m_Head ) {
		m_Serial.Write("h");
	} else { 
		m_Serial.Write("H");
	}

	m_Head = 1 - m_Head;

	long cx,cy;
	GetCurrentPosition(cx,cy);

}


void CPickobearDlg::OnNMCustomdrawRotate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CPickobearDlg::OnBnClickedUpleft()
{
	m_Serial.Write("l");
	m_Serial.Write("u");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedUpright()
{
	m_Serial.Write("r");
	m_Serial.Write("u");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedLeftdown()
{
	m_Serial.Write("l");
	m_Serial.Write("d");
	long cx,cy;
	GetCurrentPosition(cx,cy);
}


void CPickobearDlg::OnBnClickedBottomleft()
{
	m_Serial.Write("r");
	m_Serial.Write("d");
	long cx,cy;
	GetCurrentPosition(cx,cy);
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
	char* token;
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

		token = strtok( buffer, seps );
	
		i = 0;

		while (token != NULL) {

			if( 1 == sscanf (token, "%s", &list[i] ) ) {
				token = strtok (NULL, seps);
				i++;
				
				if( i == 5 ) {
					// too many inputs
					token = NULL;
				}

			} else {
				// error in input
				token = NULL;
			}
		}

		m_ComponentList.AddItem(list[0], list[1], list[2], list[3],list[4]);

	}	while( 1 ) ;

	m_File.Close();	
}


CString GetLoadFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir)
{
	CString szFile;
	TCHAR tmpFile[MAX_PATH];

	OPENFILENAME ofn;

	szFile.Empty();
	memset( tmpFile, 0, MAX_PATH );

	if( pStartDir)
		_tcscpy_s(tmpFile, sizeof(tmpFile)-1,pStartDir);

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

	if( pStartDir){
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
		 
		MoveHead(entry.x,entry.y);

	}
	 
	// Park machine
	m_Serial.Write("p");

	// switch state to idle
	m_MachineState = MS_IDLE ;
	return true;
}

void CPickobearDlg::OnBnClickedGo()
{

	// switch state to GO
	m_MachineState = MS_GO ;

	// Start thread for 'GO'
	HANDLE h = CreateThread(NULL, 0, &CPickobearDlg::goSetup, (LPVOID)this, 0, NULL);

}

void CPickobearDlg::OnBnClickedFeeder()
{
	CFeederSetup dlg;
	INT_PTR nResponse = dlg.DoModal();
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

bool GetCurrentPosition ( long &x,long &y)
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;

	ASSERT( pDlg );

	// read offset in pulses
	m_Serial.Write("?");

	// give arduino time to answer
	Sleep(200);

	char lineBuffer[256];
	unsigned int linePtr = 0;
	char *token;
	char xString[20];
	char yString[20];

	do {
		char sbyte;
		// does this timeout ?
		m_Serial.Read(&sbyte,1);

		// end of line
		if( sbyte == '\n' ) {

			// ASCIZ terminate the buffer
			lineBuffer[linePtr]  = 0;

			// lineBuffer Should now contain something like 1000,1000
			token = strtok (lineBuffer, ",");
			if( token) {
				// convert token to a string
				sscanf (token, "%s", &xString );

				token = strtok (NULL, ",");
				if( token ) {

					sscanf(token, "%s", &yString );	


					break;

				}
			}
		}
		// add to buffer.
		// it'll wrap automatically
		lineBuffer[linePtr++] = sbyte;

	} while( 1 );

	x = atol( xString );
	y = atol( yString );

	( ( CWnd* ) pDlg->GetDlgItem( IDC_X_POS ) )->SetWindowText( CString(xString) ) ;
	( ( CWnd* ) pDlg->GetDlgItem( IDC_Y_POS ) )->SetWindowText( CString(yString) ) ;

	return true;

}

void CPickobearDlg::OnBnClickedZero()
{
	MoveHead(0,0);
}


void CPickobearDlg::OnBnClickedOffset()
{
	ASSERT( m_ComponentList.entry );

	long cx,cy;
	GetCurrentPosition(cx,cy);

	// set the offset.
	m_ComponentList.m_OffsetX = cx - m_ComponentList.entry->x; 
	m_ComponentList.m_OffsetY = cy - m_ComponentList.entry->y;
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
