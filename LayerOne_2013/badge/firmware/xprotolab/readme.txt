Gabotronics - December 2010
Xprotolab XMEGA AVR Oscilloscope and Development Kit
HEX and EEP Files
ELF Files if using dedicated programmer

Follow this guide for firmware updating:
http://www.gabotronics.com/product-manuals/firmware-updating.htm

www.gabotronics.com
email me at: gabriel@gabotronics.com

This binary file is offered for free, feel free to contact me with feedback,
suggestions and bug reports. I will be glad to improve this project with your
ideas.

This binary file contains the full version of the oscilloscope.

Version History:
2.15	- Minor bug fixed in the XY mode when the ROLL is enabled.
2.14	- Minor improvements to UI
2.13	- Prevent going to sleep when connected to USB
2.12	- Updated interface protocol. The PC interface needs to be updated as well.
2.11	- Better handling when stopping the scope
        - Added Trigger Timeout control
2.10	- USB restart when USB communication lost
2.09	- Code optimizations
2.08	- XY mode:
			* The graph can be moved vertically
			* Cursors
2.07	- If VCC is lower than about 3.15V:
			* An empty battery icon will appear
			* No writes to EEPROM
2.06	- Fixed bug in Frequency Counter
2.05	- Fxed bug in I2C sniffer
2.04	- No change
2.03	- Display's power off sequence fixed
2.02	- Extended reset signal on the power up of the display, added delay before initializing display
2.01	- Extended reset signal on the power up of the display
2.00	- Getting ready to send out a newsletter, reving up to 2.00
1.96	- Improved frequency counter accuracy
1.95	- Changed Frequency counter back to 1sec capture to increase accuracy
		- Fixed bug with the UART sniffer in SINGLE mode
1.94	- More stable VDC readings in the METER mode
		- Fixed bug with the I2C sniffer in SINGLE mode
1.93	- Minor bug fixes with the sniffer intro text
1.92	- Fixed AutoSetup in slow sampling rate
1.91	- Fixed bug with frequency counter
		- Improved Auto Setup when the signals have offset
1.90	- Fixed bug with frequency counter
1.89	- Doubled sampling frequency at slower sampling rates, more samples can be averaged
1.88	- Faster frequency counter (0.5s capture instead of 1s)
		- Frequency counter now displays 8 digits instead of 4
		- Frequency counter can measure and display frequencies over 10MHz  with 1Hz resolution
1.87	- Fixed bug from 1.86 that disabled the post-trigger
		- Approximate values of the 3.3V and the 2.048V reference are shown in the factory settings
1.86	- Fixed bug with display of post trigger value
		- Corrected adjustment of trigger mark
		- Meter VDC now done with 12bit ADC -> more accuracy
		- Added frequency counter on the external trigger, measures up to 9999kHz
		- Improved response of buttons
1.85	- Minor bug fixes
		- Minor changes to the font
		- Improved refresh rate
1.84	- Minor bug fixes
1.83	- Compatible with PC interface v0.17
1.82	- Improved Autosetup
		- Bug fix with the logic data in ROLL mode
		- Bug fix with the flash of the green LED, from ver 1.81
1.81	- Minor bug fix
1.80	- Fixed bug with the display of the trigger value
1.79	- Two new trigger modes: Dual edge trigger, and Window Trigger
		- The trigger mark can move continuosly, and not in discrete steps
		- Correctly position trigger mark if the channel is inverted
		- Corrected the autosetup if the channels are inverted
		- New AWG Sweep features:
			- Sweep speed control
			- Up / Down Control
			- Acceleration mode
			- Range can be set independent of vertical cursors
			- Three pixels on the bottom of the screen shows the range and sweep counter
		- The 3x5 font now contains lowercase letters, useful for the protocol sniffer when displaying ASCII
		- The 3.3V and 2.0V voltages are checked at startup, if out of range, an error is displayed
1.78	- Cursors positions are now saved
		- AWG Sweep on all parameters
