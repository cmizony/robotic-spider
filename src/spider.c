#include <stdlib.h>
#include <stdio.h>
#include "spider.h"
#include "terminal.h" 

#include "api.h"
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <termios.h>


/*
	Correspondance id <> motors
	motors[0] n'existe pas
	motors[1] = id 1
	...
*/
#define NB_MOTORS 18
#define NB_ARMS 6
#define TOLERANCE 10
#define LATENCY 5000
#define PI 3.14159265

typedef enum 
  {
    WALK, STOP, SIT, STAND, TURN_LEFT, TURN_RIGHT    
  } action;

pthread_t thread_motor;
pthread_t thread_keyboard;
pthread_mutex_t mutex_move = PTHREAD_MUTEX_INITIALIZER;
int user_torque = 600;
int move_end;
int raw_mode = 0;
int GAIN = 80;
int SPEED = 1;
action current_action = SIT;

void initSpider(void) {
  int i;
  move_end = 0;
  motors=malloc(sizeof(struct Motor *)*NB_MOTORS+1);
  arms=malloc(sizeof(struct Struct *)*NB_ARMS);
  for (i=0;i<= NB_MOTORS;i++)
    motors[i]=malloc(sizeof(struct Motor));

  for (i=0;i<= NB_ARMS;i++)
    {
      arms[i]=malloc(sizeof(struct Arm));
    }
  initArms( arms, motors );
    
  initAllMaxMinMotors( motors );
  printAllPositions( NB_MOTORS );
  setAllTorques(0);
  setStandPos( motors );
  startMotorHandle();
  startKeyboardHandle();
  waitAndIncr(4);

}


void setAllTorques(int value){
	int i;
	for(i=1;i<=NB_MOTORS;i++)
		motors[i]->torque=value;
}

void moveSpider (void) {
  while (!spiderMoveEnd()){
    moveSpiderMotors();
  }
  pthread_mutex_lock( &mutex_move );
  move_end = 1;
  pthread_mutex_unlock( &mutex_move );
}

void moveSpiderMotors(void){
  int i;
  for (i=1;i<=NB_MOTORS;i++)
    moveSpiderMotor(i);
}

void moveSpiderMotor(int id){
  setTorque(id,motors[id]->torque);
  moveMotor(id,motors[id]->pos);
}

void setSpiderTorque(int id,int value){
	if (value < 0 || value > 1024)
		return;
	motors[id]->torque=value;
}

void setSpiderSinusPos(int id, int t){
    motors[id]->pos = (int)(sin(4.0*PI*(float)(t%1024)/1023)*
			    ((motors[id]->max)-motors[id]->min)/2+
			    ((motors[id]->max)+motors[id]->min)/2);
}


void setMotorPosition(int id_motor, int pos)
{
    motors[id_motor]->pos = pos;
}

int motorMoveEnd(int id){
  return (getPosition(id)<=motors[id]->pos+TOLERANCE && 
	  getPosition(id) >= motors[id]->pos-TOLERANCE);
}

int spiderMoveEnd(void) {
	int i;
	for (i=1;i<=NB_MOTORS;i++)
	  {
	    if (!motorMoveEnd(i))
	      return 0;
	  }
	return 1;
}
void wait()
{
  pthread_mutex_lock( &mutex_move );
  move_end = 0;
  pthread_mutex_unlock( &mutex_move );
  while ( move_end == 0)
    {

      usleep( LATENCY );
    }
}

void waitAndIncr(int torque_incr)
{
  pthread_mutex_lock( &mutex_move );
  move_end = 0;
  pthread_mutex_unlock( &mutex_move );
  while ( move_end == 0)
    {
      int i;
      for (i=1;i<=NB_MOTORS;i++)
	{
	  setSpiderTorque(i,motors[i]->torque+torque_incr);
	}
      usleep( LATENCY );
    }
}


void* motorHandle(void* param)
{
  printf("[INFO] Handle Motor Thread created. \n");
  unsigned int time = 1;
  while (1)
    {
      moveSpider();
      time++;
    }
  
  return NULL;
}

void startMotorHandle()
{
   pthread_create( &thread_motor, NULL, &motorHandle, motors);
}

void startKeyboardHandle()
{
  pthread_create( &thread_keyboard, NULL, &keysControl,NULL);
}

void moveSpiderArm(int id, int a1, int a2, int a3)
{
  setMotorPosition( arms[ id ]->motors[0]->id, a1); 
  setMotorPosition( arms[ id ]->motors[1]->id, a2); 
  setMotorPosition( arms[ id ]->motors[2]->id,  a3); 
}

void riseArm(int id)
{
  if (id == 1 || id==2 || id ==4 )
    setMotorPosition(arms[id]->motors[1]->id, 700);
  else
    setMotorPosition(arms[id]->motors[1]->id, 300);
}

