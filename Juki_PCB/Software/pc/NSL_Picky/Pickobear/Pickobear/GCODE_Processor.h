#ifndef _GCODE_PROCESSOR_
#define _GCODE_PROCESSOR_		( 1 )

#include <windows.h>
#include <string>

class GCODE_Processor {

	 friend class CPickobearDlg;
			
	// callback after GCODE has run
	typedef bool (*gcode_callback)(void*pThis,void*userdata);

	GCODE_Processor() {
			

		Multi::Thread::Create<void>(&GCODE_Processor::ProcessLoop, this, NULL); 
	}

	~GCODE_Processor();

public:

	bool Add( std::string gcode, std::string error_message ,gcode_callback func_ptr_completed )
	{
		gcode_command command;
	
		// Create structure
		command.gcode = gcode;
		command.error_message = error_message;
		command.func_ptr_completed = func_ptr_completed;

		// add to list
		command_buffer.push_back( command );

		return true;
	}
	
	// is the buffer empty?
	bool QueueEmpty(void) {
		return command_buffer.empty();
	}

private:

	// Current command
	std::string m_GCODECMDBuffer;

	// Because this calls back to the dialog class, it can't be here.
	void ProcessLoop( LPVOID userdata );

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


};

#endif 
//_GCODE_PROCESSOR_
