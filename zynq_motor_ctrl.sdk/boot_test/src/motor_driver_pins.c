#include "motor_driver_pins.h"

#define SPI_DEVICE_ID		XPAR_XSPIPS_0_DEVICE_ID
#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID

XSpiPs spi_a;
XGpioPs gpio_emio;
u8 freq_sck;

u8 read_arry[30] = {0};
u8 write_arry[30] = {0};

s32 motor_exio_all_ctrl_pins(void){

	s32 Status;
	u32 io_read;
	XGpioPs_Config *Config;

	Config = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XGpioPs_CfgInitialize(&gpio_emio, Config, Config->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XGpioPs_SelfTest(&gpio_emio);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	io_read = XGpioPs_Read(&gpio_emio, 2);
	io_read = (io_read & 0xFFFFFE00);
	XGpioPs_Write(&gpio_emio, 2, io_read);

	XGpioPs_SetDirection(&gpio_emio, 2, 0x000001FF);
	XGpioPs_SetOutputEnable(&gpio_emio, 2, 0x000001FF);

	// this reset the external SPI GPIO
	XGpioPs_WritePin(&gpio_emio, emio_spi_xio_nrst, pin_on);
	usleep(100);
	XGpioPs_WritePin(&gpio_emio, emio_spi_xio_nrst, pin_off);
	usleep(10);
	XGpioPs_WritePin(&gpio_emio, emio_spi_xio_nrst, pin_on);
	usleep(10);

	return XST_SUCCESS;
}

void zynq_write_fw_bw_pins(u32 Pin, u32 pin_state){
	XGpioPs_WritePin(&gpio_emio, Pin, pin_state);
}

u32 zynq_read_fw_bw_pins(u32 Pin){

	u32 pin_state;

	pin_state = XGpioPs_ReadPin(&gpio_emio, Pin);

	return pin_state;
}

s32 zynq_to_motor_init(void){

	s32 Status;
	XSpiPs_Config *Config;

	motor_exio_all_ctrl_pins();

	Config = XSpiPs_LookupConfig(SPI_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XSpiPs_CfgInitialize(&spi_a, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XSpiPs_SelfTest(&spi_a);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	Status = XSpiPs_SetClkPrescaler(&spi_a, XSPIPS_CLK_PRESCALE_256);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	freq_sck = XSpiPs_GetClkPrescaler(&spi_a);


	Status = XSpiPs_SetOptions(&spi_a, XSPIPS_MASTER_OPTION |
				XSPIPS_DECODE_SSELECT_OPTION |
				XSPIPS_CLK_ACTIVE_LOW_OPTION |
				XSPIPS_CLK_PHASE_1_OPTION
			);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XSpiPs_SetSlaveSelect(&spi_a, 0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XSpiPs_Enable(&spi_a);

	// read the EXIO configuration register
	write_arry[0] = 0x41;
	Status = XSpiPs_PolledTransfer(&spi_a, write_arry, read_arry, 24);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// write the EXIO configuration register
	write_arry[0] = 0x40;
	write_arry[2] = 0x00;	// IO direction A
	write_arry[3] = 0x00;	// IO direction B
	Status = XSpiPs_PolledTransfer(&spi_a, write_arry, NULL, 4);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	write_arry[1] = 0x0C;
	write_arry[2] = 0xff;	// IO pull-up A
	write_arry[3] = 0xff;	// IO pull-up B
	Status = XSpiPs_PolledTransfer(&spi_a, write_arry, NULL, 4);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	write_arry[1] = 0x12;
	write_arry[2] = 0x00;	// IO A
	write_arry[3] = 0x00;	// IO B
	Status = XSpiPs_PolledTransfer(&spi_a, write_arry, NULL, 4);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This API is untilizing the BANK=0 mode at the SPI IO extension.
// The addressing is sequential when controlling the registers.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
s32 zynq_to_motor_ctrl(u8 io_a, u8 io_b){

	s32 Status;

	write_arry[0] = 0x40;
	write_arry[1] = 0x12;
	write_arry[2] = io_a;	// IO A
	write_arry[3] = io_b;	// IO B
	
	Status = XSpiPs_PolledTransfer(&spi_a, write_arry, NULL, 4);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
