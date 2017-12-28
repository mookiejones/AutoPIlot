 /*********************************************************
 PRINT 1 TAB
 these print routines were developed to print GPS and NAV data.  The Print 2 tab is
 the OUTPUT TAB provided with Pololu MinIMU9 software for Gyro compass, They need to be
 integrated in terms of functions and varibles.
 ****************************************************/
 
        void Print_interval(){
          //Allows user to set Serial Port printing interval   
       int  print_timer = millis()-PT_old;
        //temp = print_time*1000;
        if (print_timer  > print_time*1000){
          print_level = print_level_max;
          PT_old = millis();
        }
        else{ print_level=0;}
       /************************** // diagnostic
        print_level=0;
        Serial.print("millis(): "); Serial.println(millis());
        Serial.print("print_time: "); Serial.println(print_time);
        Serial.print("PT_old: "); Serial.println(PT_old);
        Serial.print("Print Interval: "); Serial.println(print_time*1000);
        ***************************************************/
        }  // end of  void Print_interval()
  
  /*********************** PRINT LCD ******************************/
void LCD(){
    // set the cursor to column 0, line 1
    // (note: counting begins with 0):
    String RP;
    int UTC_seconds;
    lcd.begin(20, 4); // initializes LCD and clears it 4 lines 20 characters each
   
    if(Screen == 0)
    {   
     lcd.setCursor(0, 0);
     lcd.print("        ");
     lcd.setCursor(0, 0);
     lcd.print(Waypoint_next); 
          
    // lcd.print(HDG) also prints in compass for fast print rate, prints here for more stable LCD view
     lcd.setCursor(0, 1);
     lcd.print("HDG");
     lcd.setCursor(4, 1);
     lcd.print(heading,0);
       
       
     lcd.setCursor(0, 3);
     lcd.print("          ");
     lcd.setCursor(0,3);
     lcd.print(Mode);
      
     lcd.setCursor(11, 1);   
     lcd.print("HTS");
     lcd.setCursor(15, 1);
     lcd.print(heading_to_steer,1);
      
     lcd.setCursor(13,3);
     lcd.print("X");
     lcd.setCursor(15,3);
     lcd.print(XTE,0);
    
     if( RUDDER_MODE == 0)  // IF THERE IS A RUDDER POSITION INDICATOR
    {
     lcd.setCursor(5,3);
     lcd.print("Rud    "); // extra spaces clear old data
     lcd.setCursor(9,3);
     lcd.print(rudder_position,0);
    }
   
    if(MSG >0) // MSG  0 is null otherwise print message here
    {
      lcd.setCursor(0,2);
     switch (MSG)
      {
       // case 0:
       // Message = "                    ";
       // break;
        
        case 1:
         lcd.print( " NO GPS, 0 to Clear"); 
        break;
        
        case 2:
         lcd.print("NO GPS Steer HTS"); 
        break;    
        
        case 3:
         lcd.print( "New WPT Press 2 Turn"); 
        break;
        
        case 4:
         lcd.print("Turning to New WPT"); 
        break;
      } // end switch   
    } // end if MSG > 0 
    /* 
     // TEMORARY
     lcd.setCursor(5,3);
     lcd.print("CMD    "); // extra spaces clear old data
     lcd.setCursor(9,3);
     lcd.print(rudder_command,0);
*/
     
  if (GPS_Available  && GPRMC_fix)
    {     
       lcd.setCursor(11, 0);
       lcd.print("BRG");
       lcd.setCursor(15, 0);
       lcd.print(Bearing_to_destination,0);     
    
    if(MSG == 0) // if MSG not zero, i. e. null, print the current message here
     {
       lcd.setCursor(0, 2);
       lcd.print("COG");
       lcd.setCursor(4, 2);
       lcd.print(course,0);      
       
       lcd.setCursor(11, 2);
       lcd.print("CTS"); 
       lcd.setCursor(15, 2);
       lcd.print(course_to_steer,1);
     }
       
     } // End if GPRMC and GPAPB true     
  
  //if(GPRMC_fix && !GPAPB_fix) {
   //  lcd.setCursor(0, 0);
    // lcd.print("No WPT"); 
    // delay(LCD_delay); 
   //}
} // END IF SCREEN = 0 

    /************* SCREEN = 1 ****************/
      if(Screen == 1)
     {
      lcd.setCursor(0,0);
      lcd.print("BRG ");
      lcd.print(Bearing_to_destination,3); 
       
      lcd.setCursor(11,0);
      lcd.print("XDE ");
      lcd.print(XTE_differential_error,2);
      
      lcd.setCursor(0,1);
      lcd.print("XTE ");
      lcd.print(XTE,1);

      lcd.setCursor(11,1);
      lcd.print("Xint");
      lcd.print(XTE_integral_error,0);      
      
      lcd.setCursor(0,2);
      lcd.print("BOD ");
      lcd.print(Bearing_origin_to_destination,1);
      
      lcd.setCursor(11,2);
      lcd.print("BRT ");
      lcd.print(bearingrate);
     
      lcd.setCursor(0,3);
      lcd.print("XCR ");
      lcd.print(XTE_course_correction); 
      
     /* lcd.setCursor(11,3);
      lcd.print("Rud      "); // extra spaces clear old data
      lcd.setCursor(15,3);
      lcd.print(rudder_position,0);
    */
    
      lcd.setCursor(11,3);
     lcd.print("CMD    "); // extra spaces clear old data
     lcd.setCursor(15,3);
     lcd.print(rudder_command,0);
      
      /* 
      lcd.setCursor(0,1);
      lcd.print("DXTE ");
      lcd.print(XTE_differential_error); 
      */ 
     }// end screen 1
     
     
     
    /************* SCREEN = 2 ****************/
    
   
     
     if(Screen == 2)
     {
      
      lcd.setCursor(0, 0);
     //lcd.print(GPS_status); //no gps, no waypoint, or waypoint 
      lcd.print("Lt "); lcd.print(Lat_current);
      lcd.setCursor(10,0);
      lcd.print("Ln "); lcd.print(Lon_current);
   //  This is a diagnostic it it prints seconds if GPS is processing
      lcd.setCursor(0,1);
      lcd.print("UTC ");
      lcd.print(UTC);
      /*
      lcd.setCursor(0,2);
      lcd.print("UTC Start ");
      lcd.print(UTC_start);
      
      lcd.setCursor(0,3);
      lcd.print("Max DT ");
      lcd.print(Time_decimal_MAX_seconds,0);
      */      
      lcd.setCursor(0,3);
      lcd.print("MAGV ");
      lcd.print(Magnetic_Variation,1);
      
      lcd.setCursor(0,2); 
      lcd.print("Rudder SPD "); 
    //  lcd.setCursor(15,0);
      lcd.print(motorspeed); 
      
     } // End if screen = 2
 
   if(Screen == 3)
   {
     lcd.setCursor(0,0);
     if(Next_Turn >0) lcd.print("Next Turn R ");
      else lcd.print("Next Turn L ");
      lcd.print(abs(Next_Turn), 1); 
      
     lcd.setCursor(0,1);
     lcd.print("Dist To Wpt = ");
     lcd.print(Range_Destination,2);
     
   //  lcd.setCursor(0,3);
  //   lcd.print("Press 3 to Make Turn");
      
      
   }  // End screen = 3
   
     if(Screen == 4)
   {
     
     lcd.setCursor(0,0);
     lcd.print("PAST WAYPOINT PRESS 3");
     lcd.setCursor(0,1);
     if(Next_Turn >0) lcd.print("Turn Right ");
      else lcd.print("Turn Left ");
      lcd.print(abs(Next_Turn), 1); 
  
     lcd.setCursor(0,3);
     lcd.print(" then Press 3 for GPS");
      
      
   }  // End screen = 4
 
}  // END Void LCD()

/****************************************************/
    void NAV_DATA_PRINT()
    {
      
     // Serial.println(" UTC, waypoint, BRG, RNG, BOD, COG, XTE, XTE_Corr, CTS,HDG,  HTS, CMD, RUD);
      Serial.print(UTC_string); Serial.print(", ");
      Serial.print(Waypoint_next); Serial.print(", ");
      Serial.print(Bearing_to_destination); Serial.print(", ");
      Serial.print(Range_Destination,3); Serial.print(", ");
      Serial.print(Bearing_origin_to_destination ); Serial.print(", ");
      Serial.print(course ); Serial.print(", ");
      Serial.print(XTE); Serial.print(", ");
      Serial.print(XTE_course_correction ); Serial.print(", ");
      Serial.print(course_to_steer); Serial.print(", ");
      Serial.print(heading); Serial.print(", ");
      Serial.print(heading_to_steer); Serial.print(", ");
      Serial.print(bearingrate); Serial.print(", ");
      Serial.print(rudder_command ); Serial.print(", ");
      Serial.print(rudder_position);Serial.print(", ");
      Serial.println();
    }  // end NAV_DATA_PRINT
    
 /***********************************************************************************/   


