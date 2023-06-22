#include "sbus.h"
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

#define debug Serial

//cannon esc
#define escpin 6

//wheel motors
#define pwm1 3
#define pwm2 2
#define pwm3 5
#define pwm4 4
#define dir1 23
#define dir2 26
#define dir3 29
#define dir4 32
#define brake1 24
#define brake2 27
#define brake3 33
#define brake4 30

//steppers
#define DIRl 42
#define STEPl 41
#define ENl 40

#define DIRr 39
#define STEPr 38
#define ENr 37

#define DIRp 36
#define STEPp 35
#define ENp 34

AccelStepper stepper1(1, STEPl, DIRl);  // left stepper
AccelStepper stepper2(1, STEPr, DIRr);  // right stepper
AccelStepper stepper3(1, STEPp, DIRp);  // push stepper

// motor1 => front left
// motor2 => front right
// motor3 => rear left
// motor4 => rear right

Servo esc;
int pwmval = 255;  //set pwm motor speed to 0

//SBUS parameter
bool manual;
// const int min_val = 282;
// const int mid_val = 1002;
// const int max_val = 1722;
const int min_val = 1050;
const int mid_val = 1500;
const int max_val = 1950;

// const int pre_min_val = 802;
// const int pre_max_val = 1202;
const int pre_min_val = 1300;  // =1050 + 350
const int pre_max_val = 1700;  // =1950 - 350

const int min_speed = 10;   // movement minimum speed
const int max_speed = 150;  // movement maximum speed

const int const_tim = 50;

int cur_ch9_val = min_val;
bool cont_shoot_pos = false;

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial1);
bfs::SbusData data;
String channels = "";

void setup() {
  debug.begin(115200);
  debug.setTimeout(100);

  esc.attach(escpin, 1050, 1950);  //attach esc pin
  esc.writeMicroseconds(1050);     //initialize esc to mode

  //set all steppers max speed
  stepper1.setMaxSpeed(1000);
  stepper2.setMaxSpeed(1000);
  stepper3.setMaxSpeed(1000);

  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dir4, OUTPUT);

  pinMode(brake1, OUTPUT);
  pinMode(brake2, OUTPUT);
  pinMode(brake3, OUTPUT);
  pinMode(brake4, OUTPUT);

  // set brake to stop
  brakes(1);

  // set motors initial speed to 0
  analogWrite(pwm1, pwmval);
  analogWrite(pwm2, pwmval);
  analogWrite(pwm3, pwmval);
  analogWrite(pwm4, pwmval);

  sbus_rx.Begin();

  // ch1 -> right hor
  // ch2 -> right ver
  // ch3 -> left ver
  // ch4 -> left hor
  // ch5 -> E (left)
  // ch6 -> F (right)
  // ch7 -> G (left)
  // ch8 -> H (right)
  // ch9 -> C (left)
  // ch10 -> D (right)
  // ch11 -> A
  // ch12 -> B
}

void loop() {
  channels = "";
  if (sbus_rx.Read()) {
    data = sbus_rx.data();
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      data.ch[i] = map(data.ch[i], 282, 1722, 1050, 1950);
      channels = channels + String(data.ch[i]) + ",";
      // debug.print(channels);
      // debug.print("\t");
    }
  }
  SBUS();
}

