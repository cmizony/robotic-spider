#include <stdlib.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>
#include "api.h"

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46
#define P_TORQUE_LIMIT		34

// Defulat setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID		1

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

/* position entre 0 et 1023 (512 = milieu) */
void moveMotor(int id,int position) {
	dxl_write_word( id, P_GOAL_POSITION_L, position );
	PrintErrorCode();
}

void initApi () {
	int deviceIndex = 0;
	int baudnum = 1;

	///////// Open USB2Dynamixel ////////////
	if( dxl_initialize(deviceIndex, baudnum) == 0 )	
		printf( "Failed to open USB2Dynamixel!\n" );
	else
		printf( "Succeed to open USB2Dynamixel!\n" );
}

void moveMotorAndWait(int id, int position) {
	int index = 0;
	int Moving, PresentPos;
	int CommStatus;
	moveMotor(id,position);

	do{
		// Read present position
		PresentPos = dxl_read_word( id, P_PRESENT_POSITION_L );
		CommStatus = dxl_get_result();

		if( CommStatus == COMM_RXSUCCESS ){
			printf( "%d   %d\n",position, PresentPos );
			PrintErrorCode();
		}
		else{
			PrintCommStatus(CommStatus);
			break;
		}

		// Check moving done
		Moving = dxl_read_byte( id , P_MOVING );
		CommStatus = dxl_get_result();
		if( CommStatus == COMM_RXSUCCESS ){
			if( Moving == 0 ){
				
			}
			PrintErrorCode();
		}
		else{
			PrintCommStatus(CommStatus);
			break;
		}
	}while(Moving == 1);
}

void closeApi() {
	dxl_terminate();
}

int isMoving(int id)
{
  int CommStatus;
  int i = dxl_read_byte( id , P_MOVING );
  return i;
}

// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
  return;
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");
}

int getPosition(int id){
  int i = dxl_read_word(id,P_PRESENT_POSITION_L);
  return i;
}

void setTorque(int id, int value) {
	dxl_write_word(id,P_TORQUE_LIMIT,value);
	PrintErrorCode();
}

