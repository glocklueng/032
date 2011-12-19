
// PickobearDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "SerialMFC.h"
#include "afxcmn.h"
#include "OpenGLControl.h"
#include "TextDump.h"

#include <iostream>
#include <fstream>

enum {
	NT_REAL = 0,
	NT_INTEGER = 1,
	NT_DOUBLE = 2,
	NT_HEX = 3,
	NT_BINARY = 4
};

#define MAX_X_TABLE		( 190000 )
#define MAX_Y_TABLE		( 190000 )

#define CAMERA_DEFAULT_UPDATE_RATE_MS		( 100 )
#define CAMERA_SLOW_UPDATE_RATE_MS			( 300 )

#define pulsestoum(x) (x*25)

CString GetSaveFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir);
CString GetLoadFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir);
bool SetCurrentPosition ( long x,long y);
bool BuildGCodeMove( char *output, int length, int mode , long x, long y, long speed );

class Feeder;


// component class
class CListCtrl_Components : public CListCtrl
{
	afx_msg void OnNcLButtonDblClk (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP();

	void PreSubclassWindow();
public:

	// component database
	typedef struct CompDatabase_tag  {
		
		// iten number
		unsigned int item;
		
		// position
		long x,y;
		
		// rotation of part
		short rot;

		// feeder id (by name)
		char feeder[256];
		
		// name of component
		char label[256];
		
		// value of label
		char value[256];
		
		// type
		char type[256];

		// side of pcb
		BYTE side;

	} CompDatabase;
	
	// database list
	std::vector<CompDatabase> m_ComponentDatabase;
	
	// temp pointer
	CompDatabase *entry;

	/// Offset of item in PCB
	long m_OffsetX;
	long m_OffsetY;

	long m_OffsetX_bottom;
	long m_OffsetY_bottom;

	long m_OffsetX_top;
	long m_OffsetY_top;

	int m_FeederId;

public:
	CListCtrl_Components() :
	  m_OffsetX(0),
	  m_OffsetY(0),
	  m_OffsetX_top(0),
	  m_OffsetY_top(0),
	  m_OffsetX_bottom(0),
	  m_OffsetY_bottom(0)
	{
		m_FeederId = -1;
		m_Count = 0;
		entry = NULL;
	
	}

	unsigned long m_Count ;

	unsigned long GetCount( void )
	{
		return m_ComponentDatabase.size();
	}


	void AssignFeeder(int idd, const char *string ) {

		ASSERT( string );

		strncpy_s( m_ComponentDatabase.at(idd).feeder,string, 256 );

	}

	CompDatabase  at( int i ) {
		return m_ComponentDatabase.at(i);
	}
 
	void AddItem( const char *label,const char *type,const char *value,const char *x,const char *y,const char *rot,const char *side)
	{
		ASSERT( label ) ;
		ASSERT( type ) ;
		ASSERT( value ) ;
		ASSERT( x ) ;
		ASSERT( y ) ;
		ASSERT( rot ) ;

		CompDatabase entry;

		strcpy_s( entry.label, label );
		strcpy_s( entry.type, type);
		strcpy_s( entry.value , value);
		
		memset(entry.feeder,0,sizeof(entry.feeder));

		// convert to level of accuracy pnp can handle
		entry.x = (atol( x )/40)*40 ;
		entry.y = (atol( y )/40)*40 ;
		entry.rot = atoi( rot ) ;
		entry.side = atoi (side);

		CString temp( entry.label );

		int Index = InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);

		temp = type;

		SetItemText(Index,1,temp);
		SetItemText(Index,2,CString(value));
		SetItemText(Index,3,CString(x));
		SetItemText(Index,4,CString(y));
		SetItemText(Index,5,CString(rot));
		temp = L"NA";
		if ( strlen( entry.feeder ) == 0  ) {
			temp.Format(L"%s",entry.feeder);
		}
		SetItemText(Index,6,temp);

		SetItemText(Index,7,CString(side));

		// add to database
		m_ComponentDatabase.push_back ( entry );

		m_Count ++;
	}

