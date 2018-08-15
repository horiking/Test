/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

//#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
//#elif defined(HI_TECH_C)
//    #include <htc.h>        /* HiTech General Include File */
//#elif defined(__18CXX)
//    #include <p18cxxx.h>    /* C18 General Include File */
//#endif

//#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

//#endif

#include "init.h"

#include "ecan.h"
#include <PPS.h>

#define PPSIn(fn,pin)    iPPSInput(IN_FN_PPS##fn,IN_PIN_PPS##pin)
#define PPSOut(fn,pin)    iPPSOutput(OUT_PIN_PPS##pin,OUT_FN_PPS##fn)

 /* This code example demonstrates a method to configure the ECAN module to transmit
    Standard ID CAN messages. */
    /* Include fuse configuration code here. Optionally the fuse configuration can be specified
    via MPLAB IDE Menu operations. */
    //FUSE_CONFIGURATION_MACROS_COME_HERE
    #define NUM_OF_ECAN_BUFFERS 32
    /* This is the ECAN message buffer declaration. Note the buffer alignment. */
    unsigned int ecan1MsgBuf[NUM_OF_ECAN_BUFFERS][8]
    __attribute__((aligned(NUM_OF_ECAN_BUFFERS * 16)));
    
    unsigned int ecan1MsgRxBuf[NUM_OF_ECAN_BUFFERS][8]
    __attribute__((aligned(NUM_OF_ECAN_BUFFERS * 16)));
    

void ecan_set_op_mode(ECAN_OP_MODE mode) {
    //CANCON &= 0x1F;                         // clear previous mode
    //CANCON |= mode;                         // set new mode
    C1CTRL1bits.REQOP = mode;    
    while( ecan_get_op_mode() != mode ); // Wait till desired mode is set.
}

ECAN_OP_MODE ecan_get_op_mode(void) {
    return C1CTRL1bits.OPMODE;
}

