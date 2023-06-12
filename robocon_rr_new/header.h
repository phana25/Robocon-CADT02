/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial1);
/* SBUS object, writing SBUS */
bfs::SbusTx sbus_tx(&Serial1);
/* SBUS data */
bfs::SbusData data;

//declare pin wheels
int pwm1 = 3;
int rev1 = 23;
int brake1 = 24;

int pwm2 = 2;
int rev2 = 26;
int brake2 = 27;

int pwm3 = 4;
int rev3 = 32;
int brake3 = 33;

int pwm4 = 5;
int rev4 = 29;
int brake4 = 30;

int maxSpeed = 150;
int minSpeed = 255;

//Canon position
int actuatorPwm = 7;
int actuatorINA = 44;
int actuatorINB = 45;


//Motor canon
Servo motorCanon;

//Push Ring
//number 5
Servo servoPush;
//number 3
Servo container1;
//number 4
Servo container2;
//number 1
Servo container3;
//number 2
Servo container4;

#define DIRx 36
#define STEPx 35
#define EN 34
AccelStepper x(1,STEPx,DIRx);
MultiStepper steppers;

int buttonA = 1050;
int buttonB = 1050;
int CanonSpeed = 1000;

long xx = 0;
