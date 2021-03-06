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
#include "zynq_can.h"

#include "motion.h"

u32 pin_fw[] = {emio_mot_a_fw, emio_mot_b_fw, emio_mot_c_fw, emio_mot_d_fw};
u32 pin_bw[] = {emio_mot_a_bw, emio_mot_b_bw, emio_mot_c_bw, emio_mot_d_bw};

//
u32 VelFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS] = {0};
u16 dac[4] = {0, 0, 0, 0};
_Float64 u[4] = {0.0, 0.0, 0.0, 0.0};
_Float64 e[3][4] = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
_Float64 y[3][4] = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
_Float64 y_0[4] = {0.0, 0.0, 0.0, 0.0};
_Float64 v[4] = {0.0, 0.0, 0.0, 0.0};
_Float64 w[4] = {0.0, 0.0, 0.0, 0.0};
_Float64 a[4] = {0.0, 0.0, 0.0, 0.0};
_Float64 cmd_a[4] = {0.0, 0.0, 0.0, 0.0};
_Float64 a_y[5][4] = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
_Float64 a_v[5][4] = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
_Float64 s[4] = {0.0, 0.0, 0.0, 0.0};
_Float32 odom[3] = {0.0, 0.0, 0.0};
_Float64 p_t[4] = {0.00625, 0.00625, 0.00625, 0.00625};
_Float64 duration = 60000.0;

void SpeedOut(_Float64 dt) {
	u32 count[4] = {0, 0, 0, 0};
	double p[4] = {0.0, 0.0, 0.0, 0.0};
	double t[4] = {0.0, 0.0, 0.0, 0.0};
	double K = 10000000.*M_PI*2.*R/PULSE_RATIO; //32777.283352454
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

void Integrator(_Float64 dt) {
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

void Differentiator(_Float64 (*last)[4], _Float64 *vel, _Float64 *acc, _Float64 dt) {
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

void Setpoint(_Float64 dt, XCanPs *Can) {
	s32 Status;
	u32 RxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS] = {0};
	u16 x, y, z;
	u8 *ptr_x, *ptr_y, *ptr_z, b;
	_Float64 vx, vy, wz;

	ptr_x = (u8*)(&x);
	ptr_y = (u8*)(&y);
	ptr_z = (u8*)(&z);

	Status = RecvFrame(Can, -1, RxFrame, 10);
	if (Status != XST_SUCCESS) {
		//Status = SendFrame(&Can, 0x01, TxFrame, (u8) sizeof(TxFrame));
		return;
	}

	if (CheckID(0x88, RxFrame)) {
		u8 *FramePtr;
		FramePtr = (u8 *)(&RxFrame[2]);
		for (int i=0;i<2;i++, ptr_x++, ptr_y++, ptr_z++, FramePtr++) {
			*ptr_x = *FramePtr;
			*ptr_y = *(FramePtr+2);
			*ptr_z = *(FramePtr+4);
		}
		b = (u8)*(((u8*)(&RxFrame[2]))+6);

		//convert and limit the velocity
		/*vx = (_Float64)x / 65535.0 * LIM_SPEED;
		vy = (_Float64)y / 65535.0 * LIM_SPEED;
		wz = (_Float64)z / 65535.0 * LIM_SPEED;

		vx = (vx > MAX_VEL)? MAX_VEL:vx;
		vy = (vy > MAX_VEL)? MAX_VEL:vy;
		wz = (wz > MAX_OMEGA)? MAX_OMEGA:wz;

		vx *= (1 - ((b & 0b100)>>1));
		vy *= (1 - (b & 0b010));
		wz *= (1 - ((b & 0b001)<<1));
		*/

		vx = (1 - ((b & 0b100)>>1)) * ( ((_Float64)x > MAX_VEL16)? MAX_VEL:(_Float64)x / 65535.0 * LIM_SPEED );
		vy = (1 - (b & 0b010))      * ( ((_Float64)y > MAX_VEL16)? MAX_VEL:(_Float64)y / 65535.0 * LIM_SPEED );
		wz = (1 - ((b & 0b001)<<1)) * ( ((_Float64)z > MAX_OMEGA16)? MAX_OMEGA:(_Float64)z / 65535.0 * LIM_SPEED );

		//Inv Kinematics + round to 0.01
		v[0] = - vx + vy + (lx_ly)*wz;
		v[1] =   vx + vy + (lx_ly)*wz;
		v[2] = - vx - vy + (lx_ly)*wz;
		v[3] =   vx - vy + (lx_ly)*wz;
	} else if (CheckID(0x99, RxFrame)) {
		u8 *FramePtr;
		FramePtr = (u8 *)(&RxFrame[2]);

		switch (*FramePtr) {
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
		}
	}
}

void Ctrl(_Float64 Ts, _Float64 fb_dt, _Float64 cmd_dt) {
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
	_Float64 stop_vel = 0x020 * LIM_SPEED / (4095. * MIN_SPEED);
	u8 su = *((s64 *)(&u[idx]))>>63;
	u8 sy = *((s64 *)(&y[0][idx]))>>63;

	//send dac
	dac[idx] = (u16)floor(fabs(u[idx] / (LIM_SPEED*DAC_COMPEN)) * 4095.);//LIM_SPEED*1.05 compensate offset of estv and cmdv

	if (dac[idx] > MAX_DAC) {
		dac[idx] = MAX_DAC;
	} else if (dac[idx] < 0x000) {
		dac[idx] = 0x000;
	} else if (start_dac > dac[idx] && dac[idx] > stop_dac) {
		dac[idx] = start_dac;
	}

	//direction
	if (su != sy) {
		p_t[idx] = (a[idx] == 0.0)? plant_t : (stop_vel - y[0][idx]) / a[idx];
		p_t[idx] = (p_t[idx] > 0.05)? plant_t : p_t[idx];
	}

	if (fabs(y[0][idx]) < stop_vel && dac[idx] > stop_dac) {
		if (su) {
			zynq_write_fw_bw_pins(pin_fw[idx], pin_off);
			zynq_write_fw_bw_pins(pin_bw[idx], pin_on);
			p_t[idx] = plant_t;
		} else {
			zynq_write_fw_bw_pins(pin_fw[idx], pin_on);
			zynq_write_fw_bw_pins(pin_bw[idx], pin_off);
			p_t[idx] = plant_t;
		}
	}


	dac_val[0] = *(pt_dac+1);
	dac_val[1] = *pt_dac;

	dac_send_val(dac_addr, dac_val);
}

void MotionInit() {
    zynq_to_motor_init();
    dac_init_val();

    AXIGPIOInit();
}

void MotorBootUp() {
	u8 zero[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	zynq_to_motor_ctrl(0x88, 0x88);
	MotorOFF();
	dac_send_four_val(zero);
}

void MotorOFF() {
	for (int i=0;i<4;i++) {
		zynq_write_fw_bw_pins(pin_fw[i], pin_off);
	  	zynq_write_fw_bw_pins(pin_bw[i], pin_off);
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


	/*XTime_GetTime(&t_softstop);
	do{
		XTime_GetTime(&t_now);
	}while((0.1 > (1.0*(t_now - t_softstop) / (COUNTS_PER_SECOND))));*/
}
