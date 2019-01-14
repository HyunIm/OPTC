#ifndef __CODRAGON_ROBOT_PROTOCOL_H__
#define __CODRAGON_ROBOT_PROTOCOL_H__

#define HEADER1 0xFF
#define HEADER2 0xFF

/* Protocol Command Set PC to DRC */
#define P2D_HOVIS_REBOOT_MOTOR 0x09
#define P2D_HOVIS_EEP_WRITE 0x11
#define P2D_HOVIS_EEP_READ 0x12
#define P2D_HOVIS_RAM_WRITE 0x13
#define P2D_HOVIS_RAM_READ 0x14
#define P2D_HOVIS_CON_CHECK 0x15
#define P2D_HOVIS_PLAY_MOTION 0x16
#define P2D_HOVIS_PLAY_TASK 0x17
#define P2D_HOVIS_PLAY_BUZZ 0x18
#define P2D_HOVIS_STAT 0x19
#define P2D_HOVIS_ROLLBACK 0x1A
#define P2D_HOVIS_REBOOT 0x1B
#define P2D_HOVIS_ZIGBEE 0x1C
#define P2D_HOVIS_REMOCON 0x1D
#define P2D_HOVIS_SERVO_FW_UPDATE 0x1E

/* Protocol Command Set DRC to PC */
#define D2P_HOVIS_EEP_WRITE 0x51
#define D2P_HOVIS_EEP_READ 0x52
#define D2P_HOVIS_RAM_WRITE 0x53
#define D2P_HOVIS_RAM_READ 0x54
#define D2P_HOVIS_CON_CHECK 0x55
#define D2P_HOVIS_PLAY_MOTION 0x56
#define D2P_HOVIS_PLAY_TASK 0x57
#define D2P_HOVIS_PLAY_BUZZ 0x58
#define D2P_HOVIS_STAT 0x59
#define D2P_HOVIS_ROLLBACK 0x5A
#define D2P_HOVIS_REBOOT 0x5B
#define D2P_HOVIS_ZIGBEE 0x5C
// #define D2P_HOVIS_REMOCON 0x5D
#define D2P_HOVIS_SERVO_FW_UPDATE 0x5E

#define HOVIS_DRC_pID	0xFD		// DRC is fixed : 253
#define HOVIS_MOTOR_pID	0xFE
#define HOVIS_MOTION_PACKET 0x09
#define HOVIS_MOTION 0x00
#define HOVIS_MOTION_PLAY 0x00
#define HOVIS_MOTION_PLAY_FIRST_FRAME 0x01
#define HOVIS_REMOCON_PACKET 0x0A
#define HOVIS_REMOCON_CHANNEL 0x61
#define HOVIS_REMOCON_pID	0xFE
#define HOVIS_REMOCON_TIME	0x08		// 1sec = 8
#define HOVIS_SENSOR_PACKET 0x09
#define HOVIS_SENSOR_ADDRESS 0x55		// Address 85
#define HOVIS_SENSOR_BYTESIZE	0x07	// Sensor flag 1, Acc 6
#define HOVIS_SENSOR_RECEIVE_PACKET	0x12	// Sensor byte 7, Packet 9, Error 2
#define HOVIS_MOTION_PACKET 0x09
#define HOVIS_RECEIVE_MOTION_ADDRESS 0x44		// Address 68
#define HOVIS_RECEIVE_MOTION_BYTESIZE 0x01		// Playing Motion 1
#define HOVIS_RECEIVE_MOTION_PACKET	0x0C	// Playing Motion 1, Packet 9, Error 2
#define HOVIS_REBOOT_PACKET	0x07

///////////////////////////////////////////////////////////////////////////////
void DelayLoop(int delay_time);
void Send_Play_Motion(BYTE motionNumber);
void init_robot(void);
void Receive_Motion();
void resetDRC();
void resetMotor();
////////////////////////////////////////////////////////////////////////////////

#endif // __CODRAGON_ROBOT_PROTOCOL_H__
