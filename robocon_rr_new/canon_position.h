void moveActuator(){
    
  if(data.ch[5]==1950){ // go down
    analogWrite(actuatorPwm,120);
    long pos[1];
    pos[0] = xx;
    steppers.moveTo(pos);
    steppers.runSpeedToPosition();
    xx-=500;
    digitalWrite(actuatorINA,LOW);
    digitalWrite(actuatorINB,HIGH);
  }else if(data.ch[5]==1050){ // go up
    
    analogWrite(actuatorPwm,170);
    long pos[1];
    pos[0] = xx;
    steppers.moveTo(pos);
    steppers.runSpeedToPosition();
    xx+=500;
    digitalWrite(actuatorINA,HIGH);
    digitalWrite(actuatorINB,LOW);
  }else if(data.ch[5]==1500){
    digitalWrite(actuatorINA,LOW);
    digitalWrite(actuatorINB,LOW);
  }
}

void refill(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Acceleration2 X: ");
  Serial.println(a.acceleration.x);
  if (data.ch[10]==1950){
    if(a.acceleration.x > 7){
      digitalWrite(actuatorINA,LOW);
    digitalWrite(actuatorINB,HIGH);
    }else{
    digitalWrite(actuatorINA,LOW);
    digitalWrite(actuatorINB,LOW);
    }
//    if ( actuatorINA == LOW && actuatorINB == LOW){
//      container1.write(95);
//    container2.write(100);
//    container3.write(170);
//    container4.write(120);
//    servoPush.write(80);
//    }
//  }else{
//    if(a.acceleration.x < 9){
//      digitalWrite(actuatorINA,LOW);
//    digitalWrite(actuatorINB,HIGH);
//    }else{
//    digitalWrite(actuatorINA,LOW);
//    digitalWrite(actuatorINB,LOW);
//  }
  }
}
