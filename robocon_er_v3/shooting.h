void CannonSpeed() {
  if (data.ch[11] != 1950) {
    if (cannonSpeed < 1200) {
      cannonSpeed = 1200;
    } else if (cannonSpeed > 1500) {
      cannonSpeed = 1500;
    }
    if (data.ch[5] != 1500) {
      if (data.ch[5] == 1050) {
        cannonSpeed += 1;
      } else if (data.ch[5] == 1950) {
        cannonSpeed -= 1;
      }
    }
  } else {
    if (data.ch[5] == 1500) {
      cannonSpeed = 1450;
    }
    //highest speed 
    else if (data.ch[5] == 1050) {
      cannonSpeed = 1300;
    }
    //lowest speed
    else if (data.ch[5] == 1950) {
      cannonSpeed = 1400;
    }
  }
  motorCanon.writeMicroseconds(cannonSpeed);
  Serial.println(cannonSpeed);
  Serial.println(data.ch[5]);
  lcd.backlight();
  lcd.print(cannonSpeed);
}

void containerServoNPushRing() {
   if (data.ch[8] != buttonA) {
        
        analogWrite(pwmR, 200);
        digitalWrite(pwmL, LOW);
        delay(1300);
        analogWrite(pwmL, 200);
        digitalWrite(pwmR, LOW);
        delay(1300);
        digitalWrite(pwmR, LOW);
        digitalWrite(pwmL, LOW);
        buttonA = data.ch[8];
      } else {
        buttonA = data.ch[8];
      }
//  long pos1[1];
//  if (data.ch[9] == 1500) {
//    if (data.ch[4] != 1500) {
//      if (data.ch[4] == 1950) {  //down
//        pos1[0] = containerPosition;
//        containerSteppers1.moveTo(pos1);
//        containerSteppers1.runSpeedToPosition();
//        containerPosition -= 2500;
//      } else if (data.ch[4] == 1050) {  //up
//        pos1[0] = containerPosition;
//        containerSteppers1.moveTo(pos1);
//        containerSteppers1.runSpeedToPosition();
//        containerPosition += 2500;
//      }
//    }
//    containerBottom = containerPosition;
//    containerTop = containerPosition + 83800;
//  } else {
//    if (data.ch[9] == 1950) {
//      //3200 = 8mm, 4800 = 1 ring 28
//      pos1[0] = containerTop;
//      containerSteppers1.moveTo(pos1);
//      containerSteppers1.runSpeedToPosition();
//      if (data.ch[8] != buttonA) {
//        pushRing.write(10);
//        delay(2000);
//        pushRing.write(65);
//        analogWrite(pwmR, 200);
//        digitalWrite(pwmL, LOW);
//        delay(1300);
//        analogWrite(pwmL, 200);
//        digitalWrite(pwmR, LOW);
//        delay(1300);
//        digitalWrite(pwmR, LOW);
//        digitalWrite(pwmL, LOW);
//        containerTop += 4800;
//        pos1[0] = containerTop;
//        containerSteppers1.moveTo(pos1);
//        containerSteppers1.runSpeedToPosition();
//        pushRing.write(10);
//        delay(2000);
//        pushRing.write(65);
//        buttonA = data.ch[8];
//      } else {
//        buttonA = data.ch[8];
//      }
//      containerPosition = containerTop;
//    } else {
//      pos1[0] = containerBottom;
//      containerSteppers1.moveTo(pos1);
//      containerSteppers1.runSpeedToPosition();
//      containerPosition = containerBottom;
//    }
//  }
}
