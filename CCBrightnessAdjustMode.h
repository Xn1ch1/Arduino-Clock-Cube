void brightnessAdjustFunction() {

  Tlc.setAll(clockBrightness);
  
  if (irShortPressActive[0])  {

    //Add 1 to the multiplier
    lrdMultiplier ++;
    //Reset back to 1 if it reaches 5
    if (lrdMultiplier > 4) {

      lrdMultiplier = 1;

    }

  }

  if (irLongPressActive[0] && !timeDisplayModeActive) {
    
    turnOffBrightnessAdjustMode();

  }

  if (timeDisplayModeActive) {

    turnOffTimeDisplayMode();
    
  }

}
