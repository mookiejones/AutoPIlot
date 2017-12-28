  void Recieve_ET2Data()
{ 
    course = ET2data.course;
    SOG = ET2data.SOG;
    MagVar = ET2data.SD_MagVar;
    XTE = ET2data.XTE;
    Waypoint_next = ET2data.Waypoint_next;
    Bearing_origin_to_destination = ET2data.Bearing_origin_to_destination;
    Bearing_to_destination = ET2data.Bearing_to_destination;
    Range_Destination = ET2data.Range_Destination; 
  //  UTC = ET2data.UTC;
    Next_Turn = ET2data.Next_Turn;
    heading = ET2data.heading;
    heading_to_steer = ET2data.heading_to_steer;
    course_to_steer = ET2data.course_to_steer;
    bearingrate = ET2data.bearingrate;
    rudder_position =  ET2data.rudder_position;
    Mode = ET2data.Mode;
    MSG = ET2data.MSG;
  
     if (!GPRMC_fix) GPS_status = "LOST GPS";
     if (GPRMC_fix) GPS_status = "GPS OK";
     //UTC_TIMER();
     
     KEYPAD();  // this call to keypad is to speed up keypad responsivenss because TFT print is so slow
     MESSAGES();
     Print_Screen();
     
     if (Print_ET2data) PrintET2data();    

      if (Print_ETtime)
     {
         Serial.print("end ET time = ");
         Serial.println(millis());
         Serial.println("----------------");
     }
  
}  // end Recieve_ET2Data()
/************************************************************/
void MESSAGES()
{
  switch (MSG)
  {
  case 0:
  Message = "              ";
  break;
  
  case 1:
   Message = " NO GPS,      0 to Clear"; 
  break;
  
  case 2:
   Message = "NO GPS       Steering HTS"; 
  break;
  
  case 3:
   Message = "New WPT Press 2 to Turn"; 
  break;
  
  case 4:
   Message = "Turning to   New Waypoint"; 
  break;
  
  } // end switch
} // end MESSAGES

/*************************************************************/
void PrintET2data()
{
  Serial.println();
  Serial.print("UTC  ");  Serial.println(UTC);
  Serial.print("course = ");  Serial.println (course);
  Serial.print("Speed = ");  Serial.println(SOG);  // speed has not been tested while moving
  Serial.print("Mag Variation = ");  Serial.println(MagVar);
  Serial.print("XTE (in Feet) = "); Serial.println(XTE);
  Serial.print("Next Waypoint = ");  Serial.println(Waypoint_next);
  Serial.print("Bearing Origin to Destination = ");  Serial.println(Bearing_origin_to_destination);
  Serial.print("Range to Destination = ");  Serial.println(Range_Destination);
  Serial.print("Next_Turn "); Serial.println(Next_Turn,1);
  Serial.print("heading "); Serial.println(heading,0);
  Serial.print("HTS "); Serial.println(heading_to_steer,0);
  Serial.print("CTS "); Serial.println(course_to_steer,0);
  Serial.print("Bearing Rate "); Serial.println(bearingrate);
  Serial.print("Rudder Position "); Serial.println(rudder_position);
  Serial.print("Mode "); Serial.println(Mode);
  
}
