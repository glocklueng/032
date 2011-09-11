/*
	JukiStepper.h - Juki Stepper library for Arduino

	This library interfaces with the Juki Stepper Motor Driver and other standard stepper controllers
	that use the 2 wire Step/Direction interface.  Based on RepRap Stepper by  Zach Smith that was Loosely based 
	on the Stepper library by Tom Igoe & others: http://www.arduino.cc/en/Reference/Stepper
		
	More information on the stepper driver circuit here: http://make.rrrf.org/smd

	Memory Usage Estimate: 13 bytes

	History:
	
	* (0.1) Forked library by Zach Smith.
	* (0.2) Optimizations to reduce code overhead by Zach Smith
	* (0.3) Added delays for optocoupled driver boards as well as variables to record enable/direction status.
	* (0.4) Rewrote and refactored all code.  Fixed major interrupt bug by Zach Smith.
	* (0.5) Adapted for Juki 360 Pick and Place by charliex [Null Space Labs]

	License: GPL v2.0
*/

// ensure this library description is only included once
#ifndef JukiStepper_h
#define JukiStepper_h

#ifndef _AVR_IO_H_
	#include	<avr/io.h>
#endif

#ifndef WProgram_h
	#include	"WProgram.h"
#endif
#ifndef HardwareSerial_h
	#include	"HardwareSerial.h"
#endif


#define RS_FORWARD ( 1 )
#define RS_REVERSE ( 0 )

// library interface description
class JukiStepper {
  public:
    // constructors:
	JukiStepper(long number_of_steps, 
						 long delay, 
						 byte step_minus , byte step_plus);

    // various setters methods
	void setRPM(long rpm);
    void setSpeed(long speed);
	void setDirection(bool direction);
	void setSteps(long steps);
	
	long getMicros();
	
    //various methods dealing with stepping.
	void pulse();
	void enable();
	void disable();
	
	//various internal variables: READ ONLY!  Do not set these directly.
	long rpm;					// Speed in RPMs
	long step_delay;  			// delay between steps, in processor ticks, based on speed
    long number_of_steps;		// total number of steps this motor can take
	bool enabled;				//are we enabled?
	bool direction;				//what is our direction?
	long delay;					// delay after HIGH LOW

	// motor pin numbers:
    byte step_plus_pin;				//the step signal pin.
    byte step_minus_pin;			//the step signal pin.

  private:


    byte direction_pin;			//direction.
    byte enable_pin;			//the enable pin.
};

#endif
