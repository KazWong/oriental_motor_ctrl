/*
 * motion.c
 *
 *  Created on: 18 Aug 2020
 *      Author: whwong
 */
#include <math.h>
#include "xil_printf.h"
#include "xil_types.h"

#include "xtime_l.h"

#include "motor_driver_pins.h"
#include "motor_dac_ctrl.h"
#include "motor_axi_gpio.h"

#include "motion.h"

const u32 pin_fw[] = {emio_mot_a_fw, emio_mot_b_fw, emio_mot_c_fw, emio_mot_d_fw};
const u32 pin_bw[] = {emio_mot_a_bw, emio_mot_b_bw, emio_mot_c_bw, emio_mot_d_bw};
const u8 dac_addr[] = {DAC_IIC_ADDR_A, DAC_IIC_ADDR_B, DAC_IIC_ADDR_C, DAC_IIC_ADDR_D};

double MAX_VEL = 2.0;					// 4.916592502868;
double MAX_OMEGA = 4.71238898;			// 11.80454382 MAX_VEL/(lx_ly);

//
u16 dac[4] = {0, 0, 0, 0};
double u[4] = {0.0, 0.0, 0.0, 0.0};
double e[3][4] = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
double y[3][4] = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
double y_0[4] = {0.0, 0.0, 0.0, 0.0};
double v[4] = {0.0, 0.0, 0.0, 0.0};
double w[4] = {0.0, 0.0, 0.0, 0.0};
double a[4] = {0.0, 0.0, 0.0, 0.0};
double cmd_a[4] = {0.0, 0.0, 0.0, 0.0};
double a_y[5][4] = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
double a_v[5][4] = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
double s[4] = {0.0, 0.0, 0.0, 0.0};
float odom[3] = {0.0, 0.0, 0.0};

double p_t[4] = {0.00625, 0.00625, 0.00625, 0.00625};
XTime t_ctrl, t_p[4];

void MotionInit() {
    zynq_to_motor_init();
    dac_init_val();

    AXIGPIOInit();
}

