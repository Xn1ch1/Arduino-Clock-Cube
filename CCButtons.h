void checkButtonStatus() {

  if (orientation > -orentationFlag && orientation < orentationFlag){c3 = 1;} else {c3 = 2;}

  for (char iR = 0; iR < c3; iR++) {

    if (irDistance[iR] > irDistanceFlag[iR]) {

      /*
              OBJECT IN RANGE
      */
      if (!irActivated[iR]) {

        /*
                OBJECT IN RANGE & NOT ACTIVATED
        */
        //SET CURRENT STATUS TO TRUE
        irCurrent[iR] = true;

        //FIRST TIME ACTIVATION
        if (irCurrent[iR] != irPrevious[iR]) {

          //RECORD THE TIME OF ACIVATION
          irTimer[iR] = millis() % 65535;
          irPrevious[iR] = true;

        }

        //OBJECT IN PLACE LONG ENOUGH TO ENABLE SWITCH
        if ((millis() % 65535) - irTimer[iR] > irShortPressTime) {

          irActivated[iR] = true;

        }

      } else {

        /*
                OBJECT IN RANGE & ACTIVATED
        */
        if (irLongPressActiveOnce[iR]) {

          irLongPressActive[iR] = false;

        } else {

          //CHECK IS OBJECT IN RANGE LONG ENOUGH FOR LONG PRESS
          if ((millis() % 65535) - irTimer[iR] > irLongPressTime) {

            irLongPressActive[iR] = true;
            irLongPressActiveOnce[iR] = true;
            soundBuzzer(2, 50, 50);
         
          }

        }

      }

    } else {

      /*
              OBJECT NOT IN RANGE
      */
      if (!irActivated[iR]) {

        /*
                OBJECT NOT IN RANGE & NOT ACTIVATED
        */
        irTimer[iR] = 0;
        irCurrent[iR] = false;
        irPrevious[iR] = false;
        irLongPressActive[iR] = false;
        irLongPressActiveOnce[iR] = false;
        irShortPressActive[iR] = false;

      } else {

        /*
                OBJECT NOT IN RANGE & ACTIVATED
        */
        if (irLongPressActiveOnce[iR]) {

          //LONG PRESS WAS ACTIVE
          irLongPressActiveOnce[iR] = false;

        } else {

          irShortPressActive[iR] = true;
          soundBuzzer(1, 50, 50);

        }

        irActivated[iR] = false;

      }

    }

  }

}
