Gabotronics - June 2010
XMEGA AVR Oscilloscope and Development Kit

www.gabotronics.com
email me at: gabriel@gabotronics.com

Version History:
1.08: Fixed AWG flicker when adusting the offset
1.07: Fixed AWG bug when trying to set frequency below 1.91Hz
1.06: Improved handling off a defective SD card
1.05: Saved current application when powering off
      VCC check before writing to EEPROM in some functions
1.04: Fixed a bug with the automatic cursors at slow sampling rates
1.03: Control of MSO thru the serial port
1.02: Bug Fixed: Min and Max values were not displayed correctly if the value was negative.
1.01: Fixed bug in the SaveCSV function
1.00: Compiling using latest WinAVR compiler: ver 20100110
      Using EEPROM functions from compiler
      MSO: Fixed invert signal bug
      MSO: Using analog comparators for triggering
      MSO: Reference waveforms moved to EEPROM
      AWG: Frequency adjust improved, manual updated
0.99: All storage functions implemented:
      - BMP format save
      - CSV format save
      - WFM format save/load/erase
      Renamed terminal.c to storage.c
0.98: RTC clock (32.768kHz) now used in timer functions
      MSO: - Ext. trigger bug fixed
           - Slow sampling rate acquisition rewritten
           - Fixed slow sampling rate bugs
           - Added more sampling rates: 5S/div, 10S/div, 20S/div, 50S/div
0.97: Fonts moved from EEPROM to Program memory.
      Renamed xmultikit.c to main.c, renamed xmultikit.h to main.h
      Implemented AWG calibration function
      AWG now shows amplitude in volts
      Slew rate control on ADC clock and CH select clock
      More robust clock initialization
      Added reference waveform
0.96: New Schematic, Project name changed to XMEGALAB
      Remapped all pins and functions
      AWG: Implemented new offset for Xmegalab
      The AWG can now be controlled within the MSO
0.95: MSO: Implemented offset calibration.
      MSO: Fixed Meter mode bug.
      MSO: Implemented all measurements in Meter mode.
      MSO: Reduced refresh rate in Meter mode for better readability.
0.94: File System: The file system will initialize whenever the SD Card is inserted
      AWG: Save setttings on EEPROM.
      AWG: Automatically start when the XMultiKit powers up.
      MSO: Save BMP file to SD Card
      MSO: More accurate automatic cursors
      General: Reduced RAM usage
0.93: MSO: BMP file transfer with Hyperterminal working.
0.92: MSO: Implemented all trigger sources
0.91: MSO: Improved triggering
      Logic Analizer: Added parallel and serial decoding
0.90: MSO:  - Menus now show if an item is active or not.
            - Speed optimizations.
            - Eliminated LCD Flicker at slow sampling rates.
0.89: MSO: Improved automatic cursors, Added CH1+CH2 and CH1*CH2 math.
0.88: MSO: Lots of updates, added automatic cursors.
0.87: MSO: Filled in some missing items in the help menu. Started to work on the autosetup routine.
0.86: Nothing specific, a lot of improvements here and there.
0.85: MSO: SPECTRUM ANALYZER WORKING!
0.84: MSO: Voltmeter mode implemented. FFT windows implemented.
0.83: MSO: Voltmeter mode starting to work - big fonts added.
0.82: MSO: Trigger working for fast sample rates (1uS/div,2uS/div,5uS/div)
0.81: MSO: All sampling rates working, now to work on the triggering...
0.80: MSO: Many improvements, better sampling, better menus, fixed some bugs
0.79: MSO: Menus, digital inputs and basic triggering.
0.78: MSO: Slow sampling rates starting to work.
0.77: MSO: Gain control working, EEPROM save working.
0.76: MSO: Fixed minor bugs, LED flashing correctly, Horizontal Position added.
0.75: MSO 1uS/div, 2uS/div, 5uS/div starting to work, added more comments.
0.74: Added M_E constant to parse.h
0.73: Minor updates to the MSO.
0.72: MSO Menus starting to take shape.
0.71: MSO application update, ability to sample at 16MSPS.
0.7:  Added the AVR-MAX Chess from Andre Adrian, an AVR port of H. G. Muller Chess engine.
0.6:  Included a Recursive Descent Parser (Calculator).
0.5:  AWG is working!
0.4:  SD Card with FAT File System working!!! (Thanks to ChaN's library).
0.3:  32MHz clk, Serial RAM, 1KHz Test pin, UART working.
0.2:  Keypad, buttons, encoders working.
0.1:  LCD Display and LEDs working.

www.gabotronics.com
email me at: gabriel@gabotronics.com

In no event shall Gabotronics be liable for any special, indirect, incidential
or concequential damages resulting from the use or inability to use this software.
