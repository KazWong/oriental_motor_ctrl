#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xbasic_types.h"

#include "sleep.h"

#include "motor_rtl.h"
#include "motor_dac.h"
#include "motor_gpio.h"
#include "zynq_can.h"

XCanPs Can;

u32 RxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS] = {0};

int main(void){
	s32 Status;

    init_platform();
    InitWheels();
// Motor
    zynq_to_motor_init();
    dac_init_val();
    zynq_to_motor_ctrl(0x88, 0x88);
// CAN
    CanPsPolledExample(CAN_DEVICE_ID, &Can);
// AXI_GPIO
    AXIGPIOInit();

    while(1) {

    }

    cleanup_platform();
    return 0;
}
