#include <xc.h>
#include "system.h"

#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

// Select Internal FRC at POR
//_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure POSC in XT mode
//_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);

void ConfigureOscillator(void) {
    
    // input: 20 MHz crystal FIN
    // output: 100 MHz FPLLO
    
    // 50 MIPS
    CLKDIVbits.PLLPRE = 2;  // N1 = 4 = PLLPRE+2        5 MHz
    PLLFBDbits.PLLDIV = 38; // M = 40 = PLLDIV+2        200 MHz
    CLKDIVbits.PLLPOST = 0; // N2 = 2 = 2*(PLLPOST+1)   100 MHz
    
    // 40 MIPS = Fcy -> 50 ns pro Zyklus (gemessen)
    /*CLKDIVbits.PLLPRE = 2;  // N1 = 4 = PLLPRE+2        5 MHz
    PLLFBDbits.PLLDIV = 30; // M = 30 = PLLDIV+2        160 MHz
    CLKDIVbits.PLLPOST = 0; // N2 = 2 = 2*(PLLPOST+1)   80 MHz*/
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.DOZEN = 0;
    
    // Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b011);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);

 /*   // Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONH(0x03);          // Set OSCCONH for clock switch
    __builtin_write_OSCCONL(OSCCON | 0x01); // Start clock switching
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b011);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);*/
}
#if 0
        /* Disable Watch Dog Timer */
        RCONbits.SWDTEN = 0;

        /* When clock switch occurs switch to Pri Osc controlled by FPR<4:0> */
        __builtin_write_OSCCONH(0x03);  /* Set OSCCONH for clock switch */
        __builtin_write_OSCCONL(0x01);  /* Start clock switching */
        while(OSCCONbits.COSC != 0b011);

        /* Wait for Clock switch to occur */
        /* Wait for PLL to lock, if PLL is used */
        /* while(OSCCONbits.LOCK != 1); */
#endif

//}
