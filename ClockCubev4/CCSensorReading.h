/*


   SENSOR FUNCTIONS


*/

int irSensorRead(int ir) {

  for (c1 = 0; c1 < irSensorPollTimes; c1 ++) {

    digitalWrite(irEmitterPin[ir], LOW);                //Send out a burst of Infrared
    delay(1);                                           //Delay one mili second to allow calculation of distance
    irAmbient[ir] = analogRead(irSensorPin[ir]);        //Read the Infrared receiver to detect any resudual IR light
    digitalWrite(irEmitterPin[ir], HIGH);               //Send out a burst of Infrared
    delay(1);                                           //Delay one mili second to allow calculation of distance
    irObstical[ir] = analogRead(irSensorPin[ir]);       //Read the Infrared received to detect any bounced light
    irValue[ir][c1] = irAmbient[ir] - irObstical[ir];   //Deduct the ambiant light from the bounced light read
    irDistance[ir] += irValue[ir][c1]; 
    
  }

  return (irDistance[ir] / irSensorPollTimes);

}

int accelerometerSensorRead() {

  adxl.readAccel(&xAxis, &yAxis, &zAxis);               //Collect the readings from the ADXL345 accelerometer
  adxlTotal = adxlTotal - adxlArray[adxlCurrReading];   //Remove the first reading from the array from the total
  adxlArray[adxlCurrReading] = xAxis;                   //Add the X Axis to the array of readings
  adxlTotal = adxlTotal + adxlArray[adxlCurrReading];   //Add that same reading to the total
  adxlCurrReading ++;                                   //Increase the current reading counter

  if (adxlCurrReading >= adxlNumReadings) {
    adxlCurrReading = 0;                                //Take the average of the last nth number of reading stored in the array
  }

  return (adxlTotal / adxlNumReadings);

}

int lrdSensorRead() {

  lrdTotal = lrdTotal - lrdReadingArray[lrdCurrReading];         //Remove the first reading from the array from the total
  lrdReadingArray[lrdCurrReading] = analogRead(lrdPin);             //Add the current level to the array of readings
  lrdTotal = lrdTotal + lrdReadingArray[lrdCurrReading];         //Add that same reading to the total
  lrdCurrReading ++;                                      //Increase the current reading counter

  if (lrdCurrReading >= lrdNumReadings) {
    lrdCurrReading = 0;
  }

  return (lrdTotal / lrdNumReadings * lrdMultiplier);  //Take the average of the last nth number of reading stored in the array

}
