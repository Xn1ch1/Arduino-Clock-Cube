void timeAdjustFunction() {

  //Check to see if Time Edit mode is off
  if (!timeAdjustModeActive) {

    flashClockFace();

    //Take the current time and assign to temporary variable to allow it to be edited
    timeHourTemp = timeHour;
    timeMinuteTemp = (timeMinute / 5) * 5;

    //Turn on Time Edit Mode
    timeAdjustModeActive = true;

  }

  Tlc.clear();

  if (irShortPressActive[0]) {

    timeHourTemp ++;

    if (timeHourTemp == 24) {

      timeHourTemp = 0;

    }

  }

  if (irShortPressActive[1]) {

    timeMinuteTemp = timeMinuteTemp + 5;

    if (timeMinuteTemp == 60) {

      timeMinuteTemp = 0;

    }

  }
  
  if (timeHourTemp < 12) {
    Tlc.set(timeAdjustTlcArrayHours[timeHourTemp], clockBrightness);
    Tlc.set(tlcAMPM, 0);
  } else {
    Tlc.set(timeAdjustTlcArrayHours[timeHourTemp-12], clockBrightness);  
    Tlc.set(tlcAMPM, clockBrightness);
  }

  Tlc.set(timeAdjustTlcArrayMinutes[timeMinuteTemp / 5], clockBrightness);
  
}
