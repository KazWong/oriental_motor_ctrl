#include <math.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_types.h"

#include "sleep.h"
#include "xtime_l.h"

#include "motor_dac_ctrl.h"
#include "zynq_can.h"

#include "motion.h"
#include "canopen.h"

#define DEBUG

double duration = 60000.0;
XTime t_speedout, t_est_a, t_est_ca, t_in, t_cmd;

#ifdef DEBUG
#include <stdio.h>
static inline void Console(char* msg) {
	printf("%s\n\r", msg);
}
#else
static inline void Console(char* msg) {

}
#endif

int Init() {
	init_platform();
	Console("System Boot.");

	CANOpenInit();
	MotionInit();

    return XST_SUCCESS;
}

void End() {
	MotorOFF();
    cleanup_platform();
}

void PushPos(int id, double msg[4]) {
	s16 int_s16 = 0;
	u8 u8_msg[8];

	int_s16 = 0;
	for (int i=0;i<4;i++) {
		s8 *ptr = (s8*)&int_s16;
		int_s16 = (s16)(msg[i] * 100.);
		u8_msg[2*i] = *(ptr);
		u8_msg[2*i+1] = *(ptr+1);
	}

	PushMsgBuf(id, 8, u8_msg);
}

void PushState(int id, double msg[4]) {
	s16 int_s16 = 0;
	u8 u8_msg[8];

	int_s16 = 0;
	for (int i=0;i<4;i++) {
		s8 *ptr = (s8*)&int_s16;
		int_s16 = (s16)(32767. * msg[i] / LIM_SPEED);
		u8_msg[2*i] = *(ptr);
		u8_msg[2*i+1] = *(ptr+1);
	}

	PushMsgBuf(id, 8, u8_msg);
}

void PushOdom(int id, double msg) {
	u8 u8_msg[8];

	for (int i=0;i<4;i++) {
		u8 *ptr = (u8*)&msg;
		u8_msg[i] = *(ptr+i);
	}

	PushMsgBuf(id, 8, u8_msg);
}

void ResetTimer() {
	ResetMotionTimer();
	XTime_GetTime(&t_speedout);
	XTime_GetTime(&t_est_a);
	XTime_GetTime(&t_est_ca);
	XTime_GetTime(&t_in);
	XTime_GetTime(&t_cmd);
}

void UpdateVel() {
	XTime t_now;

	XTime_GetTime(&t_now);
	if (CMD_T <= ( 1.0 * (t_now - t_cmd) / (COUNTS_PER_SECOND) )) {
		SetVel(vel_frame, (double)(1.0 * (t_now - t_cmd) / (COUNTS_PER_SECOND)));
		XTime_GetTime(&t_cmd);
		PushState(PDO_WHEEL_W, w);
	}
}

void UpateSense() {
	XTime t_now;

	XTime_GetTime(&t_now);
	if (FB_T <= ( 1.0 * (t_now - t_speedout) / (COUNTS_PER_SECOND) )) {
		double y0[4] = {y[0][0], y[0][1], y[0][2], y[0][3]};
		SpeedOut((double)(1.0 * (t_now - t_speedout) / (COUNTS_PER_SECOND)));
		XTime_GetTime(&t_speedout);
		if (state == OP) {
			PushState(PDO_WHEEL_Y, y0);
		}
	}

	XTime_GetTime(&t_now);
	if (EST_A_T <= ( 1.0 * (t_now - t_est_a) / (COUNTS_PER_SECOND) )) {
		Differentiator(a_y, y_0, a, (double)(1.0 * (t_now - t_est_a) / (COUNTS_PER_SECOND)));
		XTime_GetTime(&t_est_a);
		if (state == OP) {
			PushState(PDO_WHEEL_A, a);
		}
	}

	XTime_GetTime(&t_now);
	if (EST_CA_T <= ( 1.0 * (t_now - t_est_ca) / (COUNTS_PER_SECOND) )) {
		Differentiator(a_v, v, cmd_a, (double)(1.0 * (t_now - t_est_ca) / (COUNTS_PER_SECOND)));
		XTime_GetTime(&t_est_ca);
	}

	XTime_GetTime(&t_now);
	if (IN_T <= ( 1.0 * (t_now - t_in) / (COUNTS_PER_SECOND) )) {
		Integrator((double)(1.0 * (t_now - t_in) / (COUNTS_PER_SECOND)));
		XTime_GetTime(&t_in);
		if (state == OP) {
			PushPos(PDO_WHEEL_S, s);
			PushOdom(PDO_ODOM_X, odom[0]);
			PushOdom(PDO_ODOM_Y, odom[1]);
			PushOdom(PDO_ODOM_Z, odom[2]);
		}
	}
}

