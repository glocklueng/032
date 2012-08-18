/**
 
 NULLSPACE LABS - HEXAPOD
 
 Author: Arko & MMCA

 *****************  Givin' credit where credit is due  **********************
 * \file
 *
 * \brief PWMA example driver for AVR32 UC3.
 *
 * This file provides an example for the PWMA on AVR32 UC3 devices.
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage
 *
 * \section intro Introduction
 * This application provides an example usage of the PWMA module on UC3
 * products. This example shows how to configure a PWMA and output it on a
 * GPIO.
 *
 * <b>Operating mode</b>:\n Check the used PWMA pin with an oscilloscope, you
 * should see a PWM square wave with varying duty-cycle.
 * - On \b UC3-L0 Xplained, the pin is connected to the red color on the
 *   RGB-LED, showing a cycling fade-out effect.
 * - On \b UC3L-EK, the PWMA-pin is connected to LED0.
 * - On \b STK600-RC3UC3D, the PWMA-pin is connected to PORTA.PA07.
 *
 * \section files Main Files
 * - pwma.c: PWMA driver;
 * - pwma.h: PWMA driver header file;
 * - pwma_example.c: PWMA example application.
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC for AVR32 and IAR Systems compiler
 * for AVR32. Other compilers may or may not work.
 *
 * \section deviceinfo Device Info
 * All AVR32 devices with a PWMA module can be used. This example has been
 * tested with the following setup:
 * - UC3-L0 Xplained
 * - UC3L-EK
 * - STK600-RC3UC3D
 *
 * \section setupinfo Setup Information
 * CPU speed: <i>Internal 120MHz RC oscillator set to run at 15MHz</i>.
 *
 * \section contactinfo Contact Info
 * For more info about Atmel AVR32 visit
 * <A href="http://www.atmel.com/products/AVR32/" >Atmel AVR32</A>\n
 * Support and FAQ: http://support.atmel.no/
 ******************  Givin' credit where credit is due  **********************
 */

#include <avr32/io.h>
#include "gpio.h"
#include "pwma.h"
#include "delay.h"
#include "genclk.h"
#include "sysclk.h"
#include "conf_pwma_example.h"




// DEFINES GOES HERE


/**
 * \brief Main function. Execution starts here.
 *
 * Sets up the PWM module and PWM output, and runs forever cycling through an
 * varying PWM duty cycle pattern.
 */
int main(void)
{
	struct genclk_config gcfg;
	
	uint8_t servo_top = 34;	//34
	uint8_t servo_middle = 20;
	uint8_t servo_bottom = 14;	// 8
	
	
	static uint8_t duty = 10;
	static uint8_t direction = 0;
	
	
	board_init();
	/**
	 * \note the call to sysclk_init() will disable all non-vital
	 * peripheral clocks, except for the peripheral clocks explicitly
	 * enabled in conf_clock.h.
	 */
	sysclk_init();

	
	/**
	 * Start generic clock 3 for the PWM module running at half the clock
	 * speed of the CPU clock.
	 */
	genclk_enable_source(GENCLK_SRC_RC32K);
	genclk_config_defaults(&gcfg, AVR32_PM_GCLK_GCLK3);
	genclk_config_set_source(&gcfg, GENCLK_SRC_RC32K);
	genclk_config_set_divider(&gcfg, 2);
	genclk_enable(&gcfg, AVR32_PM_GCLK_GCLK3);

	/**
	 * Configure the PWM channel with a TOP of 255 and a starting duty
	 * cycle of 0.
	 */
	/*
	pwma_config_and_enable(&AVR32_PWMA, (1 << PWMA11_GPIO) 
									  | (1 << PWMA12_GPIO) 
									  | (1 << PWMA13_GPIO)
									  | (1 << PWMA14_GPIO)
									  | (1 << PWMA15_GPIO)
									  | (1 << PWMA16_GPIO)
									  | (1 << PWMA17_GPIO)
									  | (1 << PWMA18_GPIO)
									  | (1 << PWMA19_GPIO)
									  | (1 << PWMA20_GPIO)
									  | (1 << PWMA21_GPIO)
									  | (1 << PWMA22_GPIO)
									  | (1 << PWMA23_GPIO)
									  | (1 << PWMA24_GPIO)
									  | (1 << PWMA25_GPIO)
									  | (1 << PWMA26_GPIO)
									  | (1 << PWMA27_GPIO)
									  | (1 << PWMA28_GPIO)
									  | (1 << PWMA29_GPIO)
									  | (1 << PWMA30_GPIO)
									  | (1 << PWMA31_GPIO)
									  | (1 << PWMA32_GPIO)
									  | (1 << PWMA33_GPIO)
									  | (1 << PWMA34_GPIO)
									  | (1 << PWMA35_GPIO)
									  , 255, 0);   
	*/
	
	pwma_config_and_enable(&AVR32_PWMA,   (1 << 11)
										| (1 << 12)
										| (1 << 13)
										| (1 << 14)
										| (1 << 15)
										| (1 << 16)
										| (1 << 17)
										| (1 << 18)
										| (1 << 19)
										| (1 << 20)
										| (1 << 21)
										| (1 << 22)
										| (1 << 23)
										| (1 << 24)
										| (1 << 25)
										| (1 << 26)
										| (1 << 27)
										| (1 << 28)
										| (1 << 29)
										| (1 << 30)
										| (1 << 31)
										| (1 << 32)
										| (1 << 33)
										| (1 << 34)
										| (1 << 35)
										, 255, 0);
	
	//pwma_config_and_enable(&AVR32_PWMA, (1 << PWM_CH7), 255, 0);   
	
	/* Initialize the delay routine by providing the CPU clock speed. */
	delay_init(sysclk_get_cpu_hz());
	
	delay_ms(100);
	
	///pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH5), servo_middle);
	///pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH6), servo_middle);
	///pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH7), servo_middle);
	
	pwma_set_channels_value(&AVR32_PWMA, (1 << 11), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 12), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 13), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 14), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 15), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 16), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 17), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 18), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 19), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 20), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 21), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 22), servo_middle);
	/*
	pwma_set_channels_value(&AVR32_PWMA, (1 << 23), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 24), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 25), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 26), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 27), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 28), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 29), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 30), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 31), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 32), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 33), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 34), servo_middle);
	pwma_set_channels_value(&AVR32_PWMA, (1 << 35), servo_middle);
	*/
	
	for (;;) {
		/**
		 * First set a new duty cycle value to the PWM channel, then
		 * increment the duty cycle by one. Since the duty variable is
		 * an uint8_t type it will wrap around at 255 and go to zero.
		 */
		
		/*** CALCULATE LEG POSITIONS ****/
		if(duty >= servo_bottom && duty <= servo_top && direction == 0)
		{
			duty += 1;
		}
		else
		{
			if(duty >= servo_bottom)
			{
				duty -= 1;
				direction = 1;	
			}
			else
			{
				duty = servo_bottom;
				direction = 0;
			}
		}

		/*** CALCULATE LEG POSITIONS ****/
		
		
		/*** MOVE LEGS ****/
		//pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH5), duty);
		//delay_ms(100);
		//pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH6), duty);
		//delay_ms(100);
		//pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH7), duty);
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << 11), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 12), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 13), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 14), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 15), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 16), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 17), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 18), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 19), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 20), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 21), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 22), duty);
		/*
		pwma_set_channels_value(&AVR32_PWMA, (1 << 23), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 24), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 25), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 26), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 27), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 28), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 29), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 30), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 31), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 32), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 33), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 34), duty);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 35), duty);
		*/

		delay_ms(100);
		/*** MOVE LEGS ****/
	}
}
