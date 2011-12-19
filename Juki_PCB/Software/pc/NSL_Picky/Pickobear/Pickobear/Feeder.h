/*
	XXXXXXX  XXXXXXX  XXXXXXX  XXXXXX   XXXXXXX  XXXXXX
	XX   X   XX   X   XX   X   XX  XX   XX   X   XX  XX
	XX X     XX X     XX X     XX  XX   XX X     XX  XX
	XXXX     XXXX     XXXX     XX  XX   XXXX     XXXXX
	XX X     XX X     XX X     XX  XX   XX X     XX XX
	XX       XX   X   XX   X   XX  XX   XX   X   XX  XX
	XXXX     XXXXXXX  XXXXXXX  XXXXXX   XXXXXXX  XXX  XX

*/

#ifndef _FEEDER_H_
#define _FEEDER_H_ (1)

#include "PickobearDlg.h"

// Feeder Class
// Handles all types of feeder data
class Feeder {


public:
	
	enum ErorrCodes {
		FEEDER_NO_ERROR,
		FEEDER_OUT_OF_PARTS,
		FEEDER_TX_OUT_OF_BOUNDS,
		FEEDER_TY_OUT_OF_BOUNDS,
		FEEDER_BX_OUT_OF_BOUNDS,
		FEEDER_BY_OUT_OF_BOUNDS,
	} ;


	Feeder() {};
	
	
	CListCtrl_FeederList::FeederDatabase  *entry;

	// constructor from feeder
	Feeder( CListCtrl_FeederList::FeederDatabase &e ) {

		// store pointer to entry.
		entry = &e;

		SetPosition( e.x , e.y );
		SetRightBottomXY (e.lx, e.ly);
		SetRotation( e.rot );
		SetXCount( e.countx );
		SetYCount( e.county );
		SetCount( e.countx * e.county );
		SetLabel( CString(e.label) );
		SetPart( e.componentIndex );

	}

	~Feeder(){
	}

	CString ErrorMessage( void ) 
	{
	
		switch ( last_error ) {

			case FEEDER_NO_ERROR:
				return L"No Error";
			case FEEDER_OUT_OF_PARTS:
				return L"No Parts Left";
			case FEEDER_TX_OUT_OF_BOUNDS:
				return L"Top X is out of bounds";
			case FEEDER_TY_OUT_OF_BOUNDS:
				return L"Top Y is out of bounds";
			case FEEDER_BX_OUT_OF_BOUNDS:
				return L"Bottom X is out of bounds";
			case FEEDER_BY_OUT_OF_BOUNDS:
				return L"Bottom Y is out of bounds";
			default:
				return L"Unknown Error";

		};
		
		return L"No Error";

	}


	// Set location of Feeder
	bool SetPosition( long x, long y )
	{
		// check bounds

		// store
		feeder_position_x = x;
		feeder_position_y = y;

		return true;
	}

	bool GetPosition( long &x, long &y )
	{
		x = feeder_position_x;
		y = feeder_position_y;

		return true;
	}

	// poorly named
	bool SetRightBottomXY( long x,long y )
	{
		// check Bounds

		bottom_position_x = x;
		bottom_position_y = y;

		return true;
	}

	bool GetRightBottomXY( long &x,long &y )
	{
		x = bottom_position_x ;
		y = bottom_position_y ;

		return true;
	}


	// Rotation of feeder ( front/left right angle ) after rotating from here, part should be in neutral orientation
	bool SetRotation( double rotation_in_degrees ) 
	{
		degrees = rotation_in_degrees ;

		return true;
	}

	bool GetRotation( double &rotation_in_degrees  ) 
	{
		rotation_in_degrees = degrees ;

		return true;
	}

	// return the angle in pulses
	bool GetRotationInPulses( unsigned long &pulses) 
	{

		pulses = ( unsigned long ) (   ( 1000.0 / 360.0  ) * degrees  );
		
		return true;

	}

	// number of parts in feeder
	bool SetCount( unsigned int count )
	{
		number_of_parts = count ;
		return true;
	}

	bool GetCount( unsigned int &count )
	{
		count = number_of_parts;
		return true;
	}

	// Unique name of feeder
	bool SetLabel( CString name ) 
	{
		label = name;

		return true;
	}

