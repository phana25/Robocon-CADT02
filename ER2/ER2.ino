#include "sbus.h"
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

#define debug Serial
#define home_ring 48
#define minPush 49
#define maxPush 13
#define ina 46
#define inb 47
#define escpin 6
#define pwm1 3
#define pwm2 2
#define pwm3 5
#define pwm4 4
#define dir1 23
#define dir2 26
#define dir3 29
#define dir4 32

const int Mpush_ring_Min = 1600;
const int Mpush_ring_Max = 700;
const int minspeed = 100;
const int maxspeed = 140;

int canonSpeed = 1000;
int buttonA = 282;

int buttonB;
int buttonG;
int timeReload = 0;
int canonMode;

Servo Mpush_ring, esc;
AccelStepper Mpull_ring(1, 41, 42);

int Mpush_ring_angle = 1600;
int pwmval = 0;

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial1);
bfs::SbusData data;
String channels = "";
boolean manual = false;

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
  esc.writeMicroseconds(900);
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
    buttonB = 0;
    buttonG = 3;
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
    if (data.ch[2] == 1002 && data.ch[3] == 1002 && data.ch[0] == 1002 && data.ch[1] == 1002) {
      wheel("sto", 0, 1000);
    }
    if (data.ch[0] != 1002) {
      if (data.ch[0] > 1202) {
        int velocity = map(data.ch[0], 1202, 1722, 70, 90);
        wheel("rr", velocity, 50);
      } else if (data.ch[0] < 802) {
        int velocity = map(data.ch[0], 802, 282, 70, 90);
        wheel("rl", velocity, 50);
      }
    } else {
      if (data.ch[2] > 1202 && data.ch[3] > 802 && data.ch[3] < 1202) {
        int velocity = map(data.ch[2], 1202, 1722, minspeed, maxspeed);
        wheel("fw", velocity, 50);
      } else if (data.ch[2] < 802 && data.ch[3] > 802 && data.ch[3] < 1202) {
        int velocity = map(data.ch[2], 802, 282, minspeed, maxspeed);
        wheel("rv", velocity, 50);
      }
      if (data.ch[3] > 1202 && data.ch[2] > 802 && data.ch[2] < 1202) {
        int velocity = map(data.ch[3], 1202, 1722, minspeed, maxspeed);
        wheel("rg", velocity, 50);
      } else if (data.ch[3] < 802 && data.ch[2] > 802 && data.ch[2] < 1202) {
        int velocity = map(data.ch[3], 802, 282, minspeed, maxspeed);
        wheel("lf", velocity, 50);
      }
    }
    CanonSpeed();
    if (data.ch[8] != buttonA) {
      shoot();
      reload();
      buttonA = data.ch[8];
    }

    //container
    if (data.ch[9] != 1002) {
      buttonB = 1;
      Movehome_ring();
    }
    if (data.ch[9] == 1002 && buttonB == 1) {
      pick_up(5350);
      reload();
      buttonB = 0;
    }
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
    pick_up(350);
    timeReload++;
  } else {
    Movehome_ring();
    timeReload = 0;
  }
}

void Mpush(int dir) {
  if (dir == 0) {
    analogWrite(ina, 255);
    analogWrite(inb, 0);
  } else if (dir == 1) {
    analogWrite(ina, 0);
    analogWrite(inb, 255);
  } else {
    analogWrite(ina, 0);
    analogWrite(inb, 0);
  }
}

void shoot() {
  while (digitalRead(maxPush) == 1) {
    Mpush(1);
  }
  while (digitalRead(minPush) == 1) {
    Mpush(0);
  }
  Mpush(2);
}

void wheel(String strcmd, int velo, long tim) {
  if (strcmd.indexOf("sto") > -1) {
    accMove(velo, tim);
  }
  if (strcmd.indexOf("fw") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 0);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("rv") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 1);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("lf") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 0);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("rg") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 1);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("rr") > -1) {
    digitalWrite(dir1, 0);
    digitalWrite(dir2, 0);
    digitalWrite(dir3, 0);
    digitalWrite(dir4, 0);
    accMove(velo, tim);
  }
  if (strcmd.indexOf("rl") > -1) {
    digitalWrite(dir1, 1);
    digitalWrite(dir2, 1);
    digitalWrite(dir3, 1);
    digitalWrite(dir4, 1);
    accMove(velo, tim);
  }
}

void accMove(int Speed, long tim) {
  if (Speed > pwmval) {
    for (int i = pwmval; i <= Speed; i++) {
      analogWrite(pwm1, i);
      analogWrite(pwm2, i);
      analogWrite(pwm3, i);
      analogWrite(pwm4, i);
      delayMicroseconds(tim);
    }
  } else if (Speed < pwmval) {
    for (int i = pwmval; i >= Speed; i--) {
      analogWrite(pwm1, i);
      analogWrite(pwm2, i);
      analogWrite(pwm3, i);
      analogWrite(pwm4, i);
      delayMicroseconds(tim);
    }
  }
  pwmval = Speed;
}

void CanonSpeed() {
  if (data.ch[10] == 282) {
    canonMode = 0;
  } else if (data.ch[10] == 1002) {
    canonMode = 1;
  } else if (data.ch[10] == 1722) {
    canonMode = 2;
  }

  if (canonMode == 0) {
    if (data.ch[5] != 1002) {
      if (data.ch[5] == 282) {
        canonSpeed --;
      } else if (data.ch[5] == 1722) {
        canonSpeed ++;
      }
    }
    if (canonSpeed < 1000) {
      canonSpeed = 1000;
    } else if (canonSpeed > 1900) {
      canonSpeed = 1900;
    }
  } else if (canonMode == 1) {
    if (data.ch[5] == 1002) {
      canonSpeed = 1000;
    } else if (data.ch[5] == 282) {
      canonSpeed = 1280;
    } else if (data.ch[5] == 1722) {
      canonSpeed = 1141;
    }
  } else if (canonMode == 2) {
    if (data.ch[5] == 1002) {
      buttonG = 3;
    }
    if (buttonG != 0 && data.ch[5] != 1002) {
      if (data.ch[5] == 282) {
        buttonG = 2;
      } else if (data.ch[5] == 1722) {
        buttonG = 1;
      }
      if (buttonG == 2) {
        canonSpeed--;
        buttonG = 0;
      }
      else if (buttonG == 1) {
        canonSpeed ++;
        buttonG = 0;
      }
    }
  }
  esc.writeMicroseconds(canonSpeed);
  debug.println(canonSpeed);
}
