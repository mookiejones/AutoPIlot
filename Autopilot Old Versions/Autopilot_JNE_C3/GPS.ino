/***********************************************************************
GPS data is processed on separate Arduino UNO and GPS2 receives the data.
This saves processing time.  The MinIMU9 could not process data fast enough with
the time out it took the GPS to get data. With separate processing data is transfered
using Easy Transfer by Bill Porter in one millisecond.

**********************************************************************/

void get_GPS_data()
{
  //check and see if a data packet has come in. 
   if (Print_ETtime){
    Serial.print("start ET time  ");
    Serial.println(millis());
   }
     //{
      // assigns received data to my variable names
       course = ETdata.SD_course;
     //  course_to_steer = ETdata.SD_course_to_steer;
      // CTS_MorT = ETdata.SD_CTS_MorT;
       SOG = ETdata.SD_SOG;
       MagVar = ETdata.SD_MagVar;
       XTE = ETdata.SD_XTE; // XTE positive means right of track, steer left
       XTE_differential_error = ETdata.SD_XTE_differential_error;
     //  XTE_LR = ETdata.SD_XTE_LR;
     //  XTE_unit = ETdata.SD_XTE_unit;
      // if(XTE_LR == "L") XTE = -XTE; 
      // if(XTE_unit == "N") XTE = 6000 * XTE; 
      // if(XTE_unit == "F") XTE = XTE;
       Waypoint_next = ETdata.SD_Waypoint_next;
      // Waypoint_next.trim();
     //  NEMA_sentence = ETdata.SD_NEMA_sentence;
       GPRMC_fix = ETdata.SD_GPRMC_fix;
       GPAPB_fix = ETdata.SD_GPAPB_fix;
    //   Origin_Waypoint = ETdata.SD_Origin_Waypoint;
       Bearing_origin_to_destination = ETdata.SD_Bearing_origin_to_destination;
       BOD_MorT = ETdata.SD_BOD_MorT;
       Bearing_to_destination =  ETdata.SD_Bearing_to_destination;
       Range_Destination = ETdata.SD_Range_Destination;;
     //  UTC_string = ETdata.SD_UTC; 
      // UTC_string = UTC_string.substring(0,6);
      UTC = ETdata.SD_UTC;
     //  Date = ETdata.SD_Date; 
     //  Lat_current = ETdata.SD_Lat_current;
    //   Lon_current = ETdata.SD_Lon_current;
      Next_Turn = ETdata.SD_NEXT_TURN;
      // Time_decimal = ETdata.SD_Time_decimal;
 //  } //end if ET.receiveData
   
     if (!GPRMC_fix) GPS_status = "LOST GPS";
     if (GPRMC_fix) GPS_status = "GPS OK";
     //UTC_TIMER();
    
     if (Print_ETdata) PrintETdata();    
   // } //end if ET.receiveData
      if (Print_ETtime)
     {
         Serial.print("end ET time = ");
         Serial.println(millis());
         Serial.println("----------------");
     }
  // } // end if timer > 1000  
}  // end void get_GPS_data()

void PrintETdata()
{
  Serial.println();
  Serial.print("UTC  ");  Serial.println(UTC_string);
  Serial.print("Date  ");  Serial.println(Date);
  Serial.print("course = ");  Serial.println (course);
 // Serial.print("course to steer = ");  Serial.println(course_to_steer);
 // Serial.print("CTS mag or True = ");
  Serial.println(CTS_MorT);
  Serial.print("Speed = ");  Serial.println(SOG);
  // speed has not been tested while moving
  Serial.print("Mag Variation = ");  Serial.println(MagVar);
  Serial.print("XTE (in Feet) = "); Serial.println(XTE);
  Serial.print("differential cross track error: ");
  Serial.println(XTE_differential_error);
  //Serial.print("XTE L/R = "); Serial.println(XTE_LR);
  //Serial.print("XTE Units = "); Serial.println(XTE_unit);
  Serial.print("Next Waypoint = ");  Serial.println(Waypoint_next);
  Serial.print("Bearing Origin to Destination = ");  Serial.println(Bearing_origin_to_destination);
  Serial.print("BOD mag or True = "); Serial.println(BOD_MorT);
 // Serial.print("NEMA Sentence = "); Serial.println(NEMA_sentence);
  Serial.print("GPRMC fix status = "); Serial.println(GPRMC_fix);
  Serial.print("GPAPB fix status = "); Serial.println(GPAPB_fix);
  Serial.print("Range to Destination = ");  Serial.println(Range_Destination);
  Serial.print("Bearing to Destination "); Serial.println(Bearing_to_destination);
 // Serial.print("Latitude current = "); Serial.println(Lat_current);
//  Serial.print("Longitude current = "); Serial.println(Lon_current);
}

