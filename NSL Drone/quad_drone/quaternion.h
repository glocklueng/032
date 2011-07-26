// quaternion.h
//
//
//============================================================================//
extern void eulerToQuat(fm_fixed *q, fm_fixed *euler_angle);
//============================================================================//
extern void quatToEuler(fm_fixed *q, fm_fixed *euler_angle);
//============================================================================//
extern void eulerFloatToFixed(float *euler_float, fm_fixed *euler_fixed);
//============================================================================//
extern void eulerFixedToFloat(float *euler_float, fm_fixed *euler_fixed);
//============================================================================//
extern void angleFloatToFixed(float *angle_float, fm_fixed *angle_fixed);
//============================================================================//
extern void angleFixedToFloat(float *angle_float, fm_fixed *angle_fixed);
//============================================================================//