/*
 * File:   ecan.h
 * Author: d.lohse
 *
 * Created on 2. Juli 2014, 16:45
 */

// bit rate: 100 kbit/s
// Tbit = 10 µs
// Tosc = 0,015625 µs
// Tbit != TQ * 16 -> TQ = 0,625 µs
// -> BRP = 19

#ifndef ECAN_H
#define	ECAN_H

#include "init.h"

#define ECAN_FREQ       1000000UL

#define ECAN_FUNC_MODE_VAL ECAN_MODE_0

#define TQcount 5
#define Tbit 1/(double)ECAN_FREQ
#define TQ Tbit/TQcount

// BRP value - Value must be between 1-64 inclusive.
#define ECAN_BRP_VAL (uint16_t)((TQ*SYS_FREQ/2)-1) // 19
// -> mit 40 MHz: TQ = 50 ns
// -> mit 50 MHz: TQ = 40 ns

// infos p.451
// SJW value - Value must be between 1-4 inclusive.
#define ECAN_SJW_VAL 1//1//1

// PHSEG1 value - Value must be between 1-8 inclusive.
#define ECAN_PHSEG1_VAL 1//3//7

// PHSEG2 value - Value must be between 1-8 inclusive.
#define ECAN_PHSEG2_VAL 1//3//6

// PROPSEG value - Value must be between 1-8 inclusive.
#define ECAN_PROPSEG_VAL 2//1//2

// Use freely progrmable PHSEG2 or calculated value.
#define ECAN_PHSEG2_MODE_VAL ECAN_PHSEG2_MODE_PROGRAMMABLE
    #define ECAN_PHSEG2_MODE_AUTOMATIC      0
    #define ECAN_PHSEG2_MODE_PROGRAMMABLE   1
//
// Bus Sample Mode
#define ECAN_BUS_SAMPLE_MODE_VAL ECAN_BUS_SAMPLE_MODE_THRICE        // ZU TESTEN
    #define ECAN_BUS_SAMPLE_MODE_ONCE       0
    #define ECAN_BUS_SAMPLE_MODE_THRICE     1
//
// Wakeup Mode
#define ECAN_WAKEUP_MODE_VAL ECAN_WAKEUP_MODE_ENABLE
    #define ECAN_WAKEUP_MODE_ENABLE         0
    #define ECAN_WAKEUP_MODE_DISABLE        1
//
// Filter Mode
#define ECAN_FILTER_MODE_VAL ECAN_FILTER_MODE_DISABLE
    #define ECAN_FILTER_MODE_DISABLE        0
    #define ECAN_FILTER_MODE_ENABLE         1
//
// CANTX1 Drive Mode
#define ECAN_TXDRIVE_MODE_VAL ECAN_TXDRIVE_MODE_VDD                
    #define ECAN_TXDRIVE_MODE_TRISTATE  0
    #define ECAN_TXDRIVE_MODE_VDD       1
//
// CANTX2 pin mode
#define ECAN_TX2_MODE_VAL ECAN_TX2_MODE_DISABLE                // ZU TESTEN
    #define ECAN_TX2_MODE_DISABLE   0
    #define ECAN_TX2_MODE_ENABLE    1
//
// CANTX2 Source
#define ECAN_TX2_SOURCE_VAL ECAN_TX2_SOURCE_COMP                // ZU TESTEN
    #define ECAN_TX2_SOURCE_COMP    0
    #define ECAN_TX2_SOURCE_CLOCK   1
//
// CAN Capture Mode
#define ECAN_CAPTURE_MODE_VAL ECAN_CAPTURE_MODE_DISABLE                // ZU TESTEN
    #define ECAN_CAPTURE_MODE_DISABLE   0
    #define ECAN_CAPTURE_MODE_ENABLE    1
// CLKSEL
    // use 0 for pll-clock

    #define ECAN_MSG_STD    0
    #define ECAN_MSG_XTD    1

// functional mode
typedef enum _ECAN_FUNC_MODE {
    ECAN_MODE_0 =    0x00,
    ECAN_MODE_1 =    0x40,
    ECAN_MODE_2 =    0x80
} ECAN_FUNC_MODE;

// operation mode
typedef enum _ECAN_OP_MODE {
    ECAN_OP_MODE_NORMAL  = 0,
    ECAN_OP_MODE_DISABLE = 1,
    ECAN_OP_MODE_LOOP    = 2,
    ECAN_OP_MODE_LISTEN  = 3,
    ECAN_OP_MODE_CONFIG  = 4
} ECAN_OP_MODE;

typedef enum _ECAN_TX_MSG_FLAGS
 {
    ECAN_TX_PRIORITY_BITS    = 0b00000011,
    ECAN_TX_PRIORITY_0       = 0b00000000,
    ECAN_TX_PRIORITY_1       = 0b00000001,
    ECAN_TX_PRIORITY_2       = 0b00000010,
    ECAN_TX_PRIORITY_3       = 0b00000011,

    ECAN_TX_FRAME_BIT        = 0b00100000,
    ECAN_TX_STD_FRAME        = 0b00000000,
    ECAN_TX_XTD_FRAME        = 0b00100000,

    ECAN_TX_RTR_BIT          = 0b01000000,
    ECAN_TX_NO_RTR_FRAME     = 0b00000000,
    ECAN_TX_RTR_FRAME        = 0b01000000
} ECAN_TX_MSG_FLAGS;

typedef union _CAN_MESSAGE_ID
{
    unsigned long ID;

    struct
    {
        struct
        {
            unsigned SIDL:3;        // SIDL<5:7>
            unsigned SIDH:5;        // SIDH<0:4>
        } BYTE1;
        struct
        {
            unsigned SIDHU:3;       // SIDH<5:7>
            unsigned EIDL_LN:5;     // EIDL<0:4>
        } BYTE2;
        struct
        {
            unsigned EIDL_UN:3;     // EIDL<5:7>
            unsigned EIDH_LN:5;     // EIDH<0:4>
        } BYTE3;
        struct
        {
            unsigned EIDH_UN:3;     // EIDH<5:7>
            unsigned EIDHU:2;       // SIDL<0:1>
            unsigned :3;
        } BYTE4;
    } ID_VALS;

    // This is to allow individual byte access within message id.
    struct
    {
        BYTE BYTE_1;
        BYTE BYTE_2;
        BYTE BYTE_3;
        BYTE BYTE_4;
    } BYTES;
} CAN_MESSAGE_ID;

//void ECANSetFunctionalMode(ECAN_FUNC_MODE mode);

void ecan_set_op_mode(ECAN_OP_MODE mode);
ECAN_OP_MODE ecan_get_op_mode(void);

void ecan_init(void);

uint16_t ecan_read_message(uint8_t n);

void ecan_send_message(WORD* data, uint8_t datalength, unsigned int id);
//void sendmessage(unsigned char sendebyte);

//BOOL ECANSendMessage( unsigned long id, BYTE* data, BYTE dataLen, ECAN_TX_MSG_FLAGS msgFlags);

//void can_transmit(uint8_t buffer);

#endif	/* ECAN_H */

