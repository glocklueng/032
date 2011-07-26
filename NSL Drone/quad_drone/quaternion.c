#include "inc/lm3s9b96.h"
#include "math.h"
#include "fixedmath.h"
#include "IQmath/IQmathLib.h"

#include "quaternion.h"


//  Natural Constants
// *************************
volatile static const float pi = 3.141592f;	    // Pi
static const float rad_byte = 40.584510;	    // Convert from Radians to Byte (0...255)
static const float convert_Pi_180 = 0.017453f;	    // Pi/180 - Convert Degrees to Radians
static const float convert_180_Pi = 57.29577f;	    // 180/Pi - Convert Radians to Degrees

//  Quaternion Variables and Constants
// *************************
//  Readings in Euler Angles
//  [0][0] : X Euler Angle - Cosine Phi
//  [1][0] : Y Euler Angle - Cosine Theta
//  [2][0] : Z Euler Angle - Cosine Psi
//  [0][1] : X Euler Angle - Sine Phi
//  [1][1] : Y Euler Angle - Sine Theta
//  [2][1] : Z Euler Angle - Sine Psi 
fm_fixed euler_trig[3][2] = {{0,0},{0,0},{0,0}};
// *************************


//*****************************************************************************
//  Convert Euler to Quaternions
//  
// Euler Angle Matrix
// [0] - eulerAngle - Phi - X
// [1] - eulerAngle - Theta - Y
// [2] - eulerAngle - Psi - Z
//
// Quaternion Matrix
//  [0] : q0
//  [1] : q1
//  [2] : q2
//  [3] : q3
//
void eulerToQuat(fm_fixed *q, fm_fixed *euler_angle)
{   
    euler_angle[0] = fixdiv(euler_angle[0],ftofix(2));
    euler_angle[1] = fixdiv(euler_angle[1],ftofix(2));
    euler_angle[2] = fixdiv(euler_angle[2],ftofix(2));
    
    euler_trig[0][0] = fixcos(euler_angle[0]);
    euler_trig[1][0] = fixcos(euler_angle[1]);
    euler_trig[2][0] = fixcos(euler_angle[2]);

    euler_trig[0][1] = fixsin(euler_angle[0]);
    euler_trig[1][1] = fixsin(euler_angle[1]);
    euler_trig[2][1] = fixsin(euler_angle[2]);
    
    q[0] = fixadd((fixmul(fixmul(euler_trig[0][0],euler_trig[1][0]),euler_trig[2][0])),(fixmul(fixmul(euler_trig[0][1],euler_trig[1][1]),euler_trig[2][1])));
    q[1] = fixsub((fixmul(fixmul(euler_trig[0][1],euler_trig[1][0]),euler_trig[2][0])),(fixmul(fixmul(euler_trig[0][0],euler_trig[1][1]),euler_trig[2][1])));
    q[2] = fixadd((fixmul(fixmul(euler_trig[0][0],euler_trig[1][1]),euler_trig[2][0])),(fixmul(fixmul(euler_trig[0][1],euler_trig[1][0]),euler_trig[2][1])));
    q[3] = fixsub((fixmul(fixmul(euler_trig[0][0],euler_trig[1][0]),euler_trig[2][1])),(fixmul(fixmul(euler_trig[0][1],euler_trig[1][1]),euler_trig[2][0])));

}
//*****************************************************************************


//*****************************************************************************
//  Convert Quaternions to Euler
//  
// Euler Angle Matrix
// [0] - eulerAngle - Phi - X
// [1] - eulerAngle - Theta - Y
// [2] - eulerAngle - Psi - Z
//
// Quaternion Matrix
//  [0] : q0
//  [1] : q1
//  [2] : q2
//  [3] : q3
//
void quatToEuler(fm_fixed *q, fm_fixed *euler_angle)
{
    euler_angle[0] = fixatan2((fixmul(ftofix(2),fixadd(fixmul(q[0],q[1]),fixmul(q[2],q[3])))),fixsub(ftofix(1),(fixmul(ftofix(2),fixadd(fixmul(q[1],q[1]),fixmul(q[2],q[2]))))));
    euler_angle[1] = fixasin(fixmul(ftofix(2),fixsub(fixmul(q[0],q[2]),fixmul(q[3],q[1]))));
    euler_angle[2] = fixatan2((fixmul(ftofix(2),fixadd(fixmul(q[0],q[3]),fixmul(q[1],q[2])))),fixsub(ftofix(1),(fixmul(ftofix(2),fixadd(fixmul(q[2],q[2]),fixmul(q[3],q[3]))))));
}
//*****************************************************************************


//*****************************************************************************
// Convert Euler Angle - Floating Point to Fixed Point  (degrees)
void eulerFloatToFixed(float *euler_float, fm_fixed *euler_fixed)
{
  // Euler Angle Matrix
  // [0] - eulerAngle - Phi - X
  // [1] - eulerAngle - Theta - Y
  // [2] - eulerAngle - Psi - Z
  euler_fixed[0] = ftofix(euler_float[0]*convert_Pi_180*rad_byte);	
  euler_fixed[1] = ftofix(euler_float[1]*convert_Pi_180*rad_byte); 
  euler_fixed[2] = ftofix(euler_float[2]*convert_Pi_180*rad_byte);
}
//*****************************************************************************    
  
   
//*****************************************************************************
// Convert Euler Angle - Fixed Point to Floating Point  (degrees)
void eulerFixedToFloat(float *euler_float, fm_fixed *euler_fixed)
{
  // Euler Angle Matrix
  // [0] - eulerAngle - Phi - X
  // [1] - eulerAngle - Theta - Y
  // [2] - eulerAngle - Psi - Z
  euler_float[0] = (fixtof(euler_fixed[0])/(rad_byte))*convert_180_Pi;	
  euler_float[1] = (fixtof(euler_fixed[1])/(rad_byte))*convert_180_Pi;
  euler_float[2] = (fixtof(euler_fixed[2])/(rad_byte))*convert_180_Pi;
}
//*****************************************************************************    
 


//*****************************************************************************
// Convert Angle - Floating Point to Fixed Point  (degrees)
void angleFloatToFixed(float *angle_float, fm_fixed *angle_fixed)
{
    //float temp_angle_float = *angle_float;  
    *angle_fixed = ftofix((*angle_float)*convert_Pi_180*rad_byte);
}
//***************************************************************************** 



//*****************************************************************************
// Convert Angle - Fixed Point to Floating Point  (degrees)
void angleFixedToFloat(float *angle_float, fm_fixed *angle_fixed)
{
    //fm_fixed temp_angle_fixed = *angle_fixed;
    *angle_float = (fixtof(*angle_fixed)/(rad_byte))*convert_180_Pi;	
}
//*****************************************************************************   