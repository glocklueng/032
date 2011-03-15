// xbee.h
//
//
//============================================================================//
extern void 
UARTSend(const unsigned char *pucBuffer, unsigned long ulCount); 
//============================================================================//
extern void 
sendDataTelemetry(float *imu, float dt); 
//============================================================================//
extern void 
sendControlTelemetry(float torque); 
//============================================================================//