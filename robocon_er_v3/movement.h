 void moveForward() {
  int dutyCycle = map(data.ch[2], 1500, 1950, MINSpeed, MAXSpeed);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, dutyCycle);

  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, dutyCycle);

  digitalWrite(rev1, LOW);
  analogWrite(pwm1, dutyCycle);

  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, dutyCycle);

  Serial.println("Moving Forward");
}

void moveBackward() {
  int dutyCycle = map(data.ch[2], 1500, 1050, MINSpeed, MAXSpeed);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, dutyCycle);

  digitalWrite(rev3, LOW);
  analogWrite(pwm3, dutyCycle);

  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, dutyCycle);

  digitalWrite(rev4, LOW);
  analogWrite(pwm4, dutyCycle);

  Serial.println("Moving Backward");
}

void moveLeft() {
  int dutyCycle = map(data.ch[3], 1500, 1050, MINSpeed, MAXSpeed);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, dutyCycle);

  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, dutyCycle);

  digitalWrite(rev1, LOW);
  analogWrite(pwm1, dutyCycle);

  digitalWrite(rev4, LOW);
  analogWrite(pwm4, dutyCycle);

  Serial.println("Moving Left");
}

void moveRight() {
  int dutyCycle = map(data.ch[3], 1500, 1950, MINSpeed, MAXSpeed);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, dutyCycle);

  digitalWrite(rev3, LOW);
  analogWrite(pwm3, dutyCycle);

  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, dutyCycle);

  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, dutyCycle);

  Serial.println("Moving Right");
}

void FDR() {
  int dutyCycle = map(data.ch[2], 1600, 1950, MINSpeed, MAXSpeed);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, dutyCycle);

  analogWrite(pwm3, 0);

  analogWrite(pwm1, 0);  // stop pwm4 to stop backward motion

  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, dutyCycle);

  Serial.println("Moving Right Upward");
}

void FDL() {
  int dutyCycle = map(data.ch[2], 1600, 1950, MINSpeed, MAXSpeed);
  
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, dutyCycle);

  analogWrite(pwm3, 0);

  analogWrite(pwm1, 0);  // stop pwm4 to stop backward motion
  digitalWrite(rev4, LOW);
  analogWrite(pwm4, dutyCycle);

  Serial.println("Moving Left Upward");
}

void BDR() {
  int dutyCycle = map(data.ch[3], 1600, 1950, MINSpeed, MAXSpeed);
  analogWrite(pwm2, 0);

  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, dutyCycle);

  digitalWrite(rev1, LOW);
  analogWrite(pwm1, dutyCycle);  // stop pwm4 to stop backward motion

  analogWrite(pwm4, 0);
  

  Serial.println("Moving Right Backward");
}

void BDL() {
  int dutyCycle = map(data.ch[2], 1600, 1050, MINSpeed, MAXSpeed);
  analogWrite(pwm2, 0);

  digitalWrite(rev3, LOW);
  analogWrite(pwm3, dutyCycle);

  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, dutyCycle);  // stop pwm4 to stop backward motion

  analogWrite(pwm4, 0);

  Serial.println("Moving Left Backward");
}

void rotateLeft() {
  int dutyCycle = map(data.ch[0], 1500, 1050, MINSpeed, 200);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, dutyCycle);

  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, dutyCycle);

  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, dutyCycle);

  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, dutyCycle);

  Serial.println("Rotating Left");
}

void rotateRight() {
  int dutyCycle = map(data.ch[0], 1500, 1950, MINSpeed, 200);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, dutyCycle);

  digitalWrite(rev3, LOW);
  analogWrite(pwm3, dutyCycle);

  digitalWrite(rev1, LOW);
  analogWrite(pwm1, dutyCycle);

  digitalWrite(rev4, LOW);
  analogWrite(pwm4, dutyCycle);

  Serial.println("Rotating Right");
}

void stopWheel() {
  digitalWrite(rev1, LOW);
  digitalWrite(rev2, LOW); 
  digitalWrite(rev3, LOW);
  digitalWrite(rev4, LOW);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void movement() {
  //  control wheels
  if (data.ch[0] > 1500) {
    rotateRight();
  } else if (data.ch[0] < 1500) {
    rotateLeft();
  } else if (data.ch[2] > 1500 && 1400 < data.ch[3] && data.ch[3] < 1600) {  // forward
    moveForward();
  } else if (data.ch[2] < 1500 && 1400 < data.ch[3] && data.ch[3] < 1600) {  // backward
    moveBackward();
  } else if (data.ch[3] > 1500 && 1400 < data.ch[2] && data.ch[2] < 1600) {  // right
    moveRight();
  } else if (data.ch[3] < 1500 && 1400 < data.ch[2] && data.ch[2] < 1600) {  // left
    moveLeft();
  } else if (data.ch[2] > 1600 && data.ch[3] < 1400) {  //forward diagonal left
    FDL();
  } else if (data.ch[2] > 1600 && data.ch[3] > 1600) {  //forward diagonal right
    FDR();
  } else if (data.ch[2] < 1400 && data.ch[3] < 1400) {  //backward diagonal left
    BDL();
  }

  else if (data.ch[3] > 1600 && data.ch[2] < 1400) {  //backward diagonal right
    BDR();
  } else  // no movement
  {
    stopWheel();
  }
}