	void SaveDatabase ( void ) 
	{
		CString filename;
				
		if( m_ComponentDatabase.size() == 0 ) {
			return;
		}


		filename = ::GetSaveFile( _T("Supported Files Types(*.pbr)\0*.pbr\0\0"),_T("Choose a filename to save components in"),_T("") );

		if( filename.Find(L".pbr") == -1 ) {
			filename.Append(L".pbr");
		}

		std::ofstream os (filename, std::ios::out | std::ios::binary);

		int size1 = m_ComponentDatabase.size();
		os.write((const char*)&size1, sizeof(size1));
		os.write((const char*)&m_ComponentDatabase.front(), m_ComponentDatabase.size() * sizeof(CompDatabase));
		os.close();

		CRegKey regKey;
		DWORD regEntry = 100;
		long lResult;

		if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
			lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

		if (ERROR_SUCCESS == lResult)
		{

			regKey.SetStringValue(_T("componentDatabase"), filename,REG_SZ);

			// then close the registry key
			regKey.Close();
		}

	}

	void LoadDatabase ( CString name ) 
	{
		int size1;
		CString filename;

		if( name.GetLength() == 0 ){

			filename = ::GetLoadFile( 
				_T("Supported Files Types(*.pbr)\0*.pbr\0\0"),
				_T("Choose name to load components from"),_T("") 
			);

			if(filename.GetLength() == 0 ) {
				return ;

			}

			CRegKey regKey;
			DWORD regEntry = 100;
			long lResult;

			if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
				lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

			if (ERROR_SUCCESS == lResult)
			{
	
				regKey.SetStringValue(_T("componentDatabase"), filename,REG_SZ);

				// then close the registry key
				regKey.Close();
			}


		} else 
			filename = name;

		if( m_Count ) {
			
			m_ComponentDatabase.clear();
		}

		std::ifstream is( filename, std::ios::binary );
			
		is.read( (char*)&size1, sizeof( size1) );
		
		m_Count = size1;	
		m_ComponentDatabase.resize(m_Count);
		
		is.read((char*)&m_ComponentDatabase.front(), m_ComponentDatabase.size() * sizeof( CompDatabase ) );
		
		is.close();
		
		RebuildList();

	}

	void RebuildList( void );

	afx_msg void OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
};

