void updateTimeOnFace() {

  Tlc.clear();

  if (timeMinute < 5) {

    Tlc.set(timeDisplayTlcArrayMinutes[0], clockBrightness);

  } else {

    for (c1 = 1; c1 < timeMinute / 5 + 1; c1 ++) {

      Tlc.set(timeDisplayTlcArrayMinutes[c1], clockBrightness / clockBrightnessInactiveDivider);

    }

    Tlc.set(timeDisplayTlcArrayMinutes[c1-1], clockBrightness);

  }

  if (amNotPm) {

    if (timeHour == 0) {

      Tlc.set(timeDisplayTlcArrayHours[0], clockBrightness);

    } else {

      for (c1 = 1; c1 < timeHour + 1; c1 ++) {

        Tlc.set(timeDisplayTlcArrayHours[c1], clockBrightness / clockBrightnessInactiveDivider);

      }

      Tlc.set(timeDisplayTlcArrayHours[c1-1], clockBrightness);

    }

  } else {

    if (timeHour == 12) {

      Tlc.set(timeDisplayTlcArrayHours[0], clockBrightness);

    } else {

      for (c1 = 1; c1 < timeHour - 11; c1 ++) {

        Tlc.set(timeDisplayTlcArrayHours[c1], clockBrightness / clockBrightnessInactiveDivider);

      }

      Tlc.set(timeDisplayTlcArrayHours[c1-1], clockBrightness);

    }

  }

  Tlc.set(tlcAMPM, clockBrightness * !amNotPm);
  Tlc.set(tlcAlarm, clockBrightness * alarmSet);

  if (timeMinute % 5 == 0) {

    singleMinuteTimer = millis();

  } else {

    if(millis() - singleMinuteTimer > singleMinuteDelay - ((timeMinute % 5) * singleMinuteFlashDelay)){

      if(singleMinuteFlashCounter == 0){

        if (timeMinute > 55){
          Tlc.set(timeDisplayTlcArrayMinutes[0], clockBrightness);
        }else{
          Tlc.set(timeDisplayTlcArrayMinutes[timeMinute / 5 + 1], clockBrightness  / (singleMinuteFlashCounter + 1));
        }
        singleMinuteFlashTimer = millis();
        singleMinuteFlashCounter = 0.5;

      } else {

        if(millis() - singleMinuteFlashTimer > singleMinuteFlashDelay){

          singleMinuteFlashCounter = singleMinuteFlashCounter + 0.5;

          if (singleMinuteFlashCounter == floor(singleMinuteFlashCounter)){

            if (timeMinute > 55){
              Tlc.set(timeDisplayTlcArrayMinutes[0], 0);
            }else{
              Tlc.set(timeDisplayTlcArrayMinutes[timeMinute / 5 + 1], 0);
            }

          } else {

            if (timeMinute > 55){
              Tlc.set(timeDisplayTlcArrayMinutes[0], clockBrightness / (singleMinuteFlashCounter + 1));
            }else{
              Tlc.set(timeDisplayTlcArrayMinutes[timeMinute / 5 + 1], clockBrightness / (singleMinuteFlashCounter + 1));
            }

          }

          singleMinuteFlashTimer = millis();

          if (singleMinuteFlashCounter > timeMinute % 5){

            singleMinuteTimer = millis();
            singleMinuteFlashCounter = 0;
            singleMinuteFlashTimer = 0;
            if (timeMinute > 55){
              Tlc.set(timeDisplayTlcArrayMinutes[0], 0);
            }else{
              Tlc.set(timeDisplayTlcArrayMinutes[timeMinute / 5 + 1], 0);
            }

          }

        }

      }

    }

  }

}

void timeDisplayFunction() {

  //Check to see if Clock mode is off
  if (!timeDisplayModeActive) {

    flashClockFace();

    //Define clock mode on
    timeDisplayModeActive = true;

  }

  //Check the alarm time against the real time and activate the alarm if it is enabled (within the first
  if (alarmSet && !alarmRang && alarmMinute == timeMinute && alarmHour == timeHour) {

    //Turn on the alarm itself
    alarmModeActive = true;
    alarmRang = true;

  }

  if (alarmRang && timeMinute == alarmMinute + 1 && alarmHour == timeHour) {

    alarmRang = false;

  }

  updateTimeOnFace();

  if (irShortPressActive[0]) {

    alarmSet = !alarmSet;
    EEPROM.write(alarmSetMemory, alarmSet);

  }

  if (irLongPressActive[0]) {

    brightnessAdjustModeActive = true;

  }

}
