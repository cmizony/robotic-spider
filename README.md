# Robotic Spider
*Autonomous spider robot which can walk and turn on multiple surfaces*

![robotic-sipder](http://camille.mizony.com/img/portfolio/p2.png)

## Bioloid kit

The kit is created by **Robotic** and the spider can be pieced together in 20 steps detailed on 
[the manual](BIO_PRM_KingSpider_ASM_EN.pdf).
It contains 18 motors that each can be controlled using the Dynamixel SDK.

The SDK is written in C language with a very low level set of instruction available on their 
[api reference](http://support.robotis.com/en/software/dynamixel_sdk/api_reference.htm)

## Code

Here is the implementation (in [api.c](src/api.c)) of the low level API for the motors to wrap the Dynamixel SDK:
```c
void moveMotor(int id, int position);
void moveMotorAndWait(int id, int position);
int getPosition(int id);
void setTorque(int id, int value);
int isMoving(int id);
```

Then on top of that [spider.c](src/spider.c) contains the implementation of the key functions to make the spider moving and turn around. 
Here are the list of action supported:
* Stop
* Stand
* Turn Right
* Turn Left
* Sit
* Walk
* Increase speed
* Lower speed
