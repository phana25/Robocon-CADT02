#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Servo.h>
#include <MultiStepper.h>
#include <AccelStepper.h>
#include "sbus.h"
#include "header.h"
#include "initialize.h"
#include "movement.h"
#include "shooting.h"

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

  pinMode(pwmR, OUTPUT);
  pinMode(pwmL, OUTPUT);

  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_rx.Begin();
  sbus_tx.Begin();

  motorCanon.attach(6,1000,2000);
  pushRing.attach(8);
  
  x1.setMaxSpeed(15000.0);
  containerSteppers1.addStepper(x1);
  pinMode(EN1, OUTPUT);
  digitalWrite(EN1, 0);

  initialize();
    // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  
}

void loop() {
  if (sbus_rx.Read()) {
    /* Grab the received data */
    data = sbus_rx.data();

    for (int8_t i = 0; i < data.NUM_CH; i++) {
      data.ch[i] = map(data.ch[i], 282, 1722, 1050, 1950);
      // Serial.print(data.ch[i]);
      // Serial.print("\t");
    }
    CannonSpeed();
    movement();
    containerServoNPushRing();
    Serial.println(cannonSpeed);
    /* Set the SBUS TX data to the received data */
    sbus_tx.data(data);
    /* Write the data to the servos */
    sbus_tx.Write();
  }
}
