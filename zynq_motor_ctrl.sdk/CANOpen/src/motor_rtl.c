/*
 * motor_rtl.c
 *
 *  Created on: 25 Feb 2020
 *      Author: whwong
 */

#include "motor_rtl.h"

XGpioPs axi_gpio_0, axi_gpio_1;

s32 AXIGPIOInit(void){
	s32 Status;
	XGpioPs_Config *Config;

	Config = XGpioPs_LookupConfig(AXI_GPIO_0_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XGpioPs_CfgInitialize(&axi_gpio_0, Config, Config->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Config = XGpioPs_LookupConfig(AXI_GPIO_1_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XGpioPs_CfgInitialize(&axi_gpio_1, Config, Config->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

void UpdateSpeed() {
	for (int i=0;i<NUM_OF_WHEELS;i++) {
		wheels[i].fb_vel = (float)(MAX_SPD * Xil_In32(speedout_addr[i]) / counter_Hz);
	}
}
