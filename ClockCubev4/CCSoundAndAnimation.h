/*


   ANIMATION FUNCTIONS


*/
void rotateClockWise(int nTimes) {

  for (c1 = 0; c1 < nTimes; c1++) {

    for (c2 = 0; c2 < 12; c2++) {
      animationValues[c2] = Tlc.get(c2);
      animationValues[c2 + 16] = Tlc.get(c2 + 16);
    }

    for (c2 = 1; c2 < 12; c2++) {
      Tlc.set(c2, animationValues[c2 - 1]);
      Tlc.set(c2 + 16, animationValues[c2 + 15]);
    }

    Tlc.set(0, animationValues[11]);
    Tlc.set(16, animationValues[27]);

    Tlc.update();
    delay(animationDelay);

  }

}

void rotateAntiClockWise(int nTimes) {

  for (c1 = 0; c1 < nTimes; c1++) {

    for (c2 = 0; c2 < 12; c2++) {
      animationValues[c2] = Tlc.get(c2);
      animationValues[c2 + 16] = Tlc.get(c2 + 16);
    }

    for (c2 = 0; c2 < 11; c2++) {
      Tlc.set(c2, animationValues[c2 + 1]);
      Tlc.set(c2 + 16, animationValues[c2 + 17]);
    }

    Tlc.set(11, animationValues[0]);
    Tlc.set(27, animationValues[16]);

    Tlc.update();
    delay(animationDelay);

  }

}

void flashClockFace() {

  for (c1 = 0; c1 < flashTimes; c1++) {

    Tlc.setAll(clockBrightness);
    Tlc.update();
    delay(animationDelay);
    Tlc.clear();
    Tlc.update();
    delay(animationDelay);

  }

}

/*


    SOUND FUNCTIONS



*/
void soundBuzzer(int repeatTimes, int soundDuration, int delayDuration) {

  for (c1 = 1; c1 <= repeatTimes; c1++) {

    analogWrite(alarmBuzzerPin, soundDuration);
    delay(delayDuration);
    analogWrite(alarmBuzzerPin, 0);
    delay(delayDuration);

  }

}
