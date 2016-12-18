/*===================================================================*/
/*                                                                   */
/*  pNesX_System_Nucleo.cpp : The function which depends on a system */
/*                         (for Nucleo F446RE)                       */
/*                                                                   */
/*  2016/1/20  Racoon                                                */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/


#include "mbed.h"
#include "SDFileSystem.h"
#include "tft.h"
//#include "SWO.h"

// ID that game pad data is sent on
#define CAN_SEND_ID	(0x7e1)

// ID that game pad data is received from
#define CAN_RECV_ID (0x7e0)

// Memory interface
enum {
    CAN_READ_BYTE = CAN_SEND_ID + 1,
    CAN_READ_WORD,
    CAN_READ_DWORD,

    CAN_WRITE_BYTE,
    CAN_WRITE_WORD,
    CAN_WRITE_DWORD
};

#define USE_CAN (1)

#ifdef USE_CAN
CAN can1 ( PB_8, PB_9 );

// not usng second CAN yet.,
//CAN can2(PB_5, PB_6);
#endif

//#define ORIGINAL_GAMEPAD
//#define PS_GAMEPAD
#define NES_GAMEPAD

#if !defined(PS_GAMEPAD)
#include "USBHostGamepad.h"
USBHostGamepad *pad;
#else
#include "pspad.h"
#endif

#include "pNesX.h"
#include "pNesX_System.h"

extern int pNesX_Filer();
extern void ApuMute ( bool mute );


// temporary store for joystick value
static DWORD canval2;

// if there is a CAN msg available to read from
static bool msgAvailable = false;

SDFileSystem *sd; //(PB_15, PB_14, PB_13, PA_9, "sd", NC, SDFileSystem::SWITCH_NONE, 20000000);

DigitalIn userbutton ( USER_BUTTON );

extern WORD LineData[][256];
extern WORD *pDrawData;

//#define SHOW_FPS

#if defined(SHOW_FPS)
Timer timer;
int fps = 0;
int dmawait;
#endif

//SWO_Channel swo;

/*-------------------------------------------------------------------*/
/*  ROM image file information                                       */
/*-------------------------------------------------------------------*/

extern char szRomFolder[];
extern char szRomFilename[];
extern char szRomPath[];

char szRomName[ 256 ];
char szSaveName[ 256 ];
int nSRAM_SaveFlag;

// Palette data
const WORD NesPalette[ 64 ] = {
    0xef7b, 0x1f00, 0x1700, 0x5741, 0x1090, 0x04a8, 0x80a8, 0xa088, 0x8051, 0xc003, 0x4003, 0xc002, 0x0b02, 0x0000, 0x0000, 0x0000,
    0xf7bd, 0xdf03, 0xdf02, 0x3f6a, 0x19d8, 0x0be0, 0xc0f9, 0xe2e2, 0xe0ab, 0xc005, 0x4005, 0x4805, 0x5104, 0x0000, 0x0000, 0x0000,
    0xdfff, 0xff3d, 0x5f6c, 0xdf9b, 0xdffb, 0xd3fa, 0xcbfb, 0x08fd, 0xc0fd, 0xc3bf, 0xca5e, 0xd35f, 0x5b07, 0xcf7b, 0x0000, 0x0000,
    0xffff, 0x3fa7, 0xdfbd, 0xdfdd, 0xdffd, 0x38fd, 0x96f6, 0x15ff, 0xcffe, 0xcfdf, 0xd7bf, 0xdbbf, 0xff07, 0xdffe, 0x0000, 0x0000
};

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

int LoadSRAM();
int SaveSRAM();

/*===================================================================*/
/*                                                                   */
/*           LoadSRAM() : Load a SRAM                                */
/*                                                                   */
/*===================================================================*/
int LoadSRAM()
{
    /*
     *  Load a SRAM
     *
     *  Return values
     *     0 : Normally
     *    -1 : SRAM data couldn't be read
     */

    return 0;
}

/*===================================================================*/
/*                                                                   */
/*           SaveSRAM() : Save a SRAM                                */
/*                                                                   */
/*===================================================================*/
int SaveSRAM()
{
    /*
     *  Save a SRAM
     *
     *  Return values
     *     0 : Normally
     *    -1 : SRAM data couldn't be written
     */


    // Successful
    return 0;
}

