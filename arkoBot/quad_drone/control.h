// control.h
//
//

//============================================================================//
void control(float *imu, float *cmd_angles, float *data_telemetry);
//============================================================================//
void motorSpinup();
//============================================================================//
void PIDTune(char cmd);
//============================================================================//