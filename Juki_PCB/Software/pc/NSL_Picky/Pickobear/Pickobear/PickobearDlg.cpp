
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
	ON_STN_DBLCLK(IDC_CAM2, &CPickobearDlg::OnStnDblclickCam2)
	ON_STN_CLICKED(IDC_CAM1, &CPickobearDlg::OnStnClickedCam1)
	ON_WM_NCRBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CListCtrl_Components, CListCtrl)
END_MESSAGE_MAP()


void CListCtrl_Components::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif

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
	( ( CWnd* ) GetDlgItem( IDC_DOWNCAM ) )->SetWindowText( m_oglWindow.oglCreate( rect, rect1, this, 3 ) );

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
	int nInterval =( rect.Width() / 5);

	m_ComponentList.InsertColumn(0, _T("Name"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(1, _T("Type"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(2, _T("X"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(3, _T("Y"),LVCFMT_CENTER,nInterval);
	m_ComponentList.InsertColumn(4, _T("R"),LVCFMT_CENTER,nInterval);

	//m_ComponentList.AddItem("R1","0805","10","20","90");

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
	m_Serial.Write(" ");
}


void CPickobearDlg::OnBnClickedRight()
{
	m_Serial.Write("r");
}


void CPickobearDlg::OnBnClickedPark()
{
	m_Serial.Write("p");
}


void CPickobearDlg::OnBnClickedTool1()
{
	m_Serial.Write("1");
}


void CPickobearDlg::OnBnClickedTool2()
{
	m_Serial.Write("2");
}


void CPickobearDlg::OnBnClickedTool3()
{
	m_Serial.Write("3");
}


void CPickobearDlg::OnBnClickedTool4()
{
	m_Serial.Write("4");
}


void CPickobearDlg::OnBnClickedTool5()
{
	m_Serial.Write("5");
}


void CPickobearDlg::OnBnClickedTool6()
{
	m_Serial.Write("6");
}


void CPickobearDlg::OnBnClickedUp()
{
	m_Serial.Write("u");
}


void CPickobearDlg::OnBnClickedDown()
{
	m_Serial.Write("d");
}


void CPickobearDlg::OnBnClickedLeft()
{
	m_Serial.Write("l");
}


void CPickobearDlg::OnBnClickedHead()
{
	m_Serial.Write("h");
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
}


void CPickobearDlg::OnBnClickedUpright()
{
	m_Serial.Write("r");
	m_Serial.Write("u");
}


void CPickobearDlg::OnBnClickedLeftdown()
{
	m_Serial.Write("l");
	m_Serial.Write("d");
}


void CPickobearDlg::OnBnClickedBottomleft()
{
	m_Serial.Write("r");
	m_Serial.Write("d");
}

void CPickobearDlg::OnBnClickedLoad()
{
	CString m_LoadFile = ::GetLoadFile( _T("Supported Files Types(*.pck)\0*.pck\0\0"),_T("Pick board to load"),_T("") );
}


void CPickobearDlg::OnBnClickedSave()
{
	CString m_SaveFile = ::GetSaveFile( _T("Supported Files Types(*.pck)\0*.pck\0\0"),_T("Pick board to save"),_T("") );
}

void CPickobearDlg::OnBnClickedImport()
{
	int i;
	const char seps[] = ",";
	char* token;

	CString m_LoadFile = ::GetLoadFile( 
		_T("Supported Files Types(*.csv)\0*.csv\0\0"),
		_T("Pick board to load"),
		NULL
	);

	static char list[10][100];
	static char buffer[1024];

	CFile m_File;

	m_File.Open( m_LoadFile  ,CFile::modeRead ) ;

	do {

		for( i = 0 ; i < sizeof( buffer) -1 ; i++ ) {
	
			// if ended
			if( 0 == m_File.Read( &buffer[i],1) )
				goto end;
	
			// line end
			if( buffer[i] == '\n')
				break;
		}

		// last character
		buffer[i] = 0;

		token = strtok (buffer, seps);
	
		i = 0;

		while (token != NULL) {

			sscanf (token, "%s", &list[i] );
			token = strtok (NULL, seps);
			i++;
		}

		m_ComponentList.AddItem(list[0], list[1], list[2], list[3],list[4]);
	}	while( 1 ) ;

	end:;
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




void CPickobearDlg::OnStnDblclickCam2()
{
	__asm int 3
}


void CPickobearDlg::OnStnClickedCam1()
{
	__asm int 3
}


void CPickobearDlg::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
		__asm int 3
	CDialog::OnNcRButtonDown(nHitTest, point);
}