void downArm( int id)
{
  if ( id ==0 || id ==3 )
    setMotorPosition(arms[id]->motors[1]->id, 500);
  else
    setMotorPosition(arms[id]->motors[1]->id, 500);
}

int minMotorSin(int id)
{
  if(id == 13 || id == 2 || id == 14 ||id == 1)
    return ((motors[id]->min+motors[id]->max)/2+motors[id]->min)/2;
  return ((motors[id]->min+motors[id]->max)/2+motors[id]->min)*1.1/2;
}

int maxMotorSin(int id)
{
  if(id == 13 || id ==2 || id == 14 || id == 1)
    return ((motors[id]->min+motors[id]->max)/2+motors[id]->max)/2;
  return ((motors[id]->min+motors[id]->max)/2+motors[id]->max)*0.9/2;
}

void turnRightSpider(int torque)
{
  int i =4;
  setAllTorques( torque);
  while(i--)
    {
      downArm(1);
      downArm(2);
      downArm(5);
      wait();
      riseArm(0);
      riseArm(3);
      riseArm(4);

      setMotorPosition(1, minMotorSin(1));
      setMotorPosition(8, maxMotorSin(8));
      setMotorPosition(13, maxMotorSin(13));


      setMotorPosition(2, minMotorSin(2));
      setMotorPosition(7, minMotorSin(7));
      setMotorPosition(14, minMotorSin(14));
      wait();
      //--------------------

      downArm(0);
      downArm(3);
      downArm(4);
      wait();
      riseArm(1);
      riseArm(2);
      riseArm(5);

      setMotorPosition(1, maxMotorSin(1));
      setMotorPosition(8, minMotorSin(8));
      setMotorPosition(13, minMotorSin(13));


      setMotorPosition(2, maxMotorSin(2));
      setMotorPosition(7, maxMotorSin(7));
      setMotorPosition(14,maxMotorSin(14));
      wait();
    }
}


void setMotorRelativePosition(int id, int pos)
{
  setMotorPosition(id,motors[id]->ini_pos+motors[id]->direction*pos);
}

void getSinValue(int t, float*i, float* j,float* k,float* l,int gain, int speed)
{
  *i = sin ( (t / 100.0)*speed)* gain;
  *k = sin ( (t / 200.0)*speed)* gain;
  *l = sin ( (t / 200.0)*speed+ PI/2)* gain;
  *j= -(*i);
  if ( *i < 0 )
    *i=0;
  if ( *j< 0 )
    *j=0;

  if ( *k <0)
    *k*=-1;
  if ( *l <0)
    *l*=-1;
}
void walk(int t, int gain, int speed)
{
  float i,j,k,l;
  float coef_left = 1.0;
  float coef_right = 1.0;
  getSinValue(t,&i,&j,&k,&l,gain, speed);
  //monte / desc
  setMotorRelativePosition(4,(int)i);
  setMotorRelativePosition(16,(int)i);
  setMotorRelativePosition(9,(int)i);
 
  setMotorRelativePosition(15,(int)j);
  setMotorRelativePosition(10,(int)j);
  setMotorRelativePosition(3,(int)j);
  
  //deplacement
  setMotorRelativePosition(14,(int)(k*coef_left));
  setMotorRelativePosition(7,(int)-(k*1.1*coef_left));
  setMotorRelativePosition(2,(int)(k*coef_left));
  
  setMotorRelativePosition(13,(int)(-l*coef_right));
  setMotorRelativePosition(1,(int)(-l*coef_right));
  setMotorRelativePosition(8 ,(int)(l*1.1*coef_right));
}

void turnLeftRelative(int t, int gain, int speed)
{
  float i,j,k,l;
  float coef_left = 0.50;
  float coef_right = 0.50;
  getSinValue(t,&i,&j,&k,&l, gain, speed);
  //monte / desc
  setMotorRelativePosition(4,(int)i);
  setMotorRelativePosition(16,(int)i);
  setMotorRelativePosition(9,(int)i);
 
  setMotorRelativePosition(15,(int)j);
  setMotorRelativePosition(10,(int)j);
  setMotorRelativePosition(3,(int)j);
  
  //deplacement
  setMotorRelativePosition(14,(int)(k*-coef_left));
  setMotorRelativePosition(7,(int)-(k*1.1*coef_left));
  setMotorRelativePosition(2,(int)(k*-coef_left));
  
  setMotorRelativePosition(13,(int)(-l*coef_right));
  setMotorRelativePosition(1,(int)(-l*coef_right));
  setMotorRelativePosition(8 ,(int)(l*1.1*-coef_right));
}
void turnRightRelative(int t, int gain, int speed)
{
  float i,j,k,l;
  float coef_left = 0.50;
  float coef_right = 0.50;
  getSinValue(t,&i,&j,&k,&l, gain, speed);
  //monte / desc
  setMotorRelativePosition(4,(int)j);
  setMotorRelativePosition(16,(int)j);
  setMotorRelativePosition(9,(int)j);
 
  setMotorRelativePosition(15,(int)i);
  setMotorRelativePosition(10,(int)i);
  setMotorRelativePosition(3,(int)i);
  
  //deplacement
  setMotorRelativePosition(14,(int)(k*-coef_left));
  setMotorRelativePosition(7,(int)-(k*1.1*coef_left));
  setMotorRelativePosition(2,(int)(k*-coef_left));
  
  setMotorRelativePosition(13,(int)(-l*coef_right));
  setMotorRelativePosition(1,(int)(-l*coef_right));
  setMotorRelativePosition(8 ,(int)(l*1.1*-coef_right));
}

