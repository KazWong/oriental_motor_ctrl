#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_types.h"

#include "sleep.h"
#include "xtime_l.h"

#include "motor_dac_ctrl.h"
#include "zynq_can.h"

#include "motion.h"

XCanPs Can;

int Init() {
	//s32 Status;
	//u8 START[] = {'s', 't', 'a', 'r', 't', '0', '0', '1'};

	//int count = 0;

	init_platform();
	print("System Boot.\n\r");
// Motor
	MotionInit();

// CAN
    CanPsPolledExample(CAN_DEVICE_ID, &Can);

// Ready Msg
    /*while (count < 10) {
    	START[7] = count;
    	Status = SendFrame(&Can, count, START, (u8) sizeof(START));
    	if (Status != XST_SUCCESS) {
    		print("Send failed.\n\r");
    		return XST_FAILURE;
    	}
    	count++;
    	usleep(1000);
    }*/
    return XST_SUCCESS;
}

void End() {
	//u8 EXIT[] = {'5', '5', '5', '5', '5', '5', '5', '5'};

	MotorOFF();

	//SendFrame(&Can, 0x66, EXIT, (u8) sizeof(EXIT));

    cleanup_platform();
}


void CANout(_Float64 fb_dt) {
	s16 int_s16 = 0;
	u8 msg_11[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	u8 msg_22[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	u8 msg_33[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	u8 msg_44[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	u8 msg_55[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	u8 msg_66[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	int_s16 = 0;
	for (int i=0;i<4;i++) {
		s8 *ptr = (s8*)&int_s16;
		int_s16 = (s16)(s[i] * 100.);
		msg_11[2*i] = *(ptr);
		msg_11[2*i+1] = *(ptr+1);
	}

	int_s16 = 0;
	for (int i=0;i<4;i++) {
		s8 *ptr = (s8*)&int_s16;
		int_s16 = (s16)(32767. * a[i] / LIM_SPEED);
		msg_22[2*i] = *(ptr);
		msg_22[2*i+1] = *(ptr+1);
	}

	int_s16 = 0;
	for (int i=0;i<4;i++) {
		s8 *ptr = (s8*)&int_s16;
		int_s16 = (s16)(32767. * y[0][i] / LIM_SPEED);
		msg_33[2*i] = *(ptr);
		msg_33[2*i+1] = *(ptr+1);
	}

	int_s16 = 0;
	for (int i=0;i<4;i++) {
		s8 *ptr = (s8*)&int_s16;
		int_s16 = (s16)(32767. * w[i] / LIM_SPEED);
		msg_44[2*i] = *(ptr);
		msg_44[2*i+1] = *(ptr+1);
	}

	for (int i=0;i<4;i++) {
		u8 *ptr = (u8*)&odom[0];
		msg_55[i] = *(ptr+i);
	}

	for (int i=0;i<4;i++) {
		u8 *ptr = (u8*)&odom[1];
		msg_55[i+4] = *(ptr+i);
	}

	for (int i=0;i<4;i++) {
		u8 *ptr = (u8*)&odom[2];
		msg_66[i] = *(ptr+i);
	}

	SendFrame(&Can, 0x11, msg_11, (u8) sizeof(msg_11));//pos
	SendFrame(&Can, 0x22, msg_22, (u8) sizeof(msg_22));//acc
	SendFrame(&Can, 0x33, msg_33, (u8) sizeof(msg_33));//vel
	SendFrame(&Can, 0x44, msg_44, (u8) sizeof(msg_44));//cmd
	SendFrame(&Can, 0x55, msg_55, (u8) sizeof(msg_55));//odom xy
	SendFrame(&Can, 0x66, msg_66, (u8) sizeof(msg_66));//odom z
}

int main(void) {
	u8 dac_addr[] = {DAC_IIC_ADDR_A, DAC_IIC_ADDR_B, DAC_IIC_ADDR_C, DAC_IIC_ADDR_D};
	XTime t_start, t_now, t_ctrl, t_speedout, t_cmd, t_Ta, t_Tca, t_CANout, t_est_a, t_est_ca;
	XTime t_p[4];
	double  elapsed_time = 0.0;
	float ctrl_t = 1./100., fb_hz = 500., cmd_hz = 60., can_hz = 20., est_a_hz = 20., est_ca_hz = 20.;

	Init();
	MotorBootUp();

	/*5s Startup time
	for (int i=0;i<4;i++) {
		zynq_write_fw_bw_pins(pin_fw[i], pin_off);
		zynq_write_fw_bw_pins(pin_bw[i], pin_off);
	}
	XTime_GetTime(&t_start);
	XTime_GetTime(&t_now);
	while (1.0 > 1.0 * (t_now - t_start) / (COUNTS_PER_SECOND)) XTime_GetTime(&t_now);

	*/

	XTime_GetTime(&t_start);
	XTime_GetTime(&t_now);
	XTime_GetTime(&t_speedout);
	XTime_GetTime(&t_cmd);
	XTime_GetTime(&t_est_a);
	XTime_GetTime(&t_est_ca);
	XTime_GetTime(&t_ctrl);
	XTime_GetTime(&t_CANout);
	for (int i=0;i<4;i++)
		XTime_GetTime(&t_p[i]);

	while (elapsed_time < duration) {
		XTime_GetTime(&t_now);
		if (1./fb_hz <= ( 1.0 * (t_now - t_speedout) / (COUNTS_PER_SECOND) )) {
			SpeedOut((_Float64)(1.0 * (t_now - t_speedout) / (COUNTS_PER_SECOND)));
			XTime_GetTime(&t_speedout);
		}

		XTime_GetTime(&t_now);
		if (1./can_hz <= ( 1.0 * (t_now - t_CANout) / (COUNTS_PER_SECOND) )) {
			CANout((_Float64)(1.0 * (t_now - t_Ta) / (COUNTS_PER_SECOND)));
			XTime_GetTime(&t_CANout);
		}

		XTime_GetTime(&t_now);
		if (1./est_a_hz <= ( 1.0 * (t_now - t_est_a) / (COUNTS_PER_SECOND) )) {
			XTime_GetTime(&t_Ta);
			Differentiator(a_y, y_0, a, (_Float64)(1.0 * (t_now - t_est_a) / (COUNTS_PER_SECOND)));
			XTime_GetTime(&t_est_a);
		}

		XTime_GetTime(&t_now);
		if (1./est_ca_hz <= ( 1.0 * (t_now - t_est_ca) / (COUNTS_PER_SECOND) )) {
			XTime_GetTime(&t_Tca);
			Differentiator(a_v, v, cmd_a, (_Float64)(1.0 * (t_now - t_est_ca) / (COUNTS_PER_SECOND)));
			XTime_GetTime(&t_est_ca);
		}

		XTime_GetTime(&t_now);
		if (1./cmd_hz <= ( 1.0 * (t_now - t_cmd) / (COUNTS_PER_SECOND) )) {
			Setpoint((_Float64)(1.0 * (t_now - t_cmd) / (COUNTS_PER_SECOND)), &Can);
			XTime_GetTime(&t_cmd);
		}

		XTime_GetTime(&t_now);
		if (ctrl_t <= ( 1.0 * (t_now - t_ctrl) / (COUNTS_PER_SECOND) )) {
			Integrator((_Float64)(1.0 * (t_now - t_ctrl) / (COUNTS_PER_SECOND)));
			Ctrl((_Float64)(1.0 * (t_now - t_ctrl) / (COUNTS_PER_SECOND)),
								(_Float64)(1.0 * (t_now - t_Ta) / (COUNTS_PER_SECOND)),
								(_Float64)(1.0 * (t_now - t_cmd) / (COUNTS_PER_SECOND)));
			XTime_GetTime(&t_ctrl);
		}

		for (int i=0;i<4;i++) {
			XTime_GetTime(&t_now);
			if (p_t[i] <= ( 1.0 * (t_now - t_p[i]) / (COUNTS_PER_SECOND) )) {
				Plant(i, dac_addr[i]);
				XTime_GetTime(&t_p[i]);
			}
		}

		XTime_GetTime(&t_now);
		elapsed_time = 1.0 * (t_now - t_start) / (COUNTS_PER_SECOND);
	}

	SoftStop(dac);
	End();
	return 0;
}
