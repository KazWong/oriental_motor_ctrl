/*
 * motor_axi_gpio.c
 *
 *  Created on: 26 Feb 2020
 *      Author: whwong
 */


#include "motor_axi_gpio.h"

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
