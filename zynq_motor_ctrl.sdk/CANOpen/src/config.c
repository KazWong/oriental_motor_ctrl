/*
 * config.c
 *
 *  Created on: 25 Feb 2020
 *      Author: whwong
 */

#include "config.h"

void InitWheels() {
	dac_addr[0] = DAC_IIC_ADDR_A;
	dac_addr[1] = DAC_IIC_ADDR_B;
	dac_addr[2] = DAC_IIC_ADDR_C;
	dac_addr[3] = DAC_IIC_ADDR_D;

	speedout_addr[4] = mot_A_speedout;
	speedout_addr[4] = mot_B_speedout;
	speedout_addr[4] = mot_C_speedout;
	speedout_addr[4] = mot_D_speedout;

	wheels[FL].pins[M0] = MTC_A_M0;
	wheels[FL].pins[MB_FREE] = MTC_A_MB_FREE;
	wheels[FL].pins[STPM] = MTC_A_STPM;
	wheels[FL].pins[ALRM_RST] = MTC_A_ALRM_RST;
	wheels[FL].pins[FW] = emio_mot_A_fw;
	wheels[FL].pins[BW] = emio_mot_A_bw;

	wheels[FR].pins[M0] = MTC_B_M0;
	wheels[FR].pins[MB_FREE] = MTC_B_MB_FREE;
	wheels[FR].pins[STPM] = MTC_B_STPM;
	wheels[FR].pins[ALRM_RST] = MTC_B_ALRM_RST;
	wheels[FR].pins[FW] = emio_mot_B_fw;
	wheels[FR].pins[BW] = emio_mot_B_bw;

	wheels[BL].pins[M0] = MTC_C_M0;
	wheels[BL].pins[MB_FREE] = MTC_C_MB_FREE;
	wheels[BL].pins[STPM] = MTC_C_STPM;
	wheels[BL].pins[ALRM_RST] = MTC_C_ALRM_RST;
	wheels[BL].pins[FW] = emio_mot_C_fw;
	wheels[BL].pins[BW] = emio_mot_C_bw;

	wheels[BR].pins[M0] = MTC_D_M0;
	wheels[BR].pins[MB_FREE] = MTC_D_MB_FREE;
	wheels[BR].pins[STPM] = MTC_D_STPM;
	wheels[BR].pins[ALRM_RST] = MTC_D_ALRM_RST;
	wheels[BR].pins[FW] = emio_mot_D_fw;
	wheels[BR].pins[BW] = emio_mot_D_bw;
}
