#include <avr32/io.h>
#include "gpio.h"
#include "pwma.h"
#include "delay.h"
#include "genclk.h"
#include "sysclk.h"
#include "conf_pwma.h"
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
	
	gpio_enable_module_pin(PWMA09_GPIO, PWM09_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA11_GPIO, PWM11_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA12_GPIO, PWM12_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA13_GPIO, PWM13_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA15_GPIO, PWM15_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA16_GPIO, PWM16_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA17_GPIO, PWM17_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA18_GPIO, PWM18_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA19_GPIO, PWM19_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA20_GPIO, PWM20_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA21_GPIO, PWM21_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA22_GPIO, PWM22_PWM_FUNCTION);
	
	
	gpio_enable_module_pin(PWMA23_GPIO, PWM23_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA25_GPIO, PWM25_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA26_GPIO, PWM26_PWM_FUNCTION);
	
	//gpio_enable_module_pin(PWMA27_GPIO, PWM27_PWM_FUNCTION);
	
	gpio_enable_module_pin(PWMA28_GPIO, PWM28_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA29_GPIO, PWM29_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA30_GPIO, PWM30_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA31_GPIO, PWM31_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA32_GPIO, PWM32_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA33_GPIO, PWM33_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA34_GPIO, PWM34_PWM_FUNCTION);
	gpio_enable_module_pin(PWMA35_GPIO, PWM35_PWM_FUNCTION);
	
	
	scif_start_rc32k();
	scif_gc_setup(  AVR32_PM_GCLK_PWMA, 
					SCIF_GCCTRL_RC32K, 
					true,              
					2);                 
	scif_gc_enable(AVR32_PM_GCLK_PWMA);
	
	
	pwma_config_and_enable( &AVR32_PWMA, (1 << PWM_CH1)
									   | (1 << PWM_CH2)
									   | (1 << PWM_CH3)
									   | (1 << PWM_CH4)
									   | (1 << PWM_CH5)
									   | (1 << PWM_CH6)
									   | (1 << PWM_CH7)
									   | (1 << PWM_CH8)
									   | (1 << PWM_CH9)
									   | (1 << PWM_CH10)
									   | (1 << PWM_CH11)
									   | (1 << PWM_CH12)
									   | (1 << PWM_CH13)
									   | (1 << PWM_CH14)
									   | (1 << PWM_CH15)
									   | (1 << PWM_CH16)
									   | (1 << PWM_CH17)
									   | (1 << PWM_CH18)
									   | (1 << PWM_CH19)
									   | (1 << PWM_CH20)
									   | (1 << PWM_CH21)
									   | (1 << PWM_CH22)
									   | (1 << PWM_CH23)
									   | (1 << PWM_CH24)
									   , PWMA_PERIOD_CYCLES, PWMA_DUTY_CYCLE_INIT_VAL );
							 
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
			current_duty_cycle += 5;
		}
		else
		{
			if(current_duty_cycle >= servo_bottom)
			{
				current_duty_cycle -= 5;
				direction = 1;
			}
			else
			{
				current_duty_cycle = servo_bottom;
				direction = 0;
			}
		}
		/*** CALCULATE LEG POSITIONS ****/
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH1) | (1 << PWM_CH2) | (1 << PWM_CH3) | (1 << PWM_CH4)
											|(1 << PWM_CH5) | (1 << PWM_CH6) | (1 << PWM_CH7) | (1 << PWM_CH8)
											|(1 << PWM_CH9) | (1 << PWM_CH10) | (1 << PWM_CH11) | (1 << PWM_CH12)
											|(1 << PWM_CH13) | (1 << PWM_CH14) | (1 << PWM_CH15) | (1 << PWM_CH16)
											|(1 << PWM_CH17) | (1 << PWM_CH18) | (1 << PWM_CH19) | (1 << PWM_CH20)
											|(1 << PWM_CH21) | (1 << PWM_CH22) | (1 << PWM_CH23) | (1 << PWM_CH24), current_duty_cycle);
		
		
		/*
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH1) | (1 << PWM_CH2) | (1 << PWM_CH3) | (1 << PWM_CH4), current_duty_cycle);

		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH5) | (1 << PWM_CH6) | (1 << PWM_CH7) | (1 << PWM_CH8), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH9) | (1 << PWM_CH10) | (1 << PWM_CH11) | (1 << PWM_CH12), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH13) | (1 << PWM_CH14) | (1 << PWM_CH15) | (1 << PWM_CH16), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH17) | (1 << PWM_CH18) | (1 << PWM_CH19) | (1 << PWM_CH20), current_duty_cycle);

		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH21) | (1 << PWM_CH22) | (1 << PWM_CH23) | (1 << PWM_CH24), current_duty_cycle);
		*/
		/*
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH1) | (1 << PWM_CH2), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH3) | (1 << PWM_CH4), current_duty_cycle);

		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH5) | (1 << PWM_CH6), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH7) | (1 << PWM_CH8), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH9) | (1 << PWM_CH10), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH11) | (1 << PWM_CH12), current_duty_cycle);
				
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH13) | (1 << PWM_CH14), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH15) | (1 << PWM_CH16), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH17) | (1 << PWM_CH18), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH19) | (1 << PWM_CH20), current_duty_cycle);

		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH21) | (1 << PWM_CH22), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (1 << PWM_CH23) | (1 << PWM_CH24), current_duty_cycle);
		*/
		
		
		//delay_ms(15);

	}while(1);
	
}
