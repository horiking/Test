/////////////////////////////////////////////////////////////////
// Autor: Thomas Kramer, Institut für Fluidtechnik, TU Dresden //
/////////////////////////////////////////////////////////////////

#ifndef INIT_H
#define	INIT_H


/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        50000000UL
#define XTAL_FREQ       20000000UL
#define FCY             SYS_FREQ
// In XC16 they use "FCY", which is the instruction clock frequency, which is 1/2 of the oscillator frequency. 

#define ECAN_ID         1

#define PERIOD_PWM      12500        
#define PERIOD_DAC      PERIOD_PWM/2      
#define CYC_CAN         4

//#define led_green     LATBbits.LATB6
//#define led_red       LATBbits.LATB7


typedef unsigned char BYTE;
typedef unsigned short int  WORD;

typedef union {
    struct {
        unsigned adc_i         :1;
        unsigned adc_u         :1;
        unsigned adc_finished  :1;
        unsigned outofband     :1;
        unsigned normal        :1;
        unsigned last_sw_op    :1;  // last switching operation
        unsigned current_state :1;  // current state
        unsigned state_unknown :1;  // Ld in Hysteresis area
    };
} state_t;

/*
typedef union {
    struct {
        unsigned start_open    :1;
        unsigned start_close   :1;
        unsigned start_state   :1;
        unsigned open          :1;
        unsigned close         :1;
        unsigned state_pos     :1;
        unsigned state_neg     :1;
    };
} valve_mode_t;
*/
/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

//void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
/*
void timer0_init(void);
void timer1_init(void);

void int_init(void);*/

#endif