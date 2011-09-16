
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
	DDX_Control(pDX, IDC_LIST1, m_Components);
	DDX_Control(pDX, IDC_GO, GO);
	DDX_Text(pDX, IDC_X_POS, m_headXPos);
	DDX_Text(pDX, IDC_Y_POS, m_headYpos);
	DDX_Text(pDX, IDC_THRESHOLD, m_Threshold1);
	DDX_Text(pDX, IDC_THRESHOLD2, m_Threshold2);
}

BEGIN_MESSAGE_MAP(CPickobearDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_LIST1, &CPickobearDlg::OnLbnSelchangeList1)
	ON_EN_KILLFOCUS(IDC_THRESHOLD, &CPickobearDlg::OnEnChangeThreshold)
	ON_EN_KILLFOCUS(IDC_THRESHOLD2, &CPickobearDlg::OnEnChangeThreshold2)
END_MESSAGE_MAP()


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
	m_oglWindow.oglCreate( rect, rect1, this, 2 );

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_CAM2)->GetWindowRect(rect);
	GetDlgItem(IDC_CAM2)->GetWindowRect(rect1);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow1.oglCreate( rect, rect1, this, 3 );

	// Setup the OpenGL Window's timer to render
	m_oglWindow1.m_unpTimer = m_oglWindow1.SetTimer(1, 1, 0);



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
