// DemoDlg.h : header file
//

#if !defined(AFX_DEMODLG_H__51B03386_1002_11D7_865D_0030BD08B6D9__INCLUDED_)
#define AFX_DEMODLG_H__51B03386_1002_11D7_865D_0030BD08B6D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MIDIKeyboard.h"
#include "MIDIOutDevice.h"
#include "MIDIInDevice.h"

#include "LongMsg.h"

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

class CDemoDlg : public CDialog, public CPianoCtrlListener,
                 public midi::CMIDIReceiver
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor
    ~CDemoDlg();

    void OnNoteOn(CPianoCtrl &PianoCtrl, unsigned char NoteId);
    void OnNoteOff(CPianoCtrl &PianoCtrl, unsigned char NoteId);

    // Receives short messages
    void ReceiveMsg(DWORD Msg, DWORD TimeStamp);

    // Receives long messages
    void ReceiveMsg(LPSTR Msg, DWORD BytesRecorded, DWORD TimeStamp) {}

    // Called when an invalid short message is received
    void OnError(DWORD Msg, DWORD TimeStamp);

    // Called when an invalid long message is received
    void OnError(LPSTR Msg, DWORD BytesRecorded, DWORD TimeStamp) {}

// Dialog Data
	//{{AFX_DATA(CDemoDlg)
	enum { IDD = IDD_MIDIDEVDEMOV2_DIALOG };
		CComboBox	m_GMCombo;
        CStatic     m_Command;
        CStatic     m_Channel;
        CStatic     m_Data1;
        CStatic     m_Data2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    CMIDIKeyboard m_Keys;
    midi::CMIDIOutDevice m_OutDevice;
    midi::CMIDIInDevice m_InDevice;

	// Generated message map functions
	//{{AFX_MSG(CDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPrefNoteColor();
	afx_msg void OnPrefMididevices();
	afx_msg void OnFileExit();
    afx_msg void OnSelchangeGmList();
	afx_msg void OnHelpAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODLG_H__51B03386_1002_11D7_865D_0030BD08B6D9__INCLUDED_)
