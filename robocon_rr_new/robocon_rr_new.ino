#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Servo.h>
#include <MultiStepper.h>
#include <AccelStepper.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
Adafruit_MPU6050 mpu;

#include "sbus.h"
#include "header.h"
#include "initialize.h"
#include "movement.h"
#include "shooting.h"
#include "canon_position.h"
#include "stepper.h"
//#include "autoRefill.h"


void setup() {

  pinMode(pwm1, OUTPUT);
  pinMode(rev1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(rev2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(rev3, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(rev4, OUTPUT);
  pinMode(brake1, OUTPUT);
  pinMode(brake2, OUTPUT);
  pinMode(brake3, OUTPUT);
  pinMode(brake4, OUTPUT);
  pinMode(actuatorINA, OUTPUT);
  pinMode(actuatorINB, OUTPUT);

  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_rx.Begin();
  sbus_tx.Begin();
  
  motorCanon.attach(6);

  motorCanon.writeMicroseconds(1000);

  servoPush.attach(8); //5
  
  container1.attach(9); //3
  container2.attach(10); //4
  container3.attach(11); //1
  container4.attach(12); //2

   x.setMaxSpeed(13000.0);
  steppers.addStepper(x);
  pinMode(EN,OUTPUT);
  digitalWrite(EN,0);
  initialize();
   // initialize the LCD
  lcd.begin();
}

void loop (){
  if (sbus_rx.Read()) {
    /* Grab the received data */
    data = sbus_rx.data();

    for (int8_t i = 0; i < 12; i++) {
      data.ch[i] = map(data.ch[i], 282, 1722, 1050, 1950);
      Serial.print(i);
      Serial.print(":");
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }
    Serial.println();
    canonSpeed();
    moveActuator();
    movement();
    printLCD();
    containerServo();
    runto();
    refill();

    /* Set the SBUS TX data to the received data */
    sbus_tx.data(data);
    /* Write the data to the servos */
    sbus_tx.Write();
  }
}
