/** MCP4725A2T-E/CH **/

#ifndef _MOTOR_DAC_H
#define _MOTOR_DAC_H

#include <stdio.h>
#include "platform.h"

#include "xparameters.h"
#include "xiicps.h"
#include "xplatform_info.h"

#include "sleep.h"
#include "config.h"


#define IIC_DEVICE_ID		XPAR_XIICPS_0_DEVICE_ID
#define IIC_SCLK_RATE		90000
//#define IIC_SLAVE_ADDR			0x55
//#define SLV_MON_LOOP_COUNT		0x00FFFFFF

s32 dac_init_val(void);
s32 dac_send_all_val(u8* write_arry, u8 len);
s32 dac_send_val(u8 addr, u8* write_arry);

#endif
