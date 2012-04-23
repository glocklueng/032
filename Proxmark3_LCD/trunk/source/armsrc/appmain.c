#define WITH_LCD 1

// vaid lf tag id
// 2200 56F6 A5 27
// 2200 56f6 a5
// 0108 72e7 7c
// 2200 534F A8 96

#define MAX_MENU ( 7)

//-----------------------------------------------------------------------------
//The main application`code. This is the first thing called after start.c
// executes.
// Jonathan Westhues, Mar 2006
// Edits by Gerhard de Koning Gans, Sep 2007 (##)
//-----------------------------------------------------------------------------

#include <proxmark3.h>
#include <stdlib.h>
#include "apps.h"
#include "legicrf.h"
#include "msd.h"

#ifdef WITH_LCD
#include "gui_layout.h"
#include "glcd.h"
#endif

#ifdef GCC
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end
#else
#include <stdarg.h>
#endif

#pragma data_alignment=16
uint32_t BigBuf[BIG_BUFFER];

unsigned int songi = 0;
extern const char song[][512];

int kvsprintf(char const *fmt, void *arg, int radix, va_list ap);
void PrepBuffer();
void play(const char *p,int);
int GetSong(void);

//=============================================================================
// A buffer where we can queue things up to be sent through the FPGA, for
// any purpose (fake tag, as reader, whatever). We go MSB first, since that
// is the order in which they go out on the wire.
//=============================================================================

BYTE ToSend[256];
int ToSendMax;
static int ToSendBit;

struct common_area common_area 
#ifdef GCC
__attribute__((section(".commonarea"))) 
#endif
;

void BufferClear(void)
{
	memset(BigBuf,0,sizeof(BigBuf));
	Dbprintf("Buffer cleared (%i bytes)",sizeof(BigBuf));
}

void ToSendReset(void)
{
	ToSendMax = -1;
	ToSendBit = 8;
}

void ToSendStuffBit(int b)
{
	if(ToSendBit >= 8) {
		ToSendMax++;
		ToSend[ToSendMax] = 0;
		ToSendBit = 0;
	}

	if(b) {
		ToSend[ToSendMax] |= (1 << (7 - ToSendBit));
	}

	ToSendBit++;

	if(ToSendBit >= sizeof(ToSend)) {
		ToSendBit = 0;
		DbpString("ToSendStuffBit overflowed!");
	}
}

//=============================================================================
// Debug print functions, to go out over USB, to the usual PC-side client.
//=============================================================================

void DbpString(char *str)
{
	/* this holds up stuff unless we're connected to usb */
	if (!UsbConnected())
		return;

	UsbCommand c;
	c.cmd = CMD_DEBUG_PRINT_STRING;
        // charliex: potential overflow fix
	c.arg[0] = MIN(sizeof(c.d.asBytes)-1, strlen(str));
	memcpy(c.d.asBytes, str, c.arg[0]);

	UsbSendPacket((BYTE *)&c, sizeof(c));
	SpinDelay(50);
}

void Dbprintf(const char *fmt, ...) {
// should probably limit size here; oh well, let's just use a big buffer
	char output_string[128];
	va_list ap;

	va_start(ap, fmt);
	kvsprintf(fmt, output_string, 10, ap);
	va_end(ap);
 
	DbpString(output_string);
}

void Dbprintf_lcd(int x,int y,const char *fmt, ...) {
// should probably limit size here; oh well, let's just use a big buffer
	char output_string[128];
	va_list ap;

	va_start(ap, fmt);
	kvsprintf(fmt, output_string, 10, ap);
	va_end(ap);
 
	lcd_gotoxy(POS(x),LINE(y));
	lcd_puts(output_string);
}
// prints HEX & ASCII
void Dbhexdump(int len, uint8_t *d) {
	int l=0,i;
	char ascii[9];

	while (len>0) {
		if (len>8) l=8;
		else l=len;
		
		memcpy(ascii,d,l);
		ascii[l]=0;	
		
		// filter safe ascii
		for (i=0;i<l;i++) 
			if (ascii[i]<32 || ascii[i]>126) ascii[i]='.';

		Dbprintf("%-8s %*D",ascii,l,d," ");

		len-=8;
		d+=8;		
	}
}

//-----------------------------------------------------------------------------
// Read an ADC channel and block till it completes, then return the result
// in ADC units (0 to 1023). Also a routine to average 32 samples and
// return that.
//-----------------------------------------------------------------------------
static int ReadAdc(int ch)
{
	uint32_t d;

	AT91C_BASE_ADC->ADC_CR = AT91C_ADC_SWRST;
	AT91C_BASE_ADC->ADC_MR =
		ADC_MODE_PRESCALE(32) |
		ADC_MODE_STARTUP_TIME(16) |
		ADC_MODE_SAMPLE_HOLD_TIME(8);
	AT91C_BASE_ADC->ADC_CHER = ADC_CHANNEL(ch);

	AT91C_BASE_ADC->ADC_CR = AT91C_ADC_START;
	while(!(AT91C_BASE_ADC->ADC_SR & ADC_END_OF_CONVERSION(ch)))
		;
	d = AT91C_BASE_ADC->ADC_CDR[ch];

	return d;
}

int AvgAdc(int ch) // was static - merlok
{
	int i;
	int a = 0;

	for(i = 0; i < 32; i++) {
		a += ReadAdc(ch);
	}

	return (a + 15) >> 5;
}

