void canonSpeed(){
  int dutyCycle1 = map(data.ch[6], 950, 1950, 0, 1900);
  
  motorCanon.writeMicroseconds(dutyCycle1);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(dutyCycle1);
  lcd.setCursor(0, 1);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Acceleration2 X: ");
  lcd.print(a.acceleration.x);
  
  
 
}

void printLCD(){
  int dutyCycle = map(data.ch[6], 950, 1950, 0, 1900);
  
  
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(dutyCycle);
  lcd.setCursor(0, 1);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Acceleration2 X: ");
  lcd.print(a.acceleration.x);
  
  
}

void containerServo(){
  if(data.ch[8] != buttonA){
    container1.write(95);
    container2.write(100);
    container3.write(170);
    container4.write(120);
    servoPush.write(80);
  }
  else{
    servoPush.write(12);
    container1.write(10);
    container2.write(170);
    container3.write(100);
    container4.write(230);
    buttonA = data.ch[8];
  }
}
