
// pfunkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pfunk.h"
#include "pfunkDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

FMOD_SYSTEM            *fsystem;
FMOD_SOUND             *sound;
FMOD_CHANNEL           *channel = 0;
FMOD_RESULT             result;
FMOD_MODE               mode = FMOD_2D | FMOD_OPENUSER | FMOD_LOOP_NORMAL | FMOD_HARDWARE;
int                     key;
int                     channels = 2;
FMOD_CREATESOUNDEXINFO  createsoundexinfo;
unsigned int            version;
	
extern void __cdecl  setup();
extern void __cdecl loop();

static int ok = 0;

	
FMOD_RESULT F_CALLBACK pcmsetposcallback(FMOD_SOUND *sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype)
{
    /*
        This is useful if the user calls FMOD_Channel_SetPosition and you want to seek your data accordingly.
    */
    return FMOD_OK;
}

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

signed short getrb(void);

FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND *sound, void *data, unsigned int datalen)
{
    unsigned int  count;
    static float  t1 = 0, t2 = 0;        // time
    static float  v1 = 0, v2 = 0;        // velocity
    signed short *stereo16bitbuffer = (signed short *)data;
	
    for (count=0; count<datalen>>2; count++)        // >>2 = 16bit stereo (4 bytes per sample)
    {
        //*stereo16bitbuffer++ = (signed short)(sin(t1) * 32767.0f);    // left channel
        //*stereo16bitbuffer++ = (signed short)(sin(t2) * 32767.0f);    // right channel
		signed short sample;

		sample = getrb();
		*stereo16bitbuffer++ = (signed short)sample;    // left channel
        *stereo16bitbuffer++ = (signed short)sample;    // right channel

        t1 += 0.01f   + v1;
        t2 += 0.0142f + v2;
        v1 += (float)(sin(t1) * 0.002f);
        v2 += (float)(sin(t2) * 0.002f);
    }

    return FMOD_OK;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpfunkDlg dialog




CpfunkDlg::CpfunkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpfunkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpfunkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RENDER_WINDOW, m_LCDPanelA);
	DDX_Control(pDX, IDC_RENDER_WINDOW2, m_LCDPanelB);
	DDX_Control(pDX, IDC_RENDER_WINDOW3, m_LCDPanelC);
	DDX_Control(pDX, IDC_RENDER_WINDOW4, m_LCDPanelD);
	DDX_Control(pDX, IDC_E1, m_EncoderEdit1);
	DDX_Control(pDX, IDC_E2, m_EncoderEdit2);
	DDX_Control(pDX, IDC_E3, m_EncoderEdit3);
	DDX_Control(pDX, IDC_E4, m_EncoderEdit4);
	DDX_Control(pDX, IDC_EVOLUME, m_EncoderVolume);
	DDX_Control(pDX, IDC_ENG, m_Eng);
	DDX_Control(pDX, IDC_OSC, m_Osc);
	DDX_Control(pDX, IDC_ENV, m_Env);
	DDX_Control(pDX, IDC_MTX, m_Mtx);
	DDX_Control(pDX, IDC_LFO, m_LFo);
	DDX_Control(pDX, IDC_BACK, m_Back);
	DDX_Control(pDX, IDC_Menu, m_Menu);
}

