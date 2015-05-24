#ifndef __SPIDER_POS__H_
#define __SPIDER_POS__H_

#include <stdlib.h>
#include <stdio.h>

#include "spider.h"
#include "api.h"



struct Motor;
struct Arm;


void setSitPos( struct Motor** motors );
void setStandPos( struct Motor** motors );
void initArm(struct Arm* arm, int id_start,struct Motor** motors);
void initArms(struct Arm** arms, struct Motor** motors);
void printAllPositions(int nb_motor);
void initMotor(int id,int min, int max,int direction, struct Motor **motors);
void initAllMaxMinMotors(struct Motor** motors);


#endif
