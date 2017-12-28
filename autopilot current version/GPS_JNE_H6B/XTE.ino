

/***************** CROSS TRACK ERROR CALC *********************/

void Get_Cross_Track_Error()
{
     float angle1;
     float angle2;  // angle between BOD an course 
    
     Destination_Bearing ();// Note comment this out to use Bearing_to_Destination from GPRMB (less accurate, more robust)
     angle1 = Bearing_origin_to_destination - Bearing_to_destination; 
     if(angle1 > 180) angle1 = 360 - angle1; // keep angle < 180 with clockwise as positive
     if(angle1 < -180) angle1 = 360 + angle1;
     XTE = Range_Destination*6000 * sin(PI*angle1/180.0);// XTE in feet, if XTE positive right of track steer left
     ETdata.SD_XTE = XTE;
     angle2 = course - Bearing_origin_to_destination; // positive if Dxte is positive
     if(angle2 > 180) angle2 = 360 - angle2; // keep angle < 180 with clockwise as positive
     if(angle2 < -180) angle2 = 360 + angle2;
     XTE_differential_error = SOG* sin(PI*angle2/180.0); // cross track velocity in knots
     ETdata.SD_XTE_differential_error = XTE_differential_error;
     
 
     if(print_Nav_Data)
     {
      Serial.println();
      Serial.print("Waypoint Next ");
      Serial.println(Waypoint_next);
      Serial.print("BOD: ");
      Serial.println(Bearing_origin_to_destination);
      Serial.println("WPT Lat Lon followed by current Lat Lon ");
      Serial.print(Current_Waypoint_Lat,5); Serial.print("  "); Serial.println(Current_Waypoint_Lon,5);
      Serial.print(Lat_current,5); Serial.print("  "); Serial.println(Lon_current,5);
      Serial.print("BRG and BRG by LAT Lon ");
      Serial.print(Bearing_to_destination,1);Serial.print("  "); Serial.println(Bearing_to_destination_by_LatLon,3);
      Serial.print("Range: ");
      Serial.println(Range_Destination,3);    
      Serial.print("XTE: ");
      Serial.println( XTE,1);  
      Serial.print ("XTE rate ");
      Serial.println(XTE_differential_error,1);
     }  // end print_Nav_Data
    
}// End Get cross track error

/***************  DESTINATION BEARING  *******************************/
  void Destination_Bearing ()
  {
      float Dx1, Dy1;
      float bearing;
      
      // First get current waypoint lat and lon
      for (int i = 0; i< Number_of_waypoints; i++) 
     {
         if (data_WPL[5] == Waypoint_next)
        {
         Current_Waypoint_Lat = Waypoint_Lat[i];
         Current_Waypoint_Lon = Waypoint_Lon[i];
         break;
        }
     }
      
      Dx1 = cos(Lat_current*PI/180)*360000*(Current_Waypoint_Lon - Lon_current);
      Dy1 = 360000*(Current_Waypoint_Lat - Lat_current);
      bearing = (180/PI) * atan2(Dx1 , Dy1); // note 180/PI*atan2 returns -180 to +180 accounts for sign Dx and Dy
      if(bearing < 0 ) bearing = 360 + bearing;
      Bearing_to_destination_by_LatLon = bearing;
      
      //ETdata.SD_Bearing_to_destination = Bearing_to_destination; // same as course to steer

  }  // End Destination Bearing
  /*********************************************************************/

