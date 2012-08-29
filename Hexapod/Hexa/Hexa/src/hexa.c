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
	
	
	pwma_config_and_enable( &AVR32_PWMA, (PWM_CH1)
									   | (PWM_CH2)
									   | (PWM_CH3)		
									   | (PWM_CH4)		// Base 1
									   | (PWM_CH5)		// Leg  1
									   | (PWM_CH6)		// Foot 1
									   | (PWM_CH7)		// Base 2
									   | (PWM_CH8)		// Leg  2
									   | (PWM_CH9)		// Foot 2
									   | (PWM_CH10)		// Base 3
									   | (PWM_CH11)		// Leg  3	
									   | (PWM_CH12)		// Foot 3
									   | (PWM_CH13)		
									   | (PWM_CH14)		
									   | (PWM_CH15)
									   | (PWM_CH16)		// Base 4
									   | (PWM_CH17)		// Leg  4
									   | (PWM_CH18)		// Foot 4
									   | (PWM_CH19)		// Base 5
									   | (PWM_CH20)		// Leg  5
									   | (PWM_CH21)		// Foot 5
									   | (PWM_CH22)		// Base 6
									   | (PWM_CH23)		// Leg  6
									   | (PWM_CH24)		// Foot 6
									   , PWMA_PERIOD_CYCLES, PWMA_DUTY_CYCLE_INIT_VAL );

	
//		SERVOS			{Base,Leg,Foot}
	uint8_t servos[6][3] = {{23,10,28},
						    {19,10,28},
							{17,10,28},
							{16,10,28},
							{22,10,28},
							{22,10,28}};
	
	//		CENTER			{Base,Leg,Foot}
	uint8_t center[6][3] = {{23,10,28},
						    {19,10,28},
							{17,10,28},
							{16,10,28},
							{22,10,28},
							{22,10,28}};
	
	//		DIRECTION		   {Base,Leg,Foot}
	uint8_t direction[6][3] = {{0,0,0},
						       {0,0,0},
							   {0,0,0},
						       {0,0,0},
						       {0,0,0},
						       {0,0,0}};
															
	uint8_t base_width = 4; // 2
	uint8_t leg_width = 4;  // 2
	uint8_t foot_width = 2;
	 					 
	// CCW +
	// CW -					 
	
	delay_init(sysclk_get_cpu_hz());
	
	// Center all motors
	// ----------------------------------------------------
	// Update Leg 1
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH4), servos[0][0]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH5), servos[0][1]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH6), servos[0][2]);
	
	// Update Leg 2
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH7), servos[1][0]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH8), servos[1][1]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH9), servos[1][2]);
	
	// Update Leg 3
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH10), servos[2][0]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH11), servos[2][1]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH12), servos[2][2]);
	
	// Update Leg 4
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH16), servos[3][0]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH17), servos[3][1]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH18), servos[3][2]);
	
	// Update Leg 5
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH19), servos[4][0]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH20), servos[4][1]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH21), servos[4][2]);
	
	// Update Leg 6
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH22), servos[5][0]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH23), servos[5][1]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH24), servos[5][2]);
	// ----------------------------------------------------
	
	do
	{
		/* DANCE MODE
		int l = 0;
		int r = 0;
		
		for(r = 0; r < 6; r++)
		{
			// Move Base
			if(servos[r][1] >= center[r][1]-base_width && servos[r][1] <= center[r][1]+base_width && direction[r][1] == 0)
			{
				servos[r][1] += 1;
			}
			else
			{
				if(servos[r][1] >= center[r][1]-base_width)
				{
					servos[r][1] -= 1;
					direction[r][1] = 1;
				}
				else
				{
					servos[r][1] = center[r][1]-base_width;
					direction[r][1] = 0;
				}
			}
		
			// Move Leg
			if(servos[r][2] >= center[r][2] && servos[r][2] <= center[r][2]+base_width && direction[r][2] == 0)
			{
				servos[r][2] += 1;
			}
			else
			{
				if(servos[r][2] >= center[r][2])
				{
					servos[r][2] -= 1;
					direction[r][2] = 1;
				}
				else
				{
					servos[r][2] = center[r][2];
					direction[r][2] = 0;
				}
			}
		}
		
		
		// Update Leg 1
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH4), servos[0][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH5), servos[0][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH6), servos[0][2]);
		
		// Update Leg 2
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH7), servos[1][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH8), servos[1][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH9), servos[1][2]);
		
		// Update Leg 3
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH10), servos[2][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH11), servos[2][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH12), servos[2][2]);
		
		// Update Leg 4
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH16), servos[3][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH17), servos[3][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH18), servos[3][2]);
		
		// Update Leg 5
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH19), servos[4][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH20), servos[4][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH21), servos[4][2]);
		
		// Update Leg 6
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH22), servos[5][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH23), servos[5][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH24), servos[5][2]);
		*/
		
		
		// Lift Legs
		servos[1][1] += 10;
		servos[1][2] += 10;
		servos[3][1] += 10;
		servos[3][2] += 10;
		servos[5][1] += 10;
		servos[5][2] += 10;
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH8), servos[1][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH9), servos[1][2]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH17), servos[3][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH18), servos[3][2]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH23), servos[5][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH24), servos[5][2]);
		
		
		// Move Body Forward
		servos[0][0] += 4;
		servos[2][0] += 4;
		servos[4][0] -= 4;
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH4), servos[0][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH10), servos[2][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH19), servos[4][0]);
		
		
		// Move Legs Forward
		servos[1][0] -= 4;
		servos[3][0] += 4;
		servos[5][0] += 4;
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH7), servos[1][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH16), servos[3][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH22), servos[5][0]);
		
		
		// Drop Legs
		servos[1][1] -= 10;
		servos[1][2] -= 10;
		servos[3][1] -= 10;
		servos[3][2] -= 10;
		servos[5][1] -= 10;
		servos[5][2] -= 10;
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH8), servos[1][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH9), servos[1][2]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH17), servos[3][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH18), servos[3][2]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH23), servos[5][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH24), servos[5][2]);
		
		
		// Lift Legs
		servos[0][1] += 6;
		servos[0][2] += 6;
		servos[2][1] += 6;
		servos[2][2] += 6;
		servos[4][1] += 6;
		servos[4][2] += 6;
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH5), servos[0][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH6), servos[0][2]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH11), servos[2][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH12), servos[2][2]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH20), servos[4][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH21), servos[4][2]);
		
		
		// Move Body Forward
		servos[1][0] += 4;
		servos[3][0] -= 4;
		servos[5][0] -= 4;
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH7), servos[1][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH16), servos[3][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH22), servos[5][0]);
		
		// Move Legs Forward
		servos[0][0] -= 4;
		servos[2][0] -= 4;
		servos[4][0] += 4;
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH4), servos[0][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH10), servos[2][0]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH19), servos[4][0]);
		
		
		// Drop Legs
		servos[0][1] -= 6;
		servos[0][2] -= 6;
		servos[2][1] -= 6;
		servos[2][2] -= 6;
		servos[4][1] -= 6;
		servos[4][2] -= 6;
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH5), servos[0][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH6), servos[0][2]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH11), servos[2][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH12), servos[2][2]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH20), servos[4][1]);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH21), servos[4][2]);
		
	
		
	}while(1);
	
}