void printInformation()
{
  printf("\n================[ Information ] ================\n\n");
  int i;
  for( i=1; i <= NB_MOTORS ; ++i ) 
    {
      printf("Motors %d \t Torque %d \n",i,motors[i]->torque );
    }
}

void* keysControl(void* param)
{
  int key=0;
  
  while(1)
    {
      key = getchar();
      mode_raw(0);
      //if a no-key has been pressed
      if ( key == 0 || key == 10 )
	return NULL;
      //otherwise, do the attributed function and clear the buffer
      switch ( key )
	{
	case 32 /* space*/:
	  printf(" > Stop ! <\n");
	  current_action = STOP;
	  setAllTorques( 10 );
	  break;
	case 'z':
	  printf(" > Stand ! < \n");
	  setStandPos( motors  );
	  current_action = STAND;
	  break;
	case 67:
	  printf(" > Turn Right ! < \n");
	  current_action = TURN_RIGHT;
	  break;
	case 68:
	  printf(" > Turn Left ! < \n");
	  current_action = TURN_LEFT;
	  break;
	case 's':
	  printf(" > Sit ! <\n");
	  setSitPos( motors);
	  current_action = SIT;
	  break;
	case 65:
	  printf(" > Walk ! <\n");
	  current_action = WALK;
	  break;
	case 66:
	  break;
	case 'e':
	  printf(" > 1000 Torque <\n");
	  setAllTorques( 1000);
	  break;
	case 'a':
	  printf(" > No Torque ! <\n");
	  setAllTorques( 10 );
	  break;
	case 'q':
	  setAllTorques( (user_torque-=10 ) );
	  printf(" > Torque -- !  Value : %d<\n",user_torque);
	  
	  break;
	case 'd':
	  printf(" > Torque ! Value : %d<\n", user_torque);
	  setAllTorques( (user_torque +=10) );
	  break;
	case 'i':
	  printInformation();
	  break;
	case 'x':
	  SPEED++;
	  printf(" > Speed Up ! Value : %d<\n", SPEED);
	  break;
	case 'w':
	  SPEED--;
	  printf(" > Slow Down ! Value : %d<\n", SPEED);
	  break;
	case 'v':
	  printf(" > Gain More ! Value : %d<\n", GAIN);
	  GAIN++;
	  break;
	case 'c':
	  printf(" > Gain Less ! Value : %d<\n", GAIN);
	  GAIN--;
	  break;
	case 'k':
	  printKeys();
	  break;
	case 'r':
	  raw_mode = (raw_mode+1) %2;
	  mode_raw(raw_mode);
	  printf(" ==== RAW MODE : %d ======\n\r",raw_mode );
	  break;
	default:
	  break;
	}
      mode_raw( raw_mode );
      
      key = 0;
    }
}
void printKeys()
{
  printf("Actions : \n");
  printf("Stop \t Space \n");
  printf("Stand \t z \n");
  printf("Sit \t s \n");

  printf("SpeedUp \t x \n");
  printf("SlowDown \t w \n");

  printf("GainUp \t v \n");
  printf("GainDown \t c \n");

  printf("Torque ++ \t d \n");
  printf("Torque --\t q \n");
  printf("Walk \t Up_Arrow \n");
  printf("Information \t i \n");
  printf("------------------\n");
  printf("Enable /Disable Raw Mode :  \t r \n");
  printf(" \n");
}

 
void startSpider(void){
  setAllTorques(0);
  setStandPos( motors );
  waitAndIncr(10);

  setAllTorques(500);
  unsigned int i;

  printf("Start Control Loop\n");
  setAllTorques(600);

  printKeys();
  raw_mode = mode_raw(1);
  for(i = 1;;i++){
    //check des valeurs
    if ( user_torque < 10 )
      user_torque = 10;
    if ( user_torque > 1024 )
      user_torque = 1024;
    if ( SPEED > 4 || SPEED < 0 )
      SPEED = 2;

    //check des etats
    if ( current_action == WALK )
      walk( i, GAIN, SPEED );
    if ( current_action == TURN_RIGHT )
      turnRightRelative(i, GAIN, SPEED);
    if ( current_action == TURN_LEFT )
      turnLeftRelative(i, GAIN, SPEED);

    usleep(2000);    
  }
  raw_mode = mode_raw(0);
}
