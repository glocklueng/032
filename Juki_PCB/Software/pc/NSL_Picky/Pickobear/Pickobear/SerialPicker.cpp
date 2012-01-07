// SerialPicker.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "SerialPicker.h"
#include "afxdialogex.h"


// CSerialPicker dialog

IMPLEMENT_DYNAMIC(CSerialPicker, CDialogEx)

CSerialPicker::CSerialPicker(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialPicker::IDD, pParent)
{
	 //Initialize COM (Required by CEnumerateSerial::UsingWMI)
  CoInitialize(NULL);

  //Initialize COM security (Required by CEnumerateSerial::UsingWMI)
  HRESULT hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
  if (FAILED(hr))
  {
    _tprintf(_T("Failed to initialize COM security, Error:%x\n"), hr);
    CoUninitialize();
    return ;  
  }
  
}

BOOL CSerialPicker::OnInitDialog()
{	

	iIndex = 0;

	CDialog::OnInitDialog();

	m_SerialList.ResetContent();


#if defined CENUMERATESERIAL_USE_STL
  std::vector<UINT> ports;
#if defined _UNICODE
  std::vector<std::wstring> friendlyNames;
  std::vector<std::wstring> sPorts;
#else
  std::vector<std::string> friendlyNames;
  std::vector<std::string> sPorts;
#endif  
  size_t i;
#elif defined _AFX
  CUIntArray ports;
  CStringArray friendlyNames;
  CStringArray sPorts;
  INT_PTR i;
#else
  CSimpleArray<UINT> ports;
  CSimpleArray<CString> friendlyNames;
  CSimpleArray<CString> sPorts;
  int i;
#endif

  _tprintf(_T("CreateFile method reports\n"));
  DWORD dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingCreateFile(ports))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<ports.size(); i++)
      _tprintf(_T("COM%d\n"), ports[i]);
  #else
    for (i=0; i<ports.GetSize(); i++)
      _tprintf(_T("COM%d\n"), ports[i]);

  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingCreateFile failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingCreateFile, %d ms\n"), GetTickCount() - dwStartTick);

  _tprintf(_T("QueryDosDevice method reports\n"));
  dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingQueryDosDevice(ports))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<ports.size(); i++)
      _tprintf(_T("COM%d\n"), ports[i]);
  #else
    for (i=0; i<ports.GetSize(); i++)
      _tprintf(_T("COM%d\n"), ports[i]);
  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingQueryDosDevice failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingQueryDosDevice, %d ms\n"), GetTickCount() - dwStartTick);

  _tprintf(_T("GetDefaultCommConfig method reports\n"));
  dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingGetDefaultCommConfig(ports))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<ports.size(); i++)
      _tprintf(_T("COM%d\n"), ports[i]);
  #else
	  for (i=0; i<ports.GetSize(); i++){
      _tprintf(_T("COM%d\n"), ports[i]);
	  _RPT1(_CRT_WARN,"COM%d\n", ports[i]);
	  CString port;
	  port.Format(L"COM%d",ports[i]);
	  m_SerialList.AddString( port );
  }
  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingGetDefaultCommConfig failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingGetDefaultCommConfig, %d ms\n"), GetTickCount() - dwStartTick);

  _tprintf(_T("Device Manager (SetupAPI - GUID_DEVINTERFACE_COMPORT) reports\n"));
  dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingSetupAPI1(ports, friendlyNames))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<ports.size(); i++)
      _tprintf(_T("COM%d <%s>\n"), ports[i], friendlyNames[i].c_str());
  #else
    for (i=0; i<ports.GetSize(); i++)
      _tprintf(_T("COM%d <%s>\n"), ports[i], friendlyNames[i].operator LPCTSTR());
  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingSetupAPI1 failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingSetupAPI1, %d ms\n"), GetTickCount() - dwStartTick);

  _tprintf(_T("Device Manager (SetupAPI - Ports Device information set) reports\n"));
  dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingSetupAPI2(ports, friendlyNames))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<ports.size(); i++)
      _tprintf(_T("COM%d <%s>\n"), ports[i], friendlyNames[i].c_str());
  #else
    for (i=0; i<ports.GetSize(); i++)
      _tprintf(_T("COM%d <%s>\n"), ports[i], friendlyNames[i].operator LPCTSTR());
  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingSetupAPI2 failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingSetupAPI2, %d ms\n"), GetTickCount() - dwStartTick);

  _tprintf(_T("EnumPorts method reports\n"));
  dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingEnumPorts(ports))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<ports.size(); i++)
      _tprintf(_T("COM%d\n"), ports[i]);
  #else
    for (i=0; i<ports.GetSize(); i++){
      _tprintf(_T("COM%d\n"), ports[i]);
	  _RPT1(_CRT_WARN,"COM%d\n", ports[i]);
	}
  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingEnumPorts failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingEnumPorts, %d ms\n"), GetTickCount() - dwStartTick);

  _tprintf(_T("WMI method reports\n"));
  dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingWMI(ports, friendlyNames))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<ports.size(); i++)
      _tprintf(_T("COM%d <%s>\n"), ports[i], friendlyNames[i].c_str());
  #else
    for (i=0; i<ports.GetSize(); i++)
      _tprintf(_T("COM%d <%s>\n"), ports[i], friendlyNames[i].operator LPCTSTR());
  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingWMI failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingWMI, %d ms\n"), GetTickCount() - dwStartTick);

  _tprintf(_T("ComDB method reports\n"));
  dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingComDB(ports))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<ports.size(); i++)
      _tprintf(_T("COM%d\n"), ports[i]);
  #else
    for (i=0; i<ports.GetSize(); i++)
      _tprintf(_T("COM%d\n"), ports[i]);
  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingComDB failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingComDB, %d ms\n"), GetTickCount() - dwStartTick);

  _tprintf(_T("Registry method reports\n"));
  dwStartTick = GetTickCount();
  if (CEnumerateSerial::UsingRegistry(sPorts))
  {
  #ifdef CENUMERATESERIAL_USE_STL
    for (i=0; i<sPorts.size(); i++)
      _tprintf(_T("%s\n"), sPorts[i].c_str());
  #else
    for (i=0; i<sPorts.GetSize(); i++)
      _tprintf(_T("%s\n"), sPorts[i].operator LPCTSTR());
  #endif
  }
  else
    _tprintf(_T("CEnumerateSerial::UsingRegistry failed, Error:%d\n"), GetLastError());
  _tprintf(_T("Take taken calling CEnumerateSerial::UsingRegistry, %d ms\n"), GetTickCount() - dwStartTick);

 

  m_SerialList.SetCurSel( iIndex );

  return TRUE;
}

CSerialPicker::~CSerialPicker()
{
//Close down COM
  CoUninitialize();
}

void CSerialPicker::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERIAL_LIST, m_SerialList);
}


BEGIN_MESSAGE_MAP(CSerialPicker, CDialogEx)
	ON_CBN_SELCHANGE(IDC_SERIAL_LIST, &CSerialPicker::OnCbnSelchangeStepsize)
END_MESSAGE_MAP()


// CSerialPicker message handlers
void CSerialPicker::OnCbnSelchangeStepsize()
{

	int i = m_SerialList.GetCurSel();

	// GetWindowsText hasn't updated by now
	m_SerialList.GetLBText(i,m_Port);

	CStringA c(m_Port);
	_RPT1(_CRT_WARN,"Selected %s\n",c);

}
