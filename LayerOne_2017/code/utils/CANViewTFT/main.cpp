#include "mbed.h"
#include "tft.h"

#define TX_ID   (0x7e0)
#define RX_ID   (0x7e1)


DigitalOut led1(LED1);


CAN          can(PB_8, PB_9);  // CAN Rx pin name, CAN Tx pin name

char counter = 0;


#define NUMBER_TEXT_LINES   ( 23 )

int index =0;
char buffer[NUMBER_TEXT_LINES][1024];
bool available = false;
      
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

    memset ( &msg, 0, sizeof ( msg ) );

    if ( can.read ( msg ) ) {

            sprintf(buffer[index%NUMBER_TEXT_LINES],"0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", 
                msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4],msg.data[5], msg.data[6], msg.data[7]  
            );
        available = true;
        index++;
    }
}

int main() {
   
   set_back(0);
   tft_init();  

    tft_clear(TFT_BLACK);
    tft_set_window(32, 8, TFT_WIDTH + 32 - 1, TFT_HEIGHT + 8 - 1);

    set_back(1);
   
   CANMessage msg,txMsg;
   
    can.frequency(1000000);                     // set bit rate to 1Mbps
    can.attach(&onMsgReceived);                 // attach 'CAN receive-complete' interrupt handler
   
    while(1) {


            for(unsigned char q=0;q<NUMBER_TEXT_LINES;q++) 
            {
                tft_text(0,q*8,buffer[q],0xfff,0);
            
            }
            
            led1 = !led1;
        }
        //it(0.2);
    
}
