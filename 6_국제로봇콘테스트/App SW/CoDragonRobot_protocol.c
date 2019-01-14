#include <stdio.h>
#include <string.h>
#include "process.h"
#include "CoDragonRobot_protocol.h"
#include "uart_api.h"

void DelayLoop(int delay_time)
{
	while (delay_time)
		delay_time--;
}



void init_robot()
{
	BYTE Init_Buffer[10] = { 0, };
	Init_Buffer[0] = 0xff;
	Init_Buffer[1] = 0xff;
	Init_Buffer[2] = 0x0a;
	Init_Buffer[3] = 0xfe;
	Init_Buffer[4] = 0x03;
	Init_Buffer[5] = 0xa2;
	Init_Buffer[6] = 0x5c;
	Init_Buffer[7] = 0x34;
	Init_Buffer[8] = 0x01;
	Init_Buffer[9] = 0x60;
	uart1_buffer_write(Init_Buffer, 10);

	return;
}

void Send_Play_Motion(BYTE motionNumber)
{
	printf("Motion Start %d\n", motionNumber);
	BYTE Command_Buffer[9] = { 0, };
	BYTE CS1, CS2;

	CS1 = (HOVIS_MOTION_PACKET ^ HOVIS_DRC_pID ^ P2D_HOVIS_PLAY_MOTION ^ motionNumber ^ 0x00) & 0xFE;
	CS2 = (~(HOVIS_MOTION_PACKET ^ HOVIS_DRC_pID ^ P2D_HOVIS_PLAY_MOTION ^ motionNumber ^ 0x00)) & 0xFE;

	Command_Buffer[0] = HEADER1;
	Command_Buffer[1] = HEADER2;
	Command_Buffer[2] = HOVIS_MOTION_PACKET;
	Command_Buffer[3] = HOVIS_DRC_pID;
	Command_Buffer[4] = P2D_HOVIS_PLAY_MOTION;
	Command_Buffer[5] = CS1;
	Command_Buffer[6] = CS2;
	Command_Buffer[7] = motionNumber;
	Command_Buffer[8] = 0x00;

	uart1_buffer_write(Command_Buffer, 9);

	Receive_Motion();

	return;
}


void Receive_Motion()
{
	BYTE Command_Buffer[9] = { 0, };
	BYTE buf[HOVIS_RECEIVE_MOTION_PACKET];
	BYTE CS1, CS2;
	int count = 0;
	int i;



	CS1 = (HOVIS_MOTION_PACKET ^ HOVIS_DRC_pID ^ P2D_HOVIS_RAM_READ ^ HOVIS_RECEIVE_MOTION_ADDRESS ^ HOVIS_RECEIVE_MOTION_BYTESIZE) & 0xFE;
	CS2 = (~(HOVIS_MOTION_PACKET ^ HOVIS_DRC_pID ^ P2D_HOVIS_RAM_READ ^ HOVIS_RECEIVE_MOTION_ADDRESS ^ HOVIS_RECEIVE_MOTION_BYTESIZE)) & 0xFE;

	Command_Buffer[0] = HEADER1;
	Command_Buffer[1] = HEADER2;
	Command_Buffer[2] = HOVIS_MOTION_PACKET;
	Command_Buffer[3] = HOVIS_DRC_pID;
	Command_Buffer[4] = P2D_HOVIS_RAM_READ;
	Command_Buffer[5] = CS1;
	Command_Buffer[6] = CS2;
	Command_Buffer[7] = HOVIS_RECEIVE_MOTION_ADDRESS;
	Command_Buffer[8] = HOVIS_RECEIVE_MOTION_BYTESIZE;


	while (1)
	{
		int boolean;
		int i;
		count++;
		uart1_buffer_write(Command_Buffer, 9);
		memset(buf, 1, 12);
		uart1_buffer_read(buf, HOVIS_RECEIVE_MOTION_PACKET);

		if (buf[0] != 0xff && buf[1] != 0xff)
		{
			uart_txbuf_clear(UART1);
			uart_rxbuf_clear(UART1);
		}
		printf("\n");

		if (buf[10] != 0)
		{
			if (buf[11] == 0x31)
			{
				resetMotor();
				break;
			}

		}
		boolean = buf[9];

		if (boolean == 0 && count != 1)
			break;
		usleep(10000);
	}
	printf("end2\n");
	usleep(20000);

	return;
}

void resetDRC()
{
	BYTE Command_Buffer[7] = { 0, };
	BYTE CS1, CS2;

	CS1 = (HOVIS_REBOOT_PACKET ^ HOVIS_DRC_pID ^ P2D_HOVIS_REBOOT) & 0xFE;
	CS2 = (~(HOVIS_REBOOT_PACKET ^ HOVIS_DRC_pID ^ P2D_HOVIS_REBOOT)) & 0xFE;

	Command_Buffer[0] = HEADER1;
	Command_Buffer[1] = HEADER2;
	Command_Buffer[2] = HOVIS_REBOOT_PACKET;
	Command_Buffer[3] = HOVIS_DRC_pID;
	Command_Buffer[4] = P2D_HOVIS_REBOOT;
	Command_Buffer[5] = CS1;
	Command_Buffer[6] = CS2;

	uart1_buffer_write(Command_Buffer, 7);
	usleep(3);

	return;
}

void resetMotor()
{
	BYTE Command_Buffer[7] = { 0, };
	BYTE CS1, CS2;

	CS1 = (HOVIS_REBOOT_PACKET ^ HOVIS_MOTOR_pID ^ P2D_HOVIS_REBOOT_MOTOR) & 0xFE;
	CS2 = (~(HOVIS_REBOOT_PACKET ^ HOVIS_MOTOR_pID ^ P2D_HOVIS_REBOOT_MOTOR)) & 0xFE;

	Command_Buffer[0] = HEADER1;
	Command_Buffer[1] = HEADER2;
	Command_Buffer[2] = HOVIS_REBOOT_PACKET;
	Command_Buffer[3] = HOVIS_MOTOR_pID;
	Command_Buffer[4] = P2D_HOVIS_REBOOT_MOTOR;
	Command_Buffer[5] = CS1;
	Command_Buffer[6] = CS2;

	uart1_buffer_write(Command_Buffer, 7);
	sleep(2);
	resetDRC();
	sleep(2);
	init_robot();
	usleep(500000);

	return;
}
