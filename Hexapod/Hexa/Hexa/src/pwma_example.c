#include <avr32/io.h>
#include "gpio.h"
#include "pwma.h"
#include "delay.h"
#include "genclk.h"
#include "sysclk.h"
#include "conf_pwma_example.h"
#include "board.h"
#include "conf_clock.h"
#include "power_clocks_lib.h"

#define EXAMPLE_CPUCLK_HZ               (48000000UL)
#define EXAMPLE_TARGET_DFLL_FREQ_HZ     (96000000UL)
#define EXAMPLE_TARGET_MCUCLK_FREQ_HZ   (48000000UL)
#define EXAMPLE_TARGET_PBACLK_FREQ_HZ   (48000000UL)

//! Number of period cycles for the PWMA module (0 - 0xff).
#define PWMA_PERIOD_CYCLES    0xFF
//! Minimum number of duty cycles for a PWM channel.
#define PWMA_MIN_DUTY_CYCLES  0

//! Maximum number of duty cycles for a PWM channel.
#define PWMA_MAX_DUTY_CYCLES  0xFF

#define PWMA_DUTY_CYCLES_STEP 0xF // 0xF // 0x11 // 0x19

#define PWMA_DUTY_CYCLE_INIT_VAL  0

#define PWMA_DUTY_CYCLE_INCREASE  (+1)
#define PWMA_DUTY_CYCLE_DECREASE  (-1)


unsigned char duty_cycles_per_led[LED_COUNT] =
{
	PWMA_DUTY_CYCLE_INIT_VAL,
	PWMA_DUTY_CYCLE_INIT_VAL,
	PWMA_DUTY_CYCLE_INIT_VAL,
	PWMA_DUTY_CYCLE_INIT_VAL,
};

static int current_led = 0;  // LED0

static scif_gclk_opt_t gc_dfllif_ref_opt = { SCIF_GCCTRL_SLOWCLOCK, 0, false};
static pcl_freq_param_t pcl_dfll_freq_param = {
	.main_clk_src = PCL_MC_DFLL0,
	.cpu_f        = EXAMPLE_TARGET_MCUCLK_FREQ_HZ,
	.pba_f        = EXAMPLE_TARGET_PBACLK_FREQ_HZ,
	.pbb_f        = EXAMPLE_TARGET_PBACLK_FREQ_HZ,
	.dfll_f       = EXAMPLE_TARGET_DFLL_FREQ_HZ,
	.pextra_params = &gc_dfllif_ref_opt
};

static unsigned long init_clock( unsigned long cpuclk_hz )
{
  unsigned long ret_val = 0u;

  // Program the DFLL and switch the main clock source to the DFLL.
  pcl_configure_clocks(&pcl_dfll_freq_param);

  return (ret_val);
}

static void init_system( void );
static void init_system( void )
{
  int32_t ret_val = 0u;

  /* 1. Configure and start the DFLL0 in closed-loop mode to generate a frequency of 96MHz.
     2. Set Flash Wait state.
     3. Configure CPU, PBA, PBB clock to 48MHz.
     4. Set up the GCLK_CAT for QMatrix operation.  */
  ret_val = init_clock( EXAMPLE_CPUCLK_HZ );
  if(ret_val != 0u)
  {
    while(1); /* Clock configuration failed. */
  }


}

