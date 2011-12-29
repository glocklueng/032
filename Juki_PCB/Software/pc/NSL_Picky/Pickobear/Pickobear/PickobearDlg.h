
// PickobearDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "SerialMFC.h"
#include "afxcmn.h"
#include "OpenGLControl.h"
#include "TextDump.h"
#include "AlertBox.h"

#include <iostream>
#include <fstream>
#include "afxvslistbox.h"

enum {
	NT_REAL = 0,
	NT_INTEGER = 1,
	NT_DOUBLE = 2,
	NT_HEX = 3,
	NT_BINARY = 4
};

#define MAX_X_TABLE							( 364550 )
#define MAX_Y_TABLE							( 517000 )

// PickoBear custom msgs
#define PB_UPDATE_ALERT						(WM_APP + 1)
#define PB_UPDATE_XY						(PB_UPDATE_ALERT + 1)
#define PB_TEXT_OUT							(PB_UPDATE_XY + 1 )


// for updating the alert box
#define	WAITING_FOR_CMD_ACK					( 1 )
#define WAITING_FOR_CMD_MOVE_ACK			( WAITING_FOR_CMD_ACK +1 )

// last calibration was
//was 228370
//is  227830

#define CAMERA_OFFSET						( 73900-540+120 )
#define CAMERA_DEFAULT_UPDATE_RATE_MS		( 10 )
#define CAMERA_SLOW_UPDATE_RATE_MS			( 300 )

#define pulsestoum(x) (x*25)

CString GetSaveFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir);
CString GetLoadFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir);
bool SetCurrentPosition ( long x,long y);
bool BuildGCodeMove( char *output, int length, int mode , long x, long y, long speed );

class Feeder;