u8 SoftStop_NonBlk() {
	for (int i=0;i<4;i++) {
		if (fabs(0.0 - y[0][i]) > 0.00000000001) {
			v[0] = v[1] = v[2] = v[3] = 0.0;

			UpateSense();
			Ctrl_Plant();
			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}

void BDCmd(u8 opt) {
	switch (opt) {
	case 0xEF:
		duration = 0;
		break;
	case 0xDD:
		s[0] = s[1] = s[2] = s[3] = 0.0;
		odom[0] = odom[1] = odom[2] = 0.0;
		break;
	case 0xDF:
		u[0] = u[1] = u[2] = u[3] = 0.0;
		break;
	default:
		break;
	}
	opt = 0x00;
}

u8 BootUp(u8 ret) {
	switch (ret) {
	case ENTRY:
		Console("BootUp Entry.");
		if (SoftStop_NonBlk() == XST_SUCCESS )
			return LOOP;
		else
			return ENTRY;
	case LOOP:
		Console("BootUp Loop.");
		CANOpenBootUp();
		MotorBootUp();
		ClearMotionState();
		ResetTimer();
		return EXIT;
	case EXIT:
		Console("BootUp Exit.");
		state = PREOP;
		return ENTRY;
	};
	return 51;
}


u8 Reset(u8 ret) {
	switch (ret) {
	case ENTRY:
		Console("Reset Entry.");
		if (SoftStop_NonBlk() == XST_SUCCESS )
			return LOOP;
		else
			return ENTRY;
	case LOOP:
		Console("Reset Loop.");
		ClearMotionState();
		return EXIT;
	case EXIT:
		Console("Reset Exit.");
		state = BOOTUP;
		return ENTRY;
	};
	return 51;
}

u8 Stopped(u8 ret) {
	switch (ret) {
	case ENTRY:
		Console("Stopped Entry.");
		if (SoftStop_NonBlk() == XST_SUCCESS )
			return LOOP;
		else
			return ENTRY;
	case LOOP:
		Console("Stopped Loop.");
		UpateSense();
		return LOOP;
	case EXIT:
		return EXIT;
	};
	return 51;
}

u8 PreOp(u8 ret) {
	switch (ret) {
	case ENTRY:
		Console("PreOp Entry.");
		if (SoftStop_NonBlk() == XST_SUCCESS ){
			BootupMsg();
			return LOOP;
		} else
			return ENTRY;
	case LOOP:
		Console("PreOp Loop.");
		UpateSense();
		return LOOP;
	case EXIT:
		return EXIT;
	};
	return 51;
}

u8 Op(u8 ret) {
	switch (ret) {
	case ENTRY:
		//return LOOP;
	case LOOP:
		Console("Op Loop.");
		UpateSense();
		UpdateVel();
		Ctrl_Plant();
		return LOOP;
	case EXIT:
		return EXIT;
	};
	return 51;
}

static inline int H(unsigned char B0) {
	return ( (B0>>7)|(B0<<1) )&0x0F; //Magic
}

int main(void) {
 	XTime t_now, t_start;
 	double  elapsed_time = 0.0;
 	u8 (*state_machine[])(u8) = {0, PreOp, Op, Reset, Stopped, BootUp};

	Init();

	XTime_GetTime(&t_start);
	while (elapsed_time < duration) {
		RecvMsg();

		//BD Command
		BDCmd(cmd_opt);
		//Motion Driver
		state_ret = state_machine[H(state)](state_ret);

		SendMsg();

		XTime_GetTime(&t_now);
		elapsed_time = 1.0 * (t_now - t_start) / (COUNTS_PER_SECOND);
	}

	SoftStop();
	End();
	return 1;
}
