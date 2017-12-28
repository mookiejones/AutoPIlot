#if Compass == 1
void BNO055()
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);
heading = event.orientation.x;
roll = event.orientation.y;  // in degrees
pitch = event.orientation.z;
  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */

  /* The processing sketch expects data as roll, pitch, heading */ 
/*
  Serial.print(F("Orientation: "));
  Serial.print((float)event.orientation.x);
  Serial.print(F(" "));
  Serial.print((float)event.orientation.y);
  Serial.print(F(" "));
  Serial.print((float)event.orientation.z);
  Serial.println(F(""));
*/
  /* Also send calibration data for each sensor. */
/*
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  Serial.print(F("Calibration: "));
  Serial.print(sys, DEC);
  Serial.print(F(" "));
  Serial.print(gyro, DEC);
  Serial.print(F(" "));
  Serial.print(accel, DEC);
  Serial.print(F(" "));
  Serial.println(mag, DEC);
*/
/**********************************************/
//  ROUTINES FROM BNO055 EXAMPLE rawdata

  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2

  // To get euler angles
 // imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

//BEARING RATE TWO WAYS 
  //imu::Vector<3> gyros = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
 // bearingrate = -ToDeg(float(gyros.y()));
 // bearingrate = -ToDeg((float(gyros.z())*cos(roll) + float(gyros.y())*sin(roll))*cos(pitch) - float(gyros.x())*sin(pitch)); // 
 //The method above works great on the Pololu IMU9 but doesn't work here because of my confusion with xy axes and or the accelerometers not calibrating
 //Testing on a turntable showed the delta_heading/delta_time to work well so will go with that for now
  delta_heading = heading - heading_old;
    if (abs(delta_heading) > 180) // this limits error to < 180 
      {
         if(heading_old < heading) delta_heading = delta_heading - 360;
         if(heading_old > heading) delta_heading = 360 + delta_heading;
      }
  float BR2_DeltaT = float((millis()-bearingrate2_time)/1000.0);     
  bearingrate = delta_heading /BR2_DeltaT;
  bearingrate2_time = millis();
  heading_old = heading;
// if (counter2 >47){Serial.print(bearingrate); Serial.print(",  "); Serial.print(bearingrate2); Serial.print(",  "); Serial.println( BR2_DeltaT);}
  
  /* Display the floating point data */
 /*
  Serial.print("X: ");
  Serial.print(euler.x());
  Serial.print(" Y: ");
  Serial.print(euler.y());
  Serial.print(" Z: ");
  Serial.print(euler.z());
  Serial.print("\t\t");
*/
  /*
  // Quaternion data
  imu::Quaternion quat = bno.getQuat();
  Serial.print("qW: ");
  Serial.print(quat.w(), 4);
  Serial.print(" qX: ");
  Serial.print(quat.y(), 4);
  Serial.print(" qY: ");
  Serial.print(quat.x(), 4);
  Serial.print(" qZ: ");
  Serial.print(quat.z(), 4);
  Serial.print("\t\t");
  */

  /* Display calibration status for each sensor. */
 
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  bnoCAL_status = int(system)*1000 + int(gyro)*100 + int(accel)*10 + int(mag); 
  if(Screen == 0){
  lcd.setCursor(6,3); // prints system cal status last 4 digit on line 0 of LCD
  lcd.print(system),lcd.print(gyro);lcd.print(accel);lcd.print(mag);
  }
 /*
  Serial.print("CALIBRATION: Sys=");
  Serial.print(system, DEC);
  Serial.print(" Gyro=");
  Serial.print(gyro, DEC);
  Serial.print(" Accel=");
  Serial.print(accel, DEC);
  Serial.print(" Mag=");
  Serial.println(mag, DEC);
*/
}  // End BNO055

#endif

/**************************************************************************/