1.77    - Fixed AWG discontinuity when changing frequency
		- 3 new AWG Sweep options:
			- Slower Sweep option
			- Ping Pong AWG Sweep mode
			- Start and Stop Frequencies can be set with the vertical cursors
1.76	- Removed Splash screen to gain program space
		- Added two new waveforms to the AWG:
			- Exponential (it is no longer a custom wave)
			- Noise (the AWG buffer is fill with random data)
		- The custom waveform is now set with an ECG
		- Minimum frequency of the AWG changed from 1.91Hz to 1Hz
1.75	- USB interface bug fix
1.74	- Use pin on external interface to signal power down
1.73	- Fixed bug with key input in the protocol sniffer
1.72 	- Space optimizations
1.71	- Space optimizations
1.70	- Improved power up sequence for display
1.69	- Fixed bug that prevented the device to go to sleep at the speified time
		- Improved data transmition to the display
1.68	- Nyquist Frequency shown in FFT mode
		- Bug fixes
1.67	- Added commands to the interface
1.66	- Changed menu flow: Main menus are now:
          HORIZONTAL -> VERTICAL -> TRIGGER -> MODE -> MISCELLANEOUS
		- Fixed FFT cursor bug
1.65	- Optimized trigger
1.64	- More stable slope trigger
1.63	- Bug fixes on the new ROLL mode
1.62	- Bug fixes, Trigger menu changed
1.61	- ROLL mode
1.60	- USB Communication
1.59    - Fixed XY mode CH1 phase
1.58    - Long press menu to power off
        - Long press K1 for auto setup
        - K2+K3 in the default menu will center the horizontal window
1.57    - Changed time base 512us/div to 500us/div
1.56	- Improved  FFT dynamic range
          Replaced FFT cosine window with the Blackman window
        - IQ FFT
1.55    - 16-bit offset calibration data
1.54	- Scope, FFT, and XY mode can be displayed simultaneously
1.53	- Added trigger hold off
1.52	- Improved offset compensation by creating a table of gain/rate values
1.51	- Average on display trace (ELASTIC)
		- Cursor tracking
		- Bug Fixes
1.50    - Validate important settings when reading from EEPROM
1.49    - Space optimizations
1.48    - AWG: Corrected duty cycle polarity on square waves
1.47    - Temporally use USART for PC communications, while USB is in development
1.46    - Space optimizations
1.45    - Space optimizations
1.44    - Fixed minor AWG bugs
1.43    - Increased max AWG frequency from 62.5kHz to 125kHz
          Reduced AWG harmonic distortion
1.42    - Minor space optimizations
1.41    - Converted project to AVR Studio 5
          Ported code from Xprotolab hardware 1.5
1.40    - Fixed bug which prevented the sniffer to display data
1.39    - Fixed minor bug with the automatic cursors
1.38    - Properly scaled the waveform in XY mode to allow more use of the screen
1.37    - Minor update to limit the refresh rate on meter mode
1.36    - Minor update to reduce current consumption
1.35    - Sniffer variables and Logic channel variables are no longer shared
1.34    - Changed logic menu flow to make it more intuitive
        - Ability to stop and start the sniffer
        - Added Sniffer modes:
            - Normal mode: Continuous operation, when the buffer is filled, all pages are erased and the index goes back to page 1
            - Single mode: The sniffer will stop when the buffer is filled.
            - Circular mode: New data will be placed at the end of the last page and older data will be shifted towards the first page
1.33    - Changed trigger menu flow to make it more intuitive
        - Added more interface commands
1.32    - Post Trigger value shown in the menu
1.31    - Better representation of the trigger mark for the slope mode
1.30    - Two trigger modes: edge and slope
        - Implemented falling edge trigger on the logic channels 
