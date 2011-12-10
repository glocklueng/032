
// PickobearDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "SerialMFC.h"
#include "afxcmn.h"

#include <iostream>
#include <fstream>


#define pulsestoum(x) (x*25)


CString GetSaveFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir);
CString GetLoadFile( const TCHAR *ptypes, const TCHAR*caption, const TCHAR *pStartDir);
bool SetCurrentPosition ( long x,long y);
void BuildGCodeMove( char *output, int length, int mode , long x, long y, long speed );



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

		// feeder id
		int feeder;
		
		// name of component
		char label[256];
		
		// value of label
		char value[256];
		
		// type
		char type[256];

	} CompDatabase;
	
	// database list
	std::vector<CompDatabase> m_Database;
	
	// temp pointer
	CompDatabase *entry;

	/// Offset of item in PCB
	long m_OffsetX;
	long m_OffsetY;

	int m_FeederId;

public:
	CListCtrl_Components() :
	  m_OffsetX(0),
	  m_OffsetY(0)
	{
		m_FeederId = -1;
		m_Count = 0;
		entry = NULL;
	
	}

	unsigned long m_Count ;

	unsigned long GetCount( void )
	{
		return m_Database.size();
	}


	void AssignFeeder(int idd, int id ) {

		m_FeederId = id;

		m_Database.at(idd).feeder = id;


	}

	CompDatabase  at( int i ) {
		return m_Database.at(i);
	}
 
	void AddItem( const char *label,const char *type,const char *value,const char *x,const char *y,const char *rot)
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
		
		entry.feeder = -1;

		// convert to level of accuracy pnp can handle
		entry.x = (atol( x )/40)*40 ;
		entry.y = (atol( y )/40)*40 ;
		entry.rot = atoi( rot ) ;

		CString temp( entry.label );

		int Index = InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);

		temp = type;

		SetItemText(Index,1,temp);
		SetItemText(Index,2,CString(value));
		SetItemText(Index,3,CString(x));
		SetItemText(Index,4,CString(y));
		SetItemText(Index,5,CString(rot));
		temp = L"NA";
		if ( entry.feeder != -1 ) {
			temp.Format(L"%d",entry.feeder);
		}
		SetItemText(Index,6,temp);

		// add to database
		m_Database.push_back (entry );

		m_Count ++;
	}

	void SaveDatabase ( void ) 
	{
		CString filename;
		 
		filename = ::GetSaveFile( _T("Supported Files Types(*.pbr)\0*.pbr\0\0"),_T("Choose a filename to save components in"),_T("") );

		std::ofstream os (filename, std::ios::out | std::ios::binary);

		int size1 = m_Database.size();
		os.write((const char*)&size1, sizeof(size1));
		os.write((const char*)&m_Database.front(), m_Database.size() * sizeof(CompDatabase));
		os.close();
	}

	void LoadDatabase ( void ) 
	{
		int size1;
		CString filename;

		filename = ::GetLoadFile( 
			_T("Supported Files Types(*.pbr)\0*.pbr\0\0"),
			_T("Choose name to load components from"),_T("") 
		);

		if(filename.GetLength() == 0 ) {
			return ;

		}

		if( m_Count ) {
			
			m_Database.clear();
		}

		std::ifstream is( filename, std::ios::binary );
			
		is.read( (char*)&size1, sizeof( size1) );
		
		m_Count = size1;	
		m_Database.resize(m_Count);
		
		is.read((char*)&m_Database.front(), m_Database.size() * sizeof( CompDatabase ) );
		
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
	typedef struct CompDatabase_tag  {
		
		// index ( not used )
		unsigned int item;
		
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

	std::vector<FeederDatabase> m_Database;
	
	FeederDatabase *entry;

	void SaveDatabase ( void ) 
	{
		CString filename;
		 
		filename = ::GetSaveFile( _T("Supported Files Types(*.fdr)\0*.fdr\0\0"),_T("Pick name to save database too"),_T("") );

		std::ofstream os (filename, std::ios::out | std::ios::binary);

		int size1 = m_Database.size();
		os.write((const char*)&size1, sizeof(size1));
		os.write((const char*)&m_Database.front(), m_Database.size() * sizeof(FeederDatabase));
		os.close();
	}

	void LoadDatabase ( void ) 
	{
		int size1;
		CString filename;

		// get the name of the file to load
		filename = ::GetLoadFile( 
			_T("Supported Files Types(*.fdr)\0*.fdr\0\0"),
			_T("Pick name to load database from"),_T("") 
		);

		// zero length file ?
		if(filename.GetLength() == 0 ) {
			return ;

		}

		// if anything in the database
		if( m_Count ) {
			
			m_Database.clear();
		}

		// open the file stream
		std::ifstream is( filename, std::ios::binary );
			
		// read the number of elements in the stream
		is.read( (char*)&size1, sizeof( size1) );
		
		// store it
		m_Count = size1;	
	
		// reserve space for the list
		m_Database.resize(m_Count);
		
		// read into list
		is.read((char*)&m_Database.front(), m_Database.size() * sizeof( FeederDatabase ) );
		
		// close file
		is.close();

		RebuildList();

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

	// number of items in list ( should match m_DataBase.size();
	unsigned long m_Count ;

	unsigned long GetCount( void )
	{
		return m_Database.size();
	}

	// fetch entry at index
	FeederDatabase &at( int i ) {
		if( i < m_Count ) 
			return m_Database.at(i);
		
		return *entry ;
	}
 
	void RebuildList ( void ) 
	{		
		CString temp;

		// remove any and all items from List
		DeleteAllItems();

		// for all items loaded
		for ( unsigned int i = 0 ; i < m_Count ; i ++ ) {
		
			// fetch entry 
			entry = &m_Database.at( i  ) ;
			
			temp = entry->label;

			// add first item
			int Index = InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);

			// convert to string
			temp.Format(L"%d",entry->x);
			SetItemText(Index,1,temp);
			
			temp.Format(L"%d",entry->y);
			SetItemText(Index,2,temp);

			temp.Format(L"%d",entry->rot);
			SetItemText(Index,3,temp);

			temp.Format(L"%d",entry->lx);
			SetItemText(Index,4,temp);

			temp.Format(L"%d",entry->ly);
			SetItemText(Index,5,temp);

			temp.Format(L"%d",entry->countx);
			SetItemText(Index,6,temp);

			temp.Format(L"%d",entry->county);
			SetItemText(Index,7,temp);


		}
	}

	// add an item to the list
	void AddItem( const char *label,const long x,long y,short rot )
	{
		ASSERT( label ) ;
		
		FeederDatabase entry;

		// zero it
		memset(&entry, 0, sizeof( FeederDatabase ) );

		// copy the label
		strcpy_s(entry.label,label );

		// convert to level of accuracy pnp can handle
		entry.x = (( x )/40)*40 ;
		entry.y = (( y )/40)*40 ;

		// rotation of the part
		entry.rot = ( rot ) ;

		// conver the sting from utf8
		CString temp( label );

		int Index = InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);
		temp.Format(L"%d",x);
		SetItemText(Index,1,temp);
		temp.Format(L"%d",y);
		SetItemText(Index,2,temp);
		temp.Format(L"%d",rot);
		SetItemText(Index,3,temp);
	
		// add to database
		m_Database.push_back (entry );

		// add one
		m_Count ++;
	}

////

	afx_msg void OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd,CPoint pos );
};

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

	// head state
	char m_Head;

	// tell thread to quit ( if true )
	int m_Quit;

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

