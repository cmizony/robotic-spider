#ifndef SPIDER
#define SPIDER

#include "spiderPosition.h"

struct Motor{
  int id;
  int min;
  int max;
  int pos;
  int torque;
  int ini_pos;
  int direction;
};

struct Arm{
  struct Motor* motors[3];

};

struct Motor** motors;
struct Arm** arms;

void initSpider(void);

void moveSpider(void);
void setSpiderTorque(int,int);
void moveSpiderMotor(int);
void moveSpiderMotors(void);
int motorMoveEnd(int id);
int spiderMoveEnd(void);
void startSpider(void);
void setAllTorques(int value);
void setMotorPosition(int id_motor, int pos);

void getSinValue(int t, float*i, float* j,float* k,float* l,int gain, int speed);

void* motorHandle(void*);
void startMotorHandle();
void* keysControl(void*);
void startKeyboardHandle();

void setSpiderSinusPos(int id, int t);
void turnRightRelative(int t, int gain, int speed);
void turnLeftRelative(int t, int gain, int speed);
void walk(int t, int gain, int speed);

void handle_end_prog();
void wait();
void waitAndIncr(int torque_incr);
void riseArm(int id);
void moveSpiderArm(int id, int a1, int a2, int a3);
void printKeys();
void printInformation();
#endif