1.29    - Fixed bug with the vertical position of the trigger mark
1.28    - Added channel average feature for fast sampling rates
1.27    - Fixed bug with AWG Frequency sweep when using very slow sampling rates.
        - 2x Oversampling to reduce trigger jitter.
        - Eliminated the use of floats variables, which saved lots of program space
1.26    - Fixed Offset calibration bug from version 1.25
1.25    - Complete rewrite of acquisition code, enabling pre and post trigger acquisition.
        - Major Speed and Space optimizations
        - Optimized CRC calculations for XMODEM transfer
1.24    - PC Interface: On fast sampling rates, sending the automatic data will start with a "start of frame", that represents which channel will be sent
1.23    - PC Interface: AWG send settings working
1.22    - Set free trigger when in Meter mode
        - Changed the auto setup function to avoid stalling the program if a waveform is changing continuosly
1.21    - Added more functionality to the PC app
        - Changed the direction of the position MOVE controls
1.20    - Included AWG in interface protocol
1.19    - Basic PC display and control working
1.18    - Improved functions for PC interface
1.17	- No firmware change, but these fuses need to be set:
		Brown out in power down enabled
		Brown out in active enabled
		Brown out level: 2.9V
1.16    - The UART and SPI protocol sniffers can now show ASCII data or HEX data
        - Fixed bug where fast sampling rates where disabled after exiting from sniffer
1.15    - Fixed bug with I2C sniffer, when the Master sent a repeated start
1.14    - The PC interface can now request the firmware version on the Xprotolab
1.13    - Minor bug fix with menu sequence
1.12    - Oscilloscope no longer shows a "clipped" wave if the signal goes beyond the screen.
1.11    - Implemented screen snapshot using Hyperterminal
1.10    - Grid settings are now saved
1.09    - Fixed bug with SINGLE trigger, where the second trace was displayed instead of the first
1.08    - Disable UART when the Xprotolab is connected to a regular USB port on a PC
1.07    - Sampling rate and channel gains will not be changed when entering and leaving meter mode
        - Changed display method in the meter mode to avoid the menu to write on itself
        - When the channel math is enabled and a channel is inverted, the label will be properly
          displayed, instead of showing only "CH1+CH2" or "CH1xCH2"
1.06    - Minor space optimizations
1.05    - Fixed internal conversions which resulted in lower displayed voltages
1.04    - Minor space optimizations
1.03    - Fixed bug in the frequency measurement when the signal has a high offset.
        - Fixed bug in the autosetup when the signal has an offset
        - Fixed bug, the trigger level value was shown incorrectly
1.02    - Fixed bug with the screensaver timer
        - Fixed waveform discontinuity when changing AWG parameters
        - Increased maximum AWG frequency from 31.25KHz to 62.5KHz
        - AWG Frequency readout can now show values above 9999Hz.
1.01    - AWG: Added additional waveform, stored in EEPROM. It is preset with an exponential wave.
        - AWG: Frequency Sweep option!!!
1.00    - Implemented SPI and I2C protocol sniffers
        The SPI's MOSI pin decoding is done in hardware, so it can decode data at high speed,
        but the SPI's MISO pin decoding is implemented in software, so the maximum clock allowed will be limited.
        The actual speed will be shown in the specifications after some tests.
        The I2C is implemented in software as well, but at least the usual I2C clock speed of 400kHz works ok.
        The maximum clock will be specified after testing.
0.99    - Improved performance on the Meter mode
0.98    - Speed and space optimizations
        - 20mS/div is now a slow sampling rate.
          (In slow sampling rates, samples are drawn continuosly as acquired)
          This improves the tactile switch response time (Before, the switch interrupt had to wait up to 320mS for the acquisition to be complete)
        - Added channel math on slow sampling rates
        - Eliminated noise when using high gains at 1mS/div and 2mS/div
        - Improved triggering at slow sampling rates