void MeasureAntennaTuning(void)
{
	uint8_t *dest = (uint8_t *)BigBuf;
	int i, ptr = 0, adcval = 0, peak = 0, peakv = 0, peakf = 0;;
	int vLf125 = 0, vLf134 = 0, vHf = 0;	// in mV

	UsbCommand c;

	DbpString("Measuring antenna characteristics, please wait.");
	memset(BigBuf,0,sizeof(BigBuf));

/*
 * Sweeps the useful LF range of the proxmark from
 * 46.8kHz (divisor=255) to 600kHz (divisor=19) and
 * read the voltage in the antenna, the result left
 * in the buffer is a graph which should clearly show
 * the resonating frequency of your LF antenna
 * ( hopefully around 95 if it is tuned to 125kHz!)
 */
	FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_READER);
	
	for (i=255; i>19; i--) {
		FpgaSendCommand(FPGA_CMD_SET_DIVISOR, i);
		SpinDelay(20);
		// Vref = 3.3V, and a 10000:240 voltage divider on the input
		// can measure voltages up to 137500 mV
		adcval = ((137500 * AvgAdc(ADC_CHAN_LF)) >> 10);
		if (i==95) 	vLf125 = adcval; // voltage at 125Khz
		if (i==89) 	vLf134 = adcval; // voltage at 134Khz

		dest[i] = adcval>>8; // scale int to fit in byte for graphing purposes
		if(dest[i] > peak) {
			peakv = adcval;
			peak = dest[i];
			peakf = i;
		}
	}

	// Let the FPGA drive the high-frequency antenna around 13.56 MHz.
	FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_READER_RX_XCORR);
	SpinDelay(20);
	// Vref = 3300mV, and an 10:1 voltage divider on the input
	// can measure voltages up to 33000 mV
	vHf = (33000 * AvgAdc(ADC_CHAN_HF)) >> 10;

	c.cmd = CMD_MEASURED_ANTENNA_TUNING;
	c.arg[0] = (vLf125 << 0) | (vLf134 << 16);
	c.arg[1] = vHf;
	c.arg[2] = peakf | (peakv << 16);

	UsbSendPacket((uint8_t *)&c, sizeof(c));
}
void MeasureAntennaTuningHf(void)
{
  int vHf = 0;	// in mV 
  DbpString("Measuring HF");  	
  
 // LED_A_OFF();
  
  for (;;) { 
  // Let the FPGA drive the high-frequency antenna around 13.56 MHz. 212 		
    
    FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_READER_RX_XCORR); 
//  	FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_READER);
  
	SpinDelay(50);  		
    // Vref = 3300mV, and an 10:1 voltage divider on the input 216 		
    // can measure voltages up to 33000 mV 		
    vHf = (33000 * AvgAdc(ADC_CHAN_HF)) >> 10;  		

	PWMC_BeepN(1,vHf*4);
             
//    Dbprintf("%d mV",vHf); 
    Dbprintf_lcd(0,10,"%d mV",vHf); 
	
     if (BUTTON_PRESS()) 
      break;  	
    }  	
    DbpString("cancelled");
  PWMC_DisableChannel(1);
LED_A_ON();
  }
  
void MeasureAntennaTuningLf(void)
{
  int vHf = 0;	// in mV 
  DbpString("Measuring LF");  	
  
 // LED_A_OFF();
  	FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_READER);
 	FpgaSendCommand(FPGA_CMD_SET_DIVISOR, 95);
 
  for (;;) { 
    
  	FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_READER);
 
	SpinDelay(50);  		
    // Vref = 3300mV, and an 10:1 voltage divider on the input 216 		
    // can measure voltages up to 33000 mV 		
    vHf = (137500 * AvgAdc(ADC_CHAN_LF)) >> 10;  		

	PWMC_BeepN(1,vHf*4);
             
    Dbprintf("%d mV",vHf); 
    Dbprintf_lcd(0,10,"%d mV",vHf); 
     if (BUTTON_PRESS()) 
      break;  	
    }  	
  DbpString("cancelled");
  
  PWMC_DisableChannel(1);

  LED_A_ON();
  
}
  

void SimulateTagHfListen(void)
{
	uint8_t *dest = (uint8_t *)BigBuf;
	int n = sizeof(BigBuf);
	uint8_t v = 0;
	int i;
	int p = 0;

	// We're using this mode just so that I can test it out; the simulated
	// tag mode would work just as well and be simpler.
	FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_READER_RX_XCORR | FPGA_HF_READER_RX_XCORR_848_KHZ | FPGA_HF_READER_RX_XCORR_SNOOP);

	// We need to listen to the high-frequency, peak-detected path.
	SetAdcMuxFor(GPIO_MUXSEL_HIPKD);

	FpgaSetupSsc();

	i = 0;
	for(;;) {
		if(AT91C_BASE_SSC->SSC_SR & (AT91C_SSC_TXRDY)) {
			AT91C_BASE_SSC->SSC_THR = 0xff;
		}
		if(AT91C_BASE_SSC->SSC_SR & (AT91C_SSC_RXRDY)) {
			uint8_t r = (uint8_t)AT91C_BASE_SSC->SSC_RHR;

			v <<= 1;
			if(r & 1) {
				v |= 1;
			}
			p++;

			if(p >= 8) {
				dest[i] = v;
				v = 0;
				p = 0;
				i++;

				if(i >= n) {
					break;
				}
			}
		}
	}
	DbpString("simulate tag (now type bitsamples)");
}

