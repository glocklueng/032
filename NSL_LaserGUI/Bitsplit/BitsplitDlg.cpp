
// BitsplitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bitsplit.h"
#include "BitsplitDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CBitSplitDlg dialog



CString GetLoadFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir)
{
	CString szFile;
	TCHAR tmpFile[MAX_PATH];

	OPENFILENAME ofn;

	szFile.Empty();
	memset( tmpFile, 0, MAX_PATH );

	if( pStartDir){
		_tcscpy_s(tmpFile, sizeof(tmpFile)-1,pStartDir);
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


CBitSplitDlg::CBitSplitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBitSplitDlg::IDD, pParent)
	, m_BlackAndWhite(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Filename = _T("");
}

void CBitSplitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NUMBER_OF_LEVELS, m_NumberLayers);
	DDX_Control(pDX, IDC_PICTURE, m_ImageControl);
	DDX_Control(pDX, IDC_NUM_LAYERS, m_NumLevels);
	DDX_Check(pDX, IDC_BLACK_AND_WHITE, m_BlackAndWhite);
}

BEGIN_MESSAGE_MAP(CBitSplitDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LoadBMP, &CBitSplitDlg::OnBnClickedLoadbmp)
	ON_BN_CLICKED(IDC_SAVE_IMAGES, &CBitSplitDlg::OnBnClickedSaveImages)
	ON_BN_CLICKED(IDC_EXPORT_DXF, &CBitSplitDlg::OnBnClickedExportDxf)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_NUMBER_OF_LEVELS, &CBitSplitDlg::OnNMCustomdrawNumberOfLevels)
END_MESSAGE_MAP()


// CBitSplitDlg message handlers

BOOL CBitSplitDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	m_NumberLayers.SetRange(2,255);
	m_NumberLayers.SetPos( 10 );


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBitSplitDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBitSplitDlg::OnPaint()
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
HCURSOR CBitSplitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBitSplitDlg::OnBnClickedLoadbmp()
{
	m_Filename = GetLoadFile( _T("Supported Files Types(*.bmp)\0*.bmp\0\0"),_T("Pick a BMP"),NULL);

	m_Bitmap.Load( m_Filename) ;

	if( m_Bitmap.IsIndexed() ) 
		MessageBox(_T("Indexed BMP, not supported"),_T("Error"),0 );

	DrawPicture();
}

void CBitSplitDlg::OnBnClickedSaveImages()
{
	CImage newBitmap;
	
	UpdateData();

	int step = m_NumberLayers.GetPos();

	if( m_Bitmap.IsNull() ) return;

	// clone it
	newBitmap.Create( m_Bitmap.GetWidth(),m_Bitmap.GetHeight(),m_Bitmap.GetBPP());
	m_Bitmap.BitBlt(newBitmap.GetDC(), 0, 0);

	COLORREF low,high;
	float div;

	div = 255.0f / step;

	if (div == 0.0f ) {
		div = 1.0f;
	}

	for( int i = 0 ; i < step ; i ++ ) {

		low  = RGB(div*i,div*i,div*i);
		high = RGB(div*(i+1),div*(i+1),div*(i+1));

		BOOL  b = CreateImage( &newBitmap, &m_Bitmap, low , high );

		if( b && newBitmap ) {

			CString output;

			output.Format(_T("test%03d.bmp"),i);
			
			Sleep(1);
			newBitmap.Save(output, Gdiplus::ImageFormatBMP);
		}
	}
}


void CBitSplitDlg::OnBnClickedExportDxf()
{
}

void CBitSplitDlg::DrawPicture()   
{    
	RECT rect;   

    m_ImageControl.GetWindowRect( &rect );   
    ScreenToClient( &rect );   

	CBitmap *bmp = CBitmap::FromHandle(m_Bitmap);
   
    CDC *pDC = GetDC();   
	CDC memDC;
	
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(bmp);

	pDC->StretchBlt(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,&memDC,0,0,GetCBitmapWidth(*bmp),GetCBitmapHeight(*bmp),SRCCOPY);

	ReleaseDC(pDC);
	DeleteObject(bmp);

}   

// convert RGB to grey
COLORREF Grey( COLORREF rgb )
{
	float r,g,b;
	float grey;
		
	// Break into component values
	r = GetRValue(rgb);
	g = GetGValue(rgb);
	b = GetBValue(rgb);

	grey = (0.30f * r ) +( 0.59f * g) + (0.11f * b);

	return RGB(grey,grey,grey);
}

BOOL CBitSplitDlg::CreateImage( CImage *newBitmap, CImage *source_bitmap, COLORREF low_level, COLORREF high_level) 
{
	BOOL ret;


	// flagged as don not create file, all black
	ret = FALSE ;

	for( int y = 0; y  < GetCBitmapHeight( *source_bitmap ) ; y++ ) {

		for( int x = 0; x  <GetCBitmapWidth( *source_bitmap ) ; x++ ) {

			COLORREF source;

			// convert to grey (check if its already greyscale)
			source = Grey(source_bitmap->GetPixel( x, y) );

			if ( source >= low_level && source < high_level ) {

				// if its a black and white image, just write out full white
				if( m_BlackAndWhite ) { 
					
					if( newBitmap->IsIndexed() ) {
						newBitmap->SetPixelIndexed(x,y, 0);
					} else {
						newBitmap->SetPixel(x,y, source);
					}
				
				} else {			
				
					newBitmap->SetPixel(x,y, source );
				
				}

				ret = TRUE ;

			} else {
				// all black
				if( m_BlackAndWhite ) { 
					if( newBitmap->IsIndexed() )
						newBitmap->SetPixelIndexed(x,y, 1);
					else
						newBitmap->SetPixel(x,y, RGB(0xff,0xff,0xff));
				} else {
					newBitmap->SetPixel(x,y, RGB(0xff,0xff,0xff));
				}
			}
		}
	}
	
	return ret;

}

void CBitSplitDlg::OnNMCustomdrawNumberOfLevels(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	{

		CString num;
	
		num.Format(_T("%d"), m_NumberLayers.GetPos() );

		//Duplicated..
		m_NumLevels.SetWindowTextW( num );
		
	}
	*pResult = 0;
}
