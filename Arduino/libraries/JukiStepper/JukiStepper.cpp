
#include "JukiStepper.h"

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
JukiStepper::JukiStepper(long number_of_steps, 
						 long delay, 
						 byte step_minus , byte step_plus)
{
	//init our variables.
	this->setSpeed(0);

	//get our parameters
	this->number_of_steps = number_of_steps;
	
	this->step_minus_pin = step_minus;
	this->step_plus_pin  = step_plus;
	
	// setup the pins on the microcontroller:
	pinMode(this->step_minus_pin, OUTPUT);
	pinMode(this->step_plus_pin, OUTPUT);

	this->enable();
	this->setDirection(RS_FORWARD);

	this->delay = delay;
}

/*
  Sets the speed in ticks per step
*/
void JukiStepper::setSpeed(long speed)
{
	step_delay = speed;
	
	if (step_delay > 0)
		rpm = 960000000UL / (step_delay * number_of_steps);
	else
		rpm = 0;
}

/*
  Sets the speed in revs per minute
*/
void JukiStepper::setRPM(long new_rpm)
{
	if (new_rpm == 0)
	{
		step_delay = 0;
		rpm = 0;
	}
	else
	{
		rpm = new_rpm;
		
		//lets use the highest precision possible... processor ticks.
		// 16MHZ = 16,000,000 ticks/sec * 60 seconds in a minute = 960,000,000 ticks / minute
		// take the total # of ticks / steps per rev / number of revolutions per minute = ticks per step
		step_delay = (960000000UL / number_of_steps) / rpm;
	}
}

void JukiStepper::setSteps(long steps)
{
	number_of_steps = steps;
	
	//recalculate our speed.
	this->setRPM(this->rpm);
}

void JukiStepper::setDirection(byte direction)
{
	this->direction = direction; //save our direction.

	disable();
}

long JukiStepper::getMicros()
{
	return step_delay / 16;
}

void JukiStepper::enable()
{
	if (this->step_minus_pin != 255)
	{
		digitalWrite(step_minus_pin, HIGH);
		digitalWrite(step_plus_pin, HIGH);
		delayMicroseconds(10); //make sure it stabilizes
	}

	enabled = true;
}

void JukiStepper::disable()
{
	if (this->enable_pin != 255)
	{
		// doesn't really have a disable, so just leave high
		digitalWrite(step_minus_pin, HIGH);
		digitalWrite(step_plus_pin, HIGH);

		delayMicroseconds(10); //make sure it stabilizes
	}

	enabled = false;
}

//this sends a pulse to the stepper controller.
void JukiStepper::pulse()
{
	if ( this->direction == RS_REVERSE ) {
		digitalWrite(this->step_minus_pin, LOW);
		delayMicroseconds( 1 ); //make sure it stabilizes... for opto isolated stepper drivers.
		digitalWrite(this->step_minus_pin, HIGH);
		delayMicroseconds(  delay ); //make sure it stabilizes... for opto isolated stepper drivers.
	} else {
		digitalWrite(this->step_plus_pin, LOW);
		delayMicroseconds( 1 ); //make sure it stabilizes... for opto isolated stepper drivers.
		digitalWrite(this->step_plus_pin, HIGH);
		delayMicroseconds(  delay ); //make sure it stabilizes... for opto isolated stepper drivers.
	}

}
