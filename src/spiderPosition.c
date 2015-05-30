#include "spiderPosition.h"


void setStandPos(struct Motor** motors) {

	motors[1]->ini_pos=270;
	motors[2]->ini_pos=750;
	motors[3]->ini_pos=525;
	motors[4]->ini_pos=492;
	motors[5]->ini_pos=510;
	motors[6]->ini_pos=488;
	motors[7]->ini_pos=520;
	motors[8]->ini_pos=493;
	motors[9]->ini_pos=510;
	motors[10]->ini_pos=521;
	motors[11]->ini_pos=499;
	motors[12]->ini_pos=526;
	motors[13]->ini_pos=614;
	motors[14]->ini_pos=414;
	motors[15]->ini_pos=529;
	motors[16]->ini_pos=503;
	motors[17]->ini_pos=519;
	motors[18]->ini_pos=523;

	motors[1]->pos=270;
	motors[2]->pos=750;
	motors[3]->pos=525;
	motors[4]->pos=492;
	motors[5]->pos=510;
	motors[6]->pos=488;
	motors[7]->pos=520;
	motors[8]->pos=493;
	motors[9]->pos=510;
	motors[10]->pos=521;
	motors[11]->pos=499;
	motors[12]->pos=526;
	motors[13]->pos=614;
	motors[14]->pos=414;
	motors[15]->pos=529;
	motors[16]->pos=503;
	motors[17]->pos=519;
	motors[18]->pos=523;


}
void setSitPos(struct Motor** motors) {
	motors[1]->pos=404;
	motors[2]->pos=647;
	motors[3]->pos=368;
	motors[4]->pos=705;
	motors[5]->pos=572;
	motors[6]->pos=536;
	motors[7]->pos=522;
	motors[8]->pos=508;
	motors[9]->pos=670;
	motors[10]->pos=350;
	motors[11]->pos=479;
	motors[12]->pos=537;
	motors[13]->pos=636;
	motors[14]->pos=390;
	motors[15]->pos=666;
	motors[16]->pos=335;
	motors[17]->pos=459;
	motors[18]->pos=523;
}
void initArm(struct Arm* arm, int id_start,struct Motor** motors)
{
  arm->motors[0] = motors[id_start];
  arm->motors[1] = motors[id_start+2];
  arm->motors[2] = motors[id_start+4];

}
void initArms(struct Arm** arms, struct Motor **motors)
{
  initArm( arms[0],1,motors );
  initArm( arms[1],2,motors );
  initArm( arms[2],7,motors );
  initArm( arms[3],8,motors );
  initArm( arms[4],13,motors );
  initArm( arms[5],14,motors );
}


void printAllPositions(int nb_motor){
	int i;
	for (i=1;i<=nb_motor;i++)
		printf("motors[%d]->pos=%d;\n",i,getPosition(i));
}



void initMotor(int id,int min, int max,int direction, struct Motor **motors){
	motors[id]->id=id;
	motors[id]->min=min;
	motors[id]->max=max;
	motors[id]->direction = direction*-1;
}
void initAllMaxMinMotors(struct Motor** motors)
{
  initMotor(1,200,500, 1, motors );
  initMotor(2,506,805,1, motors );
  initMotor(3,202,823,1, motors );
  initMotor(4,222,828,-1, motors );
  initMotor(5,300,750,1, motors );
  initMotor(6,10,750,1, motors );
  initMotor(7,232,800, 1, motors );
  initMotor(8,195,806,1, motors );
  initMotor(9,208,825,-1, motors );
  initMotor(10,200,815,1, motors );
  initMotor(11,10,700,1, motors );
  initMotor(12,250,800,1, motors );
  initMotor(13,523,810, 1, motors );
  initMotor(14,212,507,1, motors );
  initMotor(15,225,795,-1, motors );
  initMotor(16,196,804,1, motors );
  initMotor(17,250,750,1, motors );
  initMotor(18,250,850,1, motors );
}
