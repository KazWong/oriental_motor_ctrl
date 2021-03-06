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
#define MIN_SPEED	0.131109133			// v = HzpiD/150, 40Hz
#define LIM_DAC		0x0FFF				// Final Protection
#define DAC_COMPEN	1.03310880392010	// 1.05*6.45/LIM_SPEED;

#define MAX_VEL16	13081				// MAX_VEL / 65535.0 * LIM_SPEED
#define MAX_OMEGA16	31406				// MAX_OMEGA / 65535.0 * LIM_SPEED

//Ziegler–Nichols method: Ku=1.33, Tu=0.5, Kp=0.798, Ki=3.192, Kd=0.0049875
//noload gain Kp: 0.5, Ki: 4.8, Kd: 0.005
#define Kp			0.6					// 0.245;
#define Ki			2.8					// 1.092;
#define Kd			0.0					// 0.0028;

//Motion Profile
//double MAX_ACC = 0.77;
//double MAX_DEC = 1.1;
#define MAX_ACC_VEL	0.0077				// MAX_ACC*t_ctrl
#define MAX_DEC_VEL	0.0090				// MAX_DEC*t_ctrl

//
#define PLANT_T		0.00625
#define CTRL_T		0.01				// 100.hz
#define FB_T		0.002				// 500.hz
#define CMD_T		0.01666666666666	// 60.hz
#define EST_A_T		0.05				// 20.hz
#define EST_CA_T	0.05				// 20.hz
#define IN_T		0.01				// 100.hz

extern u16 dac[4];
extern double u[4];
extern double e[3][4];
extern double y[3][4];
extern double y_0[4];
extern double v[4];
extern double w[4];
extern double a[4];
extern double cmd_a[4];
extern double a_y[5][4];
extern double a_v[5][4];
extern double s[4];
extern float odom[3];
extern double p_t[4];

u8 MotorBootUp();
void MotionInit();
void MotorOFF();
void ResetMotionTimer();
void ClearMotionState();

void SpeedOut(double dt);
void Integrator(double dt);
void Differentiator(double (*last)[4], double *vel, double *acc, double dt);
void SetVel(u32 vel_frame[], double dt);
void Ctrl(double Ts);
void Plant(int idx, u8 dac_addr);

void SoftStop();
void Ctrl_Plant();

#endif /* SRC_MOTION_H_ */