	bool GetLabel( CString &name ) 
	{
		name = label;

		return true;
	}

	// Does feeder match this name ?
	bool MatchLabel( CString match_name )
	{
		// no label defined
		if( label.GetLength() == 0 ) {
			return false;
		}

		// Nothing passed in
		if( match_name.GetLength() == 0 ) {
			return false;
		}

		if( match_name == label ) {
			return true;
		}

		return false;
	}

	// Set the tray counts x and y
	bool SetXCount( unsigned long x_count ) 
	{
		number_parts_x = x_count;

		return true;
	}

	bool SetYCount( unsigned long y_count ) 
	{
		number_parts_y = y_count;

		return true;
	}

	bool GetXCount( unsigned long &x_count ) 
	{
		x_count = number_parts_x ;

		return true;
	}

	bool GetYCount( unsigned long &y_count ) 
	{
		y_count = number_parts_y ;

		return true;
	}


	bool SetPart( unsigned long part_index ) 
	{
		current_part = part_index;
		return true;
	}

	bool GetPart( unsigned long &part_index ) 
	{
		part_index = current_part;

		return true;
	}

	// Advance index to next part
	bool AdvancePart( void )
	{
		_RPT2(_CRT_WARN,"AdvancePart: %d %d\n",current_part, number_of_parts);

		// if there are any parts left available
		if (current_part < number_of_parts ) {
		
			current_part ++ ;

			entry->componentIndex = current_part;
			

			return true;
		}
		
		last_error = FEEDER_OUT_OF_PARTS ;

		return false;
	}

	// how many parts are left in the feeder (add infinite ? )
	bool PartsLeft( long &remaining )
	{
		remaining = ( number_of_parts - current_part );
		_RPT1(_CRT_WARN,"PartsLeft: remaining %d\n",remaining);

		return true;
	}

	// Calculates feeder position for next part to pickup, does not advance part!
	bool GetNextPartPosition( unsigned long &ret_x, unsigned long &ret_y) 
	{
		long remaining;
		bool ret;

		// Check feeder is correctly defined ?

		// First check to see if parts are left.
		ret = PartsLeft( remaining );
		if( ret == false, remaining == 0  ){
	
			last_error = FEEDER_OUT_OF_PARTS ;
			
			return false;
		}

		// Check if its not a tray feeder, if so just return position
		if( !( bottom_position_y || bottom_position_x ) ) {

			ret_x = feeder_position_x;
			ret_y = feeder_position_y;
			
			return true;
		}

		// by here, there are parts left and it is a tray style feeder

		long x,y;

		// starting location
		x =  feeder_position_x;
		y =  feeder_position_y;

		// start position
		_RPT2(_CRT_WARN," x = %d,  y = %d\n",x,y);

		long dx,dy;

		// calculate difference
		dx = abs( bottom_position_x - feeder_position_x );
		dy = abs( feeder_position_y - bottom_position_y );

		dx /= number_parts_x;
		dy /= number_parts_y;
		
		_RPT2(_CRT_WARN,"dx = %d, dy = %d\n",dx,dy);

		int ix,iy;

		// take the current part index, calculate remainder against parts in ---> x 
		ix = current_part % ( number_parts_x  + 1 ) ;
		iy = current_part / ( number_parts_x  + 1 ) ;

		_RPT2(_CRT_WARN,"ix = %d, iy = %d\n",ix,iy);

		/// calculate new position
		x +=  (dx * ix );
		y -= ( dy * iy );

		_RPT2(_CRT_WARN," x = %d,  y = %d\n",x,y);

		ret_x = x;
		ret_y = y;
		
		return true;
	}


private:

		// position of feeder in pnp units
		long feeder_position_x,feeder_position_y;

		// position of tray feeder in pnp units, right, bototom most part
		long bottom_position_x,bottom_position_y;

		// degrees of rotation for part to be in neutral position
		double degrees;

		// unique name of feeder
		CString label;

		// total number of parts the tray can hold
		unsigned long number_of_parts;

		// which part are we at in the feeder/tray
		unsigned long current_part;

		// number of parts in x/y direction for tray feeders, only set for tray feeders ( new sub class ? )
		unsigned long number_parts_x;
		unsigned long number_parts_y;

		// which tool to use
		unsigned int tool ;

		ErorrCodes last_error;
};

#endif
