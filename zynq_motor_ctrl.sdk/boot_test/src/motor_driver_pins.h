#ifndef _MOTOR_DRIVER_PINS_H
#define _MOTOR_DRIVER_PINS_H

#include <stdio.h>
#include "platform.h"

#include "xparameters.h"
#include "xspips.h"
#include "xgpiops.h"
#include "xplatform_info.h"

#include "sleep.h"

// These control signals are controlled by GPIOB (MCP23S18)
#define	MTC_A_M0			0x80
#define	MTC_A_MB_FREE		0x40
#define	MTC_A_STPM			0x20
#define	MTC_A_ALRM_RST		0x10
#define	MTC_B_M0			0x08
#define	MTC_B_MB_FREE		0x04
#define	MTC_B_STPM			0x02
#define	MTC_B_ALRM_RST		0x01

// These control signals are controlled by GPIOA (MCP23S18)
#define	MTC_C_M0			0x80
#define	MTC_C_MB_FREE		0x40
#define	MTC_C_STPM			0x20
#define	MTC_C_ALRM_RST		0x10
#define	MTC_D_M0			0x08
#define	MTC_D_MB_FREE		0x04
#define	MTC_D_STPM			0x02
#define	MTC_D_ALRM_RST		0x01

// IO pin number
#define	emio_spi_xio_nrst		54
#define	emio_mot_a_fw			55
#define	emio_mot_a_bw			56
#define	emio_mot_b_fw			57
#define	emio_mot_b_bw			58
#define	emio_mot_c_fw			59
#define	emio_mot_c_bw			60
#define	emio_mot_d_fw			61
#define	emio_mot_d_bw			62

#define pin_on					0x01
#define pin_off					0x00

s32 motor_exio_all_ctrl_pins(void);
s32 zynq_to_motor_init(void);
s32 zynq_to_motor_ctrl(u8 io_a, u8 io_b);

void zynq_write_fw_bw_pins(u32 Pin, u32 pin_state);
u32 zynq_read_fw_bw_pins(u32 Pin);

#endif