BEGIN_MESSAGE_MAP(CpfunkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CpfunkDlg message handlers

enum eHBITMAP
{
    Background,
    Scale1,
    Knob1,
    Dot1,

    Scale2,
    Knob2,
    Dot2,

    BitmapCount
};

HBITMAP     m_hBitmap[BitmapCount];

UINT ThreadFunc(LPVOID pParam)
{
	while( 1 ) {

		if ( ok ) {

			loop();

			FMOD_System_Update( fsystem );
			Sleep(10);

		}
	}

	return 0;
}


BOOL CpfunkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	
	m_hBitmap[Scale1]	= (HBITMAP)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP9), IMAGE_BITMAP, 0,0,LR_CREATEDIBSECTION | LR_DEFAULTCOLOR);
	m_hBitmap[Knob1]	= (HBITMAP)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP9), IMAGE_BITMAP, 0,0,LR_CREATEDIBSECTION | LR_DEFAULTCOLOR);
	m_hBitmap[Dot1]		= (HBITMAP)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0,0,LR_CREATEDIBSECTION | LR_DEFAULTCOLOR);

	m_EncoderEdit1.SetBitmap(m_hBitmap[Scale1], CVMRotaryFaderCtrl::Scale);
	m_EncoderEdit1.SetBitmap(m_hBitmap[Knob1], CVMRotaryFaderCtrl::Knob);
	m_EncoderEdit1.SetBitmap(m_hBitmap[Dot1], CVMRotaryFaderCtrl::Dot);
	
	m_EncoderEdit2.SetBitmap(m_hBitmap[Scale1], CVMRotaryFaderCtrl::Scale);
	m_EncoderEdit2.SetBitmap(m_hBitmap[Knob1], CVMRotaryFaderCtrl::Knob);
	m_EncoderEdit2.SetBitmap(m_hBitmap[Dot1], CVMRotaryFaderCtrl::Dot);
	
	m_EncoderEdit3.SetBitmap(m_hBitmap[Scale1], CVMRotaryFaderCtrl::Scale);
	m_EncoderEdit3.SetBitmap(m_hBitmap[Knob1], CVMRotaryFaderCtrl::Knob);
	m_EncoderEdit3.SetBitmap(m_hBitmap[Dot1], CVMRotaryFaderCtrl::Dot);
	
	m_EncoderEdit4.SetBitmap(m_hBitmap[Scale1], CVMRotaryFaderCtrl::Scale);
	m_EncoderEdit4.SetBitmap(m_hBitmap[Knob1], CVMRotaryFaderCtrl::Knob);
	m_EncoderEdit4.SetBitmap(m_hBitmap[Dot1], CVMRotaryFaderCtrl::Dot);
	
	m_EncoderVolume.SetBitmap(m_hBitmap[Scale1], CVMRotaryFaderCtrl::Scale);
	m_EncoderVolume.SetBitmap(m_hBitmap[Knob1], CVMRotaryFaderCtrl::Knob);
	m_EncoderVolume.SetBitmap(m_hBitmap[Dot1], CVMRotaryFaderCtrl::Dot);
	
	m_LCDPanelA.StringMode(TRUE, TRUE);
	//m_LCDPanelA.LoadMatrix( IDB_MATRIX5x7 );  
	m_LCDPanelA.SetSegmentStyle(Segment16 , 1);
	m_LCDPanelA.SetSegmentSize(.8f,1);
	m_LCDPanelA.SetValueNbDigit( 20,0 ); 
	m_LCDPanelA.GetBorder()->SetStyle(CGdiBorder::Chrome);
	m_LCDPanelA.SetWindowText( ("PfUNK SOFTWARE SYNTH"));
	m_LCDPanelA.SetScrollSpeed(0);
	m_LCDPanelA.ShowGlass(80, 1 );

	m_LCDPanelB.StringMode(TRUE, TRUE);
	m_LCDPanelB.LoadMatrix( IDB_MATRIX5x7 );  
	m_LCDPanelB.SetSegmentStyle(MatrixSquare  , 1);
	m_LCDPanelB.SetSegmentSize(.9f,1);
	m_LCDPanelB.SetValueNbDigit( 20,0 ); 
	m_LCDPanelB.GetBorder()->SetStyle(CGdiBorder::Chrome);
	m_LCDPanelB.SetWindowText( (" [ NULLSPACE LABS ]"));
	m_LCDPanelB.SetColor( RGB(0,150,0) );
	m_LCDPanelB.ShowGlass(80, 1 );

	m_LCDPanelC.StringMode(TRUE, TRUE);
	m_LCDPanelC.LoadMatrix( IDB_MATRIX5x7 );  
	m_LCDPanelC.SetSegmentStyle(MatrixSquare , 1);
	m_LCDPanelC.SetSegmentSize(1.f,1);
	m_LCDPanelC.SetValueNbDigit( 20,0 ); 
	m_LCDPanelC.GetBorder()->SetStyle(CGdiBorder::Raised);
	m_LCDPanelC.SetWindowText( ("BOOTING"));
	m_LCDPanelC.SetColor( RGB(0,0,255) );
	//m_LCDPanelC.SetBlink( 100 );
	m_LCDPanelC.ShowGlass(80, 1 );

	m_LCDPanelD.StringMode(TRUE, TRUE);
	m_LCDPanelD.LoadMatrix( IDB_MATRIX5x7 );  
	m_LCDPanelD.SetSegmentStyle(MatrixSquare , 1);
	m_LCDPanelD.SetSegmentSize(1.f,1);
	m_LCDPanelD.SetValueNbDigit( 20,0 ); 
	m_LCDPanelD.GetBorder()->SetStyle(CGdiBorder::Chrome);
	m_LCDPanelD.SetWindowText( ("BEEP BLOP BLIP"));
	m_LCDPanelD.SetColor( RGB(255,0,255) );