/*
void dance(void)
{
	int l = 0;
	int r = 0;
	
	for(r = 0; r < 6; r++)
	{
		// Move Base
		if(servos[r][1] >= center[r][1]-base_width && servos[r][1] <= center[r][1]+base_width && direction[r][1] == 0)
		{
			servos[r][1] += 1;
		}
		else
		{
			if(servos[r][1] >= center[r][1]-base_width)
			{
				servos[r][1] -= 1;
				direction[r][1] = 1;
			}
			else
			{
				servos[r][1] = center[r][1]-base_width;
				direction[r][1] = 0;
			}
		}
		
		// Move Leg
		if(servos[r][2] >= center[r][2] && servos[r][2] <= center[r][2]+base_width && direction[r][2] == 0)
		{
			servos[r][2] += 1;
		}
		else
		{
			if(servos[r][2] >= center[r][2])
			{
				servos[r][2] -= 1;
				direction[r][2] = 1;
			}
			else
			{
				servos[r][2] = center[r][2];
				direction[r][2] = 0;
			}
		}
	}

	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH5)|(PWM_CH8)|(PWM_CH11)|(PWM_CH17)|(PWM_CH20)|(PWM_CH23), servos[0][1]);
	pwma_set_channels_value(&AVR32_PWMA, (PWM_CH6)|(PWM_CH9)|(PWM_CH12)|(PWM_CH18)|(PWM_CH21)|(PWM_CH24), servos[0][2]);
}
*/







		/*
		uint8_t base_duty_cycle = 23;
		uint8_t leg_duty_cycle = 10;
		uint8_t foot_duty_cycle = 28;
		
		uint8_t servo_base_top = 26;		// IN // CCW +
		uint8_t servo_base_middle = 23;		// CENTER
		uint8_t servo_base_bottom = 20;		// OUT // CW -
		uint8_t servo_base_direction = 0;
		
		uint8_t servo_leg_top = 22;			// IN // CCW
		uint8_t servo_leg_middle = 10;		// CENTER
		uint8_t servo_leg_bottom = 4;		// OUT // CW
		uint8_t servo_leg_direction = 0;
		
		uint8_t servo_foot_top = 33;		// IN // CCW
		uint8_t servo_foot_middle = 28;		// CENTER
		uint8_t servo_foot_bottom = 14;		// OUT // CW
		uint8_t servo_foot_direction = 0;
		
		
		uint8_t servo_base_1 = 23;
		uint8_t servo_base_2 = 19;
		uint8_t servo_base_3 = 17;
		uint8_t servo_base_4 = 16;
		uint8_t servo_base_5 = 22;
		uint8_t servo_base_6 = 22;
		
		int leds_intensity_direction = PWMA_DUTY_CYCLE_INCREASE;
		int i;
		
		/*
		// CALCULATE FOOT POSITIONS
		if(foot_duty_cycle >= servo_foot_bottom && foot_duty_cycle <= servo_foot_top && servo_foot_direction == 0)
		{
			foot_duty_cycle += 1;
		}
		else
		{
			if(foot_duty_cycle >= servo_foot_bottom)
			{
				foot_duty_cycle -= 1;
				servo_foot_direction = 1;
			}
			else
			{
				foot_duty_cycle = servo_foot_bottom;
				servo_foot_direction = 0;
			}
		}
		// CALCULATE FOOT POSITIONS 
		
		// CALCULATE LEG POSITIONS 
		if(leg_duty_cycle >= servo_leg_bottom && leg_duty_cycle <= servo_leg_top && servo_leg_direction == 0)
		{
			leg_duty_cycle += 1;
		}
		else
		{
			if(leg_duty_cycle >= servo_leg_bottom)
			{
				leg_duty_cycle -= 1;
				servo_leg_direction = 1;
			}
			else
			{
				leg_duty_cycle = servo_leg_bottom;
				servo_leg_direction = 0;
			}
		}
		// CALCULATE LEG POSITIONS 
		
		// CALCULATE BASE POSITIONS 
		if(base_duty_cycle >= servo_base_bottom && base_duty_cycle <= servo_base_top && servo_base_direction == 0)
		{
			base_duty_cycle += 1;
		}
		else
		{
			if(base_duty_cycle >= servo_base_bottom)
			{
				base_duty_cycle -= 1;
				servo_base_direction = 1;
			}
			else
			{
				base_duty_cycle = servo_base_bottom;
				servo_base_direction = 0;
			}
		}
		// CALCULATE BASE POSITIONS 
		*/
		


		/* FAST SPEED UPDATE ALL
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH1) | (PWM_CH2) | (PWM_CH3) | (PWM_CH4)
											|(PWM_CH5) | (PWM_CH6) | (PWM_CH7) | (PWM_CH8)
											|(PWM_CH9) | (PWM_CH10) | (PWM_CH11) | (PWM_CH12)
											|(PWM_CH13) | (PWM_CH14) | (PWM_CH15) | (PWM_CH16)
											|(PWM_CH17) | (PWM_CH18) | (PWM_CH19) | (PWM_CH20)
											|(PWM_CH21) | (PWM_CH22) | (PWM_CH23) | (PWM_CH24), current_duty_cycle);
		/* */
		
		
		/* MEDIUM SPEED UPDATE ALL
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH1) | (PWM_CH2) | (PWM_CH3) | (PWM_CH4), current_duty_cycle);

		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH5) | (PWM_CH6) | (PWM_CH7) | (PWM_CH8), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH9) | (PWM_CH10) | (PWM_CH11) | (PWM_CH12), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH13) | (PWM_CH14) | (PWM_CH15) | (PWM_CH16), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH17) | (PWM_CH18) | (PWM_CH19) | (PWM_CH20), current_duty_cycle);

		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH21) | (PWM_CH22) | (PWM_CH23) | (PWM_CH24), current_duty_cycle);
		/* */
		
		
		/* SLOW SPEED UPDATE ALL */
		/*
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH1) | (PWM_CH2), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH3) | (PWM_CH4), current_duty_cycle);

		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH5) | (PWM_CH6), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH7) | (PWM_CH8), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH9) | (PWM_CH10), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH11) | (PWM_CH12), current_duty_cycle);
				
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH13) | (PWM_CH14), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH15) | (PWM_CH16), current_duty_cycle);
		
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH17) | (PWM_CH18), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH19) | (PWM_CH20), current_duty_cycle);

		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH21) | (PWM_CH22), current_duty_cycle);
		pwma_set_channels_value(&AVR32_PWMA, (PWM_CH23) | (PWM_CH24), current_duty_cycle);
		/* */