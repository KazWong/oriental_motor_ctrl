/*
 * motor_motion.c
 *
 *  Created on: 25 Feb 2020
 *      Author: whwong
 */

#include "motor_motion.h"

void InvKin(vector3 *v) {
	wheels[FL].cmd_vel = (v->x - v->y - (lx + ly)*v->z) / r;
	wheels[FR].cmd_vel = (v->x + v->y + (lx + ly)*v->z) / r;
	wheels[BL].cmd_vel = (v->x + v->y - (lx + ly)*v->z) / r;
	wheels[BR].cmd_vel = (v->x - v->y + (lx + ly)*v->z) / r;
}

vector3 FwdKin(wheel *wheels) {
	vector3 v;

	v.x =    wheels[FL].fb_vel + wheels[FR].fb_vel + wheels[BL].fb_vel + wheels[BR].fb_vel;
	v.y =  - wheels[FL].fb_vel + wheels[FR].fb_vel + wheels[BL].fb_vel - wheels[BR].fb_vel;
	v.z = (- wheels[FL].fb_vel + wheels[FR].fb_vel - wheels[BL].fb_vel + wheels[BR].fb_vel) / (lx + ly);

	return v;
}

float DACGainLUT(wheel wheel) {
	return 0.0;
}

vector3 Profile(vector3 v) {
	return v;
}

float PID(wheel wheel) {
	u[0] = u[1] + Kp*(e[0] - e[1]) + Ki*Ts*(e[0]) + (Kd/Ts)*(e[0] - 2*e[1] + e[2]);

	u[1] = u[0];
	e[2] = e[1];
	e[1] = e[0];

	return u[0];
}

float FFFB(wheel wheel) {
	float u, u1, u2;
	float dac;
	u8 high_dac = 0x0, low_dac = 0x0;

	e[0] = wheel.cmd_vel - wheel.fb_vel;

	u1 = PID(wheel);
	u2 = DACGainLUT(wheel);

	u = u1 + u2;

	dac = u * 4095. / 6.554;
}
