/* ========================================
 *
 * Strip Light Controller
 * By Mark Hastings
 *
 * 05/27/2013  v1.0  Mark Hastings   Initial working version
 *
 * ========================================
*/

#if !defined(CY_SLIGHTS_`$INSTANCE_NAME`_H)
#define CY_SLIGHTS_`$INSTANCE_NAME`_H

#include "cytypes.h"
#include "cyfitter.h"

/* Function Prototypes */
void   `$INSTANCE_NAME`_Start(void);
void   `$INSTANCE_NAME`_Stop(void);
void   `$INSTANCE_NAME`_WriteColor(uint32 color);
void   `$INSTANCE_NAME`_DisplayClear(uint32 color);
void   `$INSTANCE_NAME`_MemClear(uint32 color);
void   `$INSTANCE_NAME`_Trigger(uint32 rst);
uint32 `$INSTANCE_NAME`_Ready(void);

void   `$INSTANCE_NAME`_DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, int32 fill, uint32 color);
void   `$INSTANCE_NAME`_DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color);
void   `$INSTANCE_NAME`_DrawCircle (int32 x0, int32 y0, int32 radius, uint32 color);
void   `$INSTANCE_NAME`_Pixel(int32 x, int32 y, uint32 color);
uint32 `$INSTANCE_NAME`_GetPixel(int32 x, int32 y);
uint32 `$INSTANCE_NAME`_ColorInc(uint32 incValue);
void   `$INSTANCE_NAME`_Dim(uint32 dimLevel); 

#define `$INSTANCE_NAME`_DimLevel_0   0
#define `$INSTANCE_NAME`_DimLevel_1   1
#define `$INSTANCE_NAME`_DimLevel_2   2
#define `$INSTANCE_NAME`_DimLevel_3   3
#define `$INSTANCE_NAME`_DimLevel_4   4

#define `$INSTANCE_NAME`_IRQ_Enable() CyIntEnable(`$INSTANCE_NAME`_IRQ__INTC_NUMBER ); 
#define `$INSTANCE_NAME`_IRQ_Disable() CyIntDisable(`$INSTANCE_NAME`_IRQ__INTC_NUMBER );
CY_ISR_PROTO(`$INSTANCE_NAME`_ISR);

/* Register Definitions */
#define `$INSTANCE_NAME`_DATA         (*(reg8 *) `$INSTANCE_NAME`_B_WS2811_dshifter_u0__F0_REG)
#define `$INSTANCE_NAME`_DATA_PTR     ((reg8 *)  `$INSTANCE_NAME`_B_WS2811_dshifter_u0__F0_REG)

#define `$INSTANCE_NAME`_CONTROL      (*(reg8 *) `$INSTANCE_NAME`_B_WS2811_ctrl__CONTROL_REG)
#define `$INSTANCE_NAME`_STATUS       (*(reg8 *) `$INSTANCE_NAME`_B_WS2811_StatusReg__STATUS_REG)

#define `$INSTANCE_NAME`_Period       (*(reg8 *) `$INSTANCE_NAME`_B_WS2811_pwm8_u0__F0_REG)
#define `$INSTANCE_NAME`_Period_PTR   ((reg8 *)  `$INSTANCE_NAME`_B_WS2811_pwm8_u0__F0_REG)

#define `$INSTANCE_NAME`_Compare0     (*(reg8 *) `$INSTANCE_NAME`_B_WS2811_pwm8_u0__D0_REG)
#define `$INSTANCE_NAME`_Compare1     (*(reg8 *) `$INSTANCE_NAME`_B_WS2811_pwm8_u0__D1_REG)

#define `$INSTANCE_NAME`_Period2      (*(reg8 *) `$INSTANCE_NAME`_B_WS2811_pwm8_u0__F1_REG)
#define `$INSTANCE_NAME`_Period2_PTR  ((reg8 *)  `$INSTANCE_NAME`_B_WS2811_pwm8_u0__F1_REG)

#define `$INSTANCE_NAME`_ACTL0_REG    (*(reg8 *) `$INSTANCE_NAME`_B_WS2811_pwm8_u0__DP_AUX_CTL_REG)
#define `$INSTANCE_NAME`_DISABLE_FIFO  0x03


#define `$INSTANCE_NAME`_Channel      (*(reg8 *) `$INSTANCE_NAME`_StringSel_Sync_ctrl_reg__CONTROL_REG)
#define `$INSTANCE_NAME`_Channel_PTR  ((reg8 *)  `$INSTANCE_NAME`_StringSel_Sync_ctrl_reg__CONTROL_REG)


#define MAKE_RGB( r,g,b )_(((DWORD)((BYTE)(r))) | ((DWORD)((BYTE)(g)) << 8) | ((DWORD)((BYTE)(b)) << 16) ))


/* Status Register Constants  */
#define `$INSTANCE_NAME`_FIFO_EMPTY       0x01
#define `$INSTANCE_NAME`_FIFO_NOT_FULL    0x02
#define `$INSTANCE_NAME`_STATUS_ENABLE    0x80
#define `$INSTANCE_NAME`_STATUS_XFER_CMPT 0x40

/* Control Register Constants */
#define `$INSTANCE_NAME`_ENABLE         0x01
#define `$INSTANCE_NAME`_DISABLE        0x00
#define `$INSTANCE_NAME`_RESTART        0x02
#define `$INSTANCE_NAME`_CNTL           0x04
#define `$INSTANCE_NAME`_FIFO_IRQ_EN    0x08
#define `$INSTANCE_NAME`_XFRCMPT_IRQ_EN 0x10
#define `$INSTANCE_NAME`_ALL_IRQ_EN     0x18
#define `$INSTANCE_NAME`_NEXT_ROW       0x20

