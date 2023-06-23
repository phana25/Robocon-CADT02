#include "sbus.h"
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

#define debug Serial
#define home_ring 48  //container sensor pin
#define minPush 49    //ring pusher sensor pin
#define maxPush 12    //ring pusher sensor pin
#define ina 9         //driver pin
#define inb 10        //driver pin
#define escpin 6      //cannon esc pin
//wheel motors pins
#define pwm1 3
#define pwm2 2
#define pwm3 5
#define pwm4 4
#define dir1 23
#define dir2 26
#define dir3 29
#define dir4 32
//container steppers pin: 41, 42

const int Mpush_ring_Min = 1600;
const int Mpush_ring_Max = 800;

// movement speed
int min_speed = 90;
int max_speed = 110;
const int const_tim = 50;

int canonSpeed = 1185;
int buttonA = 282;
int buttonB = 282;
int buttonC;
int switchF;
int timeReload = 0;
int canonMode;
int containerMode;

Servo Mpush_ring, esc;
AccelStepper Mpull_ring(1, 41, 42);

int Mpush_ring_angle = 1600;
int pwmval = 0;

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial1);
bfs::SbusData data;
String channels = "";
boolean manual = false;
const int min_val = 282;
const int mid_val = 1002;
const int max_val = 1722;

const int pre_min_val = 802;
const int pre_max_val = 1202;

void setup() {

  debug.begin(115200);
  debug.setTimeout(100);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dir4, OUTPUT);
  analogWrite(pwm1, pwmval);
  analogWrite(pwm2, pwmval);
  analogWrite(pwm3, pwmval);
  analogWrite(pwm4, pwmval);
  pwmval = 50;
  pinMode(ina, OUTPUT);
  pinMode(inb, OUTPUT);
  pinMode(home_ring, INPUT_PULLUP);
  pinMode(minPush, INPUT_PULLUP);
  pinMode(maxPush, INPUT_PULLUP);
  Mpush_ring.attach(8, 544, 2400);
  esc.attach(escpin, 1000, 1500);
  Mpull_ring.setMaxSpeed(3000);
  Mpush_ring.writeMicroseconds(Mpush_ring_angle);
  esc.writeMicroseconds(1050);
  sbus_rx.Begin();
  delay(5000);
  Movehome_ring();

  channels = "";
  if (sbus_rx.Read()) {
    data = sbus_rx.data();
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      channels = channels + String(data.ch[i]) + ",";
    }
    buttonA = data.ch[8];
    buttonC = 0;
    switchF = 0;
    buttonB = data.ch[9];
  }
}

