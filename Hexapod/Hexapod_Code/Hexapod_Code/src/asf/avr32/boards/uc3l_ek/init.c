/*****************************************************************************
 *
 * \file
 *
 * \brief AT32UC3L-EK board initialization.
 *
 * Copyright (c) 2010 Atmel Corporation. All rights reserved.
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
 ******************************************************************************/



#include "compiler.h"
#include "uc3l_ek.h"
#include "conf_board.h"
#include "gpio.h"
#include "board.h"


void board_init(void)
{
	// Configure the pins connected to LEDs as output and set their default
	// initial state to high (LEDs off).
	gpio_configure_pin(PWMA11_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA12_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA13_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA14_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA15_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA16_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA17_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA18_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA19_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA20_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA21_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA22_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	gpio_configure_pin(PWMA23_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA24_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA25_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA26_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA27_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA28_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA29_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA30_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA31_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA32_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA33_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA34_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(PWMA35_GPIO, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	
	/* Enable the PWMA output on the example PWM pin. */
	gpio_enable_module_pin(PWMA11_GPIO, AVR32_PWMA_11_FUNCTION);
	gpio_enable_module_pin(PWMA12_GPIO, AVR32_PWMA_12_FUNCTION);
	gpio_enable_module_pin(PWMA13_GPIO, AVR32_PWMA_13_FUNCTION);
	gpio_enable_module_pin(PWMA14_GPIO, AVR32_PWMA_14_FUNCTION);
	gpio_enable_module_pin(PWMA15_GPIO, AVR32_PWMA_15_FUNCTION);
	gpio_enable_module_pin(PWMA16_GPIO, AVR32_PWMA_16_FUNCTION);
	gpio_enable_module_pin(PWMA17_GPIO, AVR32_PWMA_17_FUNCTION);
	gpio_enable_module_pin(PWMA18_GPIO, AVR32_PWMA_18_FUNCTION);
	gpio_enable_module_pin(PWMA19_GPIO, AVR32_PWMA_19_FUNCTION);
	gpio_enable_module_pin(PWMA20_GPIO, AVR32_PWMA_20_FUNCTION);
	gpio_enable_module_pin(PWMA21_GPIO, AVR32_PWMA_21_FUNCTION);
	gpio_enable_module_pin(PWMA22_GPIO, AVR32_PWMA_22_FUNCTION);
	
	gpio_enable_module_pin(PWMA23_GPIO, AVR32_PWMA_23_FUNCTION);
	gpio_enable_module_pin(PWMA24_GPIO, AVR32_PWMA_24_FUNCTION);
	gpio_enable_module_pin(PWMA25_GPIO, AVR32_PWMA_25_FUNCTION);
	gpio_enable_module_pin(PWMA26_GPIO, AVR32_PWMA_26_FUNCTION);
	gpio_enable_module_pin(PWMA27_GPIO, AVR32_PWMA_27_FUNCTION);
	gpio_enable_module_pin(PWMA28_GPIO, AVR32_PWMA_28_FUNCTION);
	gpio_enable_module_pin(PWMA29_GPIO, AVR32_PWMA_29_FUNCTION);
	gpio_enable_module_pin(PWMA30_GPIO, AVR32_PWMA_30_FUNCTION);
	gpio_enable_module_pin(PWMA31_GPIO, AVR32_PWMA_31_FUNCTION);
	gpio_enable_module_pin(PWMA32_GPIO, AVR32_PWMA_32_FUNCTION);
	gpio_enable_module_pin(PWMA33_GPIO, AVR32_PWMA_33_FUNCTION);
	gpio_enable_module_pin(PWMA34_GPIO, AVR32_PWMA_34_FUNCTION);
	gpio_enable_module_pin(PWMA35_GPIO, AVR32_PWMA_35_FUNCTION);
	
	
	//gpio_enable_module_pin(AVR32_PIN_PA21, AVR32_PWMA_21_FUNCTION);
	//gpio_enable_module_pin(AVR32_PIN_PA20, AVR32_PWMA_20_FUNCTION);
	//gpio_enable_module_pin(AVR32_PIN_PA13, AVR32_PWMA_13_FUNCTION);

/*
#if defined (CONF_BOARD_COM_PORT)
	// USART GPIO pin configuration.
	static const gpio_map_t COMPORT_GPIO_MAP =
	{
		{USART_RXD_PIN, USART_RXD_FUNCTION },
		{USART_TXD_PIN, USART_TXD_FUNCTION }
	};
	gpio_enable_module(COMPORT_GPIO_MAP,
			sizeof(COMPORT_GPIO_MAP) / sizeof(COMPORT_GPIO_MAP[0]));
#endif

#if defined (CONF_BOARD_TWI)
	static const gpio_map_t TWI_GPIO_MAP =
	{
		{WLESS_TWIMS0_TWD_PIN, WLESS_TWIMS0_TWD_FUNCTION},
		{WLESS_TWIMS0_TWCK_PIN, WLESS_TWIMS0_TWCK_FUNCTION}
	};

	// TWI gpio pins cofiguration
	gpio_enable_module(TWI_GPIO_MAP, sizeof(TWI_GPIO_MAP) / sizeof(TWI_GPIO_MAP[0]));
#endif
*/
}