u8 MotorBootUp() {
	u8 zero[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	zynq_to_motor_ctrl(0x88, 0x88);
	MotorOFF();
	dac_send_four_val(zero);

	return XST_SUCCESS;
}

void MotorOFF() {
	for (int i=0;i<4;i++) {
		zynq_write_fw_bw_pins(pin_fw[i], pin_off);
	  	zynq_write_fw_bw_pins(pin_bw[i], pin_off);
	}
}

void ResetMotionTimer() {
	XTime_GetTime(&t_ctrl);
	XTime_GetTime(&t_p[0]);
	XTime_GetTime(&t_p[1]);
	XTime_GetTime(&t_p[2]);
	XTime_GetTime(&t_p[3]);
}

void ClearMotionState() {
	for (int i=0;i<3;i++)
		odom[i] = 0.0;

	for (int i=0;i<4;i++) {
		p_t[i] = 0.00625;
		s[i] = cmd_a[i] = a[i] = w[i] = v[i] = y_0[i] = u[i] = dac[i] = 0.0;
	}

	for (int i=0;i<3;i++)
		for (int j=0;j<4;j++) {
			e[i][j] = y[i][j] = 0.0;
		}

	for (int i=0;i<5;i++)
		for (int j=0;j<4;j++) {
			a_y[i][j] = a_v[i][j] = 0.0;
		}
}

void SpeedOut(double dt) {
	u32 count[4] = {0, 0, 0, 0};
	double p[4] = {0.0, 0.0, 0.0, 0.0};
	double t[4] = {0.0, 0.0, 0.0, 0.0};
	double K = 10000000.*M_PI*2.*R/150.; //32777.283352454
	double alp = dt / (0.04 + dt);

	count[0] = Xil_In32(0x41200000);
	count[1] = Xil_In32(0x41200008);
	count[2] = Xil_In32(0x41210000);
	count[3] = Xil_In32(0x41210008);

	for (int i=0;i<4;i++) {
		double _y;

		t[i] = (double)(count[i] >> 19);
		p[i] = (double)(count[i] & 0x7FFFF);
		_y = (p[i] == 0.0 || t[i] == 0.0)? 0.0 : K * t[i] / p[i];
		if (zynq_read_fw_bw_pins(pin_bw[i]) && !zynq_read_fw_bw_pins(pin_fw[i]))
			_y = -_y;
		y[0][i] += alp*(_y - y[0][i]);

		y[2][i] = y[1][i];
		y[1][i] = y_0[i] = y[0][i];
	}
}

void Integrator(double dt) {
	double sx, sy, rz, _s[4];

	for (int i=0;i<4;i++) {
		_s[i] = y[0][i] * dt;
	}

	//Fwd Kinematics + odom est
	sx = (-_s[0] + _s[1] - _s[2] + _s[3]) / 4.;
	sy = ( _s[0] + _s[1] - _s[2] - _s[3]) / 4.;
	rz = ( _s[0] + _s[1] + _s[2] + _s[3]) / (lx_ly*4.);

	if (odom[2]+rz > 2.*M_PI)
		odom[2] = (odom[2]+rz) - 2.*M_PI;
	else if (odom[2]+rz < 0.0)
		odom[2] = 2.*M_PI - (odom[2]+rz);
	else
		odom[2] += rz;
	odom[0] += sx * cos(odom[2]) - sy * sin(odom[2]);
	odom[1] += sx * sin(odom[2]) + sy * cos(odom[2]);

	for (int i=0;i<4;i++)
		s[i] += _s[i];
}

void Differentiator(double (*last)[4], double *vel, double *acc, double dt) {
	double alp = dt / (0.1 + dt);

	for (int i=0;i<4;i++) {
		double _acc;

		last[4][i] = last[3][i];
		last[3][i] = last[2][i];
		last[2][i] = last[1][i];
		last[1][i] = last[0][i];
		last[0][i] = vel[i];

		//_acc = (3*last[0][i] - 4*last[1][i] + last[2][i]) / (2*dt); //1st order Backward Difference
		_acc = (-last[4][i] + 8*last[3][i] - 8*last[1][i] + last[0][i]) / (12*dt); //1st order Center Difference
		acc[i] += alp*(_acc - acc[i]);
	}
}

void SetVel(u32 vel_frame[], double dt) {
	//s32 Status;
	u16 x, y, z;
	u8 *ptr_x, *ptr_y, *ptr_z, b;
	double vx, vy, wz;

	ptr_x = (u8*)(&x);
	ptr_y = (u8*)(&y);
	ptr_z = (u8*)(&z);

	u8 *FramePtr;
	FramePtr = (u8 *)(&vel_frame[2]);
	for (int i=0;i<2;i++, ptr_x++, ptr_y++, ptr_z++, FramePtr++) {
		*ptr_x = *FramePtr;
		*ptr_y = *(FramePtr+2);
		*ptr_z = *(FramePtr+4);
	}
	b = (u8)*(((u8*)(&vel_frame[2]))+6);

	//convert and limit the velocity
	vx = (1 - ((b & 0b100)>>1)) * ( ((double)x > MAX_VEL16)? MAX_VEL:(double)x / 65535.0 * LIM_SPEED );
	vy = (1 - (b & 0b010))      * ( ((double)y > MAX_VEL16)? MAX_VEL:(double)y / 65535.0 * LIM_SPEED );
	wz = (1 - ((b & 0b001)<<1)) * ( ((double)z > MAX_OMEGA16)? MAX_OMEGA:(double)z / 65535.0 * LIM_SPEED );

	//Inv Kinematics + round to 0.01
	v[0] = - vx + vy + (lx_ly)*wz;
	v[1] =   vx + vy + (lx_ly)*wz;
	v[2] = - vx - vy + (lx_ly)*wz;
	v[3] =   vx - vy + (lx_ly)*wz;
}

void Ctrl(double Ts) {
	//trapezoidal profile
	for (int i=0; i<4; i++) {
		float dv = v[i] - w[i];

		if (w[i] >= 0.0 && dv > MAX_ACC_VEL)
			w[i] = w[i] + MAX_ACC_VEL;
		else if (w[i] >= 0.0 && dv < -MAX_DEC_VEL)
			w[i] = w[i] - MAX_DEC_VEL;
		else if (w[i] < 0.0 && dv  > MAX_DEC_VEL)
			w[i] = w[i] + MAX_DEC_VEL;
		else if (w[i] < 0.0 && dv < -MAX_ACC_VEL)
			w[i] = w[i] - MAX_ACC_VEL;
		else
			w[i] = w[i] + dv;
	}

	//PID
	for (int i=0; i<4; i++) {
		e[2][i] = e[1][i];
		e[1][i] = e[0][i];
		e[0][i] = w[i] - y[0][i];

		u[i] = w[i];
		//u[i] = u[i] + (Kp*(e[0][i]-e[1][i]) + Ki*Ts*(e[0][i]) + (Kd/Ts)*(e[0][i] - 2*e[1][i] + e[2][i]));
		//u[i] = u[i] + (Kp*(-y[0][i]+y[1][i]) + Ki*Ts*(e[0][i]) + (Kd/Ts)*(-y[0][i] + 2*y[1][i] - y[2][i]));
	}
}

void Plant(int idx, u8 dac_addr) {
	u8 dac_val[] = {0x00, 0x00};
	u8 *pt_dac = (u8*)&dac[idx];
	u16 start_dac = (u16)(4095. * MIN_SPEED / LIM_SPEED);
	u16 stop_dac = 0x020;
	double stop_vel = 0x020 * LIM_SPEED / (4095. * MIN_SPEED);
	u8 su = *((s64 *)(&u[idx]))>>63;
	u8 sy = *((s64 *)(&y[0][idx]))>>63;

	//send dac
	dac[idx] = (u16)floor(fabs(u[idx] / (LIM_SPEED*DAC_COMPEN)) * 4095.);//LIM_SPEED*1.05 compensate offset of estv and cmdv

	if (dac[idx] > LIM_DAC) {
		dac[idx] = LIM_DAC;
	} else if (dac[idx] < 0x000) {
		dac[idx] = 0x000;
	} else if (start_dac > dac[idx] && dac[idx] > stop_dac) {
		dac[idx] = start_dac;
	}

	//direction
	if (su != sy) {
		p_t[idx] = (a[idx] == 0.0)? PLANT_T : (stop_vel - y[0][idx]) / a[idx];
		p_t[idx] = (p_t[idx] > 0.05)? PLANT_T : p_t[idx];
	}

	if (fabs(y[0][idx]) < stop_vel && dac[idx] > stop_dac) {
		if (su) {
			zynq_write_fw_bw_pins(pin_fw[idx], pin_off);
			zynq_write_fw_bw_pins(pin_bw[idx], pin_on);
			p_t[idx] = PLANT_T;
		} else {
			zynq_write_fw_bw_pins(pin_fw[idx], pin_on);
			zynq_write_fw_bw_pins(pin_bw[idx], pin_off);
			p_t[idx] = PLANT_T;
		}
	}


	dac_val[0] = *(pt_dac+1);
	dac_val[1] = *pt_dac;

	dac_send_val(dac_addr, dac_val);
}



void Ctrl_Plant() {
	XTime t_now;

	XTime_GetTime(&t_now);
	if (CTRL_T <= ( 1.0 * (t_now - t_ctrl) / (COUNTS_PER_SECOND) )) {
		Integrator((double)(1.0 * (t_now - t_ctrl) / (COUNTS_PER_SECOND)));
		Ctrl( (double)(1.0 * (t_now - t_ctrl) / (COUNTS_PER_SECOND)) );
		XTime_GetTime(&t_ctrl);
	}

	for (int i=0;i<4;i++) {
		XTime_GetTime(&t_now);
		if (p_t[i] <= ( 1.0 * (t_now - t_p[i]) / (COUNTS_PER_SECOND) )) {
			Plant(i, dac_addr[i]);
			XTime_GetTime(&t_p[i]);
		}
	}
}

void SoftStop() {
	XTime t_now, t_softstop;
	double ctrl_hz = 550.0;
	u8 dac_val[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	for (int i=0;i<4;i++)
		dac[i] = (u16)floor(fabs(y[0][i] / (LIM_SPEED*DAC_COMPEN)) * 4095.);

	XTime_GetTime(&t_now);
	XTime_GetTime(&t_softstop);
	while (dac[0] > 0x0000 || dac[1] > 0x0000 || dac[2] > 0x0000 || dac[3] > 0x0000) {
		if (1./ctrl_hz < ( 1.0 * (t_now - t_softstop) / (COUNTS_PER_SECOND) )) {
			for (int i=0; i<4; i++) {
				u8 *pt_dac = (u8*)&dac[i];
				if ( (s32)(dac[i] - 0x6) > 0) {
					dac[i] = dac[i] - 0x6;
				} else {
					dac[i] = 0;
				}
				dac_val[2*i] = *(pt_dac+1);
				dac_val[2*i+1] = *pt_dac;
			}

			dac_send_four_val(dac_val);
			XTime_GetTime(&t_softstop);
		}

		XTime_GetTime(&t_now);
	}


	XTime_GetTime(&t_softstop);
	do{
		XTime_GetTime(&t_now);
	}while((0.1 > (1.0*(t_now - t_softstop) / (COUNTS_PER_SECOND))));
}
