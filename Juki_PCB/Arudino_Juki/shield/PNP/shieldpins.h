#ifndef _SHIELDPINS_H_
#define _SHIELDPINS_H_ (1)

// Pin mappings
#if 1

// new board

// xy lIMITS these should be now on pins capable of hardware interrupts in arduino
#define XL1 ( 18 )
#define XL2 ( 19 ) 
#define YL2 ( 20 ) 
#define YL1 ( 21 ) 

#define XL3 ( 24 ) 
#define YL3 ( 29 ) 
#define XL4 ( 25 ) 
#define YL4 ( 40 ) 

// x/y home
#define XHM     ( 26 ) 
#define YHM      ( 41 ) 

// these two seem mixed up
#define HEADDN     ( 37 )
#define DEG90      ( 36 )


#define TACSENSE   ( 38 )
#define PCBDETECTOR ( 39 )
#define PCBIN ( 5 )
#define PCBOUT  ( 6 )
#define PINUP ( 7 )
#define READYIN ( 8 )
#define DISPENSEREND ( 9 )
#define MORG ( 11 )
#define MCENT ( 12 )

#define CWSW ( 13 )
#define CCWSW ( 35 )

#define XPLUS  ( 4 )
#define XMINUS ( 3 )
#define YPLUS  ( 2  )
#define YMINUS ( 14 )


#define FAST ( 15 )
#define TEACH ( 16 )
//switch on touch pad
#define T_VAC ( 17 )
// switch on touch pad
#define T_HEAD ( 33 )

#define SPAREA ( 32 )
#define SPAREB ( 34 )
#define SPAREC ( 10 )

#define SPARE7 ( 53 )
#define SPARE8 ( A12 )
#define SPOT ( 52 )
#define VCHANGE ( 51 )
#define DISPENSERSTART ( 50 )
#define CVYM ( 49 )
#define STOPPER ( 48 )
#define LOCATOR ( 47 )
#define SUPPORTER ( 46 )
#define VAC ( 45 ) 
#define HEAD ( 44 ) 
#define ROT (43 ) 
#define CENTERING (42  )
#define TAPEKNOCK ( A15 )
#define SPARE6 ( A14 )

#define ACENT ( A13 )
#define SPARE1 ( A7 )
#define SPARE2 ( A8 )
#define SPARE3 ( A9 )
#define SPARE4 ( A10 )
#define SPARE5 ( A11 )
#define READYOUT ( A6 )
#define CW_CCW ( A5 )
#define PULSE ( A4 )

// motors
#define YCW ( A2 )
#define YCCW ( A3 )
#define XCW ( A0 )
#define XCCW ( A1 )

#else

// oldbboard

// xy lIMITS
#define XL1 ( 23 )
#define YL1 ( 27 ) 
#define XL2 ( 22 ) 
#define YL2 ( 28 ) 
#define XL3 ( 24 ) 
#define YL3 ( 29 ) 
#define XL4 ( 25 ) 
#define YL4 ( 30 ) 

// x/y home
#define XHM         ( 26 ) 
#define YHM         ( 41 ) 

#define HEADDN      ( 1 )
#define DEG90       ( 2 )
#define TACSENSE    ( 3 )
#define PCBDETECTOR ( )
#define PCBIN ( )
#define PCBOUT  ( )
#define PINUP ( )
#define READYIN ( )
#define DISPENSEREND ( )
#define MORG ( 11 )
#define MCENT ( )

#define CWSW ( )
#define CCWSW ( )

#define XPLUS  ( 40 )
#define XMINUS ( 39 )
#define YPLUS  ( 0  )
#define YMINUS ( 11 )


#define FAST ( 15 )
#define TEACH ( 16 )
#define T_VAC ( 17 )
// switch on touch pad
#define T_HEAD ( 18 )

#define SPAREA ( )
#define SPAREB ( )
#define SPAREC ( )

#define SPARE7 ( )
#define SPARE8 ( )
#define SPOT ( 52 )
#define VCHANGE( )
#define DISPENSERSTART ( )
#define CVYM( )
#define STOPPER ( )
#define LOCATOR ( )
#define SUPPORTER ( )
#define VAC ( 45 ) 
#define HEAD ( 44 ) 
#define ROT (43 ) 
#define CENTERING (42  )
#define TAPEKNOCK ( A14 )
#define SPARE6 ( A13 )

#define ACENT ( )
#define SPARE1 ( A7 )
#define SPARE2 ( A8 )
#define SPARE3 ( A9 )
#define SPARE4 ( A10)
#define SPARE5 ( A11)
#define READYOUT ( )
#define CW_CCW ( )
#define PULSE ( )

// motors
#define YCW ( A2 )
#define YCCW ( A3 )
#define XCW ( A0 )
#define XCCW ( A1 )


#endif

#endif