class CListCtrl_FeederList : public CListCtrl
{
	afx_msg void OnNcLButtonDblClk (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP();

	void PreSubclassWindow();
public:
	typedef struct FeederDatabase_tag  {
		
		// component index, starts at 0, goes up
		unsigned int componentIndex;

		// coordinates in um
		long x,y;
		
		// rotation in degrees
		short rot;

		// name for feeder
		char label[256];

/*
 x,y is the centre of the component
 count is the number of components
 lx,ly is the centre of the right most, bottom most component.

		                  count x --->
		x,y------+-------+-------+-------+-------+
		 |       |       |       |       |       | count y      
		 +-------+-------+-------+-------+-------+ |
		 |       |       |       |       |       | |    
		 +-------+-------+-------+-------+-------+\ /
		 |       |       |       |       |       |      
		 +-------+-------+-------+-------+-------+lx,ly

*/

		// bottom right coordinate
		long lx,ly;

		// number off components  --->x, --->y
		unsigned long countx,county;

		// which tool to use
		unsigned char tool;

	} FeederDatabase;
		
	CMenu *cMenu;

	std::vector<FeederDatabase> mFeederDatabase;

	// New feeder list
	std::vector<Feeder> m_Feeders;
	
	FeederDatabase *entry;

	void SaveDatabase ( void ) 
	{

		if( mFeederDatabase.size() == 0 ) {
			return;
		}

		CString filename;
		 
		filename = ::GetSaveFile( _T("Supported Files Types(*.fdr)\0*.fdr\0\0"),_T("Pick name to save database too"),_T("") );
		// zero length file ?
		if(filename.GetLength() == 0 ) {
			return ;

		}
				
		if( filename.Find(L".fdr") == -1 ) {
			filename.Append(L".fdr");
		}


		std::ofstream os (filename, std::ios::out | std::ios::binary);

		int size1 = mFeederDatabase.size();
		os.write((const char*)&size1, sizeof(size1));
		os.write((const char*)&mFeederDatabase.front(), mFeederDatabase.size() * sizeof(FeederDatabase));
		os.close();
	}

	void LoadDatabase ( CString name ) 
	{
		int size1;
		CString filename;

		if( name.GetLength() == 0 ) {
			// get the name of the file to load
			filename = ::GetLoadFile( 
				_T("Supported Files Types(*.fdr)\0*.fdr\0\0"),
				_T("Pick name to load database from"),_T("") 
			);

			// zero length file ?
			if(filename.GetLength() == 0 ) {
				return ;

			}
				
			CRegKey regKey;
			DWORD regEntry = 100;
			long lResult;

			if ((lResult = regKey.Open(HKEY_CURRENT_USER,  _T("Software\\NullSpaceLabs\\PickoBear\\Settings"))) != ERROR_SUCCESS)
				lResult = regKey.Create(HKEY_CURRENT_USER, _T("Software\\NullSpaceLabs\\PickoBear\\Settings"));

			if (ERROR_SUCCESS == lResult)
			{
	
				regKey.SetStringValue(_T("feederDatabase"), filename,REG_SZ);

				// then close the registry key
				regKey.Close();
			}


		} else {
			filename = name;
		}

		// if anything in the database
		if( m_Count ) {
			
			mFeederDatabase.clear();
		}

		// open the file stream
		std::ifstream is( filename, std::ios::binary );
			
		// read the number of elements in the stream
		is.read( (char*)&size1, sizeof( size1) );
		
		// store it
		m_Count = size1;	
	
		// reserve space for the list
		mFeederDatabase.resize(m_Count);
		
		// read into list
		is.read((char*)&mFeederDatabase.front(), mFeederDatabase.size() * sizeof( FeederDatabase ) );
		
		// close file
		is.close();

		RebuildList();

	}

	// search for feeder by name
	int Search( const char *name ) 
	{
		ASSERT( name );

		for( int i = 0 ; i < m_Count ; i++ ) {
			
			if(strcmp(name, mFeederDatabase.at(i).label) == 0 ) {
				return i;
			}
		}

		// nope
		return -1;
	}


	/// Offset of item in PCB
	long m_OffsetX;
	long m_OffsetY;


public:
	CListCtrl_FeederList()
	{
		m_OffsetX = 0 ;
		m_OffsetY = 0;
		m_Count = 0;
		entry = NULL;
	}

	// number of items in list ( should match mFeederDatabase.size();
	unsigned long m_Count ;

	unsigned long GetCount( void )
	{
		return mFeederDatabase.size();
	}

	// fetch entry at index
	FeederDatabase &at( int i ) {
		if( i < m_Count ) 
			return mFeederDatabase.at(i);
		
		return *entry ;
	}
 
	void RebuildList ( void ) ;

	// add an item to the list
	void AddItem( const char *label,const long x,long y,short rot );
////

	afx_msg void OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd,CPoint pos );
};

class CFeederSetup;

// CPickobearDlg dialog
class CPickobearDlg : public CDialog
{
private:
	// each of the camera windows
	COpenGLControl m_UpCameraWindow;
	COpenGLControl m_DownCameraWindow;
	// camera update thread
	COGLThread m_OGLThread;

	// point to feeder dialogue
	CFeederSetup *m_pFeederDlg ;

	CTextDump	*m_TextEdit;

	bool bCameraHead;

	bool bSetWaitDone;

	// machine is doing something
	bool bBusy;

	// head state
	char m_Head;

	// machine has been homed
	bool m_Homed;


	//how long Sleep is in camera updates (ms)
	unsigned int m_CameraUpdateRate;

	// states the machine could be in
	enum eMachineState {

		MS_IDLE,
		MS_STOP,
		MS_GO

	};

	// current state
	eMachineState m_MachineState;

	int m_Speed;
	// camera goto (1) or head goto ( 0 )s
	int m_CameraMode ;
public:

	// if board is flipped
	bool bFlip;

		// tell thread to quit ( if true )
	int m_Quit;


// Construction
public:
	CPickobearDlg(CWnd* pParent = NULL);	// standard constructor

	CSerialMFC m_Serial;

	void WriteSerial( const char *text);

	void EmptySerial ( void ) ;
	int SendCommand( int command );
	
	// checks all the parts and feeders etc are all defined correctly, pass true if all parts(on side) are being placed.