// Construction
public:
	CPickobearDlg(CWnd* pParent = NULL);	// standard constructor

	CSerialMFC m_Serial;
	
	void EmptySerial ( void ) ;
	int SendCommand( int command );

	// set the camera threads
	static DWORD WINAPI goCamera(LPVOID pThis);
	static DWORD WINAPI goSetup(LPVOID pThis);

	DWORD goThread(void );
	DWORD cameraThread(void );

	// check acknowledgement fron pnp
	char CheckAck(char *ack1);

	unsigned char VacuumTest( void );
	// move head to x,y
	bool MoveHead( long  x, long y );
	bool MoveHeadRel(  long x, long y ); 
	bool MoveHeadSlow(  long x, long y );

	~CPickobearDlg(){
		
		// tell thread to close
		m_Quit = 1;

		// close out serial
		m_Serial.Close();
	}

	// send a command to the PNP
	void SendCommand(const char *cmd)
	{
		DWORD written;

		m_Serial.Write(cmd,&written,0,INFINITE);
	};


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

	void GotoTestPad( void );
	afx_msg LRESULT OnSerialMsg (WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedAddLowerright();
	CComboBox m_StepSize;
	afx_msg void OnCbnSelchangeStepsize();
	afx_msg void OnBnClickedEditfeeder();
	afx_msg void OnBnClickedAssignfeeder();
};