0.97    - Added channel math options: CH1+CH2 and CH1*CH2
0.96    - Screensaver: To extend the OLED life and to reduce power,
          there is a new option to enter sleep mode after a certain
          amount of minutes.  This timeout is configured in the factory options
          The system will wakeup after a key is pressed.
        - Horizontal scroll enabled when acquisition is stopped
        - The screen won't be cleared when doing the auto key repeat
        - Fixed bug when the autosetup was used with the FFT log enabled
        - Instead of having the user select which oscillator to use,
          the micro will automatically try the external crystal oscillator,
          if it fails, it will use the internal oscillator
0.95    - Some tweaks on the autosetup
0.94:   - Oscilloscope autosetup by pressing K2 and K3 in the main menu
        - Fixed CH2 autosetup bug
        - Added more features to the voltmeter: VDC, VPP and Frequency display
        - The autosetup is done quicker
0.93:   - The device will start with the internal oscillator by default,
          the default oscillator can be changed to the external crystal in the factory options
        - The device will automatically adjust the gain and sampling rate settings 
          when in voltmeter mode
        - Fixed bugs in the voltmeter mode (displaying incorrect voltage)
0.92:   - Added more UART decoding options: parity and number of stop bits
        - Changed gain settings from:
            10, 5, 2.5, 1.25, 0.625, 0.3125, 0.15625 V/div
            to
            5, 2.5, 1.25, 0.625, 0.3125, 0.15625, 78.125m V/div
        - The channel position (0V reference) can be set to any level from the top to the bottom of the screen
        - Adjusting the channel position won't clip the signal anymore
        - Enabled the external trigger
0.91    - UART Protocol Sniffer implemented.
        - Fixed slow sampling rate timing, messed up in version 0.90
0.90    - Reduced noise by slowing down ADC clock in slow sample rates
        - Fixed bug that prevented the display of the fft at slow sample rates
        - Fixed bug where some variables accessed the same memory space
        - Space optimizations, need to squeeze in more features...
0.80    - Improved switch debounce
        - Added a bit more histerisys on trigger detection
        - Space optimizations
        - Offset compensation
        - Logic input options: No Pull, Pull-up, Pull-down
        - Enabled sampling rate 8uS/div for CH1 and logic inputs only
0.70    - Hardware modification for improved performance:
          R1,R4,R5,R6,R11,R13 change to 3K ohm, 1%
          C2,C4,C9 change to 330pF
          This modification will reduce crosstalk and reduce noise.
        - Added cursor menu items: automatic cursors, lock cursors, reference waveform
        - Space optimizations
        - For better use of the DMA, changed sampling rates
          100uS/div, 200uS/div, 500uS/div to:
          128uS/div, 256uS/div, 512uS/div
        - Enabled gain 10V/div
0.60:   - Enabled sampling rates: 16uS/div, 32uS/div, 64uS/div
0.50:   - Speed optimizations
        - Fixed slow sampling rate display bug
        - Removed unnecesary files
0.40:   - FFT: When both channels are on, the screen is divided for both FFTs
        - Fixed some weird characters in the fonts
        - Logic: Parallel and Serial decoding working
0.30:   - CH2 shaky trigger fixed
        - Test at startup enabled
        - Correct version shown in splash screen
        - Fixed slow sampling rate bug
        - Auto repeat keys when pressed for more than 1 second (for K2 and K3)
        - Fixed CH2 horizontal cursor bug
        - Trigger shortcut
        - Trigger level display now shown in volts
        - Logic inputs now shown correctly with slow sample rates
        - Logic input can now be trigger source
0.20: Logic inputs, trigger level fixed, data invert fix
0.10: First Version, code ported from Xminilab, but still a lot to do

www.gabotronics.com
email me at: gabriel@gabotronics.com

This source code is offered for free, feel free to contact me with feedback,
suggestions and bug reports. I will be glad to improve this project with your
ideas.

Copyright 2010 Gabriel Anzziani
This project is distributed under the terms of the GNU General Public License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

In no event shall Gabotronics be liable for any special, indirect, incidential
or concequential damages resulting from the use or inability to use this software.