//	m_LCDPanelD.SetBlink( 100 );
	m_LCDPanelD.ShowGlass(80, 1 );

	m_LCDPanelA.SetWindowText("");
	m_LCDPanelB.SetWindowText("");
	m_LCDPanelC.SetWindowText("");
	m_LCDPanelD.SetWindowText("");
	

    /*
        Create a System object and initialize.
    */
    result = FMOD_System_Create(&fsystem);
    ERRCHECK(result);

    result = FMOD_System_GetVersion(fsystem, &version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }
	  
	mode |= FMOD_CREATESTREAM;

    result = FMOD_System_Init(fsystem, 32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);

	memset(&createsoundexinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    createsoundexinfo.cbsize            = sizeof(FMOD_CREATESOUNDEXINFO);              /* required. */
    createsoundexinfo.decodebuffersize  = 44100;                                       /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
    createsoundexinfo.length            = 44100 * channels * sizeof(signed short) * 5; /* Length of PCM data in bytes of whole song (for Sound::getLength) */
    createsoundexinfo.numchannels       = channels;                                    /* Number of channels in the sound. */
    createsoundexinfo.defaultfrequency  = 44100;                                       /* Default playback rate of sound. */
    createsoundexinfo.format            = FMOD_SOUND_FORMAT_PCM16;                     /* Data format of sound. */
    createsoundexinfo.pcmreadcallback   = pcmreadcallback;                             /* User callback for reading. */
    createsoundexinfo.pcmsetposcallback = pcmsetposcallback;                           /* User callback for seeking. */

    result = FMOD_System_CreateSound(fsystem, 0, mode, &createsoundexinfo, &sound);
    ERRCHECK(result);

	result = FMOD_System_PlaySound(fsystem, FMOD_CHANNEL_FREE, sound, 0, &channel);
    ERRCHECK(result);
	
	AfxBeginThread(ThreadFunc,(LPVOID)0);

	SetTimer(1,10,0);

	setup();
	ok = 1;


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void shutdown(void)
{

	/*
		Shut down
	*/
	result = FMOD_Sound_Release(sound);
	ERRCHECK(result);
	result = FMOD_System_Close(fsystem);
	ERRCHECK(result);
	result = FMOD_System_Release(fsystem);
	ERRCHECK(result);

}

CpfunkDlg::~CpfunkDlg()
{
	shutdown();
}

void CpfunkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CpfunkDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CpfunkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Do the black background
BOOL CpfunkDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    CBrush myBrush(RGB(0, 0, 0));
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);   
    return bRes;                       
}

void CpfunkDlg::OnTimer(UINT nIDEvent) 
{
	if( ok && nIDEvent == 1 ) {

	}


	CDialogEx::OnTimer(nIDEvent);
}