// callback after GCODE has run
typedef bool (*gcode_callback)(void*pThis,void*userdata);


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

	unsigned long m_Count ;

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

	unsigned long GetCount( void )
	{
		return m_ComponentDatabase.size();
	}

	void AssignFeeder(unsigned int idd, const char *string ) {

		ASSERT( string );

		if (string == NULL ) {
			AfxMessageBox(L"Error:Assign feeder no data error", MB_OK|MB_ICONSTOP );
			return ;
		}

		// assert ?
		if ( idd > GetCount() ) {
			AfxMessageBox(L"Error:AssignFeeder range error", MB_OK|MB_ICONSTOP );
			return ;
		}

		strncpy_s( m_ComponentDatabase.at(idd).feeder,string, 256 );

	}

	CompDatabase at( unsigned int i ) {

		if (  i > m_Count ) {

			AfxMessageBox(L"Error:CompDatabase.at range error", MB_OK|MB_ICONSTOP );
		}

		return m_ComponentDatabase.at(i);
	}
 
	// search for component by name
	int Search( const char * name) 
	{

		for( unsigned int i = 0 ; i < m_Count ; i++ ) {
			
			if( strcmp(name, m_ComponentDatabase.at(i).label ) == 0 ) {

				return i;

			}
		}

		// nope
		return -1;
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

		strcpy_s( entry.label , label );
		strcpy_s( entry.type  , type  );
		strcpy_s( entry.value , value );
		
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

		for( unsigned int i = 0 ; i < m_Count ; i++ ) {
			
			if(strcmp(name, mFeederDatabase.at( i ).label) == 0 ) {
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
	FeederDatabase &at( unsigned int i ) {

		if(i < m_Count ) {
		
			return mFeederDatabase.at( i );
		}

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
	
	// The camera windows
	COpenGLControl m_UpCameraWindow;
	COpenGLControl m_DownCameraWindow;

	// camera update thread
	COGLThread m_OGLThread;

	// point to feeder dialogue
	CFeederSetup *m_pFeederDlg ;

	// Text edit control
	CTextDump	*m_TextEdit;

	// if true, camera and head are swapped
	bool bCameraHead;

	std::string m_GCODECMDBuffer;
	
	HANDLE threadProcessGCODE;

	bool AddGCODECommand( std::string gcode ,std::string error_message, gcode_callback func_ptr_completed  );

	typedef struct gcode_command_tag {
		
		// GCODE instruction
		std::string gcode;
		
		// message to display after error
		std::string error_message;

		// function to call after completed ( Typically this is only for moves )
		gcode_callback func_ptr_completed;

	} gcode_command;

	// buffer of commands
	std::vector<gcode_command> command_buffer;

	// is the buffer empty?
	bool QueueEmpty(void){
		return command_buffer.empty();
	}

	// machine is doing something
	bool bBusy;

	// head state
	char m_Head;

	// vacuum state
	char m_Vacuum;

	// limit states
	unsigned char m_LimitState;

	// machine has been homed
	bool m_Homed;

	//how long Sleep is in camera updates (ms)
	unsigned int m_CameraUpdateRate;

	// thread handle for camera update routine
	HANDLE threadHandleCamera;

	// states the machine could be in
	enum eMachineState {

		// chillin'
		MS_IDLE = 1 ,
		// gui asked machine to stop
		MS_STOP = 2,
		// machine is doing something
		MS_GO = 3,
		// emergency stop
		MS_ESTOP = 4

	};
	
	// current state of machine
	eMachineState m_MachineState;

// multiple PCB spport

	// this stores the offset of each PCB ( top and bottom )
	typedef struct PCBEntry_tag  {

		//top left
		long x_top;
		long y_top;

		//bottom right
		long x_bottom;
		long y_bottom;

	}PCBEntry;

	// number of entries in list ( this seems pointless since vector has a list count)
	unsigned int m_PCBCount;
	std::vector<PCBEntry> m_PCBs;


	// slew speed
	unsigned int m_Speed;

public:

	// if board is flipped
	bool bFlip;

	// tell thread to quit ( if true )
	int m_Quit;

	// used by the gui to pass text from threads to the console
	CString m_TextOut;



// Construction
public:
	CPickobearDlg(CWnd* pParent = NULL);	// standard constructor

	CSerialMFC m_Serial;
	
	CAlertBox *m_AlertBox;

	static DWORD WINAPI StartGCODEThread(LPVOID pThis);
	DWORD ProcessGCODECommandsThread(void );

// Callbacks

	// default callback
	static bool UpdatePosition_callback(void *pThis, void *userdata );
	// actual callback
	bool UpdatePosition_cb2( void *userdata ) ;
	
	// called after home is finished
	static bool Home_callback( void *pThis, void *userdata ) ;
	bool Home_cb2(void *userdata ) ;
	
	// called after GCODE commands has been submitted
	bool GCODE_CommandAck_callback( void *pThis, void *userdata ); 
	bool GCODE_CommandAck_cb2(void *userdata ) ;

// end callbacks

	void SetControls( boolean state );
	
	// checks all the parts and feeders etc are all defined correctly, pass true if all parts(on side) are being placed.

	bool PreRunCheck( bool all_parts );

	// set the camera threads
	void goCamera(LPVOID pThis);
	static DWORD WINAPI goSetup(LPVOID pThis);

	static DWORD WINAPI goSingleSetup(LPVOID pThis);

	DWORD goThread(void );
	DWORD cameraThread(void );
	DWORD goSingleThread(void );

	// check acknowledgement fron pnp
	char CheckAck(char *ack1);
	bool CheckX( void );

	// move head to x,y
	bool MoveHead( long  x, long y ,bool wait);
	bool MoveHeadRel(  long x, long y,bool wait ); 
	bool MoveHeadSlow(  long x, long y,bool wait );

	// check to see if if gui thinks we are homed
	bool HomeTest( void ); 

	/// set to true if a database save is in order
	bool m_FeedersModified;
	bool m_ComponentsModified;

	~CPickobearDlg();

	bool m_Simulate;

// Dialog Data
	enum { IDD = IDD_PICKOBEAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// these are now only allowed to be run from the GCODE thread
	void InternalWriteSerial( const char *text,bool noConsole);
	void EmptySerial ( void ) ;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT UpdateAlertMessage (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT UpdateXY (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT UpdateTextOut (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CButton GO;
	afx_msg void OnLbnSelchangeList1();
	long m_headXPos;
	long m_headYPos;
	double m_Threshold1;
	double m_Threshold2;
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
	afx_msg void OnBnClickedVacuumToggle();
	afx_msg void OnCbnSelchangeGSpeed();
	CComboBox m_SpeedSelect;
	CListCtrl m_PCBList;
	afx_msg void OnStnClickedSimulation();
	afx_msg void OnBnClickedAddPcb();
	// Current index of PCB we are placing if there are multiple PCB's
	unsigned long m_PCBIndex;
	afx_msg void OnBnClickedDeletePcb();
	afx_msg void OnBnClickedTestMode();
	afx_msg void OnClose();
	afx_msg void OnBnClickedEstop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
