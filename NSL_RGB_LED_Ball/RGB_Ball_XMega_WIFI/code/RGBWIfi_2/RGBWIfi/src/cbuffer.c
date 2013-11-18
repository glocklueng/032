/*
 * cbuffer.c
 *
 * Created: 11/12/2013 11:57:16 AM
 *  Author: charlie
 */
#define BUFSIZE     ( 256 )

static unsigned char buf[BUFSIZE];
static unsigned char *pIn, *pOut, *pEnd;
static unsigned char full;

// init
void circular_buffer_init ( void )
{
	pIn = pOut = buf;       // init to any slot in buffer
	pEnd = &buf[BUFSIZE];   // past last valid slot in buffer
	full = 0;               // buffer is empty
}

// add char 'c' to buffer
int circular_buffer_put ( unsigned char c )
{
	if ( pIn == pOut  &&  full )
	{ return 0; }           // buffer overrun

	*pIn++ = c;             // insert c into buffer

	if ( pIn >= pEnd )      // end of circular buffer?
	{ pIn = buf; }          // wrap around

	if ( pIn == pOut )      // did we run into the output ptr?
	{ full = 1; }           // can't add any more data into buffer

	return 1;               // all OK
}

int  circular_buffer_puts( const unsigned char * str)
{
	while(*str !=0 ) {

		if( 0 == circular_buffer_put( *str) ) 
			return 0;
			
		str++;
	}
	
	return 1;
}
// get a char from circular buffer
int circular_buffer_get (  unsigned char * const pc )
{
	if ( pIn == pOut  &&  !full )
	{ return 0; }           // Empty buffer

	*pc = *pOut++;              // pick up next char to be returned
     
	// end of circular buffer? 
	if ( pOut >= pEnd ) {
		pOut = buf; 
	}         // wrap around

	full = 0;               // there is at least 1 slot
	return 1;               // *pc has the data to be returned
}

int circular_empty(void) 
{
	if( pIn == pOut  ) {
		return 1;
	}
	
	return 0;
}