#include "..\src\fmdisplay.h"

extern FMDisplay          fmDisplay;
extern LiquidCrystal      lcd;

void LCDPrint(int row,int col, const char*text)
{
	static char string[4][20];
	
	if ( theApp.pDlg  == NULL ){
		return;
	}

	row = lcd.y;
	col = lcd.x;

	for(int i = 0 ;i <sizeof(string[col])-1;i++) {
		if( string[col][i] == 0 ) string[col][i] = 32;
	}

	switch( row ) {

		case 0:

			strncpy(&string[0][col],text,min(strlen(text),sizeof( string[0] ) - col) );
			theApp.pDlg->m_LCDPanelA.SetWindowText( string[0] );
			break;

		case 1:

			strncpy(&string[1][col],text,min(strlen(text),sizeof( string[1] ) - col) );
			theApp.pDlg->m_LCDPanelB.SetWindowText( string[1] );
			break;

		case 2:

			strncpy(&string[2][col],text,min(strlen(text),sizeof( string[2] ) - col) );
			theApp.pDlg->m_LCDPanelC.SetWindowText( string[2] );
			break;

		case 3:

			strncpy(&string[3][col],text,min(strlen(text),sizeof( string[3] ) - col) );
			theApp.pDlg->m_LCDPanelD.SetWindowText( string[3] );
			break;
	}
}

void __cdecl delayMicroseconds(long ms)
{
    __int64 time1 = 0, time2 = 0, freq = 0;

    QueryPerformanceCounter((LARGE_INTEGER *) &time1);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    do {
        QueryPerformanceCounter((LARGE_INTEGER *) &time2);

    } while((time2-time1) < ms);
}


long __cdecl micros(void)
{
	return clock();
}

void __cdecl pinMode(int,int)
{
}

#define HC165_CLOCK  17
#define HC165_LOAD   18
#define HC165_DATA   19

static char restartEncoder = 1;
static unsigned char encoderIndex = 0;


void __cdecl digitalWrite(int port ,int val)
{
	switch( port ) {

		case HC165_LOAD:
			restartEncoder = 1;
			encoderIndex =  0 ;
			break;

		case HC165_CLOCK:
					
			if( val == 1 ) {
				encoderIndex ++;
			}
			break;


	}
}

unsigned char __cdecl digitalRead(int port)
{ 
	int ret  = 0;

	if( port == HC165_DATA )  {

		switch( encoderIndex ) {
		
			case 0:break;
			case 1:break;
			case 2:break;
			case 3:break;
			case 4:break;
			case 5:
				ret = theApp.pDlg->m_Back.GetState()?1:0;break; // button 5
			case 6:
				ret = theApp.pDlg->m_Menu.GetState()?1:0;break; // button 6
			case 7:
				ret = theApp.pDlg->m_LFo.GetState()?1:0;break; // button 4
			case 8:
				ret = theApp.pDlg->m_Eng.GetState()?1:0;break; // button 2
			case 9:
				ret = theApp.pDlg->m_Osc.GetState()?1:0;break; // button 1
			case 10:
				ret = theApp.pDlg->m_Mtx.GetState()?1:0;break;
			case 11:
				ret = theApp.pDlg->m_Osc.GetState()?1:0;break;
			case 12:
				ret = theApp.pDlg->m_Env.GetState()?1:0;break; //button 0
			case 13:
				ret = theApp.pDlg->m_Back.GetState()?1:0;break;
			case 14:
				ret = theApp.pDlg->m_Back.GetState()?1:0;break;
			case 15:
				ret = theApp.pDlg->m_LFo.GetState()?1:0;break;
			default:
				_RPT1(_CRT_WARN,"not handled (%d)\n", encoderIndex );
		}
	}

	return ret;
}

void __cdecl togglePin(int)
{
}

void __cdecl delay(int length)
{
	Sleep( length) ;
}