/*===================================================================*/
/*                                                                   */
/*                  pNesX_Menu() : Menu screen                       */
/*                                                                   */
/*===================================================================*/
int pNesX_Menu()
{


    /*
     *  Menu screen
     *
     *  Return values
     *     0 : Normally
     *    -1 : Exit pNesX
     */

#if defined(SHOW_FPS)
    timer.stop();
#endif


    ApuMute ( true );

    switch ( pNesX_Filer() ) {
        case 0:  // Selected a file
            if ( pNesX_Load ( szRomPath ) == 0 ) {
                // Set a ROM image name
                strcpy ( szRomName, szRomFilename );

                // Load SRAM
                LoadSRAM();
            }

            break;

        case 1:  // Return to Emu
            break;

        case 2:  // Reset
            if ( szRomName[ 0 ] != 0 ) {
                // Reset pNesX
                pNesX_Reset();
            }

            break;

        case -1:  // Error
            printf ( "Filer Error\r\n" );

            while ( 1 );
    }

    tft_clear ( TFT_BLACK );
    tft_set_window ( 32, 8, NES_DISP_WIDTH + 32 - 1, NES_DISP_HEIGHT + 8 - 1 );

    ApuMute ( false );

#if defined(SHOW_FPS)
    timer.start();
#endif

    return 0;
}

/*===================================================================*/
/*                                                                   */
/*               pNesX_ReadRom() : Read ROM image file               */
/*                                                                   */
/*===================================================================*/
int pNesX_ReadRom ( const char *pszFileName )
{
    /*
     *  Read ROM image file
     *
     *  Parameters
     *    const char *pszFileName          (Read)
     *
     *  Return values
     *     0 : Normally
     *    -1 : Error
     */
    FileHandle* file;

    /* Open ROM file */
    file = sd->open ( pszFileName, O_RDONLY );

    if ( file == NULL ) {
        printf ( "open error\r\n" );
        return -1;
    }

    /* Read ROM Header */
    file->read ( &NesHeader, sizeof NesHeader );

    if ( memcmp ( NesHeader.byID, "NES\x1a", 4 ) != 0 ) {
        /* not .nes file */
        file->close();
        return -1;
    }

    /* Clear SRAM */
    memset ( SRAM, 0, SRAM_SIZE );

    /* If trainer presents Read Triner at 0x7000-0x71ff */
    if ( NesHeader.byInfo1 & 4 ) {
        printf ( "Read Trainer\r\n" );
        file->read ( &SRAM[ 0x1000 ], 512 );
    }

    printf ( "RomSize: %d\r\n", NesHeader.byRomSize );
    /* Allocate Memory for ROM Image */
    ROM = ( BYTE * ) malloc ( NesHeader.byRomSize * 0x4000 );
    printf ( "ROM addr:%x\r\n", ROM );

    /* Read ROM Image */
    file->read ( ROM, 0x4000 * NesHeader.byRomSize );

    if ( NesHeader.byVRomSize > 0 ) {
        /* Allocate Memory for VROM Image */
        VROM = ( BYTE * ) malloc ( NesHeader.byVRomSize * 0x2000 );
        printf ( "VROM addr:%x\r\n", VROM );

        /* Read VROM Image */
        file->read ( VROM, 0x2000 * NesHeader.byVRomSize );
    }

    /* File close */
    file->close();

    /* Successful */
    return 0;
}

/*===================================================================*/
/*                                                                   */
/*           pNesX_ReleaseRom() : Release a memory for ROM           */
/*                                                                   */
/*===================================================================*/
void pNesX_ReleaseRom()
{
    /*
     *  Release a memory for ROM
     *
     */
    if ( ROM ) {
        free ( ROM );
        ROM = NULL;
    }

    if ( VROM ) {
        free ( VROM );
        VROM = NULL;
    }
}

/*===================================================================*/
/*                                                                   */
/*      pNesX_LoadFrame() :                                          */
/*           Transfer the contents of work frame on the screen       */
/*                                                                   */
/*===================================================================*/
void pNesX_LoadFrame()
{
    /*
     *  Transfer the contents of work frame on the screen
     *
     */
#if defined(SHOW_FPS)
    fps++;

    if ( timer.read_ms() >= 1000 ) {
        timer.stop();
        printf ( "%d %d %d\r\n", fps, timer.read_ms(), dmawait );
        fps = 0;
        timer.reset();
        timer.start();
    }

    dmawait = 0;
#endif
}

void pNesX_TransmitLinedata()
{
    while ( SpiHandle.State != HAL_SPI_STATE_READY ) {
#if defined(SHOW_FPS)
        dmawait++;
#endif
    }

    HAL_SPI_Transmit_DMA ( &SpiHandle, ( uint8_t * ) pDrawData, 256 * 2 );
}

#if defined(ORIGINAL_GAMEPAD)
const BYTE UsbPadTable[] = {0x10, 0x90, 0x80, 0xa0, 0x20, 0x60, 0x40, 0x50};
#endif

