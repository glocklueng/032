// DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIDIDevDemo v2.h"
#include "DemoDlg.h"
#include "ShortMsg.h"
#include "midi.h"
#include "MIDIDevsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog


CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CDemoDlg::~CDemoDlg()
{
    if(m_InDevice.IsOpen())
    {
        if(m_InDevice.IsRecording())
        {
            m_InDevice.StopRecording();
        }   

        m_InDevice.Close();
    }

    if(m_OutDevice.IsOpen())
    {
        m_OutDevice.Close();
    }
}


// Note-on event notification
void CDemoDlg::OnNoteOn(CPianoCtrl &PianoCtrl, unsigned char NoteId)
{
    midi::CShortMsg ShortMsg(midi::NOTE_ON, 0, NoteId, 127, 0);
    ShortMsg.SendMsg(m_OutDevice);
}


// Note-off event notification
void CDemoDlg::OnNoteOff(CPianoCtrl &PianoCtrl, unsigned char NoteId)
{
    midi::CShortMsg ShortMsg(midi::NOTE_OFF, 0, NoteId, 0, 0);
    ShortMsg.SendMsg(m_OutDevice);
}


// Receives MIDI short message
void CDemoDlg::ReceiveMsg(DWORD Msg, DWORD TimeStamp)
{
    midi::CShortMsg ShortMsg(Msg, TimeStamp);
    unsigned char Command = ShortMsg.GetCommand();
    CString Text;

    // Display command value
    Text.Format("%d", ShortMsg.GetCommand());
    m_Command.SetWindowText(Text);

    // Display channel value
    Text.Format("%d", ShortMsg.GetChannel());
    m_Channel.SetWindowText(Text);

    // Display data byte 1 value
    Text.Format("%d", ShortMsg.GetData1());
    m_Data1.SetWindowText(Text);

    // Display data byte 2 value
    Text.Format("%d", ShortMsg.GetData2());
    m_Data2.SetWindowText(Text);
    
    // 
    // If this is a note-on or note-off event, notify piano control so
    // that it can update its display
    //

    if(Command == midi::NOTE_ON && ShortMsg.GetData2() > 0)
    {
        unsigned char Note = ShortMsg.GetData1();

        // Range checking
        if(Note >= m_Keys.GetLowNote() && 
            Note <= m_Keys.GetHighNote())
        {
            m_Keys.NoteOn(Note);
        }
    }
    else if(Command == midi::NOTE_OFF || (Command == midi::NOTE_ON && 
        ShortMsg.GetData2() == 0))
    {
        unsigned char Note = ShortMsg.GetData1();

        // Range checking
        if(Note >= m_Keys.GetLowNote() && 
            Note <= m_Keys.GetHighNote())
        {
            m_Keys.NoteOff(Note);
        }
    }
}


// Receives an erroneous short message
void CDemoDlg::OnError(DWORD Msg, DWORD TimeStamp)
{
    MessageBox("An illegal MIDI short message was received.", "Error",
        MB_ICONSTOP | MB_OK);
}


void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
		DDX_Control(pDX, IDC_GM_LIST, m_GMCombo);
        DDX_Control(pDX, IDC_COMMAND, m_Command);
        DDX_Control(pDX, IDC_CHANNEL, m_Channel);
        DDX_Control(pDX, IDC_DATA1, m_Data1);
        DDX_Control(pDX, IDC_DATA2, m_Data2);
	//}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_MIDI_KEYS, m_Keys);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_PREF_NOTE_COLOR, OnPrefNoteColor)
	ON_COMMAND(ID_PREF_MIDIDEVICES, OnPrefMididevices)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
    ON_CBN_SELCHANGE(IDC_GM_LIST, OnSelchangeGmList)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
    // Initialize piano control
    m_Keys.Initialize(CMIDIKeyboard::LOW_NOTE, CMIDIKeyboard::HIGH_NOTE);

    // Attach ourselves to the piano control so that we will be notified
    // when note-on and note-off events occur
    m_Keys.AttachListener(*this);

    // Initialize General MIDI combo box
    m_GMCombo.SetCurSel(0);
    
    // Attempt to open MIDI input and output devices
    try
    {
        // If there are any MIDI output devices available, open one.
        if(midi::CMIDIOutDevice::GetNumDevs() > 0)
        {
            m_OutDevice.Open(0);
        }
        // Else there are no MIDI output devices available.
        else
        {
            MessageBox("No MIDI output devices available.", "Warning", 
                MB_ICONWARNING | MB_OK);
        }

        // If there are any MIDI input devices available, open one and begin
        // recording.
        if(midi::CMIDIInDevice::GetNumDevs() > 0)
        {
            m_InDevice.SetReceiver(*this);
            m_InDevice.Open(0);
            // Start receiving MIDI events
            m_InDevice.StartRecording();           
        }
        // Else there are no MIDI input devices available.
        else
        {
            MessageBox("No MIDI input devices available.", "Warning", 
                MB_ICONWARNING | MB_OK);
        }

    }
    catch(const std::exception &ex)
    {
        MessageBox(ex.what(), "Error", MB_ICONSTOP | MB_OK);
    }
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


// Note color preference
void CDemoDlg::OnPrefNoteColor() 
{
	CColorDialog Dlg(m_Keys.GetNoteOnColor());

    if(Dlg.DoModal() == IDOK)
    {
        m_Keys.SetNoteOnColor(Dlg.GetColor());
    }	
}


// MIDI device preferences
void CDemoDlg::OnPrefMididevices() 
{
    try
    {
	    CMIDIDevsDlg Dlg;

        //
        // Initialize MIDI device dialog box
        //

        if(m_OutDevice.IsOpen())
        {
            Dlg.SetOutDevId(m_OutDevice.GetDevID());
        }

        if(m_InDevice.IsOpen())
        {
            Dlg.SetInDevId(m_InDevice.GetDevID());
        }

        // Run dialog box
        if(Dlg.DoModal() == IDOK)
        {
            // If the client clicked OK and they chose to change the 
            // MIDI output device
            if(Dlg.IsOutDevChanged())
            {
                m_OutDevice.Close();
                m_OutDevice.Open(Dlg.GetOutDevId());   
                OnSelchangeGmList();
            }

            // If the client clicked OK and they chose to change the 
            // MIDI input device
            if(Dlg.IsInDevChanged())
            {
                if(m_InDevice.IsOpen())
                {
                    if(m_InDevice.IsRecording())
                    {
	                    // Stop receiving MIDI events 	
                        m_InDevice.StopRecording();   	
                    }
                
                    m_InDevice.Close(); 	
                }

                m_InDevice.Open(Dlg.GetInDevId());   	
                
                // Start receiving MIDI events 	
                m_InDevice.StartRecording(); 
            }
        }	
    }
    catch(const std::exception &ex)
    {
        MessageBox(ex.what(), "Error", MB_ICONSTOP | MB_OK);
    }
}


// Exit the program
void CDemoDlg::OnFileExit() 
{
	SendMessage(WM_CLOSE);
}


// Select a new General MIDI patch
void CDemoDlg::OnSelchangeGmList() 
{
	unsigned char Patch = m_GMCombo.GetCurSel();

    // Construct and send program change message
    midi::CShortMsg Msg(midi::PROGRAM_CHANGE, 0, Patch, 0, 0);
    Msg.SendMsg(m_OutDevice);	

    // Make sure the piano control regains focus when we are done with 
    // the Genera MIDI combo box
    m_Keys.SetFocus();
}


void CDemoDlg::OnHelpAbout() 
{
	CAboutDlg Dlg;

    Dlg.DoModal();	
}
