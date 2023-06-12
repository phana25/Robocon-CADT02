void initialize() {
  motorCanon.writeMicroseconds(1500);
  delay(5000);
  
  pushRing.write(65);
  
  if (sbus_rx.Read()) {
    data = sbus_rx.data();
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      data.ch[i] = map(data.ch[i], 282, 1722, 1050, 1950);
    }

    buttonA = data.ch[8];

    sbus_tx.data(data);
    sbus_tx.Write();
  }
  delay(5000);
}
