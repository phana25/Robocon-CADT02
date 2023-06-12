/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial1);
/* SBUS object, writing SBUS */
bfs::SbusTx sbus_tx(&Serial1);
/* SBUS data */
bfs::SbusData data;

//declare pin wheels
//wheel 1 pin 4 j43
int pwm1 = 4;
int rev1 = 32;
int brake1 = 33;

//wheel 2 pin 5 j42
int pwm2 = 5;
int rev2 = 29;
int brake2 = 30;

//wheel 3 pin 2 j41
int pwm3 = 2;
int rev3 = 26;
int brake3 = 27;

//wheel 4 pin 3 j40
int pwm4 = 3;
int rev4 = 23;
int brake4 = 24;

//shoot
//foward
int pwmR = 9;
//backward
int pwmL = 10;

int MAXSpeed = 230;
int MINSpeed = 255;

//Motor canon
Servo motorCanon;

//container
#define DIRx1 42
#define STEPx1 41
#define EN1 40
AccelStepper x1(1, STEPx1, DIRx1);
MultiStepper containerSteppers1;

long xx = 0;

int buttonA = 1050;
int cannonSpeed = 1500;

long containerPosition = 0;
long containerTop = 0;
long containerBottom = 0;
long pushRingPosition = 0;
long shootRingPosition = 0;

Servo pushRing;
