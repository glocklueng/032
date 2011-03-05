// imu.h
//
//

// I2C Definitions
    #define I2C_SEND false
    #define I2C_RECV true

// Gyro Definitions
                                      // CHECK THIS ADDRESS LSB in the new board revision - SDO pin will be held high..
    #define GYRO_ADDRESS 0x69         // Gyroscope Address         01101001b  
    #define GYRO_MULT_REGISTER 0x80   // Gyroscope Multi Register  10000000b   
  
    #define L3G4_WHO_AM_I           0x0F
    #define L3G4_CTRL_REG1          0x20
    #define L3G4_CTRL_REG2          0x21
    #define L3G4_CTRL_REG3          0x22
    #define L3G4_CTRL_REG4          0x23     // Default to 250dps
    #define L3G4_CTRL_REG5          0x24
    #define L3G4_REFERENCE          0x25
    #define L3G4_OUT_TEMP           0x26
    #define L3G4_STATUS_REG         0x27
    #define L3G4_OUT_X_L            0x28
    #define L3G4_OUT_X_H            0x29
    #define L3G4_OUT_Y_L            0x2A
    #define L3G4_OUT_Y_H            0x2B
    #define L3G4_OUT_Z_L            0x2C
    #define L3G4_OUT_Z_H            0x2D
    #define L3G4_FIFO_CTRL_REG      0x2E
    #define L3G4_FIFO_SRC_REG       0x2F
    #define L3G4_INT1_CFG           0x30
    #define L3G4_INT1_SRC           0x31
    #define L3G4_INT1_THS_XH        0x32
    #define L3G4_INT1_THS_XL        0x33
    #define L3G4_INT1_THS_YH        0x34
    #define L3G4_INT1_THS_YL        0x35
    #define L3G4_INT1_THS_ZH        0x36
    #define L3G4_INT1_THS_ZL        0x37
    #define L3G4_INT1_DURATION      0x38

    #define CTRL_REG_4_DEFAULT      0x00   // Default Settings (250dps)
    #define CTRL_REG_4_STPLUS       0x02   // Self Test Positive (+130dps)
    #define CTRL_REG_4_STMINUS      0x06   // Self Test Negative (-130dps)

// Compass Definitions                             
  
    #define COMP_ADDRESS 0x1e         // Compass Address         00011110b  
    #define COMP_READ_ADDRESS 0x3d    // Compass Read Address    00111101b
    #define COMP_WRITE_ADDRESS 0x3c   // Compass Write Address   00111100b 
    
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


//============================================================================//
void readIMU(float *imu);

void readAccel(unsigned long *temp, unsigned long *x_acc,
               unsigned long *y_acc, unsigned long *z_acc);

void readGyro(signed long *x_gyro, signed long *y_gyro,
              signed long *z_gyro);

void readCompass(signed long *x_axis, signed long *y_axis,
                 signed long *z_axis);

void imuStartup();

void sensorsSelfTest();
//============================================================================//
