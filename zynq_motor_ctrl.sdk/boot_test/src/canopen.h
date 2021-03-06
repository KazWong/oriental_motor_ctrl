/*
 * canopen.h
 *
 *  Created on: 18 Aug 2020
 *      Author: whwong
 */

#ifndef SRC_CANOPEN_H_
#define SRC_CANOPEN_H_

#define NODE_ID			0x30
#define FREEMSG_ID		0x388

//COB-ID
#define NMT_NM			0x700	//NMT node monitoring
#define NMT_NC			0x000	//NMT node control
#define SYNC			0x080	//Sync
#define TIMESTAMP		0x100	//TimeStamp
#define SDO_TX			0x580	//SDO Transmit
#define SDO_RX			0x600	//SDO Receive

//NMT command code
#define PREOP			0x80	//Pre-Operational
#define RESET			0x81	//Reset Control State Machine
#define BOOTUP			0x82	//Boot Up
#define OP				0x01	//Operational
#define STOP			0x02	//Stooped

//SDO object
#define ID_OBJ			0x18

//Tx PDO COB-ID
#define PDO_WHEEL_S		0x381
#define PDO_WHEEL_A		0x382
#define PDO_WHEEL_Y		0x383
#define PDO_WHEEL_W		0x384
#define PDO_ODOM_X		0x385
#define PDO_ODOM_Y		0x386
#define PDO_ODOM_Z		0x387

//Rx PDO COB-ID
#define PDO_CMD_VEL		0x401
#define PDO_CMD_OPT		0x60

#define ID_NMT_NC		(((NMT_NC) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_TIMESTAMP	(((TIMESTAMP) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_WHEEL_S	(((PDO_WHEEL_S) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_WHEEL_A	(((PDO_WHEEL_A) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_WHEEL_Y	(((PDO_WHEEL_Y) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_WHEEL_W	(((PDO_WHEEL_W) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_ODOM_X	(((PDO_ODOM_X) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_ODOM_Y	(((PDO_ODOM_Y) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_ODOM_Z	(((PDO_ODOM_Z) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_CMD_VEL	(((PDO_CMD_VEL) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)
#define ID_PDO_CMD_OPT	(((PDO_CMD_OPT) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)

#define ID_ID_OBJ		(((ID_OBJ) << XCANPS_IDR_ID1_SHIFT) & XCANPS_IDR_ID1_MASK)

#define ENTRY			0
#define LOOP			1
#define EXIT			2

#define MAX_RINGBUF		128

struct _PDOMsg {
	int cob_id;
	u8 msg[8];
	u8 size;
};
typedef struct _PDOMsg PDOMsg;


//Velocity command
extern u32 vel_frame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];
//State Machine Operation
extern u8 state, state_ret;
//Optional command
extern u8 cmd_opt;

void CANOpenInit();
u8 CANOpenBootUp();
void BootupMsg();

void PushMsgBuf(int id, u8 size, const u8* msg);
void FreeMsg(const u8* msg);
void RecvMsg();
void SendMsg();

#endif /* SRC_CANOPEN_H_ */
