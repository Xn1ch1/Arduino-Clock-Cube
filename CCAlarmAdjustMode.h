void alarmAdjustFunction() {

  //Check to see if Alarm Mode is off
  if (!alarmAdjustModeActive) {

    flashClockFace();

    //Set alarm mode to true to indicate new operating mode
    alarmAdjustModeActive = true;

  }

  Tlc.clear();
  Tlc.set(tlcAlarm, clockBrightness);

  if (irShortPressActive[0]) {

    //Add 5 minutes to the alarm setting
    alarmMinute = alarmMinute + 5;

    //Go back to 0 when 60 minutes has been reached
    if (alarmMinute == 60) {

      alarmMinute = 0;

    }

  }
  
  if (irLongPressActive[0]) {

    //Add 5 minutes to the alarm setting
    alarmMinute = alarmMinute - 5;

    //Go back to 0 when 60 minutes has been reached
    if (alarmMinute == 0) {

      alarmMinute = 60;

    }

  }

  if (irShortPressActive[1]) {

    //Add one to the current hour
    alarmHour ++;

    //Set the hour back to 0 if it has reached 24
    if (alarmHour == 24) {

      alarmHour = 0;

    }

  }
  
  if (irLongPressActive[1]) {

    //Change from am to pm and vice versa
    if(alarmHour < 12) {
      alarmHour = alarmHour + 12;
    } else {
      alarmHour = alarmHour - 12;
    }

  }

  if (alarmHour < 12) {
    Tlc.set(alarmAdjustTlcArrayHours[alarmHour], clockBrightness);
    Tlc.set(tlcAMPM, 0);
  } else {
    Tlc.set(alarmAdjustTlcArrayHours[alarmHour - 12], clockBrightness);  
    Tlc.set(tlcAMPM, clockBrightness);
  }

  Tlc.set(alarmAdjustTlcArrayMinutes[alarmMinute / 5], clockBrightness);

}
