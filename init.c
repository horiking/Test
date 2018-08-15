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
#include <PPS.h>

#define PPSIn(fn,pin)    iPPSInput(IN_FN_PPS##fn,IN_PIN_PPS##pin)
#define PPSOut(fn,pin)    iPPSOutput(OUT_PIN_PPS##pin,OUT_FN_PPS##fn)

 /* This code example demonstrates a method to configure the ECAN module to transmit
    Standard ID CAN messages. */
    /* Include fuse configuration code here. Optionally the fuse configuration can be specified
    via MPLAB IDE Menu operations. */

    unsigned int TxBufferA[4] __attribute__((space(xmemory)));
    unsigned int TxBufferB[4] __attribute__((space(xmemory)));
    
    unsigned int RxBufferA[4] __attribute__((space(xmemory)));
    unsigned int RxBufferB[4] __attribute__((space(xmemory)));
    
void init_spi_adc(void) {


//    Set up DMA Channel 0 to Transmit in Continuous Ping-Pong Mode:

    IFS1bits.DMA2IF = 0;    // DMA0 Interrupt Flag
    IEC1bits.DMA2IE = 1;    // DMA0 Interrupt Enable
    //DMA0CS0 = 0;
    DMA2CON = 0x2002;
    DMA2CONbits.DIR = 1;    // Read from DPSRAM (or RAM) address, write to peripheral address
    DMA2CONbits.MODE = 2;   // Continuous, Ping-Pong modes enabled
    DMA2CONbits.MODE = 3;   // One-Shot, Ping-Pong modes enabled (one block transfer from/to each DMA buffer)
    DMA2CONbits.MODE = 0;
    
    DMA2STAL = (unsigned int)&TxBufferA;
    DMA2STAH = (unsigned int)&TxBufferA;
    
    DMA2REQ = 0x0021;   // SPI2 Transfer Done
    DMA2PAD = (volatile unsigned int) &SPI2BUF;
    
    DMA2CNT = 3;
    DMA2CONbits.CHEN = 1;

//    Set up DMA Channel 1 to Receive in Continuous Ping-Pong Mode:

/*    IFS2bits.DMA3IF = 0;
    IEC2bits.DMA3IE = 1;
    
    //DMA3CON = 0x0002;
    DMA3CONbits.DIR = 0;    // Read from Peripheral address, write to DPSRAM (or RAM) address
    DMA3CONbits.MODE = 2;   // Continuous, Ping-Pong modes enabled
    //DMA3CONbits.MODE = 3;   // One-Shot, Ping-Pong modes enabled (one block transfer from/to each DMA buffer)
    
    DMA3STAL = (unsigned int)&RxBufferA;
    DMA3STAH = (unsigned int)&RxBufferB;
    
    DMA3REQ = 0x0021; // SPI2 Transfer Done
    DMA3PAD = (volatile unsigned int) &SPI2BUF;
    
    DMA3CNT = 3;*/
//    DMA3CONbits.CHEN = 1;
    
// for SPI1 Master Mode:
    
 /*   SPI2STAT = 0x0;                             // disable the SPI module (just in case)
    // SPI2CON1 = 0x0161;                          // FRAMEN = 0, SPIFSD = 0, DISSDO = 0, MODE16 = 0; SMP = 0; CKP = 1; CKE = 1; SSEN = 0; MSTEN = 1; SPRE = 0b000, PPRE = 0b01
    SPI2CON1bits.DISSDO = 0;
    SPI2CON1bits.MODE16 = 1;
    SPI2CON1bits.SMP = 0;
    SPI2CON1bits.SSEN = 0;
    SPI2CON1bits.MSTEN = 1;
    SPI2CON1bits.SPRE = 0b000;
    SPI2CON1bits.PPRE = 0b01;//0b10;//
            
    SPI2CON1bits.CKE = 0x01;                    // Serial output data changes on transition from Idle clock state to active clock state
    SPI2CON1bits.CKP = 0x00;                    // Idle state for clock is a low level; active state is a high level
    SPI2STAT = 0x8000;                          // enable the SPI module
    */
    // Interrupt Controller Settings
    IFS2bits.SPI2IF = 0;    // SPI1 Interrupt Flag
    
    // SPI1CON1 Register Settings
    SPI2CON1bits.MODE16 = 1; // Communication is word-wide (16 bits)
    SPI2CON1bits.MSTEN = 1; // Master mode enabled
    
    // SPI1CON2 Register Settings
    SPI2CON2bits.FRMEN = 0; // Framed mode disabled
    
    // SPI1STAT Register Settings
    SPI2STATbits.SPISIDL = 0; // Continue module operation in Idle mode
    SPI2STATbits.SPIBEC = 0; // Buffer Length = 1 Word
    SPI2STATbits.SPIROV = 0; // No Receive Overflow has occurred
    SPI2STATbits.SPIEN = 1; // Enable SPI module
    
    // Force First Word After Enabling SPI
    DMA2REQbits.FORCE=1;
    while (DMA2REQbits.FORCE == 1);
    IEC2bits.SPI2IE = 1;
    
}

void TxData(void) {
    // TxBufferA[0] = 0b10011101;
    // TxBufferA[1] = 0b10011101;
        uint16_t    i;
   /* for( i = 0; i < 4; i++ ) {
        TxBufferA[i] = i;
    }*/
    TxBufferA[0] = 213;
    TxBufferA[1] = 123;
    TxBufferA[2] = 233;
    TxBufferA[3] = 63;
/*
    for( i = 0; i < 4; i++ ) {
        TxBufferB[i] = 4 + i;
    }

    for( i = 0; i < 4; i++ ) {
        RxBufferA[i] = 0xDEED;
        RxBufferB[i] = 0xDEED;
    }*/
}

void ProcessRxData(void) {
    
}