/*===================================================================*/
/*                                                                   */
/*             pNesX_PadState() : Get a joypad state                 */
/*                                                                   */
/*===================================================================*/
void pNesX_PadState ( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem )
{
    /*
     *  Get a joypad state
     *
     *  Parameters
     *    DWORD *pdwPad1                   (Write)
     *      Joypad 1 State  R L D U St Se B A
     *
     *    DWORD *pdwPad2                   (Write)
     *      Joypad 2 State
     *
     *    DWORD *pdwSystem                 (Write)
     *      Input for pNesX
     *
     */

#if defined(ORIGINAL_GAMEPAD)
    *pdwPad1 = ( ( pad->report[4] & 0x20 ) >> 5 ) |
               ( ( pad->report[4] & 0x10 ) >> 3 ) |
               ( ( pad->report[5] & 0x30 ) >> 2 );

    if ( ! ( pad->report[4] & 8 ) ) {
        *pdwPad1 |= UsbPadTable[pad->report[4] & 7];
    }

    *pdwPad1 = *pdwPad1 | ( *pdwPad1 << 8 );

    *pdwPad2 = 0;
    *pdwSystem = ( ( pad->report[5] & 0x4 ) >> 2 ) |
                 ( ( ( pad->report[5] & 0xa ) == 0xa ) << 1 );

    USBHost::poll();

#elif defined(NES_GAMEPAD)

    *pdwSystem = 0 ;
    *pdwPad1 = 0 ;
    /*
    01 02 03 04 05 06 07 08
    -----------------------
    01 80 80 7f 7f 0f 00 00
    LEFT     00
    RIGHT    FF
    UP          00
    DOWN        FF
    SELECT            10
    START             20
    B              1F
    A              2F
    */

    if ( pad->report[3] == 0x00 ) {*pdwPad1 |= KEYPAD_LEFT; *pdwSystem ^= PAD_SYS_LEFT;}

    if ( pad->report[3] == 0xFF ) {*pdwPad1 |= KEYPAD_RIGHT; *pdwSystem ^= PAD_SYS_RIGHT;}

    if ( pad->report[4] == 0x00 ) {*pdwPad1 |= KEYPAD_UP; *pdwSystem ^= PAD_SYS_UP;}

    if ( pad->report[4] == 0xFF ) {*pdwPad1 |= KEYPAD_DOWN; *pdwSystem ^= PAD_SYS_DOWN;}

    if ( pad->report[6] & 0x10 ) {*pdwPad1 |= KEYPAD_SELECT;}

    if ( pad->report[6] & 0x20 ) {*pdwPad1 |= KEYPAD_START;}

    if ( pad->report[5] & 0x10 ) {*pdwPad1 |= KEYPAD_B;}

    if ( pad->report[5] & 0x20 ) {*pdwPad1 |= KEYPAD_A;}

    if ( pad->report[6] == 0x30 ) { *pdwSystem = PAD_SYS_QUIT; }

#ifdef USE_CAN

    {
        static DWORD lastpad;

        if ( *pdwPad1 != lastpad ) {
            can1.write ( CANMessage ( CAN_SEND_ID, ( char* ) pdwPad1, 4 ) );
            lastpad = *pdwPad1;
        }
    }

    if ( msgAvailable ) {

        *pdwPad2 = canval2;

    }	else
#endif
    {
        // network ?
        *pdwPad2 = 0;
    }


    USBHost::poll();
#else

    unsigned short pad1, pad2;
    pspad_read ( &pad1, &pad2 );
    // R L D U St Se B A

    // SE -- -- ST U R D L       L2 R2 L1 R1 TR O X SQ
    *pdwPad1 = ( ( pad1 & 0x0400 ) >> 3 ) |
               ( ( pad1 & 0x0100 ) >> 2 ) |
               ( ( pad1 & 0x0200 ) >> 4 ) |
               ( ( pad1 & 0x0800 ) >> 7 ) |
               ( ( pad1 & 0x1000 ) >> 9 ) |
               ( ( pad1 & 0x8000 ) >> 13 ) |
               ( ( pad1 & 1 ) << 1 ) |
               ( ( pad1 & 2 ) >> 1 );

    *pdwPad1 = *pdwPad1 | ( *pdwPad1 << 8 );

    *pdwPad2 = 0;

    *pdwSystem = ( ( pad1 & 0x80 ) >> 7 ) |
                 ( ( ( pad1 & 0x50 ) == 0x50 ) << 1 );

#endif

}

