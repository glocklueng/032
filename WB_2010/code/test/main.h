/**
 * \file main.h
 *
 * Main header file
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

#define SPICLK_PORT     PORTC   //!< SPI CLOCK Port for digital poti
#define SPIDO_PORT      PORTC   //!< SPI DATA Port for digital poti
#define SPICS_PORT      PORTC   //!< SPI CHIPSELECT Port for digital poti
#define SPICLK_DDR      DDRC    //!< SPI CLOCK Direction for digital poti
#define SPIDO_DDR       DDRC    //!< SPI DATA Direction for digital poti
#define SPICS_DDR       DDRC    //!< SPI CHIPSELECT Direction for digital poti
#define SPICLK          1       //!< SPI CLOCK Pin for digital poti
#define SPIDO           2       //!< SPI DATA Pin for digital poti
#define SPICS           3       //!< SPI CHIPSELECT Pin for digital poti

#define BANDWADJ1_RES   1       //!< Digital potentiometer for VCO1
#define BANDWADJ2_RES   0       //!< Digital potentiometer for VCO2

#define FREQSET_DDR     DDRC    //!< Digital frequency set direction
#define FREQSET_PORT    PORTC   //!< Digital frequency set port
#define FREQSET         PC4     //!< Digital frequency set pin

#define POWERON_PORT    PORTB   //!< Power ON switch port
#define POWERON         PB6     //!< Power ON switch pin
#define POWERON_DDR     DDRB    //!< Power ON direction

#define PROGKEY_PORT    PORTB   //!< Program key port
#define PROGKEY_PIN     PINB    //!< Program key input
#define PROGKEY         PB4     //!< Program key pin
#define PROGKEY_DDR     DDRB    //!< Program key direction

#define POWERCTL1_PORT  PORTB   //!< Power control port VCO1
#define POWERCTL1       PB7     //!< Power control pin VCO1
#define POWERCTL1_DDR   DDRB    //!< Power control direction VCO1

#define POWERCTL2_PORT  PORTD   //!< Power control port VCO2
#define POWERCTL2       PD3     //!< Power control pin VCO2
#define POWERCTL2_DDR   DDRD    //!< Power control direction VCO2

#define LEDDDR          DDRD    //!< Direction for LED
#define LEDPORT         PORTD   //!< Port for LED
#define LED             PD7     //!< Pin for LED

extern volatile char in_char;

extern void set_sawtooth_low(void);
extern void set_sawtooth_high(void);
extern void set_resistor(uint8_t rnum, uint8_t rval);
extern void delay_ms(uint16_t ms);

#endif /* MAIN_H_ */