#define `$INSTANCE_NAME`_TRANSFER           `$Transfer_Method`
#define `$INSTANCE_NAME`_TRANSFER_FIRMWARE  0
#define `$INSTANCE_NAME`_TRANSFER_ISR       1
#define `$INSTANCE_NAME`_TRANSFER_DMA       2

#define `$INSTANCE_NAME`_SPEED        `$SPEED`
#define `$INSTANCE_NAME`_SPEED_400KHZ 0
#define `$INSTANCE_NAME`_SPEED_800KHZ 1

#define `$INSTANCE_NAME`_MEMORY_TYPE  `$Display_Memory`
#define `$INSTANCE_NAME`_MEMORY_RGB   0
#define `$INSTANCE_NAME`_MEMORY_LUT   1

#if (CY_PSOC3 || CY_PSOC5LP)
    #define  `$INSTANCE_NAME`_PERIOD     ((BCLK__BUS_CLK__KHZ)/`$ClockSpeedKhz`)
#elif (CY_PSOC4)
    #define  `$INSTANCE_NAME`_PERIOD     ((CYDEV_BCLK__HFCLK__KHZ)/`$ClockSpeedKhz`)
#endif /* CY_PSOC5A */
        

#define  `$INSTANCE_NAME`_DATA_ZERO  ((`$INSTANCE_NAME`_PERIOD * 20)/25) 
#define  `$INSTANCE_NAME`_DATA_ONE   ((`$INSTANCE_NAME`_PERIOD * 12)/25) 


#if (`$INSTANCE_NAME`_SPEED_800KHZ)
    #define `$INSTANCE_NAME`_BYTE_TIME_US 10u
    #define `$INSTANCE_NAME`_WORD_TIME_US 30u
#else
    #define `$INSTANCE_NAME`_BYTE_TIME_US 20u
    #define `$INSTANCE_NAME`_WORD_TIME_US 60u
#endif


#define `$INSTANCE_NAME`_COLUMNS     `$LEDs_per_Strip`
#define `$INSTANCE_NAME`_ROWS        `$Channels`
#define `$INSTANCE_NAME`_MIN_X        0u
#define `$INSTANCE_NAME`_MAX_X        (`$INSTANCE_NAME`_COLUMNS - 1)
#define `$INSTANCE_NAME`_MIN_Y        0u
#define `$INSTANCE_NAME`_MAX_Y        (`$INSTANCE_NAME`_ROWS - 1)

//#define `$INSTANCE_NAME`_RBCOLORS     48
#define `$INSTANCE_NAME`_COLOR_WHEEL_SIZE  24


#define `$INSTANCE_NAME`_RED_MASK   0x0000FF00
#define `$INSTANCE_NAME`_GREEN_MASK 0x000000FF
#define `$INSTANCE_NAME`_BLUE_MASK  0x00FF0000

#if(`$INSTANCE_NAME`_MEMORY_TYPE == `$INSTANCE_NAME`_MEMORY_RGB)
   #define getColor( a ) `$INSTANCE_NAME`_CLUT[a]
#else  /* Else use lookup table */
   #define getColor( a ) a
#endif


#define `$INSTANCE_NAME`_CWHEEL_SIZE 24
#define `$INSTANCE_NAME`_YELLOW      getColor(1)
#define `$INSTANCE_NAME`_GREEN       getColor(5)
#define `$INSTANCE_NAME`_ORANGE      getColor(20)
#define `$INSTANCE_NAME`_BLACK       getColor((0 + `$INSTANCE_NAME`_CWHEEL_SIZE))
#define `$INSTANCE_NAME`_OFF         getColor((0 + `$INSTANCE_NAME`_CWHEEL_SIZE))
#define `$INSTANCE_NAME`_LTBLUE      getColor((1 + `$INSTANCE_NAME`_CWHEEL_SIZE))
#define `$INSTANCE_NAME`_MBLUE       getColor((2 + `$INSTANCE_NAME`_CWHEEL_SIZE))
#define `$INSTANCE_NAME`_BLUE        getColor((3 + `$INSTANCE_NAME`_CWHEEL_SIZE))
#define `$INSTANCE_NAME`_LTGREEN     getColor((4 + `$INSTANCE_NAME`_CWHEEL_SIZE))
#define `$INSTANCE_NAME`_MGREEN      getColor((8 + `$INSTANCE_NAME`_CWHEEL_SIZE))
//#define `$INSTANCE_NAME`_GREEN       (12 + `$INSTANCE_NAME`_CWHEEL_SIZE) 
#define `$INSTANCE_NAME`_LTRED       getColor((16 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 
#define `$INSTANCE_NAME`_LTYELLOW    getColor((20 + `$INSTANCE_NAME`_CWHEEL_SIZE))
#define `$INSTANCE_NAME`_MGRED       getColor((32 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 
#define `$INSTANCE_NAME`_RED         getColor((48 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 
#define `$INSTANCE_NAME`_MAGENTA     getColor((51 + `$INSTANCE_NAME`_CWHEEL_SIZE))
#define `$INSTANCE_NAME`_WHITE       getColor((63 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 

#define `$INSTANCE_NAME`_SPRING_GREEN getColor((64 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 
#define `$INSTANCE_NAME`_TURQUOSE    getColor((65 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 
#define `$INSTANCE_NAME`_CYAN        getColor((66 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 
#define `$INSTANCE_NAME`_OCEAN       getColor((67 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 
#define `$INSTANCE_NAME`_VIOLET      getColor((68 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 
#define `$INSTANCE_NAME`_RASPBERRY   getColor((69 + `$INSTANCE_NAME`_CWHEEL_SIZE)) 

#define `$INSTANCE_NAME`_RESET_DELAY_US  55

#endif  /* CY_SLIGHTS_`$INSTANCE_NAME`_H */

//[] END OF FILE