void loop() {
  channels = "";
  if (sbus_rx.Read()) {
    data = sbus_rx.data();
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      channels = channels + String(data.ch[i]) + ",";
    }
  }
  if (data.ch[11] == 1722) {
    manual = true;
  } else {
    manual = false;
  }
  if (manual == true) {
    if (data.ch[2] == mid_val && data.ch[3] == mid_val && data.ch[0] == mid_val && data.ch[1] == mid_val) {
      wheels("sto", 0, 1000);
      // set brake to stop
    }
    if (data.ch[0] != mid_val) {
      if (data.ch[0] > pre_max_val) {
        int velocity = map(data.ch[0], pre_max_val, max_val, 60, 80);
        wheels("rr", velocity, const_tim);
      } else if (data.ch[0] < pre_min_val) {
        int velocity = map(data.ch[0], pre_min_val, min_val, 60, 80);
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

    //turn on esc cannon
    CanonSpeed();

    //shoot
    if (data.ch[8] != buttonA && containerMode == 1) {
      shoot();
      reload();
      buttonA = data.ch[8];
    }

    //container
    container();
  }
  if (debug.available() > 0) {

    String cmd = debug.readString();
    if (cmd.indexOf("rl") > -1) {
      reload();
    }
    if (cmd.indexOf("sh") > -1) {
      shoot();
    }
    if (cmd.indexOf("pd") > -1) {
      plat(false);
    }
    if (cmd.indexOf("pu") > -1) {
      plat(true);
    }
    if (cmd.indexOf("ru") > -1) {
      pick_up(5350);
    }
  }
}

void accMpush_ring(int dir_ang, int acc) {
  if ((dir_ang <= Mpush_ring_Min) && (dir_ang >= Mpush_ring_Max)) {
    if (dir_ang > Mpush_ring_angle) {
      for (int i = Mpush_ring_angle; i <= dir_ang; i++) {
        Mpush_ring.writeMicroseconds(i);
        delayMicroseconds(acc);
      }
    } else {
      for (int i = Mpush_ring_angle; i >= dir_ang; i--) {
        Mpush_ring.writeMicroseconds(i);
        delayMicroseconds(acc);
      }
    }
    Mpush_ring_angle = dir_ang;
  }
}

void Movehome_ring() {
  Mpull_ring.setSpeed(-1000);
  //  Mpull_ring.setAcceleration(1000);
  if (digitalRead(home_ring) == 1) {
    while (digitalRead(home_ring) == 1) {
      Mpull_ring.runSpeed();
    }
  }
  timeReload = 0;
  Mpull_ring.stop();
}

void plat(boolean stat) {
  Mpull_ring.setSpeed(1000);
  Mpull_ring.setAcceleration(10000);
  if (stat == true) {
    Mpull_ring.move(350);
  } else {
    Mpull_ring.move(-350);
  }
  while (Mpull_ring.distanceToGo() != 0) {
    Mpull_ring.run();
  }
}

void pick_up(long h_ring) {
  Mpull_ring.setSpeed(1000);
  Mpull_ring.setAcceleration(1000);
  Mpull_ring.move(h_ring);
  while (Mpull_ring.distanceToGo() != 0) {
    Mpull_ring.run();
  }
}

void reload() {
  accMpush_ring(Mpush_ring_Max, 500);
  delay(500);
  accMpush_ring(Mpush_ring_Min, 500);
  delay(500);
  if (timeReload < 9) {
    pick_up(330);
    timeReload++;
  } else if (timeReload == 9) {
    timeReload++;
  } else {
    timeReload = 0;
    Movehome_ring();
  }
}

void Mpush(int dir) {
  if (dir == 0) {
    analogWrite(ina, 100);
    analogWrite(inb, 0);
  } else if (dir == 1) {
    analogWrite(ina, 0);
    analogWrite(inb, 150);
  } else {
    analogWrite(ina, 0);
    analogWrite(inb, 0);
  }
}

void shoot() {
  while (digitalRead(maxPush) == 1) {
    Mpush(1);
  }
  // delay(300);
  while (digitalRead(minPush) == 1) {
    Mpush(0);
  }
  Mpush(2);
}

void wheels(String strcmd, float velo, long tim) {
  float offSet1;
  float offSet2;
  float offSet3;
  float offSet4;

  if (strcmd.indexOf("sto") > -1) {
    offSet1 = 0;
    offSet2 = 0;
    offSet3 = 0;
    offSet4 = 0;
    accMove(velo, tim, offSet1, offSet2, offSet3, offSet4);
  }
  if (strcmd.indexOf("fw") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 0);
    // offSet1 = 7;
    // offSet2 = 0;
    // offSet3 = 10;
    // offSet4 = 13;
    offSet1 = 10;
    offSet2 = 0;
    offSet3 = 0;
    offSet4 = 13;
    accMove(velo, tim, offSet1, offSet2, offSet3, offSet4);
  }
  if (strcmd.indexOf("rv") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 1);
    // offSet1 = -10;
    // offSet2 = -10;
    // offSet3 = 0;
    // offSet4 = 30;
    offSet1 = 0;
    offSet2 = 0;
    offSet3 = 0;
    offSet4 = 13;
    accMove(velo, tim, offSet1, offSet2, offSet3, offSet4);
  }
  if (strcmd.indexOf("lf") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 0);
    // offSet1 = 13;
    // offSet2 = 0;
    // offSet3 = 10;
    // offSet4 = 0;
    offSet1 = 0;
    offSet2 = 0;
    offSet3 = 0;
    offSet4 = 15;
    accMove(velo, tim, offSet1, offSet2, offSet3, offSet4);
  }
  if (strcmd.indexOf("rg") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 1);
    // offSet1 = 27;
    // offSet2 = -5;
    // offSet3 = 0;
    // offSet4 = 0;
    offSet1 = 0;
    offSet2 = 0;
    offSet3 = 0;
    offSet4 = 15;
    accMove(velo, tim, offSet1, offSet2, offSet3, offSet4);
  }
  if (strcmd.indexOf("rr") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 0);
    offSet1 = 0;
    offSet2 = 0;
    offSet3 = 0;
    offSet4 = 15;
    accMove(velo, tim, offSet1, offSet2, offSet3, offSet4);
  }
  if (strcmd.indexOf("rl") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 1);
    offSet1 = 0;
    offSet2 = 0;
    offSet3 = 0;
    offSet4 = 15;
    accMove(velo, tim, offSet1, offSet2, offSet3, offSet4);
  }
}

