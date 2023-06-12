void initialize(){
  motorCanon.writeMicroseconds(950);
 
  delay(2000);
  container1.write(10);
  container2.write(170);
  container3.write(100);
  container4.write(230);
  servoPush.write(80);

  if (sbus_rx.Read()) {
    data = sbus_rx.data();
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      data.ch[i] = map(data.ch[i], 282, 1722, 1050, 1950);
    }
     
    buttonA = data.ch[8];
    
    sbus_tx.data(data);
    sbus_tx.Write();
  }

  
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  delay(500);
}
