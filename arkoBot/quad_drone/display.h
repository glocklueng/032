//  display.h
//                
//============================================================================//
void InitDisplay();
//============================================================================//
void UpdateDisplay();
//============================================================================//
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
//============================================================================//
void UARTIntHandler(void);
//============================================================================//