void ecan_init(void) {

    PPSIn (_C1RX,_RP37);
    PPSOut(_C1TX,_RP38);
//    TRISC |= 0b10000000;
// output (TANRX)
//    TRISC &= 0b10111111;
    
    // Put module into Configuration mode.
    ecan_set_op_mode(ECAN_OP_MODE_CONFIG);
    while(C1CTRL1bits.OPMODE != 4);
    C1CTRL1bits.WIN = 0;
  //  C1CTRL1bits.WIN = 1;

    // Set Bit rate values as per defines.
    C1CFG1 = ((ECAN_SJW_VAL-1) << 6) | (ECAN_BRP_VAL);
    C1CFG2 = (ECAN_PHSEG2_MODE_VAL << 7) | (ECAN_BUS_SAMPLE_MODE_VAL << 6) | ((ECAN_PHSEG1_VAL-1) << 3) | (ECAN_PROPSEG_VAL-1);
    C1CFG2 |= (ECAN_WAKEUP_MODE_VAL << 7) | (ECAN_FILTER_MODE_VAL << 6) | (ECAN_PHSEG2_VAL-1);

    // Set CANTX2, TXDRIVE and CAN Capture modes.
   // CIOCON = (ECAN_TX2_SOURCE_VAL << 7) | (ECAN_TX2_MODE_VAL << 6) | (ECAN_TXDRIVE_MODE_VAL << 5) | (ECAN_CAPTURE_MODE_VAL << 4);

// ecan_set_op_mode(ECAN_OP_MODE_NORMAL);

    
// RECEIVE SETTINGS
/*   
    RXB0CON = 0b00100000; // only valid messages with standard id

    // acceptance and mask filter: only allow id = 0b 0000 0000 0010
    RXM0SIDH = 0b11111111; // mask all id-bits
    RXM0SIDL = 0b11100000;

    RXF0SIDH = VALVE_ID >> 3; //0b00000000; // filter id = 2
    RXF0SIDL = VALVE_ID << 5; //0b01000000;
    
    PIE5bits.RXB0IE = 1;
*/
    
// DMA SETTINGS
    
    C1FCTRL = 0xC01F; // No FIFO, 32 Buffers
    // Assign 32x8word Message Buffers for ECAN1 in device RAM. This example uses DMA0 for TX.
    //Refer to 21.8.1 ?DMA Operation for Transmitting Data? for details on DMA channel
    // configuration for ECAN transmit. 
    
    // Data Transfer Size: Word Transfer Mode
    DMA0CONbits.SIZE = 0x0;
    // Data Transfer Direction: device RAM to Peripheral
    DMA0CONbits.DIR = 0x1;
    // DMA Addressing Mode: Peripheral Indirect Addressing mode
    DMA0CONbits.AMODE = 0x2;
    // Operating Mode: Continuous, Ping-Pong modes disabled
    DMA0CONbits.MODE = 0x0;
    // Assign ECAN1 Transmit event for DMA Channel 0
    DMA0REQ = 70;
    // Set Number of DMA Transfer per ECAN message to 8 words
    DMA0CNT = 7;
    // Peripheral Address: ECAN1 Transmit Register
    DMA0PAD = (volatile unsigned int)&C1TXD;
 
    // Channel Enable: Enable DMA Channel 0
    DMA0CONbits.CHEN = 0x1; // hier hängt es mit ecan und spi
      
    // Configure Message Buffer 0 for Transmission and assign priority
    C1TR01CONbits.TXEN0 = 0x1;
    C1TR01CONbits.TX0PRI = 0x3;
    // second buffer
    C1TR01CONbits.TXEN1 = 0x1;
    C1TR01CONbits.TX1PRI = 0x2;
    // third buffer
    C1TR23CONbits.TXEN2 = 0x1;
    C1TR23CONbits.TX2PRI = 0x2;
    
    // At this point the ECAN1 module is ready to transmit a message. Place the ECAN module in
    // Normal mode. 
//    C1CTRL1bits.REQOP = 0;
//    while(C1CTRL1bits.OPMODE != 0);
    //C1CTRL1bits.REQOP = 2;
    //while(C1CTRL1bits.OPMODE != 2);
  
// RECEIVE SETTINGS
    C1CTRL1bits.WIN = 1;
    // Set up the CAN module for 250kbps speed with 10 Tq per bit. //
//    C1CFG1 = 0x47; // BRP = 8 SJW = 2 Tq
//    C1CFG2 = 0x2D2;
    C1FCTRL = 0xC01F; // No FIFO, 32 Buffers

    DMA1CONbits.SIZE = 0x0;
    DMA1CONbits.DIR = 0x0;
    DMA1CONbits.AMODE = 0x2;
    DMA1CONbits.MODE = 0x0;
    DMA1REQ = 34;
    DMA1CNT = 7;
    DMA1PAD = (volatile unsigned int)&C1RXD;
    DMA1STAL = (unsigned int) &ecan1MsgRxBuf;
    DMA1STAH = (unsigned int) &ecan1MsgRxBuf;
    DMA1CONbits.CHEN = 0x1;

    // Channel Interrupt Enable: Enable DMA Channel 1 Interrupt
    IEC0bits.DMA1IE = 1;   
    
    // Select Acceptance Filter Mask 0 for Acceptance Filter 0
    C1FMSKSEL1bits.F0MSK=0x0;
    
    // Configure Acceptance Filter Mask 0 register to mask SID<2:0>
    // Mask Bits (11-bits) : 0b111 1111 1000 
    C1RXM0SIDbits.SID = 0x7F8;
    
    // Configure Acceptance Filter 0 to match standard identifier
    // Filter Bits (11-bits): 0b011 1010 xxx with the mask setting, message with SID
    // range 0x1D0-0x1D7 will be accepted by the ECAN module.
    C1RXF0SIDbits.SID = 0x01D0;
    
    // Acceptance Filter 0 to check for Standard Identifier
    C1RXM0SIDbits.MIDE = 0x1;
    C1RXF0SIDbits.EXIDE= 0x0;
    
    // Acceptance Filter 0 to use Message Buffer 10 to store message
    C1BUFPNT1bits.F0BP = 0xA; // 0xA;
    
    // Filter 0 enabled for Identifier match with incoming message
    C1FEN1bits.FLTEN0=0x1;
    
    // Clear Window Bit to Access ECAN
    // Control Registers 
    C1CTRL1bits.WIN=0x0;
    
    // Place the ECAN module in normal mode. 
    C1CTRL1bits.REQOP = 0;
    while(C1CTRL1bits.OPMODE != 0);
    // The following code shows one example of how the application can wait
    //for a message to be received in message buffer 10
    
    // Enable ECAN1 receive Interrupt
   // C1INTEbits.RBIE = 1;
   
}


uint16_t ecan_read_message(uint8_t n) {
    return ecan1MsgRxBuf[10][n];
}

