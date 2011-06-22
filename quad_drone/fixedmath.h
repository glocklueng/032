#ifndef FIXEDMATH_H        
#define FIXEDMATH_H    

typedef long long       int64;
typedef signed long	fm_fixed;                // 16.16 


/* These prototypes exist for documentation only. */

/* Function: itofix
 */
fm_fixed itofix(int x);

/* Function: fixtoi
 */
int fixtoi(fm_fixed x);

/* Function: fixfloor
 */
int fixfloor(fm_fixed x);

/* Function: ftofix
 */
fm_fixed ftofix(double x);

/* Function: fixtof
 */
double fixtof(fm_fixed x);

/* Function: fixadd
 */
fm_fixed fixadd(fm_fixed x, fm_fixed y);

/* Function: fixsub
 */
fm_fixed fixsub(fm_fixed x, fm_fixed y);

/* Function: ffixmul
 */
fm_fixed fixmul(fm_fixed x, fm_fixed y);

/* Function: fixdiv
 */
fm_fixed fixdiv(fm_fixed x, fm_fixed y);

/* Function: fixcos
 */
fm_fixed fixcos(fm_fixed x);

/* Function: fixsin
 */
fm_fixed fixsin(fm_fixed x);

/* Function: fixtan
 */
fm_fixed fixtan(fm_fixed x);

/* Function: fixacos
 */
fm_fixed fixacos(fm_fixed x);

/* Function: fixasin
 */
fm_fixed fixasin(fm_fixed x);


/**
 *  Fixed point inverse tangent. Does a binary search on the tan table.
 */
fm_fixed fixatan(fm_fixed x);

/** 
 *  Like the libc atan2, but for fixed point numbers.
 */
fm_fixed fixatan2(fm_fixed y, fm_fixed x);

#endif