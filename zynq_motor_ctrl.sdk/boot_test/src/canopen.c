/*
 * canopen.c
 *
 *  Created on: 18 Aug 2020
 *      Author: whwong
 */
#include "zynq_can.h"
#include <stdlib.h>

#include "canopen.h"

u32 vel_frame[XCANPS_MAX_FRAME_SIZE_IN_WORDS] = {0};
u8 state = BOOTUP, state_ret = ENTRY;
u8 cmd_opt = 0x00;
PDOMsg PDOMsg_buffer[MAX_RINGBUF];
int PDOMsg_buf_start_idx = 0;
int PDOMsg_buf_end_idx = 0;

XCanPs Can;


void CANOpenInit() {
	state = BOOTUP;
	CanPsPolledExample(CAN_DEVICE_ID, &Can);
}

u8 CANOpenBootUp() {
	for (int i=0;i<MAX_RINGBUF;i++) {
		PDOMsg_buffer[i].cob_id = 0x00;
		PDOMsg_buffer[i].size = 0x00;
		for (int j=0;j<8;j++)
			PDOMsg_buffer[i].msg[j] = 0x00;
	}
	PDOMsg_buf_start_idx = 0;
	PDOMsg_buf_end_idx = 0;

	return XST_SUCCESS;
}

void BootupMsg() {
	u8 bootup[] = {0x00};
	PushMsgBuf(NMT_NM+NODE_ID, 1, bootup);
}

void FreeMsg(const u8* msg) {
	PushMsgBuf(FREEMSG_ID, (u8) sizeof(msg), msg);
}

void SendMsg() {
//Send PDO msg
	if (PDOMsg_buf_start_idx < PDOMsg_buf_end_idx) {
		if (SendFrame_NonBlk(&Can, PDOMsg_buffer[PDOMsg_buf_start_idx].cob_id,
				PDOMsg_buffer[PDOMsg_buf_start_idx].msg,
				PDOMsg_buffer[PDOMsg_buf_start_idx].size) == XST_SUCCESS) {
				//(u8) sizeof(PDOMsg_buffer[PDOMsg_buf_start_idx].msg));

			PDOMsg_buf_start_idx += 1;
			if (PDOMsg_buf_start_idx == MAX_RINGBUF)
				PDOMsg_buf_start_idx = 0;
		}
	}
}

void PushMsgBuf(int id, u8 size, const u8* msg) {
	PDOMsg_buffer[PDOMsg_buf_end_idx].cob_id = id;
	PDOMsg_buffer[PDOMsg_buf_end_idx].size = size;
	for (int i=0;i<size;i++) {
		PDOMsg_buffer[PDOMsg_buf_end_idx].msg[i] = msg[i];
	}

	PDOMsg_buf_end_idx += 1;
	if (PDOMsg_buf_end_idx == MAX_RINGBUF)
		PDOMsg_buf_end_idx = 0;
}

void RecvMsg() {
//CAN Receive
	s32 Status;
	u8 *frame_ptr;
	u8 msg[8];
	u32 RxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];

	Status = RecvFrame_NonBlk(&Can, -1, RxFrame, 10);
	if (Status != XST_SUCCESS) {
		//Status = SendFrame(&Can, 0x01, TxFrame, (u8) sizeof(TxFrame));
		return;
	}

	switch(RxFrame[0]) {
	case ID_NMT_NC: //State change Only
		frame_ptr = (u8* )(&RxFrame[2]);
		if (frame_ptr[1] == NODE_ID && \
				state != frame_ptr[0] && \
				(frame_ptr[0] == PREOP || frame_ptr[0] == RESET || \
				 frame_ptr[0] == BOOTUP || frame_ptr[0] == OP || \
				 frame_ptr[0] == STOP)) {
			state = RxFrame[2];
			state_ret = ENTRY;
		}
		break;
	case ID_TIMESTAMP:
		break;
	case ID_PDO_WHEEL_S:
	case ID_PDO_WHEEL_A:
	case ID_PDO_WHEEL_Y:
	case ID_PDO_WHEEL_W:
	case ID_PDO_ODOM_X:
	case ID_PDO_ODOM_Y:
	case ID_PDO_ODOM_Z:
		//Tx PDOs
		break;
	case ID_PDO_CMD_VEL: //PDO msg for velocity command
		for (int i=0;i<XCANPS_MAX_FRAME_SIZE_IN_WORDS;i++)
			vel_frame[i] = RxFrame[i];
		break;
	case ID_PDO_CMD_OPT: //PDO msg for optional command
		cmd_opt = *(u8 *)(&RxFrame[2]);
		break;
	case ID_ID_OBJ:
		msg[0] = PDO_CMD_OPT;
		PushMsgBuf(ID_OBJ, 1, msg);
		break;
	}
}
