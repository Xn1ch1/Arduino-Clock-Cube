void turnOffBrightnessAdjustMode() {

  Serial.println("TURNING OFF BRIGHTNESS MODE");
  
  //Store brightness multiplier to the EEPROM and turn off brightness mode
  EEPROM.write(lrdMultiplierMemory, lrdMultiplier);
  brightnessAdjustModeActive = false;

}

void turnOffAlarmAdjustMode() {

  //Write Adjusted Alarm to EEPROM
  EEPROM.write(alarmHourMemory, alarmHour);
  EEPROM.write(alarmMinuteMemory, alarmMinute);

  alarmAdjustModeActive = false;

}

void turnOffTimeDisplayMode() {

  Serial.println("TURNING OFF TIME DISPLAY MODE");
  
  timeDisplayModeActive = false;

}

void turnOffTimeAdjustMode() {

  //Write the edited time to the RTC
  rtc.adjust(DateTime(now.day(), now.month(), now.year(), timeHourTemp, timeMinuteTemp, 0));
  //Turn off the mode
  timeAdjustModeActive = false;

}

void turnOffAlarm() {

  //Turn off the alarm if it is running
  //Set alarm activation to false
  //Reset the timers and counts for next run
  //Turn off the alarm sound
  //Reset the alarm time

  alarmModeActive = false;
  alarmToneTimer = 0;
  alarmAnimationTimer = 0;
  alarmToneCounter = 0;
  analogWrite(alarmBuzzerPin, 0);
  alarmHour = EEPROM.read(alarmHourMemory);
  alarmMinute = EEPROM.read(alarmMinuteMemory);

}
void snoozeAlarm() {

  alarmModeActive = false;
  alarmToneTimer = 0;
  alarmAnimationTimer = 0;
  alarmToneCounter = 0;
  analogWrite(alarmBuzzerPin, 0);

  if (alarmMinute > 59) {

    alarmMinute = 0;
    alarmHour = alarmHour + 1;
    
  } else {

    alarmMinute = alarmMinute + 5;
    
  }

}
