#include "motor_dac_ctrl.h"

XIicPs Iic;
u32 freq_scl;

s32 dac_init_val(void){

	s32 Status;
	XIicPs_Config *Config;

	u8 write_arry[3] = {0};	// DAC value

	Config = XIicPs_LookupConfig(IIC_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XIicPs_SelfTest(&Iic);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	XIicPs_SetSClk(&Iic, 90000);
	freq_scl = XIicPs_GetSClk(&Iic);

	Status = XIicPs_MasterSendPolled(&Iic, write_arry, 2, DAC_IIC_ADDR_A);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	Status = XIicPs_MasterSendPolled(&Iic, write_arry, 2, DAC_IIC_ADDR_B);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	Status = XIicPs_MasterSendPolled(&Iic, write_arry, 2, DAC_IIC_ADDR_C);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	Status = XIicPs_MasterSendPolled(&Iic, write_arry, 2, DAC_IIC_ADDR_D);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	return XST_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// the write_arry format is:
// DAC-A: 0x##, 0x##, 0x00
// DAC-B: 0x##, 0x##, 0x00
// DAC-C: 0x##, 0x##, 0x00
// DAC-D: 0x##, 0x##, 0x00
////////////////////////////////////////////////////////////////////////////////
s32 dac_send_four_val(u8* write_arry){

	s32 Status;
	u8* ary_ptr = write_arry;

	Status = XIicPs_MasterSendPolled(&Iic, ary_ptr, 2, DAC_IIC_ADDR_A);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	ary_ptr = ary_ptr + 2;

	Status = XIicPs_MasterSendPolled(&Iic, ary_ptr, 2, DAC_IIC_ADDR_B);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	ary_ptr = ary_ptr + 2;

	Status = XIicPs_MasterSendPolled(&Iic, ary_ptr, 2, DAC_IIC_ADDR_C);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	ary_ptr = ary_ptr + 2;

	Status = XIicPs_MasterSendPolled(&Iic, ary_ptr, 2, DAC_IIC_ADDR_D);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	return XST_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// the write_arry format is:
// DAC-#: 0x##, 0x##, 0x00
////////////////////////////////////////////////////////////////////////////////
s32 dac_send_val(u8 addr, u8* write_arry){

	s32 Status;

	Status = XIicPs_MasterSendPolled(&Iic, write_arry, 2, addr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	while (XIicPs_BusIsBusy(&Iic));

	return XST_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
