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

#define PWMA_PERIOD_CYCLES    0xFF

#define PWMA_MIN_DUTY_CYCLES  0

#define PWMA_MAX_DUTY_CYCLES  0xFF

#define PWMA_DUTY_CYCLES_STEP 0xF 

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
  
  pcl_configure_clocks(&pcl_dfll_freq_param);

  return (ret_val);
}

static void init_system( void );
static void init_system( void )
{
  int32_t ret_val = 0u;

  ret_val = init_clock( EXAMPLE_CPUCLK_HZ );
  if(ret_val != 0u)
  {
    while(1); 
  }
}

int main(void)
{
	init_system();
	
	gpio_enable_module_pin(LED0_GPIO, LED0_PWM_FUNCTION);
	gpio_enable_module_pin(LED1_GPIO, LED1_PWM_FUNCTION);
	gpio_enable_module_pin(LED2_GPIO, LED2_PWM_FUNCTION);
	gpio_enable_module_pin(LED3_GPIO, LED3_PWM_FUNCTION);
	
	scif_start_rc32k();
	scif_gc_setup(  AVR32_PM_GCLK_PWMA, 
					SCIF_GCCTRL_RC32K, 
					true,              
					2);                 
	scif_gc_enable(AVR32_PM_GCLK_PWMA);

	pwma_config_and_enable( &AVR32_PWMA, (1 << 20) | (1 << 21) | (1 << 13) | (1 << 34), PWMA_PERIOD_CYCLES, PWMA_DUTY_CYCLE_INIT_VAL );
	
	uint8_t current_duty_cycle = 0;
	int leds_intensity_direction = PWMA_DUTY_CYCLE_INCREASE;
	int i;

	uint8_t servo_top = 34;	//34
	uint8_t servo_middle = 20;
	uint8_t servo_bottom = 14;	// 8
	uint8_t direction = 0;
	
	do
	{
		/*** CALCULATE LEG POSITIONS ****/
		if(current_duty_cycle >= servo_bottom && current_duty_cycle <= servo_top && direction == 0)
		{
			current_duty_cycle += 1;
		}
		else
		{
			if(current_duty_cycle >= servo_bottom)
			{
				current_duty_cycle -= 1;
				direction = 1;
			}
			else
			{
				current_duty_cycle = servo_bottom;
				direction = 0;
			}
		}

		/*** CALCULATE LEG POSITIONS ****/
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << 20) | (1 << 21), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (1 << 13) | (1 << 34), current_duty_cycle);
				
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