/*===================================================================*/
/*                                                                   */
/*             pNesX_MemoryCopy() : memcpy                           */
/*                                                                   */
/*===================================================================*/
void *pNesX_MemoryCopy ( void *dest, const void *src, int count )
{
    /*
     *  memcpy
     *
     *  Parameters
     *    void *dest                       (Write)
     *      Points to the starting address of the copied block to destination
     *
     *    const void *src                  (Read)
     *      Points to the starting address of the block of memory to copy
     *
     *    int count                        (Read)
     *      Specifies the size, in bytes, of the block of memory to copy
     *
     *  Return values
     *    Pointer of destination
     */

    memcpy ( dest, src, count );
    return dest;
}

/*===================================================================*/
/*                                                                   */
/*             pNesX_MemorySet() :                                   */
/*                                                                   */
/*===================================================================*/
void *pNesX_MemorySet ( void *dest, int c, int count )
{
    /*
     *  memset
     *
     *  Parameters
     *    void *dest                       (Write)
     *      Points to the starting address of the block of memory to fill
     *
     *    int c                            (Read)
     *      Specifies the byte value with which to fill the memory block
     *
     *    int count                        (Read)
     *      Specifies the size, in bytes, of the block of memory to fill
     *
     *  Return values
     *    Pointer of destination
     */

    memset ( dest, c, count );
    return dest;
}

/*===================================================================*/
/*                                                                   */
/*                DebugPrint() : Print debug message                 */
/*                                                                   */
/*===================================================================*/
void pNesX_DebugPrint ( char *pszMsg )
{
}

/**
 * @brief   'CAN receive-complete' interrup handler.
 * @note    Called on arrival of new CAN message.
 *          Keep it as short as possible.
 * @param
 * @retval
 */
void onMsgReceived()
{
    CANMessage msg;

    // address to read/wrte
    WORD	address;

    // default to SRAM
    uint8_t *bank_ptr = &RAM[0];
    uint8_t bank = 0;

    memset ( &msg, 0, sizeof ( msg ) );


    if ( can1.read ( msg ) ) {

        // less code
        address = ( ( WORD ) msg.data[0] << 8 + ( WORD ) msg.data[1] );

        // RAM,SRAM,ROM,ROMBANK0/1/2/3,PPURAM,VROM,PPUBANK
        bank = msg.data[2];

        // decide which bank
        switch ( bank ) {
            case 0:
                //RAM
                break;

            case 1:
                bank_ptr = &SRAM[0];
                break;

            case 2:
                bank_ptr = ROM;
                break;

            case 3:
                bank_ptr = &PPURAM[0];
                break;

            case 4:
                bank_ptr = VROM;
                break;

            case 5:
                bank_ptr = ( uint8_t* ) &PPUBANK[0];
                break;

            case 6:
                bank_ptr = ( uint8_t* ) &SPRRAM[0];
                break;

        }

        switch  ( msg.id  ) {
            case CAN_RECV_ID :

                /// so far only mark it as available when its a gamepad message..
                msgAvailable = true;

                //memcpy(&canval2,(char*)&msg.data[0],4);
                canval2 = ( unsigned long ) ( msg.data[3] << 24 ) + ( ( unsigned long ) msg.data[2] << 16 ) + ( ( unsigned long ) msg.data[1] << 8 ) + ( ( unsigned long ) msg.data[0] );
                break;

            case CAN_READ_BYTE:
                break;

            case CAN_READ_WORD:
                break;

            case CAN_READ_DWORD:
                break;

            case CAN_WRITE_BYTE:
                break;

            case CAN_WRITE_WORD:
                break;

            case CAN_WRITE_DWORD:
                break;
        }
    }
}


/*===================================================================*/
/*                                                                   */
/*                pNesX Initialise                                   */
/*                                                                   */
/*===================================================================*/
int main()
{

#ifdef USE_CAN
    //	can1.reset();
    can1.frequency ( 1000000 );
    can1.attach ( &onMsgReceived );              // attach 'CAN receive-complete' interrupt handler

#endif

    // TFT initialize
    tft_init();
    tft_clear ( TFT_BLACK );
    tft_set_window ( 32, 8, NES_DISP_WIDTH + 32 - 1, NES_DISP_HEIGHT + 8 - 1 );

    // SD card Initialize
    sd = new SDFileSystem ( PB_15, PB_14, PB_13, PA_9, "sd", NC, SDFileSystem::SWITCH_NONE, 20000000 );
    sd->crc ( true );
    sd->large_frames ( true );
    sd->write_validation ( true );

    strcpy ( szRomFolder, "/sd" );

#if !defined(PS_GAMEPAD)
    // USB Gamepad Initialize
    pad = new USBHostGamepad();
    pad->connect();
#else
    pspad_init();
#endif

    // Apu Initialize
    ApuInit();

    /*-------------------------------------------------------------------*/
    /*  Start pNesX                                                      */
    /*-------------------------------------------------------------------*/

    pNesX_Main();

}


