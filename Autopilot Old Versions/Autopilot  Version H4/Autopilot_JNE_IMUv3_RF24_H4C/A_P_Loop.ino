  
 /***********************
 this loop attempts to integrate gyro and GPS with Gyro and PID at 50 hz 
 **************************/
 
   void A_P_Loop()
  { 
    static int DT_test;  // to print loop times
    int rudder_duration = 40;  // this is time the rudder remains on each time it is turned, this slows down rudder
    if((millis()-timer)>=20)  // Main loop runs at 50Hz
  {
    counter++;
    counter2++;
    counter3++;
    timer_old = timer;
    timer=millis();
    if (timer>timer_old)
      G_Dt = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
    else
      G_Dt = 0;
    
    // *** DCM algorithm
    // Data adquisition
    Read_Gyro();   // This read gyro data
    Read_Accel();     // Read I2C accelerometer
      // Calculations...
    Matrix_update(); 
    Normalize();
    Drift_correction();
    Euler_angles();
    Bearing_Rate();
 
    if(ET.receiveData()) 
      {
        get_GPS_data();
      } // end if(ET.receive data())
    
    Is_GPS_Available();  //Checks UTC failing to increase for more than 6 seconds 
    if( !GPS_Available && GPS_Was_Available)  
    {
      MSG = 1; // NO GPS
      if(Steering_Mode == 2) MSG = 2; // No Gps Steering HTS
      Waypoint_next = "GPS LOST";
    }
   
    // read compass more often
    if (counter > 8)  // with delays loop runs at 10Hz  
     {
      counter=0;
      Read_Compass();    // Read I2C magnetometer
      Compass_Heading(); // Calculate magnetic heading Pololus Mag heading    
      JNE_AP_Compass_Correction();  // compute true heading
      //Tracking_Error(); // Difference between GPS course and compass heading, updates continuously @ 1hz
/*
      if(Serial_Remote_Is_Connected)
         {
           Remote_Send_Data (); 
           ET2.sendData();  // Sends data to Remote
         }
 */   
 #if RF24_Attached == 1
     sendData1(); // RF send data1
 #endif
   
    Steer_PID(); 
   
   } // end if counter > 8 10 Hz loop
      
      
   #if PRINT_DATA == 1 // See tab Print_2 needed for Python display note thi is conditional compiling
   printdata();
   #endif
   
   if(Print_Gyro)
    {  
    Serial.print("Gyros ");
    Serial.print(ToDeg( Gyro_Vector[0])); Serial.print("' ");
    Serial.print( ToDeg(Gyro_Vector[1])); Serial.print("' ");
    Serial.println( ToDeg(Gyro_Vector[2]));
      lcd.setCursor(0,0);
      lcd.print ("GYROS");
      lcd.setCursor(0,1);
      lcd.print(ToDeg(Gyro_Vector[0]));
       lcd.setCursor(0,2);
      lcd.print(ToDeg(Gyro_Vector[1]));
       lcd.setCursor(0,3);
      lcd.print(ToDeg(Gyro_Vector[2]));
    }
  }  // end fast loop if millis > 20
  
  if (counter2 >47) // print loop
  {
   //this executes at 1 hz 
   counter2 = 0; 
   //get_GPS_data(); //  This receives the GPS data from separate Arduino, does so in 2 milliseconds 
   if(Print_LCD_AP) LCD(); //This is main LCD print of GPS and Compass can be turned off to use 
   //LCD to print special purpose diagnostics
   if(Print_LCD_IMU9) LCDprint(); 
   if (print_Nav_Data) NAV_DATA_PRINT();  // Print_1 Tab 
  // DT_test = millis() - DT_test;
  // Serial.println(DT_test);  // diagnostic to check loop timing  
  // DT_test = millis();
  //Serial.println(heading);  
  }  // end counter2 1 hz loop
  
/*  if (counter3 >= 3000 && Print_heading) // 3000 = 1 minute Special purpose print loop for diagnotics, comment out
  {
      //Read_Compass();    // Read I2C magnetometer this is not main loop read compass here for diagnotic delete this line
      //Compass_Heading(); // Calculate magnetic heading   Delete this line  
   
    //Serial.print(millis()/1000); Serial.print(", ");
    //Serial.print (magnetom_x); Serial.print(", ");
    //Serial.print (magnetom_y); Serial.print(", ");
    //Serial.print (magnetom_z); Serial.print(", ");
   //Serial.print (c_magnetom_x,8); Serial.print(", ");
   //Serial.print (c_magnetom_y,8); Serial.print(", ");
   // Serial.print (pitch,8); Serial.print(", ");
   // Serial.print (roll,8); Serial.print(", ");
    // Serial.print ("Heading = ");
   // Serial.print(MAG_X,8); Serial.print(", ");
    //Serial.print(MAG_Y,8); Serial.print(", ");
   // Serial.print(MAG_Heading_Degrees,5); Serial.print(", ");
   AVG_Heading = .9 *AVG_Heading + .1 *MAG_Heading_Degrees;
   //Serial.println(AVG_Heading,1); 
    Serial.println(heading); 
    counter3 = 0;
    }  // end slow  print loop
*/    
  }  // end void AP loop
 
