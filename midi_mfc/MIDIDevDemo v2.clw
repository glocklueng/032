; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDemoDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MIDIDevDemo v2.h"

ClassCount=5
Class1=CDemoApp
Class2=CDemoDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_DEMO_DIALOG
Resource2=IDR_MAINFRAME
Class4=CMIDIKeyboard
Resource3=IDD_ABOUTBOX
Resource4=IDD_MIDI_DEVS_DLG
Class5=CMIDIDevsDlg
Resource5=IDR_MAIN_MENU

[CLS:CDemoApp]
Type=0
HeaderFile=MIDIDevDemo v2.h
ImplementationFile=MIDIDevDemo v2.cpp
Filter=N
LastObject=CDemoApp

[CLS:CDemoDlg]
Type=0
HeaderFile=DemoDlg.h
ImplementationFile=DemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_HELP_ABOUT

[CLS:CAboutDlg]
Type=0
HeaderFile=DemoDlg.h
ImplementationFile=DemoDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_DEMO_DIALOG]
Type=1
Class=CDemoDlg
ControlCount=12
Control1=IDC_MIDI_KEYS,PianoCtrl,1342242816
Control2=IDC_GM_LIST,combobox,1344274435
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_COMMAND,static,1342308352
Control10=IDC_CHANNEL,static,1342308352
Control11=IDC_DATA2,static,1342308352
Control12=IDC_DATA1,static,1342308352

[CLS:CMIDIKeyboard]
Type=0
HeaderFile=MIDIKeyboard.h
ImplementationFile=MIDIKeyboard.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CMIDIKeyboard

[MNU:IDR_MAIN_MENU]
Type=1
Class=?
Command1=ID_FILE_EXIT
Command2=ID_PREF_NOTE_COLOR
Command3=ID_PREF_MIDIDEVICES
Command4=ID_HELP_ABOUT
CommandCount=4

[DLG:IDD_MIDI_DEVS_DLG]
Type=1
Class=CMIDIDevsDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_MIDI_OUT_DEVS,combobox,1344339971
Control5=IDC_MIDI_IN_DEVS,combobox,1344339971
Control6=IDC_STATIC,static,1342308352

[CLS:CMIDIDevsDlg]
Type=0
HeaderFile=MIDIDevsDlg.h
ImplementationFile=MIDIDevsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMIDIDevsDlg
VirtualFilter=dWC

