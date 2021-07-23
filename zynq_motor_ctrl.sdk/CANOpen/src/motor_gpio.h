#ifndef _MOTOR_GPIO_H
#define _MOTOR_GPIO_H

#include <stdio.h>
#include "platform.h"

#include "xparameters.h"
#include "xspips.h"
#include "xgpiops.h"
#include "xplatform_info.h"

#include "sleep.h"
#include "config.h"

#define pin_on					0x01
#define pin_off					0x00

s32 motor_exio_all_ctrl_pins(void);
s32 zynq_to_motor_init(void);
s32 zynq_to_motor_ctrl(u8 io_a, u8 io_b);

s32 zynq_write_dir(wheelConfig pos, dirState state);
dirState zynq_read_dir(wheelConfig pos);
void zynq_write_pins(u32 Pin, u32 pin_state);
u32 zynq_read_pins(u32 Pin);

#endif
