/*
 * motor_axi_gpio.h
 *
 *  Created on: 26 Feb 2020
 *      Author: whwong
 */

#ifndef SRC_MOTOR_AXI_GPIO_H_
#define SRC_MOTOR_AXI_GPIO_H_

#include <stdio.h>
#include "platform.h"

#include "xparameters.h"
#include "xplatform_info.h"
#include "xgpiops.h"

#define AXI_GPIO_0_DEVICE_ID	XPAR_AXI_GPIO_0_DEVICE_ID
#define AXI_GPIO_1_DEVICE_ID	XPAR_AXI_GPIO_1_DEVICE_ID

s32 AXIGPIOInit(void);

#endif /* SRC_MOTOR_AXI_GPIO_H_ */