/************************ UTC TIMER ***************/
/*
    void UTC_TIMER()
    //sets waypoint to NO GPS if no update for more than x seconds
    {
      int max_no_UTC = 4;
     float Time_decimal_start;
     
      //  UTC_delta = UTC - UTC_old;
        Date_Time();  // gets UTC converted to decimal
        Time_decimal_delta = Time_decimal - Time_decimal_old;
        if (millis() < 20000)   
        { 
           UTC_start = UTC;     // captures UTC_start after 20 sec
           Time_decimal_start = Time_decimal;
        }
        
         if (millis() > 60000)  // after the first minute
           {
             //if(UTC_delta > UTC_MAX) UTC_MAX = UTC_delta; 
          //   UTC_MAX = max(UTC_MAX , UTC_delta);  // captures maximum UTC Delta between new and old UTC
             Time_decimal_MAX = max(Time_decimal_MAX, Time_decimal_delta);
             Time_decimal_MAX_seconds = Time_decimal_MAX * 3600 * 24;
         }
        
       // if(UTC > UTC_old)
           if (Time_decimal > Time_decimal_old)
        {
          
          // UTC_old = UTC;
           Time_decimal_old = Time_decimal;
           UTC_timer_old = millis()/1000;
        }
        UTC_timer = millis()/1000 - UTC_timer_old;
        //if (UTC == UTC_old)  UTC_timer = millis()/1000 - UTC_timer_old;
        
        if( UTC_timer > max_no_UTC) Waypoint_next = "NO GPS";
      
        Serial.print("UTC: ");
        Serial.println(UTC);
        Serial.print ("UTC Timer: ");
        Serial.println(UTC_timer);
        Serial.print("UTC timer old ");
        Serial.println(UTC_timer_old);
       
    } // end UTC_TIMER
   */ 
    /************************* DATE TIME CALCULATOR ****************************/
    
    void Date_Time()
    {
      char char1[7];
      String year;
      String month;
      String day;
      String hour;
      String minute;
      String second;
      float fsecond, fminute, fhour, fday;
      
      string1 = String(Date);
      day = string1.substring(0,2);
      month = string1.substring(2,4);
      year = string1.substring(4,6);
      
      string1 = UTC_string;
      hour = string1.substring(0,2);
      minute = string1.substring(2,4);
      second = string1.substring(4,6);
    
      strtoflo(second);
      fsecond = float1;
     // Serial.print("sec float: "); Serial.println(fsecond);
      
       strtoflo(minute);
      fminute = float1;
     // Serial.print("min float: "); Serial.println(fminute);
      
      strtoflo(hour);
      fhour = float1;
     // Serial.print("hour float: "); Serial.println(fhour);
     
       strtoflo(day);
      fday = float1;
     //Serial.print("Day float: "); Serial.println(fday);
     
     Time_decimal = fday +(fhour + fminute/60 + fsecond / 3600)/24;
    
      /*
      Serial.print("year: "); Serial.println(year);
      Serial.print("month: "); Serial.println(month);
      Serial.print("day: "); Serial.println(day);
      Serial.print("hour: "); Serial.println(hour);
      Serial.print("minute: "); Serial.println(minute);
      Serial.print("second: "); Serial.println(second);
      Serial.print("Decimal time: "); Serial.println(Time_decimal,4);
      Serial.println();
      */
    }  // End void data time
    
    void strtoflo(String string1)
    {
      char char_buf[16];
      string1.toCharArray(char_buf,16);
      long1 = strtol(char_buf,&pEnd,10);
      float1 = float(long1); 
    } // end String_to_float
