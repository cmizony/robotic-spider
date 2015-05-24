#ifndef API
#define API

void initApi();
void moveMotor(int id, int position);
void moveMotorAndWait(int id, int position);
void closeApi();
int getPosition(int id);
void setTorque(int id, int value);
int isMoving(int id);
#endif