void SBUS() {
  if (data.ch[10] == max_val) {
    manual = true;
  } else {
    manual = false;
  }

  if (manual == true) {
    if (data.ch[2] == mid_val && data.ch[3] == mid_val && data.ch[0] == mid_val && data.ch[1] == mid_val) {
      wheels("sto", 0, 1000);
      // set brake to stop
      brakes(1);
    }
    if (data.ch[0] != mid_val) {
      if (data.ch[0] > pre_max_val) {
        int velocity = map(data.ch[0], pre_max_val, max_val, 70, 90);
        wheels("rr", velocity, const_tim);
      } else if (data.ch[0] < pre_min_val) {
        int velocity = map(data.ch[0], pre_min_val, min_val, 70, 90);
        wheels("rl", velocity, const_tim);
      }
    } else {
      if (data.ch[2] > pre_max_val && data.ch[3] > pre_min_val && data.ch[3] < pre_max_val) {
        int velocity = map(data.ch[2], pre_max_val, max_val, min_speed, max_speed);
        wheels("fw", velocity, const_tim);
      } else if (data.ch[2] < pre_min_val && data.ch[3] > pre_min_val && data.ch[3] < pre_max_val) {
        int velocity = map(data.ch[2], pre_min_val, min_val, min_speed, max_speed);
        wheels("rv", velocity, const_tim);
      }
      if (data.ch[3] > pre_max_val && data.ch[2] > pre_min_val && data.ch[2] < pre_max_val) {
        int velocity = map(data.ch[3], pre_max_val, max_val, min_speed, max_speed);
        wheels("rg", velocity, const_tim);
      } else if (data.ch[3] < pre_min_val && data.ch[2] > pre_min_val && data.ch[2] < pre_max_val) {
        int velocity = map(data.ch[3], pre_min_val, min_val, min_speed, max_speed);
        wheels("lf", velocity, const_tim);
      }
    }

    //control container
    if (data.ch[8] == min_val) {
      if (data.ch[11] == min_val) {
        move_container(0);
        cont_shoot_pos = false;
      } else if (data.ch[11] == max_val) {
        move_container(1);
        cont_shoot_pos = false;
      } else if (data.ch[11] == mid_val) {
        if (cont_shoot_pos == false) {
          move_container(2);
          cont_shoot_pos = true;
        }

        if (data.ch[9] != cur_ch9_val) {
          shootReload();
          Serial.print("shoot");
          cur_ch9_val = data.ch[9];
        }
      }
    }

    //find pos 0 for steppers
    if (data.ch[8] == mid_val) {
      stepper1.setSpeed(800);
      stepper2.setSpeed(800);

      while (data.ch[8] == mid_val) {
        stepper1.runSpeed();
        stepper2.runSpeed();
        // Serial.println("*");
        if (sbus_rx.Read()) {
          data = sbus_rx.data();
        }
      }
    } else if (data.ch[8] == max_val) {
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
    }

    //set cannon speed
    if (data.ch[6] > min_val) {
      int tmp_speed = map(data.ch[6], min_val, max_val, 1050, 1950);
      esc.writeMicroseconds(tmp_speed);
    } else {
      esc.writeMicroseconds(1050);
    }
  }
}

void wheels(String strcmd, int velo, long tim) {
  if (strcmd.indexOf("sto") > -1) {
    brakes(1);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("fw") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 1);

    brakes(0);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("rv") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 0);

    brakes(0);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("lf") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 0);

    brakes(0);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("rg") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 1);

    brakes(0);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("rr") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 0);

    brakes(0);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("rl") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 1);

    brakes(0);
    accMove(velo, tim);
  }
}

void accMove(int speed, long tim) {
  speed = map(speed, 0, 255, 255, 0);  //map the reverse value where 0 is the fastest speed
  if (speed > pwmval) {
    for (int i = pwmval; i <= speed; i++) {
      analogWrite(pwm1, i);
      analogWrite(pwm2, i);
      analogWrite(pwm3, i);
      analogWrite(pwm4, i);
      delayMicroseconds(tim);
    }
  } else if (speed < pwmval) {
    for (int i = pwmval; i >= speed; i--) {
      analogWrite(pwm1, i);
      analogWrite(pwm2, i);
      analogWrite(pwm3, i);
      analogWrite(pwm4, i);
      delayMicroseconds(tim);
    }
  }
  pwmval = speed;
}

void brakes(bool onOff) {
  digitalWrite(brake1, onOff);
  digitalWrite(brake2, onOff);
  digitalWrite(brake3, onOff);
  digitalWrite(brake4, onOff);
}

void move_container(int mode) {
  if (mode == 0) {  // mode pickup rings
    stepper1.setSpeed(1000);
    stepper2.setSpeed(1000);
    stepper1.setAcceleration(10000.0);
    stepper2.setAcceleration(10000.0);
    stepper1.moveTo(0);
    stepper2.moveTo(0);

    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
    }

  } else if (mode == 1) {  // mode standard (run)
    stepper1.setSpeed(1000);
    stepper2.setSpeed(1000);
    stepper1.setAcceleration(10000.0);
    stepper2.setAcceleration(10000.0);
    stepper1.moveTo(-350);
    stepper2.moveTo(-350);

    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
    }

  } else if (mode == 2) {  // mode shooting
    stepper1.setSpeed(1000);
    stepper2.setSpeed(1000);
    stepper1.setAcceleration(10000);
    stepper2.setAcceleration(10000);
    stepper1.moveTo(-400);
    stepper2.moveTo(-400);

    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
    }
  }
}

void shootReload() {
  //stepper push ring to cannon
  stepper3.setSpeed(1000);
  stepper3.setAcceleration(10000);
  stepper3.moveTo(-3500);

  while (stepper3.distanceToGo() != 0) {
    stepper3.run();
  }

  delay(500);

  //stepper pull back to pos 0
  stepper3.moveTo(0);
  while (stepper3.distanceToGo() != 0) {
    stepper3.run();
  }

  //side steppers move up one ring pos
  stepper1.setSpeed(1000);
  stepper2.setSpeed(1000);
  stepper1.setAcceleration(10000.0);
  stepper2.setAcceleration(10000.0);
  stepper1.move(-350);
  stepper2.move(-350);

  while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
    stepper1.run();
    stepper2.run();
  }
}