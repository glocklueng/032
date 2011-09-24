
// PickobearDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "Serial.h"
#include "afxcmn.h"

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
		CString label;
		CString type;
	} CompDatabase;
	
	std::vector<CompDatabase> m_Database;
	
	CompDatabase *entry;

	
	/// Offset of item in PCB
	long m_OffsetX;
	long m_OffsetY;

public:
	CListCtrl_Components()
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

	CompDatabase  at( int i ) {
		return m_Database.at(i);
	}
 
	void AddItem( const char *label,const char *type,const char *x,const char *y,const char *rot)
	{
		ASSERT( label ) ;
		ASSERT( type ) ;
		ASSERT( x ) ;
		ASSERT( y ) ;
		ASSERT( rot ) ;

		CompDatabase entry;

		entry.label = label;
		entry.type = type;
		entry.x = atol( x ) ;
		entry.y = atol( y ) ;
		entry.rot = atoi( rot ) ;

		int Index = InsertItem(LVIF_TEXT, 0,entry.label, 0, 0, 0, NULL);

			SetItemText(Index,1,entry.type);
			SetItemText(Index,2,CString(x));
			SetItemText(Index,3,CString(y));
			SetItemText(Index,4,CString(rot));
			SetItemText(Index,5,_T("NA"));

		// add to database
		m_Database.push_back (entry );

		m_Count ++;
	}



	afx_msg void OnHdnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult);
};


// CPickobearDlg dialog
class CPickobearDlg : public CDialog
{
private:
	// each of the camera windows
	COpenGLControl m_oglWindow;
	COpenGLControl m_oglWindow1;
	CFeederSetup *m_pFeederDlg ;
	
		char m_Head;

	enum eMachineState {
		MS_IDLE,MS_GO
	};

	eMachineState m_MachineState;

// Construction
public:
	CPickobearDlg(CWnd* pParent = NULL);	// standard constructor
	
	
	static DWORD WINAPI goSetup(LPVOID pThis);
	DWORD goThread(void );
	char CheckAck(char ack,char ack1);
	bool MoveHead( long x, long y );

	~CPickobearDlg(){

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
};
