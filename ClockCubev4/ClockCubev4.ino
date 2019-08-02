/*

    The Clock Cube by Craig Hancock

    Arduino Pin Configuration
    0
    1
    2
    3          >             TLC5940 GLCSK
    4
    5          >             Buzzer Signal
    6
    7          >             Top IR LED
    8          >             Bottom IR LED
    9          >             TLC5940 XLAT
    10         >             TLC5940 BLANK
    11         >             TLC5940 SIN
    12
    13         >             TLC5940 SCLK
    A0         >             Top IR Photo-transistor LED Cathode Via 4k7 Resistor
    A1         >             Bottom IR Photo-transistor LED Cathode Via 4k7 Resistor
    A2         >             lrdPin Signal
    A3
    A4         >             SCL Input ADXL345 > SCL Input DS3212
    A5         >             SDA Input ADXL345 > SDA Input DS3212
    GND        >             TLC5940 VPRG + ADXL345 + DS3212 + Buzzer + TLC5940 + Photo Transistors + IR LEDs + lrdPin
    5V         >             ADXL345 + DS3212 + Buzzer + TLC5940 + Photo Transistors + IR LEDs + lrdPin
    3.3V       >             SDA > SCL Via 100k Resistor

*/

/*


      LIBRARIES


*/
#include "Wire.h"
#include "Tlc5940.h"
#include "RTClib.h"
#include "EEPROM.h"
#include "ADXL345.h"

/*


      PIN CONFIG


*/
int alarmBuzzerPin = 5;
int lrdPin = A2;
int irSensorPin[2] = {A0, A1};
int irEmitterPin[2] = {7, 8};

/*


      EEPROM ADDRESS CONFIG


*/
int alarmHourMemory = 0;
int alarmMinuteMemory = 1;
int lrdMultiplierMemory = 2;
boolean alarmSetMemory = 3;

/*


      ALARM SETTINGS


*/
int alarmMinute = 0;
int alarmHour = 0;
int alarmToneCounter = 0;
boolean alarmSet = false;
boolean alarmToneOn = false;
boolean alarmSnooze = false;
boolean alarmRang = false;
unsigned int alarmAnimationTimer = 0;
unsigned int alarmToneTimer = 0;

/*


      TIME SETTINGS


*/
int timeHourTemp = 0;
int timeMinuteTemp = 0;
unsigned char timeHour = 0;
unsigned char timeMinute = 0;
unsigned char timeSecond = 0;
boolean amNotPm = false;

/*


      LIGHT SENSOR SETTINGS


*/
const int lrdNumReadings = 10;
int lrdReadingArray[lrdNumReadings];
int lrdCurrReading = 0;
int lrdTotal = 0;
int lrdMultiplier = 0;

/*


      ORIENTATION SENSOR SETTINGS


*/
const int adxlNumReadings = 5;
int adxlArray[adxlNumReadings];
int xAxis, yAxis, zAxis;
int adxlCurrReading = 0;
int adxlTotal = 0;

/*

    PROXIMITY SENSOR SETTINGS
       [0] = TOP SENSOR
       [1] = BOTTOM SENSOR

*/
int irAmbient[2];
int irObstical[2];
int irValue[2][10];
int irDistance[2];
unsigned int irTimer[2];
boolean irCurrent[2] = {false};
boolean irPrevious[2] = {false};
boolean irActivated[2] = {false};
boolean irLongPressActive[2] = {false};
boolean irLongPressActiveOnce[2] = {false};
boolean irShortPressActive[2] = {false};

/*


      GENERAL USE


*/
int c1;
int c2;
int c3;
int clockBrightness = 0;
int clockBrightnessInactiveDivider = 2;
int orientation = 0;
int animationValues[28];
char timeDisplayTlcArrayMinutes[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
char timeDisplayTlcArrayHours[12] = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
char alarmAdjustTlcArrayMinutes[12] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2};
char alarmAdjustTlcArrayHours[12] = {19, 20, 21, 22, 23, 24, 25, 26, 27, 16, 17, 18};
char timeAdjustTlcArrayMinutes[12] = {9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7, 8};
char timeAdjustTlcArrayHours[12] = {25, 26, 27, 16, 17, 18, 19, 20, 21, 22, 23, 24};
char tlcAMPM = 31;
char tlcAlarm = 15;

/*


      TIMINGS & ACTIVATION POINTS


*/
int irShortPressTime = 25;
int irLongPressTime = 1100;
int orentationFlag = 240;
int animationDelay = 250;
int flashTimes = 3;
int irDistanceFlag[2] = {950, 950};
int irSensorPollTimes = 10;
int alarmAnimationFrameTime = 50;
int alarmToneDelayShort = 150;
int alarmToneDelayLong = 400;
int singleMinuteDelay = 3000;
int singleMinuteFlashDelay = 100;
unsigned long singleMinuteTimer = 0;
unsigned long singleMinuteFlashTimer = 0;
float singleMinuteFlashCounter = 0;