int main(void)
{
	init_system();
	
	
	gpio_enable_module_pin(LED0_GPIO, LED0_PWM_FUNCTION);
	gpio_enable_module_pin(LED1_GPIO, LED1_PWM_FUNCTION);
	gpio_enable_module_pin(LED2_GPIO, LED2_PWM_FUNCTION);
	gpio_enable_module_pin(LED3_GPIO, LED3_PWM_FUNCTION);
	
	////scif_start_rc120M();
	scif_start_rc32k();
	// Setup the generic clock at 120MHz
	scif_gc_setup(  AVR32_PM_GCLK_PWMA, // The generic clock to setup
					SCIF_GCCTRL_RC32K, // The input clock source to use for the generic clock  SCIF_GCCTRL_RC120M
					true,              // Disable the generic clock divisor
					2);                 // divfactor = DFLL0freq/gclkfreq
	// Enable the generic clock
	scif_gc_enable(AVR32_PM_GCLK_PWMA);

	//#
	//# Enable all the PWMAs to output to all LEDs. LEDs are default turned on by
	//# setting PWM duty cycles to 0.
	//#
	pwma_config_and_enable( &AVR32_PWMA, (1 << 20) | (1 << 21) | (1 << 13) | (1 << 34), PWMA_PERIOD_CYCLES, PWMA_DUTY_CYCLE_INIT_VAL );
	
	int current_duty_cycle;
	int leds_intensity_direction = PWMA_DUTY_CYCLE_INCREASE;
	int i;


	do
	{
		// Get the current duty cycle (at that point, all LEDs have the same duty cycle value).
		///current_duty_cycle = duty_cycles_per_led[current_led];
		// Change the duty cycle
		///current_duty_cycle += leds_intensity_direction*PWMA_DUTY_CYCLES_STEP;
		// Constrain the duty cycle to be in [PWMA_MIN_DUTY_CYCLES,PWMA_MAX_DUTY_CYCLES]
		///current_duty_cycle = max(current_duty_cycle, PWMA_MIN_DUTY_CYCLES);
		///current_duty_cycle = min(current_duty_cycle, PWMA_MAX_DUTY_CYCLES);
		// Update the duty cycle of all LEDs.
		///for(i=0; i<LED_COUNT;i++)
		///duty_cycles_per_led[i] = current_duty_cycle;
		
		///////pwma_set_channels_value(&AVR32_PWMA, (1 << 20) | (1 << 21) | (1 << 13) | (1 << 34), current_duty_cycle++);
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << 20) | (1 << 21), current_duty_cycle++);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 13) | (1 << 34), 0xFF-current_duty_cycle);
				
		// Change the direction of the LEDs intensity change.
		///if((current_duty_cycle == PWMA_MAX_DUTY_CYCLES) || (current_duty_cycle == PWMA_MIN_DUTY_CYCLES))
		///{
		///	leds_intensity_direction *= -1;
		///}
		// Make the changes progressive to a human eye by slowing down the rates of
		// the duty cycle changes.
		delay_ms(15);
	}while(1);
	
	
	
	/*
	struct genclk_config gcfg;
	uint8_t duty = 0;

	board_init();
	sysclk_init();
	delay_init(sysclk_get_cpu_hz());
	
	
	
	genclk_config_defaults(&gcfg, EXAMPLE_PWMA_CHANNEL_GCLK);
	genclk_config_set_source(&gcfg, GENCLK_SRC_RC120M); //GENCLK_SRC_RC32K GENCLK_SRC_RC120M
	genclk_config_set_divider(&gcfg, 2);
	genclk_enable(&gcfg, EXAMPLE_PWMA_CHANNEL_GCLK);


	pwma_config_and_enable(&AVR32_PWMA, (1 << 20) | (1 << 21), 255, 0);
	
	//pwma_config_and_enable(&AVR32_PWMA, (1 << 20), 255, 0);
	//pwma_config_and_enable(&AVR32_PWMA, (1 << 21), 255, 0);
	
	//gpio_enable_module_pin(13, EXAMPLE_PWMA_FUNCTION);
	
	gpio_enable_module_pin(20, 4);
	gpio_enable_module_pin(21, 4);
	
	for (;;) 
	{
		pwma_set_channels_value(&AVR32_PWMA, (1 << 20) | (1 << 21), duty++);
		
		//pwma_set_channels_value(&AVR32_PWMA, (1 << 20) , duty++);
		//pwma_set_channels_value(&AVR32_PWMA, (1 << 21) , duty++);
		
		delay_ms(10);
	}
	*/
}
