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

#include "pwm.h"
#include <PPS.h>

void pwm_init(void) {
    
    TRISBbits.TRISB10 = 0;   // output
    TRISBbits.TRISB11 = 0;   // output
    TRISBbits.TRISB12 = 0;   // output
    TRISBbits.TRISB13 = 0;   // output
    TRISBbits.TRISB14 = 0;   // output
    TRISBbits.TRISB15 = 0;   // output
    
    // Set PWM Periods on PHASEx Registers
    //4 KHz 50MHz/12500
    PHASE1 = PERIOD_PWM - 1;//Phasenverschiebung
    PHASE2 = PERIOD_PWM - 1;
    PHASE3 = PERIOD_PWM - 1;
 
 //   PHASE3 = 800;
    /* Set Duty Cycles */
    PDC1 = 200;
    PDC2 = PDC1;
    PDC3 = PDC1;
 
 //   PDC3 = 200;
    /* Set PWM Mode to Complementary */
    IOCON1 = 0xC000;
    IOCON2 = 0xC002;
    IOCON3 = 0xC000;
    /* Set Dead Time Values */
    DTR1 = DTR2 = 200;      //Dead Time Register H
    ALTDTR1 = ALTDTR2 = 200;//Dead Time Register L
    ALTDTR3 = 0;
    PHASE3 = PERIOD_PWM - 1;
 
 //   PHASE3 = 800;
    /* Set Duty Cycles */
    PDC1 = 200;
    PDC2 = PDC1;
    PDC3 = PDC1;
 
 //   PDC3 = 200;
    /* Set PWM Mode to Complementary */
    IOCON1 = 0xC000;
    IOCON2 = 0xC002;
    IOCON3 = 0xC000;
    


    /* Set Primary Time Base, Edge-Aligned Mode and Independent Duty Cycles */
 //   PWMCON1 = PWMCON2 = 0x0000;
    /* Set Independent Time Bases, Center-Aligned mode and Independent Duty Cycles */
   // PWMCON1 = 0x0604;
    PWMCON1bits.TRGIEN = 1; // A trigger event generates an interrupt request
    PWMCON1bits.ITB = 1;    // PHASEx/SPHASEx registers provide time base period for this PWM generator
    PWMCON1bits.CAM = 1;    // Center-Aligned mode is enabled
   
    PWMCON2bits.TRGIEN = 1; // A trigger event generates an interrupt request
    PWMCON2bits.ITB = 1;    // PHASEx/SPHASEx registers provide time base period for this PWM generator
    PWMCON2bits.CAM = 1;    // Center-Aligned mode is enabled
    
  //  PWMCON2 = 0x0204;
 //   PWMCON3 = 0x0204;
    // Configure Faults
    FCLCON1 = FCLCON2 = 0x0003;
    
    // 1:1 Prescaler
    PTCON2bits.PCLKDIV = 0; // no prescale
    /*Interrupts Config*/
   // TRIG1 = 0;
   // TRGCON1 = 0;
  //  TRGCON1bits.TRGSTRT = 4;         //         This and the next 3 lines are highlighted by the error message
    
    IFS5bits.PWM1IF = 0;
    IPC23bits.PWM1IP = 4;
    IEC5bits.PWM1IE = 1;
    
    IFS5bits.PWM2IF = 0;
    IPC23bits.PWM2IP = 4;
    IEC5bits.PWM2IE = 1;
    
    /* Independent PWM ADC Triggering */
    TRIG1 = 0; // Point at which the ADC module is to be triggered by primary PWM
    TRIG2 = PERIOD_PWM - 1; // Point at which the ADC module is to be triggered by primary PWM
    TRGCON1bits.TRGDIV = 1; // Trigger output divider set to trigger ADC on every trigger match event
    TRGCON1bits.TRGSTRT = 0; // First ADC trigger event occurs after four trigger match events 
    
    TRGCON2bits.TRGDIV = 3; // Trigger output divider set to trigger ADC on every trigger match event
    TRGCON2bits.TRGSTRT = 0; // First ADC trigger event occurs after four trigger match events 
    
    //PWMCON1bits.TRGIEN = 1; // Trigger event generates an interrupt request 
    //while (PWMCON1bits.TRGSTAT == 1); // Wait for ADC interrupt status change 
    
    // Intialize the ADC
/*	
    AD1CON1bits.SSRCG = 1;      // normal PWM trigger
    AD1CON1bits.SSRC = 0;       // PWM Generator 1 primary trigger compare ends sampling and starts conversion
    
    AD1CON1bits.AD12B = 1;      // 12-bit, 1-channel ADC operation
    AD1CON1bits.ASAM = 1;       // Sampling begins immediately after last conversion; SAMP bit is auto-set
    AD1CON2bits.VCFG = 0;       // AVdd - AVss
    AD1CON3bits.ADCS = 2;       // 3 ? TCY = TAD
    AD1CHS0bits.CH0SA = 0;      // Channel 0 positive input is AN0
    AD1CHS0bits.CH0NA = 0;      // Channel 0 negative input is VREFL
    AD1CON2bits.CHPS = 0;       // Converts CH0
    
    IFS0bits.AD1IF = 0;         // clear Interrupt Flag
    IPC3bits.AD1IP = 7;         // Interrupt is Priority 7
    IEC0bits.AD1IE = 1;         // enable Interrupt
    
	
	AD1CON1bits.ADON = 1;             // Start the ADC module //	
*/    
    /* Enable PWM Module */
    PTCON = 0x8000;

/*    T1CON = 0;            // Clear Timer 1 configuration
    T1CONbits.TCKPS = 1;  // Set timer 1 prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
    PR1 = 20000;          // Set Timer 1 period (max value is 65535)
    _T1IP = 1;            // Set Timer 1 interrupt priority
    _T1IF = 0;            // Clear Timer 1 interrupt flag
    //_T1IE = 1;            // Enable Timer 1 interrupt
    T1CONbits.TON = 1;    // Turn on Timer 1*/

}