void alarmModeFunction() {

  //Check to see if animation timer has reached its change point
  if ((millis() % 65535) - alarmAnimationTimer > alarmAnimationFrameTime) {

    //Collect the values of the current LEDs
    for (c1 = 0; c1 < 12; c1++) {
      animationValues[c1] = Tlc.get(c1);
      animationValues[c1 + 16] = Tlc.get(c1 + 16);
    }

    for (c1 = 0; c1 < 12; c1++) {
      Tlc.set(c1, animationValues[c1 + 1]);
      Tlc.set(c1 + 17, animationValues[c1 + 16]);
    }

    Tlc.set(11, animationValues[0]);  
    Tlc.set(16, animationValues[27]);

    //Record the current time stamp
    alarmAnimationTimer = millis() % 65535;

  }

  if (alarmToneCounter == 1){
    
    if ((millis() % 65535) - alarmToneTimer > alarmToneDelayShort) {
      alarmToneCounter = 2;
      alarmToneTimer = millis() % 65535;
    }
    
  } else if (alarmToneCounter == 2){
    
    if ((millis() % 65535) - alarmToneTimer > alarmToneDelayShort) {
      alarmToneCounter = 3;
      alarmToneTimer = millis() % 65535;
    }
    
  } else if (alarmToneCounter == 3){
    
    if ((millis() % 65535) - alarmToneTimer > alarmToneDelayShort) {
      alarmToneCounter = 4;
      alarmToneTimer = millis() % 65535;
    }
    
  } else {
    
    if ((millis() % 65535) - alarmToneTimer > alarmToneDelayLong) {
      alarmToneCounter = 1;
      alarmToneTimer = millis() % 65535;
    }
    
  }

  Tlc.set(tlcAMPM, (alarmToneCounter % 2) * 1000);
  analogWrite(alarmBuzzerPin, (alarmToneCounter % 2) * 50);

  if (irLongPressActive[0]) {
    turnOffAlarm();
  }

  if (irShortPressActive[0]) {
    snoozeAlarm();
  }
  
}
void initiateAlarmMode(){

  Tlc.clear();

  //Set up the LEDs for rotation (or other pattern)
  Tlc.set(0, 1000);
  Tlc.set(1, 500);
  Tlc.set(2, 250);
  Tlc.set(3, 50);
  Tlc.set(4, 25);
  
  Tlc.set(16, 1000);
  Tlc.set(27, 500);
  Tlc.set(26, 250);
  Tlc.set(25, 50);
  Tlc.set(24, 25);

  //Record the time stamp to regulate the animation timing
  alarmAnimationTimer = millis() % 65535;

  //Record the current time
  alarmToneTimer = millis() % 65535;
  //Add one to the counter
  alarmToneCounter = 1;

}

