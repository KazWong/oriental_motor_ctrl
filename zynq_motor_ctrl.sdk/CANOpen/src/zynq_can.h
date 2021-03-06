#ifndef _ZYNQ_CAN_H
#define _ZYNQ_CAN_H

#include "platform.h"
#include "xil_printf.h"

#include "xparameters.h"
#include "xcanps.h"

#include "xplatform_info.h"

#include "sleep.h"


#define CAN_DEVICE_ID	XPAR_XCANPS_0_DEVICE_ID

#define XCANPS_MAX_FRAME_SIZE_IN_WORDS (XCANPS_MAX_FRAME_SIZE / sizeof(u32))

#define FRAME_DATA_LENGTH 		8  /* Frame Data field length */

#define TEST_MESSAGE_ID			0x88

/*
 * Timing parameters to be set in the Bit Timing Register (BTR).
 * These values are for a 1000 Kbps baudrate assuming the CAN input clock
 frequency
 * is 100 MHz.
 */
#define TEST_BTR_SYNCJUMPWIDTH		3
// this value is counted from zero
#define TEST_BTR_SECOND_TIMESEGMENT	3
// this value is counted from zero
#define TEST_BTR_FIRST_TIMESEGMENT	4
// this two value 1st ts and 2nd ts combine to form a BRPR
// which BRPR is then added 1 by default

// NTQ = [(TS1 + 1) + (TS2 + 1) + 1]
// Baud Rate - clock / ( NTQ x BPR )

#define TEST_BRPR_BAUD_PRESCALAR	9



/* Driver instance */


s32 CanPsPolledExample(u16 DeviceId, XCanPs *CanInstPtr);

s32 SendFrame(XCanPs *InstancePtr, u32 txrx_id, u8* frame_data, u8 len);
s32 RecvFrame(XCanPs *InstancePtr, u32 txrx_id, u32* RxFrame, u32 timeout);


#endif
