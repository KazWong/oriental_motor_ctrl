/*
 * motion.h
 *
 *  Created on: 18 Aug 2020
 *      Author: whwong
 */

#ifndef SRC_MOTION_H_
#define SRC_MOTION_H_

/*******************************************
 * Notation
 *
 * lx_ly: half of the distance between front wheels + half of the distance between front wheel and the rear wheels
 * R: radius of the wheels
 * Kp: proportional gain in PID loop
 * Ki: integral gain in PID loop
 * Kd: derivative gain in PID loop
 * u: input to plant
 * e: error of ref - feedback
 * y: feedback velocity from each wheels
 * v: command velocity for each wheels
 * w: velocity after motion profile for each wheels
 * a: feedback acceleration
 * cmd_a: command acceleration
 * a_y, a_v: array of past acceleration values
 * s: position for each wheels
 *
********************************************/


#define lx_ly		0.4165
#define R			0.07825
#define LIM_SPEED	6.555456670490
#define MIN_SPEED	0.131109133			//v = HzpiD/150, 40Hz
#define MAX_VEL		2.0					//4.916592502868;
#define MAX_OMEGA	4.71238898			//11.80454382 MAX_VEL/(lx_ly);
#define MAX_DAC		0x0FFF				//Final Protection
#define DAC_COMPEN	1.03310880392010	//1.05*6.45/LIM_SPEED;
#define PULSE_RATIO	300.0				// 30ppr x Gear ratio

#define MAX_VEL16	13081				//MAX_VEL / 65535.0 * LIM_SPEED
#define MAX_OMEGA16	31406				//MAX_OMEGA / 65535.0 * LIM_SPEED

//Ziegler–Nichols method: Ku=1.33, Tu=0.5, Kp=0.798, Ki=3.192, Kd=0.0049875
//noload gain Kp: 0.5, Ki: 4.8, Kd: 0.005
#define Kp			0.6					//0.245;
#define Ki			2.8					//1.092;
#define Kd			0.0					//0.0028;

//Motion Profile
//_Float64 MAX_ACC = 0.77;
//_Float64 MAX_DEC = 1.1;
#define MAX_ACC_VEL	0.0077				// MAX_ACC*t_ctrl
#define MAX_DEC_VEL	0.0090				// MAX_DEC*t_ctrl

//
#define plant_t		0.00625

extern u32 VelFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];
extern u16 dac[4];
extern _Float64 u[4];
extern _Float64 e[3][4];
extern _Float64 y[3][4];
extern _Float64 y_0[4];
extern _Float64 v[4];
extern _Float64 w[4];
extern _Float64 a[4];
extern _Float64 cmd_a[4];
extern _Float64 a_y[5][4];
extern _Float64 a_v[5][4];
extern _Float64 s[4];
extern _Float32 odom[3];
extern _Float64 p_t[4];
extern _Float64 duration;


void SpeedOut(_Float64 dt);
void Integrator(_Float64 dt);
void Differentiator(_Float64 (*last)[4], _Float64 *vel, _Float64 *acc, _Float64 dt);
void Setpoint(_Float64 dt, XCanPs *Can);
void Ctrl(_Float64 Ts, _Float64 fb_dt, _Float64 cmd_dt);
void Plant(int idx, u8 dac_addr);
void MotorBootUp();
void MotionInit();
void MotorOFF();
void SoftStop();

#endif /* SRC_MOTION_H_ */
