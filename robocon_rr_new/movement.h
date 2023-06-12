
void moveForward() {
  int dutyCycle = map(data.ch[2], 1500, 1950, minSpeed, maxSpeed);
  digitalWrite(rev1, LOW);
  analogWrite(pwm1, dutyCycle);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, dutyCycle);
  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, dutyCycle);
  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, dutyCycle);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("Foward");
}

void moveBackward() {
  int dutyCycle = map(data.ch[2], 1500, 1050, minSpeed, maxSpeed);
  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, dutyCycle);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, dutyCycle);
  digitalWrite(rev3, LOW);
  analogWrite(pwm3, dutyCycle);
  digitalWrite(rev4, LOW);
  analogWrite(pwm4, dutyCycle);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("Backward");
}

void moveRight() {
  int dutyCycle = map(data.ch[3], 1500, 1950, minSpeed, maxSpeed);
  digitalWrite(rev1, LOW);
  analogWrite(pwm1, dutyCycle);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, dutyCycle);
  digitalWrite(rev3, LOW);
  analogWrite(pwm3, dutyCycle);
  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, dutyCycle);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("Right");
}

void moveLeft() {
  int dutyCycle = map(data.ch[3], 1500, 1050, minSpeed, maxSpeed);
  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, dutyCycle);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, dutyCycle);
  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, dutyCycle);
  digitalWrite(rev4, LOW);
  
  analogWrite(pwm4, dutyCycle);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("Left");
}

void rotateRight() {
  int dutyCycle = map(data.ch[0], 1500, 1950, minSpeed, 200);
  digitalWrite(rev1, LOW);
  analogWrite(pwm1, dutyCycle);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, dutyCycle);
  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, dutyCycle);
  digitalWrite(rev4, LOW);
  analogWrite(pwm4, dutyCycle);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("Rotate Right");
}

void rotateLeft() {
  int dutyCycle = map(data.ch[0], 1500, 1050, minSpeed, 200);
  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, dutyCycle);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, dutyCycle);
  digitalWrite(rev3, LOW);
  analogWrite(pwm3, dutyCycle);
  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, dutyCycle);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("Rotate Left");
}

void FDL() {
  int dutyCycle = map(data.ch[2], 1600, 1950, minSpeed, maxSpeed);
  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, dutyCycle);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, 255);
  digitalWrite(rev3, LOW);
  analogWrite(pwm3, dutyCycle);
  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, 255);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("FDL");
}

void FDR() {
  int dutyCycle = map(data.ch[2], 1600, 1950, minSpeed, maxSpeed);
  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, 255);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, dutyCycle);
  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, 255);
  digitalWrite(rev4, LOW);
  analogWrite(pwm4, dutyCycle);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("FDR");
}

void BDL() {
  int dutyCycle = map(data.ch[2], 1600, 1050, minSpeed, maxSpeed);
  digitalWrite(rev1, HIGH);
  analogWrite(pwm1, 255);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, dutyCycle);
  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, 255);
  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, dutyCycle);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("BDL");
}

void BDR() {
  int dutyCycle = map(data.ch[3], 1600, 1950, minSpeed, maxSpeed);
  digitalWrite(rev1, LOW);
  analogWrite(pwm1, dutyCycle);
  digitalWrite(rev2, HIGH);
  analogWrite(pwm2, 255);
  digitalWrite(rev3, HIGH);
  analogWrite(pwm3, dutyCycle);
  digitalWrite(rev4, HIGH);
  analogWrite(pwm4, 255);
  digitalWrite(brake1, HIGH);
  digitalWrite(brake2, HIGH);
  digitalWrite(brake3, HIGH);
  digitalWrite(brake4, HIGH);
  Serial.println("BDR");
}

void stopWheel() {
  digitalWrite(rev1, LOW);
  analogWrite(pwm1, 0);
  digitalWrite(rev2, LOW);
  analogWrite(pwm2, 0);
  digitalWrite(rev3, LOW);
  analogWrite(pwm3, 0);
  digitalWrite(rev4, LOW);
  analogWrite(pwm4, 0);
  digitalWrite(brake1, LOW);
  digitalWrite(brake2, LOW);
  digitalWrite(brake3, LOW);
  digitalWrite(brake4, LOW);
}

void movement() {
  //control wheels
  if (data.ch[0] > 1500) {
    rotateRight();
  }
  else if (data.ch[0] < 1500) {
    rotateLeft();
  }
  else if (data.ch[2] > 1500 && 1400 < data.ch[3] && data.ch[3] < 1600) { // forward
    moveForward();
  }
  else if (data.ch[2] < 1500 && 1400 < data.ch[3] && data.ch[3] < 1600) { // backward
    moveBackward();
  }
  else if (data.ch[3] > 1500 && 1400 < data.ch[2] && data.ch[2] < 1600) { // right
    moveRight();
  }
  else if (data.ch[3] < 1500 && 1400 < data.ch[2] && data.ch[2] < 1600) { // left
    moveLeft();
  }
  else if (data.ch[2] > 1600 && data.ch[3] < 1400) { //forward diagonal left
    FDL();
  }
  else if (data.ch[2] > 1600 && data.ch[3] > 1600) { //forward diagonal right
    FDR();
  }
  else if (data.ch[2] < 1400 && data.ch[3] < 1400 ) { //backward diagonal left
    BDL();
  }

  else if (data.ch[3] > 1600 && data.ch[2] < 1400) { //backward diagonal right
    FDR();
  }
  else  // no movement
  {
    analogWrite(pwm1, 0);
    analogWrite(pwm2, 0);
    analogWrite(pwm3, 0);
    analogWrite(pwm4, 0);

    digitalWrite(brake1, LOW);
    digitalWrite(brake2, LOW);
    digitalWrite(brake3, LOW);
    digitalWrite(brake4, LOW);
  }
}