void ReadMem(int addr)
{
	const uint8_t *data = ((uint8_t *)addr);

	Dbprintf("%x: %02x %02x %02x %02x %02x %02x %02x %02x",
		addr, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
}

/* osimage version information is linked in */
extern struct version_information version_information;
/* bootrom version information is pointed to from _bootphase1_version_pointer */

void SendVersion(void)
{
	char temp[48]; /* Limited data payload in USB packets */
	DbpString("Prox/RFID mark3 RFID instrument");
	
	/* Try to find the bootrom version information. Expect to find a pointer at 
	 * symbol _bootphase1_version_pointer, perform slight sanity checks on the
	 * pointer, then use it.
	 */
	char *bootrom_version = *(char**)&version_information;
	
        if(0)//bootrom_version < _flash_start || bootrom_version >= _flash_end ) 
        {
		DbpString("bootrom version information appears invalid");
	} else 
        {
		FormatVersionInformation(temp, sizeof(temp), "bootrom: ", bootrom_version);
		DbpString(temp);
	}
	
	FormatVersionInformation(temp, sizeof(temp), "os: ", &version_information);
	DbpString(temp);

	FpgaGatherVersion(temp, sizeof(temp));
	DbpString(temp);
}

#ifdef WITH_LF
// samy's sniff and repeat routine
void SamyRun()
{
	DbpString("Stand-alone mode! No PC necessary.");

	// 3 possible options? no just 2 for now
#define OPTS 2

	int high[OPTS], low[OPTS];

	// Oooh pretty -- notify user we're in elite samy mode now
	LED(LED_RED,	200);
	LED(LED_ORANGE, 200);
	LED(LED_GREEN,	200);
	LED(LED_ORANGE, 200);
	LED(LED_RED,	200);
	LED(LED_ORANGE, 200);
	LED(LED_GREEN,	200);
	LED(LED_ORANGE, 200);
	LED(LED_RED,	200);

	int selected = 0;
	int playing = 0;

	// Turn on selected LED
	LED(selected + 1, 0);

	for (;;)
	{
		UsbPoll(FALSE);
		WDT_HIT();

		// Was our button held down or pressed?
		int button_pressed = BUTTON_HELD(1000);
		SpinDelay(300);

		// Button was held for a second, begin recording
		if (button_pressed > 0)
		{
			LEDsoff();
			LED(selected + 1, 0);
			LED(LED_RED2, 0);

			// record
			DbpString("Starting recording");

			// wait for button to be released
			while(BUTTON_PRESS())
				WDT_HIT();

			/* need this delay to prevent catching some weird data */
			SpinDelay(500);

			CmdHIDdemodFSK(1, &high[selected], &low[selected], 0);
			Dbprintf("Recorded %x %x %x", selected, high[selected], low[selected]);

			LEDsoff();
			LED(selected + 1, 0);
			// Finished recording

			// If we were previously playing, set playing off
			// so next button push begins playing what we recorded
			playing = 0;
		}

		// Change where to record (or begin playing)
		else if (button_pressed)
		{
			// Next option if we were previously playing
			if (playing)
				selected = (selected + 1) % OPTS;
			playing = !playing;

			LEDsoff();
			LED(selected + 1, 0);

			// Begin transmitting
			if (playing)
			{
				LED(LED_GREEN, 0);
				DbpString("Playing");
				// wait for button to be released
				while(BUTTON_PRESS())
					WDT_HIT();
				Dbprintf("%x %x %x", selected, high[selected], low[selected]);
				CmdHIDsimTAG(high[selected], low[selected], 0);
				DbpString("Done playing");
				if (BUTTON_HELD(1000) > 0)
					{
					DbpString("Exiting");
					LEDsoff();
					return;
					}

				/* We pressed a button so ignore it here with a delay */
				SpinDelay(300);

				// when done, we're done playing, move to next option
				selected = (selected + 1) % OPTS;
				playing = !playing;
				LEDsoff();
				LED(selected + 1, 0);
			}
			else
				while(BUTTON_PRESS())
					WDT_HIT();
		}
	}
}
#endif

/*
OBJECTIVE
Listen and detect an external reader. Determine the best location
for the antenna.

INSTRUCTIONS:
Inside the ListenReaderField() function, there is two mode.
By default, when you call the function, you will enter mode 1.
If you press the PM3 button one time, you will enter mode 2.
If you press the PM3 button a second time, you will exit the function.

DESCRIPTION OF MODE 1:
This mode just listens for an external reader field and lights up green
for HF and/or red for LF. This is the original mode of the detectreader
function.

DESCRIPTION OF MODE 2:
This mode will visually represent, using the LEDs, the actual strength of the
current compared to the maximum current detected. Basically, once you know
what kind of external reader is present, it will help you spot the best location to place
your antenna. You will probably not get some good results if there is a LF and a HF reader
at the same place! :-)

LIGHT SCHEME USED:
*/
static const char LIGHT_SCHEME[] = {
		0x0, /* ----     | No field detected */
		0x1, /* X---     | 14% of maximum current detected */
		0x2, /* -X--     | 29% of maximum current detected */
		0x4, /* --X-     | 43% of maximum current detected */
		0x8, /* ---X     | 57% of maximum current detected */
		0xC, /* --XX     | 71% of maximum current detected */
		0xE, /* -XXX     | 86% of maximum current detected */
		0xF, /* XXXX     | 100% of maximum current detected */
};
static const int LIGHT_LEN = sizeof(LIGHT_SCHEME)/sizeof(LIGHT_SCHEME[0]);

void ListenReaderField(int limit)
{
	int lf_av, lf_av_new, lf_baseline= 0, lf_count= 0, lf_max;
	int hf_av, hf_av_new,  hf_baseline= 0, hf_count= 0, hf_max;
	int mode=1, display_val, display_max, i;

#define LF_ONLY		1
#define HF_ONLY		2

	LEDsoff();

	lf_av=lf_max=ReadAdc(ADC_CHAN_LF);

	if(limit != HF_ONLY) {
		Dbprintf("LF 125/134 Baseline: %d", lf_av);
		lf_baseline = lf_av;
	}

	hf_av=hf_max=ReadAdc(ADC_CHAN_HF);

	if (limit != LF_ONLY) {
		Dbprintf("HF 13.56 Baseline: %d", hf_av);
		hf_baseline = hf_av;
	}

	for(;;) {
		if (BUTTON_PRESS()) {
			SpinDelay(500);
			switch (mode) {
				case 1:
					mode=2;
					DbpString("Signal Strength Mode");
					break;
				case 2:
				default:
					DbpString("Stopped");
					LEDsoff();
					return;
			}
		}
		WDT_HIT();

		if (limit != HF_ONLY) {
			if(mode==1) {
				if (abs(lf_av - lf_baseline) > 10) LED_D_ON();
				else                               LED_D_OFF();
			}
			
			++lf_count;
			lf_av_new= ReadAdc(ADC_CHAN_LF);
			// see if there's a significant change
			if(abs(lf_av - lf_av_new) > 10) {
				Dbprintf("LF 125/134 Field Change: %x %x %x", lf_av, lf_av_new, lf_count);
				lf_av = lf_av_new;
				if (lf_av > lf_max)
					lf_max = lf_av;
				lf_count= 0;
			}
		}

		if (limit != LF_ONLY) {
			if (mode == 1){
				if (abs(hf_av - hf_baseline) > 10) LED_B_ON();
				else                               LED_B_OFF();
			}
			
			++hf_count;
			hf_av_new= ReadAdc(ADC_CHAN_HF);
			// see if there's a significant change
			if(abs(hf_av - hf_av_new) > 10) {
				Dbprintf("HF 13.56 Field Change: %x %x %x", hf_av, hf_av_new, hf_count);
				hf_av = hf_av_new;
				if (hf_av > hf_max)
					hf_max = hf_av;
				hf_count= 0;
			}
		}
		
		if(mode == 2) {
			if (limit == LF_ONLY) {
				display_val = lf_av;
				display_max = lf_max;
			} else if (limit == HF_ONLY) {
				display_val = hf_av;
				display_max = hf_max;
			} else { /* Pick one at random */
				if( (hf_max - hf_baseline) > (lf_max - lf_baseline) ) {
					display_val = hf_av;
					display_max = hf_max;
				} else {
					display_val = lf_av;
					display_max = lf_max;
				}
			}
			for (i=0; i<LIGHT_LEN; i++) {
				if (display_val >= ((display_max/LIGHT_LEN)*i) && display_val <= ((display_max/LIGHT_LEN)*(i+1))) {
					if (LIGHT_SCHEME[i] & 0x1) LED_C_ON(); else LED_C_OFF();
					if (LIGHT_SCHEME[i] & 0x2) LED_A_ON(); else LED_A_OFF();
					if (LIGHT_SCHEME[i] & 0x4) LED_B_ON(); else LED_B_OFF();
					if (LIGHT_SCHEME[i] & 0x8) LED_D_ON(); else LED_D_OFF();
					break;
				}
			}
		}
	}
}

void UsbPacketReceived(uint8_t *packet, int len)
{
	UsbCommand *c = (UsbCommand *)packet;
	UsbCommand ack;
	ack.cmd = CMD_ACK;
	
	len = len;
        
	switch(c->cmd) {
#ifdef WITH_LF

		case CMD_ACQUIRE_RAW_ADC_SAMPLES_125K:
			AcquireRawAdcSamples125k((BOOL)c->arg[0]);
			UsbSendPacket((uint8_t*)&ack, sizeof(ack));
			break;
#endif

#ifdef WITH_LF
		case CMD_MOD_THEN_ACQUIRE_RAW_ADC_SAMPLES_125K:
			ModThenAcquireRawAdcSamples125k(c->arg[0],c->arg[1],c->arg[2],c->d.asBytes);
			break;
		case CMD_HID_DEMOD_FSK:
			CmdHIDdemodFSK(0, 0, 0, 1);				// Demodulate HID tag
			break;
		case CMD_HID_SIM_TAG:
			CmdHIDsimTAG(c->arg[0], c->arg[1], 1);					// Simulate HID tag by ID
			break;
		case CMD_HID_CLONE_TAG:
			CopyHIDtoT5567(c->arg[0], c->arg[1]);					// Clone HID tag by ID to T55x7
			break;
		case CMD_EM410X_WRITE_TAG:
			WriteEM410x(c->arg[0], c->arg[1], c->arg[2]);
			break;
		case CMD_READ_TI_TYPE:
			ReadTItag();
			break;
		case CMD_WRITE_TI_TYPE:
			WriteTItag(c->arg[0],c->arg[1],c->arg[2]);
			break;
		case CMD_SIMULATE_TAG_125K:
			LED_A_ON();
			SimulateTagLowFrequency(c->arg[0], c->arg[1], 1);
			LED_A_OFF();
			break;
		case CMD_LF_SIMULATE_BIDIR:
			SimulateTagLowFrequencyBidir(c->arg[0], c->arg[1]);
			break;
#endif

#ifdef WITH_ISO15693
		case CMD_ACQUIRE_RAW_ADC_SAMPLES_ISO_15693:
			AcquireRawAdcSamplesIso15693();
			break;
#endif

		case CMD_BUFF_CLEAR:
			BufferClear();
			break;

#ifdef WITH_ISO15693
		case CMD_READER_ISO_15693:
			ReaderIso15693(c->arg[0]);
			break;
#endif

		case CMD_READER_LEGIC_RF:
			LegicRfReader(c->arg[0], c->arg[1]);
			break;

#ifdef WITH_ISO15693
		case CMD_SIMTAG_ISO_15693:
			SimTagIso15693(c->arg[0]);
			break;
#endif

#ifdef WITH_ISO14443b
		case CMD_ACQUIRE_RAW_ADC_SAMPLES_ISO_14443:
			AcquireRawAdcSamplesIso14443(c->arg[0]);
			break;
#endif

#ifdef WITH_ISO14443b
		case CMD_READ_SRI512_TAG:
			ReadSRI512Iso14443(c->arg[0]);
			break;
  
		case CMD_READ_SRIX4K_TAG:
        	ReadSRIX4KIso14443(c->arg[0]);
            break;
#endif

#ifdef WITH_ISO14443a
		case CMD_READER_ISO_14443a:
			ReaderIso14443a(c, &ack);
			break;
#endif

#ifdef WITH_ISO14443a
		case CMD_READER_MIFARE:
			ReaderMifare(c->arg[0]);
			break;
#endif
      
		case CMD_MIFARE_READBL:
			MifareReadBlock(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_MIFARE_READSC:
			MifareReadSector(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_MIFARE_WRITEBL:
			MifareWriteBlock(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_MIFARE_NESTED:
			MifareNested(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_MIFARE_CHKKEYS:
			MifareChkKeys(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_SIMULATE_MIFARE_CARD:
			Mifare1ksim(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		
		// emulator
		case CMD_MIFARE_SET_DBGMODE:
			MifareSetDbgLvl(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_MIFARE_EML_MEMCLR:
			MifareEMemClr(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_MIFARE_EML_MEMSET:
			MifareEMemSet(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_MIFARE_EML_MEMGET:
			MifareEMemGet(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;
		case CMD_MIFARE_EML_CARDLOAD:
			MifareECardLoad(c->arg[0], c->arg[1], c->arg[2], c->d.asBytes);
			break;

#ifdef WITH_ISO14443b
		case CMD_SNOOP_ISO_14443:
			SnoopIso14443();
			break;
#endif

#ifdef WITH_ISO14443a
		case CMD_SNOOP_ISO_14443a:
			SnoopIso14443a();
			break;
#endif
                case CMD_MEASURE_ANTENNA_TUNING_HF:
                  MeasureAntennaTuningHf();
                  break;
		case CMD_MEASURE_ANTENNA_TUNING_LF:
		  MeasureAntennaTuningLf();
		  break;

		case CMD_SIMULATE_TAG_HF_LISTEN:
			SimulateTagHfListen();
			break;

#ifdef WITH_ISO14443b
		case CMD_SIMULATE_TAG_ISO_14443:
			SimulateIso14443Tag();
			break;
#endif
		
#ifdef WITH_ISO14443a
		case CMD_SIMULATE_TAG_ISO_14443a:
			SimulateIso14443aTag(c->arg[0], c->arg[1]);  // ## Simulate iso14443a tag - pass tag type & UID
			break;
#endif

		case CMD_MEASURE_ANTENNA_TUNING:
			MeasureAntennaTuning();
			break;

		case CMD_LISTEN_READER_FIELD:
			ListenReaderField(c->arg[0]);
			break;


		case CMD_FPGA_MAJOR_MODE_OFF:		// ## FPGA Control
			FpgaWriteConfWord(FPGA_MAJOR_MODE_OFF);
			SpinDelay(200);
			LED_D_OFF(); // LED D indicates field ON or OFF
			break;

		case CMD_PLAY:
		 	 play(song[ c->arg[0] ] ,0) ;
			break;
					 

		case CMD_DOWNLOAD_RAW_ADC_SAMPLES_125K: {
			
			UsbCommand n;
                        
			if(c->cmd == CMD_DOWNLOAD_RAW_ADC_SAMPLES_125K) {
				n.cmd = CMD_DOWNLOADED_RAW_ADC_SAMPLES_125K;
			} else {
				n.cmd = CMD_DOWNLOADED_RAW_BITS_TI_TYPE;
			}
			n.arg[0] = c->arg[0];
                        
			// check this for access, seems to be ok asDwords should be at base+16
			memcpy(
				   n.d.asDwords, 
				   BigBuf+c->arg[0], 
				   12*sizeof(DWORD));
						
			
			UsbSendPacket((BYTE *)&n, sizeof(n));
			break;
		}

		case CMD_DOWNLOADED_SIM_SAMPLES_125K: {
			uint8_t *b = (uint8_t *)BigBuf;
			memcpy(b+c->arg[0], c->d.asBytes, 48);
			UsbSendPacket((uint8_t*)&ack, sizeof(ack));
			break;
		}


		case CMD_READ_MEM:
			ReadMem(c->arg[0]);
			break;

		case CMD_SET_LF_DIVISOR:
			FpgaSendCommand(FPGA_CMD_SET_DIVISOR, c->arg[0]);
			break;

		case CMD_SET_ADC_MUX:
			switch(c->arg[0]) {
				case 0: SetAdcMuxFor(GPIO_MUXSEL_LOPKD); break;
				case 1: SetAdcMuxFor(GPIO_MUXSEL_LORAW); break;
				case 2: SetAdcMuxFor(GPIO_MUXSEL_HIPKD); break;
				case 3: SetAdcMuxFor(GPIO_MUXSEL_HIRAW); break;
			}
			break;

		case CMD_VERSION:


			SendVersion();
			//sd_init();
                       // sd_info crashes
			//sd_info();
//			TurnOff();
			break;


#ifdef WITH_LCD
		case CMD_LCD_RESET:
			lcd_reset();
			break;
		case CMD_LCD:
			lcd_send(c->arg[0]);
			break;
#endif
		case CMD_SETUP_WRITE:
		case CMD_FINISH_WRITE:
		case CMD_HARDWARE_RESET:
			USB_D_PLUS_PULLUP_OFF();
			SpinDelay(1000);
			SpinDelay(1000);
			AT91C_BASE_RSTC->RSTC_RCR = (AT91_REG)RST_CONTROL_KEY | AT91C_RSTC_PROCRST;
			for(;;) {
				// We're going to reset, and the bootrom will take control.
			}
		
		case CMD_START_FLASH:
			if(common_area.flags.bootrom_present) {
				common_area.command = COMMON_AREA_COMMAND_ENTER_FLASH_MODE;
			}
			USB_D_PLUS_PULLUP_OFF();
			AT91C_BASE_RSTC->RSTC_RCR = (AT91_REG)RST_CONTROL_KEY | AT91C_RSTC_PROCRST;
			for(;;);
	
			
		case CMD_DEVICE_INFO: {
			UsbCommand lc;
			lc.cmd = CMD_DEVICE_INFO;
			lc.arg[0] = DEVICE_INFO_FLAG_OSIMAGE_PRESENT | DEVICE_INFO_FLAG_CURRENT_MODE_OS;
			if(common_area.flags.bootrom_present) lc.arg[0] |= DEVICE_INFO_FLAG_BOOTROM_PRESENT;
			UsbSendPacket((BYTE*)&lc, sizeof(lc));
		}
			break;
		default:
			Dbprintf("%s: 0x%04x","unknown command:",c->cmd);
			break;
	}
}

void TurnOff(void)
{
	AT91C_BASE_PIOA->PIO_ODR = 0xffffffff;		// output disable reg
	AT91C_BASE_PIOA->PIO_PER = 0xffffffff;		// peripheral enable reg
	AT91C_BASE_PIOA->PIO_PPUDR = 0xffffffff;	// pullup disable reg
	AT91C_BASE_PMC->PMC_MCKR = 0x18;			// master clock, slow clock
	AT91C_BASE_PMC->PMC_MOR = 0;				// main osc disable
	AT91C_BASE_PMC->PMC_PCDR = 0xffffffff;		// peripheral clock disable reg
	AT91C_BASE_PMC->PMC_SCDR = 0xffffffff;		// system clock disable reg
	AT91C_BASE_VREG->VREG_MR = 1;				// VREG in standby
}

int CButton;

int curX=0, curY=1;
int HIDhigh=0, HIDlow=0;

void Check_Button(void)
{
	CButton=0;
	while(BUTTON_PRESS()) {
		CButton=1;
		WDT_HIT();
	}
	while(BUTTON_UP()) {
		CButton=2;
		WDT_HIT();
	}
	while(BUTTON_DOWN()) {
		CButton=3;
		WDT_HIT();
	}
	while(BUTTON_LEFT()) {
		CButton=4;
		WDT_HIT();
		if( curY == 5 ) {
		 
		  if( songi > 0 ) 
			  songi --;
		 else
		   songi = GetSong()-1;
		 
		  play(song[songi],1);
			SpinDelay(150);
		}
	}
	while(BUTTON_RIGHT()) {
		CButton=5;
		WDT_HIT();
		if( curY == 5 )  {
			
		  if(songi < GetSong() ) 
			   songi ++;
		else
		  songi = 0;
		
			play(song[songi],1);
			SpinDelay(150);
		}

	}
	LED_A_ON();
}

// Convert a hex nibble to ASCII
int h2a(BYTE b)
{
	int offset=0;
	if (b>9)
		offset=7;

	return 0x30+b+offset;
}

void PrepBuffer()
{
	int i, max, min;
	BYTE *dest = (BYTE *)BigBuf;
	int n = sizeof(BigBuf);

	max=127;
	min=127;
	// prepare the buffer, find min and max, then use average as threshold
	for (i=0; i<n; i++) {
		if (dest[i]>max) max=dest[i];
		if (dest[i]<min) min=dest[i];
	}
	min = (max + min) / 2;
	for (i=0; i<n; i++) {
		if (dest[i] > min ) dest[i]=1;
		else dest[i]=0;
	}
}

void Action_Button(void)
{
	int i;
        
	switch (CButton) {
		// Center
		case 1:
		 switch(  curY )
		 {
		 case 5:
			    lcd_setcolor(GUI_CLR_BG, BLACK);
			    lcd_gotoxy(POS(1),LINE(8));
				lcd_puts(" Play music          ");
		  		play(song[songi],0);
							
				SpinDelay(200);

			case 6:
			  MeasureAntennaTuningHf();
				SpinDelay(200);
					break;
			case 7:
			  MeasureAntennaTuningLf();
				SpinDelay(200);
					break;
		  }
			break;
		// Up
		case 2:
		PWMC_Beep(1,10000,50);
//         MeasureAntennaTuningHf();
#ifdef WITH_LCD
                lcd_setcolor(GUI_CLR_BG, GREEN);
	        lcd_gotoxy(POS(curX),LINE(curY));
			lcd_puts(" ");
	        lcd_setcolor(GUI_CLR_BG, BLACK);
	        lcd_gotoxy(POS(1),LINE(8));
			lcd_puts("                      ");
	        lcd_gotoxy(POS(1),LINE(9));
			lcd_puts("                      ");
			
			if(curY<2)
			  curY=MAX_MENU;
			else 
			  curY--;
			
	        lcd_setcolor(GUI_CLR_BG, GREEN);
	        lcd_gotoxy(POS(curX),LINE(curY));
			lcd_puts("*");
#endif
			break;
		// Down
		case 3:
			PWMC_Beep(1,10000,50);
#ifdef WITH_LCD
	        lcd_setcolor(GUI_CLR_BG, GREEN);
	        lcd_gotoxy(POS(curX),LINE(curY));
			lcd_puts("  ");
	        lcd_setcolor(GUI_CLR_BG, BLACK);
	        lcd_gotoxy(POS(1),LINE(8));
			lcd_puts("                      ");
	        lcd_gotoxy(POS(1),LINE(9));
			lcd_puts("                      ");

			if(curY>MAX_MENU)
			  curY=1;
			else
			  curY++;

	        lcd_setcolor(GUI_CLR_BG, GREEN);
	        lcd_gotoxy(POS(curX),LINE(curY));
			lcd_puts("*");
#endif
			break;
		// Left
		case 4:
		// Right
		case 5:
			
		  PWMC_Beep(1,10000,50);
			
		  switch (curY) {
				case 1:
#ifdef WITH_LCD
			        lcd_setcolor(GUI_CLR_BG, BLACK);
			        lcd_gotoxy(POS(1),LINE(8));
					lcd_puts(" Recording HID tag... ");
#endif
					CmdHIDdemodFSK(1, &HIDhigh, &HIDlow, 0);
#ifdef WITH_LCD
			        lcd_gotoxy(POS(1),LINE(9));
					lcd_puts(" TAG:                 ");
			        lcd_gotoxy(POS(5),LINE(9));
					for (i=0; i<8; i++)
					{
						lcd_putchar(h2a((HIDhigh>>((7-i)*4))&0xf));
					}
					for (i=0; i<8; i++)
					{
						lcd_putchar(h2a((HIDlow>>((7-i)*4))&0xf));
					}
#endif
					break;
				case 2:
#ifdef WITH_LCD
			        lcd_setcolor(GUI_CLR_BG, BLACK);
			        lcd_gotoxy(POS(1),LINE(8));
					lcd_puts(" Replaying HID tag... ");
#endif
					CmdHIDsimTAG(HIDhigh, HIDlow, 0);
					break;
				case 3:
#ifdef WITH_LCD
			        lcd_setcolor(GUI_CLR_BG, BLACK);
			        lcd_gotoxy(POS(1),LINE(8));
					lcd_puts(" Recording raw tag... ");
#endif
					AcquireRawAdcSamples125k((BOOL)0);
					PrepBuffer();
					break;
				case 4:
#ifdef WITH_LCD
			        lcd_setcolor(GUI_CLR_BG, BLACK);
			        lcd_gotoxy(POS(1),LINE(8));
					lcd_puts(" Replaying raw tag... ");
#endif
					SimulateTagLowFrequency(sizeof(BigBuf), 0,1);
					break;
			case 5:
					break;
			}

			break;
		default:
			break;
	}

}
void BootROM(void);
// Philips PCF8833 LCD controller command codes
#define NOP 0x00 // nop
#define SWRESET 0x01 // software reset
#define BSTROFF 0x02 // booster voltage OFF
#define BSTRON 0x03 // booster voltage ON
#define RDDIDIF 0x04 // read display identification
#define RDDST 0x09 // read display status
#define SLEEPIN 0x10 // sleep in
#define SLEEPOUT 0x11 // sleep out
#define PTLON 0x12 // partial display mode
#define NORON 0x13 // display normal mode
#define INVOFF 0x20 // inversion OFF
#define INVON 0x21 // inversion ON
#define DALO 0x22 // all pixel OFF
#define DAL 0x23 // all pixel ON
#define SETCON 0x25 // write contrast
#define DISPOFF 0x28 // display OFF
#define DISPON 0x29 // display ON
#define CASET 0x2A // column address set
#define PASET 0x2B // page address set
#define RAMWR 0x2C // memory write
#define RGBSET 0x2D // colour set
#define PTLAR 0x30 // partial area
#define VSCRDEF 0x33 // vertical scrolling definition
#define TEOFF 0x34 // test mode
#define TEON 0x35 // test mode
#define MADCTL 0x36 // memory access control
#define SEP 0x37 // vertical scrolling start address
#define IDMOFF 0x38 // idle mode OFF
#define IDMON 0x39 // idle mode ON
#define COLMOD 0x3A // interface pixel format
#define SETVOP 0xB0 // set Vop
#define BRS 0xB4 // bottom row swap
#define TRS 0xB6 // top row swap
#define DISCTR 0xB9 // display control
#define DOR 0xBA // data order
#define TCDFE 0xBD // enable/disable DF temperature compensation
#define TCVOPE 0xBF // enable/disable Vop temp comp
#define EC 0xC0 // internal or external oscillator
#define SETMUL 0xC2 // set multiplication factor
#define TCVOPAB 0xC3 // set TCVOP slopes A and B
#define TCVOPCD 0xC4 // set TCVOP slopes c and d
#define TCDF 0xC5 // set divider frequency
#define DF8COLOR 0xC6 // set divider frequency 8-color mode
#define SETBS 0xC7 // set bias system
#define RDTEMP 0xC8 // temperature read back
#define NLI 0xC9 // n-line inversion

//-----------------------------------------------------------------------------
// Set up the Serial Peripheral Interface as master
// Used to talk to the attached peripherals (FPGA, LCD, uSD)
//-----------------------------------------------------------------------------
void testSetupSpi(int mode)
{
  mode = mode ;
  
	// PA5  -> SPI_NCS3 chip select (spare)
	// PA9  -> SPI_NCS1 chip select (microSD)
	// PA10 -> SPI_NCS2 chip select (LCD)
	// PA11 -> SPI_NCS0 chip select (FPGA)
	// PA12 -> SPI_MISO Master-In Slave-Out
	// PA13 -> SPI_MOSI Master-Out Slave-In
	// PA14 -> SPI_SPCK Serial Clock

	// Disable PIO control of the following pins, allows use by the SPI peripheral
	AT91C_BASE_PIOA->PIO_PDR =
			GPIO_NCS0	|
			GPIO_NCS1 	|
			GPIO_NCS2 	|
			GPIO_NCS3 	|
			GPIO_MISO	|
			GPIO_MOSI	|
			GPIO_SPCK;

	AT91C_BASE_PIOA->PIO_ASR =
			GPIO_NCS0	|
			GPIO_MISO	|
			GPIO_MOSI	|
			GPIO_SPCK;

	AT91C_BASE_PIOA->PIO_BSR =
			GPIO_NCS1	|
			GPIO_NCS2	|
			GPIO_NCS3;

	//enable the SPI Peripheral clock
	AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_SPI);
	// Enable SPI
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;


			AT91C_BASE_SPI->SPI_MR =
				(11 << 16)			|	// Peripheral Chip Select (selects LCD SPI_NCS2 or PA10)
				AT91C_SPI_MODFDIS	|	// Mode Fault Detection disabled
				AT91C_SPI_MSTR;			// Master Mode
				AT91C_BASE_SPI->SPI_CSR[2] =
				( 1 << 24)			|	// Delay between Consecutive Transfers (32 MCK periods)
				( 1 << 16)			|	// Delay Before SPCK (1 MCK period)
				( 6 << 8)			|	// Serial Clock Baud Rate (baudrate = MCK/6 = 24Mhz/6 = 4M baud
				AT91C_SPI_BITS_9	|	// Bits per Transfer (9 bits)
				AT91C_SPI_NCPHA;		// Clock Phase data captured on leading edge, changes on following edge
}

//-----------------------------------------------------------------------------
// Transmit data over the SPI then receive any incoming data.
// Data can be variable length (8 - 16 bits)
 //-----------------------------------------------------------------------------
void WriteSpiCommand( unsigned int command )
{
      SetupSpi(SPI_LCD_MODE);		// enable relevant channel

    while ( !( AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TDRE ) ); // wait for SPI ready state
    // clear bit 8 - indicates a "command"
    command = (command & (unsigned int)~0x0100);
    AT91C_BASE_SPI->SPI_TDR = command;	// send data


}

void WriteSpiData( unsigned int data)
{

      SetupSpi(SPI_LCD_MODE);		// enable relevant channel

    while ( !( AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TDRE ) ); // wait for SPI ready state
  // set bit 8, indicates "data"
  data = (data | 0x0100);

    AT91C_BASE_SPI->SPI_TDR = data;	// send data


}

int testmain(void)
{
	if(common_area.magic != COMMON_AREA_MAGIC || common_area.version != 1) {
		/* Initialize common area */
		memset(&common_area, 0, sizeof(common_area));
		common_area.magic = COMMON_AREA_MAGIC;
		common_area.version = 1;
	}
	common_area.flags.osimage_present = 1;

	LED_D_OFF();
	LED_C_OFF();
	LED_B_OFF();
	LED_A_OFF();

	UsbStart();

	// The FPGA gets its clock from us from PCK0 output, so set that up.
	AT91C_BASE_PIOA->PIO_BSR = GPIO_PCK0;
	AT91C_BASE_PIOA->PIO_PDR = GPIO_PCK0;
	AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK0;
	
	// PCK0 is PLL clock / 4 = 96Mhz / 4 = 24Mhz
	AT91C_BASE_PMC->PMC_PCKR[0] = AT91C_PMC_CSS_PLL_CLK | AT91C_PMC_PRES_CLK_4;
	AT91C_BASE_PIOA->PIO_OER = GPIO_PCK0;

	// Reset SPI
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SWRST;
	
	// Reset SSC
	//charlie
	//AT91C_BASE_SSC->SSC_CR = AT91C_SSC_SWRST;
	
	//charlie
	AT91C_BASE_SSC->SSC_CR = AT91C_SSC_SWRST | AT91C_SSC_RXDIS | AT91C_SSC_TXDIS ;

	// Load the FPGA image, which we have stored in our flash.
	FpgaDownloadAndGo();

//  BufferClear();

		
                        
#ifdef WITH_LCD
 
    LED_A_ON();
    
    lcd_init();
    
    lcd_setcolor(GUI_CLR_BG, GUI_CLR_TXT);
    lcd_clear();
    
    // test text on different colored backgrounds
    lcd_setcolor(GUI_CLR_BG, GREEN);
    lcd_gotoxy(POS(0),LINE(1));
    lcd_puts("* Record HID tag     ");
    lcd_setcolor(GUI_CLR_BG, RED);
    lcd_gotoxy(POS(0),LINE(2));
    lcd_puts("  Replay HID tag     ");
    lcd_setcolor(GUI_CLR_BG, YELLOW);
    lcd_gotoxy(POS(0),LINE(3));
    lcd_puts("  Record raw tag     ");
    lcd_setcolor(GUI_CLR_BG, BLUE);
    lcd_gotoxy(POS(0),LINE(4));
    lcd_puts("  Replay raw tag     ");
    lcd_gotoxy(POS(0),LINE(5));
    lcd_puts("  Music Playback     ");
    lcd_gotoxy(POS(0),LINE(6));
    lcd_puts("  Tune HF Antenna    ");
    lcd_gotoxy(POS(0),LINE(7));
    lcd_puts("  Tune LF Antenna    ");

#endif


           
#if 0
	        int i;
                volatile int j ;

		for(i=0;i<100;i++)  {
			j += ReadAdc(ADC_CHAN_LF);
		}
		
	
		i = 0;
		
		srand(j);
		j = rand()% (sizeof(song) / sizeof(song[0])) ;
	
		play(song[j],0);
		
#endif

		//play(song[63],0);
		
	for(;;) {
                
		UsbPoll(FALSE);
		Check_Button();
		WDT_HIT();
		Action_Button();
		WDT_HIT();
		
	}
}