// void accMove(int Speed, long tim, int offSet1, int offSet2, int offSet3, int offSet4) {
//   if (Speed > pwmval) {
//     for (int i = pwmval; i <= Speed; i++) {
//       analogWrite(pwm1, i + offSet1);
//       analogWrite(pwm2, i + offSet2);
//       analogWrite(pwm3, i + offSet3);
//       analogWrite(pwm4, i + offSet4);

//       delayMicroseconds(tim);
//     }
//   } else if (Speed < pwmval) {
//     for (int i = pwmval; i >= Speed; i--) {
//       analogWrite(pwm1, i + offSet1);
//       analogWrite(pwm2, i + offSet2);
//       analogWrite(pwm3, i + offSet3);
//       analogWrite(pwm4, i + offSet4);

//       delayMicroseconds(tim);
//     }
//   }
//   pwmval = Speed;
// }

void accMove(float Speed, long tim, float offSet1, float offSet2, float offSet3, float offSet4) {
  if (Speed > pwmval) {
    float tmp = Speed - pwmval;
    for (float i = 0; i <= tmp; i++) {
      analogWrite(pwm1, pwmval + (i * (1 + offSet1 / (Speed - pwmval))));
      analogWrite(pwm2, pwmval + (i * (1 + offSet2 / (Speed - pwmval))));
      analogWrite(pwm3, pwmval + (i * (1 + offSet3 / (Speed - pwmval))));
      analogWrite(pwm4, pwmval + (i * (1 + offSet4 / (Speed - pwmval))));

      Serial.print(String(pwmval + (i * (1 + offSet1 / (Speed - pwmval)))));
      Serial.print(":");
      Serial.println(String(pwmval + (i * (1 + offSet4 / (Speed - pwmval)))));

      delayMicroseconds(tim);
    }
  } else if (Speed < pwmval) {
    float tmp = pwmval - Speed;
    for (float i = 0; i <= tmp; i++) {
      analogWrite(pwm1, pwmval - (i * (1 - offSet1 / (pwmval - Speed))));
      analogWrite(pwm2, pwmval - (i * (1 - offSet2 / (pwmval - Speed))));
      analogWrite(pwm3, pwmval - (i * (1 - offSet3 / (pwmval - Speed))));
      analogWrite(pwm4, pwmval - (i * (1 - offSet4 / (pwmval - Speed))));

      Serial.print(String(pwmval - (i * (1 - offSet1 / (pwmval - Speed)))));
      Serial.print(":");
      Serial.println(String(pwmval - (i * (1 - offSet4 / (pwmval - Speed)))));

      delayMicroseconds(tim);
    }
  }
  pwmval = Speed;
}

void CanonSpeed() {
  if (data.ch[6] == 282) {
    canonMode = 0;
  } else if (data.ch[6] == 1002) {
    canonMode = 1;
  } else if (data.ch[6] == 1722) {
    canonMode = 2;
  }
  if (canonMode == 0) {
    if (data.ch[5] == 1002) {
      canonSpeed = 1462;
    } else if (data.ch[5] == 282) {
      canonSpeed = 1317;
    } else if (data.ch[5] == 1722) {
      canonSpeed = 1800;
    }
  } else if (canonMode == 1) {
    if (data.ch[5] != 1002) {
      if (data.ch[5] == 282) {
        if (data.ch[9] != buttonB) {
          canonSpeed--;
          buttonB = data.ch[9];
        }
      } else if (data.ch[5] == 1722) {
        if (data.ch[9] != buttonB) {
          canonSpeed++;
          buttonB = data.ch[9];
        }
      }
    }
  } else if (canonMode == 2) {
    if (data.ch[5] != 1002) {
      if (data.ch[5] == 282) {
        canonSpeed--;
      } else if (data.ch[5] == 1722) {
        canonSpeed++;
      }
    }
  }

  if (canonSpeed < 1185) {
    canonSpeed = 1185;
  } else if (canonSpeed > 2000) {
    canonSpeed = 2000;
  }
  esc.writeMicroseconds(canonSpeed);
  // debug.println(canonSpeed);
  //type 1: 1317(old ring), 1310(new ring)
  //type 2: 1462(new ring)
}

void container() {
  if (data.ch[10] == 282) {
    containerMode = 0;
  } else if (data.ch[10] == 1002) {
    containerMode = 1;
  } else if (data.ch[10] == 1722) {
    containerMode = 2;
  }
  if (containerMode == 0) {
    Movehome_ring();
    buttonC = 0;
    min_speed = 100;
    max_speed = 120;
  }
  if (containerMode == 2 && buttonC == 0) {
    plat(true);
    buttonC = 1;
    min_speed = 100;
    max_speed = 140;
  }
  if (containerMode == 1 && buttonC == 1) {
    plat(false);
    pick_up(5250);
    reload();
    buttonC = 0;
  }
}