	bool PreRunCheck( bool all_parts );

	// set the camera threads
	static DWORD WINAPI goCamera(LPVOID pThis);
	static DWORD WINAPI goSetup(LPVOID pThis);

	static DWORD WINAPI goSingleSetup(LPVOID pThis);

	DWORD goThread(void );
	DWORD cameraThread(void );
	DWORD goSingleThread(void );

	// check acknowledgement fron pnp
	char CheckAck(char *ack1);
	char CheckX( void );

	unsigned char VacuumTest( void );
	// move head to x,y
	bool MoveHead( long  x, long y );
	bool MoveHeadRel(  long x, long y ); 
	bool MoveHeadSlow(  long x, long y );

	// check to see if if gui thinks we are homed
	bool HomeTest( void ); 


	bool m_FeedersModified;
	bool m_ComponentsModified;


	~CPickobearDlg(){
		
		if (m_ComponentsModified ) {
			m_ComponentList.SaveDatabase();
		}

		if (m_FeedersModified ) {
			m_FeederList.SaveDatabase();
		}

		// tell thread to close
		m_Quit = 1;
		if( m_Serial.IsOpen() )
			// close out serial
			m_Serial.Close();
	}

	// send a command to the PNP
	void SendCommand(const char *cmd)
	{
		DWORD written;

		m_Serial.Write(cmd,&written,0,INFINITE);
	};


	bool m_Simulate;

// Dialog Data
	enum { IDD = IDD_PICKOBEAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton GO;
	afx_msg void OnLbnSelchangeList1();
	long m_headXPos;
	long m_headYPos;
	double m_Threshold1;
	double m_Threshold2;
	afx_msg void OnEnChangeThreshold();
	afx_msg void OnEnChangeThreshold2();
	afx_msg void OnBnClickedHome();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedPark();
	afx_msg void OnBnClickedTool1();
	afx_msg void OnBnClickedTool2();
	afx_msg void OnBnClickedTool3();
	afx_msg void OnBnClickedTool4();
	afx_msg void OnBnClickedTool5();
	afx_msg void OnBnClickedTool6();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedHead();
	afx_msg void OnNMCustomdrawRotate(NMHDR *pNMHDR, LRESULT *pResult);
	// rotation for head
	CSliderCtrl m_Rotation;
	CListCtrl_Components m_ComponentList;
	afx_msg void OnBnClickedUpleft();
	afx_msg void OnBnClickedUpright();
	afx_msg void OnBnClickedLeftdown();
	afx_msg void OnBnClickedBottomleft();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedGo();
	afx_msg void OnBnClickedFeeder();
	afx_msg void OnBnClickedZero();
	afx_msg void OnBnClickedOffset();
	afx_msg void OnBnClickedGoff();
	afx_msg void OnBnClickedGoxy();
	long m_GOX;
	long m_GOY;
	afx_msg void OnEnChangeGox();
	afx_msg void OnEnChangeGoy();
	CListCtrl_FeederList m_FeederList;
	afx_msg void OnBnClickedAddFeeder();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedLoadFeeder();
	afx_msg void OnBnClickedSaveFeeder();
	afx_msg void OnBnClickedH2C();
	afx_msg void OnBnClickedC2H();
	CComboBox m_UpCamera;
	CComboBox m_DownCamera;
	afx_msg BOOL OnToolTipNotify( UINT id,NMHDR* pNMHDR,LRESULT* pResult );
	void GotoTestPad( void );
	afx_msg LRESULT OnSerialMsg (WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeDownCamera();
	afx_msg void OnCbnSelchangeUpCamera();
	afx_msg void OnBnClickedAddLowerright();
	CComboBox m_StepSize;
	afx_msg void OnCbnSelchangeStepsize();
	afx_msg void OnBnClickedEditfeeder();
	afx_msg void OnBnClickedAssignfeeder();
	afx_msg void OnBnClickedConsole();
	afx_msg void OnBnClickedGo2();
	afx_msg void OnBnClickedEditComponent();
	afx_msg void OnBnClickedSwapHeadCamera();
	int m_Side;
	afx_msg void OnBnClickedDeleteFeeder();
	afx_msg void OnBnClickedPcbFlip();
};
