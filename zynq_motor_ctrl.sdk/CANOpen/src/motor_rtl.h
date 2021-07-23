/*
 * motor_rtl.h
 *
 *  Created on: 25 Feb 2020
 *      Author: whwong
 */

#ifndef SRC_MOTOR_RTL_H_
#define SRC_MOTOR_RTL_H_

#include <stdio.h>
#include "platform.h"

#include "xparameters.h"
#include "xplatform_info.h"
#include "xgpiops.h"

#include "config.h"

#define AXI_GPIO_0_DEVICE_ID	XPAR_AXI_GPIO_0_DEVICE_ID
#define AXI_GPIO_1_DEVICE_ID	XPAR_AXI_GPIO_1_DEVICE_ID

s32 AXIGPIOInit(void);

#endif /* SRC_MOTOR_RTL_H_ */
