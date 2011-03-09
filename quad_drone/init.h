// init.h
//
//============================================================================//
//         

// I2C Definitions
    #define I2C_SEND false
    #define I2C_RECV true

// Compass Definitions
    #define COMP_ADDRESS 0x1e         // Compass Address         00011110b  
    #define COMP_READ_ADDRESS 0x3d    // Compass Read Address    00111101b
    #define COMP_WRITE_ADDRESS 0x3c   // Compass Write Address   00111100b 
    #define COMP_MODE_REG 0x02        // Compass Mode Register   00000010b 

// Gyro Definitions
    #define GYRO_ADDRESS       0x69   // Gyroscope Address        
    #define L3G4_CTRL_REG1     0x20   // Control Register 1
    #define ENABLE_GYRO        0x0f   // Enable PD, Zen, Yen, Xen

// Timer Init
// --------------------------
// extern unsigned int gTimeMS;
// --------------------------

//============================================================================//
// Initalize ADC
void InitADC();

// Initalize PWM
void InitPWM();

// Initalize I2C
void InitI2C();

// Initalize UART
void InitUART();
         
// Initalize Timer
void InitTIMER();
//============================================================================//