void ecan_send_message(WORD* data, uint8_t datalength, unsigned int id) {

    
//    unsigned long address;   
    DMA0STAL = (unsigned int) &ecan1MsgBuf;
    DMA0STAH = (unsigned int) &ecan1MsgBuf;
    /* Write to message buffer 0 */
    
        /* CiTRBnSID = 0bxxx1 0010 0011 1100
        IDE = 0b0
        SRR = 0b0
        SID<10:0>= 0b100 1000 1111 */ 
        ecan1MsgBuf[0][0] = id << 2;//0x123C;
        
        /* CiTRBnEID = 0bxxxx 0000 0000 0000
        EID<17:6> = 0b0000 0000 0000 */
        ecan1MsgBuf[0][1] = 0x0000;
        
        /* CiTRBnDLC = 0b0000 0000 xxx0 1111
        EID<17:6> = 0b000000
        RTR = 0b0
        RB1 = 0b0
        RB0 = 0b0
        DLC = 0b1111 */
        ecan1MsgBuf[0][2] = 0x0008;
        
        /* Write message data bytes */
        ecan1MsgBuf[0][3] = data[0];    // counter
        ecan1MsgBuf[0][4] = data[1];    // isoll
        ecan1MsgBuf[0][5] = data[2];    // iist
        ecan1MsgBuf[0][6] = data[3];    // hallsonde
        
        ecan1MsgBuf[1][0] = (id + 1) << 2;//0x123C;
        
        /* CiTRBnEID = 0bxxxx 0000 0000 0000
        EID<17:6> = 0b0000 0000 0000 */
        ecan1MsgBuf[1][1] = 0x0000;
        
        ecan1MsgBuf[1][2] = 0x0008;
        
        /* Write message data bytes */
        ecan1MsgBuf[1][3] = data[0];    // counter
        ecan1MsgBuf[1][4] = data[4];    // xsoll
        ecan1MsgBuf[1][5] = data[5];    // xist
        ecan1MsgBuf[1][6] = data[6];    // dacout
        
        
        ecan1MsgBuf[2][0] = (id + 2) << 2;//0x123C;
        
        /* CiTRBnEID = 0bxxxx 0000 0000 0000
        EID<17:6> = 0b0000 0000 0000 */
        ecan1MsgBuf[2][1] = 0x0000;
        
        ecan1MsgBuf[2][2] = 0x0008;
        
        /* Write message data bytes */
        ecan1MsgBuf[2][3] = data[0];    // counter
        ecan1MsgBuf[2][4] = data[7];    // lockin1
        ecan1MsgBuf[2][5] = data[8];    // lockin2
        ecan1MsgBuf[2][6] = data[9];    // 
        
        /* Request message buffer 0 transmission */
        C1TR01CONbits.TXREQ0 = 0x1;
        C1TR01CONbits.TXREQ1 = 0x1;
        C1TR23CONbits.TXREQ2 = 0x1;
        /* The following shows an example of how the TXREQ bit can be polled to check if transmission
        is complete. */
        /*_LATB8 = 0;
        while(C1TR01CONbits.TXREQ0 == 1);
        _LATB8 = 1;*/
        /* Message was placed successfully on the bus */
       // __delay_ms(100);

    
}
/*
BOOL ECANSendMessage( unsigned long id, BYTE* data, BYTE dataLen, ECAN_TX_MSG_FLAGS msgFlags) {

     
#define buffers 2


    BYTE i,j;
    BYTE *ptr, *tempPtr;
    BYTE* pb[9];
    BYTE temp;

    //
     // Since there are more than one transmit buffers and they are scattered in
     // SFR map, prepare table of all transmit buffers.
     //
    pb[0]=(BYTE*)&TXB0CON;
    pb[1]=(BYTE*)&TXB1CON;
    pb[2]=(BYTE*)&TXB2CON;


    //
     // Depending on whether only mode 0 is used or not, we would only need to check
     // either 3 or all buffers.
     //
    for ( i = 0; i < buffers; i++ )

    {
        //
         // Use local pointer to reduce overall code.
         // It will be more efficient to access using pointer instead of index.
         //
        ptr = pb[i];
        tempPtr = ptr;

        //
        // Check to see if this buffer is empty by looking at BnCON.TXREQ bit (bit3).
         //
        if ( !(*ptr & 0x08) ) {

           LATCbits.LATC4 = 1; // Set transmit priority in BnCON register.
            *ptr &= ~ECAN_TX_PRIORITY_BITS;
            *ptr |= msgFlags & ECAN_TX_PRIORITY_BITS;

            // Also save DLC value.
            if ( msgFlags & ECAN_TX_RTR_BIT )
                temp = 0x40 | dataLen;
            else
                temp = dataLen;
            // Use temp to reduce evaluation of *(ptr+5) to only once.
            *(ptr+5) = temp;

            // Set standard or extended message type.
            if ( msgFlags & ECAN_TX_FRAME_BIT )
                temp = ECAN_MSG_XTD;

            else
                temp = ECAN_MSG_STD;

            // And rearrange given id accordingly.
            CANIDToRegs((BYTE*)(ptr+1), id, temp);

            // Prepare for data byte access.
            ptr += 6;
            // Copy given number of data bytes.
            for ( j = 0 ; j < dataLen; j++ )
                *ptr++ = *data++;

            // If this buffer is configured to automatically handle RTR messages,
            // do not set TXREQ bit.  TXREQ bit will be set whenever matching RTR is received.
            if ( !(*tempPtr & 0x04) )
                *tempPtr |= 0x08;
            LATCbits.LATC4 = 0;
            return TRUE;
        }
    }

    // There were no empty buffers.
     LATCbits.LATC4 = 0;
    return FALSE;
}

*/