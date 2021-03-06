#include "zynq_can.h"
#include "xtime_l.h"

s32 CanPsPolledExample(u16 DeviceId, XCanPs *CanInstPtr){

	s32 Status;
	XCanPs_Config *ConfigPtr;

	/*
	 * Initialize the Can device.
	 */
	ConfigPtr = XCanPs_LookupConfig(DeviceId);
	if (CanInstPtr == NULL) {
		return XST_FAILURE;
	}
	Status = XCanPs_CfgInitialize(CanInstPtr,
					ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Run self-test on the device, which verifies basic sanity of the
	 * device and the driver.
	 */
	Status = XCanPs_SelfTest(CanInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enter Configuration Mode so we can setup Baud Rate Prescaler
	 * Register (BRPR) and Bit Timing Register (BTR).
	 */
	XCanPs_EnterMode(CanInstPtr, XCANPS_MODE_CONFIG);
	while(XCanPs_GetMode(CanInstPtr) != XCANPS_MODE_CONFIG);

	/*
	 * Setup Baud Rate Prescaler Register (BRPR) and
	 * Bit Timing Register (BTR).
	 */
	XCanPs_SetBaudRatePrescaler(CanInstPtr, TEST_BRPR_BAUD_PRESCALAR);
	XCanPs_SetBitTiming(CanInstPtr, TEST_BTR_SYNCJUMPWIDTH,
				TEST_BTR_SECOND_TIMESEGMENT, TEST_BTR_FIRST_TIMESEGMENT);

	/*
	 * Enter Loop Back Mode.
	 */
	XCanPs_EnterMode(CanInstPtr, XCANPS_MODE_NORMAL);
	while(XCanPs_GetMode(CanInstPtr) != XCANPS_MODE_NORMAL);

	return XST_SUCCESS;
}

s32 SendFrame(XCanPs *InstancePtr, u32 txrx_id, u8* frame_data, u8 len){
	u32 TxFrame[4];
	u8 *FramePtr;
	int Index;
	s32 Status;
	XTime t_now, t_start;

	if (len > 8)
		return XST_FAILURE;

	/*
	 * Create correct values for Identifier and Data Length Code Register.
	 */
	TxFrame[0] = (u32)XCanPs_CreateIdValue(txrx_id, 0, 0, 0, 0);
	TxFrame[1] = (u32)XCanPs_CreateDlcValue((u32)len);
	/*
	 * Now fill in the data field with known values so we can verify them
	 * on receive.
	 */

	FramePtr = (u8 *)(&TxFrame[2]);
	for (Index = 0; Index < len; Index++) {
		*FramePtr++ = frame_data[Index];
	}

	/*
	 * Wait until TX FIFO has room.
	 */
	while (XCanPs_IsTxFifoFull(InstancePtr) == TRUE);

	/*
	 * Now send the frame.
	 *
	 * Another way to send a frame is keep calling XCanPs_Send() until it
	 * returns XST_SUCCESS. No check on if TX FIFO is full is needed anymore
	 * in that case.
	 */
	Status = XCanPs_Send(InstancePtr, TxFrame);

	XTime_GetTime(&t_start);
	do{
		XTime_GetTime(&t_now);
	}while((0.00025 > (1.0*(t_now - t_start) / (COUNTS_PER_SECOND))));


	if( XCanPs_GetBusErrorStatus(InstancePtr) > 0 ){
		XCanPs_ClearBusErrorStatus(InstancePtr, 0x1F);
		//XCanPs_Reset(InstancePtr);
	}

	return Status;
}

s32 RecvFrame(XCanPs *InstancePtr, u32 txrx_id, u32* RxFrame, u32 timeout){
	s32 Status;
	XTime t_now, t_start;
	double timeout_ms = timeout * 0.000001;

	/*
	 * Wait until a frame is received.
	 */
	XTime_GetTime(&t_start);
	do{
		XTime_GetTime(&t_now);
	}while((XCanPs_IsRxEmpty(InstancePtr) == TRUE) && (timeout_ms > (1.0*(t_now - t_start) / (COUNTS_PER_SECOND)) ));

	if(timeout == 0){
		return XST_DEVICE_NOT_FOUND;
	}

	/*
	 * Receive a frame and verify its contents.
	 */
	Status = XCanPs_Recv(InstancePtr, RxFrame);
	if (Status == XST_SUCCESS) {
		/*
		 * Verify Identifier and Data Length Code.
		 */
		if (txrx_id != -1)
			if (RxFrame[0] != (u32)XCanPs_CreateIdValue(txrx_id, 0, 0, 0, 0))
				return XST_DEVICE_NOT_FOUND;
	}

	return Status;
}

u8 CheckID(u32 txrx_id, u32* RxFrame) {
	if (RxFrame[0] == (u32)XCanPs_CreateIdValue(txrx_id, 0, 0, 0, 0))
		return 1;
	else
		return 0;
}


