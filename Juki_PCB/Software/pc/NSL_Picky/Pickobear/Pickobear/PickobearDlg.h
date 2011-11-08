
// PickobearDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "Serial.h"
#include "afxcmn.h"

#include <iostream>
#include <fstream>

class CListCtrl_Components : public CListCtrl
{
	afx_msg void OnNcLButtonDblClk (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP();

	void PreSubclassWindow();
public:
	typedef struct CompDatabase_tag  {
		unsigned int item;
		unsigned long x,y;
		short rot;
		int feeder;
		CString label;
		CString value;
		CString type;
	} CompDatabase;
	
	std::vector<CompDatabase> m_Database;
	
	CompDatabase *entry;

	
	/// Offset of item in PCB
	long m_OffsetX;
	long m_OffsetY;

	int m_FeederId;

public:
	CListCtrl_Components()
	{
		m_OffsetX = 0 ;
		m_OffsetY = 0;
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

		entry.label = label;
		entry.type = type;
		entry.value  = value;
		entry.feeder = -1;

		// convert to level of accuracy pnp can handle
		entry.x = (atol( x )/40)*40 ;
		entry.y = (atol( y )/40)*40 ;
		entry.rot = atoi( rot ) ;

		int Index = InsertItem(LVIF_TEXT, 0,entry.label, 0, 0, 0, NULL);

			SetItemText(Index,1,entry.type);
			SetItemText(Index,2,CString(value));
			SetItemText(Index,3,CString(x));
			SetItemText(Index,4,CString(y));
			SetItemText(Index,5,CString(rot));
			CString temp(L"NA");
			if ( entry.feeder != -1 ) {
				temp.Format(L"%d",entry.feeder);
			}
			SetItemText(Index,6,temp);

		// add to database
		m_Database.push_back (entry );

		m_Count ++;
	}
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
		unsigned int item;
		unsigned long x,y;
		short rot;
		char label[256];
	} FeederDatabase;
		
	CMenu cMenu;

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

		filename = ::GetLoadFile( 
			_T("Supported Files Types(*.fdr)\0*.fdr\0\0"),
			_T("Pick name to load database from"),_T("") 
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
		
		is.read((char*)&m_Database.front(), m_Database.size() * sizeof( FeederDatabase ) );
		
		is.close();

		CString temp;

		DeleteAllItems();

		for ( int i = 0 ; i < m_Count ; i ++ ) {
		
			entry = &m_Database.at( i  ) ;
			
			temp = entry->label;

			int Index = InsertItem(LVIF_TEXT, 0,temp, 0, 0, 0, NULL);

			SetItemText(Index,1,temp);
			temp.Format(L"%d",entry->x);
			SetItemText(Index,2,temp);
			temp.Format(L"%d",entry->y);
			SetItemText(Index,3,temp);
			temp.Format(L"%d",entry->rot);
			SetItemText(Index,4,temp);
		}
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

	unsigned long m_Count ;

	unsigned long GetCount( void )
	{
		return m_Database.size();
	}

	FeederDatabase  at( int i ) {
		return m_Database.at(i);
	}
 
	void AddItem( const char *label,const long x,long y,short rot )
	{
		ASSERT( label ) ;
		
		FeederDatabase entry;

		memset(&entry, 0, sizeof( FeederDatabase ) );

		strcpy_s(entry.label,label );

		// convert to level of accuracy pnp can handle
		entry.x = (( x )/40)*40 ;
		entry.y = (( y )/40)*40 ;
		entry.rot = ( rot ) ;

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

		m_Count ++;
	}
	afx_msg void OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd,CPoint pos );
};

// CPickobearDlg dialog
class CPickobearDlg : public CDialog
{
private:
	// each of the camera windows
	COpenGLControl m_oglWindow;
	COpenGLControl m_oglWindow1;
	
	COGLThread m_OGLThread;

	CFeederSetup *m_pFeederDlg ;
	
	char m_Head;
	int m_Quit;


	enum eMachineState {
		MS_IDLE,MS_GO
	};

	eMachineState m_MachineState;

// Construction
public:
	CPickobearDlg(CWnd* pParent = NULL);	// standard constructor
	
	int SendCommand( int command );

	static DWORD WINAPI goCamera(LPVOID pThis);
	static DWORD WINAPI goSetup(LPVOID pThis);

	DWORD goThread(void );
	DWORD cameraThread(void );

	char CheckAck(char ack,char ack1);
	bool MoveHead( long x, long y );

	~CPickobearDlg(){
		
		m_Quit = 1;

		m_Serial.Close();
	}

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
//	CListCtrl m_Components;
	CButton GO;
	afx_msg void OnLbnSelchangeList1();
	long m_headXPos;
	long m_headYpos;
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
	afx_msg void OnBnClickedUpdate2();
	afx_msg void OnBnClickedUpdate3();
};