/*


      MODE SETTINGS


*/
boolean timeDisplayModeActive = false;
boolean alarmAdjustModeActive = false;
boolean timeAdjustModeActive = false;
boolean brightnessAdjustModeActive = false;
boolean alarmModeActive = false;

/*


      SHORTHAND CODE


*/
RTC_DS1307 rtc;
ADXL345 adxl;
DateTime now;

/*


      CUSTOM LIBRARIES


*/
#include "CCSoundAndAnimation.h"
#include "CCSensorReading.h"
#include "CCModeManagement.h"
#include "CCAlarmActiveMode.h"
#include "CCAlarmAdjustMode.h"
#include "CCBrightnessAdjustMode.h"
#include "CCTimeAdjustMode.h"
#include "CCTimeDisplayMode.h"
#include "CCButtons.h"

void setup() {

  Serial.begin(9600);

  //SET PIN MODES FOR IR
  for (c1 = 0; c1 < 2; c1++) {
    pinMode(irEmitterPin[c1], OUTPUT);
    digitalWrite(irEmitterPin[c1], LOW);
  }

  Wire.begin();
  rtc.begin();
  Tlc.init();
  adxl.powerOn();

  //SET NOW AS THE RTC MODULE
  now = rtc.now();

  //SET COMPILED DATE AND TIME
  DateTime compiled = DateTime(__DATE__, __TIME__);

  //CHECK RTC TIME VS COMPILE & ADJUST IF THE CLOCK IS OUT
  //if (now.unixtime() < compiled.unixtime()) {

  //rtc.adjust(DateTime(__DATE__, __TIME__));

  //}

  //GET ALL VALUES FROM MEMORY
  alarmHour = EEPROM.read(alarmHourMemory);
  alarmMinute = EEPROM.read(alarmMinuteMemory);
  lrdMultiplier = EEPROM.read(lrdMultiplierMemory);
  alarmSet = EEPROM.read(alarmSetMemory);

}

void loop() {

  readEverything();

  /* ALARM EDIT MODE */
  if (orientation < -orentationFlag) {

    if (alarmModeActive) {
      turnOffAlarm();
    }
    if (brightnessAdjustModeActive) {
      turnOffBrightnessAdjustMode();
    }
    if (timeDisplayModeActive) {
      turnOffTimeDisplayMode();
      rotateClockWise(3);
    }
    if (timeAdjustModeActive) {
      turnOffTimeAdjustMode();
      rotateClockWise(6);
    }

    alarmAdjustFunction();

  }

  /* TIME EDIT MODE */
  if (orientation > orentationFlag) {

    if (alarmModeActive) {
      turnOffAlarm();
    }
    if (brightnessAdjustModeActive) {
      turnOffBrightnessAdjustMode();
    }
    if (timeDisplayModeActive) {
      turnOffTimeDisplayMode();
      rotateAntiClockWise(3);
    }
    if (alarmAdjustModeActive) {
      turnOffAlarmAdjustMode();
      rotateAntiClockWise(6);
    }

    timeAdjustFunction();

  }

  /* TIME DISPLAY MODE */
  if (orientation > -orentationFlag && orientation < orentationFlag) {

    if (alarmAdjustModeActive) {
      turnOffAlarmAdjustMode();
      rotateAntiClockWise(3);
    }
    if (timeAdjustModeActive) {
      turnOffTimeAdjustMode();
      rotateClockWise(3);
    }

    if (!brightnessAdjustModeActive && !alarmModeActive) {
      timeDisplayFunction();
    }

    if (alarmModeActive) {
      if (timeDisplayModeActive) {
        turnOffTimeDisplayMode();
        initiateAlarmMode();
      }
      alarmModeFunction();
    }
    if (brightnessAdjustModeActive) {
      brightnessAdjustFunction();
    }

  }

  Tlc.update();

  Serial.print("IR Distance 0 (");
  Serial.print(irDistance[0]);
  Serial.print(")   :   IR Distance 1 (");
  Serial.print(irDistance[1]);
  Serial.print(")\r");

}

void readEverything() {

  clockBrightness = lrdSensorRead();
  orientation = accelerometerSensorRead();
  irDistance[0] = irSensorRead(0);
  irDistance[1] = irSensorRead(1);
  checkButtonStatus();
  now = rtc.now();
  timeHour = now.hour();
  timeMinute = now.minute();
  timeSecond = now.second();
  amNotPm = now.hour() < 12;

}
