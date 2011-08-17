// init.h
//
//============================================================================//
//         

// I2C Definitions
    #define I2C_SEND false
    #define I2C_RECV true

// Compass Definitions
    #define COMP_CONFIG_A 0x00        // Compass Config Reg A    00000000b  
    #define COMP_CONFIG_B 0x01        // Compass Config Reg B    00000001b
    #define COMP_MODE_REG 0x02        // Compass Mode Register   00000010b 
    #define COMP_DATA_X_MSB 0x03      // Compass Output X MSB    00000011b  
    #define COMP_DATA_X_LSB 0x04      // Compass Output X LSB    00000100b
    #define COMP_DATA_Y_MSB 0x05      // Compass Output Y MSB    00000101b 
    #define COMP_DATA_Y_LSB 0x06      // Compass Output Y LSB    00000110b  
    #define COMP_DATA_Z_MSB 0x07      // Compass Output Z MSB    00000111b
    #define COMP_DATA_Z_LSB 0x08      // Compass Output Z LSB    00001000b 
    #define COMP_STATUS_REG 0x09      // Compass Status Reg      00001001b 
    #define COMP_ID_REG_A 0x0a        // Compass ID Reg A        00001010b  
    #define COMP_ID_REG_B 0x0b        // Compass ID Reg B        00001011b
    #define COMP_ID_REG_C 0x0c        // Compass ID Reg C        00001100b 

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
