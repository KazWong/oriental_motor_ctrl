/*
 * config.h
 *
 *  Created on: 25 Feb 2020
 *      Author: whwong
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xplatform_info.h"

//Parameters of the agv
#define NUM_OF_WHEELS	4
#define r				0.07825
#define lx				0.421
#define ly				0.421
#define MAX_SPD			2.0*3.141592653589*r / 150.0

/** DAC **/
#define DAC_IIC_ADDR_A			0x65
#define DAC_IIC_ADDR_B			0x64
#define DAC_IIC_ADDR_C			0x63
#define DAC_IIC_ADDR_D			0x62

/** GPIO **/
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
#define	emio_mot_A_fw			55
#define	emio_mot_A_bw			56
#define	emio_mot_B_fw			57
#define	emio_mot_B_bw			58
#define	emio_mot_C_fw			59
#define	emio_mot_C_bw			60
#define	emio_mot_D_fw			61
#define	emio_mot_D_bw			62

/** AXI GPIO **/
#define counter_Hz			131
#define mot_A_speedout		0x41200000
#define mot_B_speedout		0x41200008
#define mot_C_speedout		0x41210000
#define mot_D_speedout		0x41210008


typedef enum dir_state {STOP, BWD, FWD, EM_STOP} dirState;
typedef enum gpio_pin_mapping {M0, MB_FREE, STPM, ALRM_RST, FW, BW} pinMapping;
typedef enum Configuration {FL, FR, BL, BR} wheelConfig;
typedef struct _wheel{
	dirState state;
	float cmd_vel;
	float fb_vel;
	u8 alarm;
	u8 dir_pins[2];
	u8 pins[4];
} wheel;

u32 speedout_addr[4];
u8 dac_addr[4];
wheel wheels[4];

void InitWheels();

#endif /* SRC_CONFIG_H_ */
