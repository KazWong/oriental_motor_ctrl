#ifndef _MOTOR_DAC_CTRL_H
#define _MOTOR_DAC_CTRL_H

#include <stdio.h>
#include "platform.h"

#include "xparameters.h"
#include "xiicps.h"
#include "xplatform_info.h"

#include "sleep.h"

#define IIC_DEVICE_ID			XPAR_XIICPS_0_DEVICE_ID
#define IIC_SLAVE_ADDR			0x55
#define SLV_MON_LOOP_COUNT		0x00FFFFFF

#define DAC_IIC_ADDR_A			0x65
#define DAC_IIC_ADDR_B			0x64
#define DAC_IIC_ADDR_C			0x63
#define DAC_IIC_ADDR_D			0x62

s32 dac_init_val(void);
s32 dac_send_four_val(u8* write_arry);
s32 dac_send_val(u8 addr, u8* write_arry);

#endif
