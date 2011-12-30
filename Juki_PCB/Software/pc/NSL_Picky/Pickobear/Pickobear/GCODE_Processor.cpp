#include "stdafx.h"


void GCODE_Processor::ProcessLoop( LPVOID userdata ) 
{
	// won't be created until GUI is started

	while( AfxGetApp()->m_pMainWnd == NULL) 
		Sleep( 100 );

	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	// grab a duplicate handle for the thread so we can suspend resume it when needed.
	if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &pDlg->GCODE_CPU_Thread,
		DUPLICATE_SAME_ACCESS, FALSE, DUPLICATE_SAME_ACCESS)) { 
			return; 
	} 
	while( 1 ) {

		// pointer to gcode statement callback
		gcode_callback func_ptr;

		// Machine Busy?

		// Yield thread
		while( QueueEmpty() ) {

			// yield, this is the minimum time it'll react, Sleep is only guaranteed to be not less than 10ms
			Sleep( 10 ) ;

			// Main app wants us to quit
			if( pDlg->m_Quit ) {
				return;
			}

			// We could process limits/xy update in here, since machine won't be doing anything here

		}

		// copy into buffer, std::vector front returns the oldest entry.
		m_GCODECMDBuffer = command_buffer.front().gcode;
		_RPT1(_CRT_WARN,"%s\n",m_GCODECMDBuffer.c_str());

		// shortname for function pointer
		func_ptr = command_buffer.front().func_ptr_completed;

		// correctly terminate the command
		if( m_GCODECMDBuffer.find("\n") == std::string::npos ) {
			m_GCODECMDBuffer.append("\n");
		}

		do {
			// Send to serial port, and write to console
			pDlg->InternalWriteSerial( m_GCODECMDBuffer.c_str(), false) ;

			// check for main app asking to quit
			if( pDlg->m_Quit ) {
				return;
			}

			// Command ACK ( ok ) 

			// Read from serial
			char ch;

			std::string ackString;

			ackString.erase();

			do{
				DWORD lengthRead = 0;
				
				pDlg->m_Serial.Read(&ch,sizeof(ch),&lengthRead,0,4000 );

				if( lengthRead  ) {
					ackString += ch;
				}

			} while(ch!='\n');

			// got ack
			if( ackString == "ok" ) {
				// continue
				break;
			}

			// give it a chance to catch up
			Sleep( 100 );

		}while(1);

	}
}

GCODE_Processor::~GCODE_Processor()
{
}

