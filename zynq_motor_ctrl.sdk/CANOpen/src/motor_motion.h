/*
 * motor_motion.h
 *
 *  Created on: 25 Feb 2020
 *      Author: whwong
 */

#ifndef SRC_MOTOR_MOTION_H_
#define SRC_MOTOR_MOTION_H_

#include "config.h"

typedef struct _vector3{
	float x;
	float y;
	float z;
} vector3;

float Kp = 1.0;
float Ki = 0.0;
float Kd = 0.0;
float Ts = 1.0/30.0;

float u[2];
float e[3];

void InvKin(vector3 *v);
vector3 FwdKin(wheel *wheels);

void OpenGainLUT();

vector3 Profile(vector3 v);

float PID(wheel wheel);

float FFFB();

#endif /* SRC_MOTOR_MOTION_H_ */
