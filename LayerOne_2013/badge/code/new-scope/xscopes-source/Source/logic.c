/*****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
November 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#include <stdint.h>
#include <avr/interrupt.h>
#include "logic.h"
#include "main.h"
#include "mso.h"
#include "USB\usb_xmega.h"

//    0   1   2   3   4   5   6   7
//  |...|...|...|...|...|...|...|...|
//   ^
//   |
//   sample bit 3 times
//
// Display the HEX value of the digital stream
void HEXSerial(void) {
    int8_t start,end,i;
    uint8_t data,d=0,temp5,temp6,temp7;
    int16_t increment,counter;
    uint8_t result=0;
    // The vertical cursors define the section to decode
    start = M.VcursorA;
    end   = M.VcursorB;
    // If the vertical cursors are off, use the entire screen
    if(!testbit(Mcursors, cursorv)) {
        start = 0;
        end   = 127;
    }
    increment = (end-start);
    if(start>end) {
        increment = -(start-end);
    }
	increment<<=3;
    if(Srate>=11) {
        increment*=2;
        start=start<<1;
    }
    temp5 = M.CHDpos;
    for(temp6=128, temp7=7; temp6; temp6=temp6>>1, temp7--) {
        counter = (start<<8);
        if(Srate<11) counter+=(M.HPos<<8);
        counter+=increment;
        if(CHDmask&temp6) {
            for(i=1; i<=32; i++) {
                data = D.CHDdata[hibyte(counter)];
                if(!(i&0x03)) { // Time to check bits when i is multiple of 4
                    result<<=1;
                    if(d>1) result+=1;
                    d=0;
                }
                else {
                    if(testbit(data,temp7)) d++;
                }
                counter+=increment;
            }
            lcd_goto(120,temp5>>3);
            printhex(result);
            temp5+=8;
        }
    }
}

const uint8_t BAUDA[8] PROGMEM = {
    3,      // 1200
    1,      // 2400
    253,    // 4800
    245,    // 9600
    229,    // 19200
    197,    // 38400
    110,    // 57600
    23,      // 115200
};

const uint8_t BAUDB[8] PROGMEM = {
    253,    // 1200
    237,    // 2400
    220,    // 4800
    204,    // 9600
    188,    // 19200
    172,    // 38400
    168,    // 57600
    164     // 115200
};

const uint16_t TCC1val[8] PROGMEM = {
    26665,  // 1200
    13332,  // 2400
    6665,   // 4800
    3332,   // 9600
    1666,   // 19200
    832,    // 38400
    554,    // 57600
    277,    // 115200
};

void LogicDMA(void) {
    uint8_t dummy;
    setbit(DMA.CH0.CTRLA,6);    // reset DMA CH0
    // Flush receive buffer
    while(testbit(USARTC0.STATUS,7)) {
         dummy=USARTC0.DATA;
    }
    DMA.CH0.ADDRCTRL  = 0b10000101;     // reload source addr after each burst, incr dest, reload dest end block
    DMA.CH0.TRFCNT    = 640;            // buffer size
    DMA.CH0.DESTADDR0 = (((uint16_t) Temp.LOGIC.data.Serial.RX)>>0*8) & 0xFF;
    DMA.CH0.DESTADDR1 = (((uint16_t) Temp.LOGIC.data.Serial.RX)>>1*8) & 0xFF;
    DMA.CH0.DESTADDR2 = 0;
    if(M.CHDdecode==rs232) {
        DMA.CH0.TRIGSRC   = 0x4B;               // USARTC0 is trigger source
        DMA.CH0.SRCADDR0  = (((uint16_t)(&USARTC0.DATA))>>0*8) & 0xFF;
        DMA.CH0.SRCADDR1  = (((uint16_t)(&USARTC0.DATA))>>1*8) & 0xFF;
        DMA.CH0.SRCADDR2  = 0;
    }
    else if(M.CHDdecode==spi) {
        DMA.CH0.TRIGSRC   = 0x4A;               // SPIC is trigger source
        DMA.CH0.SRCADDR0  = (((uint16_t)(&SPIC.DATA))>>0*8) & 0xFF;
        DMA.CH0.SRCADDR1  = (((uint16_t)(&SPIC.DATA))>>1*8) & 0xFF;
        DMA.CH0.SRCADDR2  = 0;
    }
    if(testbit(Mcursors, singlesniff)) DMA.CH0.CTRLA     = 0b10000100;    // no repeat, 1 byte burst
    else DMA.CH0.CTRLA     = 0b10100100;                            // repeat, 1 byte burst
}

// Protocol Sniffer
void Sniff(void) {
    uint16_t i;
    uint8_t n,addrdata, ack, data;
    uint8_t page=0; // 16 pages to display 1024 bytes of data
                    // one page has 8 lines, each line will display
                    // 8 bytes in this format: "FF+ "
    uint8_t *p=Temp.LOGIC.data.All.decoded;
    clr_display();
    for(i=0; i<1282; i++) *p++=0;   // Erase all data
    Temp.LOGIC.indrx=0; Temp.LOGIC.indtx=0;
    if(testbit(Trigger, round)) page=0x0F;
    // Setup
    PR.PRPC  = 0x00;    // Enable PORTC peripherals
    if(M.CHDdecode==i2c) {
        lcd_putsp(PSTR("I2C SNIFFER\nBIT0:SDA BIT1:SCL"));
        Temp.LOGIC.addr_ack_ptr =  Temp.LOGIC.data.I2C.addr_ack;
        Temp.LOGIC.data_ptr = Temp.LOGIC.data.I2C.decoded;
        Temp.LOGIC.addr_ack_pos = 0x80;      // counter for keeping track of all bits
        PORTC.PIN0CTRL = (PORTC.PIN0CTRL & 0b11111000) | 0b00000010;    // Sense falling edge of SDA
        PORTC.INTFLAGS = 0x03;  // Clear flags
        PORTC.INTCTRL   = 0x02; // Enable PortC INT 0
    }
    else if(M.CHDdecode==rs232) {
        lcd_putsp(PSTR("UART SNIFFER\nBIT2:RX BIT3:TX"));
        // Initialize USART for RX
        uint8_t baud;
        baud=Sniffer&0x07;
        USARTC0.BAUDCTRLA = pgm_read_byte_near(BAUDA+baud);
        USARTC0.BAUDCTRLB = pgm_read_byte_near(BAUDB+baud);
        USARTC0.CTRLC = 0x03;       // Async, No Parity, 1 stop bit, 8 data bits
        if(testbit(Sniffer,parmode)) {     // Check parity
            if(testbit(Sniffer,parity)) USARTC0.CTRLC |= 0x30;
            else USARTC0.CTRLC |= 0x20;
        }
        if(testbit(Sniffer,stopbit)) USARTC0.CTRLC |= 0x08;    // Second stop bit
        USARTC0.CTRLB = 0x10;       // Enable RX
        TCC1.CTRLA = 0x01;  // DIV1
        Temp.LOGIC.baud = (uint16_t)pgm_read_word_near(TCC1val+baud);
        TCC1.PER = Temp.LOGIC.baud/4;   // Sample at 4 times the baud rate
        TCC1.INTCTRLA = 0x02;           // Enable TCC1 interrupt
    }
    else if(M.CHDdecode==spi) {
        lcd_putsp(PSTR("SPI SNIFFER\nBIT4:/SS\nBIT5:MOSI\nBIT6:MISO\nBIT7:SCK"));
        PORTC.INTFLAGS = 0x03;  // Clear flags
        PORTC.INTCTRL   = 0x08; // Enable PortC INT 1
        i=0;
        if(testbit(Sniffer,CPOL)) {    // CPOL == 1
            PORTC.PIN7CTRL |= PORT_INVEN_bm;
            i=0x08;
        }
        else {                  // CPOL == 0
            PORTC.PIN7CTRL &= ~PORT_INVEN_bm;
        }
        if(testbit(Sniffer,CPHA)) i+=0x04; // CPHA == 1
        SPIC.CTRL = 0b01000000 + i;
    }
    LogicDMA();
    show_display(); WaitDisplay();
    while((Temp.LOGIC.indrx==0) && (Temp.LOGIC.indtx==0)) {
        if(!testbit(MStatus,gosniffer)) goto exit;
        if (testbit(Key,KD)) clrbit(MStatus,gosniffer);
        if(M.CHDdecode!=i2c) Temp.LOGIC.indrx=640-DMA.CH0.TRFCNT;
    }
////////////////////////////////////////////////////////////////////////////////////
// DISPLAY
////////////////////////////////////////////////////////////////////////////////////
    do {
        show_display(); WaitDisplay();
        clr_display();
		// USB - Send new data if previous transfer complete
		if((endpoints[1].in.STATUS & USB_EP_TRNCOMPL0_bm)) {
    		endpoints[1].in.AUXDATA = 0;				// New transfer must clear AUXDATA
            if(!testbit(Misc,sacquired)) {
               	endpoints[1].in.DATAPTR = (uint16_t)Temp.LOGIC.data.All.decoded;
        		endpoints[1].in.CNT = 640;	    // Send first half
            }
            else {
               	endpoints[1].in.DATAPTR = (uint16_t)Temp.LOGIC.data.All.decoded+640;
        		endpoints[1].in.CNT = 649;	    // Send second half
            }
    		endpoints[1].in.STATUS &= ~(USB_EP_TRNCOMPL0_bm | USB_EP_BUSNACK0_bm | USB_EP_OVF_bm);
            togglebit(Misc,sacquired);
		}
		WaitRefresh();
        // Display I2C data
        if(M.CHDdecode==i2c) {
            if(testbit(Trigger, round)) {
                cli();
                if(Temp.LOGIC.indrx>=64) {
                    i=Temp.LOGIC.indrx-64;
                }
                else {
                    i=(1024-64)+Temp.LOGIC.indrx;
                }
                sei();
                i+=(uint16_t)(page+1)*64;
                if(i>=1024) i-=1024;
            }
            else {
                i=(uint16_t)page*64;
            }
            for(n=0; n<64; n++) {
                if(!testbit(Trigger, round)) {
                    if(i>=Temp.LOGIC.indrx) break;
                }
                uint8_t shift;
                shift = (i&0x0003)*2;
                data=Temp.LOGIC.data.I2C.decoded[i];
                addrdata=Temp.LOGIC.data.I2C.addr_ack[i/4];
                ack=      (addrdata<<(shift+1))&0x80;
                addrdata= (addrdata<<shift)&0x80;
                if(addrdata) {  // Address
                    printhex(data>>1);
                    if(testbit(data,0)) {   // Read
                        if(ack) GLCD_Putchar('<');  // Ack
                        else    GLCD_Putchar('(');  // No Ack
                    }
                    else {                  // Write
                        if(ack) GLCD_Putchar('>');  // Ack
                        else    GLCD_Putchar(')');  // No Ack
                    }
                }
                else {          // Data
                    printhex(data);
                    if(ack) GLCD_Putchar('+');  // Ack
                    else    GLCD_Putchar('-');  // No Ack
                }
                GLCD_Putchar(' ');
                i++;
                if(i>=1024) i=0;
            }
        }
        // Display UART or SPI data
        else {
            Temp.LOGIC.indrx=640-DMA.CH0.TRFCNT;
            if(testbit(Mcursors, singlesniff)) {  // DMA single transfer complete
                if(!testbit(DMA.CH0.CTRLB,7)) {
                    DMA.CH0.TRFCNT = 0;
                    setbit(MStatus, stop);
                    USARTC0.CTRLB = 0x00;       // Disable RX
                    clrbit(SPIC.CTRL,6);        // Disable SPI
                }
            }
            if(testbit(MStatus, stop)) {
                lcd_line(64,8,64,30);
                lcd_goto(63,4); GLCD_Putchar('S');
                lcd_goto(63,5); GLCD_Putchar('T');
                lcd_goto(63,6); GLCD_Putchar('O');
                lcd_goto(63,7); GLCD_Putchar('P');
            }
            else lcd_line(64,8,64,64);
            lcd_goto(63,0); GLCD_Putchar(NibbleToChar(page));
            if(M.CHDdecode==spi) {
                lcd_goto(60,1); GLCD_Putchar('I');
                lcd_goto(66,1); GLCD_Putchar('O');
            }
            else {
                lcd_goto(60,1); GLCD_Putchar('R');
                lcd_goto(66,1); GLCD_Putchar('T');
            }
            // Display RX or SDI data
            lcd_goto(1,0);
            i=(uint16_t)page*40;
            if(testbit(Trigger, round)) {
                if(Temp.LOGIC.indrx>=40) {
                    i=Temp.LOGIC.indrx-40;
                }
                else {
                    i=(640-40)+Temp.LOGIC.indrx;
                }
                i+=(uint16_t)(page+1)*40;
                if(i>=640) i-=640;
            }
            for(n=0; n<40; n++) {
                if(!testbit(Trigger, round)) {
                    // cli ?
                    if(i>=Temp.LOGIC.indrx) break;
                }
                data=Temp.LOGIC.data.Serial.RX[i];
                if(testbit(Sniffer,ascii)) {
                    if(data<0x20) GLCD_Putchar('_');    // Special character
                    else GLCD_Putchar(data);
                    GLCD_Putchar(' ');
                }
                else printhex(data);
                GLCD_Putchar(' ');
                if(u8CursorX>=60) {    // Next line
                    u8CursorX = 1; u8CursorY++;
                }
                i++;
                if(i>=640) i=0;
            }
            lcd_goto(72,0);
            i=(uint16_t)page*40;
            if(testbit(Trigger, round)) {
                cli();
                if(Temp.LOGIC.indtx>=40) {
                    i=Temp.LOGIC.indtx-40;
                }
                else {
                    i=(640-40)+Temp.LOGIC.indtx;
                }
                sei();
                i+=(uint16_t)(page+1)*40;
                if(i>=640) i-=640;
            }
            // Display TX or SDO data
            for(n=0; n<40; n++) {
                if(!testbit(Trigger, round)) {
                    if(i>=Temp.LOGIC.indtx) break;
                }
                data=Temp.LOGIC.data.Serial.TX[i];
                if(testbit(Sniffer,ascii)) {
                    if(data<0x20) GLCD_Putchar('_');    // Special character
                    else GLCD_Putchar(data);
                    GLCD_Putchar(' ');
                }
                else printhex(data);
                GLCD_Putchar(' ');
                if(u8CursorX<72) {    // Next line
                    u8CursorX = 72;
                }
                i++;
                if(i>=640) i=0;
            }
        }
        if(testbit(Key, userinput)) {
            clrbit(Key, userinput);
            if (testbit(Key,KB) && testbit(Key,KC)) togglebit(Sniffer,ascii);
            if (testbit(Key,KA)) {
                togglebit(MStatus, stop);
                if(testbit(MStatus, stop)) {    // Stop sniffer
                    USARTC0.CTRLB = 0x00;       // Disable RX
                    clrbit(SPIC.CTRL,6);        // Disable SPI
                    TCC1.INTCTRLA = 0x00;       // Disable TCC1 interrupt
                    PORTC.INTCTRL = 0x00;       // Disable PORTC interrupts
                }
                else {                          // Restart Sniffer
                    if(M.CHDdecode==i2c) {
                        PORTC.INTCTRL   = 0x02;     // Enable PortC INT 0
                    }
                    else if(M.CHDdecode==rs232) {
                        TCC1.INTCTRLA = 0x02;       // Enable TCC1 interrupt
                        USARTC0.CTRLB = 0x10;       // Enable RX
                    }
                    else if(M.CHDdecode==spi) {
                        PORTC.INTCTRL   = 0x08;     // Enable PortC INT 1
                        setbit(SPIC.CTRL,6);        // Enable SPI
                    }
                    if(testbit(Mcursors, singlesniff)) {  // Restart DMA single transfer
                        if(testbit(DMA.CH0.CTRLB,4)) {
                            LogicDMA();
                        }
                    }

                }
            }
            if (testbit(Key,KB)) if(page)    page--;
            if (testbit(Key,KC)) if(page<15) page++;
            if (testbit(Key,KD)) clrbit(MStatus,gosniffer);  // Exit sniffer
        }
    } while (testbit(MStatus,gosniffer));
exit:
    Key=0;
   	endpoints[1].in.DATAPTR = (uint16_t)D.CH1data;
	endpoints[1].in.STATUS = USB_EP_BUSNACK0_bm | USB_EP_TRNCOMPL0_bm;
    clrbit(Misc,sacquired);
    USARTC0.CTRLA = 0x00;   // Disable receive interrupt
    USARTC0.CTRLB = 0x00;   // Disable UART RX
    TCC1.INTCTRLA = 0x00;   // Disable timer interrupt
    TCC1.CTRLA = 0x00;      // Disable timer
    PORTC.INTCTRL = 0x00;   // Disable PORTC interrupts
    SPIC.CTRL = 0x00;       // Disable SPI
    PR.PRPC  = 0x7C;        // Stop: TWI, USART0, USART1, SPI, HIRES
}

#define SDA_PIN 0
#define SCL_PIN 1
#define SCL (unsigned char) (VPORT2.IN & _BV(SCL_PIN))
#define SDA (unsigned char) (VPORT2.IN & _BV(SDA_PIN))

// Decode I2C (TWI) protocol
// Temp.LOGIC.data.I2C.decoded[1024] holds data
// Temp.LOGIC.data.I2C.addr[128]     hold if the byte is address or data
// Temp.LOGIC.data.I2C.ack[128]      hold if the byte had an ack
// Interrupt on falling edge of SDA
ISR(PORTC_INT0_vect) {
    uint8_t data;       // byte to accumulate data information
    uint8_t bits;       // current bit being processed
    uint8_t first=1;    // First data after start
    uint8_t *dataptr=Temp.LOGIC.data_ptr;           // temp data pointer
    uint8_t *bitsptr=Temp.LOGIC.addr_ack_ptr;       // temp bits pointer
    uint8_t current_pos=Temp.LOGIC.addr_ack_pos;    // Current bit position in byte
    const uint8_t *queue_max=Temp.LOGIC.data.I2C.decoded+1024;
    TCC0.CNTL = 24;
    setbit(TCC0.INTFLAGS, TC2_LUNFIF_bp);    // Clear trigger timeout interrupt

repeated_start:
    bits = 0;
    // wait for SCL to fall
    while(SCL) if(testbit(TCC0.INTFLAGS, TC2_LUNFIF_bp)) goto exiti2c;
sample_bit:    // now ready for next bit of the byte
    while(SCL == 0) if(testbit(TCC0.INTFLAGS, TC2_LUNFIF_bp)) goto exiti2c;  // wait for rising edge
    // update buffers here
    if (dataptr == queue_max) goto queue_is_full;
    uint8_t dt = SDA;   // Sampling SDA on the rising edge of SCL
    // we might not use this data in the cases of either stop or repeated start conditions
    // SCL is now high, wait for either of two conditions: SCL falls or SDA changes
    // split the cases to reduce the sub-loop overheads
    if (dt == 0) {
        // special case where stop condition might exist because data was zero
look_for_bit_or_stop:
        if(testbit(TCC0.INTFLAGS, TC2_LUNFIF_bp)) goto exiti2c;    // timeout
        if (SCL == 0) goto process_bit;
        if (SDA == 0) goto look_for_bit_or_stop;
        if (SCL == 0) goto process_bit; // double check, in case both signals changed at the same time
        // SCL and SDA are now high (stop condition occurred)
        goto exiti2c;
    }
    else {
        // special case where we might have a repeated start where SDA falls while SCL high
look_for_bit_or_repeated_start:
        if(testbit(TCC0.INTFLAGS, TC2_LUNFIF_bp)) goto exiti2c;    // timeout
        if (SCL == 0) goto process_bit;
        if (SDA) goto look_for_bit_or_repeated_start;
        if (SCL == 0) goto process_bit; // double check, in case both signals changed at the same time
        first=1;
        goto repeated_start;
    }
process_bit:
    // process another data bit
    if (bits == 8) {
        bits = 0;  // next SCL rise starts a new byte of data
        // looking at the acknowledge bit
        *dataptr++ = data;  // store the data

        // addr/data bits and acknowledge bits are accumulated into a byte to save memory
        // so, there are 4 addr/data and 4 ack/nack bits before we fill 1 byte
        uint8_t bits;
        bits=*bitsptr;

        if(first) bits |= current_pos;
        current_pos = current_pos>>1;
        if(!dt) bits |= current_pos;   // Ack by pulling line low
        current_pos = current_pos>>1;
        *bitsptr=bits;
        if (current_pos == 0) {  // 8 bits done
            bitsptr++;
            *bitsptr=0;
            current_pos = 0x80;
        }
        first=0;    // subsequent bytes are data bytes
    }
    else {
        // accumulate bits of the byte's information (MSB to LSB)
        data = (data << 1) | dt;
        bits++;
    }
    goto sample_bit;

    // the program comes here when the memory is full
queue_is_full:
    // If single trigger, now go to stop
    if(testbit(Mcursors, singlesniff)) {
        PORTC.INTCTRL = 0x00;   // Disable PORTC interrupts
        setbit(MStatus, stop);
        Temp.LOGIC.indrx=1024;
        goto exiti2cb;
    }
    bitsptr=Temp.LOGIC.data.I2C.addr_ack;
    *bitsptr=0;
    dataptr=Temp.LOGIC.data.I2C.decoded;   // pointer to data
    goto sample_bit;
    
exiti2c:
    Temp.LOGIC.indrx=dataptr-Temp.LOGIC.data.I2C.decoded;
exiti2cb:    
    Temp.LOGIC.data_ptr=dataptr;
    Temp.LOGIC.addr_ack_ptr=bitsptr;
    Temp.LOGIC.addr_ack_pos=current_pos;
    PORTC.INTFLAGS = 0x01;  // Clear rising edge interrupt
}

// Decode SPI protocol
// Temp.decoded: holds data
ISR(PORTC_INT1_vect) {
    uint8_t data, i=8;

    TCC0.CNTL = 24;
    setbit(TCC0.INTFLAGS, TC2_LUNFIF_bp);    // Clear trigger timeout interrupt

    if(testbit(Sniffer,CPHA)) {    // CPHA == 1
        do {
            // wait for clock rising
            while(!testbit(VPORT2.IN, 7)) if(testbit(TCC0.INTFLAGS, TC2_LUNFIF_bp)) return;
            data=data<<1;
            // wait for clock falling
            while(testbit(VPORT2.IN, 7)) if(testbit(TCC0.INTFLAGS, TC2_LUNFIF_bp)) return;
            if(testbit(VPORT2.IN, 6)) data|=0x01; // Read MISO
        } while(--i);

    }
    else {                  // CPHA == 0
        do {
            // wait for clock rising
            while(!testbit(VPORT2.IN, 7)) if(testbit(TCC0.INTFLAGS, TC2_LUNFIF_bp)) return;
            data=data<<1;
            if(testbit(VPORT2.IN, 6)) data|=0x01; // Read MISO
            // wait for clock falling
            while(testbit(VPORT2.IN, 7)) if(testbit(TCC0.INTFLAGS, TC2_LUNFIF_bp)) return;
        } while(--i);
    }
    PORTC.INTFLAGS = 0x02;  // Clear rising edge interrupt
    // Save data on buffer
    if(Temp.LOGIC.indtx>=640) Temp.LOGIC.indtx=0;
    Temp.LOGIC.data.Serial.TX[Temp.LOGIC.indtx++]=data;
    if(Temp.LOGIC.indtx>=640 && testbit(Mcursors, singlesniff)) {
        PORTC.INTCTRL   = 0x02; // Disable PortC INT 0
        setbit(MStatus, stop);
    }
}

// Bit banging to decode the TX pin:
// The interrupt monitors the TX line at four times the baud rate,
// when the start bit is sensed, the timer period is set to the baud rate
// then all the incoming characters are processed inside the interrupt.
// Before exiting, the timer is set to 4 times the baud rate again.
ISR(TCC1_OVF_vect) {
    uint8_t i, data=0, paritybit=0;

    // Start Bit
    if(testbit(VPORT2.IN, 3)) return;
    TCC1.INTFLAGS = 0x01;   // Clear overflow flag
    while(!testbit(TCC1.INTFLAGS,0));   // wait for next time slice
    // Set actual baud rate
    TCC1.PER = Temp.LOGIC.baud;

    // Data bits
    for(i=0; i<8; i++) {
        TCC1.INTFLAGS = 0x01;   // Clear overflow flag
        while(!testbit(TCC1.INTFLAGS,0));   // wait for next time bit
        data=data>>1;
        if(testbit(VPORT2.IN, 3)) {
            data|=0x80;
            paritybit++;
        }
    }

    // Check Parity
    if(testbit(Sniffer,parmode)) {
        TCC1.INTFLAGS = 0x01;   // Clear overflow flag
        while(!testbit(TCC1.INTFLAGS,0));   // wait for next time bit
        if(testbit(Sniffer,parity)) {  // Odd Parity
            if( (testbit(paritybit,0) && testbit(VPORT2.IN, 3)) ||
                (!testbit(paritybit,0) && !testbit(VPORT2.IN, 3)))
                goto exituart;   // Parity error
        }
        else {                  // Even Parity
            if( (!testbit(paritybit,0) && testbit(VPORT2.IN, 3)) ||
                (testbit(paritybit,0) && !testbit(VPORT2.IN, 3)))
                goto exituart;   // Parity error
        }
    }

    // First Stop bit
    TCC1.INTFLAGS = 0x01;   // Clear overflow flag
    while(!testbit(TCC1.INTFLAGS,0));   // wait for next time bit
    if(!testbit(VPORT2.IN, 3))
        goto exituart;       // stop bit error

    // Second Stop bit
    if(testbit(Sniffer,stopbit)) {
        TCC1.INTFLAGS = 0x01;   // Clear overflow flag
        while(!testbit(TCC1.INTFLAGS,0));   // wait for next time bit
        if(!testbit(VPORT2.IN, 3))
            goto exituart;  // stop bit error
    }

    if(Temp.LOGIC.indtx>=640) Temp.LOGIC.indtx=0;
    Temp.LOGIC.data.Serial.TX[Temp.LOGIC.indtx++]=data;
    if(Temp.LOGIC.indtx>=640 && testbit(Mcursors, singlesniff)) {
        TCC1.INTCTRLA = 0x00;           // Disable TCC1 interrupt
        setbit(MStatus, stop);
    }

exituart:
    // Idle bit
    TCC1.INTFLAGS = 0x01;   // Clear overflow flag
    while(!testbit(TCC1.INTFLAGS,0));   // wait for next time bit
    TCC1.PER = Temp.LOGIC.baud/4;
    TCC1.INTFLAGS = 0x01;   // Clear overflow flag
}
